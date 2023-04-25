/*
 * Title:			Space Critters for Agon Light
 * Author:			James Higgs (Jum Hig)
 * Created:			2023
 * Last Updated:		2023-04-21
 *
 * Revisions:
 * 2023-04-21 - Updated to use MOS 1.03
 */
 
// Ideas:
// 1. Big bird splits into 2 when shot
// 2. Dead birds drop from the sky, can kill you
// 3. If you clip a bird's wing, it drops down with erratic side movement

#include <stdio.h>
#include <ctype.h>
#include <stdlib.h>
#include <gpio.h>				// For joystick
#include "mos-interface.h"
#include "vdp.h"

#include "sprites/ufos.h"			// for bullets
#include "sprites/playerShip.h"
#include "sprites/explosions.h"
#include "sprites/bird_16x12.h"
#include "sprites/bird_32x24.h" 

#define SCREEN_WIDTH	320
#define SCREEN_HEIGHT	200

#define MAX_SPRITES		32
#define BITMAP_WIDTH		16
#define BITMAP_HEIGHT	16	

// Bitmaps ids
enum {
	BID_PLAYER0 = 0,			// 0 to 7 - Player and player bullet bitmap id's
	BID_PLAYER1,
	BID_PLAYER2,
	BID_PLAYER3,
	BID_UNUSED1,
	BID_UNUSED2,
	BID_UNUSED2,
	BID_PLAYERBULLET,
	
	BID_EXPLOSION0,			// 8 to 15 - Explosions id's
	BID_EXPLOSION1,
	BID_EXPLOSION2,
	BID_EXPLOSION3,
	BID_EXPLOSION4,
	BID_EXPLOSION5,
	BID_EXPLOSION6,
	BID_EXPLOSION7,
	
	BID_SBIRD0,				// 16 to 23 - Small bird id's			
	BID_SBIRD1,				
	BID_SBIRD2,				
	BID_SBIRD3,				
	BID_SBIRD4,				
	BID_SBIRD5,				
	BID_SBIRD6,				
	BID_ENEMYBULLET,				

	BID_BBIRD0,				// 24 to 31 - Big bird id's			
	BID_BBIRD1,				
	BID_BBIRD2,				
	BID_BBIRD3,				
	BID_BBIRD4,				
	BID_BBIRD5,				
	BID_BBIRD6			
	
};

// Game object
typedef struct
{
	UINT8 type;
	UINT16 x;
	UINT16 y;
	UINT8 imageId;				// "base" image id
	UINT8 frame;					// Current frame of animation
	UINT8 state;					// Current state of this object
	UINT16 counter;				// Internal counter for object logic
} OBJECT;

OBJECT objects[MAX_SPRITES] = {
	{ 0, 120, 50, BID_PLAYER0, 0, 0, 0 },
	{ 0, 0, 0, 0, 0, 0, 0 },
	{ 0, 0, 0, 0, 0, 0, 0 },
	{ 0, 0, 0, 0, 0, 0, 0 },
	{ 0, 0, 0, 0, 0, 0, 0 },
	{ 0, 0, 0, 0, 0, 0, 0 },
	{ 0, 0, 0, 0, 0, 0, 0 },
	{ 0, 0, 0, 0, 0, 0, 0 },
	{ 0, 0, 0, 0, 0, 0, 0 },
	{ 0, 0, 0, 0, 0, 0, 0 },
	{ 0, 0, 0, 0, 0, 0, 0 },
	{ 0, 0, 0, 0, 0, 0, 0 },
	{ 0, 0, 0, 0, 0, 0, 0 },
	{ 0, 0, 0, 0, 0, 0, 0 },
	{ 0, 0, 0, 0, 0, 0, 0 },
	{ 0, 0, 0, 0, 0, 0, 0 },
	{ 0, 0, 0, 0, 0, 0, 0 },
	{ 0, 0, 0, 0, 0, 0, 0 },
	{ 0, 0, 0, 0, 0, 0, 0 },
	{ 0, 0, 0, 0, 0, 0, 0 },
	{ 0, 0, 0, 0, 0, 0, 0 },
	{ 0, 0, 0, 0, 0, 0, 0 },
	{ 0, 0, 0, 0, 0, 0, 0 },
	{ 0, 0, 0, 0, 0, 0, 0 },
	{ 0, 0, 0, 0, 0, 0, 0 },
	{ 0, 0, 0, 0, 0, 0, 0 },
	{ 0, 0, 0, 0, 0, 0, 0 },
	{ 0, 0, 0, 0, 0, 0, 0 },
	{ 0, 0, 0, 0, 0, 0, 0 },
	{ 0, 0, 0, 0, 0, 0, 0 },
	{ 0, 0, 0, 0, 0, 0, 0 },
	{ 0, 0, 0, 0, 0, 0, 0 }
};
	

UINT8 rand255()
{
	return (rand() & 0xFF);
}

void UploadBitmaps()
{
	// Player and player bullet uses bitmaps 0 to 7
	vdp_bitmapSendData(BID_PLAYER0, PLAYER_WIDTH, PLAYER_HEIGHT, playerData[0]);
	vdp_bitmapSendData(BID_PLAYER1, PLAYER_WIDTH, PLAYER_HEIGHT, playerData[1]);		// player exploding 1
	vdp_bitmapSendData(BID_PLAYER2, PLAYER_WIDTH, PLAYER_HEIGHT, playerData[2]);		// player exploding 2
	vdp_bitmapSendData(BID_PLAYER3, PLAYER_WIDTH, PLAYER_HEIGHT, playerData[3]);		// player exploding 3
	vdp_bitmapSendData(BID_PLAYERBULLET, 16, 16, playerBulletData);	// player bullet
	
	// Explosions use bitmaps 8 to 15
	vdp_bitmapSendData(BID_EXPLOSION0, EXPLOSION_WIDTH, EXPLOSION_HEIGHT, explosionData[0]);
	vdp_bitmapSendData(BID_EXPLOSION1, EXPLOSION_WIDTH, EXPLOSION_HEIGHT, explosionData[1]);
	vdp_bitmapSendData(BID_EXPLOSION2, EXPLOSION_WIDTH, EXPLOSION_HEIGHT, explosionData[2]);
	vdp_bitmapSendData(BID_EXPLOSION3, EXPLOSION_WIDTH, EXPLOSION_HEIGHT, explosionData[3]);
	vdp_bitmapSendData(BID_EXPLOSION4, EXPLOSION_WIDTH, EXPLOSION_HEIGHT, explosionData[4]);
	vdp_bitmapSendData(BID_EXPLOSION5, EXPLOSION_WIDTH, EXPLOSION_HEIGHT, explosionData[5]);
	vdp_bitmapSendData(BID_EXPLOSION6, EXPLOSION_WIDTH, EXPLOSION_HEIGHT, explosionData[6]);
	vdp_bitmapSendData(BID_EXPLOSION7, EXPLOSION_WIDTH, EXPLOSION_HEIGHT, explosionData[7]);

	// Small enemy birds use bitmaps 16 to 23
	vdp_bitmapSendData(BID_SBIRD0, SBIRD_WIDTH, SBIRD_HEIGHT, sbirdData[0]);
	vdp_bitmapSendData(BID_SBIRD1, SBIRD_WIDTH, SBIRD_HEIGHT, sbirdData[1]);
	vdp_bitmapSendData(BID_SBIRD2, SBIRD_WIDTH, SBIRD_HEIGHT, sbirdData[2]);
	vdp_bitmapSendData(BID_SBIRD3, SBIRD_WIDTH, SBIRD_HEIGHT, sbirdData[3]);
	vdp_bitmapSendData(BID_SBIRD4, SBIRD_WIDTH, SBIRD_HEIGHT, sbirdData[4]);
	vdp_bitmapSendData(BID_SBIRD5, SBIRD_WIDTH, SBIRD_HEIGHT, sbirdData[5]);
	vdp_bitmapSendData(BID_SBIRD6, SBIRD_WIDTH, SBIRD_HEIGHT, sbirdData[6]);
	vdp_bitmapSendData(BID_ENEMYBULLET, 16, 16, enemyBulletData);
	
	// Big enemy birds use bitmaps 24 to 31
	vdp_bitmapSendData(BID_BBIRD0, BBIRD_WIDTH, BBIRD_HEIGHT, bbirdData[0]);
	vdp_bitmapSendData(BID_BBIRD1, BBIRD_WIDTH, BBIRD_HEIGHT, bbirdData[1]);
	vdp_bitmapSendData(BID_BBIRD2, BBIRD_WIDTH, BBIRD_HEIGHT, bbirdData[2]);
	vdp_bitmapSendData(BID_BBIRD3, BBIRD_WIDTH, BBIRD_HEIGHT, bbirdData[3]);
	vdp_bitmapSendData(BID_BBIRD4, BBIRD_WIDTH, BBIRD_HEIGHT, bbirdData[4]);
	vdp_bitmapSendData(BID_BBIRD5, BBIRD_WIDTH, BBIRD_HEIGHT, bbirdData[5]);
	vdp_bitmapSendData(BID_BBIRD6, BBIRD_WIDTH, BBIRD_HEIGHT, bbirdData[6]);

}

// Setup the sprites according to the level
void SetupSprites(UINT8 level)
{
	UINT8 i;
	for (i = 1; i < MAX_SPRITES; i++)
		{
		vdp_spriteHide(i);
		}
		
	for (i = 1; i < 10; i++)
		{
		objects[i].x = rand255();
		objects[i].y = i * 20 - 20;
		vdp_spriteClearFrames(i);
/* 		vdp_spriteAddFrame(i, BID_SBIRD0);
		vdp_spriteAddFrame(i, BID_SBIRD1);
		vdp_spriteAddFrame(i, BID_SBIRD2);
		vdp_spriteAddFrame(i, BID_SBIRD3);
		vdp_spriteAddFrame(i, BID_SBIRD4);
		vdp_spriteAddFrame(i, BID_SBIRD5);
		vdp_spriteAddFrame(i, BID_SBIRD6); */
		vdp_spriteAddFrame(i, BID_BBIRD0);
		vdp_spriteAddFrame(i, BID_BBIRD1);
		vdp_spriteAddFrame(i, BID_BBIRD2);
		vdp_spriteAddFrame(i, BID_BBIRD3);
		vdp_spriteAddFrame(i, BID_BBIRD4);
		vdp_spriteAddFrame(i, BID_BBIRD5);
		vdp_spriteAddFrame(i, BID_BBIRD6); 
		vdp_spriteShow(i);
		}
}

/// @param[in] argc			Argument count
/// @param[in] argv			Pointer to the argument string - zero terminated, parameters separated by spaces
int main(int argc, char * argv[]) {
	UINT16 i, j;
	UINT8 keycode, stick, flags;
	UINT16 t;
	
	vdp_mode(2);
//	vdp_cursorDisable();
	vdp_cls();

	// Set GPIO port C as input (mode 2)
//	printf("Setting up GPIO port C for joystick...\n\r");
//	setmode_PortC(0xFF, GPIOMODE_INPUT) ;
	
/* 	printf("Hello World\n\r");
	printf("Arguments:\n\r");
	printf("- argc: %d\n\r", argc);
	
	for(i = 0; i < argc; i++) {
		printf("- argv[%d]: %s\n\r", i, argv[i]);
	} */
	
	// Upload sprite data to the VDP
	// Bitmaps 0 to 8 are for player + player bullet
	printf("Uploading bitmaps...\n\r");
	UploadBitmaps();
	
	vdp_bitmapDraw(0, 200, 100);

	// Set up sprites
	// Note: BBCBASIC procedure:
	//    1. Select sprite n   (VDU 23, 27, 4, n)
	//    2. Clear frames for current sprite  (VDU 23, 27, 5)
	//    3. Add bitmap 0 as frame 0 of current sprite (VDU 23, 27, 6, 0)
	//    4. Show the current sprite (VDU 23, 27, 11)
	//    5. Set total number of sprites to display (VDU 23, 27, 7, total)
	//   Move the sprites:
	//    1. Select sprite n (VDU 23, 27, 4, n)
	//    2. Move selected sprite to  (VDU 23, 27, 13, X, Y) 
	//   Refresh the sprites
	//    1. *FX19    (wait for VSYNC)
	//    2. Refresh (VDU 23, 27, 15)
	
/* 	vdp_spriteSelect(0);
	vdp_spriteClearFramesSelected();
	vdp_spriteAddFrameSelected(0);
	vdp_spriteShowSelected();
	vdp_spriteActivateTotal(1);
	
	vdp_spriteSelect(0);
	vdp_spriteMoveToSelected(100, 100); */
	
	
	



/*
	vdp_spriteClearFrames(0);
	vdp_spriteClearFrames(1);
	vdp_spriteClearFrames(2);
	vdp_spriteClearFrames(3);
	vdp_spriteAddFrame(0, 0);			// player
	vdp_spriteAddFrame(1, 1);			// player bullet
	vdp_spriteAddFrame(2, 2);			// ufo frame 0
	vdp_spriteAddFrame(2, 3);			// ufo frame 1
	vdp_spriteAddFrame(2, 4);			// ufo frame 2
	vdp_spriteAddFrame(3, 5);			// ufo bullet

	vdp_spriteMoveTo(0, 100, 100);
	vdp_spriteMoveTo(2, 140, 140);

	vdp_spriteSelect(0);
	vdp_spriteSetFrameSelected(0);
	vdp_spriteMoveToSelected(100, 100);
	
	vdp_spriteActivateTotal(3);
*/

	printf("Setting up sprites...\n\r");
	SetupSprites(0);

//	getch();
	
	vdp_spriteClearFrames(0);
	vdp_spriteAddFrame(0, BID_PLAYER0);
	vdp_spriteShow(0);
	vdp_spriteMoveTo(0, SCREEN_WIDTH / 2, SCREEN_HEIGHT - 20);
	

	vdp_spriteActivateTotal(10);
	vdp_spriteRefresh();
//	getch();
		
	for (i = 0; i < 500; i++)
		{
		waitvblank();
		
		
		for (j = 1; j < 10; j++)
			{
			if (0 == objects[j].state)
				{
				objects[j].x++;
				if (objects[j].x == 319)
					objects[j].state = 1;
				}
			else if (1 == objects[j].state)
				{
				objects[j].x--;
				if (objects[j].x == 0)
					objects[j].state = 0;
				}
				
			vdp_spriteMoveTo(j, objects[j].x, objects[j].y);
			//vdp_spriteSetFrame(j, (UINT8)(objects[j].x / 16) % 7);
			if (0 == (i & 0x7))
				vdp_spriteNextFrame(j);
			}
			
		//vdp_scroll(0, 1, 1);
		vdp_plotColour(rand255());
		vdp_plotPoint(rand255() * 4, rand255() * 4);
		//vdp_spriteRefresh();

			
		//keycode = getsysvar8bit(sysvar_keycode);
		//keycode = getkeycode();
		keycode = getsysvar_keyascii();
		//printf("%d\n\r", keycode);
		if (27 == keycode)
			break;

/*		
		if (8 == keycode)
			vdp_spriteMoveBy(0, -2, 0);
		else if (21 == keycode)
			vdp_spriteMoveBy(0, 2, 0);
		
		if (32 == keycode)
			{
			// Fire	
			}
*/			

		GETDR_PORTC(stick);
		//printf("%d- ", stick);
		//stick = 255;
		if (0 == (stick & 8))
			vdp_spriteMoveBy(0, -2, 0);
		else if (0 == (stick & 16))
			vdp_spriteMoveBy(0, 2, 0);
/*		
		//flags = (UINT8)getsysvar16bit(sysvar_time);	//sysvar_vpd_pflags);
		//t = getsysvar16bit(sysvar_time);
		//printf("%d\n\r", t);
		//flags = getsysvar8bit(sysvar_time);
		//printf("%d\n\r", flags);
*/
		}
	
//	vdp_cursorEnable();
	
//	vdp_mode(1);
 
	return 0;
}
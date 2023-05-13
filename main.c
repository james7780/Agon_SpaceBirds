/*
 * Title:			Space Critters for Agon Light
 * Author:			James Higgs (Jum Hig)
 * Created:			2023
 * Last Updated:		2023-04-21
 *
 * Revisions:
 * 2023-04-21 - Updated to use MOS 1.03
 */
 
// VDP functions needed:
// - Read state of any key on the keyboard
// - Query sprite collisions (from fabGL)
// - Better sound functions! (Different waveforms, noise, etc)

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
#include "vdp_audio.h"

#include "sprites/ufos.h"			// for bullets
#include "sprites/playerShip.h"
#include "sprites/explosions.h"
#include "sprites/bird_16x12.h"
#include "sprites/bird_32x24_shaded.h"
#include "sprites/eggs.h" 

#define SCREEN_WIDTH	320
#define SCREEN_HEIGHT	200

#define MAX_SPRITES		32
#define BITMAP_WIDTH		16
#define BITMAP_HEIGHT	16	

#define FIRSTENEMYID	2				// index of the first enemy in the object array
#define LASTENEMYID	9				// index of the last enemy in the object array

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

	BID_EGGS0,				// 16 to 22 - Eggs id's			
	BID_EGGS1,
	BID_EGGS2,
	BID_EGGS3,
	BID_EGGS4,
	BID_EGGS5,
	BID_EGGS6,
	
	BID_SBIRD0,				// 23 to 30 - Small bird id's			
	BID_SBIRD1,				
	BID_SBIRD2,				
	BID_SBIRD3,				
	BID_SBIRD4,				
	BID_SBIRD5,				
	BID_SBIRD6,				
	BID_ENEMYBULLET,				

	BID_BBIRD0,				// 31 to 37 - Big bird id's			
	BID_BBIRD1,				
	BID_BBIRD2,				
	BID_BBIRD3,				
	BID_BBIRD4,				
	BID_BBIRD5,				
	BID_BBIRD6
	
};

// Object types (type index)
enum {
	PLAYER = 0,
	PLAYERBULLET,
	BIRD,
	BIGBIRD
};

// Object states
enum {
	ST_INACTIVE = 0,
	ST_ACTIVE,
	ST_EGG,
	ST_MOVELEFT,
	ST_MOVERIGHT,
	ST_MOVERANDOM,
	ST_DYING 
};

// Direction bitmask
enum {
	DIRN_NONE = 0,
	DIRN_LEFT = 1,
	DIRN_RIGHT = 2,
	DIRN_UP = 4,
	DIRN_DOWN = 8
};

// Game object
typedef struct
{
	UINT8 type;
	UINT16 x;
	UINT16 y;
	//UINT8 imageId;				// "base" image id
	UINT8 frame;				// Current frame of animation
	UINT8 state;				// Current state of this object
	UINT8 dirn;					// Current direction of the object
	UINT16 counter;				// Internal counter for object logic
} OBJECT;

OBJECT objects[MAX_SPRITES] = {
	{ PLAYER, 120, 50, 0, 0, 0, 0 },
	{ PLAYERBULLET, 0, 0, 0, 0, 0, 0 },
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

	// Eggs use bitmaps 16 to 22
	vdp_bitmapSendData(BID_EGGS0, EGGS_WIDTH, EGGS_HEIGHT, eggsData[0]);
	vdp_bitmapSendData(BID_EGGS1, EGGS_WIDTH, EGGS_HEIGHT, eggsData[1]);
	vdp_bitmapSendData(BID_EGGS2, EGGS_WIDTH, EGGS_HEIGHT, eggsData[2]);
	vdp_bitmapSendData(BID_EGGS3, EGGS_WIDTH, EGGS_HEIGHT, eggsData[3]);
	vdp_bitmapSendData(BID_EGGS4, EGGS_WIDTH, EGGS_HEIGHT, eggsData[4]);
	vdp_bitmapSendData(BID_EGGS5, EGGS_WIDTH, EGGS_HEIGHT, eggsData[5]);
	vdp_bitmapSendData(BID_EGGS6, EGGS_WIDTH, EGGS_HEIGHT, eggsData[6]);

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
		
	// Set up player sprite
	vdp_spriteSelect(0);		
	vdp_spriteClearFramesSelected();
	vdp_spriteAddFrameSelected(BID_PLAYER0);
	vdp_spriteAddFrameSelected(BID_PLAYER1);
	vdp_spriteAddFrameSelected(BID_PLAYER2);
	vdp_spriteAddFrameSelected(BID_PLAYER3);
	vdp_spriteShowSelected();
	vdp_spriteMoveToSelected(SCREEN_WIDTH / 2, SCREEN_HEIGHT - 20);
	objects[PLAYER].x = SCREEN_WIDTH / 2;
	objects[PLAYER].y = SCREEN_HEIGHT - 30;		// Leave gap below so player can see enemies below
	objects[PLAYER].state = ST_ACTIVE;		

	// Set up bullet sprite
	vdp_spriteSelect(1);
	vdp_spriteAddFrameSelected(BID_PLAYERBULLET);
	vdp_spriteHideSelected();
	vdp_spriteMoveToSelected(SCREEN_WIDTH / 2, SCREEN_HEIGHT);		// Offscreen
	objects[PLAYERBULLET].x = SCREEN_WIDTH / 2;
	objects[PLAYERBULLET].y = SCREEN_HEIGHT;
	objects[PLAYERBULLET].state = ST_INACTIVE;
		
	for (i = FIRSTENEMYID; i <= LASTENEMYID; i++)
		{
		objects[i].type = BIRD;
		objects[i].x = rand255();
		objects[i].y = (i - FIRSTENEMYID) * 20;
		objects[i].state = ST_EGG;		//(rand255() > 127) ? ST_MOVELEFT : ST_MOVERIGHT;
		objects[i].frame = 0; //(i % 7) + 7;
		objects[i].counter = 0;
		vdp_spriteSelect(i);
		vdp_spriteClearFramesSelected();
		vdp_spriteAddFrameSelected(BID_EGGS0);		// Frame 0 to 6 = eggs
		vdp_spriteAddFrameSelected(BID_EGGS1);
		vdp_spriteAddFrameSelected(BID_EGGS2);
		vdp_spriteAddFrameSelected(BID_EGGS3);
		vdp_spriteAddFrameSelected(BID_EGGS4);
		vdp_spriteAddFrameSelected(BID_EGGS5);
		vdp_spriteAddFrameSelected(BID_EGGS6);
/* 		vdp_spriteAddFrame(i, BID_SBIRD0);
		vdp_spriteAddFrame(i, BID_SBIRD1);
		vdp_spriteAddFrame(i, BID_SBIRD2);
		vdp_spriteAddFrame(i, BID_SBIRD3);
		vdp_spriteAddFrame(i, BID_SBIRD4);
		vdp_spriteAddFrame(i, BID_SBIRD5);
		vdp_spriteAddFrame(i, BID_SBIRD6); */
		vdp_spriteAddFrameSelected(BID_BBIRD0);		// frame 7 to 13 = bird
		vdp_spriteAddFrameSelected(BID_BBIRD1);
		vdp_spriteAddFrameSelected(BID_BBIRD2);
		vdp_spriteAddFrameSelected(BID_BBIRD3);
		vdp_spriteAddFrameSelected(BID_BBIRD4);
		vdp_spriteAddFrameSelected(BID_BBIRD5);
		vdp_spriteAddFrameSelected(BID_BBIRD6);
		vdp_spriteAddFrameSelected(BID_EXPLOSION0);	// frame 14 to 21 = explosion
		vdp_spriteAddFrameSelected(BID_EXPLOSION1);
		vdp_spriteAddFrameSelected(BID_EXPLOSION2);
		vdp_spriteAddFrameSelected(BID_EXPLOSION3);
		vdp_spriteAddFrameSelected(BID_EXPLOSION4);
		vdp_spriteAddFrameSelected(BID_EXPLOSION5);
		vdp_spriteAddFrameSelected(BID_EXPLOSION6);
		vdp_spriteAddFrameSelected(BID_EXPLOSION7);
		vdp_spriteSetFrameSelected(objects[i].frame);
		vdp_spriteShowSelected();
		}
}

// Change an object's direction
UINT8 GetRandomDirection()
{
	UINT8 LR;
	UINT8 UD;

	// Choose random left/right
	LR = rand255() & 0x03;
	if (3 == LR)
		LR = 0;
	// Choose random up/down
	UD = rand255() & 0x0C;
	if (0x0C == UD)
		UD = 0;

	return (UD | LR);
}

// Update enemy according to it's current behaviour
void UpdateEnemy(int i)
{
	UINT8 d = 0;
	UINT16 f2;
	UINT16 f4;

	OBJECT *pObject = &objects[i];

	pObject->counter++;

	f2 = (pObject->counter >> 1) & 1;		// every 2nd frame
	f4 = (pObject->counter >> 2) & 1;		// every 4th frame

	switch (pObject->state)
		{
		case ST_EGG :
			if (224 == pObject->counter)
				{
				//pObject->state = (rand255() > 127) ? ST_MOVELEFT : ST_MOVERIGHT;
				pObject->state = ST_MOVERANDOM;
				pObject->dirn = GetRandomDirection();
				pObject->counter = 0;
				}

			pObject->frame = (UINT8)pObject->counter >> 5;
			break;
		case ST_MOVERIGHT :
			pObject->x++;
			if (pObject->x == 319)
				pObject->state = ST_MOVELEFT;

			pObject->frame = 7 + ((UINT8)(pObject->counter / 8) % 7);\
			break;
		case ST_MOVELEFT :
			pObject->x--;
			if (pObject->x == 0)
				pObject->state = ST_MOVERIGHT;

			pObject->frame = 7 + ((UINT8)(pObject->counter / 8) % 7);
			break;
		case ST_MOVERANDOM :
			// Move counter frames in the current object "direction"
			// Dirn is 4-bit bitmask for left/right/up/down bits
			d = pObject->dirn; 
			if (d & DIRN_LEFT)
				{
				pObject->x--;
				if (0 == pObject->x)
					pObject->dirn = (d & 0x0C) | DIRN_RIGHT;		// Bounce off left edge of screen
				}
			else if (d & DIRN_RIGHT)
				{
				pObject->x++;
				if (pObject->x == (SCREEN_WIDTH - 32))
					pObject->dirn = (d & 0x0C) | DIRN_LEFT;		// Bounce off right edge of screen
				}

			// Only process y movement every 2nd frame (half-speed y movement)
			if (f4)
				{
				if (d & DIRN_UP)
					{
					pObject->y--;
					if (0 == pObject->y)
						pObject->dirn = (d & 0x03) | DIRN_DOWN;	// Bounce bird off top of screen
					}
				else if (d & DIRN_DOWN)
					{
					pObject->y++;
					if (SCREEN_HEIGHT == pObject->y)
						pObject->y = 0;		// Put bird back at top of screen
					}
				}

			pObject->frame = 7 + ((UINT8)(pObject->counter / 8) % 7);

			// Time to choose a new direction?
			if (64 == pObject->counter)
				{
				pObject->dirn = GetRandomDirection();		// up/donw/left/right bitmask
				pObject->counter = 0;
				}
			break;
		case ST_DYING :
			pObject->frame = 14 + ((UINT8)pObject->counter >> 2);			// Explosion frames

			if (31 == pObject->counter)
				{
				pObject->counter = 0;
				pObject->frame = 0;
				pObject->state = ST_INACTIVE;
				vdp_spriteHide(i);
				}
			break;
		}	// end switch

	// Update sprite on screen
	if (pObject->state != ST_INACTIVE)
		{
		vdp_spriteMoveTo(i, pObject->x, pObject->y);
		vdp_spriteSetFrame(i, pObject->frame);
		}

}


/// @param[in] argc			Argument count
/// @param[in] argv			Pointer to the argument string - zero terminated, parameters separated by spaces
int main(int argc, char * argv[]) {
	UINT16 i, j;
	UINT8 keycode, stick, flags;
	UINT16 t;
	UINT8 numActiveEnemies = 0;
	
	vdp_mode(2);
	vdp_cursorDisable();
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
	
//	vdp_bitmapDraw(BID_EXPLOSION3, 200, 100);

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
	
	vdp_cls();
	

	vdp_spriteActivateTotal(10);
	vdp_spriteRefresh();
//	getch();
		
	for (i = 0; i < 1000; i++)
		{
		waitvblank();
		
		// Move enemies
		numActiveEnemies = 0;
		for (j = FIRSTENEMYID; j <= LASTENEMYID; j++)
			{
			if (ST_INACTIVE == objects[j].state)
				continue;

			numActiveEnemies++;
			UpdateEnemy(j);
			}

		// All enemies dead?
		if (0 == numActiveEnemies)
			break;
			
		// Update bullet
		if (ST_ACTIVE == objects[PLAYERBULLET].state)
			{
			vdp_spriteMoveTo(1, objects[PLAYERBULLET].x, objects[PLAYERBULLET].y);
			objects[PLAYERBULLET].y -= 4;
			if (objects[PLAYERBULLET].y < 4)
				{
				objects[PLAYERBULLET].state = ST_INACTIVE;
				vdp_spriteHide(1);
				}
			else
				{
				// Check bullet against birds (collision)
				for (j = FIRSTENEMYID; j <= LASTENEMYID; j++)
					{
					if (ST_INACTIVE == objects[j].state || ST_DYING == objects[j].state)
						continue;

					if (abs((objects[j].x + 16) - (objects[PLAYERBULLET].x + 8)) < 12)
						{
						t = objects[PLAYERBULLET].y - objects[j].y;			// UINT16
						if (t < 16)
							{
							// Kill the enemy
							audio_play(1, 240, 70, 666);
							objects[j].state = ST_DYING;			// Start dying animation
							objects[j].counter = 0;
							objects[j].frame = 14;
							//vdp_spriteSetFrame(j, 7);				// Set to first frame of explosion animation
							// Also kill the bullet
							objects[PLAYERBULLET].state = ST_INACTIVE;
							vdp_spriteHide(1);
							}

						}
					}
				}
			}

/*
		// To slow (flickers)
		if (i & 0x1)
			{
			vdp_scroll(0, 2, 1);
			vdp_plotColour(rand255());
			vdp_plotPoint(rand255() * 5, 512 + rand255());
			}
 */
			
		keycode = getsysvar_keyascii();
		//keycode = getsysvar_vkeydown();
		//printf("%d\n\r", keycode);
		if (27 == keycode)
			break;

		// Move via keyboard (janky)
		if (8 == keycode && objects[PLAYER].x > 0)
			objects[PLAYER].x -= 2;
		else if (21 == keycode && objects[PLAYER].x < SCREEN_WIDTH - 2)
			objects[PLAYER].x += 2;
		

		// Read joystick and update player position and sprite
		GETDR_PORTC(stick);
		//printf("%d- ", stick);
		//stick = 255;
		if (0 == (stick & 8) && objects[PLAYER].x > 0)
			objects[PLAYER].x -= 2;
		else if (0 == (stick & 16) && objects[PLAYER].x < (SCREEN_WIDTH - 16))
			objects[PLAYER].x += 2;
		
		vdp_spriteMoveTo(0, objects[PLAYER].x, objects[PLAYER].y);
		
		if ((0 == (stick & 4) || 32 == keycode) && ST_INACTIVE == objects[PLAYERBULLET].state)
			{
			// Fire bullet 
			audio_play(0, 200, 1000, 300);
			objects[PLAYERBULLET].state = ST_ACTIVE;
			objects[PLAYERBULLET].x = objects[PLAYER].x;
			objects[PLAYERBULLET].y = objects[PLAYER].y - 16;
			vdp_spriteMoveTo(1, objects[PLAYERBULLET].x, objects[PLAYERBULLET].y);
			vdp_spriteShow(1);
			}

		vdp_spriteRefresh();
/*		
		//flags = (UINT8)getsysvar16bit(sysvar_time);	//sysvar_vpd_pflags);
		//t = getsysvar16bit(sysvar_time);
		//printf("%d\n\r", t);
		//flags = getsysvar8bit(sysvar_time);
		//printf("%d\n\r", flags);
*/
		}
	
	vdp_cursorEnable();
	
//	vdp_mode(1);

	if (0 == numActiveEnemies)
		printf("Congratulations! You defeated the defenseless Space Birds!\n\r");
	else
		printf("Fail! You did not defeat the Space Birds in time!\n\r");
 
	return 0;
}
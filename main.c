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
// 2. Dead birds drop from the sky (with flames on wings), can kill you 
// 3. If you clip a bird's wing, it drops down with erratic side movement

// Levels (affects SetupSprites() and UpdateEnemies()):
// 1. Eggs to smal birds
// 2. Eggs to small birds to big birds
// 3. Eggs to big birds, split into 2 if shot
// 4. Boss?
// Levels repeat like this (1-4 above), but:
//    - number of enemies increases
//    - Enemy bullet frequency increases
//    - Enemy bullet speed increases
//    - Enemies start shooting bullets at an angle

#include <stdio.h>
//#include <ctype.h>
#include <stdlib.h>
#include <math.h>
#include <gpio.h>				// For joystick
#include "mos-interface.h"
#include "vdp.h"
#include "vdp_audio.h"

#include "sprites/ufos.h"			// for bullets
#include "sprites/playerShip.h"
#include "sprites/explosions.h"
#include "sprites/eggs.h"
#include "sprites/bird_16x12.h"
#include "sprites/bird_32x24_shaded.h"
#include "sprites/title_16x16.h"


#define SCREEN_WIDTH	320
#define SCREEN_HEIGHT	200

#define MAX_SPRITES		32
#define BITMAP_WIDTH		16
#define BITMAP_HEIGHT		16	

#define FIRSTENEMYBULLETID	2				// index of the first enemy bullet in the object/sprite array
#define LASTENEMYBULLETID	7
#define FIRSTENEMYID		8				// index of the first enemy in the object array
#define MAX_ENEMIES		12
#define LASTENEMYID		(FIRSTENEMYID + MAX_ENEMIES - 1)	// index of the last enemy in the object array

#define MAX_BOMBDROPCHANCE	100				// Chance of any enemy dropping a bullet every frame (n/65536)


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
	BID_BBIRD6,

	BID_TITLE0,				// 38 to 46 - Title character sprites
	BID_TITLE1,
	BID_TITLE2,
	BID_TITLE3,
	BID_TITLE4,
	BID_TITLE5,
	BID_TITLE6,
	BID_TITLE7,
	BID_TITLE8
	
};

// Object types (type index)
enum {
	PLAYER = 0,
	PLAYERBULLET,
	ENEMYBULLET,
	EGG,
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

// Points for each enemy type (for scoring)
const int PointsPerType[8] = {
	0, 				// PLAYER = 0,
	0, 				// PLAYERBULLET,
	0, 				// ENEMYBULLET,
	50, 			// EGG,
	50, 			// BIRD,
	100, 			// BIGBIRD
	0
	};

// Global Variables (oooh!)
int bombDropChance = 40;				// 40 / 65536 chance
int score = 0;
int hiscore = 0;

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

	// Small enemy birds use bitmaps 23 to 29
	vdp_bitmapSendData(BID_SBIRD0, SBIRD_WIDTH, SBIRD_HEIGHT, sbirdData[0]);
	vdp_bitmapSendData(BID_SBIRD1, SBIRD_WIDTH, SBIRD_HEIGHT, sbirdData[1]);
	vdp_bitmapSendData(BID_SBIRD2, SBIRD_WIDTH, SBIRD_HEIGHT, sbirdData[2]);
	vdp_bitmapSendData(BID_SBIRD3, SBIRD_WIDTH, SBIRD_HEIGHT, sbirdData[3]);
	vdp_bitmapSendData(BID_SBIRD4, SBIRD_WIDTH, SBIRD_HEIGHT, sbirdData[4]);
	vdp_bitmapSendData(BID_SBIRD5, SBIRD_WIDTH, SBIRD_HEIGHT, sbirdData[5]);
	vdp_bitmapSendData(BID_SBIRD6, SBIRD_WIDTH, SBIRD_HEIGHT, sbirdData[6]);

	// Enemy bullet at bitmap 30
	vdp_bitmapSendData(BID_ENEMYBULLET, 16, 16, enemyBulletData);
	
	// Big enemy birds use bitmaps 31 to 37
	vdp_bitmapSendData(BID_BBIRD0, BBIRD_WIDTH, BBIRD_HEIGHT, bbirdData[0]);
	vdp_bitmapSendData(BID_BBIRD1, BBIRD_WIDTH, BBIRD_HEIGHT, bbirdData[1]);
	vdp_bitmapSendData(BID_BBIRD2, BBIRD_WIDTH, BBIRD_HEIGHT, bbirdData[2]);
	vdp_bitmapSendData(BID_BBIRD3, BBIRD_WIDTH, BBIRD_HEIGHT, bbirdData[3]);
	vdp_bitmapSendData(BID_BBIRD4, BBIRD_WIDTH, BBIRD_HEIGHT, bbirdData[4]);
	vdp_bitmapSendData(BID_BBIRD5, BBIRD_WIDTH, BBIRD_HEIGHT, bbirdData[5]);
	vdp_bitmapSendData(BID_BBIRD6, BBIRD_WIDTH, BBIRD_HEIGHT, bbirdData[6]);

	// Title sprite characters use bitmaps 38 to 46
	vdp_bitmapSendData(BID_TITLE0, TITLE_WIDTH, TITLE_HEIGHT, titleData[0]);
	vdp_bitmapSendData(BID_TITLE1, TITLE_WIDTH, TITLE_HEIGHT, titleData[1]);
	vdp_bitmapSendData(BID_TITLE2, TITLE_WIDTH, TITLE_HEIGHT, titleData[2]);
	vdp_bitmapSendData(BID_TITLE3, TITLE_WIDTH, TITLE_HEIGHT, titleData[3]);
	vdp_bitmapSendData(BID_TITLE4, TITLE_WIDTH, TITLE_HEIGHT, titleData[4]);
	vdp_bitmapSendData(BID_TITLE5, TITLE_WIDTH, TITLE_HEIGHT, titleData[5]);
	vdp_bitmapSendData(BID_TITLE6, TITLE_WIDTH, TITLE_HEIGHT, titleData[6]);
	vdp_bitmapSendData(BID_TITLE7, TITLE_WIDTH, TITLE_HEIGHT, titleData[7]);
	vdp_bitmapSendData(BID_TITLE8, TITLE_WIDTH, TITLE_HEIGHT, titleData[8]);

}

// Redraw the score display
void UpdateScore()
{
	vdp_cursorGoto(0, 0);
	printf("SCORE: %ld", score);
}

// Show the title sequence
void DoTitle()
{
	UINT8 i, keycode, startCount;
	float r = 0.0f;
	float dy;

	// Set up sprites
	for (i = 0; i < MAX_SPRITES; i++)
		{
		vdp_spriteHide(i);
		}

	vdp_cls();

	for (i = 0; i < TITLE_FRAME_COUNT; i++)
		{
		objects[i].type = 0;
		objects[i].x = i * 16 + 68;
		objects[i].y = SCREEN_HEIGHT / 3;
		vdp_spriteSelect(i);
		vdp_spriteClearFramesSelected();
		vdp_spriteAddFrameSelected(BID_TITLE0 + i);
		vdp_spriteShowSelected();
		vdp_spriteMoveToSelected(objects[i].x, objects[i].y);
		}

	vdp_spriteActivateTotal(9);
	vdp_spriteRefresh();

	vdp_cursorGoto(10, 20);
	printf("By Jum Hig 2023");
	vdp_cursorGoto(8, 22);
	printf("Press SPACE to start");

	// Wait for keypress
	startCount = 255;
	while (startCount > 0)
		{
		waitvblank();
		r += 0.03;
		for (i = 0; i < TITLE_FRAME_COUNT; i++)
			{
			dy = sin(r + ((float)i * 0.1f)) * 24.0f;
			vdp_spriteMoveTo(i, objects[i].x, objects[i].y + (int)dy);
			}

		vdp_spriteRefresh();

		keycode = getsysvar_keyascii();
		if (32 == keycode && 255 == startCount)
			startCount = SCREEN_HEIGHT;

		// Scroll screen up after we have pressed start
		if (startCount < 255)
			{
			vdp_scroll(0, 3, 1);
			vdp_plotColour(rand255());
			vdp_plotPoint(rand255() * 5, 0);
			for (i = 0; i < TITLE_FRAME_COUNT; i++)
				objects[i].y--;
			startCount--;
			}
		}

}

// Helper function - get number of enemies for a specific level
UINT8 GetNumEnemiesPerLevel(UINT level)
{
	UINT8 numEnemies;

	numEnemies = 6 + (level / 4);
	if (numEnemies > MAX_ENEMIES)
		numEnemies = MAX_ENEMIES;

	return numEnemies;
}

// Setup the sprites according to the level
void SetupSprites(UINT8 level)
{
	UINT8 i;
	UINT8 numEnemies;

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
	objects[PLAYER].type = PLAYER;
	objects[PLAYER].x = SCREEN_WIDTH / 2;
	objects[PLAYER].y = SCREEN_HEIGHT - 30;		// Leave gap below so player can see enemies below
	objects[PLAYER].state = ST_ACTIVE;		

	// Set up player bullet sprite
	vdp_spriteSelect(1);
	vdp_spriteClearFramesSelected();
	vdp_spriteAddFrameSelected(BID_PLAYERBULLET);
	vdp_spriteHideSelected();
	vdp_spriteMoveToSelected(SCREEN_WIDTH / 2, SCREEN_HEIGHT);		// Offscreen
	objects[PLAYERBULLET].type = PLAYERBULLET;
	objects[PLAYERBULLET].x = SCREEN_WIDTH / 2;
	objects[PLAYERBULLET].y = SCREEN_HEIGHT;
	objects[PLAYERBULLET].state = ST_INACTIVE;

	// Set up enemy bullet sprites
	for (i = FIRSTENEMYBULLETID; i <= LASTENEMYBULLETID; i++)
		{
		objects[i].type = ENEMYBULLET;
		objects[i].x = 0;
		objects[i].y = SCREEN_HEIGHT;
		objects[i].state = ST_INACTIVE;
		objects[i].frame = 0; 
		objects[i].counter = 0;
		vdp_spriteSelect(i);
		vdp_spriteClearFramesSelected();
		vdp_spriteAddFrameSelected(BID_ENEMYBULLET);
		vdp_spriteHideSelected();
		vdp_spriteMoveToSelected(SCREEN_WIDTH / 2, SCREEN_HEIGHT);		// Offscreen
		}

	// Set up enemy bird sprites
	// Number of enemies increases every time you defeat level 4
	numEnemies = GetNumEnemiesPerLevel(level);

	// We have to set up animation frames for all possible enemy sprites, so that we
	// can spawn additional enemies during gameplay
	for (i = FIRSTENEMYID; i <= LASTENEMYID; i++)
		{
		if (i < FIRSTENEMYID + numEnemies)
			{
			// Active enemy for this level
			objects[i].type = EGG;
			objects[i].x = (rand() % (SCREEN_WIDTH - 20)) + 10;
			objects[i].y = (i - FIRSTENEMYID) * 20;
			objects[i].state = ST_EGG;		//(rand255() > 127) ? ST_MOVELEFT : ST_MOVERIGHT;
			}
		else
			{
			// Active enemy "slot" for this level
			objects[i].type = BIRD;
			objects[i].x = 0;
			objects[i].y = SCREEN_HEIGHT;
			objects[i].state = ST_INACTIVE;
			}
		objects[i].frame = 0;
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
 		vdp_spriteAddFrameSelected(BID_SBIRD0);		// frame 7 to 13 = small bird
		vdp_spriteAddFrameSelected(BID_SBIRD1);
		vdp_spriteAddFrameSelected(BID_SBIRD2);
		vdp_spriteAddFrameSelected(BID_SBIRD3);
		vdp_spriteAddFrameSelected(BID_SBIRD4);
		vdp_spriteAddFrameSelected(BID_SBIRD5);
		vdp_spriteAddFrameSelected(BID_SBIRD6);
		vdp_spriteAddFrameSelected(BID_BBIRD0);		// frame 14 to 21 = big bird
		vdp_spriteAddFrameSelected(BID_BBIRD1);
		vdp_spriteAddFrameSelected(BID_BBIRD2);
		vdp_spriteAddFrameSelected(BID_BBIRD3);
		vdp_spriteAddFrameSelected(BID_BBIRD4);
		vdp_spriteAddFrameSelected(BID_BBIRD5);
		vdp_spriteAddFrameSelected(BID_BBIRD6);
		vdp_spriteAddFrameSelected(BID_EXPLOSION0);	// frame 22 to 29 = explosion
		vdp_spriteAddFrameSelected(BID_EXPLOSION1);
		vdp_spriteAddFrameSelected(BID_EXPLOSION2);
		vdp_spriteAddFrameSelected(BID_EXPLOSION3);
		vdp_spriteAddFrameSelected(BID_EXPLOSION4);
		vdp_spriteAddFrameSelected(BID_EXPLOSION5);
		vdp_spriteAddFrameSelected(BID_EXPLOSION6);
		vdp_spriteAddFrameSelected(BID_EXPLOSION7);
		vdp_spriteSetFrameSelected(objects[i].frame);
		if (i < FIRSTENEMYID + numEnemies)
			vdp_spriteShowSelected();				// Only show the inital number of enemies starting this level
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

// Drop a new enemy bullet
void DropEnemyBullet(OBJECT *pParentObject)
{
	UINT8 n;
	OBJECT *pBulletObject;

	if (!pParentObject)
		return;

	// Don't allow enemies to fire from too low
	if (pParentObject->y > (SCREEN_HEIGHT - 60))
		return;

	// Find first inactive enemy bullet object/sprite
	for (n = FIRSTENEMYBULLETID; n <= LASTENEMYBULLETID; n++)
		{
		pBulletObject = &objects[n];
		if (ST_INACTIVE == pBulletObject->state)
			{
			// Set up new enemy bullet 
			audio_play(0, 75, 1024, 100);
			pBulletObject->state = ST_ACTIVE;
			pBulletObject->x = (BIGBIRD == pParentObject->type) ? pParentObject->x + 8 : pParentObject->x;
			pBulletObject->y = pParentObject->y + 16;
			vdp_spriteMoveTo(n, pBulletObject->x, pBulletObject->y);
			vdp_spriteShow(n);
			break;				// don't fire multiple bullets!
			}
		}

}

// Update enemy bullet
void UpdateEnemyBullet(UINT8 n, UINT8 level)
{
	UINT8 d = 0;
	UINT16 f2;
	UINT16 f4;

	OBJECT *pObject = &objects[n];

	if (ST_ACTIVE == pObject->state)
		{
		pObject->counter++;

		// Hit player?
		// Note: Enemy bullet hitpoint = 8, 3, and enemy bullet sprite width = player sprite width
		if (pObject->y > objects[PLAYER].y && pObject->y < (objects[PLAYER].y + 16))
			{
			if (abs(pObject->x - objects[PLAYER].x) < 9)
				{
				// Kill the player
				audio_play(1, 120, 70, 666);
				objects[PLAYER].state = ST_DYING;			// Start dying animation
				objects[PLAYER].counter = 0;

				pObject->y = SCREEN_HEIGHT;			// kill this bullet below
				}
			}

		if (level < 4)
			pObject->y += 1;
		else
			pObject->y += 2;

		if (pObject->y >= SCREEN_HEIGHT)
			{
			pObject->counter = 0;
			pObject->frame = 0;
			pObject->state = ST_INACTIVE;
			vdp_spriteHide(n);
			}
		}

	// Update sprite on screen
	if (pObject->state != ST_INACTIVE)
		{
		vdp_spriteMoveTo(n, pObject->x, pObject->y);
		vdp_spriteSetFrame(n, pObject->frame);
		}

}

// Update enemy according to it's current behaviour
void UpdateEnemy(UINT8 n, UINT8 level)
{
	UINT8 d = 0;
	UINT8 i;
	UINT16 f2;
	UINT16 f4;

	OBJECT *pObject = &objects[n];
	OBJECT *pEnemyObject = NULL;

	pObject->counter++;

	f2 = (pObject->counter >> 1) & 1;		// every 2nd frame
	f4 = (pObject->counter >> 2) & 1;		// every 4th frame

	switch (pObject->state)
		{
		case ST_EGG :
			if (224 == pObject->counter)
				{
				// First 2 levels of every round, egg hatches to small bird
				if (level & 0x2)
					pObject->type = BIGBIRD;
				else
					pObject->type = BIRD;
				pObject->state = ST_MOVERANDOM;
				pObject->dirn = GetRandomDirection();
				pObject->frame = 7;
				pObject->counter = rand255();		//0; 
				}
			else
				{
				// Update egg animation frame
				pObject->frame = (UINT8)pObject->counter >> 5;
				}
			break;
//		case ST_MOVERIGHT :
//			pObject->x++;
//			if (pObject->x == 319)
//				pObject->state = ST_MOVELEFT;
//
//			pObject->frame = 7 + ((UINT8)(pObject->counter / 8) % 7);\
//			break;
//		case ST_MOVELEFT :
//			pObject->x--;
//			if (pObject->x == 0)
//				pObject->state = ST_MOVERIGHT;
//
//			pObject->frame = 7 + ((UINT8)(pObject->counter / 8) % 7);
//			break;
		case ST_MOVERANDOM :
			// Move counter frames in the current object "direction"
			// Dirn is 4-bit bitmask for left/right/up/down bits
			d = pObject->dirn; 
			if (d & DIRN_LEFT)
				{
				pObject->x--;
				if (pObject->x < 10)
					pObject->dirn = (d & 0x0C) | DIRN_RIGHT;		// Bounce off left edge of screen
				}
			else if (d & DIRN_RIGHT)
				{
				pObject->x++;
				if (pObject->x > (SCREEN_WIDTH - 32))
					pObject->dirn = (d & 0x0C) | DIRN_LEFT;		// Bounce off right edge of screen
				}

			// Only process y movement every 2nd frame (half-speed y movement)
			if (f4)
				{
				if (d & DIRN_UP)
					{
					pObject->y--;
					if (pObject->y < 5)
						{
						pObject->dirn = DIRN_DOWN;	//(d & 0x03) | DIRN_DOWN;	// Bounce bird off top of screen
						}
					}
				else if (d & DIRN_DOWN)
					{
					pObject->y++;
					// If bird goes off bottom of screen, then put back at top
					// and set it's direction to downwards
					if (pObject->y > SCREEN_HEIGHT)
						{
						pObject->y = 0;
						pObject->dirn = DIRN_DOWN;
						if (pObject->x < 40)
							pObject->dirn |= DIRN_RIGHT;
						else if (pObject->x > SCREEN_WIDTH - 40)
							pObject->dirn |= DIRN_LEFT;
						pObject->counter = 0;
						}
					}
				}

			if (BIGBIRD == pObject->type)
				pObject->frame = 14 + ((UINT8)(pObject->counter / 8) % 7);
			else
				pObject->frame = 7 + ((UINT8)(pObject->counter / 8) % 7);

			// Time to choose a new direction?
			if ((pObject->counter % 64) == 0)
				{
				pObject->dirn = GetRandomDirection();		// up/down/left/right bitmask
				}

			// Time to fire a bullet / drop a bomb?
			if (BIRD == pObject->type && rand() < bombDropChance)
				DropEnemyBullet(pObject);
			else if (BIGBIRD == pObject->type && rand() < (bombDropChance * 2))		// Big birds drop twice as many bombs
				DropEnemyBullet(pObject);

			// Small bird grows to big bird? (level 2/4 only)
			if (BIRD == pObject->type && 0x400 == pObject->counter && (level & 0x3) == 1)
				{
				pObject->type = BIGBIRD;
				pObject->counter = rand255();
				}

			// Big bird splits into 2 small birds? (level 3/4 only)
			if (BIGBIRD == pObject->type && 0x400 == pObject->counter && (level & 0x3) == 2)
				{
				// First see if we can spawn another bird
				for (i = FIRSTENEMYID; i <= LASTENEMYID; i++)
					{
					if (i == n)
						continue;

					pEnemyObject = &objects[i];
					if (ST_INACTIVE == pEnemyObject->state)
						{
						// Set up new enemy bird, going right
						audio_play(0, 75, 2000, 300);
						pEnemyObject->type = BIRD;
						pEnemyObject->state = ST_MOVERANDOM;
						pEnemyObject->x = pObject->x;
						pEnemyObject->y = pObject->y;
						pEnemyObject->counter = 32;		// Half way to next direction decision
						pEnemyObject->dirn = DIRN_RIGHT;
						vdp_spriteMoveTo(i, pEnemyObject->x, pEnemyObject->y);
						vdp_spriteShow(i);
						break;				// i will be new bird index at this point
						}
					}

				// If we could successfuly spawn a small bird then turn this bird into a small bird, going left
				if (i <= LASTENEMYID)
					{
					pObject->type = BIRD;
					pObject->counter = 32;		// Half way to next direction decision
					pObject->dirn = DIRN_LEFT;
					}
				}
			break;
		case ST_DYING :
			pObject->frame = 22 + ((UINT8)pObject->counter >> 2);			// Explosion frames

			if (31 == pObject->counter)
				{
				pObject->counter = 0;
				pObject->frame = 0;
				pObject->state = ST_INACTIVE;
				vdp_spriteHide(n);
				}
			break;
		}	// end switch

	// Update sprite on screen
	if (pObject->state != ST_INACTIVE)
		{
		vdp_spriteMoveTo(n, pObject->x, pObject->y);
		vdp_spriteSetFrame(n, pObject->frame);

		// Check for collision with player
		d = (BIGBIRD == pObject->type) ? 24 : 12;				// coll "diameter" of enemy
		i = (BIGBIRD == pObject->type) ? 8 : 0;				// enemy top-left offset relative to player sprite
		if (pObject->y > (objects[PLAYER].y - d) && pObject->y < (objects[PLAYER].y + 14))
			{
			if (abs(pObject->x + i - objects[PLAYER].x) <= d)
				{
				// Kill the player
				audio_play(1, 120, 70, 666);
				objects[PLAYER].state = ST_DYING;			// Start dying animation
				objects[PLAYER].counter = 0;
				}
			}
		}

}

/// Update the player's bullet (if active)
void UpdatePlayerBullet()
{
	UINT8 n;
	UINT8 xhit, yhit;			// for collision detection

	vdp_spriteMoveTo(PLAYERBULLET, objects[PLAYERBULLET].x, objects[PLAYERBULLET].y);

	objects[PLAYERBULLET].y -= 4;
	if (objects[PLAYERBULLET].y < 4)
		{
		objects[PLAYERBULLET].state = ST_INACTIVE;
		vdp_spriteHide(PLAYERBULLET);
		}
	else
		{
		// Check bullet against birds (collision)
		for (n = FIRSTENEMYID; n <= LASTENEMYID; n++)
			{
			if (ST_INACTIVE == objects[n].state || ST_DYING == objects[n].state)
				continue;

			// Different collision detection for egss and small birds, vs big birds
			if(BIGBIRD == objects[n].type)
				xhit = (abs((objects[n].x + 16) - (objects[PLAYERBULLET].x + 8)) < 12);
			else
				xhit = (abs(objects[n].x - objects[PLAYERBULLET].x) < 9);

			if (xhit)
				{
				if (BIGBIRD == objects[n].type)
					yhit = ((objects[PLAYERBULLET].y - objects[n].y) < 20);
				else
					yhit = ((objects[PLAYERBULLET].y - objects[n].y) < 14);

				if (yhit)
					{
					// Kill the enemy
					audio_play(1, 100, 75, 250);
					objects[n].state = ST_DYING;			// Start dying animation
					objects[n].counter = 0;
					objects[n].frame = 22;
					//vdp_spriteSetFrame(j, 7);				// Set to first frame of explosion animation
					// Also kill the bullet
					objects[PLAYERBULLET].state = ST_INACTIVE;
					vdp_spriteHide(PLAYERBULLET);
					// Update the score display
					score += PointsPerType[objects[n].type];		// Add to score, depending on enemy type we shot
					UpdateScore();
					}
				}
			}
		}

}

/// Update player movement, check for player fire
void UpdatePlayer(UINT8 keycode)
{
	UINT8 stick;

	// Move via keyboard (janky)
	if (8 == keycode && objects[PLAYER].x > 0)
		objects[PLAYER].x -= 2;
	else if (21 == keycode && objects[PLAYER].x < (SCREEN_WIDTH - 16))
		objects[PLAYER].x += 2;


	// Read joystick and update player position and sprite
	GETDR_PORTC(stick);
	//printf("%d- ", stick);
	//stick = 255;
	if (0 == (stick & 8) && objects[PLAYER].x > 0)
		objects[PLAYER].x -= 2;
	else if (0 == (stick & 16) && objects[PLAYER].x < (SCREEN_WIDTH - 16))
		objects[PLAYER].x += 2;

	vdp_spriteMoveTo(PLAYER, objects[PLAYER].x, objects[PLAYER].y);

	if ((0 == (stick & 4) || 32 == keycode) && ST_INACTIVE == objects[PLAYERBULLET].state)
		{
		// Fire bullet 
		audio_play(0, 100, 880, 100);
		objects[PLAYERBULLET].state = ST_ACTIVE;
		objects[PLAYERBULLET].x = objects[PLAYER].x;
		objects[PLAYERBULLET].y = objects[PLAYER].y - 16;
		vdp_spriteMoveTo(PLAYERBULLET, objects[PLAYERBULLET].x, objects[PLAYERBULLET].y);
		vdp_spriteShow(PLAYERBULLET);
		}

}

/// Play a level
/// @param[in] level		The level to set up and play
/// @return				The level quit state (player win, player dead etc)
UINT8 PlayLevel(UINT8 level)
{
	UINT8 endState = 0;
	UINT8 n;
	UINT8 numActiveEnemies;
	UINT8 keycode, stick, flags;
	UINT16 t;

	// Level intermission - Scroll screen and fill with stars
	if (level > 0)
		{
		// Hide all extraneous sprites besides player
		for (n = 1; n <= LASTENEMYBULLETID; n++)
			vdp_spriteHide(n);
		vdp_spriteRefresh();

		for (n = 0; n < SCREEN_HEIGHT; n++)
			{
			waitvblank();
			vdp_scroll(0, 2, 1);
			vdp_plotColour(rand255());
			vdp_plotPoint(rand255() * 5, 1023);			// plot coordinates!
			}
		}

	// Show current level and score display
	vdp_cursorGoto(30, 0);
	printf("Level: %d   ", level + 1);
	UpdateScore();

	//printf("Setting up sprites...\n\r");
	SetupSprites(level);

//	getch();

	// Activate the correct no of sprites?
	//numEnemies = GetNumEnemiesPerLevel(level);
	vdp_spriteActivateTotal(LASTENEMYID + 1);
	vdp_spriteRefresh();
//	getch();

	bombDropChance = 40 + (level * 5);				// Starting enemy bullet chance = 40/65536
	if (bombDropChance > MAX_BOMBDROPCHANCE)
		bombDropChance = MAX_BOMBDROPCHANCE;
		
	while (!endState)
		{
		waitvblank();

		// Move enemy bullets
		for (n = FIRSTENEMYBULLETID; n <= LASTENEMYBULLETID; n++)
			{
			if (objects[n].state != ST_INACTIVE)
				UpdateEnemyBullet(n, level);
			}
		
		// Move enemies
		numActiveEnemies = 0;
		for (n = FIRSTENEMYID; n <= LASTENEMYID; n++)
			{
			if (objects[n].state != ST_INACTIVE)
				{
				numActiveEnemies++;
				UpdateEnemy(n, level);
				}
			}

		// All enemies dead?
		if (0 == numActiveEnemies)
			break;
			
		// Update player bullet
		if (ST_ACTIVE == objects[PLAYERBULLET].state)
			UpdatePlayerBullet();

/*
		// Too slow (flickers)
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

		// Move player (if alive)
		if (ST_ACTIVE == objects[PLAYER].state)
			{
			UpdatePlayer(keycode);
			}
		else
			{
			// Player dying or dead
			vdp_spriteSetFrame(PLAYER, objects[PLAYER].counter / 8);
			objects[PLAYER].counter++;
			if (32 == objects[PLAYER].counter)
				endState = 1;
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

	return endState;
}


/// @param[in] argc			Argument count
/// @param[in] argv			Pointer to the argument string - zero terminated, parameters separated by spaces
int main(int argc, char * argv[]) {
	UINT16 i;
	//UINT8 n;
	//UINT8 keycode, stick, flags;
	//UINT16 t;
	//UINT8 numActiveEnemies = 0;
	UINT8 level = 0;
	UINT8 levelEndState = 0; 

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

	DoTitle();
	
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

	score = 0;
	levelEndState = 0;
	while (0 == levelEndState)
		{
		levelEndState = PlayLevel(level);
		level++;
		}

	vdp_cursorEnable();
	
//	vdp_mode(1);

	vdp_cursorGoto(0, 22);
 
	return 0;
}
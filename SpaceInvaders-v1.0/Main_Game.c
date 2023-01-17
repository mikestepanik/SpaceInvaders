// May 5, 2022
// CPE 355
//	Written by Mike Stepanik and Zebin He
// 	Space Invaders Project
//**********************************
// Notes for instructor only. Remove this block before posting to Kodiak.
// Initialize 3 rows of sprites, then continue implementing basic game.
// 1. Try not using black enemies and objects to cover tracks.
// 2. Post this project using 18 enemies so that students implement
// game with a one-dimensional array. Some students created a 
// two-dimensional array and that increases complexity.
// 3. Only include minimal sprites in this project. Students have a tendency 
// to leave all sprites included which affets memory and compiler usage.
// Post the arrays separately and provide a caution for having too many sprite arrays.
// 4. Discuss how to use animation with the two images A and B, and FrameCount.
//**********************************

// CPE355ProjectStarter.c
// File created on March 21, 2021.
//
//
// Amer Qouneh created this file based on Valvano's online video and textbook.
// See Valvano's video "Video 15.5b. Demand-based Graphics on the ST7735R" found on:
// http://users.ece.utexas.edu/~valvano/Volume1/E-Book/C15_CompleteEmbeddedSystem.htm
//
// This project shows how to initialize, move, and draw sprites.
// Code in this file serves as a starting point for designning 
// and implementing a game of Space Invaders. It does not contain a game engine. 

// This file was a copy of ST7735TestMain.c by Daniel Valvano.
// The content was edited to contain a short demo of game code.
// Amer Qouneh ported this project from ST7735_4C123 folder on 3/21/2021.

//***********************************************************
// Runs on TM4C123
// Test the functions in ST7735.c by printing basic
// patterns to the LCD.
//    16-bit color, 128 wide by 160 high LCD
// Daniel Valvano
// March 30, 2015

/* This example accompanies the book
   "Embedded Systems: Real Time Interfacing to ARM Cortex M Microcontrollers",
   ISBN: 978-1463590154, Jonathan Valvano, copyright (c) 2014

 Copyright 2015 by Jonathan W. Valvano, valvano@mail.utexas.edu
    You may use, edit, run or distribute this file
    as long as the above copyright notice remains
 THIS SOFTWARE IS PROVIDED "AS IS".  NO WARRANTIES, WHETHER EXPRESS, IMPLIED
 OR STATUTORY, INCLUDING, BUT NOT LIMITED TO, IMPLIED WARRANTIES OF
 MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE APPLY TO THIS SOFTWARE.
 VALVANO SHALL NOT, IN ANY CIRCUMSTANCES, BE LIABLE FOR SPECIAL, INCIDENTAL,
 OR CONSEQUENTIAL DAMAGES, FOR ANY REASON WHATSOEVER.
 For more information about my classes, my research, and my books, see
 http://users.ece.utexas.edu/~valvano/
 */

// hardware connections
// **********ST7735 TFT and SDC*******************
// ST7735
// Backlight (pin 10) connected to +3.3 V
// MISO (pin 9) unconnected
// SCK (pin 8) connected to PA2 (SSI0Clk)
// MOSI (pin 7) connected to PA5 (SSI0Tx)
// TFT_CS (pin 6) connected to PA3 (SSI0Fss)
// CARD_CS (pin 5) unconnected
// Data/Command (pin 4) connected to PA6 (GPIO), high for data, low for command
// RESET (pin 3) connected to PA7 (GPIO)
// VCC (pin 2) connected to +3.3 V
// Gnd (pin 1) connected to ground

//INPUTS
// slide potentiometer connected to PE2    //used for moving the spaceship
// Button connected to PE0								 //used for shooting lasers and clicking through start and gameover menus
#include <string.h>
#include <stdio.h>
#include <stdint.h>

#include "ST7735.h"
#include "PLL.h"
#include "tm4c123gh6pm.h"
#include "random.h"
#include "sprite.h"
#include "ADCSWTrigger.h"
//#include "slider.h"
#include "PeriodicSysTickInts.h"

#include "display_consts.h"
#include "menu.h"
#include "hud.h"
#include "Display_Funcs.h"
#include "game_vars.h"
#include <stdlib.h>
#include "game_consts.h"

#define right 1
#define left 2

#define Running 1
#define GameOver 2
#define NextLevel 3

int m=0; //unused
//********************************************************************************************
//
//
//                                     Images
//
// It seems that images are saved in reverse; i.e. top row is actually the bottom row of pixels.
//
// See end of Section 15.3.2. Buffer-based Graphics on the Nokia 5110 discussing Figure 15.7.
// on web page: Chapter 15: Complete Embedded System, Modified to be compatible with EE319K Lab 10
// http://users.ece.utexas.edu/~valvano/Volume1/E-Book/C15_CompleteEmbeddedSystem.htm
// The 2-D image is stored in row-major format. The image is stored up-side down. 
// When plotting it on the screen the Nokia5110_PrintBMP() function will reverse it so it is seen right-side up.
//

// Space Invaders Banner
//128x65

//********************************************************************************************

unsigned long result;     //unused in game (used for testing) 
int lastResult;						//unused in game (used for testing of making the ship postion 1 for 1 with slide meter)
int currentResult;				//unused in game (used for testing of making the ship postion 1 for 1 with slide meter)
int previous;							//unused in game (used for testing)
int drawblack=0;					//unused in game (used for testing)
int sum=0;								//unused in game (used for testing)
int counter=0;						//unused in game (used for testing)
int a=0;								//used for detecting rising edge of button
int b=0;                //used for detecting rising edge of button
int L=0;          
int rising_edge=0;
int button_input=0;
int game_status;     //Running, nextlevel, or GameOver
int highscore=0;     //not functional
int level=1;       //start at level 1
int lives=3;     
int done=0;      //semaphore for level 2
int ibound=6;   //LEVEL 1 # ENEMIES....increaseas as levels go on 
int done2=1;		//semaphore for level 3
int score=0;    
int justhit=0;  //semaphore for drawing smallexplosion
int done3=0;
int round1=0;   //unused (were going to be used for testing rounds of missiles)
int round2=0;		//unused (were going to be used for testing rounds of missiles)
int round3=0;		//unused (were going to be used for testing rounds of missiles)


void DisableInterrupts(void); 	// Disable interrupts
void EnableInterrupts(void);  	// Enable interrupts
long StartCritical (void);    	// previous I bit, disable interrupts
void EndCritical(long sr);    	// restore I bit to previous value
void WaitForInterrupt(void);  	// low power mode
void DelayWait10ms(uint32_t n); // delay function
void Button_Init_portE(void); 	// init button used for shooting lasers
int button_logic(void);					// reading button
int slide_meter (void);					// moving ship with read ADC value of  slide potentiometer
void GameTask(void);            //not used
int enemyHit(int score);				//enemy hit by player laser function
int playerHit(int lives);				//Player hit by player laser function (NOT FUNCTIONAL AND IS COMMENTED OUT)
void drawMissiles(void);
void drawBunkers(void);
void drawLasers(void);
void drawEnemys(void);
void moveLasers(void);
void moveEnemys(void);
void moveMissiles(void);
void moveSpaceShip(void);
void processbutton(void);			//detect rising edge of button press




unsigned long thirty_Hz = 2664000+1000000;	//value used in SysTick_Init for a 60hz Periodic SysTick Interrupt [NOT USED]

unsigned long sixty_Hz = 1332000;          //value used in SysTick_Init for a 60hz Periodic SysTick Interrupt	[USED]


int game_proc_finished=0;          //semaphore for finishing the Periodic SysTick Interrupt Handler Function
//slide potentiometer processing function
int slide_meter (void){
unsigned long volatile result1;
result1=result;
	
if (result1 > 2200)
		
		return left;
			else if (result1 < 1900)
				
			return right;
			
			else 
				return 0;
}

//ENEMY DECLARATIONS
//********************************************************************************************

	// Declare an array of sprites called Enemy.	
	sprite_t Enemy[18];	// Starting with 18 enemies.
	//sprite_t Enemy[6];	// Starting with 6 enemies.
	int Flag;	// Semaphore. Tells foreground to redraw image.
	int Anyalive;	// Flag to tell when game is over. When all sprites are dead.
	
	// Declare spaceship		
	spaceship_t SpaceShip;
	laser_t spaceshiplaser;
	missile_t Missile[9];
	bunker_t Bunker[3];

	//GAME INIT
//*****************************************************
	void GameInit(void){
		int x,y;
		int i;
//		int Flag;
		//Flag = 0;
	
			Enemy[i].vx = 0;	// not moving
			Enemy[i].vy = 0;	// not moving
			
		//create Enemy Missiles
		for (i = 0; i < 3; i++)
		{
			
			Missile[i].x = i*36;
			Missile[i].y = 20;
			Missile[i].vx = 0;
			Missile[i].vy = 2;
			Missile[i].image[0] = EnemyLaser0;
			//Missile[i].image[1] = Missile1;
			Missile[i].black = BlackEnemy;
			Missile[i].life = alive;
			Missile[i].w = 2;
			Missile[i].h = 9;
			Missile[i].needDraw = 1;
		}
		int a=0;
				for (i = 3; i < 6; i++)
		{ 
			a++;
			Missile[i].x = a*38;
			Missile[i].y = 20;
			Missile[i].vx = 0;
			Missile[i].vy = 2;
			Missile[i].image[0] = EnemyLaser0;
			//Missile[i].image[1] = Missile1;
			Missile[i].black = BlackEnemy;
			Missile[i].life = alive;
			Missile[i].w = 2;
			Missile[i].h = 9;
			Missile[i].needDraw = 1;
		}
		a=0;
						for (i = 6; i < 9; i++)
		{ 
			
			Missile[i].x = a*36;
		a++;
			Missile[i].y = 20;
			Missile[i].vx = 0;
			Missile[i].vy = 2;
			Missile[i].image[0] = EnemyLaser0;
			//Missile[i].image[1] = Missile1;
			Missile[i].black = BlackEnemy;
			Missile[i].life = alive;
			Missile[i].w = 2;
			Missile[i].h = 9;
			Missile[i].needDraw = 1;
		}
		

	//create lasers

		spaceshiplaser.life = dead;
		spaceshiplaser.vx = 0;
		spaceshiplaser.vy = -4;
		spaceshiplaser.image[0] = Laser0;
		spaceshiplaser.black = Laser1;
		spaceshiplaser.w = 2;
		spaceshiplaser.h = 9;

	
		y = BUNKER_START_Y, x = BUNKER_START_X;
		//y=150;
		//x=18;
		
		//CREATE BUNKERS
		for (int i = 0; i < 3; i++)
	{
		Bunker[i].x = x;
		Bunker[i].y = y;
		Bunker[i].vx = 0;
		Bunker[i].vy = 0;
		Bunker[i].black = Bunker3;
		Bunker[i].life = alive;
		Bunker[i].w = 18;
		Bunker[i].h = 5;
		Bunker[i].image[0] = Bunker0;
		Bunker[i].image[1] = Bunker1;
		Bunker[i].image[2] = Bunker2;
		Bunker[i].needDraw = 1;		
		Bunker[i].hits = 0;		
		x += (MAX_X-18)/3;
	}
		
		
		
		//Create Enemies 
		for(i=0;i<18;i++){	// Initially, 18 enemies are alive. (3 rows of 6 enemies) 
			Enemy[i].points=10;
		if (i<6){	 										//1st row of 6 enemies
		Enemy[i].x = 20*i;
		Enemy[i].y = UPPER_HUD_SIZE+10;
		Enemy[i].image[0] = SmallEnemy10pointA;
		Enemy[i].image[1] = SmallEnemy10pointB;
		}//if	
		
			if(i>5 && i<12){            //2nd row of 6 enemies
		Enemy[i].x = 20*(i-6);
		Enemy[i].y = UPPER_HUD_SIZE+20;
		Enemy[i].image[0] = BlackEnemy;    //level 1: 2nd row of enemies start off black 
		Enemy[i].image[1] = BlackEnemy;		
			}
			
					 if(i>=12 && i<18){       //3rd row of 6 enemies
		Enemy[i].x = 20*(i-12);
		Enemy[i].y = UPPER_HUD_SIZE+30;
		Enemy[i].image[0] = BlackEnemy;		//level 1: 3nd row of enemies start off black 
		Enemy[i].image[1] = BlackEnemy;
							 
	}
			Enemy[i].vx = 0;	// not moving
			Enemy[i].vy = 0;	// not moving
			
			
			Enemy[i].black = BlackEnemy;   // can be used to cover the previous position of a sprite. Use only if necessary.
			Enemy[i].life = alive;
			Enemy[i].w = 16;
		  Enemy[i].h = 10;
			Enemy[i].needDraw = 1;
}	
		
		for(i=0;i<18;i++){
	Enemy[i].vx=1;
}
			
		// Create Spaceship
		SpaceShip.x = SPACESHIP_START_X;;
		SpaceShip.y = SPACESHIP_START_Y;
		SpaceShip.vx = 1;
		SpaceShip.vy = 0;	// not moving
		SpaceShip.image[0] = PlayerShip0;
		SpaceShip.image[1] = PlayerShip0;			
		SpaceShip.black = BlackEnemy;
		SpaceShip.life = alive;
		SpaceShip.w = 18;
		SpaceShip.h = 8;
		SpaceShip.needDraw = 1;		
		
}// GameInit
	
	int i;
	//shoot function
	void Shoot(void){
	i++;
	spaceshiplaser.x = SpaceShip.x + SpaceShip.w/2;
	spaceshiplaser.y = SpaceShip.y;
	spaceshiplaser.life = alive;
	spaceshiplaser.needDraw = 1;
	
}
	
		
	// Executed by interrupt.
	void GameMove(void){
	Anyalive = 0;
	
//move SpaceShip
moveSpaceShip();

//move Enemys
moveEnemys();
		
//move Missiles
moveMissiles();
		
//process rising edge of button (systick interrupt too fast)		
processbutton();
		
//move lasers		
moveLasers();

		
//display Hud with score lives and level (highscore not functional)
Hud_Ingame(score,highscore,lives,level);
		
	
}// GameMove
	
	
	
	
	unsigned long FrameCount = 0;	// used for animation of a sprite.
	
	// Called in the main program 
	void GameDraw(void){
//		int i;	
		
		// Draw enemies
		drawEnemys();
		
		// Draw spaceship
		ST7735_DrawBitmap(SpaceShip.x, SpaceShip.y, SpaceShip.image[FrameCount], SpaceShip.w, SpaceShip.h);
	
//drawLasers
			drawLasers();
			
// drawmMissiles
			drawMissiles();
// draw black above the lower hud to cover enemy missiles
			int x1,y1;
		x1 = MIN_X;
		y1 = MAX_Y-LOWER_HUD_SIZE;		
	ST7735_FillRect(x1, y1, 127, 5,0x0000);
	
	//drawBunkers
		drawBunkers();

	
		FrameCount = (FrameCount+1)&0x01; // 0,1,0,1,... Used for animation to switch between 2 images.
}// GameDraw
	


	
	//GAME TASK IS NOT USED
	// Background thread. Called at 30 Hz by a timer interrupt.
	// For the present, no need for interrupts; call it from main().
void GameTask(void){
		// read buttons
		// playsound
		// read slide potentiometer
		GameMove();
		// check for collision
		Flag = 1;		
}//GameTask

	void SysTick_Init(unsigned long period){
  NVIC_ST_CTRL_R = 0;         // disable SysTick during setup
  NVIC_ST_RELOAD_R = period-1;// reload value
  NVIC_ST_CURRENT_R = 0;      // any write to current clears it
  NVIC_SYS_PRI3_R = (NVIC_SYS_PRI3_R&0x00FFFFFF)|0x40000000; // priority 2
                              // enable SysTick with core clock and interrupts
  NVIC_ST_CTRL_R = 0x07;
  EnableInterrupts();	// make I = 0
}
	
/////////////////////////////////////SYSTICK HANDLER////////////////////////////////////////////////////////////////////
	void SysTick_Handler(void){
		DisableInterrupts();
		counter++;  						//counter is not used

  result = ADC0_InSeq3();		//read slide potentiometer
	button_logic(); 					//read button
			
		if (rising_edge && spaceshiplaser.life==dead){
			Shoot(); 		// shoot only one at a time
		}
	
score=enemyHit(score);   	//hit detection for enemies being hit by players only (functional)
//lives=playerHit(lives);  //hit detection for Players being hit by enemies (not functionAL)
	
		GameDraw();
		GameMove();

		//condtions for level 2
	if (done==0){
	if (Enemy[0].image[0]==BlackEnemy && Enemy [1].image[0]==BlackEnemy && Enemy[2].image[0]==BlackEnemy && Enemy[3].image[0]==BlackEnemy && Enemy[4].image[0]==BlackEnemy && Enemy [5].image[0]==BlackEnemy){
		game_status=NextLevel;
	}
}
	//conditions for level 3
if(done==1){
	 if(Enemy[0].image[0]==BlackEnemy && Enemy [1].image[0]==BlackEnemy && Enemy[2].image[0]==BlackEnemy && Enemy[3].image[0]==BlackEnemy && Enemy[4].image[0]==BlackEnemy && Enemy [5].image[0]==BlackEnemy && Enemy[6].image[0]==BlackEnemy && Enemy[7].image[0]==BlackEnemy && Enemy[8].image[0]==BlackEnemy && Enemy[9].image[0]==BlackEnemy && Enemy[10].image[0]==BlackEnemy && Enemy[11].image[0]==BlackEnemy){
 game_status=NextLevel;
	done2=0;
		}
	}

	//LEVEL 2 transition
			if (game_status==NextLevel && done==0){
			level=2;
			Menu_Clear();
			Player_Ship_Clear();
			done=1;
			ibound=12;
				
				for (i=0; i<ibound;i++){
					
					Enemy[i].image[0]=MedEnemy20pointA;
					Enemy[i].image[1]=MedEnemy20pointB;
					Enemy[i].points=20;
				}
				game_status=Running;
		}
	

//LEVEL 3 transition
if (done2==0){
			level=3;
			Menu_Clear();
			Player_Ship_Clear();
			done2=1;
			ibound=18;
			done=3;
			for (i=0; i<ibound;i++){
					Enemy[i].image[0]=LargeEnemy30pointA;
					Enemy[i].image[1]=LargeEnemy30pointB;
					Enemy[i].points=30;
				}
			game_status=Running;
}	
//condtions for game over
if (done2==1){
 if(Enemy[0].image[0]==BlackEnemy && Enemy [1].image[0]==BlackEnemy && Enemy[2].image[0]==BlackEnemy && Enemy[3].image[0]==BlackEnemy && Enemy[4].image[0]==BlackEnemy && Enemy [5].image[0]==BlackEnemy && Enemy[6].image[0]==BlackEnemy && Enemy[7].image[0]==BlackEnemy && Enemy[8].image[0]==BlackEnemy && Enemy[9].image[0]==BlackEnemy && Enemy[10].image[0]==BlackEnemy && Enemy[11].image[0]==BlackEnemy && Enemy[12].image[0]==BlackEnemy && Enemy[13].image[0]==BlackEnemy && Enemy[14].image[0]==BlackEnemy && Enemy[15].image[0]==BlackEnemy && Enemy[16].image[0]==BlackEnemy && Enemy[17].image[0]==BlackEnemy){
	game_status=GameOver;
 DisableInterrupts();
	}
}
//GAME OVER TRANSITION SCREEN
	if (game_status==GameOver){
		for (int i=0; i<3; i++){
		  ST7735_FillScreen(0x07E0);
			DelayWait10ms(1);
		  ST7735_FillScreen(0x0000);
		}
			while (button_input==0){
			 if (done3==0){
			ST7735_FillScreen(0x07E0);
			DelayWait10ms(100);
			 }
			done3=1;
			DisableInterrupts();
			button_logic();
			//Hud_Clear_Lower();
			//Hud_Clear_Upper();	
			//Hud_Clear_All2();
			//Player_Ship_Clear2();
			GameOver_Menu_Win(score);
			DelayWait10ms(100);
			//ST7735_FillScreen(0x07E0);
			}
			//back to level 1
			ST7735_FillScreen(0x0000);
			game_status=Running;
			ibound=6;
			done=0;
			done2=1;
			score=0;
			level=1;
			GameInit();
			Menu_Clear();
						
		}	
 

EnableInterrupts();
			
		game_proc_finished = 1; 						//set interrupt semaphore
		 
		 
		 
}//SysTick Handler
///////////////////////////////////////////////END OF SysTick Handler//////////////////////////////////////////////////////////////////////

int enemyHit(int score){

	if (spaceshiplaser.life == alive){
		int rectx = spaceshiplaser.x; //x-co
		int recty = spaceshiplaser.y; //y-co
		int rectw = spaceshiplaser.w; //width
		int recth = spaceshiplaser.h; //height
	
		int rectxw = rectx + rectw;		//box
		int rectyh = recty + recth;		//box
//		int row=0;
		
	
	//	unsigned int i = row-1;
		
		for (i = 0; i < ibound; i++)
		{
				enemy_t Enemypos = Enemy[i];
				int posx = Enemypos.x;
				int posy = Enemypos.y;
				int posw = Enemypos.w;
				int posh = Enemypos.h;
				int posxw = posx + posw;
				int posyh = posy + posh;
			
				if ((posx <= rectx)&&(posxw >= rectxw))
				{
					if (posyh >= rectyh)
					{ 
						if (Enemy[i].hits < 10)
						{
							if (Enemy[i].image[0]==SmallEnemy10pointA||Enemy[i].image[0]==MedEnemy20pointA || Enemy[i].image[0]==LargeEnemy30pointA) {
							score=score+Enemy[i].points;
						  //spaceshiplaser.w = 16;
							//spaceshiplaser.h = 10;
							//spaceshiplaser.image[0]=SmallExplosion0;
							//spaceshiplaser.image[1]=SmallExplosion1;
							
							justhit=1;
							spaceshiplaser.life = dead;
							
							Enemy[i].life = alive;
								
		
			
							Enemy[i].image[0] = BlackEnemy;
						  Enemy[i].image[1] = BlackEnemy;
							Enemy[i].hits++;
						
								
						}
					 } // if hit -> score
					}
						
				} // if position reaches rectangle
					
				}	// left and right bound of screen
					
			
		} 
return score;
}

	

void Player_Ship_Clear(void)
{
	ST7735_FillRect(MIN_X, SpaceShip.y-SpaceShip.h+1, MAX_X, SpaceShip.h, BLACK);
}

//int playerHit(int lives)
//{
//	for (i=0; i<9; i++){
//	
//		// hit detect for enemy weapons hitting bunkers, spaceship, and floor
//	if (Missile[i].needDraw==1)
//	{
//		
//		
//		int missile_x0, missile_y0, missile_x1, missile_y1, missile_x2, missile_y2, missile_x3, missile_y3, missile_x4, missile_x5, missile_x6, missile_y6,missile_x7,missile_x8 ;
//		int m0x,m0y,m1x,m1y,m2x,m2y,m3x,m3y, m4x,m5x,m6x, m6y, m7x, m8x;
//	
//		if (Missile[i].life == alive)
//		{
//			missile_x0 = Missile[0].x;
//			missile_y0 = Missile[0].y;
//			
//			missile_x1 = Missile[1].x;
//			missile_y1 = Missile[1].y;
//			
//	    missile_x2 = Missile[2].x;
//			missile_y2 = Missile[2].y;
//			
//		
//			missile_x3 = Missile[3].x;
//			missile_y3 = Missile[3].y;
//	
//			missile_x4 = Missile[4].x;
//	

//			missile_x5 = Missile[5].x;

//			missile_x6 = Missile[6].x;
//			missile_y6 = Missile[6].y;
//			
//			missile_x7 = Missile[7].x;
//		
//	    missile_x8 = Missile[8].x;
//	
//			m0x = missile_x0 + Missile[i].w;
//			m1x = missile_x1 + Missile[i].w;
//			m2x = missile_x2 + Missile[i].w;
//			m3x = missile_x3 + Missile[i].w;
//			m4x = missile_x4 + Missile[i].w;
//			m5x = missile_x5 + Missile[i].w;
//			m6x = missile_x6 + Missile[i].w;
//			m7x = missile_x7 + Missile[i].w;
//			m8x = missile_x8 + Missile[i].w;
//			
//			m0y = missile_y0 + Missile[i].h;
//			m1y = missile_y1 + Missile[i].h;
//			m2y = missile_y2 + Missile[i].h;
//			
//			m3y = missile_y3 + Missile[i].h;
//			m6y = missile_y6 + Missile[i].h;
//		}
//	
//	if (SpaceShip.life == alive)
//		{
//			if ((SpaceShip.x <= missile_x0) && (SpaceShip.x+SpaceShip.w >= m0x))
//				{
//			//	if (round2==1){
//				///if ((SpaceShip.x <= missile_x3|| missile_x4|| missile_x5 && (SpaceShip.x+SpaceShip.w >= m3x || m4x ||m5x)
//				//}
//			
//							if (SpaceShip.y+SpaceShip.h == m0y)
//								{
//					// if lives = 1 and just got hit, set game status sephmore to gameover
//					//if (lives == 1)
//						//game_status = GameOver;
//					// if lives > 1, dec lives
//							if (lives > 1)
//									lives--;
//								}
//				}

//						
//					} //if alive (not needed)		
//			}//if need draw	
//		}//for
//}//Player hit


//void hitDetection(int score)
//{
//	enemyHit(score);
//	playerHit();
//}
	
int main(void){
	//unsigned long static m=0;
	DisableInterrupts();
	PLL_Init();
	Random_Init(1);	// Initialize to obtain random numbers.
	Output_Init();
	GameInit();
  ST7735_FillScreen(0x0000);	// set screen to black
	Button_Init_portE();
	Hud_Menu(score,highscore);
	
		while (button_input==0){
	//GameOver_Menu();
	Menu(Running);
	DelayWait10ms(30);
	button_logic();
	}
	ST7735_FillScreen(0x0000);	// set screen to black
  //ST7735_FillScreen(0x001F);	// set screen to white
	ADC0_InitSWTriggerSeq3_Ch1();	
//	Flag = 1;
	
	SysTick_Init(sixty_Hz);

	//EnableInterrupts();
	do{
		
		if (game_status==NextLevel){
			level++;
			Menu_Clear();
			Player_Ship_Clear();
		}
		
		//while(Flag==0){}
		//Flag = 0;
	//	DelayWait10ms(50000);
	
	//	m = Random32()%60; // returns a random number from 0 to 59
		//m = Random32()%10; // returns a random number from 0 to 9
		//m = Random(); // returns a random number from 0 to 255
	//	GameTask();
	if (game_proc_finished==1){
		
//			GameDraw();
			//GameTask();
			GameMove();
	}
	game_proc_finished = 0;				//CLEAR interrupt semaphore
}
	while(Anyalive);
	DelayWait10ms(15);
}//main

// Subroutine to wait 10 msec
// Inputs: None
// Outputs: None
// Notes: ...
void DelayWait10ms(uint32_t n){uint32_t volatile time;
  while(n){
    time = 727240*2/91;  // 10msec
    while(time){
      time--;
    }
    n--;
  }
}

void Button_Init_portE(void)
{
	SYSCTL_RCGCGPIO_R |= 0x10;     // 1) activate clock for Port E
	while((SYSCTL_PRGPIO_R&0x10) == 0){}; // allow time for clock to start. Wait for status bit to be true
	GPIO_PORTE_DIR_R &= ~0x01;   // make PE0 input
	GPIO_PORTE_AFSEL_R &= ~0x01;// disable alt funct on PE0
	GPIO_PORTE_DEN_R |= 0x01;   // enable digital I/O on PE0
	GPIO_PORTE_PCTL_R = (GPIO_PORTE_PCTL_R&0x001FFFF0)+0x00000000;  // configure PE0 as GPIO
	GPIO_PORTE_AMSEL_R &= ~0X01;     // disable analog functionality on PB7-0
		
}

int button_logic(void)
{
	button_input=(GPIO_PORTE_DATA_R&0x01);
	return button_input;
}




void processbutton(void){
	
	b=a;
	
		if (button_input==1){
		a=1;
	}
	else{
		a=0;
	}
	if (b==0 && button_input==1){
		rising_edge=1;
	}
	else{
	rising_edge=0;
	}
	
}






void moveSpaceShip(void){
			// Move spaceship
       int slide = slide_meter();

       if (slide == left && SpaceShip.x > 1){
           SpaceShip.x = SpaceShip.x -2; // change the velocity to opposite direction
      }
       else if (slide == right && SpaceShip.x < 110){
            SpaceShip.x =  SpaceShip.x +2; // change the velocity to opposite direction
      }
	
}



void moveLasers(void){
	
		if (spaceshiplaser.life == alive){
			spaceshiplaser.needDraw = 1;
	
		if(spaceshiplaser.y < UPPER_HUD_BOTTOM_RIGHT_Y + spaceshiplaser.h)
			spaceshiplaser.life = dead;
		spaceshiplaser.y += spaceshiplaser.vy;
			
		}


}

void moveMissiles(void){
			/////////////////////////////////////MOVE ENEMY Missile[i]S//////////////////////////////////////////////////////////
		for(i=0;i<3;i++)
		{
			if(Missile[i].life == alive){
				Missile[i].needDraw = 1;
				Anyalive = 1;
				
			if (Missile[i].y > LOWER_HUD_BOTTOM_RIGHT_Y + Missile[i].h-20)
						Missile[i].life = dead;								
				
	
	
				//Missile[i].x += Missile[i].vx;
				Missile[i].y += Missile[i].vy;
				round1=1;
			}// if
		}// for		
		
	//	DelayWait10ms(1);
		
		if (Missile[0].y>140){
				for(i=3;i<6;i++)
		{
		
			if(Missile[i].life == alive){
				Missile[i].needDraw = 1;
				Anyalive = 1;
								
				if (Missile[i].y > LOWER_HUD_BOTTOM_RIGHT_Y + Missile[i].h-20)
			Missile[i].life = dead;			
		
			
				Missile[i].x += Missile[i].vx;
				Missile[i].y += Missile[i].vy;
			round1=0;
			round2=1;
					}// if
			}// for		
		
	}// if Missile[i][0].y>140
		
	
	
			if (Missile[3].y>140){
				for(i=6;i<9;i++)
		{
			if(Missile[i].life == alive){
				Missile[i].needDraw = 1;
				Anyalive = 1;
								
		if (Missile[i].y > LOWER_HUD_BOTTOM_RIGHT_Y + Missile[i].h-20)
			Missile[i].life = dead;	
		
		
				if((Missile[i].x < 0) || (Missile[i].x > 120)){
					Missile[i].life = dead;
				}				
				Missile[i].x += Missile[i].vx;
				Missile[i].y += Missile[i].vy;
					round2=0;
					round3=1;
					}// if
			}// for		
		
	}// if Missile[i][3].y>140
	

}


void moveEnemys(void){
	int i,j;
	int changey;
	int changex;
		// Move enemies
		for(i=0;i<18;i++){
			
	
			
			// if enemies make to the bunkers, set game status semaphore to gameover
				if((Enemy[i].y > BUNKER_START_Y-BUNKER_H) || (Enemy[i].y < UPPER_HUD_TOP_LEFT_Y)){
					game_status = GameOver;
				}		

			
			if(Enemy[i].life == alive && SpaceShip.life==alive){
				Enemy[i].needDraw = 1;
				SpaceShip.needDraw=1;
				Anyalive = 1;
				Enemy[i].x += Enemy[i].vx;
				Enemy[i].y += Enemy[i].vy;
								
				// Amer Qouneh's code for this structure is clearer than Valvano's.
				// Only one of those conditions is executed and control exits.
				if((Enemy[i].y > 140) || (Enemy[i].y < 10)){
					Enemy[i].life = dead;
				}				
				if((Enemy[i].x < 0) || (Enemy[i].x > 120)){
					Enemy[i].life = dead;
				}				
//////////////////////////////////change x of enemies/////////////////////////////////////////////////////////////////////////////////////////////////////////////////
				changex=1;
			if (changex==1){

		
		if (((Enemy[17].x>112)||(Enemy[0].x==0))&&(i==17)){     //switch x direction IF (either the 1st enemy hits the left edge of screen OR last enemy hits the right edge of screen) AND (the last enemy has been moved)--> (i=17)
			for(j=0;j<18;j++){
				
		Enemy[j].vx = -1*Enemy[j].vx;  												 //change x direction (sprites move in the opposite direction that they were moving in before)
		Enemy[j].vy = 0;								
			}//j loop for all enemys
		}// if enemy17
		changey=1; 																							//value used to start changing y direction instead of x 
		changex=0;
	}//if changex==1

	
	//////////////////////////////change y of enemies//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
		if (changey==1){
			if ((Enemy[0].x==0)&&(i==17)){           						// switch y direction IF (the 1st enemy hits the left edge of the screen) AND (the last enemy has been moved)--> (i=17)
			for(j=0;j<18;j++){
				
					previous=Enemy[i].y;
		Enemy[j].vy = 1;    																	//change y direction (sprites move down the screen from the top)

			}//j loop for all enemys
		}// if enemy17
			
		if ((Enemy[0].y==40)&&(i==17)){   										// switch y direction IF (the 1st enemy hits y coordinate of 20) AND (the last enemy has been moved)--> (i=17)
			for(j=0;j<18;j++){
				
				previous=Enemy[i].y;
		Enemy[j].vy = -1;									       						 //change y direction (sprites move UP the screen from y=20)

			}//j loop for all enemys
		}// if enemy17

		
			changex=1;																					//value used to start changing x direction instead of y 
			changey=0;
		
						}//if changey==1
				
			}// if enemy alive
			
		}// for	
}




void drawEnemys(void){
			// Draw enemies
		for(int i=0;i<18;i++){
			if(Enemy[i].needDraw){
				if(Enemy[i].life == alive){
					
					//drawing the small explosion if the enemy is hit
							if ((Enemy[i].hits>=1 && Enemy[i].image[0]==BlackEnemy && justhit==1)){
			
			ST7735_DrawBitmap(Enemy[i].x, Enemy[i].y, SmallExplosion0, Enemy[i].w, Enemy[i].h);
			DelayWait10ms(10);
			ST7735_DrawBitmap(Enemy[i].x, Enemy[i].y, SmallExplosion1, Enemy[i].w, Enemy[i].h);
			justhit=0;
	    Enemy[i].hits--;
		}				
							//then draw the enemy black
					ST7735_DrawBitmap(Enemy[i].x, Enemy[i].y, Enemy[i].image[FrameCount], Enemy[i].w, Enemy[i].h);
				}else{
					//ST7735_DrawBitmap(Enemy[i].x, Enemy[i].y, Enemy[i].black, Enemy[i].w, Enemy[i].h);	// We can eliminate this line if we don't want to use Black enemies.					
				}
				Enemy[i].needDraw = 0;	// clear the semaphore				
			}// if
		}// for		
	
	
}
void drawLasers(void){
	if (spaceshiplaser.needDraw)	
					{	
							if(spaceshiplaser.life == alive)
						{
							ST7735_DrawBitmap(spaceshiplaser.x, spaceshiplaser.y, spaceshiplaser.image[0], spaceshiplaser.w, spaceshiplaser.h);
							ST7735_FillRect(spaceshiplaser.x, spaceshiplaser.y, spaceshiplaser.w, 20,0x0000);	
				
						}
				else
						{
							ST7735_DrawBitmap(spaceshiplaser.x, spaceshiplaser.y, spaceshiplaser.black, spaceshiplaser.w, spaceshiplaser.h);
						}
							spaceshiplaser.needDraw =0; // clear semaphore
					}

}

void drawBunkers(void){

	//draw bunker
		for (int i = 0; i < 3; i++)
		{
			if (Bunker[i].needDraw)
			{
				if (Bunker[i].life == alive)
				{

					if (Bunker[i].im_ch)
					{
						ST7735_DrawBitmap(Bunker[i].x, Bunker[i].y, Bunker[i].image[Bunker[i].hits], Bunker[i].w, Bunker[i].h);
						Bunker[i].im_ch = 0;
					}
					else
					{
						ST7735_DrawBitmap(Bunker[i].x, Bunker[i].y, Bunker[i].image[Bunker[i].hits], Bunker[i].w, Bunker[i].h);
					}
				}
				else
				{
					ST7735_DrawBitmap(Bunker[i].x, Bunker[i].y, Bunker[i].black, Bunker[i].w, Bunker[i].h);
					Bunker[i].needDraw = 0;
				}
				//Bunker[i].needDraw = 0;
			}
		}
}



void drawMissiles(void){
	
	
			for(i=0;i<3;i++){
			if(Missile[i].needDraw){
				if(Missile[i].life == alive){
					ST7735_DrawBitmap(Missile[i].x, Missile[i].y, Missile[i].image[0], Missile[i].w, Missile[i].h);	// with animation for sprite
					ST7735_FillRect(Missile[i].x, Missile[i].y-20, Missile[i].w, 15,0x0000);	
				}else{
					ST7735_DrawBitmap(Missile[i].x, Missile[i].y, Missile[i].black, Missile[i].w, Missile[i].h);					
				}
				Missile[i].needDraw = 0;	// clear the semaphore				
			}// if
		}// for
		
		if (Missile[0].y>140){
	
				for(i=3;i<6;i++){
			if(Missile[i].needDraw){
				if(Missile[i].life == alive){
					ST7735_DrawBitmap(Missile[i].x, Missile[i].y, Missile[i].image[0], Missile[i].w, Missile[i].h);	// with animation for sprite
					ST7735_FillRect(Missile[i].x, Missile[i].y-20, Missile[i].w, 15,0x0000);	
				}else{
					ST7735_DrawBitmap(Missile[i].x, Missile[i].y, Missile[i].black, Missile[i].w, Missile[i].h);					
				}
				Missile[i].needDraw = 0;	// clear the semaphore				
			}// if
		}// for
		
	}
	
			if (Missile[3].y>140){
	
				for(i=6;i<9;i++){
			if(Missile[i].needDraw){
				if(Missile[i].life == alive){
					ST7735_DrawBitmap(Missile[i].x, Missile[i].y, Missile[i].image[0], Missile[i].w, Missile[i].h);	// with animation for sprite
					ST7735_FillRect(Missile[i].x, Missile[i].y-20, Missile[i].w, 15,0x0000);	
				}else{
					ST7735_DrawBitmap(Missile[i].x, Missile[i].y, Missile[i].black, Missile[i].w, Missile[i].h);					
				}
				Missile[i].needDraw = 0;	// clear the semaphore				
			}// if
		}// for
		
	}
	
} //move missiles

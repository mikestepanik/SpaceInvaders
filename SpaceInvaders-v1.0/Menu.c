// Menu.c

// Draws text for start and gameover menus


#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#include "Sprite.h"
#include "display_consts.h"
#include "Menu.h"
#include "ST7735.h"
//#include "joystick.h"
#include "display_funcs.h"
//#include "game_vars.h"
#include "game_consts.h"


void Menu(int style)
{
	switch(style)
	{
		case Running:
			Start_Menu();
			break;
		case GameOver:
			GameOver_Menu();
			break;
	}
}
int done1=0;
int g=0;

void Start_Menu(void)
{

	char start_txt1[] = "PLAY";
	char start_txt2[] = "SPACE INVADERS!";
	char start_txt3[] = "(Hold to start)";
	if (done1==0){
	int x = MID_X - (strlen(start_txt1)*W_S)/2;
	int y = MENU_TOP_LEFT_Y + W_H;
	
	Draw_White_Text_Delayed_10ms(x, y, start_txt1, 35);

	x = MID_X - (strlen(start_txt2)*W_S)/2;
	y = MENU_TOP_LEFT_Y + 3*W_H;
	
	Draw_White_Text_Delayed_10ms(x, y, start_txt2, 35);
	}
	done1=1;
int	x = MID_X - (strlen(start_txt2)*W_S)/2;
int	y = MENU_TOP_LEFT_Y + 6*W_H;
	
	uint16_t color[4]={RED, BLUE, GREEN, RED};
	Draw_Text_Colorful(x, y, start_txt3,color,1);
	Delay_10ms(100);
	ST7735_FillRect(x, y, 90, 10, BLACK);
	Draw_Text_Colorful(x, y, start_txt3,color,1);

}
	


void GameOver_Menu(void)
{
	
	//Menu_Clear();
	//SubMenu_Clear();
	
	char gameover_txt1[] = "GAME OVER";
	char gameover_txt2[] = "(Hold to start)";
	
	int x = MID_X - (strlen(gameover_txt1)*W_S)/2;
	int y = MENU_TOP_LEFT_Y + 3*W_H;
	
	Draw_Text(x, y, gameover_txt1, WHITE, 1);
	
	x = MID_X - (strlen(gameover_txt2)*W_S)/2;
	y = MENU_TOP_LEFT_Y + 6*W_H;
	
	Draw_Text(x, y, gameover_txt2, WHITE, 1);
	
}
int done4=0;
//void Draw_Black_Text_Delayed_10ms(int x, int y, char *txt, uint16_t color, int size, int delay)

void GameOver_Menu_Win(int score)
{
	
	//Menu_Clear();
	//SubMenu_Clear();
	
	char gameover_txt1[] = "YOU WON!";
	char gameover_txt3[] = "Your score was:";
	char gameover_txt2[] = "(Hold to Restart)";
	if (done4==0){
	int x = ((MID_X - (strlen(gameover_txt1)*W_S))/2)+10;
	int y = MENU_TOP_LEFT_Y + 3*W_H;
	
	Draw_Black_Text_Delayed_10ms1(x, y, gameover_txt1, BLACK, 2, 35);
	}
	int x,y;
	done4=1;
	x = MID_X - (strlen(gameover_txt2)*W_S)/2;
	y = MENU_TOP_LEFT_Y + 6*W_H;
	
	Draw_Text(x, y, gameover_txt2, BLACK, 1);

	int x1,y1,x2,y2;
	
	y1 = MENU_TOP_LEFT_Y + 9*W_H;
	x1 = (MID_X-10) - (strlen(gameover_txt3)*W_S)/2;
	
	Draw_Black_Text_Delayed_10ms(x1, y1, gameover_txt3, BLACK, 1,35);
	
	char score_val[4];
	sprintf(score_val, "%d", score);

	y2 = MENU_TOP_LEFT_Y + 9*W_H;
	x2 = (MID_X - (strlen(gameover_txt3)*W_S)/2)+80;

	Draw_Black_Text_Delayed_10ms(x2, y2, score_val,BLACK,1,35);
	Delay_10ms(50);
	ST7735_FillRect(x, y, 127, strlen(gameover_txt3),0x07E0);
	Delay_10ms(50);
	//Draw_Black_Text_Delayed_10ms(x1, y1, gameover_txt3,GREEN,1,1);
	//Draw_Black_Text_Delayed_10ms(x2, y2, score_val,GREEN,1,1);
	ST7735_FillRect(x, y, 127, strlen(gameover_txt2),0x07E0);
	
	
}





void GameOver_Menu_Loss(int score)
{
	
	//Menu_Clear();
	//SubMenu_Clear();
	
	char gameover_txt1[] = "GAME OVER";
	char gameover_txt2[] = "(Hold to Restart)";
	
	int x = MID_X - (strlen(gameover_txt1)*W_S)/2;
	int y = MENU_TOP_LEFT_Y + 3*W_H;
	
	Draw_Text(x, y, gameover_txt1, WHITE, 1);
	
	x = MID_X - (strlen(gameover_txt2)*W_S)/2;
	y = MENU_TOP_LEFT_Y + 6*W_H;
	
	Draw_Text(x, y, gameover_txt2, WHITE, 1);
	
}

















void SubMenu(int style)
{
	// todo
}

//void Player_Ship_Clear(void)
//{
//	ST7735_FillRect(MIN_X, SpaceShip.y-SpaceShip.h+1, MAX_X, SpaceShip.h, BLACK);
//}

void Text_Delay_10ms(uint32_t n)
{
	uint32_t volatile time;
  while(n){
    time = 727240*2/91;  // 10msec
    while(time){
      time--;
    }
    n--;
  }
}

void Menu_Clear(void)
{
	Clear(M);
}

void SubMenu_Clear(void)
{
	Clear(SM);
}

#include <string.h>

#include "display_funcs.h"
#include "display_consts.h"
#include "ST7735.h"

void Draw_Text(int x, int y, char *txt, uint16_t color, int size)
{
	for (int i = 0; i < strlen(txt); i++)
	{
		ST7735_DrawCharS(x, y, txt[i], color, BLACK, size);
		x += W_S*size;
	}
}




void Draw_Black_Text(int x, int y, char *txt, uint16_t color, int size)
{
	for (int i = 0; i < strlen(txt); i++)
	{
		ST7735_DrawCharS(x, y, txt[i], color, WHITE, size);
		x += W_S*size;
	}
}


void Draw_Text_Delayed_10ms(int x, int y, char *txt, uint16_t color, int size, int delay)
{
	for (int i = 0; i < strlen(txt); i++)
	{
		ST7735_DrawCharS(x, y, txt[i], color, BLACK, size);
		x += W_S;
		Delay_10ms(delay);
	}
}




void Draw_Black_Text_Delayed_10ms(int x, int y, char *txt, uint16_t color, int size, int delay)
{
	for (int i = 0; i < strlen(txt); i++)
	{
		ST7735_DrawCharS(x, y, txt[i], color, GREEN, size);
		x += W_S;
		Delay_10ms(delay);
	}
}




void Draw_Black_Text_Delayed_10ms1(int x, int y, char *txt, uint16_t color, int size, int delay)
{
	for (int i = 0; i < strlen(txt); i++)
	{
		ST7735_DrawCharS(x, y, txt[i], color, GREEN, size);
		x += 12;
		Delay_10ms(delay);
	}
}






void Draw_White_Text(int x, int y, char *txt)
{
	Draw_Text(x, y, txt, WHITE, 1);
}

void Draw_Black1_Text(int x, int y, char *txt)
{
	Draw_Text(x, y, txt, BLACK, 1);
}


int z=0;
uint16_t color[4]={RED, BLUE, GREEN, RED};
void Draw_Text_Colorful(int x, int y, char *txt, uint16_t color[z], int size){
	for (int i = 0; i < strlen(txt); i++)
	{
		z++;
		if (z>3){
			z=1;
		}
		ST7735_DrawCharS(x, y, txt[i], color[z], BLACK, size);
		Delay_10ms(25);
		x += W_S*size;
	
	}
}





void Draw_White_Text_Delayed_10ms(int x, int y, char *txt, int delay)
{
	Draw_Text_Delayed_10ms(x, y, txt, WHITE, 1, delay);
}

void Clear(int area)
{
	
	int x = 0, y = 0, w = 0, l = 0;
	
	switch (area)
	{
		case UH: // Upper Hud
				x = UPPER_HUD_TOP_LEFT_X;
				y = UPPER_HUD_TOP_LEFT_Y;
				w = UPPER_HUD_BOTTOM_RIGHT_X - UPPER_HUD_TOP_LEFT_X;
				l = UPPER_HUD_BOTTOM_RIGHT_Y - UPPER_HUD_TOP_LEFT_Y;
				break;
		case LH: // Lower Hud
				x = LOWER_HUD_TOP_LEFT_X;
				y = LOWER_HUD_TOP_LEFT_Y;
				w = LOWER_HUD_BOTTOM_RIGHT_X - LOWER_HUD_TOP_LEFT_X;
				l = LOWER_HUD_BOTTOM_RIGHT_Y - LOWER_HUD_TOP_LEFT_Y;
				break;
		case M: // Menu
				x = MENU_TOP_LEFT_X;
				y = MENU_TOP_LEFT_Y;
				w = MENU_BOTTOM_RIGHT_X - MENU_TOP_LEFT_X;
				l = MENU_BOTTOM_RIGHT_Y - MENU_TOP_LEFT_Y;
				break;
		case SM: // Submenu
				x = SUBMENU_TOP_LEFT_X;
				y = SUBMENU_TOP_LEFT_Y;
				w = SUBMENU_BOTTOM_RIGHT_X - SUBMENU_TOP_LEFT_X;
				l = SUBMENU_BOTTOM_RIGHT_Y - SUBMENU_TOP_LEFT_Y;
				break;
	}
	ST7735_FillRect(x, y, w, l, BLACK);
}









void Clear2(int area)
{
	
	int x = 0, y = 0, w = 0, l = 0;
	
	switch (area)
	{
		case UH: // Upper Hud
				x = UPPER_HUD_TOP_LEFT_X;
				y = UPPER_HUD_TOP_LEFT_Y;
				w = UPPER_HUD_BOTTOM_RIGHT_X - UPPER_HUD_TOP_LEFT_X;
				l = UPPER_HUD_BOTTOM_RIGHT_Y - UPPER_HUD_TOP_LEFT_Y;
				break;
		case LH: // Lower Hud
				x = LOWER_HUD_TOP_LEFT_X;
				y = LOWER_HUD_TOP_LEFT_Y;
				w = LOWER_HUD_BOTTOM_RIGHT_X - LOWER_HUD_TOP_LEFT_X;
				l = LOWER_HUD_BOTTOM_RIGHT_Y - LOWER_HUD_TOP_LEFT_Y;
				break;
		case M: // Menu
				x = MENU_TOP_LEFT_X;
				y = MENU_TOP_LEFT_Y;
				w = MENU_BOTTOM_RIGHT_X - MENU_TOP_LEFT_X;
				l = MENU_BOTTOM_RIGHT_Y - MENU_TOP_LEFT_Y;
				break;
		case SM: // Submenu
				x = SUBMENU_TOP_LEFT_X;
				y = SUBMENU_TOP_LEFT_Y;
				w = SUBMENU_BOTTOM_RIGHT_X - SUBMENU_TOP_LEFT_X;
				l = SUBMENU_BOTTOM_RIGHT_Y - SUBMENU_TOP_LEFT_Y;
				break;
	}
	ST7735_FillRect(x, y, w, l, GREEN);
}





















void Delay_10ms(int delay)
{
	uint32_t volatile time;
  while(delay)
	{
    time = 727240*2/91;  // 10msec
    while(time)
		{
      time--;
    }
    delay--;
  }
}

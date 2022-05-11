#include "includes.h"
#include <stdlib.h>
#include  "app.h"


#define KEY1_INT_EXTI_LINE         EXTI_Line0
#define KEY2_INT_EXTI_LINE         EXTI_Line13
#define SNAKE_Max_Long 60



//snake struct
struct Snake{
	uint16_t X[SNAKE_Max_Long];
	uint16_t Y[SNAKE_Max_Long];
	u8 Long;//the size of snake
	u8 Life;//snake live flag
	u8 Direction;//the direction to move
}snake;

//food struct
struct Food{
	u8 X;//x position
	u8 Y;//y position
	u8 Yes;//need food flag
}food;

//game struct
struct Game{
	u16 Score;
	u8 Life;	
}game;



static int _getdirection(int pre_direction){
    int direction=0;
	GUI_PID_STATE state;
    GUI_PID_GetState(&state);
    if(state.Pressed){
       	if(state.x < 120){
            if(pre_direction == 1)
				direction = 3;
            else if (pre_direction == 3)
				direction = 2;
			else if (pre_direction==2)
				direction = 4;
			else
				direction = 1;
		}
		else {
			if(pre_direction == 1)
				direction = 4;
			else if (pre_direction == 4)
				direction = 2;
			else if (pre_direction==2)
				direction = 3;
			else
				direction = 1;
		}
		return direction;
	}
	else
		return pre_direction;
}

//framewin defination
static const GUI_WIDGET_CREATE_INFO _aDialogCreateSNAKE[] = {
  { FRAMEWIN_CreateIndirect, "SNAKE", 0, 0, 0, 240, 320, 0, 0x64, 0 },
};

//main function
static void _cbDialogSNAKE(WM_MESSAGE * pMsg) {
  	int i;	
	char buff[30];
	snake.Long=2;
	snake.Life=0;//snake alive
	snake.Direction=1;
	game.Score=0;
	game.Life=4;//game for 4 times
	food.Yes=1;//need food to appear
	//the initial position
	snake.X[0]=0;snake.Y[0]=50;
	snake.X[1]=10;snake.Y[1]=50;

	GUI_SetBkColor(GUI_WHITE);
  
	while(1){	
		while(1){
			GUI_Clear();
			GUI_DrawRect(0,0,240,270);
			//show the score
			sprintf(buff,"SCORE : %d",game.Score);
			GUI_DispStringHCenterAt(buff, LCD_GetXSize() / 2, 300);

			if(food.Yes==1){	
				food.X=rand()%220;
				food.Y=rand()%250;
				food.Yes=0;
			}
			//draw food
			if(food.Yes==0){
				GUI_SetColor(GUI_BLACK);
				GUI_FillRect(food.X,food.Y,food.X+10,food.Y+10);
			}
			//update snake length
			for(i=snake.Long-1;i>0;i--){
				snake.X[i]=snake.X[i-1];
				snake.Y[i]=snake.Y[i-1];
			}
			//update direction
			snake.Direction = _getdirection(snake.Direction);
			switch(snake.Direction){
				case 1:snake.X[0]+=10;break;
				case 2:snake.X[0]-=10;break;
				case 3:snake.Y[0]-=10;break;
				case 4:snake.Y[0]+=10;break;
			}
			//draw snake
			for(i=0;i<snake.Long;i++){
				GUI_SetColor(GUI_BLACK);
				GUI_FillRect(snake.X[i],snake.Y[i],snake.X[i] + 10,snake.Y[i] + 10);
			}
			GUI_Delay(200);//delay 200ms

			GUI_SetColor(GUI_WHITE);
			//hide the old snake part
			GUI_FillRect(snake.X[snake.Long-1],snake.Y[snake.Long-1],snake.X[snake.Long - 1] + 10,snake.Y[snake.Long - 1] + 10);

			GUI_SetColor(GUI_BLACK);
			//judage whether die or not
			if(snake.X[0]==0||snake.X[0]>200||snake.Y[0]==0||snake.Y[0]>260)
				snake.Life=1;//dead
			//judge the snake cllosion self
			for(i=3;i<snake.Long;i++){
				if(snake.X[i]==snake.X[0]&&snake.Y[i]==snake.Y[0])//????????????????????
				game.Life-=1;
			}
		
			if(snake.Life==1||game.Life==0){
				GUI_DispStringHCenterAt("GAME OVER!", LCD_GetXSize() / 2, LCD_GetYSize()/2);
				GUI_Delay(1000);
				GUI_Clear();
				break;
			}
			
			//snake eats the food
			if(snake.X[0]==food.X&&snake.Y[0]==food.Y){ 
				GUI_SetColor(GUI_WHITE);
				GUI_FillRect(food.X,food.Y,food.X+1,food.Y+1);
				GUI_SetColor(GUI_BLACK);
				snake.Long++;
				game.Score+=10;
				food.Yes=1;
			}
		}
	}
	

}

void FUN_ICON111Clicked(void)
{
	WM_HWIN hWin;
	OS_INFO("SNAKE create\n");
	hWin=GUI_CreateDialogBox(_aDialogCreateSNAKE, GUI_COUNTOF(_aDialogCreateSNAKE), _cbDialogSNAKE, WM_HBKWIN, 0, 0);
	

	//WM_BringToTop(hWin);
	

	while(Flag_ICON111)
	{					 				
		if(tpad_flag)WM_DeleteWindow(hWin);
    GUI_Delay(10); 				
	}
	
}

/*************************** End of file ****************************/



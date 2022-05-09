#include "includes.h"
#include <stdlib.h>
#include  "app.h"


#define KEY1_INT_EXTI_LINE         EXTI_Line0
#define KEY2_INT_EXTI_LINE         EXTI_Line13
#define SNAKE_Max_Long 60//??????



//????
struct Snake{
	uint16_t X[SNAKE_Max_Long];
	uint16_t Y[SNAKE_Max_Long];
	u8 Long;//????
	u8 Life;//????,0????,1????
	u8 Direction;//??????
}snake;

//?????
struct Food{
	u8 X;//?????
	u8 Y;//?????
	u8 Yes;//??????????,0????,1????
}food;

//??????
struct Game{
	u16 Score;//??
	u8 Life;//????	
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

//?????????????
static const GUI_WIDGET_CREATE_INFO _aDialogCreateSNAKE[] = {
  { FRAMEWIN_CreateIndirect, "SNAKE", 0, 0, 0, 240, 320, 0, 0x64, 0 },
};



static void _cbDialogSNAKE(WM_MESSAGE * pMsg) {
    
  int i;	
	snake.Long=2;//??????
	snake.Life=0;//????
	snake.Direction=1;//??????????
	game.Score=0;//?????0
	game.Life=4;//?????
	food.Yes=1;//???????
	snake.X[0]=0;snake.Y[0]=50;
	snake.X[1]=10;snake.Y[1]=50;

    // GUI_SetColor(GUI_BLACK);
		GUI_SetBkColor(GUI_WHITE);
  
	while(1){	
		while(1){
			GUI_Clear();
			GUI_DrawRect(0,10,240,270);
			if(food.Yes==1){
				//??????????	
				food.X=rand()%(190/10)*10;
				food.Y=rand()%(250/10)*10;
				food.Yes=0;
			}
			//????
			if(food.Yes==0){
				GUI_SetColor(GUI_BLACK);
				GUI_FillRect(food.X,food.Y,food.X+10,food.Y+10);
			}
			//???????
			for(i=snake.Long-1;i>0;i--){
				snake.X[i]=snake.X[i-1];
				snake.Y[i]=snake.Y[i-1];
			}
			snake.Direction = _getdirection(snake.Direction);
			switch(snake.Direction){
				case 1:snake.X[0]+=10;break;//??
				case 2:snake.X[0]-=10;break;//??
				case 3:snake.Y[0]-=10;break;//??
				case 4:snake.Y[0]+=10;break;//??
			}
			//??
			for(i=0;i<snake.Long;i++){
				GUI_SetColor(GUI_BLACK);
				GUI_FillRect(snake.X[i],snake.Y[i],snake.X[i] + 10,snake.Y[i] + 10);
			}
			GUI_Delay(500);//??100ms

			GUI_SetColor(GUI_WHITE);//?????????
			GUI_FillRect(snake.X[snake.Long-1],snake.Y[snake.Long-1],snake.X[snake.Long - 1] + 10,snake.Y[snake.Long - 1] + 10);//?????

			GUI_SetColor(GUI_BLACK);
			//??????
			if(snake.X[0]==0||snake.X[0]>200||snake.Y[0]==0||snake.Y[0]>260)
				snake.Life=1;//?????
			//??????????????????
			for(i=3;i<snake.Long;i++){
				if(snake.X[i]==snake.X[0]&&snake.Y[i]==snake.Y[0])//????????????????????
				game.Life-=1;
			}
			

			
			//??????????????????,????
			if(snake.Life==1||game.Life==0){
				GUI_DispStringHCenterAt("GAME OVER!", LCD_GetXSize() / 2, LCD_GetYSize()/2);
				GUI_Delay(2000);
				GUI_Clear();
				break;
			}
			
			//??????????
			if(snake.X[0]==food.X&&snake.Y[0]==food.Y){ 
				GUI_SetColor(GUI_WHITE);
				GUI_FillRect(food.X,food.Y,food.X+1,food.Y+1);
				GUI_SetColor(GUI_BLACK);
				snake.Long++;//????????
				game.Score+=10;
				food.Yes=1;//????????
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



#include "includes.h"
#include  "app.h"
#include <stdlib.h>

#define COLOR_BORDER           0xBD814F
#define COLOR_KEYPAD0          0xA8D403
#define COLOR_KEYPAD1          0xBF5C00
#define SNAKE_Max_Long 60//蛇的最大长度
static  KEY Key1,Key2;

//蛇结构体
struct Snake{
	uint16_t X[SNAKE_Max_Long];
	uint16_t Y[SNAKE_Max_Long];
	u8 Long;//蛇的长度
	u8 Life;//蛇的生命，0表示或者，1表示死亡
	u8 Direction;//蛇移动的方向
}snake;

//食物结构体
struct Food{
	u8 X;//食物横坐标
	u8 Y;//事物纵坐标
	u8 Yes;//判断是否需要出现食物，0表示已有，1表示需要
}food;

//游戏等级分数
struct Game{
	u16 Score;//分数
	u8 Life;//游戏等级	
}game;



//游戏结束
void gameover(){
	GUI_DispStringAt("GAME OVER!", LCD_GetXSize() / 2, LCD_GetYSize()/2);
}

//开始游戏
void play(){
	u16 i;	
	snake.Long=2;//定义蛇的长度
	snake.Life=0;//蛇还活着
	snake.Direction=1;//蛇的起始方向默认为右
	game.Score=0;//起始分数为0
	game.Life=4;//蛇的生命值
	food.Yes=1;//需要新食物出现
	snake.X[0]=0;snake.Y[0]=50;
	snake.X[1]=10;snake.Y[1]=50;	

	while(1){
		//GUI_SetColor(GUI_WHITE);
		//GUI_DrawRect(snake.X[i],snake.Y[i],10,10);
		//print Scor:game.Score
		if(food.Yes==1){
			//在设定区域内显示食物	
			food.X=rand()%(230/10)*10;
			food.Y=rand()%(260/10)*10;
			food.Yes=0;
		}
		//显示食物
		if(food.Yes==0){
			GUI_SetColor(GUI_BLACK);
			GUI_DrawRect(food.X,food.Y,10,10);
			//绘制食物点 ILI9341_DrawRectangle  (food.X,food.Y,10,10,1);
		}
		//更新蛇身体长度
		for(i=snake.Long-1;i>0;i--){
			snake.X[i]=snake.X[i-1];
			snake.Y[i]=snake.Y[i-1];
		}
		switch(snake.Direction){
			case 1:snake.X[0]+=10;break;//向右
			case 2:snake.X[0]-=10;break;//向左
			case 3:snake.Y[0]-=10;break;//向上
			case 4:snake.Y[0]+=10;break;//向下
		}
		//画蛇
		for(i=0;i<snake.Long;i++){
			GUI_DrawRect(snake.X[i],snake.Y[i],10,10);
			//一个点一个点画 ILI9341_DrawRectangle(snake.X[i],snake.Y[i],10,10,1);
		}
		GUI_Delay(500);//延迟500ms
		//Delay(0xFFFFF);
		//设置颜色 LCD_SetTextColor(BLACK)
		GUI_SetColor(GUI_WHITE);
		//消除已经走过的部分
		GUI_DrawRect(snake.X[snake.Long-1],snake.Y[snake.Long-1],10,10);
		//ILI9341_DrawRectangle(snake.X[snake.Long-1],snake.Y[snake.Long-1],10,10,1);//消除蛇身体
		//设置颜色 LCD_SetTextColor(RED)
		GUI_SetColor(GUI_BLACK);
		
		//判断是否撞墙
		if(snake.X[0]==0||snake.X[0]>240||snake.Y[0]==0||snake.Y[0]>270)
			snake.Life=1;//蛇撞墙死亡
		//当蛇的身体超过三节后判断蛇自身的碰撞
		for(i=3;i<snake.Long;i++){
			if(snake.X[i]==snake.X[0]&&snake.Y[i]==snake.Y[0])//任一坐标值与蛇头坐标相等就认为是自身碰撞
			game.Life-=1;
		}
		if(snake.Life==1||game.Life==0){//以上两种判断以后如果蛇死亡则跳出循环，重新开始
			gameover();
			GUI_Delay(2000);
			//延迟两秒后重新开始
			GUI_Clear();
			//清除屏幕ILI9341_Clear(0,0,LCD_X_LENGTH,LCD_Y_LENGTH);
		}
		//判断蛇是否吃到了食物
		if(snake.X[0]==food.X&&snake.Y[0]==food.Y){ 
			//设置颜色LCD_SetTextColor(BLACK)	;
			GUI_SetColor(GUI_WHITE);
			GUI_DrawRect(food.X,food.Y,food.X+1,food.Y+1);
			//ILI9341_DrawRectangle(food.X,food.Y,food.X+1,food.Y+1,1);//消除吃到的食物
			//LCD_SetTextColor(RED);
			GUI_SetColor(GUI_BLACK);
			snake.Long++;//蛇的身体长度加一
			game.Score+=10;
			//LCD_ShowNum(40,165,game.Score,3,16);显示成绩
			food.Yes=1;//需要重新出现食物
		}
			//LCD_ShowNum(224,165,game.Life,1,16);显示生命值	
	}
}
static const GUI_WIDGET_CREATE_INFO _aDialogCreateUSER[] = {
  	{ FRAMEWIN_CreateIndirect, "Snake", 0, 0, 0, 240, 320, 0, 0x0, 0 },//240*320大小
  { TEXT_CreateIndirect, "SCORE", GUI_ID_TEXT0, 7, 5, 218, 15, 0, 0x64, 0 },
//   { TEXT_CreateIndirect, "SNAKE", GUI_ID_TEXT1, 7, 5, 218, 50, 0, 0x64, 0 },
	  // { BUTTON_CreateIndirect, "Start", GUI_ID_BUTTON2, 118-60, 240, 120, 40, 0, 0x0, 0},
	// { BUTTON_CreateIndirect, "KEY1", GUI_ID_BUTTON0, 20, 55, 80, 40, 0, 0x0, 0 },//80*40大小
  	// { BUTTON_CreateIndirect, "KEY2", GUI_ID_BUTTON1, 120, 55, 80, 40, 0, 0x0, 0 },
};

/*********************************************************************
* 
*       Static code
*
**********************************************************************
*/

// USER START (Optionally insert additional static code)

// USER END

/*********************************************************************
*
*       _cbDialog
*/
static void _cbDialogUSER(WM_MESSAGE * pMsg) {
  	WM_HWIN hItem;
	//WM_HWIN hItem_game;
  	int Id;
	int NCode;
	uint8_t    xSize=0;
	uint16_t   ySize=0;
	hItem = pMsg->hWin;

  	// USER START (Optionally insert additional variables)
  	// USER END
  	switch (pMsg->MsgId) {
	  	case WM_DELETE:
			OS_INFO("Snake delete\n");
			Flag_ICON110 = 0;
			UserApp_Flag = 0;
			tpad_flag=0;
		  	break;

	  	case WM_INIT_DIALOG://首先从消息结构中获取框架窗口的句柄，为初始化窗口做准备
			//
			// 控件初始化

			FRAMEWIN_SetFont(hItem, GUI_FONT_16B_ASCII);
			FRAMEWIN_SetTitleHeight(hItem, 20);
			FRAMEWIN_SetTextColor(hItem,GUI_DARKGRAY);
			FRAMEWIN_AddCloseButton(hItem,FRAMEWIN_BUTTON_RIGHT,0);//关闭按钮
			
			// USER START (Optionally insert additional code for further widget initialization)
			// USER END
			//开始按钮初始化设置
			// Initialization of 'calculator'
			//
			hItem = WM_GetDialogItem(pMsg->hWin, GUI_ID_TEXT0);
			TEXT_SetTextColor(hItem, GUI_BLACK);
			TEXT_SetTextAlign(hItem, GUI_TA_LEFT | GUI_TA_VCENTER);
			TEXT_SetFont(hItem, GUI_FONT_24_ASCII);
			TEXT_SetText(hItem, "score");
			WM_BringToTop(hItem);
			TEXT_SetBkColor(hItem,GUI_WHITE);
			//
			// Initialization of 'input'
			//
			// hItem = WM_GetDialogItem(pMsg->hWin, GUI_ID_TEXT1);
			// TEXT_SetFont(hItem, GUI_FONT_24B_ASCII);
			// TEXT_SetTextColor(hItem, GUI_BLACK);
			// TEXT_SetText(hItem, "");
			// TEXT_SetBkColor(hItem,GUI_WHITE);
			// TEXT_SetTextAlign(hItem, GUI_TA_RIGHT | GUI_TA_BOTTOM);
			// hItem = WM_GetDialogItem(pMsg->hWin, GUI_ID_BUTTON2);
			// BUTTON_SetText(hItem, "开始游戏");
			// BUTTON_SetFont(hItem, &XBF_Font);
			//USER START (Optionally insert additional message handling)
			hItem = WM_GetActiveWindow();
			WM_BringToTop(hItem);
			play();
			break;
		/*
		这个消息是对话框回调函数的重点，
		所有对话框子控件的具体行为逻辑都在此消息中设置和处理。
		在此消息中以控件 ID 来区分各个不同的控件。
		使用 WM_GetId 函数从消息结构中提取出发送此条消息的源控件 ID 号，
		并从消息结构的 Data.v 成员参数中提取通知代码。
		然后使用 switch语句嵌套的方式判断当前是哪个控件发送的消息以及附加的通知代码类型
		通过判断按钮的通知代码可以知道按钮当前是已被按下还是已被释放
		*/
	//处理按钮消息
		case WM_PAINT:	//绘制背景
			xSize = WM_GetWindowSizeX(hItem);
			ySize = WM_GetWindowSizeY(hItem);
			GUI_SetColor(COLOR_BORDER);
			GUI_DrawRect(0, 0, xSize - 1, ySize - 1);
			GUI_DrawGradientV(1, 1, xSize - 2, ySize - 2, COLOR_KEYPAD0, COLOR_KEYPAD1);
			break;

		case WM_NOTIFY_PARENT://通知父窗口其中一个子窗口发生事件
			Id = WM_GetId(pMsg->hWinSrc);
			NCode = pMsg->Data.v;
			switch(Id){
				case GUI_ID_BUTTON0: // Notifications sent by 'KEY1'
		  			switch(NCode) {
		  				case WM_NOTIFICATION_CLICKED://KEY1按下
							//printf("key 1\n");
							if(snake.Direction==1)
								snake.Direction = 3;
							else if (snake.Direction==3)
								snake.Direction = 2;
							else if (snake.Direction==2)
								snake.Direction = 4;
							else
								snake.Direction = 1;
							//printf("Direction=%d\n",snake.Direction);
							LED1_ON;
							break;
		  				case WM_NOTIFICATION_RELEASED://KEY1释放
		   			 		LED1_OFF;
							break;
		  				case WM_NOTIFICATION_MOVED_OUT:
							LED1_OFF;
							break;
		 			}
		  			break;

				case GUI_ID_BUTTON1: // Notifications sent by 'KEY2'
		  			switch(NCode) {
		 				case WM_NOTIFICATION_CLICKED:
							//printf("key 2\n");
							if(snake.Direction==1)
								snake.Direction = 4;
							else if (snake.Direction==4)
								snake.Direction = 2;
							else if (snake.Direction==2)
								snake.Direction = 3;
							else
								snake.Direction = 1;
							//printf("Direction=%d\n",snake.Direction);
							LED2_ON;
							break;
		  				case WM_NOTIFICATION_RELEASED:
							LED2_OFF;
							break;
		   				case WM_NOTIFICATION_MOVED_OUT:
							LED2_OFF;
							break;
		  			}
		 			break;
/*
				case GUI_ID_BUTTON2:
				switch(NCode){
					case WM_NOTIFICATION_CLICKED://开始按钮按下
						break;
		 			case WM_NOTIFICATION_RELEASED://开始按钮释放
						// USER START (Optionally insert code for reacting on notification message)
						WM_HideWin(hItem);
						//hItem_game = WM_CreateWindow(0,0,240,270,WM_CF_MEMDEV,NULL,0);
						//WM_BringToTop(hItem_game);
						play();
						Soft_Reset();
						//WM_DeleteWindow(hItem_game);
						//BUTTON_SetText(WM_GetDialogItem(pMsg->hWin, GUI_ID_BUTTON0), "请按复位键");
						// USER END
						break;
		  			case WM_NOTIFICATION_VALUE_CHANGED:
						// USER START (Optionally insert code for reacting on notification message)
						// USER END			    
						break;
				}
			}
*/
			}
		
	  	// USER END
	  	default:
			WM_DefaultProc(pMsg);
			break;
  	}
}

//这应该是入口函数
void FUN_ICON111Clicked(void){
	WM_HWIN hWin;
	OS_INFO("USERapp create\n");
	hWin=GUI_CreateDialogBox(_aDialogCreateUSER, GUI_COUNTOF(_aDialogCreateUSER), _cbDialogUSER, WM_HBKWIN, 0, 0);
	WM_BringToTop(hWin);
	
	while(Flag_ICON110)
	{					 				
		Key_RefreshState(&Key1);//刷新按键状态
		Key_RefreshState(&Key2);//刷新按键状态
		if(Key_AccessTimes(&Key1,KEY_ACCESS_READ)!=0)//按键被按下过
		{
			LED1_TOGGLE;
			Key_AccessTimes(&Key1,KEY_ACCESS_WRITE_CLEAR);
		}
		if(Key_AccessTimes(&Key2,KEY_ACCESS_READ)!=0)//按键被按下过
		{
			LED2_TOGGLE;
			Key_AccessTimes(&Key2,KEY_ACCESS_WRITE_CLEAR);
		}
		if(tpad_flag)WM_DeleteWindow(hWin);
		GUI_Delay(10); 				
	}
}
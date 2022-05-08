#include  <stm32f10x.h>
 
#include <stdio.h>

#include "./led/bsp_led.h" 
#include "./key/bsp_exti.h"
#include "./bsp/EEPROM/bsp_i2c_gpio_ee.h"
#include "./bsp/lcd/bsp_ili9341_lcd.h"
#include "./bsp/usart/bsp_usart1.h"
#include "./SysTick/bsp_SysTick.h"  
#include "./key/bsp_key.h"  

static void LCD_Test(void);	
uint32_t time = 0;
char str[20];
uint16_t  key_flag=0;
extern mk;
char dispBuff[100];
#define SNAKE_Max_Long 60//蛇的最大长度
static void Delay ( __IO uint32_t nCount );
//蛇结构体
struct Snake
{
	uint16_t X[SNAKE_Max_Long];
	uint16_t Y[SNAKE_Max_Long];
	u8 Long;//蛇的长度
	u8 Life;//蛇的生命，0表示或者，1表示死亡
	u8 Direction;//蛇移动的方向
}snake;

//事物结构体
struct Food
{
	u8 X;//食物横坐标
	u8 Y;//事物纵坐标
	u8 Yes;//判断是否需要出现食物，0表示已有，1表示需要
}food;

//游戏等级分数
struct Game
{
	u16 Score;//分数
	u8 Life;//游戏等级	
}game;

void KEY1_IRQHandler(void)
{
  //确保是否产生了Exit line中断
	//4231
	if(EXTI_GetITStatus(KEY1_INT_EXTI_LINE) != RESET) 
	{ 
		printf("key 1\n");
		if(snake.Direction==1)
			snake.Direction = 3;
		else if (snake.Direction==3)
			snake.Direction = 2;
		else if (snake.Direction==2)
			snake.Direction = 4;
		else
			snake.Direction = 1;
		 printf("Direction=%d\n",snake.Direction);
		EXTI_ClearITPendingBit(KEY1_INT_EXTI_LINE);     
	}  
}

void KEY2_IRQHandler(void)
{
  //确保是否产生了Exit line中断
	if(EXTI_GetITStatus(KEY2_INT_EXTI_LINE) != RESET) 
	{
		printf("key 2\n");
		if(snake.Direction==1)
			snake.Direction = 4;
		else if (snake.Direction==4)
			snake.Direction = 2;
		else if (snake.Direction==2)
			snake.Direction = 3;
		else
			snake.Direction = 1;
		 printf("Direction=%d\n",snake.Direction);
		EXTI_ClearITPendingBit(KEY2_INT_EXTI_LINE);     
	} 

}

void USART1_IRQHandler(u8 GetData)//串口中断
{

if(USART_GetITStatus(USART1,USART_IT_RXNE)!=RESET)
	{
		USART_ClearITPendingBit(USART1,USART_IT_RXNE);
		GetData=USART1->DR;
		mk=GetData;
		printf("收到的数据：\n");
	  printf("%d\n",GetData);
	  
		printf("0x%x\n",mk);
		if(mk==0x1)
		{	
		snake.Direction=1;
			
		}
		if(GetData==0x2)
		{
			
    	snake.Direction=2;
			
			
		}
		
	
	}
    
}

//游戏结束
void gameover()
	{

			ILI9341_DispStringLine_EN(LINE(1),"          game over!");
		
		}
		
//开始游戏
void play()
{
	u16 i;	
	snake.Long=2;//定义蛇的长度
	snake.Life=0;//蛇还活着
	snake.Direction=1;//蛇的起始方向默认为右
	game.Score=0;//起始分数为0
	game.Life=4;//蛇的生命值
	food.Yes=1;//需要新事物出现
	snake.X[0]=0;snake.Y[0]=50;
	snake.X[1]=10;snake.Y[1]=50;


		while(1)
	{
		
		   ILI9341_DrawRectangle(0,0,240,270,0);
		

      sprintf(dispBuff,"SCORE : %d ",game.Score);
		
		  ILI9341_DispStringLine_EN(LINE(18),dispBuff);
			if(food.Yes==1)//出现新食物
			{
				//在设定区域内显示食物	

			    food.X=rand()%(230/10)*10;
					food.Y=rand()%(260/10)*10;

					food.Yes=0;
			}
				if(food.Yes==0)//有食物就需要显示
				{	
		
					 LCD_SetColors(RED,BLACK);
					 ILI9341_DrawRectangle  (food.X,food.Y,10,10,1);
				
				}
				//取得需要重新画的蛇的节数
				for(i=snake.Long-1;i>0;i--)
				{
					snake.X[i]=snake.X[i-1];
					snake.Y[i]=snake.Y[i-1];
				}

				
	
				switch(snake.Direction)
				{
					case 1:snake.X[0]+=10;break;//向右
					case 2:snake.X[0]-=10;break;//向左
					case 3:snake.Y[0]-=10;break;//向上
					case 4:snake.Y[0]+=10;break;//向下
					
					
				}
					for(i=0;i<snake.Long;i++)//画出蛇	
						
				ILI9341_DrawRectangle(snake.X[i],snake.Y[i],10,10,1);//画蛇身体
				
				

				
					 Delay(0xFFFFF);
				
				  
				    LCD_SetTextColor								(BLACK  )	;
						ILI9341_DrawRectangle(snake.X[snake.Long-1],snake.Y[snake.Long-1],10,10,1);//消除蛇身体	
					
					 LCD_SetTextColor								(RED  )	;
					//判断是否撞墙
					if(snake.X[0]==0||snake.X[0]>240||snake.Y[0]==0||snake.Y[0]>270)

						snake.Life=1;//蛇撞墙死亡
		
					//当蛇的身体超过三节后判断蛇自身的碰撞
					for(i=3;i<snake.Long;i++)
					{
						if(snake.X[i]==snake.X[0]&&snake.Y[i]==snake.Y[0])//任一坐标值与蛇头坐标相等就认为是自身碰撞
						{
							game.Life-=1;
						}
					}
					if(snake.Life==1||game.Life==0)//以上两种判断以后如果蛇死亡则跳出循环，重新开始
					{
					gameover();
						
					Delay(0xFFFFFF);
					Delay(0xFFFFFF);//延迟两秒后重新开始
				  
						
					ILI9341_Clear(0,0,LCD_X_LENGTH,LCD_Y_LENGTH);	
					break;
					}	
					//判断蛇是否吃到了食物
					if(snake.X[0]==food.X&&snake.Y[0]==food.Y)
					{ 
						
						LCD_SetTextColor								(BLACK  )	;
					//	ILI9341_DrawRectangle(food.X,food.Y,food.X+1,food.Y+1,1);//消除吃到的食物
						
						ILI9341_DrawRectangle(food.X,food.Y,10,10,1);//消除吃到的食物
						LCD_SetTextColor								(RED  )	;
						snake.Long++;//蛇的身体长度加一
						game.Score+=10;
						//LCD_ShowNum(40,165,game.Score,3,16);显示成绩
						food.Yes=1;//需要重新出现食物
					}
						//LCD_ShowNum(224,165,game.Life,1,16);显示生命值
	}	
}


int main ( void )
{

	
	
	ILI9341_GramScan ( 6 );
    
	
  ILI9341_Init ();
  LED_GPIO_Config();
	USART_Config();	
  Key_GPIO_Config();
  SysTick_Init();
  LCD_SetFont(&Font8x16);
	
	LCD_SetColors(RED,BLACK);
  EXTI_Key_Config();
  ILI9341_Clear(0,0,LCD_X_LENGTH,LCD_Y_LENGTH);
	while ( 1 )
	{ 
	 
	   
  ILI9341_DrawRectangle(0,0,240,270,0);
   
		
	 printf("test\n");
   play();


	}

}


static void Delay ( __IO uint32_t nCount )
{
  for ( ; nCount != 0; nCount -- );
	
}

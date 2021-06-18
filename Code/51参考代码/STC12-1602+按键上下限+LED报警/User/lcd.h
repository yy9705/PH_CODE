/********************************************************************************\
**                         			宏定义区									**
\********************************************************************************/
/*-------------------------------  LCD模块 ------------------------------------*/
sbit LCD_RW  			=P2^6; 				  //读写控制端
sbit LCD_RS  			=P2^5;					//数据命令选择端
sbit LCD_E   			=P2^7;					//执行使能端
#define Busy			0x80					//忙信号
#define  LCD_Data 	    P0	
#define Write				0x00					//低电平写入
#define Read				0x01					//高电平读出
#define Data				0x01					//高电平选择数据
#define Cmd				    0x00					//低电平选择命令

#define Enable		    	0x00					//跃变到低电平时执行命令
#define Disable		    	0x01    
#define True				0x01
#define False				0x00
#define LCD_Init			0x38					//初始化模式							
#define LCD_DispCtr    		0x0C					//开显示及光标设置
#define LCD_CloseCtr		0x08					//关显示
#define LCD_CLS        		0x01					//清屏幕
#define LCD_EnterSet   		0x06					//显示光标 

/*-------------------------------  LCD模块 -------------------------------------*/
void LCDInit(void);								//LCD初始化
void SetWriteCmd(void);								//设置写命令模式
void SetReadCmd(void);								//设置读命令模式
void SetWriteData(void);							//设置写数据模式
void WriteCmd(char cmd);							//写命令
void WriteData(char ddata);							//写数据
void ExecuteCmd(void);								//执行命令
void SetXY(char x,char y);							//定位显示地址
void DisplaySingleChar(char x,char y,char cchar);		//显示单个字符
void DisplayString(char x,char y,char *str); 			//显示一段字符串
void Delay1(unsigned int time);						//延时主程序
void DelayUs(unsigned int time);						//延时子程序
//void delay500ms(void) ;
bit  IsBusy(void);									//判断忙标志函数
void DisplayTime(void);								//显示时间
void DisplayAVGSpeed(void);							//显示平均速度
void DisplayDistance(void);							//显示路程

/****************************************************************************************\
**								   LCD驱动模块								**
\****************************************************************************************/

/*---------------------------------  LCD初始化函数--------------------------------------*/

void LCDInit(void)
{
    //三次显示模式设置
	LCD_Data=0;
	LCD_E=Disable;
	Delay1(5);
	WriteCmd(LCD_Init);
	Delay1(5);
	WriteCmd(LCD_Init);
	Delay1(5);
	WriteCmd(LCD_Init);

	WriteCmd(LCD_Init);	   						//初始化
	WriteCmd(LCD_CloseCtr);							//关显示
	WriteCmd(LCD_CLS);								//清屏幕
	WriteCmd(LCD_EnterSet);							//光标移动设置
	WriteCmd(LCD_DispCtr);							//显示开以及光标设置
	
}

/*---------------------------------  LCD模式设置函数--------------------------------------*\
	
	SetWriteCmd() 	 			设置LCD为写命令模式
	SetReadCmd()   		    		设置LCD为读命令模式
	SetWriteData()				设置LCD为写数据模式
	
\*----------------------------------------------------------------------------------------*/
void SetWriteCmd(void)
{
	LCD_RW=Write;
	LCD_RS=Cmd;
}

void SetReadCmd(void)
{
	LCD_RW=Read;
	LCD_RS=Cmd;
}

void SetWriteData(void)
{
	LCD_RW=Write;
	LCD_RS=Data;
}

/*---------------------------------  LCD功能执行函数--------------------------------------*\
	
	WriteCmd()					写命令
	WriteData()					写数据
	ExecuteCmd()					执行命令
	SetXY()						显示定位
	DisplaySingleChar()			显示单个字符
	DisplayString()				显示一串字符
	IsBusy()						忙标志检测
\*----------------------------------------------------------------------------------------*/
void WriteCmd(char cmd)
{
	while(IsBusy());
	LCD_Data=cmd;	
	SetWriteCmd();
	ExecuteCmd();
}

void WriteData(char ddata)
{
	while(IsBusy());
	LCD_Data=ddata;
	SetWriteData();	
	ExecuteCmd();
}

void ExecuteCmd(void)
{
	LCD_E=Enable;
	LCD_E=Disable;
}

void SetXY(char x,char y)
{
	if (y)
		x|=0x40;
	x|=0x80;
	Delay1(5);
	WriteCmd(x);

}

void DisplaySingleChar(char x,char y,char cchar)
{
	SetXY(x,y);
	WriteData(cchar);	
}

void DisplayString(char x,char y,char *str)
{
	while(*str)
	{
		Delay1(5);
		DisplaySingleChar(x++,y,*str);
		str++;
	}
}

bit IsBusy(void)
{
	LCD_Data=0xFF;
	SetReadCmd();
	ExecuteCmd();
	return (bit)(P0 & 0x80);	
}
//////////////////////////////////
//端口定义
/////////////////////////////////////////////////////////////////////
//MFRC500
/*
sbit     MF522_NSS  =P1^3 ;			//RC500片选
sbit     MF522_SCK  = P1^4;
sbit     MF522_SI   = P1^5;
sbit     MF522_SO   = P1^6;
sbit     MF522_RST  = P1^7;                  
*/
sbit     MF522_NSS  =P2^4 ;			//RC500片选 SDA
sbit     MF522_SCK  = P2^5;
sbit     MF522_SI   = P2^3;         //MOSI
sbit     MF522_SO   = P2^2;         //MISO
sbit     MF522_RST  = P2^1; 
//舵机
sbit PWN_OUT = P2^7;		//PWN输出口 

//函数原型
/////////////////////////////////////////////////////////////////////
void InitializeSystem();  
void Init_Timer0();             //定时器初始化
void doorOpen();                //开门
void servoRet();                //舵机复位           
                                    






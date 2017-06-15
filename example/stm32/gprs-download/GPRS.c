u8 NetPower=0;//0-关闭 1-启动
u8 NetActive=0;//0-休眠 1-活跃
u8 NetLink=0;//0-未链接 1-已连接
u8 NetCollect=0;//0-模块未就绪 1-模块就绪
u8 bufer[256]={"$WRITE ADMIN56BBCCDDEEFFGGHHIIJJKKLLMMNNOO12#"};

u8 GPRSmsg[50];//GPRS信息存储数组
u16 msg[]={0x0024,0x004F,0x0050,0x0045,0x004E,0x0023,0x3010};
u8	LINK_NO_ERROR=0;
u8 TCP_LINK=0X00;
u8 Timecount=0;     //TCP连接计时
u8 Doorstatus=0;			//开关门状态
u8 SendFre=0;//正常发送频率
u8 FreeSendFre=0;//闲置发送频率

char gprscard[27]={"\"101.201.051.006\",10080,2\r\n"};
char SendSym=0X00;//定时报发送标志位
char GsmBuf[USART_REC_LEN ];//通讯模块交互信息寄存器

void ResetGPRSmsgBuf()
{
			memset(GPRSmsg, 0, sizeof(GPRSmsg));

}

void ResetGsmBuf(void)//UART1
{
    memset(USART_RX_BUF, 0, sizeof(USART_RX_BUF));
    USART_RX_STA=0;
}

void TCP_LINK_TimeDeal()//TCP 连接等待时间检测
{


           if(Timecount==1)  
					 {
              TCP_LINK |=0x0A;
							
						  TIM_Cmd(TIM3, DISABLE);  //关闭TIMx外设
							Timecount=0;
						  
					 }

}
u8 BcdToChar(u8 a)
{
    if(a>=0&&a<10) return a+0x30;
    else if(a>=10&&a<=15) return a+0x37;
    else return (u8)RET_ERR;
}

int gsmstr(char* p) //gsmmBuf中寻找子字符串
{
     char* pb;
     u8 commonnum=0;u8 i,j=0;
     
     pb=USART_RX_BUF;
     for( i=0;i<200;i++)
     {
         if(*(pb+i)==*p)
         {
             for( j=0;j<strlen(p);j++)
             {
                 if(*(pb+i+j)==*(p+j)) commonnum++;
                 else {commonnum=0;j=strlen(p);}
             }
         }
         if(commonnum==strlen(p)) return RET_NO_ERR;
     }
     return RET_ERR;
}
int gsm_msg_str(char* p) //gsmmBuf中寻找子字符串
{
     char* pb;
     u8 commonnum=0;u8 i,j=0;
     
     //pb= GSMmsg;
     for( i=0;i<200;i++)
     {
         if(*(pb+i)==*p)
         {
             for( j=0;j<strlen(p);j++)
             {
                 if(*(pb+i+j)==*(p+j)) commonnum++;
                 else {commonnum=0;j=strlen(p);}
             }
         }
         if(commonnum==strlen(p)) return RET_NO_ERR;
     }
     return RET_ERR;
}
/*****************************************************************
*                                                                *
*                   功能:模块硬件启动函数                        *
*                                                                *
*****************************************************************/
void Start_up(void)
{
#if LOG==1
      
	UART3_PutString("发送模块启动指令\r\n");
#endif
  GPIO_SetBits(GPIOC,GPIO_Pin_9);	
	delay_ms(1100);
  GPIO_ResetBits(GPIOC,GPIO_Pin_9);
	delay_ms(1100);
 
}

/*****************************************************************
*                                                                *
*                   功能:模块启动检测函数                        *
*                                                                *
*****************************************************************/
u8 ModuleSwitchON(void)
{ 
	
#if LOG==1
      
	UART3_PutString("开始启动通讯模块\r\n");
#endif
	u8 i;
	
	
	if(NetPower==0)
	{
			for(i=0;i<10;i++)
				{
		
						if(gsmstr("Jump to BL")==RET_NO_ERR)
							{
										NetPower=1;
										NetActive=1;
										return RET_NO_ERR;
										break;
							}
	
	
							else 
							{
								Start_up();
								delay_ms(500);
							}
	
				}
	}
	ResetGsmBuf();
	
}
/*****************************************************************
*                                                                *
*                   功能:模块关闭函数                            *
*                                                                *
*****************************************************************/
void ModuleSwitchOFF(void)
{
	u8 ncc=0;
#if LOG==1
       
	UART3_PutString("开始关闭通讯模块\r\n");
#endif
	if(NetPower==1)
	{
		while(strstr(GsmBuf,"OK")==NULL)
		{
			ResetGsmBuf();
			UART1_PutString("AT+MSO\r\n");
			delay_ms(500);
			ncc++;
			if(ncc>3) 
			{
				UART1_PutString("AT\r\n");
				delay_ms(50);
				if(gsmstr("OK")==RET_ERR) break;
				else Start_up();
			}
		}
#if LOG==1
               
		UART3_PutString("通讯模块已关闭\r\n");
#endif
		NetLink=0;
		NetCollect=0;
		NetActive=0;
		NetPower=0;
	}
	ResetGsmBuf();
}
/*****************************************************************
*                                                                *
*                   功能:模块初始化  检测状态                    *
*                                                                *
*****************************************************************/
void  Internet_Connect_Check(void)
{ 
	u8 i,j;

#if LOG==1
      
				UART3_PutString("开始检查AT命令可用网络注册\r\n");
#endif
	delay_ms(1000);
	if(NetPower==1&&NetActive==1)
	{
	for(i=0;i<3;i++)
	{
			UART1_PutString("AT\r\n");
			delay_ms(50);
		
		if(gsmstr("OK")==RET_NO_ERR)
		{
						ResetGsmBuf();
						UART1_PutString("AT+ESIMS?\r\n");
						delay_ms(50);
						if(gsmstr("OK")==RET_NO_ERR)
								i=3;
																														
						ResetGsmBuf();
																							
		}
		
	}	
			while(1)
				{
					for(j=0;j<10;j++)
					{
						UART1_PutString("AT+CREG?\r\n");//检查网络注册
						delay_ms(1000);
						if(gsmstr("+CREG: 0,1")==RET_NO_ERR||gsmstr("+CREG: 0,5")==RET_NO_ERR)
					
							{
								NetCollect=1;//模块就绪
								ResetGsmBuf();
								break;								
							}	
							
						}	break;
					
				}				
	}
#if LOG==1
      
	UART3_PutString("关回显\r\n");
#endif

		UART1_PutString("ATE0\r\n");//关回显
		delay_ms(50);	
		UART1_PutString("AT+CNMI=2,1\r\n");//设置短信新消息提示,将短信存到SIM卡后，再给提示
		delay_ms(50);	
		UART1_PutString("AT+CMGF=1\r\n");//文本方式
		delay_ms(50);	

		ResetGsmBuf();
		
}

/**********************端口打开结果验证************************/
u8 chkConnect(void)
{
			
#if LOG==1
       
	UART3_PutString("开始检查通讯链路是否成功\r\n");
#endif  

	u8 rev=0;
	if(gsmstr("CONNECT OK")==RET_NO_ERR) 
			{
		
				rev= LINK_SUCCESS;//连接成功
			}
	else if(gsmstr("TCP CLOSED")==RET_NO_ERR) 
        {
						rev= LINK_ERROR;//错误
        }
	else if(gsmstr("ERROR")==RET_NO_ERR) 
        {									
					rev= LINK_ERROR;//错误
        }
				
	else if (gsmstr("CONNECT FAIL")==RET_NO_ERR)	
				{
					rev= LINK_ERROR;//错误								
				}
				
					ResetGsmBuf();				
					if(rev==LINK_ERROR||rev==LINK_SUCCESS) 
		return rev;
	
}

/**********************模块睡眠************************/
void Sleep_Down(void)//睡眠
{
	u8 i;
					
						DTRHIGH;								
					//	UART1_PutString("AT+EDFT=2,0\r\n");
						delay_ms(100);
						UART1_PutString("AT+CSCLK=1\r\n");
						delay_ms(100);
						UART1_PutString("AT+CSCLK=1\r\n");	
						NetActive=0;
						NetCollect=0;																																																								
																																																						
}
/**********************模块唤醒************************/
 void Wake_Up(void)//唤醒
{
		u8 i,j;
	
			DTRLOW;
			delay_ms(50);
	for(j=0;j<5;j++)
		{
				UART1_PutString("AT+CSCLK=0\r\n");
				delay_ms(10);
		}
		
		//GPIO_ResetBits(GPIOA,GPIO_Pin_8);
		for(i=0;i<3;i++)
		{
			UART1_PutString("AT\r\n");
			delay_ms(50);
			if(gsmstr("OK")==RET_NO_ERR) i=3;
			{
					NetActive=1;	//模块活动						
					break;
			}
			
		}
			ResetGsmBuf();
		
			NetCollect=1;//模块就绪
			
}
/**********************发送函数************************/
void GPRS_send_message(char *cmd,int a, int c)//cmd发送内容，a第几个服务器，c报文长度
{
	char sdcmd[20]={"AT+CIPSEND="};
#if LOG==1
       
	PutString2("开始发送信息\r\n");
#endif        
	sprintf(sdcmd+11,"%d",c);//AT+CIPSEND=sdcmd+11发送多少个字符
	strcat(sdcmd,"\r\n");

	//sdcmd[11]=a+0x30;  //第几个服务器
	UART1_PutString((char*)sdcmd);        
	delay_ms(350);
	//while(1)
	//{
		//if(gsmstr(">")==RET_NO_ERR)
			//{	
					UART1_PutString(( unsigned char*)cmd);	
					delay_ms(250);
					//ResetGsmBuf();	
			//	break;
			//}
	//}
}


void CloseTCP()
{
	
	char clcmd[16] = {"AT+CIPCLOSE=1\r\n"};  //关闭连接
	//if(NetPower==1&&NetActive==1&&NetCollect==1)
	//{						
				UART1_PutString(clcmd);	
				NetLink=0;			
	//}

}


/**********************TCP设置************************/

void GPRS_Tcp_Setting(void)
{                                                                       
#if LOG==1       
	 UART3_PutString("运行GPRS_Tcp_Setting\r\n");
#endif  
			
				u8 i;
				
				char ipcmd[51] ={"AT+CIPSTART=\"TCP\","};//发起连接
				char clcmd[16] = {"AT+CIPCLOSE=1\r\n"};  //关闭连接
	

				while(1)
				{
					
						UART1_PutString("AT+CREG?\r\n");//检查网络注册
						delay_ms(1000);
						if(gsmstr("+CREG: 0,1")==RET_NO_ERR||gsmstr("+CREG: 0,5")==RET_NO_ERR)
					
							{
								NetCollect=1;//模块就绪
								
								if(NetCollect==1)
								{
												UART1_PutString("ATE0\r\n");//关回显
												delay_ms(50);	
												UART1_PutString("AT+CNMI=2,1\r\n");//设置短信新消息提示,将短信存到SIM卡后，再给提示
												delay_ms(50);	
												UART1_PutString("AT+CMGF=1\r\n");//文本方式
												delay_ms(50);	
												UART1_PutString("AT+CIICR\r\n");//激活GPRS网络
												delay_ms(50);
												UART1_PutString("AT+CIPMUX=0\r\n");//单链接
												delay_ms(50);
												UART1_PutString("AT+CIPMODE=0,0\r\n");//非透传
												delay_ms(50);
												UART1_PutString("AT+CIPCFG=0,50,0\r\n");//50s后发心跳包
												delay_ms(50);
												UART1_PutString("AT+CIPPACK=0,\"0100A0\"\r\n");//心跳包格式
												delay_ms(50);
												ResetGsmBuf();
												memcpy((ipcmd+18), gprscard, 27);			
												UART1_PutString(ipcmd);														//发送IP,PORT	
				
												delay_ms(100);
											
								}
							
								ResetGsmBuf();
								break;
								
							}else WorkMode=0;break;
							
											
				}

																																					
					while(1)												
							{							
									delay_ms(600);
								
									TIM_Cmd(TIM3, ENABLE);  //使能TIMx外设
								
									TCP_LINK_TimeDeal();
									BLE_config();//蓝牙
								
							if( TCP_LINK==0X0A)								//等待2分钟连接超时
							{
								  TCP_LINK&=0x00;
							
								
									NetLink=0;//连接失败														
									CloseTCP();
									ResetGsmBuf();
								
									break;
							}
								
							if(gsmstr("CONNECT OK")==RET_NO_ERR)
								{
									NetLink=1;//连接成功	
						
									TIM_Cmd(TIM3, DISABLE);  //关闭TIMx外设
									Timecount=1;
									
									ResetGsmBuf();
									break;																	
								 }
							if(gsmstr("CONNECT FAIL")==RET_NO_ERR)
								{
									NetLink=0;//连接失败														
									CloseTCP();
									ResetGsmBuf();
								
									break;														
								}
							if(gsmstr("TCP CLOSED")==RET_NO_ERR)
							{
									NetLink=0;//连接失败														
									CloseTCP();
									ResetGsmBuf();
									
									break;														
							}
							if(gsmstr("ERROR")==RET_NO_ERR) 
							{
									NetLink=0;//连接失败														
									CloseTCP();
									ResetGsmBuf();
									
									break;																				
							}
							if(gsmstr("NO CARRIER")==RET_NO_ERR) 
							{
									NetLink=0;//连接失败		
									CloseTCP();
									ResetGsmBuf();
								 
									break;																				
							}
							if(gsmstr("+CREG: 0,0")==RET_NO_ERR) 
							{
									NetLink=0;//连接失败														
									CloseTCP();
									ResetGsmBuf();
									
									break;																				
							}
																																	
							 if(gsmstr("RECONNECTING")==RET_NO_ERR) 							
								{	
						
									NetLink=0;//连接失败														
									CloseTCP();
									ResetGsmBuf();
									
									break;	

							}																						
				ResetGsmBuf();
				
     }

}
/////////////////////////////////////////////////////////////////


///////////TCP CHECK//检测TCP 连接情况///////////////////////////


////////////////////////////////////////////////////////////////


void TCP_CHECK()
{
		if(gsmstr("NO CARRIER")==RET_NO_ERR||gsmstr("CONNECT FAIL")==RET_NO_ERR||gsmstr("+CREG: 0,0")==RET_NO_ERR) 
			{
									NetLink=0;//连接失败
				
									CloseTCP();

									ResetGsmBuf();
									
			}

}


//---------------------------------------------------------
//函数名称：void gsm_send_message(char* pmsg)
//函数功能：发送短消息
//输入参数：
//          pmsg,短消息指针
//返回参数：无
//---------------------------------------------------------
u8 gsm_send_message(char *pmsg)
{ 
	u8 mon=0;
	char cmd[22] = {"AT+CMGS=00000000000\r\n"};

	UART1_PutString("AT+CMGF=1\r\n");
	delay_ms(50);
	memcpy((cmd+8), telnum, 11);
	UART1_PutString(cmd);

	delay_ms(100);

	if(gsmstr(">")==RET_ERR)
		delay_ms(150);

	UART1_PutString((char*)pmsg);
	delay_ms(100); 
	UART1_Send1Char(0x1a);

	delay_ms(500);

	if(gsmstr("ERROR")==RET_NO_ERR)
	{
		UART1_PutString((char*)pmsg);
		delay_ms(100);            
		UART1_Send1Char(0x1a);
	}
	else
	{
		while(gsmstr("+CMGS: ")==RET_ERR)
		{
			delay_ms(200);
			mon++;
			if(mon>20)
       return RET_ERR;
		}
		return RET_NO_ERR;
	}
	delay_ms(100);  
	return RET_ERR;
}



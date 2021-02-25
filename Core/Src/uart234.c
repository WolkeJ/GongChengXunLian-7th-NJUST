#include "uart234.h"
#include "user_usart.h"
#include "usart.h"
#include "Max7219.h"
#include <stdio.h>
#include "ssd1306.h"
#include "i2c.h"

uint8_t uart3ok=0;

uint8_t Ov3Mode = Ov3Mode_QrCode;
uint8_t Max7219_String[]="123--213";
uint8_t CMD_Serial[] = "CMD_Serial";
uint8_t CMD_Material[] = "CMD_Material\r\n";

extern int32_t y_speed;
extern int32_t x_speed;

//uint8_t aRxBuffer[256]={0};

//void Uart3_readQRcode(){
//	HAL_UART_Receive_IT(&huart3, (uint8_t *)aRxBuffer, 11);//����OpenMV����,������aRxBuffer����ʽΪ"123+123"

//	while (HAL_UART_GetState(&huart3) != HAL_UART_STATE_READY)
//	{
//		HAL_UART_Transmit(&huart3, CMD_Serial, 10, 0xffff);
//		HAL_Delay(10);
//	}
//	Max7219_String[0]=aRxBuffer[1];
//	Max7219_String[1]=aRxBuffer[2];
//	Max7219_String[2]=aRxBuffer[3];
//	Max7219_String[5]=aRxBuffer[5];
//	Max7219_String[6]=aRxBuffer[6];
//	Max7219_String[7]=aRxBuffer[7];
//	WriteNum_Max7219(Max7219_String);
//	led_shan();
//}

uint32_t Catch1Time[6]={5000,5500,5500,7800,6000,6500};

uint8_t uart2_ServoInit[]={0x55,0x55,0x05,0x06,0x01,0x01,0x00};//������1����ʼ��
uint8_t uart2_PreCatch[]={0x55,0x55,0x05,0x06,0x02,0x01,0x00};//������2��Ԥץȡ
/*yuan liao qu,from up to down,left to right*/ 
uint8_t uart2_Catch1[]={0x55,0x55,0x05,0x06,0x03,0x01,0x00};//������3��ץ��1, 5s
uint8_t uart2_Catch2[]={0x55,0x55,0x05,0x06,0x04,0x01,0x00};//������4��ץ��2, 5s
uint8_t uart2_Catch3[]={0x55,0x55,0x05,0x06,0x05,0x01,0x00};//������5��ץ��3, 5s
uint8_t uart2_Catch4[]={0x55,0x55,0x05,0x06,0x06,0x01,0x00};//������6��ץ��1, 6s
uint8_t uart2_Catch5[]={0x55,0x55,0x05,0x06,0x07,0x01,0x00};//������7��ץ��2, 5.3s
uint8_t uart2_Catch6[]={0x55,0x55,0x05,0x06,0x08,0x01,0x00};//������8��ץ��3, 5s
/*cu jia gong qu fang,from left to right*/
uint8_t uart2_Catch7[]={0x55,0x55,0x05,0x06,0x0c,0x01,0x00};//������12
uint8_t uart2_Catch8[]={0x55,0x55,0x05,0x06,0x0a,0x01,0x00};//������10
uint8_t uart2_Catch9[]={0x55,0x55,0x05,0x06,0x0b,0x01,0x00};//������11
/*cu jia gong qu qu,from left to right*/
uint8_t uart2_Catch10[]={0x55,0x55,0x05,0x06,0x10,0x01,0x00};//������16
uint8_t uart2_Catch11[]={0x55,0x55,0x05,0x06,0x0e,0x01,0x00};//������14
uint8_t uart2_Catch12[]={0x55,0x55,0x05,0x06,0x0f,0x01,0x00};//������15
/*cheng pin qu,from up to down,left to right*/
uint8_t uart2_Catch13[]={0x55,0x55,0x05,0x06,0x14,0x01,0x00};//������20
uint8_t uart2_Catch14[]={0x55,0x55,0x05,0x06,0x12,0x01,0x00};//������18
uint8_t uart2_Catch15[]={0x55,0x55,0x05,0x06,0x13,0x01,0x00};//������19
uint8_t uart2_Catch16[]={0x55,0x55,0x05,0x06,0x17,0x01,0x00};//������23
uint8_t uart2_Catch17[]={0x55,0x55,0x05,0x06,0x15,0x01,0x00};//������21
uint8_t uart2_Catch18[]={0x55,0x55,0x05,0x06,0x16,0x01,0x00};//������22

uint32_t delayTime[]={0,0,3300,3500,3500,4600,5300,5000,0,3500,5500,5500,0,4000,4500,4500,0,3000,2500,2500,3000,4000,4000};
uint32_t delayTimePlus=2000;
	
void Uart2_servoCtr(uint8_t CMD){
	led_shan();
	switch(CMD)
	{
		case 1://��ʼλ��
			HAL_UART_Transmit(&huart2, uart2_ServoInit, 7, 0xffff);
			break;
		case 2://̧��λ�ã�׼��ץ����(ԭ����)
			HAL_UART_Transmit(&huart2, uart2_PreCatch, 7, 0xffff);
			break;
		case 3://yuan liao qu
			HAL_UART_Transmit(&huart2, uart2_Catch1, 7, 0xffff);
			HAL_Delay(3300);
			break;
		case 4:
			HAL_UART_Transmit(&huart2, uart2_Catch2, 7, 0xffff);
			HAL_Delay(3500);
			break;
		case 5:
			HAL_UART_Transmit(&huart2, uart2_Catch3, 7, 0xffff);
			HAL_Delay(3500);
			break;
		case 6:
			HAL_UART_Transmit(&huart2, uart2_Catch4, 7, 0xffff);
			HAL_Delay(4600);
			break;
		case 7:
			HAL_UART_Transmit(&huart2, uart2_Catch5, 7, 0xffff);
			HAL_Delay(5300);
			break;
		case 8:
			HAL_UART_Transmit(&huart2, uart2_Catch6, 7, 0xffff);
			HAL_Delay(5000);
			break;
		case 9://cu jia gong
			HAL_UART_Transmit(&huart2, uart2_Catch7, 7, 0xffff);
			HAL_Delay(5500);
			break;
		case 10:
			HAL_UART_Transmit(&huart2, uart2_Catch8, 7, 0xffff);
			HAL_Delay(3500);
			break;
		case 11:
			HAL_UART_Transmit(&huart2, uart2_Catch9, 7, 0xffff);
			HAL_Delay(5500);
			break;
		case 12:
			HAL_UART_Transmit(&huart2, uart2_Catch10, 7, 0xffff);
			HAL_Delay(4500);
			break;
		case 13:
			HAL_UART_Transmit(&huart2, uart2_Catch11, 7, 0xffff);
			HAL_Delay(4000);
			break;
		case 14:
			HAL_UART_Transmit(&huart2, uart2_Catch12, 7, 0xffff);
			HAL_Delay(4500);
			break;
		case 15://cheng pin qu
			HAL_UART_Transmit(&huart2, uart2_Catch13, 7, 0xffff);
			HAL_Delay(2500);
			break;
		case 16:
			HAL_UART_Transmit(&huart2, uart2_Catch14, 7, 0xffff);
			HAL_Delay(3000);
			break;
		case 17:
			HAL_UART_Transmit(&huart2, uart2_Catch15, 7, 0xffff);
			HAL_Delay(2500);
			break;
		case 18:
			HAL_UART_Transmit(&huart2, uart2_Catch16, 7, 0xffff);
			HAL_Delay(4000);
			break;
		case 19:
			HAL_UART_Transmit(&huart2, uart2_Catch17, 7, 0xffff);
			HAL_Delay(3000);
			break;
		case 20:
			HAL_UART_Transmit(&huart2, uart2_Catch18, 7, 0xffff);
			HAL_Delay(4000);
			break;
	}
	HAL_Delay(delayTimePlus);
}

uint8_t RxBuff[1];      //�����жϽ������ݵ�����
uint8_t DataBuff[50]; //������յ������ݵ����飬���ս������
uint8_t Color[6]; //������յ�����ɫ������

int RxLine=0;           //���յ������ݳ���

void Uart3_readQRcode()
{
	Ov3Mode=Ov3Mode_QrCode;
	HAL_UART_Receive_IT(&huart3, (uint8_t *)RxBuff, 1); //�򿪴����жϽ���
	while(uart3ok==0){
		HAL_UART_Transmit(&huart3, CMD_Serial, 10, 0xffff);
		x_speed=5;
		HAL_Delay(20);
		x_speed=-5;
		HAL_Delay(20);
		x_speed=0;
	}
	WriteNum_Max7219(Max7219_String);
	uart3ok=0;
	led_shan();
}


char oled_str[6]="000000";
void Uart3_readColor()
{
	Ov3Mode=Ov3Mode_ColorBlock;
	HAL_UART_Receive_IT(&huart3, (uint8_t *)RxBuff, 1); //�򿪴����жϽ���
	while(uart3ok==0){
		HAL_UART_Transmit(&huart3, CMD_Material, 12, 0xffff);
		user_main_printf("%s",CMD_Material);
		x_speed=5;
		HAL_Delay(20);
		x_speed=-5;
		HAL_Delay(20);
		x_speed=0;
	}

	ssd1306_SetCursor(10, 20);
	sprintf(oled_str,"%c%c%c/%c%c%c",Color[0],Color[1],Color[2],Color[3],Color[4],Color[5]);
  ssd1306_WriteString(oled_str, Font_16x26, Black);
//	ssd1306_SetCursor(20, 33);
//  ssd1306_WriteString("RBG", Font_11x18, Black);
	ssd1306_UpdateScreen(&hi2c1);
	uart3ok=0;
}

void HAL_UART_RxCpltCallback(UART_HandleTypeDef*UartHandle)
{
	static char setVal,RxLine_Copy,i;
		
		if(RxLine==0)//��ʼ��
		{
			RxLine_Copy=0;//��ʼ��
			uart3ok=0;//��ʼ����Ĭ��״̬����δ���
			switch(Ov3Mode)
			{
				case Ov3Mode_QrCode:
					setVal = 11;//"123+321"\r\n
				break;
				case Ov3Mode_ColorBlock:
					setVal = 10;//"RGBBRG"\r\n
				break;
			}
		}
	
		
    RxLine++;                      //ÿ���յ�һ�����ݣ�����ص����ݳ��ȼ�1
    DataBuff[RxLine-1]=RxBuff[0];  //��ÿ�ν��յ������ݱ��浽��������
    
    if((DataBuff[RxLine-1] == 0x0A)&&(DataBuff[RxLine-2] == 0x0D))            //���ս�����־λ��������ݿ����Զ��壬����ʵ����������ֻ��ʾ��ʹ�ã���һ����0xff
    {
        printf("Uart3-RXLen=%d\r\n",RxLine); 
        for(int i=0;i<RxLine;i++)
					printf("UART DataBuff[%d] = 0x%x\r\n",i,DataBuff[i]);                               

				RxLine_Copy=RxLine;
				RxLine=0;  //��ս��ճ���
				
				
				
				if(RxLine_Copy<setVal)
				{	
					//HAL_UART_Receive_IT(&huart3, (uint8_t *)RxBuff, 1); //���ܳ��Ȳ�����Ҫ�������գ�������һ�δ����жϽ��գ�����ֻ�����һ�����ݾ�ֹͣ����
				}
				else
				{
					uart3ok=1;
					RxLine_Copy=0;
					switch(Ov3Mode)
					{
						case Ov3Mode_QrCode:
							Max7219_String[0]=DataBuff[1];
							Max7219_String[1]=DataBuff[2];
							Max7219_String[2]=DataBuff[3];
							Max7219_String[5]=DataBuff[5];
							Max7219_String[6]=DataBuff[6];
							Max7219_String[7]=DataBuff[7];
						break;
						case Ov3Mode_ColorBlock:
							for(i=0;i<6;i++)
							{
								Color[i]=DataBuff[6-i];
							}
						break;
					}
					memset(DataBuff,0,sizeof(DataBuff));  //��ջ�������
					RxBuff[0]=0;
				}
				
		}
		

		if(RxLine_Copy<setVal)
		{	
			HAL_UART_Receive_IT(&huart3, (uint8_t *)RxBuff, 1); //���ܳ��Ȳ�����Ҫ�������գ�������һ�δ����жϽ��գ�����ֻ�����һ�����ݾ�ֹͣ����
		}
		
		
}

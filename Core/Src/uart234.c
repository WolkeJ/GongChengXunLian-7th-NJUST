#include "uart234.h"
#include "user_usart.h"
#include "usart.h"
#include <stdio.h>
#include "i2c.h"
#include "MAX7219/max7219_matrix.h"
#include "MAX7219/max7219.h"
#include <string.h>
#include "SCS_servo/SCS_servo.h"

uint8_t uart3ok=0;

uint8_t Ov3Mode = Ov3Mode_QrCode;
uint8_t Max7219_String[]="123--213";
uint8_t CMD_Serial[] = "CMD_Serial";
uint8_t CMD_Material[] = "CMD_Material\r\n";
uint8_t colororder[]={0,0,0,0,0,0};//����LED�������ʾ���������ҵ�˳����ʾ����Ļ��

extern int32_t y_speed;
extern int32_t x_speed;


//uint32_t Catch1Time[6]={5000,5500,5500,7800,6000,6500};

//uint8_t uart2_ServoInit[]={0x55,0x55,0x05,0x06,0x01,0x01,0x00};//������1����ʼ��
//uint8_t uart2_PreCatch[]={0x55,0x55,0x05,0x06,0x02,0x01,0x00};//������2��Ԥץȡ
///*yuan liao qu,from up to down,left to right*/ 
//uint8_t uart2_Catch1[]={0x55,0x55,0x05,0x06,0x03,0x01,0x00};//������3��ץ��1, 5s
//uint8_t uart2_Catch2[]={0x55,0x55,0x05,0x06,0x04,0x01,0x00};//������4��ץ��2, 5s
//uint8_t uart2_Catch3[]={0x55,0x55,0x05,0x06,0x05,0x01,0x00};//������5��ץ��3, 5s
//uint8_t uart2_Catch4[]={0x55,0x55,0x05,0x06,0x06,0x01,0x00};//������6��ץ��1, 6s
//uint8_t uart2_Catch5[]={0x55,0x55,0x05,0x06,0x07,0x01,0x00};//������7��ץ��2, 5.3s
//uint8_t uart2_Catch6[]={0x55,0x55,0x05,0x06,0x08,0x01,0x00};//������8��ץ��3, 5s
///*cu jia gong qu fang,from left to right*/
//uint8_t uart2_Catch7[]={0x55,0x55,0x05,0x06,0x0c,0x01,0x00};//������12
//uint8_t uart2_Catch8[]={0x55,0x55,0x05,0x06,0x0a,0x01,0x00};//������10
//uint8_t uart2_Catch9[]={0x55,0x55,0x05,0x06,0x0b,0x01,0x00};//������11
///*cu jia gong qu qu,from left to right*/
//uint8_t uart2_Catch10[]={0x55,0x55,0x05,0x06,0x10,0x01,0x00};//������16
//uint8_t uart2_Catch11[]={0x55,0x55,0x05,0x06,0x0e,0x01,0x00};//������14
//uint8_t uart2_Catch12[]={0x55,0x55,0x05,0x06,0x0f,0x01,0x00};//������15
///*cheng pin qu,from up to down,left to right*/
//uint8_t uart2_Catch13[]={0x55,0x55,0x05,0x06,0x14,0x01,0x00};//������20
//uint8_t uart2_Catch14[]={0x55,0x55,0x05,0x06,0x12,0x01,0x00};//������18
//uint8_t uart2_Catch15[]={0x55,0x55,0x05,0x06,0x13,0x01,0x00};//������19
//uint8_t uart2_Catch16[]={0x55,0x55,0x05,0x06,0x17,0x01,0x00};//������23
//uint8_t uart2_Catch17[]={0x55,0x55,0x05,0x06,0x15,0x01,0x00};//������21
//uint8_t uart2_Catch18[]={0x55,0x55,0x05,0x06,0x16,0x01,0x00};//������22

//uint32_t delayTime[]={0,0,3300,3500,3500,4600,5300,5000,0,3500,5500,5500,0,4000,4500,4500,0,3000,2500,2500,3000,4000,4000};
//uint32_t delayTimePlus=2000;
	
	
/*
���ƻ�е��ִ���ض����� (����ʽ)

CMD			action
1				��ʼλ��
2				̧��λ�ã�׼��ץ����(ԭ����)
---ԭ����---
3				ץ����
4				ץ����
5				ץ����
6				ץ����
7				ץ����
8				ץ����
---�ּӹ�---
9				ץ����
10			ץ����
11			ץ����
12			ץ����
13			ץ����
14			ץ����
---���ӹ�---
15			˳��ͬ��
16
17
18
19
20
*/
void Uart2_servoCtr(uint8_t CMD){
	DoGroup(CMD-1);
	led_shan();
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
	colororder[0]=Max7219_String[0];
	colororder[1]=Max7219_String[1];
	colororder[2]=Max7219_String[2];
	colororder[3]=Max7219_String[5];
	colororder[4]=Max7219_String[6];
	colororder[5]=Max7219_String[7];
	MAX7219_mywrite(colororder);
	MAX7219_MatrixUpdate();
	uart3ok=0;
	led_shan();
}


char led_str[6]="000000";
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
	sprintf(led_str,"%c%c%c/%c%c%c",Color[0],Color[1],Color[2],Color[3],Color[4],Color[5]);
	user_main_printf("color result: %s",led_str);
	uart3ok=0;
}

void HAL_UART_RxCpltCallback_color()//�ڴ���3�Ľ��ջص������е��ô˺���������openMV���ص���ɫʶ������
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

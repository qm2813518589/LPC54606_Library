/*********************************************************************************************************************
 * COPYRIGHT NOTICE
 * Copyright (c) 2018,��ɿƼ�
 * All rights reserved.
 * ��������QQȺ��179029047
 *
 * �����������ݰ�Ȩ������ɿƼ����У�δ��������������ҵ��;��
 * ��ӭ��λʹ�ò������������޸�����ʱ���뱣����ɿƼ��İ�Ȩ������
 *
 * @file       		�����
 * @company	   		�ɶ���ɿƼ����޹�˾
 * @author     		��ɿƼ�(QQ3184284598)
 * @version    		�鿴common.h��VERSION�궨��
 * @Software 		IAR 7.8 or MDK 5.24a
 * @Target core		LPC54606J512BD100
 * @Taobao   		https://seekfree.taobao.com/
 * @date       		2018-05-24
 * @note		
					���߶��壺
					------------------------------------ 
						ģ��ܽ�            ��Ƭ���ܽ�
						SDA(51��RX)         A9
						SCL(51��TX)         A8
						���ж�(VSY)         A0
						���ж�(HREF)		δʹ�ã���˲���
						�����ж�(PCLK)      A1        
						���ݿ�(D0-D7)		B24-B31
					------------------------------------ 
	
					Ĭ�Ϸֱ�����            188*120
					Ĭ��FPS                 50֡
 ********************************************************************************************************************/



#ifndef _SEEKFREE_MT9V032_h
#define _SEEKFREE_MT9V032_h

#include "common.h"
#include "LPC546XX_uart.h"

//��������ͷ����
#define MT9V032_DMA_CH          DMA_CH0         //��������ͷ��DMA�ɼ�ͨ��
#define MT9V032_W               188             //ͼ����   ��Χ1-752     K60�ɼ���������188
#define MT9V032_H               120             //ͼ��߶�	��Χ1-480



//--------------------------------------------------------------------------------------------------
//��������
//--------------------------------------------------------------------------------------------------
#define MT9V032_COF_UART        USART_5         //��������ͷ��ʹ�õ��Ĵ���     
#define MT9V032_COF_UART_TX     UART5_TX_A9
#define MT9V032_COF_UART_RX     UART5_RX_A8


#define MT9V032_VSYNC_PINT      PINT_CH7        //���ж���ʹ�õ�PINT�ж�ͨ��
#define MT9V032_VSYNC_PIN       A0              //���ж�����
#define MT9V032_VSYNC_IRQN      PIN_INT7_IRQn   //�жϺ�

#define MT9V032_PCLK_SCT        SCT_INPUT_MUX1  //SCT0_CAMERA_PCLK  �˴��޸ĺ�Ӧͬ����sct.h�ļ��ڵ�SCT0_CAMERA_PCLK�궨���޸���˴�һ��
#define MT9V032_PCLK            SCT0_GPI1_A1    //����ͷ����ʱ��



//--------------------------------------------------------------------------------------------------
//        ����ͷ���ݽӿ�
//        GPIO_PIN(B,0)
//        ��һ������B������ʹ��B�ڣ�����ѡ��A��B�ֱ��ӦGPIO_PIN(A,0)��GPIO_PIN(B,0)
//        �ڶ�������3������ʹ�õ���B24 - B31��,
//		  ����ѡ��0��1��2��3���ֱ��Ӧ����	�ö˿ڵ�X0-X7��X8-X15��X16-X23��X24-X31(����x������ǵ�һ����ĸ���������ڵľ���B24 - B31)
//--------------------------------------------------------------------------------------------------
#define MT9V032_DATAPORT 		GPIO_PIN(B,3)	                //DMA���ݿ�

                        
                        
                        
//����ͷ����ö��
typedef enum
{
    INIT = 0,               //����ͷ��ʼ������
    AUTO_EXP,               //�Զ��ع�����
    EXP_TIME,               //�ع�ʱ������
    FPS,                    //����ͷ֡������
    SET_COL,                //ͼ��������
    SET_ROW,                //ͼ��������
    LR_OFFSET,              //ͼ������ƫ������
    UD_OFFSET,              //ͼ������ƫ������
    GAIN,                   //ͼ��ƫ������
    CONFIG_FINISH,          //������λ����Ҫ����ռλ����
    
    SET_EXP_TIME = 0XF0,    //���������ع�ʱ������
    GET_STATUS,             //��ȡ����ͷ��������
    GET_VERSION,            //�̼��汾������
	
    SET_ADDR = 0XFE,        //�Ĵ�����ַ����
    SET_DATA                //�Ĵ�����������
}CMD;
     
     
     
                            
                            

extern uint8 mt9v032_finish_flag;  //һ��ͼ��ɼ���ɱ�־λ
extern uint8 image[MT9V032_H][MT9V032_W];      //�ڲ�ʹ��
                            
                            
                        
void  mt9v032_cof_uart_interrupt(void);
void  mt9v032_vsync(void);
void  camera_init(void);
void  mt9v032_dma(void);
void  seekfree_sendimg_032(UARTN_enum uartn);




#endif


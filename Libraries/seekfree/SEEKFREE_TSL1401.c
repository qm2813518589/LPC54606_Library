/*********************************************************************************************************************
 * COPYRIGHT NOTICE
 * Copyright (c) 2017,��ɿƼ�
 * All rights reserved.
 * ��������QQȺ��179029047
 *
 * �����������ݰ�Ȩ������ɿƼ����У�δ��������������ҵ��;��
 * ��ӭ��λʹ�ò������������޸�����ʱ���뱣����ɿƼ��İ�Ȩ������
 *
 * @file       		NRF24L01
 * @company	   		�ɶ���ɿƼ����޹�˾
 * @author     		��ɿƼ�(QQ3184284598)
 * @version    		v2.0
 * @Software 		IAR 7.7 or MDK 5.23
 * @Target core		S9KEA128AMLK
 * @Taobao   		https://seekfree.taobao.com/
 * @date       		2017-11-6
 * @note	
					���߶���
					------------------------------------ 
					    ģ��ܽ�       	��Ƭ���ܽ�
					    CCD_AO	   		�鿴SEEKFREE_TSL1401�ļ��ڵ�AD_CHANNEL2   �궨��
                        CCD_AO	   		�鿴SEEKFREE_TSL1401�ļ��ڵ�AD_CHANNEL1   �궨��
					    CCD_CLK	   		�鿴SEEKFREE_TSL1401�ļ��ڵ�CCD_CLK_PORT  �궨��
					    CCD_SI	   		�鿴SEEKFREE_TSL1401�ļ��ڵ�CCD_SI_PORT   �궨��
					------------------------------------ 
 ********************************************************************************************************************/



#include "SEEKFREE_TSL1401.h"




#define CCD_CLK_PORT   B1                     	//CCD CLK���Ŷ���
#define CCD_SI_PORT    B0                     	//CCD SI ���Ŷ���
#define AD_CHANNEL1    ADC0_SE6                 //����CCD1 AD�ɼ�ͨ��  B2
#define AD_CHANNEL2    ADC0_SE7                 //����CCD2 AD�ɼ�ͨ��  B3
#define EXPOSURE_TIME  10                       //����CCD�ع�ʱ�� ��λMS



#define CCD_CLK(x)     gpio_set (CCD_CLK_PORT, x)
#define CCD_SI(x)      gpio_set (CCD_SI_PORT,  x)


uint16 ccd_data1[128];                           //CCD1����
uint16 ccd_data2[128];                           //CCD2����

//-------------------------------------------------------------------------------------------------------------------
//  @brief      TSL1401����CCD��ʼ��
//  @param      NULL
//  @return     void
//  @since      v1.0
//  Sample usage:			
//-------------------------------------------------------------------------------------------------------------------
void ccd_init(void)     
{       
    adc_init (AD_CHANNEL1);                     //B2
    adc_init (AD_CHANNEL2);                     //B3
    gpio_init(CCD_CLK_PORT, GPO, 1);            //CLK
    gpio_init(CCD_SI_PORT,  GPO, 1);            //SI
    
    DisableInterrupts;
    pit_init_ms(pit0, EXPOSURE_TIME);   		//��ʱEXPOSURE_TIME(ms) ���ж�
    set_irq_priority(PIT_CH0_IRQn,1);			//�������ȼ�,�����Լ����������� �����÷�ΧΪ 0 - 3  ԽС���ȼ�Խ��
    enable_irq(PIT_CH0_IRQn);                   //ʹ��PIT0�ж�
    EnableInterrupts;
}


//-------------------------------------------------------------------------------------------------------------------
//  @brief      TSL1401����CCD���ݲɼ�
//  @param      NULL
//  @return     void
//  @since      v1.0
//  Sample usage:				��isr.c�����ȴ�����Ӧ���жϺ�����Ȼ����øú���(֮�����������жϱ�־λ)
//-------------------------------------------------------------------------------------------------------------------
void ccd_collect(void)
{
    uint16 i = 0;

 
    CCD_CLK(1);
    CCD_SI(0);
    

    CCD_SI(1);
    CCD_CLK(0);
    

    CCD_CLK(1);
    CCD_SI(0);
    
    
    for(i=0;i<128;i++)
    {
        CCD_CLK(0);   
        ccd_data1[i] = adc_once(AD_CHANNEL1, ADC_12bit);     
        ccd_data2[i] = adc_once(AD_CHANNEL2, ADC_12bit);    
        CCD_CLK(1);
    }
}

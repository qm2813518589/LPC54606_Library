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



#ifndef _TSL1401_c
#define _TSL1401_c

#include "headfile.h"

extern uint16 ccd_data[128];                   //CCD����


void ccd_init(void);
void ccd_collect(void);
















#endif


/*-----------------------------------------------------------------------------
 * Name:    Audio_MCB54110.h
 * Purpose: Audio BSP Header for MCB541100
 * Rev.:    1.0.0
 *----------------------------------------------------------------------------*/

/* Copyright (c) 2013 - 2016 ARM LIMITED

   All rights reserved.
   Redistribution and use in source and binary forms, with or without
   modification, are permitted provided that the following conditions are met:
   - Redistributions of source code must retain the above copyright
     notice, this list of conditions and the following disclaimer.
   - Redistributions in binary form must reproduce the above copyright
     notice, this list of conditions and the following disclaimer in the
     documentation and/or other materials provided with the distribution.
   - Neither the name of ARM nor the names of its contributors may be used
     to endorse or promote products derived from this software without
     specific prior written permission.
   *
   THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
   AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
   IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE
   ARE DISCLAIMED. IN NO EVENT SHALL COPYRIGHT HOLDERS AND CONTRIBUTORS BE
   LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR
   CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF
   SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS
   INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN
   CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE)
   ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE
   POSSIBILITY OF SUCH DAMAGE.
   ---------------------------------------------------------------------------*/
#ifndef __AUDIO_MCB54110_H
#define __AUDIO_MCB54110_H

#include "Board_Audio.h"
#include "RTE_Components.h"             // Component selection

#ifdef RTE_BSP_AUDIO_MIC_IN
extern int32_t  Audio_DMIC_Initialize (Audio_SignalEvent_t cb_event);
extern int32_t  Audio_DMIC_Uninitialize (void);
extern int32_t  Audio_DMIC_ReceiveData (void *data, uint32_t num);
extern uint32_t Audio_DMIC_GetDataRxCount (void);
extern int32_t  Audio_DMIC_Start (void);
extern int32_t  Audio_DMIC_Stop (void);
extern int32_t  Audio_DMIC_Pause (void);
extern int32_t  Audio_DMIC_Resume (void);
extern int32_t  Audio_DMIC_SetVolume (uint8_t channel, uint8_t volume);
extern int32_t  Audio_DMIC_SetMute (uint8_t channel, bool mute);
extern int32_t  Audio_DMIC_SetDataFormat (uint8_t format);
extern int32_t  Audio_DMIC_SetFrequency (uint32_t frequency);
#endif

#ifdef RTE_BSP_AUDIO_CODEC_OUT
extern int32_t  Audio_WM8904_Initialize (Audio_SignalEvent_t cb_event);
extern int32_t  Audio_WM8904_Uninitialize (void);
extern int32_t  Audio_WM8904_SendData (const void *data, uint32_t num);
extern int32_t  Audio_WM8904_ReceiveData (void *data, uint32_t num);
extern uint32_t Audio_WM8904_GetDataTxCount (void);
extern uint32_t Audio_WM8904_GetDataRxCount (void);
extern int32_t  Audio_WM8904_Start (uint8_t stream);
extern int32_t  Audio_WM8904_Stop (uint8_t stream);
extern int32_t  Audio_WM8904_Pause (uint8_t stream);
extern int32_t  Audio_WM8904_Resume (uint8_t stream);
extern int32_t  Audio_WM8904_SetVolume (uint8_t stream, uint8_t channel, uint8_t volume);
extern int32_t  Audio_WM8904_SetMute (uint8_t stream, uint8_t channel, bool mute);
extern int32_t  Audio_WM8904_SetDataFormat (uint8_t stream, uint8_t format);
extern int32_t  Audio_WM8904_SetFrequency (uint8_t stream, uint32_t frequency);
#endif

#endif /* __AUDIO_MCB54110_H */

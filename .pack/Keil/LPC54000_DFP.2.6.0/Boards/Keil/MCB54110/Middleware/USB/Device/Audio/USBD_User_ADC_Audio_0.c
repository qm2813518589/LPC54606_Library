/*------------------------------------------------------------------------------
 * MDK Middleware - Component ::USB:Device
 * Copyright (c) 2004-2017 ARM Germany GmbH. All rights reserved.
 *------------------------------------------------------------------------------
 * Name:    USBD_User_ADC_Audio_0.c
 * Purpose: USB Device Audio Device Class (ADC) User module
 *          Template for USB Audio Device Class (ADC) Sound Card
 * Rev.:    V6.3.5
 *----------------------------------------------------------------------------*/
/**
 * \addtogroup usbd_adcFunctions
 *
 * USBD_User_ADC_Audio_0.c implements the application specific
 * functionality of the Audio class and is used to create an USB Audio device
 * containing mono/stereo speakers and mono/stereo microphone.
 *
 * The implementation uses settings from USBD_Config_ADC_0.h file.
 *
 */
 
 
//! [code_USBD_User_ADC]
 
#include <stdint.h>
#include <stdbool.h>
#include <string.h>
#include "rl_usb.h"
 
#include ".\RTE\USB\USBD_Config_ADC_0.h"
 
#include "Board_Audio.h"
 
// Imported settings from USBD_Config_ADC_0.h file
#define  PLAYBACK_AVAILABLE                  (USBD_ADC0_EP_ISO_OUT_EN != 0U)
#define  PLAYBACK_CHANNELS                   (USBD_ADC0_OUT_CH_NUM)
#define  PLAYBACK_FREQ                       (USBD_ADC0_OUT_TSAM_FREQ)
#define  PLAYBACK_RESOLUTION                 (USBD_ADC0_OUT_BBITRESOLUTION)
#define  PLAYBACK_SAMPLE_SIZE               ((USBD_ADC0_OUT_BSUBFRAMESIZE >= 3U) ? 4U : USBD_ADC0_OUT_BSUBFRAMESIZE)
#define  PLAYBACK_USB_BUFFER_SAMPLES         (USBD_ADC0_OUT_BUF_SIZE)
#define  PLAYBACK_AUDIO_BUFFER_SAMPLES       (PLAYBACK_USB_BUFFER_SAMPLES / 8U)
#define  PLAYBACK_AUDIO_BUFFER_SIZE         ((PLAYBACK_AUDIO_BUFFER_SAMPLES + PLAYBACK_CHANNELS) * PLAYBACK_SAMPLE_SIZE)
 
#define  RECORD_AVAILABLE                    (USBD_ADC0_EP_ISO_IN_EN != 0U)
#define  RECORD_CHANNELS                     (USBD_ADC0_IN_CH_NUM)
#define  RECORD_FREQ                         (USBD_ADC0_IN_TSAM_FREQ)
#define  RECORD_RESOLUTION                   (USBD_ADC0_IN_BBITRESOLUTION)
#define  RECORD_SAMPLE_SIZE                 ((USBD_ADC0_IN_BSUBFRAMESIZE >= 3U) ? 4U : USBD_ADC0_IN_BSUBFRAMESIZE)
#define  RECORD_USB_BUFFER_SAMPLES           (USBD_ADC0_IN_BUF_SIZE)
#define  RECORD_AUDIO_BUFFER_SAMPLES         (RECORD_USB_BUFFER_SAMPLES / 8U)
#define  RECORD_AUDIO_BUFFER_SIZE           ((RECORD_AUDIO_BUFFER_SAMPLES + RECORD_CHANNELS) * RECORD_SAMPLE_SIZE)
 
// Supported configuration check
#if     (PLAYBACK_AVAILABLE != 0U)
#if    ((PLAYBACK_CHANNELS == 0U) || (PLAYBACK_CHANNELS > 2U))
#error   Playback channel configuration mono or stereo supported!
#endif
#endif
#if     (RECORD_AVAILABLE != 0U)
#if    ((RECORD_CHANNELS == 0U) || (RECORD_CHANNELS > 2U))
#error   Recording channel configuration mono or stereo supported!
#endif
#endif
 
 
#if     (PLAYBACK_AVAILABLE != 0U)
static   uint8_t  play_idx = 0U;
static   uint32_t play_num = 0U;
static   uint8_t  play_buf [2][PLAYBACK_AUDIO_BUFFER_SIZE] __attribute__((aligned(4U)));
#endif

#if     (RECORD_AVAILABLE != 0U)
static   uint8_t  rec_idx = 0U;
static   uint32_t rec_num = 0U;
static   uint8_t  rec_buf [2][RECORD_AUDIO_BUFFER_SIZE]    __attribute__((aligned(4U)));
#endif
 
#if ((PLAYBACK_AVAILABLE != 0U) && (PLAYBACK_RESOLUTION == 24U))
static void Repack_shl_8 (uint8_t *ptr, uint32_t num) {
  uint32_t i;
 
  for (i = 0; i < num; i ++) {
    *(__packed uint32_t *)ptr = ((*(__packed uint32_t *)ptr) << 8U) & 0xFFFFFF00U;
    ptr += 4U;
  }
}
#endif
 
// Audio Events Handling Callback function.
// \param[in]   event  notification mask
static void AudioCallback (uint32_t event) {
  uint32_t samples_available;
#if (PLAYBACK_AVAILABLE != 0U)
   int32_t num;
#endif

  // Restart new audio reception and new transmission first
#if (RECORD_AVAILABLE != 0U)
  if (event & AUDIO_EVENT_RECEIVE_COMPLETE) {
    // Start new audio data reception (double buffering)
    Audio_ReceiveData ((void *)(&rec_buf[rec_idx ^ 1U][0U]), RECORD_AUDIO_BUFFER_SAMPLES);
  }
#endif
#if (PLAYBACK_AVAILABLE != 0U)
  if ((event & AUDIO_EVENT_SEND_COMPLETE) && (play_num != 0U)) {
    // Start new audio data transmission (double buffering)
    Audio_SendData ((void *)(&play_buf[play_idx][0U]), (uint32_t)(play_num));
    play_idx ^= 1U;                     // Change active buffer index
  }
#endif
 
#if (RECORD_AVAILABLE != 0U)
  // Handling of recording
  if (event & AUDIO_EVENT_RECEIVE_COMPLETE) {
    // Compensate frequency difference between USB and Audio Interface
    // by adding or removing 1 sample for mono or 2 samples for stereo
    samples_available = USBD_ADC_WrittenSamplesPending (0U) + rec_num;
    rec_num = RECORD_AUDIO_BUFFER_SAMPLES;
    if (samples_available <= (2U * RECORD_AUDIO_BUFFER_SAMPLES)) {
      // Add (repeat last) 1 sample for mono or 2 samples for stereo
      // to send to USB if USB is faster than Audio Interface
      memcpy((void *)(&rec_buf[rec_idx][ RECORD_AUDIO_BUFFER_SAMPLES                    * RECORD_SAMPLE_SIZE]),
             (void *)(&rec_buf[rec_idx][(RECORD_AUDIO_BUFFER_SAMPLES - RECORD_CHANNELS) * RECORD_SAMPLE_SIZE]),
              RECORD_CHANNELS * RECORD_SAMPLE_SIZE);
      rec_num += RECORD_CHANNELS;
    } else if (samples_available >= (RECORD_USB_BUFFER_SAMPLES - (2U * RECORD_AUDIO_BUFFER_SAMPLES))) {
      // Remove 1 sample for mono or 2 samples for stereo 
      // to send to USB if USB is slower than Audio Interface
      rec_num -= RECORD_CHANNELS;
    }
 
    // Send last received buffer of samples to USB
    USBD_ADC_WriteSamples (0U, (void *)(&rec_buf[rec_idx][0U]), (int32_t)(rec_num));
    rec_idx ^= 1U;                      // Change active buffer index
  }
#endif
 
#if (PLAYBACK_AVAILABLE != 0U)
  // Handling of playback
  if ((event & AUDIO_EVENT_SEND_COMPLETE) && (play_num != 0U)) {
    // Read next buffer of samples from USB
    num = USBD_ADC_ReadSamples (0U, (void *)(&play_buf[play_idx][0U]), PLAYBACK_AUDIO_BUFFER_SAMPLES);
    if (num > 0) {
      play_num = (uint32_t)(num);
    } else {
      play_num = 0U;
    }
#if (PLAYBACK_RESOLUTION == 24U)
    // If samples need to be repacked form relevant 24-bit to 32-bit
    // for 32-bit Audio Interface sending (playing)
    Repack_shl_8 (&play_buf[play_idx][0U], play_num);
#endif
 
    // Compensate frequency difference between USB and Audio Interface
    // by adding or removing 1 sample for mono or 2 samples for stereo
    samples_available = USBD_ADC_ReceivedSamplesAvailable (0U) + play_num;
    if (samples_available == 0U) {
      // Stop playback audio stream
      Audio_Stop  (AUDIO_STREAM_OUT);
    } else if (samples_available <= (2U * PLAYBACK_AUDIO_BUFFER_SAMPLES)) {
      // Add (repeat last) 1 sample for mono or 2 samples for stereo
      // to send to Audio Interface if USB is slower than Audio Interface
      memcpy((void *)(&play_buf[play_idx][ play_num                      * PLAYBACK_SAMPLE_SIZE]),
             (void *)(&play_buf[play_idx][(play_num - PLAYBACK_CHANNELS) * PLAYBACK_SAMPLE_SIZE]),
              PLAYBACK_CHANNELS * PLAYBACK_SAMPLE_SIZE);
      play_num += PLAYBACK_CHANNELS;
    } else if (samples_available >= (PLAYBACK_USB_BUFFER_SAMPLES - (2U * PLAYBACK_AUDIO_BUFFER_SAMPLES))) {
      // Remove 1 sample for mono or 2 samples for stereo
      // to send to Audio Interface if USB is faster than Audio Interface
      play_num -= PLAYBACK_CHANNELS;
    }
  }
#endif
}
 
#if (PLAYBACK_AVAILABLE != 0U)
// Callback function called when data in USB buffer for speaker samples reaches half-full from empty
void USBD_ADC0_ReceivedSamples (void) {
  int32_t num;
 
  // Read initial data for playback
  num = USBD_ADC_ReadSamples (0U, (void *)(&play_buf[0U][0U]), PLAYBACK_AUDIO_BUFFER_SAMPLES);
  if (num > 0) {
    play_num = (uint32_t)(num);
  } else {
    play_num = 0U;
  }
 
#if (PLAYBACK_RESOLUTION == 24U)
  // If samples need to be repacked form relevant 24-bit to 32-bit
  // for 32-bit Audio Interface sending (playing)
  Repack_shl_8 (&play_buf[0U][0U], play_num);
#endif
  // Start initial audio data transmission (play)
  Audio_SendData ((void *)(&play_buf[0U][0U]), play_num);

  // Start playback audio stream
  Audio_Start (AUDIO_STREAM_OUT);

  // Read next data for playback from Audio callback
  play_idx = 1U;
  num = USBD_ADC_ReadSamples (0U, (void *)(&play_buf[1U][0U]), PLAYBACK_AUDIO_BUFFER_SAMPLES);
  if (num > 0) {
    play_num = (uint32_t)(num);
  } else {
    play_num = 0U;
  }
 
#if (PLAYBACK_RESOLUTION == 24U)
  // If samples need to be repacked form relevant 24-bit to 32-bit
  // for 32-bit Audio Interface sending (playing)
  Repack_shl_8 (&play_buf[1U][0U], play_num);
#endif
}
#endif
 
// Callback function called during USBD_Initialize to initialize the USB ADC class instance.
void USBD_ADC0_Initialize (void) {
 
  // Initialize Audio Interface driver
  Audio_Initialize    (&AudioCallback);
 
#if (PLAYBACK_AVAILABLE != 0U)
  // Set stereo speakers volume range to 0 to 100 with step 1, initially positioned at 50 %
  USBD_ADC_SpeakerSetVolumeRange (0U, 0U, 0U, 100U, 1U, 50U);
 
  // Initialize Audio Interface for speakers according to USBD_Config_ADC_0.h file settings, not muted and volume at 50 %
  Audio_SetDataFormat (AUDIO_STREAM_OUT, PLAYBACK_SAMPLE_SIZE | ((PLAYBACK_CHANNELS - 1U) << 7));
  Audio_SetFrequency  (AUDIO_STREAM_OUT, PLAYBACK_FREQ);
  Audio_SetMute       (AUDIO_STREAM_OUT, AUDIO_CHANNEL_MASTER, false);
  Audio_SetVolume     (AUDIO_STREAM_OUT, AUDIO_CHANNEL_MASTER, 50U);
#endif
 
#if (RECORD_AVAILABLE != 0U)
  // Set microphone volume range to 0 to 100 with step 1, initially positioned at 50 %
  USBD_ADC_MicrophoneSetVolumeRange (0U, 0U, 0U, 100U, 1U, 50U);
 
  // Initialize Audio Interface for microphone according to USBD_Config_ADC_0.h file settings, not muted and volume at 50 %
  Audio_SetDataFormat (AUDIO_STREAM_IN, RECORD_SAMPLE_SIZE | ((RECORD_CHANNELS - 1U) << 7));
  Audio_SetFrequency  (AUDIO_STREAM_IN, RECORD_FREQ);
  Audio_SetMute       (AUDIO_STREAM_IN, AUDIO_CHANNEL_MASTER, false);
  Audio_SetVolume     (AUDIO_STREAM_IN, AUDIO_CHANNEL_MASTER, 50U);
#endif
}
 
// Callback function called during USBD_Uninitialize to de-initialize the USB ADC class instance.
void USBD_ADC0_Uninitialize (void) {
  Audio_Uninitialize ();
}
 
#if (PLAYBACK_AVAILABLE != 0U)
// Callback function called when play stopped.
void USBD_ADC0_PlayStop (void) {
  Audio_Stop  (AUDIO_STREAM_OUT);       // Stop playback audio stream
}
 
// Callback function called when speaker mute setting changed.
// \param[in]     ch            channel index.
//                                - value 0: master channel
//                                - value 1: left speaker (in stereo mode)
//                                - value 2: right speaker (in stereo mode)
// \param[in]     on            current mute setting.
//                                - false :  mute off
//                                - true :   mute on
void USBD_ADC0_SetSpeakerMute (uint8_t ch, bool on) {
  Audio_SetMute (AUDIO_STREAM_OUT, ch, on);
}
 
// Callback function called when speaker volume setting changed.
// \param[in]     ch            channel index.
//                                - value 0: master channel
//                                - value 1: left speaker (in stereo mode)
//                                - value 2: right speaker (in stereo mode)
// \param[in]     vol           current volume setting.
void USBD_ADC0_SetSpeakerVolume (uint8_t ch, uint16_t vol) {
  Audio_SetVolume (AUDIO_STREAM_OUT, ch, (uint8_t)(vol));
}
#endif
 
#if (RECORD_AVAILABLE != 0U)
// Callback function called when recording started.
void USBD_ADC0_RecordStart (void) {
  rec_idx = 0U;                         // Current receiving buffer index is 0
  rec_num = RECORD_AUDIO_BUFFER_SAMPLES;// Current receiving length
  // Start initial audio data reception (record)
  Audio_ReceiveData (&rec_buf[0U][0U], RECORD_AUDIO_BUFFER_SAMPLES);
  Audio_Start (AUDIO_STREAM_IN);        // Start recording audio stream
}
 
// Callback function called when recording stopped.
void USBD_ADC0_RecordStop (void) {
  Audio_Stop  (AUDIO_STREAM_IN);        // Stop recording audio stream
}
 
// Callback function called when microphone mute setting changed.
// \param[in]     ch            channel index.
//                                - value 0: master channel
//                                - value 1: left microphone (in stereo mode)
//                                - value 2: right microphone (in stereo mode)
// \param[in]     on            current mute setting.
//                                - false :  mute off
//                                - true :   mute on
void USBD_ADC0_SetMicrophoneMute (uint8_t ch, bool on) {
  Audio_SetMute (AUDIO_STREAM_IN, ch, on);
}
 
// Callback function called when microphone volume setting changed.
// \param[in]     ch            channel index.
//                                - value 0: master channel
//                                - value 1: left microphone (in stereo mode)
//                                - value 2: right microphone (in stereo mode)
// \param[in]     vol           current volume setting.
void USBD_ADC0_SetMicrophoneVolume (uint8_t ch, uint16_t vol) {
  Audio_SetVolume (AUDIO_STREAM_IN, ch, (uint8_t)(vol));
}
#endif
 
//! [code_USBD_User_ADC]

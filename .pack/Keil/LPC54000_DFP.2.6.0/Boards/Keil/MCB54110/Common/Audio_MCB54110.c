/*-----------------------------------------------------------------------------
 * Name:    Audio_MCB54110.c
 * Purpose: Audio BSP for MCB541100
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

#include "Audio_MCB54110.h"

// Driver functions

/**
  \fn          int32_t Audio_Initialize (Audio_SignalEvent_t cb_event)
  \brief       Initialize Audio Interface
  \param[in]   cb_event  pointer to event notification function
  \returns
   - \b  0: function succeeded
   - \b -1: function failed
*/
int32_t Audio_Initialize (Audio_SignalEvent_t cb_event) {

#ifdef RTE_BSP_AUDIO_MIC_IN
  if (Audio_DMIC_Initialize (cb_event) == -1) {
    return -1;
  }
#endif

#ifdef RTE_BSP_AUDIO_CODEC_OUT
  if (Audio_WM8904_Initialize (cb_event) == -1) {
    return -1;
  }
#endif

  return 0;
}

/**
  \fn          int32_t Audio_Uninitialize (void)
  \brief       De-initialize Audio Interface
  \returns
   - \b  0: function succeeded
   - \b -1: function failed
*/
int32_t Audio_Uninitialize (void) {
#ifdef RTE_BSP_AUDIO_MIC_IN
  if (Audio_DMIC_Uninitialize () == -1) {
    return -1;
  }
#endif

#ifdef RTE_BSP_AUDIO_CODEC_OUT
  if (Audio_WM8904_Uninitialize () == -1) {
    return -1;
  }
#endif

  return 0;
}

/**
  \fn          int32_t Audio_SendData (const void *data, uint32_t num)
  \brief       Start sending data to Audio output stream
  \param[in]   data  pointer to buffer with data to send
  \param[in]   num   number of data items to send
  \returns
   - \b  0: function succeeded
   - \b -1: function failed
*/
int32_t Audio_SendData (const void *data, uint32_t num) {
#ifdef RTE_BSP_AUDIO_CODEC_OUT
  if (Audio_WM8904_SendData (data, num) == -1) {
    return -1;
  }
#endif

  return 0;
}

/**
  \fn          int32_t Audio_ReceiveData (void *data, uint32_t num)
  \brief       Start receiving data from Audio input stream
  \param[out]  data  pointer to buffer for data to receive
  \param[in]   num   number of data items to send
  \returns
   - \b  0: function succeeded
   - \b -1: function failed
*/
int32_t Audio_ReceiveData (void *data, uint32_t num) {
#ifdef RTE_BSP_AUDIO_MIC_IN
  if (Audio_DMIC_ReceiveData (data, num) == -1) {
    return -1;
  }
#endif
#ifdef RTE_BSP_AUDIO_CODEC_IN
  if (Audio_WM8904_ReceiveData (data, num) == -1) {
    return -1;
  }
#endif

  return 0;
}

/**
  \fn          uint32_t Audio_GetDataTxCount (void)
  \brief       Get transmitted data count
  \returns     number of data items transmitted
*/
uint32_t Audio_GetDataTxCount (void) {
#ifdef RTE_BSP_AUDIO_CODEC_OUT
  return (Audio_WM8904_GetDataTxCount ());
#else
  return 0;
#endif
}

/**
  \fn          uint32_t Audio_GetDataRxCount (void)
  \brief       Get received data count
  \returns     number of data items received
*/
uint32_t Audio_GetDataRxCount (void) {
  uint32_t count;
  
  count = 0;
#ifdef RTE_BSP_AUDIO_MIC_IN
  count = Audio_DMIC_GetDataRxCount ();
#endif
#ifdef RTE_BSP_AUDIO_CODEC_IN
  count = Audio_WM8904_GetDataRxCount ();
#endif

  return count;
}

/**
  \fn          int32_t Audio_Start (uint8_t stream)
  \brief       Start Audio stream
  \param[in]   stream  stream identifier
  \returns
   - \b  0: function succeeded
   - \b -1: function failed
*/
int32_t Audio_Start (uint8_t stream) {

  if (stream == AUDIO_STREAM_IN) {
#ifdef RTE_BSP_AUDIO_MIC_IN
    if (Audio_DMIC_Start () == -1) {
      return -1;
    }
#endif
#ifdef RTE_BSP_AUDIO_CODEC_IN
    if (Audio_WM8904_Start (stream) == -1) {
      return -1;
    }
#endif
  } else {
#ifdef RTE_BSP_AUDIO_CODEC_OUT
    if (Audio_WM8904_Start (stream) == -1) {
      return -1;
    }
#endif
  }

  return 0;
}

/**
  \fn          int32_t Audio__Stop (uint8_t stream)
  \brief       Stop Audio stream
  \param[in]   stream  stream identifier
  \returns
   - \b  0: function succeeded
   - \b -1: function failed
*/
int32_t Audio_Stop (uint8_t stream) {
  if (stream == AUDIO_STREAM_IN) {
#ifdef RTE_BSP_AUDIO_MIC_IN
    if (Audio_DMIC_Stop () == -1) {
     return -1;
    }
#endif
#ifdef RTE_BSP_AUDIO_CODEC_IN
    if (Audio_WM8904_Stop (stream) == -1) {
      return -1;
    }
#endif
  } else {
#ifdef RTE_BSP_AUDIO_CODEC_OUT
    if (Audio_WM8904_Stop (stream) == -1) {
      return -1;
    }
#endif
  }

  return 0;
}

/**
  \fn          int32_t Audio_Pause (uint8_t stream)
  \brief       Pause Audio stream
  \param[in]   stream  stream identifier
  \returns
   - \b  0: function succeeded
   - \b -1: function failed
*/
int32_t Audio_Pause (uint8_t stream) {
  if (stream == AUDIO_STREAM_IN) {
#ifdef RTE_BSP_AUDIO_MIC_IN
    if (Audio_DMIC_Pause () == -1) {
      return -1;
    }
#endif
#ifdef RTE_BSP_AUDIO_CODEC_IN
    if (Audio_WM8904_Pause (stream) == -1) {
      return -1;
    }
#endif
  } else {
#ifdef RTE_BSP_AUDIO_CODEC_OUT
    if (Audio_WM8904_Pause (stream) == -1) {
      return -1;
    }
#endif
  }

  return 0;
}

/**
  \fn          int32_t Audio_Resume (uint8_t stream)
  \brief       Resume Audio stream
  \param[in]   stream  stream identifier
  \returns
   - \b  0: function succeeded
   - \b -1: function failed
*/
int32_t Audio_Resume (uint8_t stream) {
  if (stream == AUDIO_STREAM_IN) {
#ifdef RTE_BSP_AUDIO_MIC_IN
    if (Audio_DMIC_Resume () == -1) {
      return -1;
    }
#endif
#ifdef RTE_BSP_AUDIO_CODEC_IN
    if (Audio_WM8904_Resume (stream) == -1) {
      return -1;
    }
#endif
  } else {
#ifdef RTE_BSP_AUDIO_CODEC_OUT
    if (Audio_WM8904_Resume (stream) == -1) {
      return -1;
    }
#endif
  }

  return 0;
}

/**
  \fn          int32_t Audio_SetVolume (uint8_t stream, uint8_t channel, uint8_t volume)
  \brief       Set volume level for Audio stream
  \param[in]   stream   stream identifier
  \param[in]   channel  channel identifier
  \param[in]   volume   volume level (0..100)
  \returns
   - \b  0: function succeeded
   - \b -1: function failed
*/
int32_t Audio_SetVolume (uint8_t stream, uint8_t channel, uint8_t volume) {

  if (stream == AUDIO_STREAM_IN) {
#ifdef RTE_BSP_AUDIO_MIC_IN
    if (Audio_DMIC_SetVolume (channel, volume) == -1) {
      return -1;
    }
#endif
#ifdef RTE_BSP_AUDIO_CODEC_IN
    if (Audio_WM8904_SetVolume (stream, channel, volume) == -1) {
      return -1;
    }
#endif
  } else {
#ifdef RTE_BSP_AUDIO_CODEC_OUT
    if (Audio_WM8904_SetVolume (stream, channel, volume) == -1) {
      return -1;
    }
#endif
  }

  return 0;
}

/**
  \fn          int32_t Audio_SetMute (uint8_t stream, uint8_t channel, bool mute)
  \brief       Set mute state for Audio stream
  \param[in]   stream   stream identifier
  \param[in]   channel  channel identifier
  \param[in]   mute     mute state
  \returns
   - \b  0: function succeeded
   - \b -1: function failed
*/
int32_t Audio_SetMute (uint8_t stream, uint8_t channel, bool mute) {

  if (stream == AUDIO_STREAM_IN) {
#ifdef RTE_BSP_AUDIO_MIC_IN
    if (Audio_DMIC_SetMute (channel, mute) == -1) {
      return -1;
    }
#endif
#ifdef RTE_BSP_AUDIO_CODEC_IN
    if (Audio_WM8904_SetMute (stream, channel, mute) == -1) {
      return -1;
    }
#endif
  } else {
#ifdef RTE_BSP_AUDIO_CODEC_OUT
    if (Audio_WM8904_SetMute (stream, channel, mute) == -1) {
      return -1;
    }
#endif
  }

  return 0;
}

/**
  \fn          int32_t Audio_SetDataFormat (uint8_t stream, uint8_t format)
  \brief       Set Audio data format
  \param[in]   stream  stream identifier
  \param[in]   format  data format
  \returns
   - \b  0: function succeeded
   - \b -1: function failed
*/
int32_t Audio_SetDataFormat (uint8_t stream, uint8_t format) {

  if (stream == AUDIO_STREAM_IN) {
#ifdef RTE_BSP_AUDIO_MIC_IN
    if (Audio_DMIC_SetDataFormat (format) == -1) {
      return -1;
    }
#endif
#ifdef RTE_BSP_AUDIO_CODEC_IN
    if (Audio_WM8904_SetDataFormat (stream, format) == -1) {
      return -1;
    }
#endif
  } else {
#ifdef RTE_BSP_AUDIO_CODEC_OUT
    if (Audio_WM8904_SetDataFormat (stream, format) == -1) {
      return -1;
    }
#endif
  }

  return 0;
}

/**
  \fn          int32_t Audio_SetFrequency (uint8_t stream, uint32_t frequency)
  \brief       Set Audio stream frequency
  \param[in]   stream     stream identifier
  \param[in]   frequency  Audio frequency in Hz
  \returns
   - \b  0: function succeeded
   - \b -1: function failed
*/
int32_t Audio_SetFrequency (uint8_t stream, uint32_t frequency) {

  if (stream == AUDIO_STREAM_IN) {
#ifdef RTE_BSP_AUDIO_MIC_IN
    if (Audio_DMIC_SetFrequency (frequency) == -1) {
      return -1;
    }
#endif
#ifdef RTE_BSP_AUDIO_CODEC_IN
    if (Audio_WM8904_SetFrequency (stream, frequency) == -1) {
      return -1;
    }
#endif
  } else {
#ifdef RTE_BSP_AUDIO_CODEC_OUT
    if (Audio_WM8904_SetFrequency (stream, frequency) == -1) {
      return -1;
    }
#endif
  }

  return 0;
}

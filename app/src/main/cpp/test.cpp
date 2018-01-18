//
// Created by capsane on 18-1-11.
//

#include <cwchar>
#include <malloc.h>
#include <pthread.h>
#include "AudioRecord.h"

//==============================================
//  Audio Record Defination
//==============================================

static pthread_t        g_AudioRecordThread;
static pthread_t *  g_AudioRecordThreadPtr = NULL;

volatile bool           g_bQuitAudioRecordThread = false;
volatile int                g_iInSampleTime = 0;
int                                 g_iNotificationPeriodInFrames = 8000/10;
// g_iNotificationPeriodInFrames should be change when sample rate changes.

static void *   AudioRecordThread( void *inArg );
void StartAudioRecordThread();
void StopAudioRecordThread();

void AudioRecordCallback(int event, void* user, void *info)
{
    if(event == android::AudioRecord::EVENT_NEW_POS)
    {
        g_iInSampleTime += g_iNotificationPeriodInFrames;
        //if(g_iInSampleTime > g_iNotificationPeriodInFrames*100)
        //      g_bQuitAudioRecordThread = true;
    }
    else if (event == android::AudioRecord::EVENT_MORE_DATA)
    {
        android::AudioRecord::Buffer* pBuff = (android::AudioRecord::Buffer*)info;
        pBuff->size = 0;
    }
    else if (event == android::AudioRecord::EVENT_OVERRUN)
    {
        LOGE(" EVENT_OVERRUN \n");
    }
}

static void *   AudioRecordThread( void *inArg )
{
    uint64_t                        inHostTime              = 0;
    void *                              inBuffer                    = NULL;
    audio_source_t              inputSource             = AUDIO_SOURCE_MIC;
    audio_format_t              audioFormat             = AUDIO_FORMAT_PCM_16_BIT;
    audio_channel_mask_t    channelConfig       = AUDIO_CHANNEL_IN_MONO; //AUDIO_CHANNEL_IN_STEREO;
    int                                     bufferSizeInBytes = 1600;
    int                                     sampleRateInHz      = 8000;
    android::AudioRecord *  pAudioRecord        = NULL;
    FILE *                                  g_pAudioRecordFile      = NULL;
    char                                        strAudioFile[]              = "/mnt/sdcard/external_sd/AudioRecordFile.pcm";

    int iNbChannels         = 1;    // 1 channel for mono, 2 channel for streo
    int iBytesPerSample = 2;    // 16bits pcm, 2Bytes
    int frameSize           = 0;    // frameSize = iNbChannels * iBytesPerSample
    int minFrameCount     = 0;    // get from AudroRecord object
    int iWriteDataCount = 0;    // how many data are there write to file

    // log the thread id for debug info
    LOGD("%s  Thread ID  = %d  \n", __FUNCTION__,  pthread_self());
    g_iInSampleTime = 0;
    g_pAudioRecordFile = fopen(strAudioFile, "wb+");


    iNbChannels = (channelConfig == AUDIO_CHANNEL_IN_STEREO) ? 2 : 1;
    frameSize   = iNbChannels * iBytesPerSample;

    android::status_t   status = android::AudioRecord::getMinFrameCount(
            &minFrameCount, sampleRateInHz, audioFormat, channelConfig);

    if(status != android::NO_ERROR)
    {
        LOGE("%s  AudioRecord.getMinFrameCount fail \n", __FUNCTION__);
        goto exit ;
    }

    LOGE("sampleRateInHz = %d minFrameCount = %d iNbChannels = %d frameSize = %d ",
         sampleRateInHz, minFrameCount, iNbChannels, frameSize);

    bufferSizeInBytes = minFrameCount * frameSize;

    inBuffer = malloc(bufferSizeInBytes);
    if(inBuffer == NULL)
    {
        LOGE("%s  alloc mem failed \n", __FUNCTION__);
        goto exit ;
    }

    g_iNotificationPeriodInFrames = sampleRateInHz/10;

    pAudioRecord  = new android::AudioRecord();
    if(NULL == pAudioRecord)
    {
        LOGE(" create native AudioRecord failed! ");
        goto exit;
    }

    pAudioRecord->set( inputSource,
                       sampleRateInHz,
                       audioFormat,
                       channelConfig,
                       0,
                       AudioRecordCallback,
                       NULL,
                       0,
                       true,
                       0);

    if(pAudioRecord->initCheck() != android::NO_ERROR)
    {
        LOGE("AudioTrack initCheck error!");
        goto exit;
    }

    if(pAudioRecord->setPositionUpdatePeriod(g_iNotificationPeriodInFrames) != android::NO_ERROR)
    {
        LOGE("AudioTrack setPositionUpdatePeriod error!");
        goto exit;
    }

    if(pAudioRecord->start()!= android::NO_ERROR)
    {
        LOGE("AudioTrack start error!");
        goto exit;
    }

    while (!g_bQuitAudioRecordThread)
    {
        inHostTime = UpTicks();
        int readLen = pAudioRecord->read(inBuffer, bufferSizeInBytes);
        int writeResult = -1;

        if(readLen > 0)
        {
            iWriteDataCount += readLen;
            if(NULL != g_pAudioRecordFile)
            {
                writeResult = fwrite(inBuffer, 1, readLen, g_pAudioRecordFile);
                if(writeResult < readLen)
                {
                    LOGE("Write Audio Record Stream error");
                }
            }

            // write PCM data to file or other stream,implement it yourself
            writeResult = WriteAudioData(
                    g_iInSampleTime,
                    inHostTime,
                    inBuffer,
                    readLen);

            //LOGD("readLen = %d  writeResult = %d  iWriteDataCount = %d", readLen, writeResult, iWriteDataCount);
        }
        else
        {
            LOGE("pAudioRecord->read  readLen = 0");
        }
    }





    exit:

    if(NULL != g_pAudioRecordFile)
    {
        fflush(g_pAudioRecordFile);
        fclose(g_pAudioRecordFile);
        g_pAudioRecordFile = NULL;
    }

    if(pAudioRecord)
    {
        pAudioRecord->stop();
        delete pAudioRecord;
        pAudioRecord == NULL;
    }

    if(inBuffer)
    {
        free(inBuffer);
        inBuffer = NULL;
    }

    LOGD("%s  Thread ID  = %d  quit\n", __FUNCTION__,  pthread_self());
    return NULL;
}
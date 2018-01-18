//
// Created by capsane on 18-1-11.
//
#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include "mediaplayer.h"

using namespace android;

int start(int x, int y, int z, int r, int q, int s) {
    int iRet = 0;
    MediaPlayer *p= new MediaPlayer();

    iRet = p->setDataSource("/data/j2me/phoneme-cache/061075.midi", NULL);
    if (0 != iRet)
    {
        printf("[setDataSource] iRet = %d\n", iRet);
    }

    iRet = p->setAudioStreamType(3);
    if (0 != iRet)
    {
        printf("[setAudioStreamType] iRet = %d\n", iRet);
    }

    iRet = p->prepare();
    if (0 != iRet)
    {
        printf("[prepare] iRet = %d\n", iRet);
    }

    iRet = p->start();
    if (0 != iRet)
    {
        printf("[start] iRet = %d\n", iRet);
    }

    p->stop();
    return 0;
}

int main()
{

    int iRet = 0;
    MediaPlayer *p= new MediaPlayer();

    iRet = p->setDataSource("/data/j2me/phoneme-cache/061075.midi", NULL);
    if (0 != iRet)
    {
        printf("[setDataSource] iRet = %d\n", iRet);
    }

    iRet = p->setAudioStreamType(3);
    if (0 != iRet)
    {
        printf("[setAudioStreamType] iRet = %d\n", iRet);
    }

    iRet = p->prepare();
    if (0 != iRet)
    {
        printf("[prepare] iRet = %d\n", iRet);
    }

    iRet = p->start();
    if (0 != iRet)
    {
        printf("[start] iRet = %d\n", iRet);
    }

    p->stop();
    return 0;
}

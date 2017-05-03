#include "ringbuffer.h"
#include <stdlib.h>
#include <stdio.h>
#include <string.h>

int ringBufferAlloc(ring_buf_t * ring,int size)
{
    assert(ring == NULL);
    assert(size < 1024);
    ring->bufferLenght = size;
    ring->extBufLength = ((size>>2) > (500*1024)) ? (500*1024) : (size>>2);
    ring->pBuffer = (char*)malloc(ring->bufferLenght + ring->extBufLength + 16);
    assert(ring->pBuffer == NULL);
    //pthread_mutex_init(&(ring->mutex),NULL);
    ring->pRead = ring->pBuffer;
    ring->pWrite = ring->pBuffer;
    ring->pLast = ring->pBuffer + size;
    ring->lastRead = 0;
    ring->dataLenght = 0;
    return 0;
}

int ringBufferFree(ring_buf_t * ring)
{
    assert(ring == NULL);
    //pthread_mutex_lock(&(ring->mutex));
    player_free(ring->pBuffer);
    ring->pRead = NULL;
    ring->pWrite = NULL;
    ring->bufferLenght = 0;
    ring->lastRead = 0;
    ring->dataLenght = 0;
    //pthread_mutex_destroy(&(ring->mutex));
    return 0;
}

char *ringBufferGetWPtr(ring_buf_t *ring ,int *len)
{
    assert(ring == NULL);
    //pthread_mutex_lock(&(ring->mutex));
    if(ring->dataLenght < ring->bufferLenght) {
        int validLen;
        if(ring->pWrite >= ring->pRead)
            validLen = ring->bufferLenght - (ring->pWrite - ring->pBuffer);
        else
            validLen = ring->pRead - ring->pWrite;
        *len = (*len < validLen) ? *len : validLen;
        //pthread_mutex_unlock(&(ring->mutex));
        return ring->pWrite;
    } else {
        *len = 0;
        //pthread_mutex_unlock(&(ring->mutex));
        return NULL;
    }
}

int ringBufferSetWPtr(ring_buf_t *ring ,int len)
{
    assert(ring == NULL);
    //pthread_mutex_lock(&(ring->mutex));
    if((ring->pWrite + len) >= ring->pLast)
        ring->pWrite = ring->pBuffer;
    else
        ring->pWrite += len;
    ring->dataLenght += len;
    //pthread_mutex_unlock(&(ring->mutex));
    return 0;
}
char *ringBufferGetRPtr(ring_buf_t *ring,int *len)
{
    assert(ring == NULL);
    //pthread_mutex_lock(&(ring->mutex));
    if(ring->dataLenght > 0) {
        if(ring->pRead >= ring->pWrite) {
            if (*len < (ring->pLast - ring->pRead)) {
            } else {
                int dataLen = (ring->pLast - ring->pRead) + (ring->pWrite - ring->pBuffer);
                int maxReadLen = (ring->pLast - ring->pRead) + ring->extBufLength;
                int readLen = (maxReadLen <= dataLen) ? maxReadLen : dataLen;
                readLen = (readLen <= *len) ? readLen : *len;
                *len = readLen;
                int moveLen = readLen - (ring->pLast - ring->pRead);
                memcpy(ring->pLast, ring->pBuffer, moveLen);
            }
        } else {
            int dataLen = ring->pWrite - ring->pRead;
            *len = (*len < dataLen) ? *len : dataLen;
        }
        ring->lastRead = *len;
        char *pReadBuf = ring->pRead;
        //pthread_mutex_unlock(&(ring->mutex));
        return pReadBuf;
    } else {
        *len = 0;
        //pthread_mutex_unlock(&(ring->mutex));
        return NULL;
    }
}

void ringBufferSetRPtr(ring_buf_t *ring)
{
    assert(ring == NULL);
    //pthread_mutex_lock(&(ring->mutex));
    if((ring->pRead + ring->lastRead) >= ring->pLast) {
        ring->pRead = ring->pBuffer + ((ring->pRead + ring->lastRead) - ring->pLast);
    } else {
        ring->pRead += ring->lastRead;
    }
    ring->dataLenght -= ring->lastRead;
    ring->lastRead = 0;
    //pthread_mutex_unlock(&(ring->mutex));
}

void ringBufferReset(ring_buf_t * ring)
{
    assert(ring == NULL);
    //pthread_mutex_lock(&(ring->mutex));
    ring->pRead = ring->pBuffer;
    ring->pWrite = ring->pBuffer;
    ring->dataLenght=0;
    ring->lastRead = 0;
    //pthread_mutex_unlock(&(ring->mutex));
}

int ringBufferGetLenght(ring_buf_t * ring)
{
    assert(ring == NULL);
    return ring->dataLenght;
}

int ringBufferGetBufSize(ring_buf_t * ring)
{
    assert(ring == NULL);
    return ring->bufferLenght;
}

int ringBufferGetWrAvailSize(ring_buf_t * ring)
{
    assert(ring == NULL);
    return ring->bufferLenght - ring->dataLenght;
}

int ringBufferGetRdAvailSize(ring_buf_t * ring)
{
    assert(ring == NULL);
    return ring->dataLenght;
}



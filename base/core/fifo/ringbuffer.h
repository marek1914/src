
typedef struct
{
    char *pBuffer;
    char *pRead;
    char *pWrite;
    char *pLast;
    int lastRead;
    int bufferLenght;
    int extBufLength;
    int dataLenght;
    pthread_mutex_t mutex;
} ring_buf_t;

int ringBufferAlloc(ring_buf_t * ring, int size);
int ringBufferFree(ring_buf_t * ring);
char *ringBufferGetWPtr(ring_buf_t * buffer, int *len);
int ringBufferSetWPtr(ring_buf_t * ring, int len);
char *ringBufferGetRPtr(ring_buf_t * ring, int *len);
void ringBufferSetRPtr(ring_buf_t * ring);
int ringBufferGetLenght(ring_buf_t * ring);
void ringBufferReset(ring_buf_t * ring);
int ringBufferGetWrAvailSize(ring_buf_t * ring);
int ringBufferGetRdAvailSize(ring_buf_t * ring);
int ringBufferGetBufSize(ring_buf_t * ring);


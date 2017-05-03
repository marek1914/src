/* a very simple circular buffer FIFO from ffmpeg */
#include <assert.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "fifo.h"

#define FFMAX(a,b) ((a) > (b) ? (a) : (b))
#define FFMIN(a,b) ((a) > (b) ? (b) : (a))

void freep(void *arg)
{
    void *val;

    memcpy(&val, arg, sizeof(val));
    memcpy(arg, &(void *){ NULL }, sizeof(val));
    free(val);
}


static FifoBuffer *fifo_alloc_common(void *buffer, size_t size)
{
    FifoBuffer *f;
    if (!buffer)
        return NULL;
    f = malloc(sizeof(FifoBuffer));
    
    if (!f) {
        free(buffer);
        return NULL;
    } else {
    	memset(f, 0, sizeof(FifoBuffer));
    }
    
    f->buffer = buffer;
    f->end    = f->buffer + size;
    fifo_reset(f);
    return f;
}

FifoBuffer *fifo_alloc(unsigned int size)
{
    void *buffer = malloc(size);
    return fifo_alloc_common(buffer, size);
}

FifoBuffer *fifo_alloc_array(size_t nmemb, size_t size)
{
    void *buffer = malloc(nmemb * size);
    return fifo_alloc_common(buffer, nmemb * size);
}

void fifo_free(FifoBuffer *f)
{
    if (f) {
        freep(&f->buffer);
        free(f);
    }
}

void fifo_freep(FifoBuffer **f)
{
    if (f) {
        fifo_free(*f);
        *f = NULL;
    }
}

void fifo_reset(FifoBuffer *f)
{
    f->wptr = f->rptr = f->buffer;
    f->wndx = f->rndx = 0;
}

int fifo_size(const FifoBuffer *f)
{
    return (uint32_t)(f->wndx - f->rndx);
}

int fifo_space(const FifoBuffer *f)
{
    return f->end - f->buffer - fifo_size(f);
}

int fifo_realloc2(FifoBuffer *f, unsigned int new_size)
{
    unsigned int old_size = f->end - f->buffer;

    if (old_size < new_size) {
        int len          = fifo_size(f);
        FifoBuffer *f2 = fifo_alloc(new_size);

        if (!f2)
            return -1; //AVERROR(ENOMEM);
        fifo_generic_read(f, f2->buffer, len, NULL);
        f2->wptr += len;
        f2->wndx += len;
        free(f->buffer);
        *f = *f2;
        free(f2);
    }
    return 0;
}

int fifo_grow(FifoBuffer *f, unsigned int size)
{
    unsigned int old_size = f->end - f->buffer;
    if(size + (unsigned)fifo_size(f) < size)
        return -1;//AVERROR(EINVAL);

    size += fifo_size(f);

    if (old_size < size)
        return fifo_realloc2(f, FFMAX(size, 2*size));
    return 0;
}

/* src must NOT be const as it can be a context for func that may need
 * updating (like a pointer or byte counter) */
int fifo_generic_write(FifoBuffer *f, void *src, int size,
                          int (*func)(void *, void *, int))
{
    int total = size;
    uint32_t wndx= f->wndx;
    uint8_t *wptr= f->wptr;

    do {
        int len = FFMIN(f->end - wptr, size);
        if (func) {
            len = func(src, wptr, len);
            if (len <= 0)
                break;
        } else {
            memcpy(wptr, src, len);
            src = (uint8_t *)src + len;
        }
// Write memory barrier needed for SMP here in theory
        wptr += len;
        if (wptr >= f->end)
            wptr = f->buffer;
        wndx    += len;
        size    -= len;
    } while (size > 0);
    f->wndx= wndx;
    f->wptr= wptr;
    return total - size;
}

int fifo_generic_peek_at(FifoBuffer *f, void *dest, int offset, int buf_size, void (*func)(void*, void*, int))
{
    uint8_t *rptr = f->rptr;

    assert(offset >= 0);

    /*
     * *ndx are indexes modulo 2^32, they are intended to overflow,
     * to handle *ndx greater than 4gb.
     */
    assert(buf_size + (unsigned)offset <= f->wndx - f->rndx);

    if (offset >= f->end - rptr)
        rptr += offset - (f->end - f->buffer);
    else
        rptr += offset;

    while (buf_size > 0) {
        int len;

        if (rptr >= f->end)
            rptr -= f->end - f->buffer;

        len = FFMIN(f->end - rptr, buf_size);
        if (func)
            func(dest, rptr, len);
        else {
            memcpy(dest, rptr, len);
            dest = (uint8_t *)dest + len;
        }

        buf_size -= len;
        rptr     += len;
    }

    return 0;
}

int fifo_generic_peek(FifoBuffer *f, void *dest, int buf_size,
                         void (*func)(void *, void *, int))
{
// Read memory barrier needed for SMP here in theory
    uint8_t *rptr = f->rptr;

    do {
        int len = FFMIN(f->end - rptr, buf_size);
        if (func)
            func(dest, rptr, len);
        else {
            memcpy(dest, rptr, len);
            dest = (uint8_t *)dest + len;
        }
// memory barrier needed for SMP here in theory
        rptr += len;
        if (rptr >= f->end)
            rptr -= f->end - f->buffer;
        buf_size -= len;
    } while (buf_size > 0);

    return 0;
}

int fifo_generic_read(FifoBuffer *f, void *dest, int buf_size,
                         void (*func)(void *, void *, int))
{
// Read memory barrier needed for SMP here in theory
    do {
        int len = FFMIN(f->end - f->rptr, buf_size);
        if (func)
            func(dest, f->rptr, len);
        else {
            memcpy(dest, f->rptr, len);
            dest = (uint8_t *)dest + len;
        }
// memory barrier needed for SMP here in theory
        fifo_drain(f, len);
        buf_size -= len;
    } while (buf_size > 0);
    return 0;
}

/** Discard data from the FIFO. */
void fifo_drain(FifoBuffer *f, int size)
{
    assert(fifo_size(f) >= size);
    f->rptr += size;
    if (f->rptr >= f->end)
        f->rptr -= f->end - f->buffer;
    f->rndx += size;
}

/* test */

int main(void)
{
    int i, j, n;

    /* create a FIFO buffer */
    FifoBuffer *fifo = fifo_alloc(13 * sizeof(int));

    /* fill data */
    for (i = 0; fifo_space(fifo) >= sizeof(int); i++)
        fifo_generic_write(fifo, &i, sizeof(int), NULL);

    /* peek at FIFO */
    n = fifo_size(fifo) / sizeof(int);
    for (i = -n + 1; i < n; i++) {
        int *v = (int *)fifo_peek2(fifo, i * sizeof(int));
        printf("%d: %d\n", i, *v);
    }
    printf("\n");

    /* peek_at at FIFO */
    n = fifo_size(fifo) / sizeof(int);
    for (i = 0; i < n; i++) {
        fifo_generic_peek_at(fifo, &j, i * sizeof(int), sizeof(j), NULL);
        printf("%d: %d\n", i, j);
    }
    printf("\n");

    /* read data */
    for (i = 0; fifo_size(fifo) >= sizeof(int); i++) {
        fifo_generic_read(fifo, &j, sizeof(int), NULL);
        printf("%d ", j);
    }
    printf("\n");

    /* test *ndx overflow */
    fifo_reset(fifo);
    fifo->rndx = fifo->wndx = ~(uint32_t)0 - 5;

	printf("-----\n");
    /* fill data */
    for (i = 0; fifo_space(fifo) >= sizeof(int); i++)
        fifo_generic_write(fifo, &i, sizeof(int), NULL);

    /* peek_at at FIFO */
    n = fifo_size(fifo) / sizeof(int);
    for (i = 0; i < n; i++) {
        fifo_generic_peek_at(fifo, &j, i * sizeof(int), sizeof(j), NULL);
        printf("%d: %d\n", i, j);
    }

    fifo_free(fifo);

    return 0;
}


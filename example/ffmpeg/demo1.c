#include <stdio.h>  
#include <libavcodec/avcodec.h>  
#include <libavformat/avformat.h>  
#include <libswscale/swscale.h>  
#include <SDL/SDL.h>  

int main(int argc, char* argv[])  
{  
    AVFormatContext *s = NULL;
    AVCodecContext  *avctx;
    AVCodec         *pCodec;
	AVPacket 		*packet;
	AVFrame *frame, *frameYUV;

	SDL_Overlay *bmp;
	SDL_Rect rect;
	SDL_Surface *screen;

	int w, h, i, index;
    int got_picture;
 	struct SwsContext *convert;

	packet=(AVPacket *)av_malloc(sizeof(AVPacket));

    av_register_all(); 

    avformat_open_input(&s, argv[1], NULL, NULL);  

    avformat_find_stream_info(s, NULL);
 
    for(i = 0; i < s->nb_streams; i++)   
        if(s->streams[i]->codec->codec_type == AVMEDIA_TYPE_VIDEO){  
            index=i;  
            break;  
        }  
 
    avctx = s->streams[index]->codec;  
    pCodec=avcodec_find_decoder(avctx->codec_id);  
    avcodec_open2(avctx, pCodec, NULL);
     
    frame=av_frame_alloc();  
    frameYUV=av_frame_alloc();  

    SDL_Init(SDL_INIT_VIDEO | SDL_INIT_AUDIO | SDL_INIT_TIMER);
   
    w = avctx->width;  
    h = avctx->height;  
    screen = SDL_SetVideoMode(w, h, 0, 0);  
 
    bmp = SDL_CreateYUVOverlay(w, h, SDL_YV12_OVERLAY, screen);   
     
    rect.x = 0;
	rect.y = 0;      
    rect.w = w;
	rect.h = h;     
     
    SDL_WM_SetCaption("Player",NULL);  
  
    convert = sws_getContext(w, h, avctx->pix_fmt, w, h, PIX_FMT_YUV420P, SWS_BICUBIC, NULL, NULL, NULL);   

    while( av_read_frame(s, packet) >= 0 ){  
        if( packet->stream_index == index ){  
            avcodec_decode_video2(avctx, frame, &got_picture, packet);  
 
            if( got_picture ){  
                SDL_LockYUVOverlay(bmp);  
                frameYUV->data[0] = bmp->pixels[0];  
                frameYUV->data[1] = bmp->pixels[2];  
                frameYUV->data[2] = bmp->pixels[1];       
                frameYUV->linesize[0] = bmp->pitches[0];  
                frameYUV->linesize[1] = bmp->pitches[2];     
                frameYUV->linesize[2] = bmp->pitches[1];  
                sws_scale(convert, frame->data, frame->linesize, 0, h, frameYUV->data, frameYUV->linesize);  
     
                SDL_UnlockYUVOverlay(bmp);   
                SDL_DisplayYUVOverlay(bmp, &rect);    
                SDL_Delay(40);  
            }  
        }  
        av_free_packet(packet);  
    }  
    sws_freeContext(convert);  
    SDL_Quit();  
  
    av_free(frameYUV);  
    avcodec_close(avctx);  
    avformat_close_input(&s);  
  
    return 0;  
}  

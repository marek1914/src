#include <libavcodec/avcodec.h>  
#include <libavformat/avformat.h>  
#include <SDL/SDL.h>  

int main(int argc, char* argv[])  
{  
    AVFormatContext *s = NULL;
    AVCodecContext  *avctx;
    AVCodec         *pCodec;
	AVPacket 		packet;
	AVFrame 		*frame = av_frame_alloc();  
	SDL_Overlay *bmp;
	SDL_Rect rect={0,0,1920,1080};
	SDL_Surface *screen;
    int got_picture;

    av_register_all(); 
    avformat_open_input(&s, argv[1], NULL, NULL);  
    avformat_find_stream_info(s, NULL);
 
    avctx = s->streams[0]->codec;  
    pCodec=avcodec_find_decoder(avctx->codec_id);  
    avcodec_open2(avctx, pCodec, NULL);

    SDL_Init(SDL_INIT_VIDEO | SDL_INIT_AUDIO | SDL_INIT_TIMER);
    screen = SDL_SetVideoMode(avctx->width, avctx->height, 0, 0);  
    bmp = SDL_CreateYUVOverlay(avctx->width, avctx->height, SDL_YV12_OVERLAY, screen);    

    while (av_read_frame(s, &packet) >= 0) {  
        if (packet.stream_index == 0) {
            avcodec_decode_video2(avctx, frame, &got_picture, &packet);  
            if (got_picture) {  
				memcpy(bmp->pixels[0],frame->data[0],1920*1080 );				
				memcpy(bmp->pixels[1],frame->data[2],960*540 );
				memcpy(bmp->pixels[2],frame->data[1],960*540 );	
				bmp->pitches[0] = 1920;
				bmp->pitches[1] = 960;
				bmp->pitches[2] = 960;
                SDL_DisplayYUVOverlay(bmp, &rect);    
                SDL_Delay(41);  
            }  
        }  
        av_free_packet(&packet);  
    }
    SDL_Quit();   
    avcodec_close(avctx);  
    avformat_close_input(&s);
    return 0;  
}  

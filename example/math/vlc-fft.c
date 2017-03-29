//vlc的fft频谱分析

fft_perform 

typedef short int sound_sample;

#define FFT_BUFFER_SIZE_LOG 9
#define FFT_BUFFER_SIZE (1 << FFT_BUFFER_SIZE_LOG) //512
/*
 * The input array is assumed to have FFT_BUFFER_SIZE elements,
 * and the output array is assumed to have (FFT_BUFFER_SIZE / 2 + 1) elements.
 */
 
//为什么输入512个 输出 257

2个float相乘，得到的结果跟我用计算器算的怎么不同呢？

p_dest[i] = p_output[i] *  ( 2 ^ 16 ) / ( ( FFT_BUFFER_SIZE / 2 * 32768 ) ^ 2 );
i=256之后 都是0 

 ( 2 ^ 16 ) / ( ( FFT_BUFFER_SIZE / 2 * 32768 ) ^ 2 ) 运算出来是：0.000002145766757

p_output[i] *  ( 2 ^ 16 ) / ( ( FFT_BUFFER_SIZE / 2 * 32768 ) ^ 2 ); 这是什么意思？？？

原始算法，也有溢出的情况，因为p_dest为int16  模的平方 乘以0.000002145766757 有时候会溢出。

分频器和频谱 都使用fft

 * vuMeter_Run: vu meter effect  是2个表盘的那个效果

加上 
p_buffs++;
选择第二个通道，发现没有什么变化。
我需要再理解一下 stereo 立体声  其实在vlc中选择left 后者right 通道，还是能听到音乐和人生，并不是
说左通道音乐，右通道人声。

p_buffer->i_nb_samples , p_effect->i_nb_chans
播mp3 i_nb_samples=1152， i_nb_chans=2

mp3 帧 1152采样，取出前512个做FFT（？） 输出512个，但只取前256个（因高频部分不重要）

    p_buffs = p_s16_buff;

    p_buffs++;  //加上这句 切换到另外一个通道，但是效果类似，因为单声道包含了立体声的音乐和人声。

    for ( i = 0 ; i < FFT_BUFFER_SIZE ; i++) {
        p_output[i]  = 0;
        p_buffer1[i] = *p_buffs;

        p_buffs += p_effect->i_nb_chans; //这里貌似在取其中一个通道数据
        if( p_buffs >= &p_s16_buff[p_buffer->i_nb_samples * p_effect->i_nb_chans] )
            p_buffs = p_s16_buff;
    }

取相同数据，用octave做fft，得到的数据基本一致（除精度区别），但是2个方式算出来的
复数，是共扼关系，如这里的fft算出是  118560.4453125000 ,-244654.5625000000
octave是 1.1856e+05 + 2.4465e+05i  

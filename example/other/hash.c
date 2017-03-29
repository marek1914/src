#include <stdio.h>


unsigned int
elf_Hash(const unsigned char *name)
{
    unsigned int h = 0, g;
 
        while (*name)
        {
             h = (h << 4) + *name++;
             if (g = h & 0xf0000000)
                  h ^= g >> 24;
                   h &= ~g;
        }
        return h;
}
main()
{
printf("0x %x \n", elf_Hash("Java_shuliang_han_sdkhelloworld_MainActivity_helloFromJNI"));


}

#include <stdint.h>
#include "ugui.h"

void set_pixel(int16_t x, int16_t y, UG_COLOR c)
{

}	

UG_GUI gui;

int main(void)
{
	UG_Init(&gui, set_pixel, 128, 64);
	UG_FontSelect(&FONT_8X12);

	UG_FillScreen(1);

	return 0;
}

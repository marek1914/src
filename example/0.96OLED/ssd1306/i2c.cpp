#include <unistd.h>
#include <stdio.h>
#include <fcntl.h>
#include "ssd1306.h"

SSD1306 display;
#define min(a,b) (((a) < (b)) ? (a) : (b))

unsigned char logo16_glcd_bmp[] =
{ 
	0x00, 0xc0,
	0x01, 0xc0,
	0x01, 0xc0,
	0x03, 0xe0,
	0xf3, 0xe0,
	0xfe, 0xf8,
    0x7e, 0xff,
    0x33, 0x9f,
  	0x1f, 0xfc,
  	0x0d, 0x70,
  	0x1b, 0xa0,
  	0x3f, 0xe0,
  	0x3f, 0xf0,
  	0x7c, 0xf0,
  	0x70, 0x70,
  	0x00, 0x30
 };

void testdrawchar(void) {
  display.setTextSize(1);
  display.setTextColor(WHITE);
  display.setCursor(0,0);

  for (uint8_t i=0; i < 168; i++) {
    if (i == '\n') continue;
    ;//display.write(i);
    if ((i > 0) && (i % 21 == 0))
      ;//display.println();
  }    
  display.display();
  usleep(1000);
}

void testdrawcircle(void) {
  for (int16_t i=0; i<display.height(); i+=2) {
    display.drawCircle(display.width()/2, display.height()/2, i, WHITE);
    display.display();
    usleep(1000);
  }
}

void testfillrect(void) {
  uint8_t color = 1;
  for (int16_t i=0; i<display.height()/2; i+=3) {
    // alternate colors
    display.fillRect(i, i, display.width()-i*2, display.height()-i*2, color%2);
    display.display();
    usleep(1000);
    color++;
  }
}

void testdrawtriangle(void) {
  for (int16_t i=0; i<min(display.width(),display.height())/2; i+=5) {
    display.drawTriangle(display.width()/2, display.height()/2-i,
                     display.width()/2-i, display.height()/2+i,
                     display.width()/2+i, display.height()/2+i, WHITE);
    display.display();
    usleep(1000);
  }
}

void testfilltriangle(void) {
  uint8_t color = WHITE;
  for (int16_t i=min(display.width(),display.height())/2; i>0; i-=5) {
    display.fillTriangle(display.width()/2, display.height()/2-i,
                     display.width()/2-i, display.height()/2+i,
                     display.width()/2+i, display.height()/2+i, WHITE);
    if (color == WHITE) color = BLACK;
    else color = WHITE;
    display.display();
    usleep(1000);
  }
}

void testdrawroundrect(void) {
  for (int16_t i=0; i<display.height()/2-2; i+=2) {
    display.drawRoundRect(i, i, display.width()-2*i, display.height()-2*i, display.height()/4, WHITE);
    display.display();
    usleep(1000);
  }
}

void testfillroundrect(void) {
  uint8_t color = WHITE;
  for (int16_t i=0; i<display.height()/2-2; i+=2) {
    display.fillRoundRect(i, i, display.width()-2*i, display.height()-2*i, display.height()/4, color);
    if (color == WHITE) color = BLACK;
    else color = WHITE;
    display.display();
    usleep(1000);
  }
}
   
void testdrawrect(void) {
  for (int16_t i=0; i<display.height()/2; i+=2) {
    display.drawRect(i, i, display.width()-2*i, display.height()-2*i, WHITE);
    display.display();
    usleep(1000);
  }
}

void testdrawline() {  
  for (int16_t i=0; i<display.width(); i+=4) {
    display.drawLine(0, 0, i, display.height()-1, WHITE);
    display.display();
    //usleep(1000);
  }
  for (int16_t i=0; i<display.height(); i+=4) {
    display.drawLine(0, 0, display.width()-1, i, WHITE);
    display.display();
    //usleep(1000);
  }
  usleep(250*1000);
  
  display.clearDisplay();
  for (int16_t i=0; i<display.width(); i+=4) {
    display.drawLine(0, display.height()-1, i, 0, WHITE);
    display.display();
    //usleep(1000);
  }
  for (int16_t i=display.height()-1; i>=0; i-=4) {
    display.drawLine(0, display.height()-1, display.width()-1, i, WHITE);
    display.display();
    //usleep(1000);
  }
  usleep(250*1000);
  
  display.clearDisplay();
  for (int16_t i=display.width()-1; i>=0; i-=4) {
    display.drawLine(display.width()-1, display.height()-1, i, 0, WHITE);
    display.display();
    //usleep(1000);
  }
  for (int16_t i=display.height()-1; i>=0; i-=4) {
    display.drawLine(display.width()-1, display.height()-1, 0, i, WHITE);
    display.display();
    //usleep(1000);
  }
  usleep(250*1000);

  display.clearDisplay();
  for (int16_t i=0; i<display.height(); i+=4) {
    display.drawLine(display.width()-1, 0, 0, i, WHITE);
    display.display();
    //usleep(1000);
  }
  for (int16_t i=0; i<display.width(); i+=4) {
    display.drawLine(display.width()-1, 0, i, display.height()-1, WHITE); 
    display.display();
    //usleep(1000);
  }
  usleep(250*1000);
}

void testscrolltext(void) {
  display.setTextSize(2);
  display.setTextColor(WHITE);
  display.setCursor(10,0);
  display.clearDisplay();
  //display.println("scroll");
  display.display();
  sleep(1);
 
  display.startscrollright(0x00, 0x0F);
  sleep(2);
  display.stopscroll();
  sleep(1);
  display.startscrollleft(0x00, 0x0F);
  sleep(2);
  display.stopscroll();
  sleep(1);    
  display.startscrolldiagright(0x00, 0x07);
  sleep(2);
  display.startscrolldiagleft(0x00, 0x07);
  sleep(2);
  display.stopscroll();
}

int main(void)   
{                
  display.begin();

  // draw a single pixel
  display.drawPixel(10, 10, WHITE);
  // Show the display buffer on the hardware.
  // NOTE: You _must_ call display after making any drawing commands
  // to make them visible on the display hardware!
  display.display();
  sleep(1);
  display.clearDisplay();

  // draw many lines
  testdrawline();
  display.display();
  sleep(1);
  display.clearDisplay();

  // draw rectangles
  testdrawrect();
  display.display();
  sleep(1);
  display.clearDisplay();

  // draw multiple rectangles
  testfillrect();
  display.display();
  sleep(1);
  display.clearDisplay();

  // draw mulitple circles
  testdrawcircle();
  display.display();
  sleep(1);
  display.clearDisplay();

  // draw a white circle, 10 pixel radius
  display.fillCircle(display.width()/2, display.height()/2, 10, WHITE);
  display.display();
  sleep(1);
  display.clearDisplay();

  testdrawroundrect();
  sleep(1);
  display.clearDisplay();

  testfillroundrect();
  sleep(1);
  display.clearDisplay();

  testdrawtriangle();
  sleep(1);
  display.clearDisplay();
   
  testfilltriangle();
  sleep(1);
  display.clearDisplay();

  // draw the first ~12 characters in the font
  testdrawchar();
  display.display();
  sleep(1);
  display.clearDisplay();

  // draw scrolling text
  testscrolltext();
  sleep(1);
  display.clearDisplay();

  // text display tests
  /*
  display.setTextSize(1);
  display.setTextColor(WHITE);
  display.setCursor(0,0);
  display.println("Hello, world!");
  display.setTextColor(BLACK, WHITE); // 'inverted' text
  display.println(3.141592);
  display.setTextSize(2);
  display.setTextColor(WHITE);
  display.print("0x"); display.println(0xDEADBEEF, HEX);
  display.display();
  sleep(2000);
  display.clearDisplay();
  */

  // miniature bitmap display
  display.drawBitmap(30, 16,  logo16_glcd_bmp, 16, 16, 1);
  display.display();
  sleep(1);

  // invert the display
  display.invertDisplay(true);
  sleep(1); 
  display.invertDisplay(false);
  sleep(1); 
  display.clearDisplay();
  return 0;
}


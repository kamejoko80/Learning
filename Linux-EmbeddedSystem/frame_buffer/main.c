#include "frame_buffer.h"

int main(int argc, char *argv[])
{
	struct framebuffer_struct fb_struct;
	struct framebuffer_struct *fbsptr;
	
	fbsptr = &fb_struct;	
	fb_init(fbsptr);
	
	fbdraw_circle(fbsptr, 400, 240, 20, 0x0000ffff);
	fbdraw_rectangle(fbsptr, 100, 100, 700, 300, 0x00ffff00);
	
	fbdraw_chinese(fbsptr, "", 300, 200);
	//fbdraw_picture(fbsptr, "Image.bmp");
	fb_close(fbsptr);
	return 0;
}


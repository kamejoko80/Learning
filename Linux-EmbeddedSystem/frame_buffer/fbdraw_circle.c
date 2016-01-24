#include "frame_buffer.h"

void dot8(struct framebuffer_struct *fb_struct,\
		int xc, int yc, int x, int y, int color)
{
        fbdraw_pixel(fb_struct, xc + x, yc + y, color);
        fbdraw_pixel(fb_struct, xc - x, yc + y, color);
        fbdraw_pixel(fb_struct, xc + x, yc - y, color);
        fbdraw_pixel(fb_struct, xc - x, yc - y, color);

        fbdraw_pixel(fb_struct, xc + y, yc + x, color);
        fbdraw_pixel(fb_struct, xc - y, yc + x, color);
        fbdraw_pixel(fb_struct, xc + y, yc - x, color);
        fbdraw_pixel(fb_struct, xc - y, yc - x, color);
}

// Bresenham algorithm
void fbdraw_circle(struct framebuffer_struct *fb_struct,\
			int xc, int yc, int r, int color)
{
	//center at (xc, yc), with radius r
	int x, y, d;
	y = r;
	d = 3 - 2 * r;
	
	for (x = 0; x <= y; x++)
	{
		// draw octants
		dot8(fb_struct,	xc, yc, x, y, color);
		
		// the point in or out of the circle  
		if (d < 0)
		{
			d += 4 * x + 6;
		}
		else
		{
			y--;
			d += 4 * (x - y) + 6;
		}
	}
}

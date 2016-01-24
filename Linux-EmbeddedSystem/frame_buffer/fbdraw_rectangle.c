#include "frame_buffer.h"

void fbdraw_rectangle(struct framebuffer_struct *fb_struct,\ 
			int x1, int y1, int x2, int y2, int color)
{
	int x, y;

	for(x = x1; x < x2; x++)
	{
		fbdraw_pixel(fb_struct, x, y1, color);
	}

	for(y = y1; y < y2; y++)
        {
                fbdraw_pixel(fb_struct, x2, y, color);
        }

	for(x = x2; x > x1; x--)
        {
                fbdraw_pixel(fb_struct, x, y2, color);
        }

	for(y = y2; y > y1; y--)
        {
                fbdraw_pixel(fb_struct, x1, y, color);
        }
}

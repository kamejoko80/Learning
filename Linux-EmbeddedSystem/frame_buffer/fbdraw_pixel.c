#include <stdio.h>
#include "frame_buffer.h"

void fbdraw_pixel(struct framebuffer_struct *fb_struct,\
			int x, int y, int color)
{
	unsigned int offset;

	// prevent the invalid value
	if((x >= 0) && (x < (fb_struct->xres))\
		&& (y >= 0) && (y < (fb_struct->yres)))
	{
		offset = y * (fb_struct->line_length) + x * 4;
		* (unsigned int *) (fb_struct->fbptr + offset) = color;
	}
	else
	{
		perror("the pixel is out of range.\n");
	}
}


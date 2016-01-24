#include <stdio.h>
#include <stdlib.h>
#include <linux/fb.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#include "frame_buffer.h"

void fbdraw_picture(struct framebuffer_struct *fb_struct, char *path)
{	
	int pic, x ,y;
	char buf[4] = {0};
	int color;

	// open the image file
	pic= open(path, O_RDONLY, 0666);
	// error checking	
	if (pic< 0)
	{
		perror("open error");
		exit(1);
	}

	// jump to the image data
	lseek(pic, 54, SEEK_SET);
	
	// draw the image from (0, 0)	
	for (y = (fb_struct->yres - 1); y >= 0; y--)
	{
		for (x = 0; x < (fb_struct->xres); x++)
		{
	//x = fb_struct->xres;
	//y = fb_struct->yres; 
			read(pic, buf, 3);
			color = (buf[0] << 24) | (buf[1] << 16) | (buf[2] << 8) | buf[3];
			fbdraw_pixel(fb_struct, x, y, color);
		}
	}
	// close the image file
	close(pic);
}


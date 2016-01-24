#include <stdio.h>
#include <linux/fb.h>
#include <sys/ioctl.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <sys/mman.h>
#include <fcntl.h>
#include "frame_buffer.h"

void fb_init(struct framebuffer_struct *fb_struct)
{
	struct fb_fix_screeninfo finfo;
	struct fb_var_screeninfo vinfo;
	int fbsize;

	// open the frame buffer device
	// get the file descriptor: fb_struct->fd
	(fb_struct->fd) = open("/dev/fb0", O_RDWR);
	if(fb_struct->fd < 0)
	{
		printf("Error opening '/dev/fb0'. Check kernel config\n");
	}
	 	
	ioctl(fb_struct->fd, FBIOGET_FSCREENINFO, &finfo);
	ioctl(fb_struct->fd, FBIOGET_VSCREENINFO, &vinfo);

	// print the visible resolution and bits per pixel
	printf("x = %d, y = %d\n", vinfo.xres, vinfo.yres);
	printf("bits = %d\n", vinfo.bits_per_pixel);
	
	fbsize = vinfo.yres * finfo.line_length;
	// get the size of the screen from visual resolution
	(fb_struct->xres)= vinfo.xres;	
 	(fb_struct->yres)= vinfo.yres;
	(fb_struct->line_length) = finfo.line_length;
	// map physics address to virtual address
	fb_struct->fbptr = (char *) mmap(NULL, fbsize, \
		PROT_READ | PROT_WRITE, MAP_SHARED, fb_struct->fd, 0);
}

void fb_close(struct framebuffer_struct *fb_struct)
{
	munmap(fb_struct->fbptr,\
		 (fb_struct->yres) * (fb_struct->line_length));
	close(fb_struct->fd);
}




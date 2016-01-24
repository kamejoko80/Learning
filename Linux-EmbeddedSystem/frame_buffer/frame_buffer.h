#ifndef __FRAME_BUFFER__
#define __FRAME_BUFFER__

struct framebuffer_struct
{
	int fd;
	unsigned char *fbptr;
	int xres;
	int yres;
	int line_length;
};

void fb_init(struct framebuffer_struct *fb_struct);
void fb_close(struct framebuffer_struct *fb_struct);

void fbdraw_pixel(struct framebuffer_struct *fb_struct,\
		 	int x, int y, int color);
void fbdraw_circle(struct framebuffer_struct *fb_struct,\
			int xc, int yc, int r, int color);
void fbdraw_rectangle(struct framebuffer_struct *fb_struct,\
			int x1, int y1, int x2, int y2, int color);
void fbdraw_picture(struct framebuffer_struct *fb_struct, char *path);

#endif


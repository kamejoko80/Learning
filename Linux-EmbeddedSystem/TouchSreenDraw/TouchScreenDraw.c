/*
 * file_name: TouchScreenDraw.c
 *
 * Copyright (C) 2015 Hoping Tang
 * 
 * This file 
 *
 */

//#include "config.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <signal.h>
#include <sys/fcntl.h>
#include <sys/ioctl.h>
#include <sys/mman.h>
#include <sys/time.h>

#include "./include/tslib.h"
#include "fbutils.h"

static int palette [] =
{
	0x000000, 0xffe080, 0xffffff, 0xe0c0a0, 0x304050, 0x80b8c0
};

#define NR_COLORS (sizeof(palette) / sizeof(palette[0]))

struct ts_button {
	int x, y, w, h;
	char *text;
	int flags;
#define BUTTON_ACTIVE 0x00000001
};

static int button_palette [6] =
{
	1, 4, 2,
	1, 5, 0
};

#define NR_BUTTONS 3

static struct ts_button buttons[NR_BUTTONS]; 

static void sig(int sig)
{
	close_framebuffer();
	fflush(stderr);
	printf("signal %d caught\n", sig);
	fflush(stdout);
	exit(1);
}

static void button_draw (struct ts_button *button)
{
        int s = (button->flags & BUTTON_ACTIVE) ? 3 : 0;
        rect (button->x, button->y, button->x + button->w - 1,
              button->y + button->h - 1, button_palette [s]);
        fillrect (button->x + 1, button->y + 1,
                  button->x + button->w - 2, 
                  button->y + button->h - 2, button_palette [s + 1]);
        put_string_center (button->x + button->w / 2,
                           button->y + button->h / 2,
                           button->text, button_palette [s + 2]);
}

static int button_handle (struct ts_button *button, struct ts_sample *samp)
{
        int inside = (samp->x >= button->x) && (samp->y >= button->y) &&
                (samp->x < button->x + button->w) &&
                (samp->y < button->y + button->h);

        if (samp->pressure > 0) 
	{
                if (inside) 
		{
                        if (!(button->flags & BUTTON_ACTIVE)) 
			{
                                button->flags |= BUTTON_ACTIVE;
                                button_draw (button);
                        }
                } else if (button->flags & BUTTON_ACTIVE) 
			{
                        	button->flags &= ~BUTTON_ACTIVE;
                        	button_draw (button);
                	}
        } else if (button->flags & BUTTON_ACTIVE) 
		{
                	button->flags &= ~BUTTON_ACTIVE;
                	button_draw (button);
                	return 1;
        	}

        return 0;
}

static void refresh_screen ()
{
        int i;

        //fillrect (0, 0, xres - 1, yres - 1, 0);
	fillrect((3 * xres) / 4, 0, xres - 1, yres - 1, 0);
        put_string_center (xres/2, yres/4,   "Test draw program", 1);
        put_string_center (xres/2, yres/4+20,"Enjoy yourself", 2);
	
	// the writing box
        for (i = 0; i < NR_BUTTONS; i++)
                button_draw (&buttons [i]);
	rect((3 * xres) / 4, yres / 2, xres - 1, yres - 1, 2);
}


// cast the pattern in box to the drawing place
void cast(void)
{
	int i, j;
	unsigned long tmp;
	for (j = yres / 2 + 1; j < yres - 1; j += 3)
		for (i = (3 * xres) / 4 + 1; i < xres - 1; i += 3)
		{
			tmp = read_framebuffer(i, j);
			if (tmp != 0)
				pixel((i - (3 * xres) / 4) / 3, (j - yres / 2) / 3, 2);
		}
}

int main()
{
	struct tsdev *ts;
	//int x, y;
	unsigned int i;
	unsigned int mode = 0;
	int quit_pressed = 0;

	char *tsdevice=NULL;

	signal(SIGSEGV, sig);
	signal(SIGINT, sig);
	signal(SIGTERM, sig);

	if( (tsdevice = getenv("TSLIB_TSDEVICE")) != NULL ) {
		ts = ts_open(tsdevice,0);
	} else {
		if (!(ts = ts_open("/dev/input/event0", 0)))
			ts = ts_open("/dev/touchscreen/ucb1x00", 0);
	}

	if (!ts) {
		perror("ts_open");
		exit(1);
	}

	if (ts_config(ts)) {
		perror("ts_config");
		exit(1);
	}

	if (open_framebuffer()) {
		close_framebuffer();
		exit(1);
	}

	//x = xres/2;
	//y = yres/2;

	for (i = 0; i < NR_COLORS; i++)
		setcolor (i, palette [i]);

	/* Initialize buttons */
	memset (&buttons, 0, sizeof (buttons));
	buttons [0].w = buttons [1].w = buttons [2].w = xres / 4;
	buttons [0].h = buttons [1].h = buttons [2].h = yres / 6;
	buttons [0].x = buttons [1].x = buttons [2].x = (3 * xres) / 4;
	buttons [0].y = 0;
	buttons [1].y = yres / 6;
	buttons [2].y = yres / 3;
	buttons [0].text = "Done";
	buttons [1].text = "Clean";
	buttons [2].text = "Quit";

	refresh_screen ();

	while (1) 
	{
		struct ts_sample samp;
		int ret;
		ret = ts_read(ts, &samp, 1);

		if (ret < 0) {
			perror("ts_read");
			close_framebuffer();
			exit(1);
		}

		if (ret != 1)
			continue;

		for (i = 0; i < NR_BUTTONS; i++)
		{
			if (button_handle (&buttons [i], &samp))
			{
				switch (i) 
				{
					case 0:
						mode = 1;
						//refresh_screen();
						break;
					case 1:
						mode = 2;
						fillrect(0,0,xres-1,yres-1,0);
						refresh_screen();
						break;
					case 2:
						quit_pressed = 1;
				}
			}
		}

		//printf("%ld.%06ld: %6d %6d %6d\n", samp.tv.tv_sec, samp.tv.tv_usec,\
		//	samp.x, samp.y, samp.pressure);

		if (samp.pressure > 0) 
		{
			if (mode == 0x80000000)
				//line (x, y, samp.x, samp.y, 2);
				if (samp.x)
					fillrect(samp.x - 1, samp.y -1,\
						samp.x + 1, samp.y + 1, 3);
			//x = samp.x;
			//printf("mode = %d\n", mode);
			//y = samp.y;
			if (mode == 0x80000001)
			{
				cast();
				mode = 0x80000000;
			}
			
			if (mode == 0x80000002)
			{
				mode = 0x80000000;
			}
			mode |= 0x80000000;
		} else
			mode &= ~0x80000000;
		if (quit_pressed)
			break;
	}// end of while(1)

	close_framebuffer();

	return 0;
}

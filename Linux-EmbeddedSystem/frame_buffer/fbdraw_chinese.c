#include "frame_buffer.h"
#include "stdio.h"
#include "string.h"

int fbdraw_chinese(struct framebuffer_struct *fb_struct, \
			unsigned char *chinese_str, int x, int y)
{
	unsigned int qu, wei, offset;
	int i, j, k, l;
	int num_per_line = 7;
	int distance = 35;
	unsigned char tmp[2];
	int length;
	FILE *fp;
	unsigned char buffer[34];
	unsigned char key[8] = {\
			0x80, 0x40, 0x20,\
			0x10, 0x08, 0x04,\
			0x02, 0x01\
    				};
	// open the chinese bitmap fonts
	if(NULL == (fp = fopen("./HZK16", "rb")))
	{
		printf("Can't open HZK16.");
		return 1;
	}
	
	length = strlen((const unsigned char *) chinese_str) / 2;
	for (i = 0; i < length; i++)
	{
		// compute location of the chinese character
		qu = chinese_str[0 + i * 2] - 0xa1;
		wei = chinese_str[1 + i * 2] - 0xa1;
		offset = (94 * qu + wei) * 32;
		// read chinese character from bitmap file
		fseek(fp, offset, SEEK_SET);
		fread(buffer, 1, 32, fp);
		// draw
		for (j = 0; j < 16; j++)
		{
			for (k = 0; k < 2; k++)
			{
				for (l = 0; l < 8; l++)
				{
					if (0 != (buffer[j * 2 + k] & key[l]))
					{
						fbdraw_pixel(fb_struct, x + i * 20 + k * 8 + l, y + j, 0x0000ffff);
					}
				}
			}
		}
	}
	return 0;
}



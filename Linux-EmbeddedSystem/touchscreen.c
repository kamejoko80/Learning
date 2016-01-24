/* Draw a circle when touching the screen.
 *
 * Author:    Chenglei Peng
 * Created:   Sep 01, 2010
 * Updated:   Oct 12, 2014
 * Copyright: (C) Chenglei Peng <pcl@nju.edu.cn>
 *
 * This program is an example for Touch Screen. You can modify it under the 
 * terms of the GNU GPL v2. 
 */

#include "lcd.h"
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <linux/input.h>

int pos_x, pos_y;
void *getkey (void);
void *draw (void);


int
main ()
{
  pthread_t thread_id1;
  pthread_t thread_id2;
  

  pthread_create (&thread_id1, NULL, &getkey, NULL);
  pthread_create (&thread_id2, NULL, &draw, NULL);
  
  pthread_join (thread_id1, NULL);
  pthread_join (thread_id2, NULL);
 

  getchar ();
}

void *
draw ()
{
  int x1, y1, i;
  initset ();
  setbkcolor (BLACK);
  clear ();

  while (1)
    {
      
	//加入判断，防止内存泄露
/*	if(pos_x <=25)
		pos_x = 26;
	else if(pos_x >= 775)
		pos_x = 774;
	else if(pos_y <= 25)
		pos_y = 26;
	else if(pos_y >= 455)
		pos_y = 454;
	else
	{
		x1 = pos_x;
		y1 = pos_y;
	};*/
	if(pos_y > 25 && pos_y < 455)
		{
		x1 = pos_x;
		y1 = pos_y;
		}
	else
	{
		if(pos_y<=25)
		{
			pos_y=26;	
			x1 = pos_x;
			y1 = pos_y;
		}
		else if(pos_y>=455)
		{
			pos_y=454;
			x1 = pos_x;
			y1 = pos_y;
		}
	}

/*	else if(pos_y <= 25){
		pos_y = 26;x1 = pos_x;
		}
	else if(pos_y >= 455)
		{pos_y = 454;x1 = pos_x;}
	else;

*/	
      setcolor (WHITE);
      draw_circle (pos_x, pos_y, 20);
      
    }

}



void *
getkey ()
{
  int keys_fd;
  struct input_event t;

  keys_fd = open ("/dev/input/event3", O_RDONLY);
  /* In Kernel V3.0.8, the touch screen device is /dev/input/event3 */

  if (keys_fd <= 0)
    {
      printf ("Opening Touch Screen Device error!\n");
      return 0;
    }

  while (1)
    {
      if (read (keys_fd, &t, sizeof (t)) == sizeof (t))
	{
	  if (t.type == EV_ABS && t.code == ABS_X)
	    pos_x = t.value;

	  else if (t.type == EV_ABS && t.code == ABS_Y)
	    pos_y = t.value;

	  //else if(t.type == EV_ABS&&t.code==ABS_PRESSURE)
	  //    printf("ABS_PRESSURE= %d \r\n",t.value); 
	}
    }
  close (keys_fd);
}

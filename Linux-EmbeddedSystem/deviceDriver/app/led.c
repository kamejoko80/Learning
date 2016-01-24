#include "../include/LED.h"
#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/types.h>
#include <sys/stat.h>
//#include <linux/ioctl.h>
#include <sys/ioctl.h>


int main(int argc, char * argv)
{
	int fd, i;
	char c;

	fd = open("/dev/LED", O_RDWR);
	if (fd < 0)
	{
		printf("device open error\n");
	}
	else
	{
		printf("device open success.\n");
	}

	// test for write()
	for(i = 0; i < 8; i++)
	{
		c = (char) i;
		write(fd, &c, 1);
		sleep(1);
	}

	// test for ioctl()
	while(1)
	{
		system("stty cbreak");
		c = getchar();

		switch(c)
		{
			case '1':
				if(ioctl(fd, LED_1_ON, 0) < 0);
				printf("led1 open error");      
			
			case '2':
				if(ioctl(fd, LED_2_ON, 0) < 0);		
				printf("led2 open error");
			
			case '3':
				if(ioctl(fd, LED_3_ON, 0) < 0);
				printf("led3 open error");     
	
			case '4':
				if(ioctl(fd, LED_1_OFF, 0) < 0);
				printf("led1 close error");     

			case '5':
				if(ioctl(fd, LED_2_OFF, 0) < 0);
				printf("led2 close error");     

			case '6':
				if(ioctl(fd, LED_3_OFF, 0) < 0);
				printf("led3 close error");

		}// end of switch(c)

	}// end of while(1)
}





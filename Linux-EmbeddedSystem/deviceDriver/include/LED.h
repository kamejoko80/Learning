#ifndef __LED__
#define __LED__

// ioctl command definition
#define LED_IOC_MAGIC	'k'
#define LED_1_ON	_IOW(LED_IOC_MAGIC,1,unsigned long)
#define LED_2_ON	_IOW(LED_IOC_MAGIC,2,unsigned long)
#define LED_3_ON	_IOW(LED_IOC_MAGIC,3,unsigned long)
#define LED_1_OFF	_IOW(LED_IOC_MAGIC,4,unsigned long)
#define LED_2_OFF	_IOW(LED_IOC_MAGIC,5,unsigned long)
#define LED_3_OFF	_IOW(LED_IOC_MAGIC,6,unsigned long)

#endif


#include <linux/kernel.h>
#include <linux/module.h>
#include <linux/io.h>
#include <linux/fs.h>
#include <asm/uaccess.h>

#include "../include/LED.h"

#define GPJ0CON 0xE0200240
#define GPJ0DAT 0xE0200244

volatile unsigned int *pConf;
volatile unsigned int *pData;

int led_open(struct inode *inode, struct file *file)
{
	return 0;
}

int led_release(struct inode *inode, struct file *file)
{
	return 0;
}

int led_write(struct file *file, char *buf, size_t length, loff_t *offset)
{
	char c;
	volatile int val;
	copy_from_user(&c, buf, 1);
	val = *pData;
	val &= ~(7 << 3);
	val |= (~c)<<3;
	*pData = val;
	return 1;
}

void led_on_off(int num)
{
	writel( (readl(pConf) &~ (0xf << (8 + 4 * num))) | (0x1 << (8 + 4 * num)), pConf);
	writeb(readb(pData) &~ (1 << (2 + num)), pData);
	printk(KERN_ALERT"LED%d", num);
}

static int led_ioctl(struct file *filp, unsigned int cmd, unsigned long arg)
{
	switch(cmd)
	{
		case	LED_1_ON:
			led_on_off(1);
			printk(KERN_ALERT" on.\n");
			break;

		case	LED_2_ON:
			led_on_off(2);
			printk(KERN_ALERT" on.\n");
			break;

		case	LED_3_ON:
			led_on_off(3);
			printk(KERN_ALERT" on.\n");
			break;

		case	LED_1_OFF:
			led_on_off(1);
			printk(KERN_ALERT" off.\n");
			break;

		case	LED_2_OFF:
			led_on_off(2);
			printk(KERN_ALERT" off.\n");
			break;

		case	LED_3_OFF:
			led_on_off(3);
			printk(KERN_ALERT" off.\n");
			break;

		default:
			break;
	}
	return 0;
}

struct file_operations fop=
{
        open:led_open,
        release:led_release,
        write:led_write,
        unlocked_ioctl:led_ioctl,
};

int init_module(void)
{
        int ret;
        ret = register_chrdev(240, "LED", &fop);

        pConf = ioremap(GPJ0CON, 4);
        pData = ioremap(GPJ0DAT, 4);
        *pConf &= 0xFF000FFF;
        *pConf |= 0xFF111FFF;
        *pData = ~(7 << 3);

        printk("LED is available now!\nHello from kernel space.\n");
        return 0;
}

void cleanup_module(void)
{
        *pData = ~ (0 << 3);
        iounmap(pConf);
        iounmap(pData);

        unregister_chrdev(240, "LED driver");

        printk("LED exit.\nGoogbye, cruel world!\n");
}

MODULE_LICENSE("GPL");


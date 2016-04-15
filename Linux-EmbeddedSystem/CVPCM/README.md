# Computer Vision Passenger Control Module

* .config  
make config file  
in /linux/kernel/

* ov5645.c  
driver for OV5640  
from /linux/kernel/drivers/media/video/  
Driver Structure: ![OV5640_DRIVER](https://lh3.googleusercontent.com/7jsnxkgSMBU3xvV8KxoLITNiqcy_nI-HOm3o7MYqiB2_yZhk_Qi8kV46sbs-Yf-K131zlzRcZ7R8jA=w5000-no)  
#Reference: [iTOP4418/.../OV5640.c](https://github.com/iTOP4418/kernel-3.4.39/blob/topeet_develop/kernel-3.4.39/drivers/media/video/ov5640.c)  

* device.c  
device tree?  
in /linux/kernel/arch/arm/plat-s5p6818/drone/

* axp22-cfg.h  
driver for axp, the voltage controller  
in /linux/kernel/arch/arm/plat-s5p6818/drone/include/

* do_mounts.c 
delay before mmc mounting to avoid its reset
in linux/kernel/init/


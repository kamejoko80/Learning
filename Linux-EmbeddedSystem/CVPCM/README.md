# Computer Vision Passenger Control Module

* .config  
make config file  
in /linux/kernel/

* ov5645.c  
driver for OV5640  
from /linux/kernel/drivers/media/video/  
Driver Structure: ![OV5640_DRIVER](https://lh3.googleusercontent.com/W3xmQLIX32a9APJk7z5zPvNkAnpS1QPinV1XpqXa7c8c4i-QVZA2R1dfUtjRf7ZPqLD71Krj-FJwPQ0vGDc5n7RtyQE)
Reference: [http://blog.csdn.net/hyanglu1573/article/details/12874527](http://blog.csdn.net/hyanglu1573/article/details/12874527)


* device.c  
device tree?  
in /linux/kernel/arch/arm/plat-s5p6818/drone/

* axp22-cfg.h  
driver for axp, the voltage controller  
in /linux/kernel/arch/arm/plat-s5p6818/drone/include/

* do_mounts.c 
delay before mmc mounting to avoid its reset
in linux/kernel/init/


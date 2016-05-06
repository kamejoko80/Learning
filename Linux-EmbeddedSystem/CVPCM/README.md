# Computer Vision Passenger Control Module

#### TODO:
- [x] Figure out how video_register_device() work,
- [x] To find the ov5640 device in `/dev/video*` to prove it.
- [ ] Try to link ov5640 subdev to Clipper host.
- [ ] Set the ov5640 dvp/mipi regs to capture a suitable video shot.
- [ ] Read video stream from ov5640 dvp/mipi.

* .config  
make config file  
in `/linux/kernel/`

* ov5645.c  
driver for OV5640  
link from `/linux/kernel/drivers/media/video/`  
Driver Structure: ![OV5640_DRIVER](https://lh3.googleusercontent.com/7jsnxkgSMBU3xvV8KxoLITNiqcy_nI-HOm3o7MYqiB2_yZhk_Qi8kV46sbs-Yf-K131zlzRcZ7R8jA=w5000-no)  
####Reference: [iTOP4418/.../OV5640.c](https://github.com/iTOP4418/kernel-3.4.39/blob/topeet_develop/kernel-3.4.39/drivers/media/video/ov5640.c)  

* device.c  
device tree?  
link from `/linux/kernel/arch/arm/plat-s5p6818/drone/`

* axp22-cfg.h  
driver for axp, the voltage controller  
link from `/linux/kernel/arch/arm/plat-s5p6818/drone/include/`

* do_mounts.c  
delay before mmc mounting to avoid its reset  
link from `/linux/kernel/init/`

* nxp_capture.c  
link from `/linux/kernel/drivers/media/video/nexell/capture/`

* nxp-vin-clipper.c
link from `/linux/kernel/drivers/media/video/nexell/capture/`

* nxp-decimator.c
link from `/linux/kernel/drivers/media/video/nexell/capture/`

* nxp-v4l2-platformdata.h  
cp from `/linux/kernel/arch/arm/mach-s5p6818/include/mach/`

* nxp-v4l2.c  
cp from `/linux/kernel/drivers/media/video/nexell/`


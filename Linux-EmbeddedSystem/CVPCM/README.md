# Computer Vision Passenger Control Module

### TODO
- [x] Figure out how `video_register_device()` work,
- [x] To find the ov5640 device in `/dev/video*` to prove it.
- [x] Try to link ov5640 subdev to Clipper host.
- [ ] Try to fix nxp-video.c
- [ ] Set the ov5640 dvp/mipi regs to capture a suitable video shot.
- [ ] Read video stream from ov5640 dvp/mipi.

### Framework
When Linux kernel booting, `nxp_v4l2_driver` defined in nxp-v4l2.c
as a `module_platform_driver` will be installed first. 
The installation will run the function `nxp_v4l2_probe()`. 
In `nxp_v4l2_probe()`, `v4l2_device_register()` and `media_device_register()`
is to run to register `v4l2_device` and `media_device` respectively 
(read more about these two functions in `v4l2-framework.txt` and `media-framework.txt`)
, then `create_nxp_capture()` and `register_nxp_capture()` 
to create and register nxp capture device respectively, finally, 
`v4l2_device_register_subdev_nodes()` to register device nodes for all subdevices, 
which is introduced in `v4l2-framework.txt`, too. 
So far, operations in kernel booting time is done.

Two functions `create_nxp_capture()` and `register_nxp_capture()` defined in 
nxp-capture.c, however, are more worth of interest for us.

`create_nxp_capture()` instantialize the structure `nxp_capture`,
and initialize its three 'children'(subdevice) of capture device ---
clipper, decimator and csi --- by `nxp_vin_clipper_init()`, `nxp_decimator_init()` 
and `nxp_csi_init()` respectively. Finaly, it links the csi(source) to clipper(sink),  
clipper(source) to decimator(sink). 

`register_nxp_capture()` 



### File list
* axp22-cfg.h  
driver for axp, the voltage controller  
link from `/linux/kernel/arch/arm/plat-s5p6818/drone/include/`

* device.c  
device tree?  
link from `/linux/kernel/arch/arm/plat-s5p6818/drone/`

* do_mounts.c  
delay before mmc mounting to avoid its reset  
link from `/linux/kernel/init/`

* media-framework.txt  

* nxp-capture.c  
link from `/linux/kernel/drivers/media/video/nexell/capture/`

* nxp-decimator.c  nxp-decimator.h  
link from `/linux/kernel/drivers/media/video/nexell/capture/`

* nxp-v4l2.c  
link from `/linux/kernel/drivers/media/video/nexell/`

* nxp-v4l2-platformdata.h  
cp from `/linux/kernel/arch/arm/mach-s5p6818/include/mach/`

* nxp-video.c  nxp-video.h  
link from `/linux/kernel/drivers/media/video/nexell/`

* nxp-vin-clipper.c  nxp-vin-clipper.h  
link from `/linux/kernel/drivers/media/video/nexell/capture/`

* ov5645_dvp.c  ov5645_mipi.c
driver for OV5640  
link from `/linux/kernel/drivers/media/video/`  
Driver Structure: ![OV5640_DRIVER](https://lh3.googleusercontent.com/7jsnxkgSMBU3xvV8KxoLITNiqcy_nI-HOm3o7MYqiB2_yZhk_Qi8kV46sbs-Yf-K131zlzRcZ7R8jA=w5000-no)  
####Reference: [iTOP4418/.../OV5640.c](https://github.com/iTOP4418/kernel-3.4.39/blob/topeet_develop/kernel-3.4.39/drivers/media/video/ov5640.c)  

* v4l2-framework.txt  
> device instances  
>  |  
>  +-sub-device instances  
>  |  
>  \-V4L2 device nodes  
>      |  
>      \-filehandle instances  


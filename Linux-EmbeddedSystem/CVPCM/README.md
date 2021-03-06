# Computer Vision Passenger Control Module

### TODO
- [x] Figure out how `video_register_device()` work,
- [x] To find the ov5640 device in `/dev/video*` to prove it.
- [x] Try to link ov5640 subdev to Clipper host.
- [x] Try to fix nxp-video.c
- [x] Set the ov5640 dvp/mipi regs to capture a suitable video shot.
- [x] Read video stream from ov5640 dvp/mipi.
- [x] Get image frame in YUYV format.

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

`register_nxp_capture()` register clipper, decimator and csi by `nxp_vin_clipper_register()`, 
`nxp_decimator_register()`, `nxp_csi_register()` respectively. And register our camera by 
`_register_sensor()`. Then, it links clipper and decimator to their video nodes. Finaly, 
it sets up the interrupt handler by `request_irq()`.

The instances of clipper, decimator and csi is similar, 
so we just introduce clipper and sensor parts. 

The most important function in the initialization of clipper is `_init_entities()`, 
in which we instantialize the structure `nxp_vin_clipper` and setup the subdevice and video node. 
Clipper subdevice is initialized by `v4l2_subdev_init()` with the subdev operations structure. 
After setting up the subdev data, video node is created and linked to the subdev by 
`media_entity_init()`, `create_nxp_video()` and `media_entity_create_link()`. 
Clipper is register by `v4l2_device_register_subdev()` and `register_nxp_video()`, 
which register clipper subdevice and the video node for it.

Similarly, sensor subdevice is registered by `v4l2_i2c_new_subdev_board()`, which calls the sensor driver register. 
The driver contains `probe()` and initialize the driver structure for the camera.

The probe() function init priv structure and create the media entity. We also add `ov5640_video_prove()` in it
to put on the power for camera and check the i2c to see if it can work.

Our application program is in `./camera/`. It contains Camera class for utilzing the video node and ioctl command.
In `init_device()`, we set the format and init the mmap.
In `start_capturing()`, we setup the frame buffer and call the `STREAMON` ioctl.
`STREAMON` will call the `streamon` instance function in `nxp-video.c` and then the instance function
refer to the `s_stream()` function in `nxp-vin-clipper.c` and then the `s_stream()` in `ov5640_dvp.c`.
The `s_stream()` function will write the regs to the camera to set up it.

The registers setting cound see more in the datasheet of OV5640.

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


# Make menuconfig
>  |
>  +-Device Drivers
>      |
>      +-Multimedia support
>          |
>          +-Nexell V4L2 Devices
>              |
>              +-Nexell Video capture interface
>                  |
>                  |





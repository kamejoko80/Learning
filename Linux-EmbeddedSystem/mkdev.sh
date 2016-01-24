#!/bin/sh

mknod console c 5 1
mknod dsp c 14 3
mknod audio c 14 4
mknod fb0 c 29 0
ln -s fb0 fb
mknod null c 1 3
mknod zero c 1 5
mknod ram0 b 1 0
mknod ram1 b 1 1
ln -s ram0 root
mknod rtc c 250 0
mknod ptmx c 5 2
mkdir pts
mknod tty c 5 0
mknod tty0 c 4 0 
mknod tty1 c 4 1
mknod mmcblk1 b 179 8
mknod mmcblk1p1 b 179 9
mkdir input
mknod input/event0 c 13 64
mknod input/event1 c 13 65
mknod input/event2 c 13 66

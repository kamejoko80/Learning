# Image Registration


Before reading `initialization.h`, you had better read [the webpages](http://blog.sina.com.cn/s/blog_6249ce370100ezlf.html)  
which will give you the basic knowledge of interpolation.

You should remember the formulation blow  
 $$f(i+u,j+v) = (1-u)(1-v)f(i,j) + (1-u)vf(i,j+1) + u(1-v)f(i+1,j) + uvf(i+1,j+1)$$  
which will be very important. Because the speed of floating point arithmetic
is slower than integer arithmetic, so we transform u,v,1-u and 1-v into integer
in the way of multipling them by 16.

As a result,the value of $$f(i+u,j+v)$$ is multiplied by `16*16`,
so we will divided the value by `16*16` in the end by the way of swift the value 8 bits right.

%Gaussian noise:  Mainly produced by the internal resistive components
%Salt & pepper noise:  Mainly produced by image Cutting
%poisson noise:  Mainly produced by photoelectric conversion

I = imread('cameraman.tif');

Gaussian = imnoise(I, 'gaussian');
Poisson = imnoise(I, 'poisson');
Salt_pepper = imnoise(I, 'salt & pepper');

Ave_Gau = Myfilter(Gaussian, 'average');
Ave_Poi = Myfilter(Poisson, 'average');
Ave_Sal = Myfilter(Salt_pepper, 'average');

Med_Gau = Myfilter(Gaussian, 'median');
Med_Poi = Myfilter(Poisson, 'median');
Med_Sal = Myfilter(Salt_pepper, 'median');

LPF_Gau = Myfilter(Gaussian, 'low pass filter');
LPF_Poi = Myfilter(Poisson, 'low pass filter');
LPF_Sal = Myfilter(Salt_pepper, 'low pass filter');

SNR_Gau = imsnr(Gaussian, I);
SNR_Poi = imsnr(Poisson, I);
SNR_Sal = imsnr(Salt_pepper, I);
SNR_Ave_Gau = imsnr(Ave_Gau, I);
SNR_Ave_Poi = imsnr(Ave_Poi, I);
SNR_Ave_Sal = imsnr(Ave_Sal, I);
SNR_Med_Gau = imsnr(Med_Gau, I);
SNR_Med_Poi = imsnr(Med_Poi, I);
SNR_Med_Sal = imsnr(Med_Sal, I);
SNR_LPF_Gau = imsnr(LPF_Gau, I);
SNR_LPF_Poi = imsnr(LPF_Poi, I);
SNR_LPF_Sal = imsnr(LPF_Sal, I);

MSE_Gau = immse(Gaussian, I);
MSE_Poi = immse(Poisson, I);
MSE_Sal = immse(Salt_pepper, I);
MSE_Ave_Gau = immse(Ave_Gau, I);
MSE_Ave_Poi = immse(Ave_Poi, I);
MSE_Ave_Sal = immse(Ave_Sal, I);
MSE_Med_Gau = immse(Med_Gau, I);
MSE_Med_Poi = immse(Med_Poi, I);
MSE_Med_Sal = immse(Med_Sal, I);
MSE_LPF_Gau = immse(LPF_Gau, I);
MSE_LPF_Poi = immse(LPF_Poi, I);
MSE_LPF_Sal = immse(LPF_Sal, I); 


subplot(3,4,1);imshow(Gaussian);
title({'Gaussian',['SNR = ', int2str(int16(SNR_Gau))],['MSE = ',int2str(int16(MSE_Gau))]});
subplot(3,4,2);imshow(Ave_Gau);
title({'Average',['SNR = ', int2str(int16(SNR_Ave_Gau))],['MSE = ',int2str(int16(MSE_Ave_Gau))]});
subplot(3,4,3);imshow(Med_Gau);
title({'Median',['SNR = ', int2str(int16(SNR_Med_Gau))],['MSE = ',int2str(int16(MSE_Med_Gau))]});
subplot(3,4,4);imshow(LPF_Gau);
title({'low pass filter',['SNR = ', int2str(int16(SNR_LPF_Gau))],['MSE = ',int2str(int16(MSE_LPF_Gau))]});


subplot(3,4,5);imshow(Poisson);
title({'Poisson',['SNR = ', int2str(int16(SNR_Poi))],['MSE = ',int2str(int16(MSE_Poi))]});
subplot(3,4,6);imshow(Ave_Poi);
title({'',['SNR = ', int2str(int16(SNR_Ave_Poi))],['MSE = ',int2str(int16(MSE_Ave_Poi))]});
subplot(3,4,7);imshow(Med_Poi);
title({'',['SNR = ', int2str(int16(SNR_Med_Poi))],['MSE = ',int2str(int16(MSE_Med_Poi))]});
subplot(3,4,8);imshow(LPF_Poi);
title({'',['SNR = ', int2str(int16(SNR_LPF_Poi))],['MSE = ',int2str(int16(MSE_LPF_Poi))]});


subplot(3,4,9);imshow(Salt_pepper);
title({'Salt & pepper',['SNR = ', int2str(int16(SNR_Sal))],['MSE = ',int2str(int16(MSE_Sal))]});
subplot(3,4,10);imshow(Ave_Sal);
title({'',['SNR = ', int2str(int16(SNR_Ave_Sal))],['MSE = ',int2str(int16(MSE_Ave_Sal))]});
subplot(3,4,11);imshow(Med_Sal);
title({'',['SNR = ', int2str(int16(SNR_Med_Sal))],['MSE = ',int2str(int16(MSE_Med_Sal))]});
subplot(3,4,12);imshow(LPF_Sal);
title({'',['SNR = ', int2str(int16(SNR_LPF_Sal))],['MSE = ',int2str(int16(MSE_LPF_Sal))]});

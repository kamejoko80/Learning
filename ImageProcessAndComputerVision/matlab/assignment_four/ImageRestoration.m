clc;
clear;
close all;

figure(1);
DegradedLena = rgb2gray(imread('DegradedLena.png'));
subplot(2,2,2);imshow(DegradedLena);title('Degraded Lena');
DegradedLenaC = fftshift(fft2(DegradedLena));
subplot(2,2,3);imshow(log(1 + abs(DegradedLenaC)),[]);title('FFT');
w = 8;
h = 15;
DegradedLenaC(137 - w : 137 + w, 190 - h : 190 + h) = 0;
DegradedLenaC(201 - w : 201 + w, 190 - h : 190 + h) = 0;
RestoredLena = real(ifft2(ifftshift(DegradedLenaC)));
subplot(2,2,4);imshow(RestoredLena, []);title('Restored Lena');


figure(2);
CapWord = rgb2gray(imread('CapWord.jpg'));
subplot(2,2,2);imshow(CapWord);
CapWordC = fftshift(fft2((CapWord)));
subplot(2,2,3);imshow(log(1 + abs(CapWordC)),[]);
length = 20;
theta = 0;
NSR = 0.005;
PSF = fspecial('motion', length, theta);
subplot(2,2,4);imshow(deconvwnr(CapWord, PSF, NSR));

figure(3);
[Defocus, map] = imread('Defocus.bmp');
Defocus = ind2gray(Defocus, map);
subplot(2,2,2);imshow(Defocus);
DefocusC = fftshift(fft2(Defocus));
subplot(2,2,3);imshow(log(1 + abs(DefocusC)),[]);
radius = 8;
NSR = 0.001;
PSF = fspecial('disk', radius);
subplot(2,2,4);imshow(deconvwnr(Defocus, PSF, NSR));

figure(4);
LicenseNum = rgb2gray(imread('LicenseNum.bmp'));
    % this bmp has no map
subplot(2,2,2);imshow(LicenseNum);
LicenseNumC = fftshift(fft2((LicenseNum)));
subplot(2,2,3);imshow(log(1 + abs(LicenseNumC)),[]);
length = 9;
theta = 45;
NSR = 0.01;
PSF = fspecial('motion', length, theta);
subplot(2,2,4);imshow(deconvwnr(LicenseNum, PSF, NSR));

figure(5);
[Motion, map] = imread('Motion.bmp');
Motion = ind2gray(Motion, map);
subplot(2,2,2);imshow(Motion);
MotionC = fftshift(fft2(Motion));
subplot(2,2,3);imshow(log(1 + abs(MotionC)),[]);
length = 41;
theta = 0;
NSR = 0.0005;
PSF = fspecial('motion', length, theta);
subplot(2,2,4);imshow(deconvwnr(Motion, PSF, NSR));


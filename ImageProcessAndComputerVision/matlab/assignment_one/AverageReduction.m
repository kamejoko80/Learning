clc;
clear;
close all;
%take colorful photos with noise of the same scene for more than 20,
%reducing the power of the noise by accumulating and averaging.

num = 1;
SUM = imread(['photos(', int2str(num), ').jpg']);
SUM = im2double(SUM);
Origin = SUM;
subplot(2,1,1);
%imshow(Origin);
imagesc(Origin);title('Origin');

num = 26;
PSNR = zeros(num - 1,1);
MSE = zeros(num - 1,1);

for count = 2 : num
    %str = strcat('photos(', int2str(count), ').jpg');
    I = imread(['photos(', int2str(count), ').jpg']);
    SUM = SUM + im2double(I);
end %end count = 2 : num

Average = SUM / num;
subplot(2,1,2);
%imshow(Average);
imagesc(Average);title('Average');

[PSNR(1), MSE(1)] = Psnr(Origin, Average);
SUM = Origin;

for count = 2 : (num - 1)
    I = imread(['photos(', int2str(count), ').jpg']);
    SUM = SUM + im2double(I);
    cAverage = SUM / count;
    [PSNR(count), MSE(count)] = Psnr(Average, cAverage);
end %end count = 2 : num

figure(2);
plot(PSNR);title('PSNR');
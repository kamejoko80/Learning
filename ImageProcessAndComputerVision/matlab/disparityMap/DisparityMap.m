% cam0,1:        camera matrices for the rectified views, in the form [f 0 cx; 0 f cy; 0 0 1], where
%   f:           focal length in pixels
%   cx, cy:      principal point  (note that cx differs between view 0 and 1)
% 
% doffs:         x-difference of principal points, doffs = cx1 - cx0
% 
% baseline:      camera baseline in mm
% 
% width, height: image size
% 
% ndisp:         a conservative bound on the number of disparity levels;
%                the stereo algorithm MAY utilize this bound and search from d = 0 .. ndisp-1
% 
% isint:         whether the GT disparites only have integer precision (true for the older datasets;
%                in this case submitted floating-point disparities are rounded to ints before evaluating)
% 
% vmin, vmax:    a tight bound on minimum and maximum disparities, used for color visualization;
%                the stereo algorithm MAY NOT utilize this information
% 
% dyavg, dymax:  average and maximum absolute y-disparities, providing an indication of
%                the calibration error present in the imperfect datasets.

cam0=[2826.171 0 1292.2; 0 2826.171 965.806; 0 0 1];
cam1=[2826.171 0 1415.97; 0 2826.171 965.806; 0 0 1];
doffs=123.77;
baseline=178.089;
width=2820;
height=1920;
ndisp=260;
isint=0;
vmin=34;
vmax=220;
dyavg=0;
dymax=0;

f = 2826.171;
Img0 = double(rgb2gray(imread('piano0.png')));
Img1 = double(rgb2gray(imread('piano1.png')));

% Img0 = double(rgb2gray(imread('0.png')));
% Img1 = double(rgb2gray(imread('1.png')));
% [height, width] = size(Img0);

disparityMap = zeros(height, width);
n = 51;
w = width - n;
% searchMax = 300;
% for i = ((n + 1) / 2):(height - (n - 1) / 2)
for i = 800:1600
    % for j = ((n + 1) / 2 + searchMax):(width - (n - 1) / 2 - searchMax)
    for j = 1000:2000
        % d = searchMatchPattern(Img0, Img1, i, j, ndisp);
        smin = 1000 * (255.^2);
        for k = (j - ndisp):j
            a = Img0((i - (n - 1) / 2):(i + (n - 1) / 2), (j - (n - 1) / 2):(j + (n - 1) / 2));
            b = Img1((i - (n - 1) / 2):(i + (n - 1) / 2), (k - (n - 1) / 2):(k + (n - 1) / 2));
            % s = SSD(a, b);
            s = sum((a(:) - b(:)) .^ 2);
            if (s < smin)
                smin = s;
                kmatch = k;
            end
        end
        d = abs(kmatch - j);
        
        %depth = baseline * f / (d + doffs);
        disparityMap(i, j) = d; %disparity;
    end
end

depthMap = (baseline * f) ./ (disparityMap + doffs);

figure;
disparityMapshow = disparityMap(800:1600,1000:2000);
disparityMapshow = disparityMapshow ./ max(max(disparityMapshow));
imshow(disparityMapshow);

figure;
depthMapshow = depthMap(800:1600,1000:2000);
depthMapshow = depthMapshow ./ max(max(depthMapshow));
surf(-depthMapshow);shading flat;view(0,90);axis equal;


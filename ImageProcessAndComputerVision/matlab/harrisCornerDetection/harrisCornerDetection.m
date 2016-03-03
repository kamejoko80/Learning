% Load images.
% lighthouse = imageSet('./lighthouse');
% read images.
lighthouse1 = imread('./lighthouse/lighthouse1.jpg');
lighthouse2 = imread('./lighthouse/lighthouse2.jpg');

% Display images to be stitched
% montage(lighthouse.ImageLocation);

% Initialize features
lighthouse1_gray = rgb2gray(lighthouse1);
lighthouse2_gray = rgb2gray(lighthouse2);

points1 = detectHarrisFeatures(lighthouse1_gray);
points2 = detectHarrisFeatures(lighthouse2_gray);

[feature1, valid_points1] = extractFeatures(lighthouse1_gray, points1);
[feature2, valid_points2] = extractFeatures(lighthouse2_gray, points2);

indexPairs = matchFeatures(feature1, feature2) ;
matchedPoints1 = valid_points1(indexPairs(:, 1));
matchedPoints2 = valid_points2(indexPairs(:, 2));

figure(1); 
ax = axes;
showMatchedFeatures(lighthouse1, lighthouse2, matchedPoints1, ...
                    matchedPoints2, 'montage', 'Parent', ax);
title(ax, 'Candidate point matches');
legend(ax, 'Matched points 1','Matched points 2');






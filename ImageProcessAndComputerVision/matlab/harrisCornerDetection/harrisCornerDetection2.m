Dahuo1 = imread('Dahuo1.jpg');
Dahuo2 = imread('Dahuo2.jpg');
Dahuo3 = imread('Dahuo3.jpg');

Dahuo1_gray = rgb2gray(Dahuo1);
Dahuo2_gray = rgb2gray(Dahuo2);
Dahuo3_gray = rgb2gray(Dahuo3);

points1 = detectHarrisFeatures(Dahuo1_gray);
points2 = detectHarrisFeatures(Dahuo2_gray);
points3 = detectHarrisFeatures(Dahuo3_gray);

[feature1, valid_points1] = extractFeatures(Dahuo1_gray, points1);
[feature2, valid_points2] = extractFeatures(Dahuo2_gray, points2);
[feature3, valid_points3] = extractFeatures(Dahuo3_gray, points3);

indexPairs1 = matchFeatures(feature1, feature2);
indexPairs2 = matchFeatures(feature2, feature3);

matchedPoints11 = valid_points1(indexPairs1(:, 1));
matchedPoints12 = valid_points2(indexPairs1(:, 2));
matchedPoints21 = valid_points2(indexPairs2(:, 1));
matchedPoints22 = valid_points3(indexPairs2(:, 2));

figure(1);
ax1 = axes;
showMatchedFeatures(Dahuo1, Dahuo2, matchedPoints11, ...
                    matchedPoints12, 'montage', 'Parent', ax1);
figure(2);
ax2 = axes;
showMatchedFeatures(Dahuo2, Dahuo3, matchedPoints21, ...
                    matchedPoints22, 'montage', 'Parent', ax2);





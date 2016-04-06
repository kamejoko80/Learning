function [L, m, new_bg_mean, new_bg_stddev, fg] = fg_extract(bg_mean, bg_stddev, cur_frame, height, width)

% Single Gaussian Background Model

defaultVarTh = 3;  % threshold for standard deviation
alpha = 0.05;

fg = ones(height, width);
frame_gray = rgb2gray(cur_frame);

% foreground extract
for i=1:height
    for j=1:width
        diff = double(frame_gray(i, j)) - bg_mean(i, j);
        if(abs(diff) <= defaultVarTh * bg_stddev(i, j))
            fg(i,j) = 0;
            % background update
            bg_mean(i, j) = bg_mean(i, j) + alpha * diff;
            diff = double(frame_gray(i,j)) - bg_mean(i, j);
            bg_stddev(i, j) = sqrt(bg_stddev(i, j) * bg_stddev(i, j) + alpha * (diff * diff - bg_stddev(i, j) * bg_stddev(i, j)));
        end
    end
end

new_bg_mean = bg_mean;
new_bg_stddev = bg_stddev;

% fg = filter2(fspecial('average',9),uint8(fg));
% fg = round(fg);

% label connected conponents
[l] = bwlabel(fg,8);
% measures Area for each labeled region in the label matrix L.
S = regionprops(l, 'Area');
fg = ismember(l, find([S.Area] >= 650));
[L, m] = bwlabel(fg, 8);



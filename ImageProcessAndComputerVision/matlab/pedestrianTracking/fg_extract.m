function [L,m,new_bg,fg,fg_color] = fg_extract(cur_bg,cur_frame,height,width)

% Single Gaussian Background Model

defaultVarTh = 3;  % threshold for standard deviation
defaultVar = 20; % standard deviation (sigma) initialized as 20
alpha = 0.05;

fg = ones(height, width);
fg_color = ones(height, width, 3);
    
frame_gray = rgb2gray(cur_frame);
  
%foreground extract
for i=1:height
    for j=1:width
        mean = cur_bg(i,j,1);
        diff = double(frame_gray(i,j)) - mean;
        if(abs(diff) <= defaultVarTh*defaultVar)
            fg(i,j) = 0;
            fg_color(i,j,:) = 0;
        else
            fg(i,j) = 255;
            fg_color(i,j,:) = double(cur_frame(i,j,:));
        end
    end
end

%background update
for i=1:height
    for j=1:width
        mean = cur_bg(i,j,1);
        var = cur_bg(i,j,2);
        diff = double(frame_gray(i,j)) - mean;
        if(abs(diff) <= defaultVarTh * defaultVar)
            cur_bg(i,j,1) = cur_bg(i,j,1) + alpha*diff;
            mean = cur_bg(i,j,1);
            diff = double(frame_gray(i,j)) - mean;
            cur_bg(i,j,2) = sqrt(var*var + alpha*(diff*diff - var*var));
        end
    end
end

% label connected conponents
[l] = bwlabel(fg,8);
% measures Area for each labeled region in the label matrix L.
S = regionprops(l, 'Area');
fg = ismember(l, find([S.Area] >= 650));

[L, m] = bwlabel(fg, 8);

new_bg = cur_bg;



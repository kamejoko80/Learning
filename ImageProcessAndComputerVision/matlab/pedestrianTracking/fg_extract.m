function [L,m,new_bg,fg,fg_color] = fg_extract(cur_bg,cur_frame,height,width)

% Single Gaussian Background Model

defaultVarTh = 3;  % threshold for standard deviation
defaultVar = 20; % standard deviation (sigma) initialized as 20
alpha = 0.05;

fg = ones(height, width);
fg_color = ones(height, width, 3);
    
frame_gray = rgb2gray(cur_frame);

% foreground extract
for i=1:height
    for j=1:width
        mean = cur_bg(i,j,1);
        var = cur_bg(i,j,2);
        diff = double(frame_gray(i,j)) - mean;
        if(abs(diff) <= defaultVarTh*defaultVar)
            fg(i,j) = 0;
            % background update
            cur_bg(i,j,1) = cur_bg(i,j,1) + alpha*diff;
            mean = cur_bg(i,j,1);
            diff = double(frame_gray(i,j)) - mean;
            cur_bg(i,j,2) = sqrt(var*var + alpha*(diff*diff - var*var));
        end
    end
end

fg_color(:,:,1) = cur_frame(:,:,1) .* uint8(fg);
fg_color(:,:,2) = cur_frame(:,:,2) .* uint8(fg);
fg_color(:,:,3) = cur_frame(:,:,3) .* uint8(fg);

% fg = filter2(fspecial('average',9),uint8(fg));
% fg = round(fg);

% label connected conponents
[l] = bwlabel(fg,8);
% measures Area for each labeled region in the label matrix L.
S = regionprops(l, 'Area');
fg = ismember(l, find([S.Area] >= 650));
[L, m] = bwlabel(fg, 8);

new_bg = cur_bg;

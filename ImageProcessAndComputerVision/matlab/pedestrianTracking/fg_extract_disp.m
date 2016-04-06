function [L, m, fg, fg_disp] = fg_extract_disp(bg, cur_frame)

% Global Threshold Background Subtraction

threshold = 30;

% foreground extract
fg = (cur_frame - bg > threshold);

% label connected conponents
[l] = bwlabel(fg, 8);
% measures Area for each labeled region in the label matrix L.
S = regionprops(l, 'Area');
fg = ismember(l, find([S.Area] >= 650));
[L, m] = bwlabel(fg, 8);

fg_disp = cur_frame .* uint8(fg);

clear all;
framepath = '..\..\..\..\..\data\pedestrainTrackingFrame\';
dispPath = '..\..\..\..\..\data\pedestrainTrackingFrame\disp290x202\';

frame = imread([framepath 'right' num2str(0) '.png']);
frame_gray = rgb2gray(frame);
% [height,width] = size(frame_gray);
% bg_mean = double(frame_gray); % init mean
% bg_stddev = ones(height, width) * 20; % init Standard Deviation (sigma) ... not Variance
last_num = 0;
cur_num = 0;

disp = load([dispPath 'disp' num2str(1) '.txt']);
disp = (abs(disp) ./ max(max(abs(disp)))) .* 255;
[height,width] = size(disp);
bg = uint8(disp);

%kalman initialization
P = [100 0 0 0;0 100 0 0;0 0 100 0;0 0 0 100];

% n is the index of frames
for n = 164:203
    % start timer
    tic
    cur_frame = imread([framepath 'right' num2str(n) '.png']);
    disp = load([dispPath 'disp' num2str(n) '.txt']);
    disp = (abs(disp) ./ max(max(abs(disp)))) .* 255;
    disp = uint8(disp);
    %[L, m, bg_mean, bg_stddev, fg] = fg_extract(bg_mean, bg_stddev, cur_frame, height, width);
    [L, m, fg, fg_disp] = fg_extract_disp(bg, disp);
    
    fg_color = ones(height, width, 3);
    fg_color(:,:,1) = cur_frame(:,:,1) .* uint8(fg);
    fg_color(:,:,2) = cur_frame(:,:,2) .* uint8(fg);
    fg_color(:,:,3) = cur_frame(:,:,3) .* uint8(fg);
    
    figure(1)
    imshow(cur_frame); hold on;
    box = regionprops(L, 'BoundingBox');
    for i=1:m
        rectangle('position', box(i).BoundingBox, 'edgecolor', 'r');       
    end
    
    figure(2)
    imshow(edge(uint8(fg_disp), 'sobel'));
    
    if n==1
        if m>0
            last_obj = feature_extract(L,m,fg_color);
            last_num = m;
        end
    else
        cur_num = m;
        if last_num ~= 0 && cur_num ~= 0
            cur_obj = feature_extract(L,m,fg_color);
            [P,pre_obj] = kalman(last_obj,cur_obj,last_num,cur_num,P);
            last_obj = pre_obj;
        end
        if last_num ~= 0 && cur_num == 0
            clear last_obj;
                clear cur_obj;
        end
        if last_num == 0 && cur_num ~= 0
            last_obj = feature_extract(L,m,fg_color);     
        end
        last_num = cur_num;
    end
    figure(3)
    imshow(cur_frame);hold on;
    if(last_num ~= 0)
        for i=1:last_num         
            x = last_obj(i).X;
            y = last_obj(i).Y;
            rectangle('Position',[x,y,3,3],'Curvature',[1,1],  'FaceColor','r');
            text(x-5,y-5,num2str(last_obj(i).No),'Color','r');
        end
    end
    
    toc
end

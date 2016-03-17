clear all;
filepath='..\..\..\..\..\data\pedestrainTrackingFrame\';

frame = imread([filepath 'right' num2str(0) '.png']);
frame_gray = rgb2gray(frame);
[height,width] = size(frame_gray);

cur_bg = zeros(height,width,2);
cur_bg(:,:,1) = double(frame_gray); % init mean
cur_bg(:,:,2) = 20;  % init Standard Deviation ... not Variance
last_num = 0;
cur_num = 0;
    
%kalman initialization
P = [100 0 0 0;0 100 0 0;0 0 100 0;0 0 0 100];

% n is the index of frames
for n = 1:203
    % start timer
    tic
    cur_frame = imread([filepath 'right' num2str(n) '.png']);
    [L,m,new_bg,fg,fg_color] = fg_extract(cur_bg,cur_frame,height,width);
    cur_bg = new_bg;
    
    figure(1)
    imshow(cur_frame);
    figure(2)
    imshow(uint8(fg_color));
    box = regionprops(L, 'BoundingBox');
    for i=1:m
        rectangle('position', box(i).BoundingBox, 'edgecolor', 'r');       
    end
    
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

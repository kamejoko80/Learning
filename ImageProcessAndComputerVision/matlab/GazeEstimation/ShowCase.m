%%
clear;
clc;

filepath = 'MPIIGaze/Data/Normalized/p00/';
for index = 1:2
    if index == 1 
        readname = [filepath, 'day04.mat'];
        temp = load(readname);
        num_i = 882;
    end
    if index == 2
        readname = [filepath, 'day09.mat'];
        temp = load(readname);
        num_i = 26;
    end
        img = temp.data.left.image(num_i, :,:);
        img = reshape(img, 36,60);

        Lable_left = temp.data.left.gaze(num_i, :)';
        gaze_theta = asin((-1)*Lable_left(2));
        gaze_phi = atan2((-1)*Lable_left(1), (-1)*Lable_left(3));

        headpose = temp.data.left.pose(num_i, :);
        M = rodrigues(headpose);
        Zv = M(:,3);
        head_theta = asin(Zv(2));
        head_phi = atan2(Zv(1), Zv(3));
        theta = gaze_theta - head_theta;
        phi = gaze_phi - head_phi;
        subplot(1,2,index);
        imshow(img);
        title(['theta:', int2str(int8(theta*180/3.1415926)), '   phi:', int2str(int8(phi*180/3.1415926))]);
end


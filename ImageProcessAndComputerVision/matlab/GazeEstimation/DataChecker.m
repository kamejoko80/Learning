%%
clear;
clc;

filepath = 'p00/';
savepath = '/DATASET/MPIIGaze/Data/Normalized/vis/';
path = dir(filepath);
path = path(3:end);
files = {path.name};

total_num=0;
for num_f=1:length(files)
    readname = [filepath, files{num_f}];
    temp = load(readname);
    total_num = total_num+length(temp.filenames);
end

fprintf('File Path Ready!\n');

index = 0;

for num_f=1:length(files)
    readname = [filepath, files{num_f}];
    temp = load(readname);

    num_data = length(temp.filenames(:,1));
    for num_i=1:num_data
        % for left
        index = index+1;
        img = temp.data.left.image(num_i, :,:);
        img = reshape(img, 36,60);


        Lable_left = temp.data.left.gaze(num_i, :)';
        theta = asin((-1)*Lable_left(2)) *180/3.1415926;
        phi = atan2((-1)*Lable_left(1), (-1)*Lable_left(3))*180/3.1415926;


        headpose = temp.data.left.pose(num_i, :);
        M = rodrigues(headpose);
        Zv = M(:,3);
        %theta = asin(Zv(2));
        theta = theta - asin(Zv(2))*180/3.1415926;
        phi = phi - atan2(Zv(1), Zv(3))*180/3.1415926;
        %phi = atan2(Zv(1), Zv(3));
        newfolder = [savepath, 'theta', int2str(int8(theta / 3) * 3), 'phi', int2str(int8(phi / 3) * 3)];
        if ~exist(newfolder, 'dir')
            mkdir(newfolder);
        end
        imwrite(img, [newfolder, '/',int2str(index), 'theta', int2str(int8(theta)),'phi',int2str(int8(phi)), '.jpg']);

        % for right
        index = index+1;
%         img = temp.data.right.image(num_i, :,:);
%         img = reshape(img, 36, 60);
%
%         Lable_right = temp.data.right.gaze(num_i,:)';
%         theta = asin((-1)*Lable_right(2));
%         phi = - atan2((-1)*Lable_right(1), (-1)*Lable_right(3));
%
%         headpose = temp.data.right.pose(num_i, :);
%         M = rodrigues(headpose);
%         Zv = M(:,3);
%         theta = asin(Zv(2));
%         phi = - atan2(Zv(1), Zv(3));
    end
    fprintf('%d / %d !\n', num_f, length(files));
end

fprintf('Saving\n');

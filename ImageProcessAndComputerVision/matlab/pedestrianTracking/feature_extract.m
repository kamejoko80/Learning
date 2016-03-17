function obj = feature_extract(L,m,cur_frame)

box=regionprops(L,'BoundingBox');
centroid = regionprops(L,'Centroid');
obj = struct;

for i = 1:m
    if box(i).BoundingBox(4)<40 && box(i).BoundingBox(2)+box(i).BoundingBox(4)/2-20>=1 && box(i).BoundingBox(2)+box(i).BoundingBox(4)/2+20<=200
        box(i).BoundingBox(2)=box(i).BoundingBox(2)+box(i).BoundingBox(4)/2-20;
        box(i).BoundingBox(4) = 40;
    end
    
    if box(i).BoundingBox(3)<40 && box(i).BoundingBox(1)+box(i).BoundingBox(3)/2-30>=1 && box(i).BoundingBox(1)+box(i).BoundingBox(3)/2+30<=291
        box(i).BoundingBox(1)=box(i).BoundingBox(1)+box(i).BoundingBox(3)/2-30;
        box(i).BoundingBox(3) = 60;
    end
    
    A = cur_frame(round(box(i).BoundingBox(2)):round(box(i).BoundingBox(2)+box(i).BoundingBox(4)-1),...
        round(box(i).BoundingBox(1)):round(box(i).BoundingBox(1)+box(i).BoundingBox(3)-1),:);
    H = hist(A);
    
    obj(i).hist = H;
    obj(i).X = centroid(i).Centroid(1);
    obj(i).Y = centroid(i).Centroid(2);
    
    % init Velocity of pedestrians
    obj(i).Vx = 0;
    obj(i).Vy = 15;
    obj(i).W = round(box(i).BoundingBox(1)+box(i).BoundingBox(3)-round(box(i).BoundingBox(1)));
    obj(i).H = round(box(i).BoundingBox(2)+box(i).BoundingBox(4)-round(box(i).BoundingBox(2)));
    obj(i).No = i;
    obj(i).behind_match = 0;
end
% add a Christmas hats on your Wechat Avatar

% Create a detector object.
faceDetector = vision.CascadeObjectDetector;

% Read input image.
I = imread('facedetect3.jpg');

% Hat parameters
Hat = imread('ChristmasHat3.png');
Hat_size_x = 700;
% Hat_offseet_y = 
% Detect faces.
bboxes = step(faceDetector, I);
face_beginx = bboxes(1);
face_beginy = bboxes(2);
face_sizex = bboxes(3);
face_sizey = bboxes(4);

% Annotate detected faces.

%% IFaces = insertObjectAnnotation(I, 'rectangle', bboxes, 'Face');
%%


r = face_sizex / Hat_size_x;
Hat = imresize(Hat, r);
[Hatx, Haty, temp] = size(Hat);

IFaces = I;

for i = 1 : Hatx
    for j = 1 : Haty
        if ((Hat(i, j, 1) + Hat(i, j, 2) + Hat(i, j, 3)) ~= 0)
            for k = 1 : 3
                if (bboxes(3) - floor(Hatx / 3.2) + i > 0 && bboxes(4) + j > 0)
                    IFaces(bboxes(3) - floor(Hatx / 3.2) + i, bboxes(4) + j, k) = Hat(i, j, k);
                end
            end
        end
    end
end

figure, imshow(IFaces), title('AddChristmasHats');
I = imread('cameraman.tif');
I = double(I);
B = I;
for i = 1 : 4
    for j = 1 : 3
        B = MyImrotate(B, 30,'bicubic');
    end
        %crop
    [w, h] = size(B);
    w = round(w / 2);
    h = round(h / 2);
    B = B( (w - 135):(w + 135), (h - 135):(h + 135));
end
        %crop
[w, h] = size(B);
w = round(w / 2);
h = round(h / 2);
B = B( (w - 127):(w + 128), (h - 127):(h + 128));

figure(1);imshow(B,[]); 

mse1 = immse(I(5:252,3:5:252),uint8(B(5:252,3:5:252)));

B1 = I;
for i = 1 : 4
    for j = 1 : 3
        B1 = imrotate(B1, 30,'bicubic');
    end
        %crop
    [w, h] = size(B1);
    w = round(w / 2);
    h = round(h / 2);
    B1 = B1( (w - 135):(w + 135), (h - 135):(h + 135));
end
        %crop
[w, h] = size(B1);
w = round(w / 2);
h = round(h / 2);
B1 = B1( (w - 127):(w + 128), (h - 127):(h + 128));

figure(2);imshow(B1,[]);

mse2 = immse(I(5:252,3:5:252),B1(5:252,3:5:252));

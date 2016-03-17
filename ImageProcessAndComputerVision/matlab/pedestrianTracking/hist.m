function Hist = hist(Mimage)

[r,c,O] = size(Mimage);
[h,s,v] = rgb2hsv(Mimage);

H = h; S = s; V = v;
h = h*360;

for i = 1:r
    for j = 1:c
        if h(i,j)<=15||h(i,j)>345
            H(i,j) = 0;
        end
        if h(i,j)<=25&&h(i,j)>15
            H(i,j) = 1;
        end
        if h(i,j)<=45&&h(i,j)>25
            H(i,j) = 2;
        end
        if h(i,j)<=55&&h(i,j)>45
            H(i,j) = 3;
        end
        if h(i,j)<=80&&h(i,j)>55
            H(i,j) = 4;
        end
        if h(i,j)<=108&&h(i,j)>80
            H(i,j) = 5;
        end
        if h(i,j)<=140&&h(i,j)>108
            H(i,j) = 6;
        end
        if h(i,j)<=165&&h(i,j)>140
            H(i,j) = 7;
        end
        if h(i,j)<=190&&h(i,j)>165
            H(i,j) = 8;
        end
        if h(i,j)<=220&&h(i,j)>190
            H(i,j) = 9;
        end
        if h(i,j)<=255&&h(i,j)>220
            H(i,j) = 10;
        end
        if h(i,j)<=275&&h(i,j)>255
            H(i,j) = 11;
        end
        if h(i,j)<=290&&h(i,j)>275
            H(i,j) = 12;
        end
        if h(i,j)<=316&&h(i,j)>290
            H(i,j) = 13;
        end
        if h(i,j)<=330&&h(i,j)>316
            H(i,j) = 14;
        end
        if h(i,j)<=345&&h(i,j)>330
            H(i,j) = 15;
        end
    end
end
for i = 1:r
    for j = 1:c
        if s(i,j)<=0.15&&s(i,j)>0
            S(i,j) = 0;
        end
        if s(i,j)<=0.4&&s(i,j)>0.15
            S(i,j) = 1;
        end
        if s(i,j)<=0.75&&s(i,j)>0.4
            S(i,j) = 2;
        end
        if s(i,j)<=1&&s(i,j)>0.75
            S(i,j) = 3;
        end
    end
end
for i = 1:r
    for j = 1:c
        if v(i,j)<=0.15&&v(i,j)>0
            V(i,j) = 0;
        end
        if v(i,j)<=0.4&&v(i,j)>0.15
            V(i,j) = 1;
        end
        if v(i,j)<=0.75&&v(i,j)>0.4
            V(i,j) = 2;
        end
        if v(i,j)<=1&&v(i,j)>0.75
            V(i,j) = 3;
        end
    end
end

for  i = 1:r
    for j = 1:c
        M(i,j) = H(i,j)*16+S(i,j)*4+V(i,j);
    end
end

Hist = imhist(uint8(M));
Hist = Hist/(r*c);

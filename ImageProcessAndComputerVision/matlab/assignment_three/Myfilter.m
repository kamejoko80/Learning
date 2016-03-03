function newImg = Myfilter(Img, method)

if (numel(size(Img)) > 2)
    Img = rbg2gray(Img);
end

switch method
    case 'average'
        newImg = uint8(filter2(fspecial('average',5), Img));
    case 'median'
        newImg = medfilt2(Img, [5,5]);
    case 'low pass filter'
        newImg = uint8(real(LPF(Img, 50)));
end
end

function newImg = LPF(oriImg, cutoff_freq)
    s = fftshift(fft2(oriImg));
    [M,N]=size(s);
    n1=floor(M/2);
    n2=floor(N/2);
    h = zeros(M, N);
    
    for i = 1 : M
        for j = 1 : N
            if ((sqrt( (i - n1)^2 + (j - n2)^2)) < cutoff_freq)
                h(i, j) = 1;
            else
                h(i, j) = 0;
            end
        end
    end
    
    s = s .* h;
    newImg = ifft2(ifftshift(s));
end

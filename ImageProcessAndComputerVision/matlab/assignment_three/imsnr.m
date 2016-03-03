function SNR = imsnr(Img, oriImg)

if any(size(Img) ~= size(oriImg))
    error('The input size is not equal to each other!');
end

Img = double(Img);
oriImg = double(oriImg);
D = Img - oriImg;

Noise = sum(sum( D(:,:) .* D(:,:)));
Signal = sum(sum( Img(:,:) .* Img(:,:)));

SNR = Signal / Noise;

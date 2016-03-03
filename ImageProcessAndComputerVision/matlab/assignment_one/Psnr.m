function [PSNR, MSE] = Psnr(X, Y)
%Calculate MSE and PSNR
%if there is no Y, Calculate MSE and PSNR with X and itself
if nargin < 2
    D = X;
else
    if any(size(X) ~= size(Y))
        error('The input size is not equal to each other!');
    end
    D = X - Y;
    if D == 0
        error('The input image is equal to each other!');
    end
end %end nargin < 2

if(max(max(D(:,:,1))) <= 1)
    Max_I = 1;
else Max_I = 255;
end

% MSE = sum(D( : ) .* D( : )) / numel(X);
% PSNR = 10 * log10( 1 ^ 2 / MSE);

MSE1 = sum(sum( D(:,:,1) .* D(:,:,1))) / (numel(X) / 3);
MSE2 = sum(sum( D(:,:,2) .* D(:,:,2))) / (numel(X) / 3);
MSE3 = sum(sum( D(:,:,3) .* D(:,:,3))) / (numel(X) / 3);
MSE = (MSE1 + MSE2 + MSE3) / 3;

PSNR1 = 10 * log10( Max_I ^ 2 / MSE1);
PSNR2 = 10 * log10( Max_I ^ 2 / MSE2);
PSNR3 = 10 * log10( Max_I ^ 2 / MSE3);
PSNR = (PSNR1 + PSNR2 + PSNR3) / 3;

display(MSE);
display(PSNR);
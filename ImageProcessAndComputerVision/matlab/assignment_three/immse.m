function MSE = immse(X, Y)

if any(size(X) ~= size(Y))
    error('The input size is not equal to each other!');
end

X = double(X);
Y = double(Y);

D = X - Y;

if D == 0
    error('The input image is equal to each other!');
end

MSE = sum(sum( D(:,:) .* D(:,:))) / numel(X);
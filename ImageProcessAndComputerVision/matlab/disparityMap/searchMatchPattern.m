function kmatch = searchMatchPattern(I0, I1, i, j, ndisp)

smin = 1000 * (255.^2);
%for k = kmatch:(width - (n - 1) / 2).
%         if (kmatch + searchMax) > (width - (n - 1) / 2)
%              kmatch = (width - (n - 1) / 2) - searchMax;
%         end
%         
%         for k = (kmatch - searchMax):(kmatch + searchMax)
for k = (j - ndisp):j
    a = I0((i - (n - 1) / 2):(i + (n - 1) / 2), (j - (n - 1) / 2):(j + (n - 1) / 2));
    b = I1((i - (n - 1) / 2):(i + (n - 1) / 2), (k - (n - 1) / 2):(k + (n - 1) / 2));
    s = SSD(a, b);
    if (s < smin)
        smin = s;
        kmatch = k;
    end
end


function s = SSD(a, b)
s = sum((a(:) - b(:)) .^ 2);
    
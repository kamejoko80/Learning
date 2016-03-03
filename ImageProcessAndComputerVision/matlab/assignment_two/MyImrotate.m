function B = MyImrotate(I, angle, method)

if (isempty(I))     
        % No rotation needed
	B = I; 
else
	I = double(I); 
	[h, w] = size(I); 
        % Convert to radians
	theta = double(angle) / 180.0 * pi;
	rotate = [cos(theta), sin(theta);
			-sin(theta), cos(theta)];
        % Calculate offsets for rotated image
	offset = w * sin(theta);    
        % Calculate limits for rotated image
	wnew = ceil( w * cos(theta) + h * sin(theta)); 
	hnew = ceil( w * sin(theta) + h * cos(theta));
	B = zeros(hnew, wnew);
	
	for u = 1 : hnew 
		for v = 1 : wnew 
                % Calculate x y in the origin image
			tmp = rotate * [u - offset; v];
			x = tmp(1);
			y = tmp(2);
			if (x <= h) && (y <= w) && (x >= 1) && (y >= 1)
					% get the real point nearest the x y
                
				x_low = floor(x); 
				x_high = ceil(x); 
				y_low = floor(y); 
				y_high = ceil(y); 
                
                if (strcmp(method, 'nearest'))
                    if (x - x_low) <= (x_high - x)
                    	x = x_low; 
                    else
            			x = x_high; 
                    end
            		if  (y - y_low) <= (y_high - y)
            			y = y_low; 
                    else
            			y = y_high; 
            		end    
                    B(u, v) = I(x, y);
                end % end if (strcmp(method, 'nearest'))
                
                if (strcmp(method, 'bilinear'))
                    p1 = I(x_low, y_low); 
                	p2 = I(x_high, y_low); 
                	p3 = I(x_low, y_high); 
                	p4 = I(x_high, y_high);
                		% bilinear method
                	s = x - x_low; 
                	t = y - y_low;
                	B(u,v) = p1 + s * (p2 - p1) + t * (p3 - p1) + s * t * (p4 + p1 - p2 - p3);   
                end % end if (strcmp(method, 'bilinear'))
                
                if (strcmp(method, 'bicubic'))
                    if (x <= h - 1) && (y <= w - 1) && (x >= 2) && (y >= 2)
                    	s = x - x_low; 
                    	t = y - y_low;
                        H = [interpolator(s + 1), interpolator(s + 0), interpolator(s - 1), interpolator(s - 2)];
                        Ht = [interpolator(t + 1);
                            interpolator(t + 0);
                            interpolator(t - 1);
                            interpolator(t - 2)];
                        Near = [I(x_low - 1, y_low - 1), I(x_low - 1, y_low + 0), I(x_low - 1, y_low + 1), I(x_low - 1, y_low + 2);
                            I(x_low + 0, y_low - 1), I(x_low + 0, y_low + 0), I(x_low + 0, y_low + 1), I(x_low + 0, y_low + 2);
                            I(x_low + 1, y_low - 1), I(x_low + 1, y_low + 0), I(x_low + 1, y_low + 1), I(x_low + 1, y_low + 2);
                            I(x_low + 2, y_low - 1), I(x_low + 2, y_low + 0), I(x_low + 2, y_low + 1), I(x_low + 2, y_low + 2)];
                        B(u, v) = H * Near * Ht;
                    end
                end % end if (strcmp(method, 'bicubic'))
                
			end % end if (x < h + 1) && (y < w + 1) 
		end
	end
end

%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
%  Function: interpolator
%  An interpolator can be obtained by applying a convolution 
%  with the following kernel in both dimensions:

function W = interpolator(x)
a = - 0.5;
if ((0 <= abs(x)) && (abs(x) < 1))
    W = (a + 2) * abs(x)^3 - (a + 3) * abs(x)^2 + 1;
end

if ((1 <= abs(x)) && (abs(x) < 2))
    W = a * abs(x)^3 - 5 * a * abs(x)^2 + 8 * a * abs(x) - 4 * a;
end

if (abs(x) >= 2)
    W = 0;
end
% if(x == 0)
%     W = 1;
% else
%     W = sin(x) / x;
% end



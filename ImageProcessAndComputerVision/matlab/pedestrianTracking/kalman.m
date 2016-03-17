function [P_new, pre_obj] = kalman(last_obj, cur_obj, last_num, cur_num, P)

%kalman filter initialization
A = [1 0 1 0;
    0 1 0 1;
    0 0 1 0;
    0 0 0 1];

H = [1 0 0 0;
    0 1 0 0];
Q= 1*eye(4);
R=1*eye(2);

P = A*P*A'+Q;
kf_gain = P*H'/(H*P*H'+R);

%kalman prediction
for i=1:last_num
    x = [last_obj(i).X,last_obj(i).Y,last_obj(i).Vx,last_obj(i).Vy]';
    x = A*x;
    last_obj(i).X = x(1);
    last_obj(i).Y = x(2);
end

% match
temp = (1:cur_num);
for i = 1:last_num
    for j = 1:cur_num
        value = imsimilar(last_obj(i).hist,cur_obj(j).hist,2);
        d = (min(last_obj(i).X,cur_obj(j).X)/max(last_obj(i).X,cur_obj(j).X))...
            *(min(last_obj(i).Y,cur_obj(j).Y)/max(last_obj(i).Y,cur_obj(j).Y));

        thr = d*value*100;
        if thr>=45
            last_obj(i).behind_match = j;
            index = find(temp == last_obj(i).No);
            temp(index) = cur_obj(j).No;
            temp(j) = last_obj(i).No;
        else
            if value>=0.7
                last_obj(i).behind_match = j;
                index = find(temp == last_obj(i).No);
                temp(index) = cur_obj(j).No;
                temp(j) = last_obj(i).No;
            end
        end
    end
end
for i=1:cur_num
    cur_obj(i).No = temp(i);
end

%kalman update
pre_obj = cur_obj;
for i=1:last_num
    index = last_obj(i).behind_match;
    if index ~= 0
        z = H*[last_obj(i).X,last_obj(i).Y,last_obj(i).Vx,last_obj(i).Vy]';
        r = [cur_obj(index).X;cur_obj(index).Y]-z;
        x = [last_obj(i).X,last_obj(i).Y,last_obj(i).Vx,last_obj(i).Vy]'+kf_gain*r;
        pre_obj(index).X = x(1);
        pre_obj(index).Y = x(2);
        pre_obj(index).Vx = x(3);
        pre_obj(index).Vy = x(4);
    end
end
P_new = P - kf_gain*H*P;
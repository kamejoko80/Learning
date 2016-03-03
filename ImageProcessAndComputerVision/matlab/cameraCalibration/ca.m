K = cameraParams.IntrinsicMatrix;
R1 = cameraParams.RotationMatrices(:,:,12);
R2 = cameraParams.RotationMatrices(:,:,14);
T1 = cameraParams.TranslationVectors(12,:);
T2 = cameraParams.TranslationVectors(14,:);

M1 = K'*[R1' T1'];
M2 = K'*[R2' T2'];
M = [M1;
    M2];

point1 = [2668,1980,1]';
point2 = [731,1096,1]';
points = [point1;
        point2];

position = M \ points;
position = position ./ (position(4));
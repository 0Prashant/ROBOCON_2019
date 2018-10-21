function [gX, gY, gZ, gXYZ] = toG( dataTable, offsets, scale )
% Converts the data in axis to g form according to scale

    [X,Y,Z] = getAxes(dataTable);
    
    gX = zeros;
    gY = zeros;
    gZ = zeros;
    gXYZ = zeros;

    for i=1:length(X)
        X(i) = X(i) + offsets(1);
        Y(i) = Y(i) + offsets(2);
        Z(i) = Z(i) + offsets(3);
        gX(i) = (X(i) / 32768) * scale;
        gY(i) = (Y(i) / 32768) * scale;
        gZ(i) = (Z(i) / 32768) * scale;
        gXYZ(i) = sqrt(gX(i)^2 + gY(i)^2 + gZ(i)^2);
    end
end

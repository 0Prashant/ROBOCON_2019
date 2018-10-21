function [ xA, yA, zA, xG, yG, zG ] = ES_all( dataTable, alpha )
% filters the accelerometer and gyroscope values of the
% datatable using exponential smoothing filter.

    [x1,y1,z1,x2,y2,z2] = getAxes(dataTable);
    
    xA = expSmooth(x1, alpha);
    yA = expSmooth(y1, alpha);
    zA = expSmooth(z1, alpha);
    xG = expSmooth(x2, alpha);
    yG = expSmooth(y2, alpha);
    zG = expSmooth(z2, alpha);

end


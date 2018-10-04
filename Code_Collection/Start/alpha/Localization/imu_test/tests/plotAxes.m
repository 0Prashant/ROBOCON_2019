function plotAxes( dataTable )
% plots the 'xAxis', 'yAxis', 'zAxis' data in 3D

    [xA, yA, zA, xG, yG, zG] = getAxes(dataTable);
    plot3(xA, yA, zA);
    hold on;
    plot3(xG, yG, zG);
    hold off;

end

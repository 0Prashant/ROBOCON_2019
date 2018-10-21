function [xA, yA, zA, xG, yG, zG] = getAxes( dataTable )
% gives array of axes data from the table
% It is necessary that the table have 'xAxis', 'yAxis'
% and 'zAxis' fields

    xA = dataTable{:,{'xA'}};
    yA = dataTable{:,{'yA'}};
    zA = dataTable{:,{'zA'}}; 
    xG = dataTable{:,{'xG'}};
    yG = dataTable{:,{'yG'}};
    zG = dataTable{:,{'zG'}};

end


ori1 = CAlpha1;     % Expected: x = 0, y = 0, z = 1
ori2 = CAlpha2;     % Expected: x = 1, y = 0, z = 0
ori3 = CAlpha3;     % Expected: x = 0, y = -1, z = 0
ori4 = CAlpha4;     % Expected: x = 0, y = 0, z = -1
ori5 = CAlpha5;     % Expected: x = -1, y = 0, z = 0
ori6 = CAlpha6;     % Expected: x = 0, y = 1, z = 0

% calculates the offset values for each axes as per the
% orientation provided.

[ori1_x, ori1_y, ori1_z] = getAxes(ori1);
[ori2_x, ori2_y, ori2_z] = getAxes(ori2);
[ori3_x, ori3_y, ori3_z] = getAxes(ori3);
[ori4_x, ori4_y, ori4_z] = getAxes(ori4);
[ori5_x, ori5_y, ori5_z] = getAxes(ori5);
[ori6_x, ori6_y, ori6_z] = getAxes(ori6);

orix = [ori1_x; ori2_x; ori3_x; ori4_x; ori5_x; ori6_x];
oriy = [ori1_y; ori2_y; ori3_y; ori4_y; ori5_y; ori6_y];
oriz = [ori1_z; ori2_z; ori3_z; ori4_z; ori5_z; ori6_z];

maxXs = zeros;
maxYs = zeros;
maxZs = zeros;
minXs = zeros;
minYs = zeros;
minZs = zeros;

for i = 1 : length(orix)
    maxXs(i) = max(orix(i));
    maxYs(i) = max(oriy(i));
    maxZs(i) = max(oriz(i));
    minXs(i) = min(orix(i));
    minYs(i) = min(oriy(i));
    minZs(i) = min(oriz(i));
end

maxX = max(maxXs);
maxY = max(maxYs);
maxZ = max(maxZs);
minX = min(minXs);
minY = min(minYs);
minZ = min(minZs);

offset_x = (maxX + minX)/2;
offset_y = (maxY + minY)/2;
offset_z = (maxZ + minZ)/2;

clearvars -except offset_x offset_y offset_z

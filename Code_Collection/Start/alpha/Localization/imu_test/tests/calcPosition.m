% 1. Calibrate the accelerometer data.
% 2. Calculate position and velocity as:
%     u = 0
%     s = ut + (1/2)at^2
%     u += at
% 3. Repeat step 2 using newly obtained u

dataTable = MRalpha;
distance = zeros;
u(1,1) = zeros;
s = zeros;

[xA, yA, zA] = toG(dataTable, [0,0,0], 2);
delta_t = 0.05;

for i = 1 : length(xA)
    u(i+1,1) = u(i,1) + xA * delta_t;
end

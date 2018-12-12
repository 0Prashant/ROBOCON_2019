% Moving Average for 200 points between 35 and 37 with window size = 4

window = 4;
mask = ones(1,window)/window;
ma_w_m1 = conv(w_m1,mask,'same');
hold on;
plot(w_m1);
plot(ma_w_m1);
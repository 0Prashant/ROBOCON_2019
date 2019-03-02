omega_m0 = -1 * omegacytron1{':',{'omega_m0'}};
omega_m1 = -1 * omegacytron1{':',{'omega_m1'}}

omega_m0 = omega_m0/100;
omega_m1 = omega_m1/100;

figure
plot(omega_m0);
title('Motor_0');
grid on

figure
plot(omega_m1);
title('Motor_1');

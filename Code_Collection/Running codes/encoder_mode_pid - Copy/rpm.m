% dutycycle to rpm
function RPM = rpm(duty_cycle, max_rpm)
    max_rpm = 360.5767;
    RPM     = (duty_cycle*max_rpm)/65535; 









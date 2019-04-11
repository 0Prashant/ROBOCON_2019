#include "motor.h"
static uint16_t time_period(uint16_t PWM_frequency)
{
        if (PWM_frequency < 2000) { //MIN PWM_FREQUENCY = 1281.738 for time period to be 16 bit
                _Error_Handler(__FILE__, __LINE__);
        }
        return ((PWM_TIMER_FREQUENCY / 2) / PWM_frequency) - 1; // In Center aligned mode period doubles hence we divide by 2
}

/* SET DUTYCYCLE primary function */
void set_DutyCycle_Primary(TIM_HandleTypeDef *htim, uint32_t Channel, uint16_t dutyCycle)
{
        uint16_t mapped_value;
        mapped_value = (time_period(MOTOR_DRIVER_FREQUENCY) * dutyCycle) / 65535;
        __HAL_TIM_SET_COMPARE(htim, Channel, mapped_value);
}

static void set_DutyCycle(motor_config *motor, uint16_t dutyCycle)
{
        set_DutyCycle_Primary(motor->htim, motor->channel, dutyCycle);
}

static void set_MotorDirection(motor_config *motor, Direction d)
{
        if (d == Direction::CLOCKWISE)       // CLOCKWISE
        {
                HAL_GPIO_WritePin(motor->in1_port, motor->in1_pin, GPIO_PIN_SET);
                HAL_GPIO_WritePin(motor->in2_port, motor->in2_pin, GPIO_PIN_RESET);
        }
        else if (d == Direction::ANTICLOCKWISE)  // ANTICLOCKWISE
        {
                HAL_GPIO_WritePin(motor->in1_port, motor->in1_pin, GPIO_PIN_RESET);
                HAL_GPIO_WritePin(motor->in2_port, motor->in2_pin, GPIO_PIN_SET);
        }
        else                    // STOP
        {
                HAL_GPIO_WritePin(motor->in1_port, motor->in1_pin, GPIO_PIN_RESET);
                HAL_GPIO_WritePin(motor->in2_port, motor->in2_pin, GPIO_PIN_RESET);
        }
}

static void set_WheelOmega(motor_config *motor, float omega)
{
        uint16_t new_omega = (65535.0 / motor->max_omega) * (omega);
        set_DutyCycle(motor, new_omega);
}



/**
 * @brief Function that sets the angular velocity of the wheel
 * @param omega The new angular of the wheel
 * @retval None
 * 
 * @note This function <b>does not</b> actually update the value of the wheel.
 *       The update() function need to be called separately for new value to be
 *       updated.
 * 
 * 
 * <pre>
 * Tasks performed by this function :
 * 1) Sets the direction of the wheel according to the sign of omega
 * 2) Sets the omega of the wheel
 * </pre>
 */
void motor::set_Omega(float omega)
{
        if (omega < 0) {
                dir_ = Direction::BACK;
        }
        else if (omega > 0) {
                dir_ = Direction::AHEAD;
        }
        else {
                dir_ = Direction::HALT;
                omega_ = 0;
        }
        omega_ = fabsf(omega);
}

void motor::update() const
{
        if (omega_ < 0) {
                _Error_Handler(__FILE__, __LINE__);
        }
        set_WheelDirection(wheel_, dir_);
        set_WheelOmega(wheel_, omega_);
}


// float Wheel::get_Omega(uint32_t dt_millis)
// {
//         float dt = (float)dt_millis / 1000.0;
//         // Get count and reset the counter
//         int16_t counts = wheel_->henc->Instance->CNT;
//         wheel_->henc->Instance->CNT = 0;

//         // w = 2*pi*f
//         // f = (counts / ppr) / dt;
//         // Nagating to make count compatible with earlier version
//         float omega = 2 * PI * counts / (dt*ppr);

//         return omega;
// }
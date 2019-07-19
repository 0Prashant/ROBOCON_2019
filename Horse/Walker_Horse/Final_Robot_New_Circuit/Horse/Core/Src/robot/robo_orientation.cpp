#include "smoothing_algorithm.h"
#include "kalman.h"
#include "mpu6050.h"
#include "hmc5883.h"

int init_OriFilters(uint32_t dt_millis);


struct MPU6050 Body_IMU;
struct HMC5883 Body_HMC;

static Vec3<float> gOmega_Bias;
static bool FIRST_ANGLE_FLAG = true;
static float first_yaw = 0;

static Exp_Smooth gXAccelAlpha35(0.035);
static Exp_Smooth gYAccelAlpha35(0.035);
static Exp_Smooth gZAccelAlpha35(0.035);

static Exp_Smooth gXGyroAlpha35(0.035);
static Exp_Smooth gYGyroAlpha35(0.035);
static Exp_Smooth gZGyroAlpha35(0.035);

static Exp_Smooth gXMagAlpha35(0.035);
static Exp_Smooth gYMagAlpha35(0.035);
static Exp_Smooth gZMagAlpha35(0.035);

static Kalman_Vars gAccel_Gyro;

static Kalman_Filter gRoll_Filter(&gAccel_Gyro, init_OriFilters);
static Kalman_Filter gPitch_Filter(&gAccel_Gyro, init_OriFilters);
static Kalman_Filter gYaw_Filter(&gAccel_Gyro, init_OriFilters);

static void IMU_Init()
{
        Body_IMU.hi2c = &hi2c1;
        Body_IMU.address = 0xD0;
        Body_IMU.a_scale = Accel_Scale::SCALE_2G;
        Body_IMU.g_scale = Gyro_Scale::SCALE_250;

        Body_HMC.hi2c = &hi2c1;
        Body_HMC.address = 0x3C;
        // Body_HMC.hard_iron_offset.set_Values(43, 45, 0);

        MPU6050_Init(&Body_IMU);
        HMC5883_Init(&Body_HMC);
}

int Angle_Init()
{
	IMU_Init();

	gOmega_Bias = MPU6050_Calc_OmegaBias(&Body_IMU, 1000);

	return 0;
}

int init_OriFilters(uint32_t dt_millis)
{

        float dt = (float)dt_millis / 1000.0;

        // For Orientation Purpose
        Mat state_model(2,2);
        state_model.at(0,0) = 1;
        state_model.at(0,1) = -dt;
        state_model.at(1,0) = 0;
        state_model.at(1,1) = 1;

        Mat control_model(2,1);
        control_model.at(0,0) = dt;
        control_model.at(1,0) = 0;

        Mat obs_model(1,2);
        obs_model.at(0,0) = 1;
        obs_model.at(0,1) = 0;

        Mat priori_error(2,2);
        priori_error.at(0,0) = 10;
        priori_error.at(0,1) = 0;
        priori_error.at(1,0) = 0;
        priori_error.at(1,1) = 10;

        Mat process_error(2,2);
        process_error.at(0,0) = 0.001;
        process_error.at(0,1) = 0;
        process_error.at(1,0) = 0;
        process_error.at(1,1) = 0.003;
        
        Mat measure_error(1,1);
        measure_error.at(0,0) = 0.04;

        gAccel_Gyro.set_F(state_model);
        gAccel_Gyro.set_B(control_model);
        gAccel_Gyro.set_H(obs_model);
        gAccel_Gyro.set_I(2);
        gAccel_Gyro.set_P(priori_error);
        gAccel_Gyro.set_Q(process_error);
        gAccel_Gyro.set_R(measure_error);

        return 0;
}

/**
 * @brief Function that provides the orientation of the robot
 * @param base_state This parameter is considered as one of the possible state
 *                   of robot.
 * @param dt_millis : The time period at which this function is called periodically
 * 
 * @retval orientation of the robot in degrees
 * 
 * 
 * <pre>
 * Tasks performed by this function
 * 1) Read the values from Accelerometer and the Gyroscope
 * </pre>
 */

Vec3<float> read_Orientation(uint32_t dt_millis)
{
        Vec3<float> accel;
        Vec3<float> gyro;
        Vec3<float> mag;
	/*
		red_start => -15.2, 13.9, 171.38
		red_mid => -16.9, 12.21. 129.24
		blue_start => -39.5, 15.4, 80
		blue_mid => -18.1, 10.45, 76.81
	 */
	Vec3<float> mag_offset(81,-36,-150.1);	// Directly add the offset here 
	//! Don't confuse with -ve sign 	
        Vec3<float> angles;

#ifdef _ENABLE_I2C_ERROR_DETECTION

        bool mpu_is_ready(true), hmc_is_ready(true);
        
        // Read MPU6050 if it is ready to be read
        if (MPU6050_Read_NormAxes(&Body_IMU) != HAL_OK) {
                mpu_is_ready = false;
        }

        // Read HMC5883 if it is ready to be read
        if (HMC5883_Read(&Body_HMC) != HAL_OK) {
                hmc_is_ready = false;
        }

        if (!mpu_is_ready || !hmc_is_ready) {
                //! Error Condition
		HAL_GPIO_WritePin(GPIOD, GPIO_PIN_14, GPIO_PIN_SET);
                error(Error::PERIPHERAL_ERROR);
                IMU_Init();
                return angles;
        }
        else {
		HAL_GPIO_WritePin(GPIOD, GPIO_PIN_14, GPIO_PIN_RESET);
        }
#else
        MPU6050_Read_NormAxes(&Body_IMU);
        HMC5883_Read(&Body_HMC);
#endif  // _ENABLE_I2C_ERROR_DETECTION

        accel = Body_IMU.norm_a_axis;
        gyro = Body_IMU.norm_g_axis - gOmega_Bias;
        mag = Body_HMC.raw_axis;
	mag -= mag_offset;
        // (accel.mult_EW(1000)).print();
        // printf("    ");
        // (gyro.mult_EW(4)).print();
        // printf("    ");
        //  mag.print();
        //  printf("\n");

        // We will consider the rotation in order: psi-theta-phi
        float ax = gXAccelAlpha35.smooth(accel.getX());
        float ay = gYAccelAlpha35.smooth(accel.getY());
        float az = gZAccelAlpha35.smooth(accel.getZ());

        float gx = gXGyroAlpha35.smooth(gyro.getX());
        float gy = gYGyroAlpha35.smooth(gyro.getY());
        float gz = gZGyroAlpha35.smooth(gyro.getZ());

        float bx = gXMagAlpha35.smooth(mag.getX());
        float by = gYMagAlpha35.smooth(mag.getY());
        float bz = gZMagAlpha35.smooth(mag.getZ());

	// printf("%ld %ld %ld\n", (int32_t)(bx), (int32_t)(by), (int32_t)(bz));
        
        float roll = atan2f(ay, az) * 57.3;

        roll = gRoll_Filter.filter(roll, gx, dt_millis);
        // printf("%ld,   ", (int32_t)(roll*1000));

        float sin_roll = sin(roll / 57.3);
        float cos_roll = cos(roll / 57.3);
        // Roll Compensated Pitch
        float pitch = atan2f((-ax), ay*sin_roll + az*cos_roll) * 57.3;

        pitch = gPitch_Filter.filter(pitch, gy, dt_millis);
        // printf("%ld,   ", (int32_t)(pitch*1000));

        float sin_pitch = sin(pitch / 57.3);
        float cos_pitch = cos(pitch / 57.3);
        // Tilt Compensated Yaw
	
	if(FIRST_ANGLE_FLAG){
		first_yaw = atan2f((bz*sin_roll - by*cos_roll), (bx*cos_pitch + by*sin_roll*sin_pitch + bz*cos_roll*sin_pitch)) * 57.3;
		FIRST_ANGLE_FLAG = false;
	}
        float yaw = atan2f((bz*sin_roll - by*cos_roll), (bx*cos_pitch + by*sin_roll*sin_pitch + bz*cos_roll*sin_pitch)) * 57.3;
	if(((first_yaw>=90) &&(first_yaw<=180))  ||  ((first_yaw>= -180) &&(first_yaw<= -90))){
		if(yaw<0){
			yaw += 360;
		}
	}
	else{

	}
	//yaw = yaw - first_yaw;
	
        // printf("%ld\n", (int32_t)(yaw*1000));

        yaw = gYaw_Filter.filter(yaw, gz, dt_millis);
        // printf("%ld\n", (int32_t)(yaw*1000));

        angles.set_Values(roll, pitch, yaw);

        return angles;
}
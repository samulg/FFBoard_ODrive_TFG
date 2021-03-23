/*
 * Odrive.h
 *
 *  Created on: Mar 17, 2021
 *      Author: Samullg
 */

#ifndef ODRIVE_H_
#define ODRIVE_H_

#include <MotorDriver.h>
#include <Encoder.h>
#include "cppmain.h"
#include "CommandHandler.h"
#include "uarthandler.h"
#include "target_constants.h"
#include <string>

using namespace std;

enum AxisState  { AXIS_STATE_UNDEFINED = 0,
		AXIS_STATE_IDLE  =1,
		AXIS_STATE_STARTUP_SEQUENCE = 2,
		AXIS_STATE_FULL_CALIBRATION_SEQUENCE = 3,
		AXIS_STATE_MOTOR_CALIBRATION = 4,
		AXIS_STATE_SENSORLESS_CONTROL =5,
		AXIS_STATE_ENCODER_INDEX_SEARCH =6,
		AXIS_STATE_ENCODER_OFFSET_CALIBRATION=7,
		AXIS_STATE_CLOSED_LOOP_CONTROL=8,
		AXIS_STATE_LOCKIN_SPIN=9,
		AXIS_STATE_ENCODER_DIR_FIND=10,
		AXIS_STATE_HOMING = 11
};

enum MotorType  {MOTOR_TYPE_HIGH_CURRENT =0,
		MOTOR_TYPE_GYMBAL =2,
		MOTOR_TYPE_ACIM=3
};

enum ControlMode  {CONTROL_MODE_VOLTAGE_CONTROL=0,
		CONTROL_MODE_TORQUE_CONTROL=1,
		CONTROL_MODE_VELOCITY_CONTROL=2,
		CONTROL_MODE_POSITION_CONTROL=3
};

enum InputMode  {INPUT_MODE_INACTIVE =0,
		INPUT_MODE_PASSTHROUGH=1,
		INPUT_MODE_VEL_RAMP=2,
		INPUT_MODE_POS_FILTER=3,
		INPUT_MODE_MIX_CHANNELS=4,
		INPUT_MODE_TRAP_TRAJ=5,
		INPUT_MODE_TORQUE_RAMP=6,
		INPUT_MODE_MIRROR=7
};


class Odrive: public MotorDriver,public Encoder,public CommandHandler{
public:
	Odrive();
	virtual ~Odrive();

	static ClassIdentifier info;
	const ClassIdentifier getInfo();


	void turn(int16_t power);
	void stop();
	void start();

/*
	void setMode(ModePWM_DRV mode);
	ModePWM_DRV getMode();

	void saveFlash(); 		// Write to flash here
	void restoreFlash();	// Load from flash

	ParseStatus command(ParsedCommand* cmd,std::string* reply);
	virtual std::string getHelpstring(){return "PWM: pwm_mode,pwm_speed\n";}*/



private:


	float current_lim= 20; //[V] Volts because of motor_type
	float vel_lim= 3; //[turns/s]
	float calibration_current = 10; //[V] Volts because of motor_type
	float brake_resistance =2;// [Ohm]
	float pole_pairs = 4;
	float torque_constant = 1.58; // [A/Nm]
	MotorType motor_type = MOTOR_TYPE_GYMBAL;
	int cpr = 40000;// [CPR]

	AxisState axis_state=AXIS_STATE_IDLE;


	void setAxisState(AxisState state);
	AxisState getAxisState();

	void setControlMode (ControlMode mode);
	ControlMode getControlMode ();

	void setParam (string param, int value);
	void setParam (string param, float value);
	float getParam (string param);

	void getFeedback (float* pos, float* vel);

	void setTorque (float torque);

	UART_HandleTypeDef *huart;

	bool active = false;


};


#endif /* ODRIVE_H_ */

/*
 * Odrive.cpp
 *
 *  Created on: Mar 17, 2021
 *      Author: Samullg
 */

#include <Odrive.h>
#ifdef ODRIVE

/*
 * Contains motor drivers based on Odrive
 */
ClassIdentifier Odrive::info = {
		 .name = "Odrive" ,
		 .id=5
 };
const ClassIdentifier Odrive::getInfo(){
	return info;
}

Odrive::Odrive()
{

}
Odrive::~Odrive()
{

void Odrive::turn(int16_t power)
{

}
void Odrive::stop()
{
	this->setAxisState(AXIS_STATE_IDLE);

}
void Odrive::start()
{
	this->setParam("axis0.motor.config.current_lim", this->current_lim);
	this->setParam("axis0.controller.config.vel_limit", this->vel_limit);
	this->setParam("axis0.motor.config.calibration_current", this->calibration_current);
	this->setParam("config.brake_resistance", this->brake_resistance);
	this->setParam("axis0.motor.config.pole_pairs", this->pole_pairs);
	this->setParam("axis0.motor.config.torque_constant", this->torque_constant);
	this->setParam("axis0.motor.config.motor_type", this->motor_type);
	this->setParam("axis0.encoder.config.cpr", this->cpr);

	this->setAxisState(AXIS_STATE_FULL_CALIBRATION_SEQUENCE);

	while(this->getAxisState()!= AXIS_STATE_IDLE)
	{
		HAL_Delay(50);
	}

	this->setAxisState(AXIS_STATE_CLOSED_LOOP_CONTROL);

}

void  Odrive::setAxisState(OdrvAxisState state)
{
	+	this->setParam("axis0.requested_state", state);
}
AxisState Odrive::getAxisState()
{
	return (AxisState) this->getParam("axis0.current_state");
}

void  Odrive::setParam (string param, int value)
{
	stringstream out;
	out<< "w " << param << " " << value  << endl;
	string out_temp=out.str();
	HAL_UART_Transmit(huart,  (uint8_t *) out_temp.c_str(), strlen(out), 20);
}
void  Odrive::setParam (string param, float value)
{
	stringstream out;
	out<< "w " << param << " " << value << endl;
}
float  Odrive::getParam (string param)
{
	stringstream out;
	out<< "r " << param  << endl;
	HAL_UART_Transmit(huart, (uint8_t *) out, strlen(out), 20);
	string in;
	HAL_UART_Receive(huart, (uint8_t *)in, 128, 500);
	return (float)in;

}

void Odrive::getFeedback (float* pos, float* vel)
{
	stringstream out;
	out<<"f 0";
	HAL_UART_Transmit(huart, (uint8_t *) out, strlen(out), 20);
	string in;
	HAL_UART_Receive(huart, (uint8_t *)in, 128, 500);
	sscanf(in,"%f %f", pos, vel);
}

void Odrive::setTorque (float torque)
{
	stringstream out;
	out<< "c 0 " << torque << endl;
	HAL_UART_Transmit(huart, (uint8_t *) out, strlen(out), 20);
}

void Odrive::setControlMode (OdrvControlMode mode)
{
	this->setParam ("axis0.controller.config.control_mode", mode);
}

OdrvControlMode Odrive::getControlMode ()
{
	return (ControlMode) this->getParam("axis0.controller.config.control_mode");
}

#endif

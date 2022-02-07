/*
 * Ams5812_i2c.h
 *
 * Created: 24-Dec-21 2:56:45 PM
 *  Author: AGVA
 */ 


#ifndef AMS5812_I2C_H_
#define AMS5812_I2C_H_
//Pmax  and  Pmin 
// const float AMS5812_0000_D_P_MIN = 0.0f;
// const float AMS5812_0000_D_P_MAX = 0.075f;
// const float AMS5812_0001_D_P_MIN = 0.0f;
// const float AMS5812_0001_D_P_MAX = 0.15f;
// const float AMS5812_0000_D_B_P_MIN = -0.075f;
// const float AMS5812_0000_D_B_P_MAX = 0.075f;
// const float AMS5812_0001_D_B_P_MIN = -0.15f;
// const float AMS5812_0001_D_B_P_MAX = 0.15f;
// const float AMS5812_0003_D_P_MIN = 0.0f;
// const float AMS5812_0003_D_P_MAX = 0.3f;
// const float AMS5812_0008_D_P_MIN = 0.0f;
// const float AMS5812_0008_D_P_MAX = 0.8f;
// const float AMS5812_0015_D_P_MIN = 0.0f;
// const float AMS5812_0015_D_P_MAX = 1.5f;
// const float AMS5812_0003_D_B_P_MIN = -0.3f;
// const float AMS5812_0003_D_B_P_MAX = 0.3f;
// const float AMS5812_0008_D_B_P_MIN = -0.8f;
// const float AMS5812_0008_D_B_P_MAX = 0.8f;
// const float AMS5812_0015_D_B_P_MIN = -1.5f;
// const float AMS5812_0015_D_B_P_MAX = 1.5f;
// const float AMS5812_0030_D_P_MIN = 0.0f;
// const float AMS5812_0030_D_P_MAX = 3.0f;
// const float AMS5812_0050_D_P_MIN = 0.0f;
// const float AMS5812_0050_D_P_MAX = 5.0f;
// const float AMS5812_0150_D_P_MIN = 0.0f;
// const float AMS5812_0150_D_P_MAX = 15.0f;
// const float AMS5812_0300_D_P_MIN = 0.0f;
// const float AMS5812_0300_D_P_MAX = 30.0f;
// const float AMS5812_0600_D_P_MIN = 0.0f;
// const float AMS5812_0600_D_P_MAX = 60.0f;
const float AMS5812_1000_D_P_MIN = 0.0f;
const float AMS5812_1000_D_P_MAX = 100.0f;
// const float AMS5812_0030_D_B_P_MIN = -3.0f;
// const float AMS5812_0030_D_B_P_MAX = 3.0f;
// const float AMS5812_0050_D_B_P_MIN = -5.0f;
// const float AMS5812_0050_D_B_P_MAX = 5.0f;
// const float AMS5812_0150_D_B_P_MIN = -15.0f;
// const float AMS5812_0150_D_B_P_MAX = 15.0f;
// const float AMS5812_0150_B_P_MIN = 11.0f;
// const float AMS5812_0150_B_P_MAX = 17.5f;
// const float AMS5812_0150_A_P_MIN = 0.0f;
// const float AMS5812_0150_A_P_MAX = 15.0f;
// const float AMS5812_0300_A_P_MIN = 0.0f;
// const float AMS5812_0300_A_P_MAX = 30.0f; // digital output at minimum pressure
 const int _digOutPmin = 3277;
 // digital output at maximum pressure
 const int _digOutPmax = 29491;  //Sensor address   #define Sensor_Address 0x78  #include "I2C_0_AVR128DA64.h"  void Sensor_init(void);  //Initialize pressure Sensor float pressureRead(void); //Read pressure data   void Sensor_init(void) {	 I2C_0_initialization();
	 I2C_0_master_initialization();
	 I2C_0_sendAddress(Sensor_Address,1);  //1 for read } float pressureRead(void)
{
	uint16_t data = I2C_0_recieveData(); //Reading first byte
	
	data = (data)&(0x7F);  //set 8th bit zero
	
	data <<= 8;                      //first byte shifted 8 bit
	
	data |= I2C_0_recieveData();     //Reading second byte
	
	I2C_0_recieveData();   //Reading third byte
	
	I2C_0_recieveData();   //Reading forth byte
	
	float pressure_count = (float)data;
	
	float pressure_in_psi = ((pressure_count-3277)/(26216/AMS5812_1000_D_P_MAX))+AMS5812_1000_D_P_MIN;
	if(pressure_in_psi > AMS5812_1000_D_P_MAX)
	{
		pressure_in_psi = 0.0;
	}
	if(pressure_in_psi <= 0)
	{
		pressure_in_psi = 0.0;
	}
	
	
	return pressure_in_psi;
	
	_delay_us(10);

	
}









#endif /* AMS5812_I2C_H_ */
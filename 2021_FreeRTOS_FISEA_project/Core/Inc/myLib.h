/*
 * myLib.h
 *
 *  Created on: Feb 1, 2021
 *      Author: florentgoutailler
 */

#ifndef INC_MYLIB_H_
#define INC_MYLIB_H_

void I2C_Scan(I2C_HandleTypeDef*,char*);
void Init(I2C_HandleTypeDef*);
void Init_BMP280(I2C_HandleTypeDef*);
void MeasureT(I2C_HandleTypeDef*,double*);
void MeasureT_BMP280(I2C_HandleTypeDef*,double*,long signed int*);
uint8_t MeasureP(I2C_HandleTypeDef*,double*,long signed int*);
void MeasureA(I2C_HandleTypeDef*,double*);
void MeasureG(I2C_HandleTypeDef*,double*);
void MeasureM(I2C_HandleTypeDef*,double*,double*,double*);
void NoiseG(I2C_HandleTypeDef*,double*);
void Average(I2C_HandleTypeDef*,double*);
void CalibG(I2C_HandleTypeDef*,double*);
void CalibM(I2C_HandleTypeDef*,double*,double*);
void Roll(I2C_HandleTypeDef*,double*,double*);
void Pitch(I2C_HandleTypeDef*,double*,double*);
void Yaw(I2C_HandleTypeDef*,double*,double*,double*);
void Yaw2(I2C_HandleTypeDef*,double*,double*,double*,double*);
//void ComplementaryFilter(I2C_HandleTypeDef*,double*,double*);
void GUI(double,double,double,double,double);
void GUI2(double);

#endif /* INC_MYLIB_H_ */

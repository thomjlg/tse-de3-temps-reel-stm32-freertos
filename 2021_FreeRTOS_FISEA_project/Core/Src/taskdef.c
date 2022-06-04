/*
 * taskdef.c
 *
 *  Created on: Jan 20, 2021
 *      Author: florentgoutailler
 */

#include "main.h"
#include "stm32f429i_discovery_lcd.h"
#include "i2c.h"
#include "myLib.h"
#include <stdio.h>
#include <stdlib.h>
#include "taskdef.h"

SemaphoreHandle_t Sem1;
SemaphoreHandle_t Sem2;
QueueHandle_t Queue1;
QueueHandle_t Queue2;
QueueHandle_t Queue3;
QueueHandle_t Queue4;
QueueHandle_t Queue5;

/** @fn void vInitTask(void *pvParameters)
*  @brief tâche d'initialisation, créé les sémaphores et les queues, ainsi que les tâches du programme.
*/
void vInitTask(void *pvParameters){
	//Création des sémaphores
	Sem1 = xSemaphoreCreateMutex();
	if (Sem1 == NULL) {
		printf("Erreur création sémaphore 1 !\r\n");
		exit(1);
	}
	vQueueAddToRegistry(Sem1, "Sem1");

	Sem2 = xSemaphoreCreateMutex();
	if (Sem2 == NULL) {
		printf("Erreur création sémaphore 2 !\r\n");
		exit(1);
	}
	vQueueAddToRegistry(Sem2, "Sem2");


	//Messages Queues
	//Entre les tâches 1a et 2a
	Queue1 = xQueueCreate(1, sizeof(double[6]));
	vQueueAddToRegistry(Queue1, "Queue1");
	Queue2 = xQueueCreate(1, sizeof(double[6]));
	vQueueAddToRegistry(Queue2, "Queue2");
	Queue3 = xQueueCreate(1, sizeof(double[6]));
	vQueueAddToRegistry(Queue3, "Queue3");
	Queue4 = xQueueCreate(1, sizeof(double[6]));
	vQueueAddToRegistry(Queue4, "Queue4");
	Queue5 = xQueueCreate(1, sizeof(double[6]));
	vQueueAddToRegistry(Queue5, "Queue5");

	// Création des tâches
	xTaskCreate(vTask1a, "task 1a", 1000, NULL, 2, NULL);
	xTaskCreate(vTask1b, "task 1b", 1000, NULL, 2, NULL);
	xTaskCreate(vTask2a, "task 2a", 1000, NULL, 3, NULL);
	xTaskCreate(vTask2b, "task 2b", 1000, NULL, 3, NULL);
	xTaskCreate(vTask3, "task 3", 1000, NULL, 1, NULL);
	xTaskCreate(vTask4, "task 4", 1000, NULL, 1, NULL);
	vTaskDelete(NULL);
}


void vTask1a(void *pvParameters){
	while(1) {
		double acc[3];
		double acc_gyro[6];

		double gyro[3];

		//Le sémaphore prend le bus I2C
		xSemaphoreTake(Sem1, portMAX_DELAY);
		//Récupération des valeurs de l'accélération
		MeasureA(&hi2c3, acc);
		//Récupération des valeurs du gyromètre
		MeasureG(&hi2c3, gyro);

		//Le sémaphore rend le bus I2C
		xSemaphoreGive(Sem1);

		//Stockage des données
		acc_gyro[0] = acc[0];
		acc_gyro[1] = acc[1];
		acc_gyro[2] = acc[2];
		acc_gyro[3] = acc[3];
		acc_gyro[4] = acc[4];
		acc_gyro[5] = acc[5];

		xQueueSend(Queue1,&acc_gyro, portMAX_DELAY);
	}
}


void vTask1b(void *pvParameters){
	while(1) {
		double press;
		long signed int Tfine;
		double theta;
		double mag[3];
		double barometer;

		double magnet_bias[3] = {0,0,0};
		double press_mag[4] = {0,0,0,0};
		double scale[3] = {1,1,1};

		//Le sémaphore prend le bus I2C
		xSemaphoreTake(Sem1, portMAX_DELAY);
		//Récupération des valeurs du magnétomètre
		MeasureM(&hi2c3,mag, magnet_bias, scale);
		//Récupération des valeurs du barometre
		MeasureT_BMP280(&hi2c3, &theta, &Tfine );
		MeasureP(&hi2c3, &press, &Tfine);

		//Le sémaphore rend le bus I2C
		xSemaphoreGive(Sem1);

		//Stockage des données
		press_mag[0] = press;
		press_mag[1] = mag[0];
		press_mag[2] = mag[1];
		press_mag[3] = mag[2];

		xQueueSend(Queue2,&press_mag, portMAX_DELAY);
		xQueueSend(Queue3,&barometer, portMAX_DELAY);
	}
}


void vTask2a(void *pvParameters){
	while(1) {
	double acc_gyro[6];
	double press_mag[4];
	double angles[3];

	//On attend la réception des données
	xQueueReceive(Queue1,&acc_gyro, portMAX_DELAY);
	xQueueReceive(Queue2,&press_mag, portMAX_DELAY);

	//Calcul de l'angle de roulis à partir des données récupérées dans vTask1a et vTask1b
	//Calcul de l'angle de tangage à partir des données récupérées dans vTask1a et vTask1b
	//Calcul de l'angle de lacet à partir des données récupérées dans vTask1a et vTask1b

	xQueueSend(Queue4,&angles, portMAX_DELAY);
	}
}


void vTask2b(void *pvParameters){
	while(1) {
	double barometer;
	double altitude;

	//On attend la réception des données
	xQueueReceive(Queue3,&barometer, portMAX_DELAY);

	//Calcul à faire plus tard

	xQueueSend(Queue5, &altitude, portMAX_DELAY);
	}

}



void vTask3(void *pvParameters){
	while(1) {
	double angles[3];
	double altitude;

	xQueueReceive(Queue4, &angles, portMAX_DELAY);
	xQueueReceive(Queue5, &altitude, portMAX_DELAY);

	xSemaphoreTake(Sem2, portMAX_DELAY);

	GUI(angles[0], angles[1], angles[2], altitude, 0);

	xSemaphoreGive(Sem2);
	}

}

void vTask4(void *pvParameters){
	while(1) {
	double angles[3];
	double altitude;
	int i;

	xQueueReceive(Queue4, &angles, portMAX_DELAY);
	xQueueReceive(Queue5, &altitude, portMAX_DELAY);

	xSemaphoreTake(Sem2, portMAX_DELAY);

	for(i = 0; i < 3; i++){
		printf("Angles: %f \r\n", angles[i]);
	}

	printf("altitude %f \r\n : ", altitude);

	xSemaphoreGive(Sem2);
	}
}





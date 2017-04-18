/************************************************************/
/*  (C) 2016 Beijing ARM Accelerator Technology Co., Ltd.   */
/*  Description: Demo for ASC Platform.                     */
/*   Performed an example for getting data form Sensors,    */
/*   then send data to OneNet Cloud by using EDP protocol   */
/*  Author: ss.pan                                          */
/*  Version: 1.01                                           */
/*  Date: 2017-03-28                                        */
/************************************************************/

#include "mbed.h"
#include "wifi_example.h"
#include "driver_mbed_TH02.h"
#include <driver_mbed_HP20x.h>
#include <driver_mbed_KalmanFilter.h>

unsigned char ret = 0;

KalmanFilter p_filter;    //Kalman filter for pressure
KalmanFilter a_filter;    //Kalman filter for altitude

float temperature = -999.999;
float humidity    = -999.999;
float pressure    = -999.999;
float pressure_filtered = -999.999;
float altitude    = -999.999;
float altitude_filtered = -999.999;

void getSensorDat(void);

/**
* @brief  main
**/
int main()
{
    connectInit();
    devLink(DEVICEID,APIKEY);
    wait_ms(50);
    HP20x.begin();   //Init HP20X to get altitude and pressure datas
    wait_ms(100);
    
    while(1) {
        getSensorDat();
        if(!checkSocketStatus()) {
            //sendJsonDat("humidity", humidity);
            sendData("humidity", humidity);
            wait_ms(500);
            //sendJsonDat("temperature", temperature);
            sendData("temperature", temperature);
            wait_ms(500);
            //sendJsonDat("pressure", pressure_filtered);
            sendData("pressure", pressure_filtered);
            wait_ms(500);
            //sendJsonDat("altitude", altitude_filtered);
            sendData("altitude", altitude_filtered);
            wait_ms(500);
        }
    }
}

/**
* @brief  get sensor data
**/
void getSensorDat(void)
{
    int ret;
    temperature = myTH02.ReadTemperature();
    humidity    = myTH02.ReadHumidity();
    wait_ms(100);

    ret = HP20x.isAvailable();
    if(OK_HP20X_DEV == ret) {
        long p_data = HP20x.ReadPressure();
        pressure = p_data/100.0f;
        pressure_filtered = p_filter.Filter(pressure);

        long a_data = HP20x.ReadAltitude();
        altitude = a_data/100.0f;
        altitude_filtered = a_filter.Filter(altitude);
    }
}

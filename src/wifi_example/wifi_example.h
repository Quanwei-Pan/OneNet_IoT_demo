#ifndef WIFI_EXAMPLE_H_H
#define WIFI_EXAMPLE_H_H

#include "mbed.h"
#include "Common.h"
#include "EdpKit.h"
#include "cJSON.h"
#include "ESP8266.h"

#include <time.h>  
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define     DEVICEID   "5116906"
#define     APIKEY     "E75QrwTpTp5hmWCxljc0Seg1KS0="
#define     APNAME     "AnChuangKongJian"
#define     APPASSWD   "jiasuqi@0111"
#define     TCPNUM     0
#define     TCPADD     "jjfaedp.hedevice.com"
#define     TCPPORT    876

#define     MAX_SEND_BUF_LEN  1024

extern      EdpPacket* send_pack;
extern      char send_buf[MAX_SEND_BUF_LEN];
extern      char read_buf[MAX_SEND_BUF_LEN];

extern      void connectInit(void);
extern      void sendJsonDat(char* dataName, float data);
extern      void sendData(char* dataName, float data);
extern      void devLink(const char* devid, const char* auth_key);
extern      int checkSocketStatus(void);


#endif


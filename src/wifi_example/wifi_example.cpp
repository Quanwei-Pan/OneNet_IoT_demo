/************************************************************/
/*  (C) 2016 Beijing ARM Accelerator Technology Co., Ltd.   */
/*  Description: Wifi Client by using Grove_wifi module.    */
/*  Author: ss.pan                                          */
/*  Version: 1.01                                           */
/*  Date: 2017-03-28                                        */
/************************************************************/

#include "wifi_example.h"

ESP8266 wifi(PA_9,PA_10);
Serial pc(SERIAL_TX, SERIAL_RX);

EdpPacket* send_pack;
char send_buf[MAX_SEND_BUF_LEN];

/**
* @brief  initial ESP8266,and connect to wifi ap
**/
void connectInit(void)
{
    bool ret;
    pc.printf("starting......\r\n");
    wifi.reset();
    wait_ms(500);
    ret = wifi.startup(1); //start as client mode
    wait_ms(200);
    if(ret != true) {
        pc.printf("startup failed\r\n");
        ret = true;
    } else {
        pc.printf("set as client mode\r\n");
    }
    wifi.connect(APNAME,APPASSWD); //conenet to a existed wifi ap
    wait_ms(200);
    ret = wifi.isConnected(); //check whether connect successed
    if(ret != true) {
        pc.printf("connect failed\r\n");
        ret = true;
    } else {
        pc.printf("now connected!\r\n");
    }
    pc.printf("the ip address is : %s\r\n",wifi.getIPAddress());  //send ip address to pc
    ret = wifi.open("TCP", TCPNUM, TCPADD, TCPPORT);   //connte to a TCP socket
    wait_ms(200);
    checkSocketStatus();
}

/**
* @brief  checek socket status by calling a rebuilded function ESP8266::isOpen()
**/
int checkSocketStatus(void)
{
    int res;
    res = wifi.isOpen();
    switch(res) {
        case  0: {
            pc.printf("socket conneceted successfully\r\n");
            return res;
        }
        case  -1: {
            pc.printf("unknown error\r\n");
            return res;
        }
        case  -2: {
            pc.printf("connected to ap, get an ip address\r\n");
            return res;
        }
        case  -4: {
            pc.printf("lost connection\r\n");
            return res;
        }
        case  -5: {
            pc.printf("unconnected to ap\r\n");
            return res;
        }
        default: {
            pc.printf("unknown error:%d\r\n",res);
            break;
        }
    }
    return res;
}


/**
* @brief  send cJSON type data
**/
void sendJsonDat(char* dataName, float data)
{
    bool ret;
    cJSON *json_data = cJSON_CreateObject();  //create a new json data
    cJSON_AddNumberToObject(json_data, dataName, data); //pack data into json package
    
    send_pack = PacketSavedataJson(DEVICEID, json_data, 3);  //pack send data into EDP package
    pc.printf("now sending JSON from data to OneNet\r\n");
    ret = wifi.send(0,(const char*)send_pack->_data,send_pack->_write_pos);   //send packge to OneNet Cloud
    if(ret != true) {   //check whether send successfully
        pc.printf("failed to  send Json data\r\n");
        ret = true;
        DeleteBuffer(&send_pack);   //delete send_packge, unless may cause memory leak
    } else {
        pc.printf("Json data sended\r\n");
        DeleteBuffer(&send_pack);  //delete send_packge, unless may cause memory leak
    }
    cJSON_Delete(json_data);   //delete json_data, unless may cause memory leak
    wait_ms(500);
}

/**
* @brief  send data to OneNet Cloud
**/
void sendData(char* dataName, float data)
{
    char text[25] = {0};
    bool ret;
    memset(send_buf,0,MAX_SEND_BUF_LEN);  //clean send_buf to ensure no remained buffer

    strcat(send_buf, ",;");
    strcat(send_buf, dataName);
    strcat(send_buf, ",");
    sprintf(text,"%8.2f",data);
    strcat(send_buf, text);
    strcat(send_buf, ";");
           
    pc.printf("now sending NORMAL data to OneNet\r\n");
    send_pack = PacketSavedataSimpleString(DEVICEID, send_buf);   //pack data into EDP Package
    ret = wifi.send(0,(const char*)send_pack->_data,send_pack->_write_pos);   //send packge to OneNet Cloud
    if(ret != true) {   //check whether send successfully
        pc.printf("failed to  send  normal data\r\n");
        ret = true;
        DeleteBuffer(&send_pack);   //delete send_packge, unless may cause memory leak
    } else {
        pc.printf("normal data sended\r\n");
        DeleteBuffer(&send_pack);  //delete send_packge, unless may cause memory leak
    }
    wait_ms(500);
}

/**
* @brief make the device link to OneNet Cloud
**/
void devLink(const char* devid, const char* auth_key)
{
    bool ret;
    pc.printf("now linking to OneNet...\r\n");
    send_pack = PacketConnect1(devid,auth_key);  //pack Device authentication information into EDP package
    wait_ms(200);
    ret = wifi.send(0,(const char*)send_pack->_data,send_pack->_write_pos);   //send packge to OneNet Cloud
    wait_ms(500);
    if(ret != true) {  //check whether send successfully
        pc.printf("failed to link to OneNet\r\n");
        ret = true;
        DeleteBuffer(&send_pack);  //delete send_packge, unless may cause memory leak
    } else {
        pc.printf("linked to OneNet\r\n");
        DeleteBuffer(&send_pack);  //delete send_packge, unless may cause memory leak
    }
}

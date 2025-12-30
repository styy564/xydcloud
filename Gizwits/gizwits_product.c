/**
************************************************************
* @file         gizwits_product.c
* @brief        Gizwits control protocol processing, and platform-related hardware initialization 
* @author       Gizwits
* @date         2017-07-19
* @version      V03030000
* @copyright    Gizwits
*
* @note         Gizwits is only for smart hardware
*               Gizwits Smart Cloud for Smart Products
*               Links | Value Added | Open | Neutral | Safety | Own | Free | Ecology
*               www.gizwits.com
*
***********************************************************/
#include <stdio.h>
#include <string.h>
#include "gizwits_product.h"
#include "led.h"
#include "relay.h"
#include "motor.h"
#include "rgb.h"
#include "step_motor.h"
#include "dht11.h"

// 外部变量声明
extern u8 step_flag;
extern u8 motor_speed;
extern RGB rgb;
extern u8 oled_ref;
extern float dht_temp;
extern float dht_humi;

static uint32_t timerMsCount;

/** Current datapoint */
dataPoint_t currentDataPoint;

/**@} */
/**@name Gizwits User Interface
* @{
*/

/**
* @brief Event handling interface

* Description:

* 1. Users can customize the changes in WiFi module status

* 2. Users can add data points in the function of event processing logic, such as calling the relevant hardware peripherals operating interface

* @param [in] info: event queue
* @param [in] data: protocol data
* @param [in] len: protocol data length
* @return NULL
* @ref gizwits_protocol.h
*/
int8_t gizwitsEventProcess(eventInfo_t *info, uint8_t *gizdata, uint32_t len)
{
    uint8_t i = 0;
    dataPoint_t *dataPointPtr = (dataPoint_t *)gizdata;
    moduleStatusInfo_t *wifiData = (moduleStatusInfo_t *)gizdata;
    protocolTime_t *ptime = (protocolTime_t *)gizdata;
    
#if MODULE_TYPE
    gprsInfo_t *gprsInfoData = (gprsInfo_t *)gizdata;
#else
    moduleInfo_t *ptModuleInfo = (moduleInfo_t *)gizdata;
#endif

    if((NULL == info) || (NULL == gizdata))
    {
        return -1;
    }

    for(i=0; i<info->num; i++)
    {
        switch(info->event[i])
        {
        case EVENT_relay:
            currentDataPoint.valuerelay = dataPointPtr->valuerelay;
            GIZWITS_LOG("Evt: EVENT_relay %d \n", currentDataPoint.valuerelay);
            if(0x01 == currentDataPoint.valuerelay)
            {
                RELAY_ON;
            }
            else
            {
                RELAY_OFF;    
            }
            break;
        case EVENT_step:
            currentDataPoint.valuestep = dataPointPtr->valuestep;
            GIZWITS_LOG("Evt: EVENT_step %d \n", currentDataPoint.valuestep);
            if(0x01 == currentDataPoint.valuestep)
            {
                step_flag = 1;
                oled_ref = 1;
                Step_Soft_Control(5,1,4);
            }
            else
            {
                step_flag = 0;
                oled_ref = 1;
                Step_Soft_Control(5,2,4);    
            }
            break;
        case EVENT_led1:
            currentDataPoint.valueled1 = dataPointPtr->valueled1;
            GIZWITS_LOG("Evt: EVENT_led1 %d \n", currentDataPoint.valueled1);
            if(0x01 == currentDataPoint.valueled1)
            {
                LED1_ON;
            }
            else
            {
                LED1_OFF;    
            }
            break;
        case EVENT_led2:
            currentDataPoint.valueled2 = dataPointPtr->valueled2;
            GIZWITS_LOG("Evt: EVENT_led2 %d \n", currentDataPoint.valueled2);
            if(0x01 == currentDataPoint.valueled2)
            {
                LED2_ON;
            }
            else
            {
                LED2_OFF;    
            }
            break;
        case EVENT_led3:
            currentDataPoint.valueled3 = dataPointPtr->valueled3;
            GIZWITS_LOG("Evt: EVENT_led3 %d \n", currentDataPoint.valueled3);
            if(0x01 == currentDataPoint.valueled3)
            {
                LED3_ON;
            }
            else
            {
                LED3_OFF;    
            }
            break;


        case EVENT_r:
            currentDataPoint.valuer = dataPointPtr->valuer;
            GIZWITS_LOG("Evt:EVENT_r %d\n",currentDataPoint.valuer);
            rgb.r = currentDataPoint.valuer;
            RGB_SendData(rgb.r, rgb.g, rgb.b);
            oled_ref = 1;
            break;
        case EVENT_g:
            currentDataPoint.valueg = dataPointPtr->valueg;
            GIZWITS_LOG("Evt:EVENT_g %d\n",currentDataPoint.valueg);
            rgb.g = currentDataPoint.valueg;
            RGB_SendData(rgb.r, rgb.g, rgb.b);
            oled_ref = 1;
            break;
        case EVENT_b:
            currentDataPoint.valueb = dataPointPtr->valueb;
            GIZWITS_LOG("Evt:EVENT_b %d\n",currentDataPoint.valueb);
            rgb.b = currentDataPoint.valueb;
            RGB_SendData(rgb.r, rgb.g, rgb.b);
            oled_ref = 1;
            break;
        case EVENT_motor:
            currentDataPoint.valuemotor = dataPointPtr->valuemotor;
            GIZWITS_LOG("Evt:EVENT_motor %d\n",currentDataPoint.valuemotor);
            motor_speed = currentDataPoint.valuemotor;
            Morot_Up(200*currentDataPoint.valuemotor);
            break;


        case WIFI_SOFTAP:
            break;
        case WIFI_AIRLINK:
            break;
        case WIFI_STATION:
            break;
        case WIFI_CON_ROUTER:
 
            break;
        case WIFI_DISCON_ROUTER:
 
            break;
        case WIFI_CON_M2M:
 
            break;
        case WIFI_DISCON_M2M:
            break;
        case WIFI_RSSI:
            GIZWITS_LOG("RSSI %d\n", wifiData->rssi);
            break;
        case TRANSPARENT_DATA:
            GIZWITS_LOG("TRANSPARENT_DATA \n");
            //user handle , Fetch data from [data] , size is [len]
            break;
        case WIFI_NTP:
            GIZWITS_LOG("WIFI_NTP : [%d-%d-%d %02d:%02d:%02d][%d] \n",ptime->year,ptime->month,ptime->day,ptime->hour,ptime->minute,ptime->second,ptime->ntp);
            break;
        case MODULE_INFO:
            GIZWITS_LOG("MODULE INFO ...\n");
#if MODULE_TYPE
            GIZWITS_LOG("GPRS MODULE ...\n");
            //Format By gprsInfo_t
            GIZWITS_LOG("moduleType : [%d] \n",gprsInfoData->Type);
#else
            GIZWITS_LOG("WIF MODULE ...\n");
            //Format By moduleInfo_t
            GIZWITS_LOG("moduleType : [%d] \n",ptModuleInfo->moduleType);
#endif
        break;
        default:
            break;
        }
    }

    return 0;
}

/**
* User data acquisition

* Here users need to achieve in addition to data points other than the collection of data collection, can be self-defined acquisition frequency and design data filtering algorithm

* @param none
* @return none
*/
void userHandle(void)
{
    // 更新RGB数据
    currentDataPoint.valuer = rgb.r;
    currentDataPoint.valueg = rgb.g;
    currentDataPoint.valueb = rgb.b;
    
    // 温湿度数据已在Timer2_CallBack中更新，此处无需重复更新
    
}

/**
* Data point initialization function

* In the function to complete the initial user-related data
* @param none
* @return none
* @note The developer can add a data point state initialization value within this function
*/
void userInit(void)
{
    memset((uint8_t*)&currentDataPoint, 0, sizeof(dataPoint_t));
    
    /** Warning !!! DataPoint Variables Init , Must Within The Data Range **/ 
    currentDataPoint.valuerelay = 0; // 继电器初始关闭
    currentDataPoint.valuestep = 0;  // 窗帘初始关闭
    currentDataPoint.valueled1 = 0;  // LED1初始关闭
    currentDataPoint.valueled2 = 0;  // LED2初始关闭
    currentDataPoint.valueled3 = 0;  // LED3初始关闭
    currentDataPoint.valuer = 0;     // RGB R初始值
    currentDataPoint.valueg = 0;     // RGB G初始值
    currentDataPoint.valueb = 0;     // RGB B初始值
    currentDataPoint.valuemotor = 0; // 电机初始速度为0
    currentDataPoint.valuetemp = 25.0; // 温度初始值
    currentDataPoint.valuehumi = 50.0; // 湿度初始值

}


/**
* @brief  gizTimerMs

* millisecond timer maintenance function ,Millisecond increment , Overflow to zero

* @param none
* @return none
*/
void gizTimerMs(void)
{
    timerMsCount++;
}

/**
* @brief gizGetTimerCount

* Read system time, millisecond timer

* @param none
* @return System time millisecond
*/
uint32_t gizGetTimerCount(void)
{
    return timerMsCount;
}

/**
* @brief mcuRestart

* MCU Reset function

* @param none
* @return none
*/
void mcuRestart(void)
{
    __set_FAULTMASK(1);
    NVIC_SystemReset();
}
/**@} */

/**
* @brief TIMER_IRQ_FUN

* Timer Interrupt handler function

* @param none
* @return none
*/
void TIMER_IRQ_FUN(void)
{
  gizTimerMs();
}

/**
* @brief UART_IRQ_FUN

* UART Serial interrupt function ，For Module communication

* Used to receive serial port protocol data between WiFi module

* @param none
* @return none
*/
void UART_IRQ_FUN(void)
{
  uint8_t value = 0;
  //value = USART_ReceiveData(USART2);//STM32 test demo
  gizPutData(&value, 1);
}


/**
* @brief uartWrite

* Serial write operation, send data to the WiFi module

* @param buf      : Data address
* @param len       : Data length
*
* @return : Not 0,Serial send success;
*           -1，Input Param Illegal
*/
int32_t uartWrite(uint8_t *buf, uint32_t len)
{
    uint32_t i = 0;
    
    if(NULL == buf)
    {
        return -1;
    }
    
    #ifdef PROTOCOL_DEBUG
    GIZWITS_LOG("MCU2WiFi[%4d:%4d]: ", gizGetTimerCount(), len);
    for(i=0; i<len; i++)
    {
        GIZWITS_LOG("%02x ", buf[i]);
    }
    GIZWITS_LOG("\n");
    #endif

    for(i=0; i<len; i++)
    {
        // 发送数据到USART2（WiFi模块连接的串口）
        while(USART_GetFlagStatus(USART2, USART_FLAG_TXE) == RESET);
        USART_SendData(USART2, buf[i]);
        
        // 0xFF转义处理
        if(buf[i] == 0xFF)
        {
            while(USART_GetFlagStatus(USART2, USART_FLAG_TXE) == RESET);
            USART_SendData(USART2, 0x55);
        }
    }

    return len;
}



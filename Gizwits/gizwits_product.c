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
#include "main.h"
#include "rgb.h"
#include "app.h"

static uint32_t timerMsCount;

/** WiFi connection status variable */
static uint8_t wifiConnectionStatus = 0;

/** Current datapoint */
dataPoint_t currentDataPoint;
protocolTime_t ntp_time; // Global NTP time variable

/**
* @brief Get current WiFi connection status
*
* @return WiFi connection status (0: disconnected, 1: connected)
*/
uint8_t getCurrentWifiStatus(void)
{
    return wifiConnectionStatus;
}

//extern TIM_HandleTypeDef htim1;
//extern float te;
//extern float hu;
//extern uint8_t tim1_up_it;
//extern uint8_t tim3_up_it;
//extern uint8_t tim4_up_it;
//extern uint16_t timer_count;
//extern uint16_t timer_count1;
//extern uint16_t timer_count2;
//extern uint16_t timer_count3;
//extern uint8_t timer_flag;
//extern uint8_t timer_flag1;
//extern uint8_t timer_flag2;
//extern uint8_t timer_flag3;
//extern uint8_t timer_flag4;
//extern uint16_t pwm_value;
//extern uint8_t key_value;
//extern uint8_t key_flag;
//extern uint8_t key_long_flag;
//extern uint8_t key_long_value;
//extern uint8_t key_long_flag1;
//extern uint8_t key_long_value1;
//extern uint8_t key_long_flag2;
//extern uint8_t key_long_value2;
//extern uint8_t key_long_flag3;
//extern uint8_t key_long_value3;
//extern uint8_t key_long_flag4;
//extern uint8_t key_long_value4;
//extern uint8_t key_long_flag5;
//extern uint8_t key_long_value5;
//extern uint8_t key_long_flag6;
//extern uint8_t key_long_value6;
//extern uint8_t key_long_flag7;
//extern uint8_t key_long_value7;
//extern uint8_t key_long_flag8;
//extern uint8_t key_long_value8;
//extern uint8_t key_long_flag9;
//extern uint8_t key_long_value9;
//extern uint8_t key_long_flag10;
//extern uint8_t key_long_value10;
//extern uint8_t key_long_flag11;
//extern uint8_t key_long_value11;
//extern uint8_t key_long_flag12;
//extern uint8_t key_long_value12;
//extern uint8_t key_long_flag13;
//extern uint8_t key_long_value13;
//extern uint8_t key_long_flag14;
//extern uint8_t key_long_value14;
//extern uint8_t key_long_flag15;
//extern uint8_t key_long_value15;
//extern uint8_t key_long_flag16;
//extern uint8_t key_long_value16;
//extern uint8_t key_long_flag17;
//extern uint8_t key_long_value17;
//extern uint8_t key_long_flag18;
//extern uint8_t key_long_value18;
//extern uint8_t key_long_flag19;
//extern uint8_t key_long_value19;
//extern uint8_t key_long_flag20;
//extern uint8_t key_long_value20;
//extern uint8_t key_long_flag21;
//extern uint8_t key_long_value21;
//extern uint8_t key_long_flag22;
//extern uint8_t key_long_value22;
//extern uint8_t key_long_flag23;
//extern uint8_t key_long_value23;
//extern uint8_t key_long_flag24;
//extern uint8_t key_long_value24;
//extern uint8_t key_long_flag25;
//extern uint8_t key_long_value25;
//extern uint8_t key_long_flag26;
//extern uint8_t key_long_value26;
//extern uint8_t key_long_flag27;
//extern uint8_t key_long_value27;
//extern uint8_t key_long_flag28;
//extern uint8_t key_long_value28;
//extern uint8_t key_long_flag29;
//extern uint8_t key_long_value29;
//extern uint8_t key_long_flag30;
//extern uint8_t key_long_value30;
//extern uint8_t key_long_flag31;
//extern uint8_t key_long_value31;
//extern uint8_t key_long_flag32;
//extern uint8_t key_long_value32;
//extern uint8_t key_long_flag33;
//extern uint8_t key_long_value33;
//extern uint8_t key_long_flag34;
//extern uint8_t key_long_value34;
//extern uint8_t key_long_flag35;
//extern uint8_t key_long_value35;
//extern uint8_t key_long_flag36;
//extern uint8_t key_long_value36;
//extern uint8_t key_long_flag37;
//extern uint8_t key_long_value37;
//extern uint8_t key_long_flag38;
//extern uint8_t key_long_value38;
//extern uint8_t key_long_flag39;
//extern uint8_t key_long_value39;
//extern uint8_t key_long_flag40;
//extern uint8_t key_long_value40;
//extern uint8_t key_long_flag41;
//extern uint8_t key_long_value41;
//extern uint8_t key_long_flag42;
//extern uint8_t key_long_value42;
//extern uint8_t key_long_flag43;
//extern uint8_t key_long_value43;
//extern uint8_t key_long_flag44;
//extern uint8_t key_long_value44;
//extern uint8_t key_long_flag45;
//extern uint8_t key_long_value45;
//extern uint8_t key_long_flag46;
//extern uint8_t key_long_value46;
//extern uint8_t key_long_flag47;
//extern uint8_t key_long_value47;
//extern uint8_t key_long_flag48;
//extern uint8_t key_long_value48;
//extern uint8_t key_long_flag49;
//extern uint8_t key_long_value49;
//extern uint8_t key_long_flag50;
//extern uint8_t key_long_value50;

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
//        case EVENT_step:
//            currentDataPoint.valuestep = dataPointPtr->valuestep;
//            GIZWITS_LOG("Evt: EVENT_step %d \n", currentDataPoint.valuestep);
//            if(0x01 == currentDataPoint.valuestep)
//            {
//                MOTOR_FORWARD;
//            }
//            else
//            {
//                MOTOR_REVERSE;    
//            }
//            break;
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
            RGB_SendData(currentDataPoint.valuer, currentDataPoint.valueg, currentDataPoint.valueb);
            break;
        case EVENT_g:
            currentDataPoint.valueg = dataPointPtr->valueg;
            GIZWITS_LOG("Evt:EVENT_g %d\n",currentDataPoint.valueg);
            RGB_SendData(currentDataPoint.valuer, currentDataPoint.valueg, currentDataPoint.valueb);
            break;
        case EVENT_b:
            currentDataPoint.valueb = dataPointPtr->valueb;
            GIZWITS_LOG("Evt:EVENT_b %d\n",currentDataPoint.valueb);
            RGB_SendData(currentDataPoint.valuer, currentDataPoint.valueg, currentDataPoint.valueb);
            break;
        case EVENT_motor:
            currentDataPoint.valuemotor = dataPointPtr->valuemotor;
            GIZWITS_LOG("Evt:EVENT_motor %d\n",currentDataPoint.valuemotor);
            motor_speed = currentDataPoint.valuemotor; // 同步更新motor_speed变量，确保UI显示正确
            if(currentDataPoint.valuemotor > 0)
            {
                Morot_Up(currentDataPoint.valuemotor);
            }
            else
            {
                Morot_Down(abs(currentDataPoint.valuemotor));
            }
            break;


        case WIFI_SOFTAP:
            break;
        case WIFI_AIRLINK:
            break;
        case WIFI_STATION:
            break;
        case WIFI_CON_ROUTER:
            wifiConnectionStatus = 1; // WiFi connected to router
            break;
        case WIFI_DISCON_ROUTER:
            wifiConnectionStatus = 0; // WiFi disconnected from router
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
            // Store NTP time in global variable
            memcpy(&ntp_time, ptime, sizeof(protocolTime_t));
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
    // Temperature and humidity sensors are not implemented yet
    // currentDataPoint.valuetemp = te;
    // currentDataPoint.valuehumi = hu;
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
    currentDataPoint.valuerelay = 0;
    currentDataPoint.valuestep = 0;
    currentDataPoint.valueled1 = 0;
    currentDataPoint.valueled2 = 0;
    currentDataPoint.valueled3 = 0;
    currentDataPoint.valuer = 0;
    currentDataPoint.valueg = 0;
    currentDataPoint.valueb = 0;
    currentDataPoint.valuemotor = 0;
    currentDataPoint.valuetemp = 25.0;
    currentDataPoint.valuehumi = 50.0;

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

* UART Serial interrupt function ��For Module communication

* Used to receive serial port protocol data between WiFi module

* @param none
* @return none
*/
void UART_IRQ_FUN(void)
{
  uint8_t value = 0;
  value = USART_ReceiveData(USART2);//STM32 test demo
  gizPutData(&value, 1);
}


/**
* @brief uartWrite

* Serial write operation, send data to the WiFi module

* @param buf      : Data address
* @param len       : Data length
*
* @return : Not 0,Serial send success;
*           -1��Input Param Illegal
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
        // ͨ��USART2�������ݵ�WiFiģ��
        while(USART_GetFlagStatus(USART2, USART_FLAG_TXE) == RESET);
        USART_SendData(USART2, buf[i]);
        
        // ����ת���ַ�
        if(i >= 2 && buf[i] == 0xFF)
        {
            // ����ת���ַ�0x55
            while(USART_GetFlagStatus(USART2, USART_FLAG_TXE) == RESET);
            USART_SendData(USART2, 0x55);
        }
    }

    return len;
}



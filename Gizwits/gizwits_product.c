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
*               styyy
***********************************************************/
#include <stdio.h>
#include <string.h>
#include "gizwits_product.h"
#include "main.h"
#include "usart1.h"


u8 time_flag = 0;//0没连接上服务器，不获取时间  1
static uint32_t timerMsCount;

/** Current datapoint */
dataPoint_t currentDataPoint;

/** WiFi connection status: 0-未连接, 1-已连接 */
uint8_t wifiConnectionStatus = 0;

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
        case EVENT_LED1:
            currentDataPoint.valueLED1 = dataPointPtr->valueLED1;
            //GIZWITS_LOG("Evt: EVENT_LED1 %d \n", currentDataPoint.valueLED1);
            if(0x01 == currentDataPoint.valueLED1)
            {
            //user handle
							LED1_ON;
            }
            else
            {
							LED1_OFF;
            }
            break;
        case EVENT_LED2:
            currentDataPoint.valueLED2 = dataPointPtr->valueLED2;
            //GIZWITS_LOG("Evt: EVENT_LED2 %d \n", currentDataPoint.valueLED2);
            if(0x01 == currentDataPoint.valueLED2)
            {
            //user handle
							LED2_ON;
            }
            else
            {
            //user handle
							LED2_OFF;
            }
            break;
        case EVENT_LED3:
            currentDataPoint.valueLED3 = dataPointPtr->valueLED3;
            //GIZWITS_LOG("Evt: EVENT_LED3 %d \n", currentDataPoint.valueLED3);
            if(0x01 == currentDataPoint.valueLED3)
            {
            //user handle
							LED3_ON;
            }
            else
            {
            //user handle  
							LED3_OFF;
            }
            break;
        case EVENT_step:
            currentDataPoint.valuestep = dataPointPtr->valuestep;
            //GIZWITS_LOG("Evt: EVENT_step %d \n", currentDataPoint.valuestep);
            if(0x01 == currentDataPoint.valuestep)
            {
            //user handle
		  				step_flag = 1;
							if(page_flag == 6)  oled_ref = 1;
							else  Step_Motor_Control(1,1,50);
            }
            else
            {
            //user handle
							step_flag = 0;
							if(page_flag == 6)  oled_ref = 1;
							else Step_Motor_Control(1,2,50);
            }
            break;
        case EVENT_relay:
            currentDataPoint.valuerelay = dataPointPtr->valuerelay;
            //GIZWITS_LOG("Evt: EVENT_relay %d \n", currentDataPoint.valuerelay);
            if(0x01 == currentDataPoint.valuerelay)
            {
            //user handle
							RELAY_ON;
            }
            else
            {
            //user handle   
							RELAY_OFF;
            }
            break;

        case EVENT_motor:
            currentDataPoint.valuemotor = dataPointPtr->valuemotor;
            //GIZWITS_LOG("Evt: EVENT_motor %d\n", currentDataPoint.valuemotor);
            switch(currentDataPoint.valuemotor)
            {
            case motor_VALUE0:
                //user handle
							  motor_speed = 0;
								if(page_flag != 4) Morot_Up(0);
                break;
            case motor_VALUE1:
                //user handle
								motor_speed = 1;
								if(page_flag != 4) Morot_Up(200);
                break;
            case motor_VALUE2:
                //user handle
								motor_speed = 2;
								if(page_flag != 4) Morot_Up(400);
                break;
            case motor_VALUE3:
                //user handle
								motor_speed = 3;
								if(page_flag != 4) Morot_Up(600);
                break;
            case motor_VALUE4:
                //user handle
								motor_speed = 4;
								if(page_flag != 4) Morot_Up(800);
                break;
            case motor_VALUE5:
                //user handle
								motor_speed = 5;
								if(page_flag != 4) Morot_Up(1000);
                break;
            default:
                break;
            }
						ui_flag = 0;
            break;

        case EVENT_R:
            currentDataPoint.valueR = dataPointPtr->valueR;
            //GIZWITS_LOG("Evt:EVENT_R %d\n",currentDataPoint.valueR);
            //user handle
			oled_ref = 1;
			rgb.r = currentDataPoint.valueR;
			RGB_SendData((uint8_t)currentDataPoint.valueR,(uint8_t)currentDataPoint.valueG,(uint8_t)currentDataPoint.valueB);
            break;
        case EVENT_G:
            currentDataPoint.valueG = dataPointPtr->valueG;
           // GIZWITS_LOG("Evt:EVENT_G %d\n",currentDataPoint.valueG);
            //user handle
			oled_ref = 1;
			rgb.g = currentDataPoint.valueG;
			RGB_SendData((uint8_t)currentDataPoint.valueR,(uint8_t)currentDataPoint.valueG,(uint8_t)currentDataPoint.valueB);
            break;
        case EVENT_B:
            currentDataPoint.valueB = dataPointPtr->valueB;
            //GIZWITS_LOG("Evt:EVENT_B %d\n",currentDataPoint.valueB);
            //user handle
			oled_ref = 1;
			rgb.b = currentDataPoint.valueB;
			RGB_SendData((uint8_t)currentDataPoint.valueR,(uint8_t)currentDataPoint.valueG,(uint8_t)currentDataPoint.valueB);
            break;


        case WIFI_SOFTAP:
            break;
        case WIFI_AIRLINK:
            break;
        case WIFI_STATION:
            break;
        case WIFI_CON_ROUTER://连接服务器成功
						time_flag = 1;
            break;
        case WIFI_DISCON_ROUTER:
 
            break;
        case WIFI_CON_M2M://联网成功
						time_flag = 1;
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
           // GIZWITS_LOG("WIFI_NTP : [%d-%d-%d %02d:%02d:%02d][%d] \n",ptime->year,ptime->month,ptime->day,ptime->hour,ptime->minute,ptime->second,ptime->ntp);
						
						if(page_flag==1)
						{
							u8 tim_buff[30] = {0};
							sprintf((char*)tim_buff,"%02d-%02d-%02d",ptime->year,ptime->month,ptime->day);
							OLED_DisplayStr(24,2,tim_buff);
							sprintf((char*)tim_buff,"%02d:%02d:%02d",ptime->hour,ptime->minute,ptime->second);
							OLED_DisplayStr(32,4,tim_buff);
						}
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
    //读取温湿度传感器数据
    DHT11_Read();
    
    //将传感器数据赋值给当前数据点，用于上报云端
    currentDataPoint.valuetemp = dht_temp;//温度数据
    currentDataPoint.valuehumi = dht_humi;//湿度数据
    
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
    /*
    currentDataPoint.valueLED1 = ;
    currentDataPoint.valueLED2 = ;
    currentDataPoint.valueLED3 = ;
    currentDataPoint.valuestep = ;
    currentDataPoint.valuerelay = ;
    currentDataPoint.valuemotor = ;
    currentDataPoint.valueR = ;
    currentDataPoint.valueG = ;
    currentDataPoint.valueB = ;
    currentDataPoint.valuetemp = ;
    currentDataPoint.valuehumi = ;
    */

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
        //USART_SendData(UART, buf[i]);//STM32 test demo
        //Serial port to achieve the function, the buf[i] sent to the module
        USART_SendData(USART2, buf[i]);
				while(USART_GetFlagStatus(USART2,USART_FLAG_TXE)==RESET);
			  
				if(i >=2 && buf[i] == 0xFF)
        {
          //Serial port to achieve the function, the 0x55 sent to the module
          //USART_SendData(UART, 0x55);//STM32 test demo
					USART_SendData(USART2,0x55);
					while (USART_GetFlagStatus(USART2, USART_FLAG_TXE) == RESET);
        }
    }


    
    return len;
}



/**
  ******************************************************************************
  * @file    rtc.h
  * @author  WI6LABS
  * @version V1.0.0
  * @date    12-December-2017
  * @brief   Header for RTC driver
  ******************************************************************************
  * @attention
  *
  * <h2><center>&copy; COPYRIGHT(c) 2017 STMicroelectronics</center></h2>
  *
  * Redistribution and use in source and binary forms, with or without modification,
  * are permitted provided that the following conditions are met:
  *   1. Redistributions of source code must retain the above copyright notice,
  *      this list of conditions and the following disclaimer.
  *   2. Redistributions in binary form must reproduce the above copyright notice,
  *      this list of conditions and the following disclaimer in the documentation
  *      and/or other materials provided with the distribution.
  *   3. Neither the name of STMicroelectronics nor the names of its contributors
  *      may be used to endorse or promote products derived from this software
  *      without specific prior written permission.
  *
  * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
  * AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
  * IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
  * DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE LIABLE
  * FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL
  * DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR
  * SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER
  * CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY,
  * OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE
  * OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
  *
  ******************************************************************************
  */

/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __RTC_H
#define __RTC_H

/* Includes ------------------------------------------------------------------*/
#include "stm32_def.h"

#ifdef HAL_RTC_MODULE_ENABLED

#ifdef __cplusplus
 extern "C" {
#endif

/* Exported types ------------------------------------------------------------*/
typedef enum {
  HOUR_FORMAT_12,
  HOUR_FORMAT_24
} hourFormat_t;

typedef enum {
  AM,
  PM
} hourAM_PM_t;

// Clock source selection
typedef enum {
  LSI_CLOCK,
  LSE_CLOCK,
  HSE_CLOCK
} sourceClock_t;

typedef void(*voidCallbackPtr)(void *);

/* Exported constants --------------------------------------------------------*/
#if defined(STM32F0xx) || defined(STM32L0xx)
#define RTC_Alarm_IRQn RTC_IRQn
#define RTC_Alarm_IRQHandler  RTC_IRQHandler
#endif

#if defined(STM32F1xx) && !defined(IS_RTC_WEEKDAY)
// Compensate missing HAL definition
#define IS_RTC_WEEKDAY(WEEKDAY) (((WEEKDAY) == RTC_WEEKDAY_MONDAY)    || \
                                 ((WEEKDAY) == RTC_WEEKDAY_TUESDAY)   || \
                                 ((WEEKDAY) == RTC_WEEKDAY_WEDNESDAY) || \
                                 ((WEEKDAY) == RTC_WEEKDAY_THURSDAY)  || \
                                 ((WEEKDAY) == RTC_WEEKDAY_FRIDAY)    || \
                                 ((WEEKDAY) == RTC_WEEKDAY_SATURDAY)  || \
                                 ((WEEKDAY) == RTC_WEEKDAY_SUNDAY))

#define IS_RTC_HOUR12(HOUR)      IS_RTC_HOUR24(HOUR)
#endif // defined(STM32F1xx) && !defined(IS_RTC_WEEKDAY)

/* RTC prescalers are set to obtain the RTC clock to 1Hz. See AN4759.
  Default prescalers are calculated for the following clock speed:
  - LSI = 32, 37 or 40 kHz
  - LSE = 32.768 kHz
  - HSE = 8 MHz

 Prescalers should be set by user in case the clock configuration is different.
*/

// LSE
#ifndef LSE_ASYNCH_PREDIV
#define LSE_ASYNCH_PREDIV 127U
#endif

#ifndef LSE_SYNCH_PREDIV
#define LSE_SYNCH_PREDIV  255U
#endif

// LSI
#ifndef LSI_ASYNCH_PREDIV
// 32kHz
#if defined(STM32L4xx) || defined(STM32F2xx) || defined(STM32F4xx) || defined(STM32F7xx)
#define LSI_ASYNCH_PREDIV 127U
// 37kHz
#elif defined(STM32L0xx) || defined(STM32L1xx)
#define LSI_ASYNCH_PREDIV 124U
// 40kHz
#elif defined(STM32F0xx) || defined(STM32F1xx) || defined(STM32F3xx)
#define LSI_ASYNCH_PREDIV 127U
#endif
#endif //LSI_ASYNCH_PREDIV

#ifndef LSI_SYNCH_PREDIV
// 32kHz
#if defined(STM32L4xx) || defined(STM32F2xx) || defined(STM32F4xx) || defined(STM32F7xx)
#define LSI_SYNCH_PREDIV 249U
// 37kHz
#elif defined(STM32L0xx) || defined(STM32L1xx)
#define LSI_SYNCH_PREDIV 295U
// 40kHz
#elif defined(STM32F0xx) || defined(STM32F1xx) || defined(STM32F3xx)
#define LSI_SYNCH_PREDIV 311U
#endif
#endif //LSI_SYNCH_PREDIV

// HSE
#ifndef HSE_ASYNCH_PREDIV
#if defined(STM32L4xx) || defined(STM32F0xx) || defined(STM32F3xx)
//HSE DIV32
#define HSE_ASYNCH_PREDIV 124U
#else
//HSE DIV8
#define HSE_ASYNCH_PREDIV  99U
#endif
#endif //HSE_ASYNCH_PREDIV

#ifndef HSE_SYNCH_PREDIV
#if defined(STM32L4xx) || defined(STM32F0xx) || defined(STM32F3xx)
//HSE DIV32
#define HSE_SYNCH_PREDIV  1999U
#else
//HSE DIV8
#define HSE_SYNCH_PREDIV  9999U
#endif
#endif //HSE_SYNCH_PREDIV

/* Exported macro ------------------------------------------------------------*/
/* Exported functions ------------------------------------------------------- */
void RTC_init(hourFormat_t format, sourceClock_t source);
void RTC_DeInit(void);

void RTC_SetTime(uint8_t hours, uint8_t minutes, uint8_t seconds, uint32_t subSeconds, hourAM_PM_t format);
void RTC_GetTime(uint8_t *hours, uint8_t *minutes, uint8_t *seconds, uint32_t *subSeconds, hourAM_PM_t *format);

void RTC_SetDate(uint8_t year, uint8_t month, uint8_t date, uint8_t day);
void RTC_GetDate(uint8_t *year, uint8_t *month, uint8_t *date, uint8_t *day);

void RTC_StartAlarm(uint8_t date, uint8_t hours, uint8_t minutes, uint8_t seconds, uint32_t subSeconds, hourAM_PM_t format);
void RTC_StopAlarm(void);
void RTC_GetAlarm(uint8_t *date, uint8_t *hours, uint8_t *minutes, uint8_t *seconds, uint32_t *subSeconds, hourAM_PM_t *format);
void attachAlarmCallback(voidCallbackPtr func, void *data);
void detachAlarmCallback(void);

#ifdef __cplusplus
 }
#endif

#endif // HAL_RTC_MODULE_ENABLED

#endif /* __RTC_H */

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
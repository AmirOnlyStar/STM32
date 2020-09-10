#ifndef _SIM808_H_
#define _SIM808_H_

#include "usart.h"
#include "stdio.h"

typedef unsigned char uint8_t;

typedef unsigned char u8;
typedef signed char s8;

typedef unsigned int u16;
typedef signed int s16;

typedef unsigned long int u32;
typedef signed long int s32;

#define GSM_BUFF_SIZE 2048



extern UART_HandleTypeDef huart1;

s8 GSM_callback(void);
s8 GSM_test_connection(void);
s8 GSM_send_atcommand(char *str_in);
s8 GSM_init(void);
s8 GSM_get_rsp(void);
//########################## buffer ##############################
s8 GSM_start_recive(void);
s8 GSM_stop_recive(void);
s8 GSM_restart_recive(void);
s8 GSM_clear_buff(void);

//########################## SMS #################################
s8 GSM_SMS(void);
s8 GSM_SMS_read(u8 index);
//########################## RING ################################
s8 GSM_ring(void);
#endif



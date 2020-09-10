#include "sim808.h"
#include "main.h"
#include "cmsis_os.h"
#include "string.h"
#include <stdlib.h>

char str_GSM[GSM_BUFF_SIZE];

uint8_t flag_RDY = 0;
uint8_t flag_call = 0;
uint8_t flag_sms = 0;

u16 state_CFUN = 0;
// Set Phone Functionality  +CFUN: 1
// 0  Minimum functionality
// 1	Full functionality (Default)
// 4 	Disable phone both transmit and receive RF circuits

u16 state_GPRS = 0;
//Network Registration Status +CGREG: 1
//0 Not registered, MT is not currently searching a new operator to register to
//1 Registered, home network
//2 Not registered, but MT is currently searching a new
//3 Registration denied
//4 Unknown
//5 Registered, roaming

u16 state_GSM= 0;
//Network Registration Status +CREG: 1
//0 Not registered, MT is not currently searching a new operator to register to
//1 Registered, home network
//2 Not registered, but MT is currently searching a new
//3 Registration denied
//4 Unknown
//5 Registered, roaming

struct SMS
{
u8 index;
u8 storage[10];
u8 phone_number[20];
u8 state[20];
u8 text[100];
}SMS_info={0,"","","",""};

struct CALL
{
u8 phone_number[20];
}CALL_info={""};

s8 GSM_callback(void)
{
	char *ptr;
//######################################################
//################# #flag ##############################
//######################################################
		if(strstr(str_GSM,"RDY"))
		{
			flag_RDY = 1;
			printf("Recive RDY\r\n");
			GSM_restart_recive();
		}else if(strstr(str_GSM,"Call Ready"))
		{
			printf("Recive Call Ready\r\n");
			flag_call = 1;
			GSM_restart_recive();
		}else if(strstr(str_GSM,"SMS Ready"))
		{
			printf("Recive SMS Ready\r\n");
			flag_sms= 1;
			GSM_restart_recive();
			
		}else if(strstr(str_GSM,"+CFUN:"))
		{
			osDelay(100);
			ptr = strstr(str_GSM,"+CFUN:");
			sscanf(ptr,"%*[^:]: %d",&state_CFUN);
			printf("state_CFUN:%d\r\n",state_CFUN);
			GSM_restart_recive();
		}
		else if(strstr(str_GSM,"+CREG:"))
		{	
			osDelay(100);
			ptr = strstr(str_GSM,"+CREG:");
			sscanf(ptr,"%*[^:]: %d",&state_GSM);
			printf("state_GSM:%d\r\n",state_GSM);
			GSM_restart_recive();
		}else if(strstr(str_GSM,"+CGREG:"))
		{
			osDelay(100);
			ptr = strstr(str_GSM,"+CGREG:");
			sscanf(ptr,"%*[^:]: %d",&state_GPRS);
			printf("state_GPRS:%d\r\n",state_GPRS);
			GSM_restart_recive();
		}
//######################################################
//################# #input #############################
//######################################################
		if(strstr(str_GSM,"+CLIP:"))
		{
			osDelay(100);
			printf("str_GSM_RING:%s\r\n",str_GSM);
			GSM_ring();
		}else if(strstr(str_GSM,"+CMTI:"))
		{
			osDelay(100);
			printf("str_GSM_SMS:%s\r\n",str_GSM);
			GSM_SMS();
		}
		return 0;
}
s8 GSM_test_connection(void)
{
	s8 ret;
	ret = GSM_send_atcommand("AT\n");
	printf("Test Connection:%d\r\n",ret);
	if(ret == 0) return 0;
	else return -1;
}

s8 GSM_send_atcommand(char *str_in)
{
	s8 ret;
	u16 len=0;
	for(int i=0;str_in[i]!= 0;i++)
	{
		len++;
	}
//	printf("LEN:%d\r\n",len);
	GSM_restart_recive();
	HAL_UART_Transmit_IT(&huart1,(uint8_t*)str_in,len);
	
	ret = GSM_get_rsp();
	return ret;
	
}

s8 GSM_init(void)
{
	s8 ret;
//	GSM_restart_recive();
	
	while(ret != 0)
	{
		ret = GSM_test_connection();
		osDelay(1000);
	}
	
	if(ret == 0)
	{
		ret = GSM_send_atcommand("AT\n");
		printf("AT Command1 RSP:%d\r\n",ret);
		osDelay(10);
		ret = GSM_send_atcommand("ATE0\n");
		printf("AT Command2 RSP:%d\r\n",ret);
		osDelay(10);
		ret = GSM_send_atcommand("AT+CMEE=1\n");
		printf("AT Command3 RSP:%d\r\n",ret);
		osDelay(10);
		ret =	GSM_send_atcommand("AT+IPR=115200\n");
		printf("AT Command4 RSP:%d\r\n",ret);
		osDelay(10);
		ret =	GSM_send_atcommand("AT+CREG=1\n");
		printf("AT Command5 RSP:%d\r\n",ret);	
		osDelay(10);
		ret =	GSM_send_atcommand("AT+CGREG=1\n");
		printf("AT Command6 RSP:%d\r\n",ret);	
		osDelay(10);
		ret =	GSM_send_atcommand("AT+CLIP=1\n");
		printf("AT Command7 RSP:%d\r\n",ret);
		osDelay(10);
		ret =	GSM_send_atcommand("AT+CMGD=1,4\n");
		printf("AT Command8 RSP:%d\r\n",ret);
		osDelay(100);
		ret =	GSM_send_atcommand("AT+CMGF=1,4\n");
		printf("AT Command9 RSP:%d\r\n",ret);
		osDelay(100);
		GSM_restart_recive();
		return 0;
	}else
	{
		printf("Connection to the GSM lost\r\n");
		return -1;
	}

}


s8 GSM_get_rsp(void)
{
	uint32_t last_time =  HAL_GetTick();
	const uint32_t TimeOut =  1000;

	while(HAL_GetTick()-last_time<TimeOut)
	{
//		printf("RSP time : %d \r\n",HAL_GetTick()-last_time);
		if(strstr(str_GSM,"OK"))
		{
//			printf("GET OK YES\r\n");
			return 0;	
		}else if(strstr(str_GSM,"+CME ERROR:")||strstr(str_GSM,"+CMS ERROR:"))
		{
//			printf("GET OK NO\r\n");
			return -1;
		}
	}
	printf("GET OK Tiem out\r\n");
	return -2;
}

s8 GSM_start_recive(void)
{
	HAL_UART_Receive_DMA(&huart1,(uint8_t*)str_GSM,sizeof(str_GSM));
	return 0;
}
s8 GSM_stop_recive(void)
{
	HAL_UART_DMAStop(&huart1);
	return 0 ;
}
s8 GSM_restart_recive(void)
{
	GSM_stop_recive();
	GSM_clear_buff();
	GSM_start_recive();
	return 0;
}
s8 GSM_clear_buff(void)
{
	memset(str_GSM,0,sizeof(str_GSM));
	return 0;
}

//########################## SMS #################################
s8 GSM_SMS(void)
{
//+CMTI: "SM",1
//	char str[]="+CMTI: \"SM\",1\0";
	char str_temp[100]="";
	s8 ret;
	char *ptr;
	
	memset(SMS_info.storage,0,sizeof(SMS_info.storage));
	memset(SMS_info.phone_number,0,sizeof(SMS_info.phone_number));
	memset(SMS_info.state,0,sizeof(SMS_info.state));
	memset(SMS_info.text,0,sizeof(SMS_info.text));
	
	ptr = strstr(str_GSM,"+CMTI:");
	sscanf(ptr,"%*[^:]: %[^,],%s",SMS_info.storage,str_temp);
	SMS_info.index = atoi(str_temp);
	
	ret = GSM_SMS_read(SMS_info.index);
	if(ret != 0) return -1;
	
	printf("SMS INFO:%s,%s,%s,%s,%d\r\n"\
		,SMS_info.storage,SMS_info.phone_number,SMS_info.state,SMS_info.text,SMS_info.index);
	
	GSM_restart_recive();
	return 0;
}
s8 GSM_SMS_read(u8 index)
{
//+CMGR: "REC UNREAD","+989333134156","","20/03/28,18:54:26+18"
//Ttt
	s8 ret;
	char *ptr;
	char str_temp[100]="";
	osDelay(1000);
	sprintf(str_temp,"AT+CMGR=%d\n",SMS_info.index);
	ret =	GSM_send_atcommand(str_temp);
	
	if(ret != 0) return -1;
	
	ret = GSM_get_rsp();
	if(ret == 0)
	{
		ptr = strstr(str_GSM,"+CMGR:");
		if(ptr != 0)
		{
			sscanf(ptr,"%*[^:]: %[^,],%[^,],%*[^\r\n]%*[\r]%*[\n]%[^\r]",SMS_info.state,SMS_info.phone_number,SMS_info.text);
	//	printf("SMS INFO 2:%s,%s,%s\r\n",SMS_info.state,SMS_info.phone_number,SMS_info.text);
			memset(str_temp,0,sizeof(str_temp));
	//	sprintf(str_temp,"AT+CMGD=%d,0\n",SMS_info.index);
	//	sprintf(str_temp,"AT+CMGD=1,4\n");
	//	printf("XXX:%s\r\n",str_temp);
		ret =	GSM_send_atcommand("AT+CMGD=1,4\n");
		if(ret != 0) return -1;
		return 0;
		}else
		{
			return -1;
		}
	}else
	{
		printf("Fail to read SMS \r\n");
		return -1;
	}
}

//########################## RING ################################
s8 GSM_ring(void)
{
	//+CLIP: "+989333134156",145,"",0,"",0
	//	char str3[]="+CLIP: \"+989333134156\",145,\"\",0,\"\",0\r\n";
	char *ptr;
	ptr = strstr(str_GSM,"+CLIP:");
	sscanf(ptr,"%*[^:]: %[^,]",CALL_info.phone_number);
	printf("STR CALL:%s\r\n",CALL_info.phone_number);
	GSM_restart_recive();
	return 0;
}




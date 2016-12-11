#ifndef   ESP8266_H_H
#define   ESP8266_H_H

#define   DEVICEID   "3225091"
#define   APIKEY     "IttMtjovk2xY0o3ElAFoWp9mAsk="

#define   AT          "AT\r\n"	
#define   CWMODE      "AT+CWMODE=1\r\n"
#define   RST         "AT+RST\r\n"
#define   CIFSR       "AT+CIFSR\r\n"
#define   CWJAP       "AT+CWJAP=\"MI_dream-box\",\"18819418069\"\r\n"
#define   CIPSTART    "AT+CIPSTART=\"TCP\",\"183.230.40.39\",876\r\n"
#define   CIPMODE     "AT+CIPMODE=1\r\n"
#define   CIPSEND     "AT+CIPSEND\r\n"
#define   CIPSTATUS   "AT+CIPSTATUS\r\n"

#define   MAX_SEND_BUF_LEN  1024

//extern    EdpPacket* send_pkg;
extern    char send_buf[MAX_SEND_BUF_LEN];

extern  void ESP8266_Init(void);
extern  void GetSendBuf(void);
extern  void SendCmd(char* cmd, char* result, int timeOut);
extern  void ESP8266_DevLink(const char* devid, const char* auth_key, int timeOut);
extern   int ESP8266_CheckStatus(int timeOut);
extern  void ESP8266_SendDat(void);

#endif



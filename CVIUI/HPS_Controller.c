#include <rs232.h>
#include <ansi_c.h>   
#include <cvirte.h>
#include <userint.h>
#include "HPS_Controller.h"
#include "HPS_my.h"
#include "ExtraVar.h"
#include "Functions.h"
#include <formatio.h>
#include <utility.h>
#include "toolbox.h" 
#include "src/modbus.h"
#include <time.h>


int comState;
int InQLength;
char error_str[64];

modbus_t *mb;    
modbus_mapping_t* mb_mapping;
int offset;


uint32_t tv_sec = 60; 
uint32_t tv_usec = 0; 

uint16_t tag_reg[32];

uint16_t send_reg[32]; 
uint8_t req;
uint8_t query[MODBUS_TCP_MAX_ADU_LENGTH];  

unsigned short control_word;  

unsigned char comPort;
unsigned char buffin[512];	  //�����������
unsigned char buffout[8]={0xFF,0xFF,0x00,0x00,0x00,0x00,0xEE,0xEE};	  //�����������
unsigned char checkSum;       //У���
unsigned short int comValue;  //����֡
unsigned char valve_ctrl;     //DA���ƶ˿�

int HPSPanelInit(void);
int HPSClose(void);
unsigned char start_sf,cool_sf,source_sf,load_sf;					   //���ٿذ�ť�ٿ�˵���Ƿ���ʾָʾ
int pump_sf,accu_sf,using_sf,HF_sf,LF_sf,temp_sf,level_sf,remote_sf,travel_sf;         //�������Ƶ�ǰһʱ��״̬


float using_p;
float return_p; 
float source_p1; 
float source_p2;    
float sys_temp; 
float sys_level; 


float dSetPress;
int ret;

float value;
unsigned short int ivalue;
unsigned char sys_p_h_byte;
unsigned char sys_p_l_byte; 
/*
int main (int argc, char *argv[])
{
	int HPSPanelHandle;
	if (InitCVIRTE (0, argv, 0) == 0)
		return -1;	// out of memory 
	if ((HPSPanelHandle = LoadPanel (0, "HPS_Controller.uir", HPSPANEL)) < 0)
		return -1;
	DisplayPanel (HPSPanelHandle);
	HPSPanelInit();
	RunUserInterface ();
	DiscardPanel (HPSPanelHandle);
	return 0;
}
*/
int HPSPanelInit(void)
{
	int rc;
	if ((HPSPanelHandle = LoadPanel (0, "HPS_Controller.uir", HPSPANEL)) < 0)
		return -1;
	//GetCtrlVal(HPSPanelHandle, HPSPANEL_COM_RING, &comPort);
	
	mb = modbus_new_rtu("COM6", 9600, 'N', 8, 1);
	if(mb == NULL)MessagePopup("����", "modbus rtu �򿪴������ѯ�������ã�");   
	
	modbus_set_debug(mb, 1);      //����1�ɿ���������Ϣ

	rc = modbus_set_slave(mb, 2);  // ���ôӻ��� 
	if(rc != 0) MessagePopup("����set slave", "modbus rtu slave error��");
	
	
	/*
	rc = modbus_rtu_get_serial_mode (mb);
	if (rc == -1) { 
         sprintf(error_str, "%s\n", modbus_strerror(errno));
		 MessagePopup("����get 485", error_str);  
		 rc = 0;
    } 
	*/
	
	/*
	rc = modbus_rtu_set_serial_mode(mb, MODBUS_RTU_RS485); 
	if (rc == -1) { 
         sprintf(error_str, "%s\n", modbus_strerror(errno));
		 MessagePopup("����set 485", error_str);  
		 rc = 0;
    } 
	*/
	
	/*
	rc = modbus_rtu_set_rts(mb, MODBUS_RTU_RTS_UP);
	if (rc == -1) { 
         sprintf(error_str, "%s\n", modbus_strerror(errno));
		 MessagePopup("����set rts ", error_str);  
		 rc = 0;
    } 
	*/
	

	/* Save original timeout */
	//modbus_get_response_timeout(mb, &tv_sec, &tv_usec);
	
	/* Define a new timeout of 200ms */
	tv_sec = 0;
	tv_usec = 20000;
	modbus_set_response_timeout(mb, tv_sec, tv_usec);
	
	
	rc = modbus_connect(mb); 
	if(rc != 0) MessagePopup("����connect", "modbus rtu connect error��"); 
	
	//����4���ڴ������Դ�żĴ������ݣ����������64���Ĵ�����ַ
	mb_mapping = modbus_mapping_new(64, 64, 64, 64);
	//With this code, only 24 registers (uint16_t) are allocated.
	//mb_mapping = modbus_mapping_new_start_address(0, 0, 0, 0, 40000, 24, 0, 0);
	
	if (mb_mapping == NULL)
	{
		sprintf(error_str, "Error mapping:%s\n", modbus_strerror(errno));
		MessagePopup("����set 485", error_str); 
		modbus_free(mb);
		return -1;
	}
	
	
	/*
	//���ô���
	comState = OpenComConfig (comPort, "", 9600, 0, 8, 1, 512, 8);
	
	if(comState < 0)
	{
		MessagePopup("����", "���ڴ򿪴������ѯ�������ã�");
		return -1;
	}
	InQLength = GetInQLen (comPort);
	if(InQLength>512)
		InQLength = 512;
	ComRd (comPort, buffin, InQLength);   
	*/
	return 0;
}

int HPSPanelClose(void)     
{
	return 0;
}

int HPSClose(void)
{
	//�ͷŴ�����Դ
	modbus_mapping_free(mb_mapping);
	modbus_close(mb);
	modbus_free(mb);
	return 0;
}

void DisplayHPSPanel(void)
{
	 DisplayPanel(HPSPanelHandle);
	 RunUserInterface();    
}

void HPSPanelExit(void)
{
	DiscardPanel(HPSPanelHandle); 
	//if(comStatus1 > 0)
	//{
	//	CloseCom (comPort1);
	//	comStatus1 = -1;
	//}
	//	if(comStatus2 > 0)
	//{
	//	CloseCom (comPort2);
	//	comStatus2 = -1;
	//}
}

int CVICALLBACK set_sys_p (int panel, int control, int event,
		void *callbackData, int eventData1, int eventData2)
{
	switch (event)
	{
		case EVENT_COMMIT:
			
				GetCtrlVal(HPSPanelHandle, HPSPANEL_SYS_P_DIAL, &dSetPress);//��ȡ��ǰ����ѹ��
				
				if(dSetPress > 21.0)
				{
					dSetPress = 21;
					SetCtrlVal (HPSPanelHandle, HPSPANEL_SYS_P_DIAL, 21.0);
				    InsertTextBoxLine (HPSPanelHandle, HPSPANEL_STATES_TEXTBOX, 0, "����ѹ�����ֻ��Ϊ21Mpa");
				}
				
				value = dSetPress / 21.0  * (13824);
				send_reg[0]  = (unsigned short int)value; 
				
				GetCtrlVal(HPSPanelHandle, HPSPANEL_SYS_P_DIAL_2, &dSetPress);//��ȡ��ǰ����ѹ��
				
				if(dSetPress > 21.0)													 
				{
					dSetPress = 21;
					SetCtrlVal (HPSPanelHandle, HPSPANEL_SYS_P_DIAL_2, 21.0);
				    InsertTextBoxLine (HPSPanelHandle, HPSPANEL_STATES_TEXTBOX, 0, "����ѹ�����ֻ��Ϊ21Mpa");
				}
				
				value = dSetPress / 21.0  * (13824) ;   
				send_reg[1]  = (unsigned short int)value;
			
			break;
	}
	return 0;
}

int CVICALLBACK sys_start (int panel, int control, int event,
		void *callbackData, int eventData1, int eventData2)
{
	unsigned char state;

//	GetCtrlVal (HPSPANEL, HPSPANEL_MOTOR_RING, &state);
	
	switch (event)
	{
		
		case EVENT_LEFT_CLICK:
			/*

			  // SetCtrlVal (HPSPANEL, HPSPANEL_MOTOR_RING, !state);
			
			   buffout[2] = 0x11;
	           buffout[3] = 0x01;
	           buffout[4] = 0x01;
			   buffout[5] = buffout[2]^buffout[3]^buffout[4];
			   
			
			   ComWrt (comPort, buffout, 8); //�������  
			*/
			GetCtrlVal(HPSPanelHandle, HPSPANEL_MOTOR_RING, &state);//��ȡ��ǰ����ѹ�� 
			
			if(state >0 )
			{
				control_word = control_word & 0xF700;    
				control_word = control_word | 0x1000;
				SetCtrlVal (HPSPANEL, HPSPANEL_MOTOR_RING, !state); 
			}
			else
			{
				control_word = control_word & 0xEF00;    
				control_word = control_word | 0x0800;
				SetCtrlVal(HPSPANEL, HPSPANEL_MOTOR_RING, !state); 
			
			}
			
			send_reg[2] = control_word;
		break;
	}
	return 0;
}

int CVICALLBACK sys_start2 (int panel, int control, int event,
		void *callbackData, int eventData1, int eventData2)
{
	unsigned char state;

//	GetCtrlVal (HPSPANEL, HPSPANEL_MOTOR_RING, &state);
	
	switch (event)
	{
		
		case EVENT_LEFT_CLICK:
			/*

			  // SetCtrlVal (HPSPANEL, HPSPANEL_MOTOR_RING, !state);
			
			   buffout[2] = 0x11;
	           buffout[3] = 0x01;
	           buffout[4] = 0x01;
			   buffout[5] = buffout[2]^buffout[3]^buffout[4];
			   
			
			   ComWrt (comPort, buffout, 8); //�������  
			*/
			
			GetCtrlVal(HPSPanelHandle, HPSPANEL_MOTOR_RING_2, &state);//��ȡ��ǰ����ѹ�� 
			
			if(state > 0)
			{
				control_word = control_word & 0xFD00;    
				control_word = control_word | 0x0400;
				SetCtrlVal (HPSPANEL, HPSPANEL_MOTOR_RING_2, !state); 
			}
			else
			{
				control_word = control_word & 0xFB00;    
				control_word = control_word | 0x0200;
				SetCtrlVal(HPSPANEL, HPSPANEL_MOTOR_RING_2, !state); 
			}
			
			send_reg[2] = control_word;
		
		break;
	}
	return 0;
}

int CVICALLBACK cool (int panel, int control, int event,
		void *callbackData, int eventData1, int eventData2)
{
	unsigned char state;
	switch (event)
	{
			/*
		case EVENT_LEFT_DOUBLE_CLICK:
		case EVENT_LEFT_CLICK:
			//GetCtrlVal (HPSPANEL, HPSPANEL_COOL_RING, &state);
		//	SetCtrlVal (HPSPANEL, HPSPANEL_COOL_RING, !state); 
			buffout[2] = 0x11;
	        buffout[3] = 0x04;
	        buffout[4] = 0x04;
			buffout[5] = buffout[2]^buffout[3]^buffout[4];
			
			
			ComWrt (comPort, buffout, 8); //������ȴ
			*/
			break;
	}
	return 0;
}

int CVICALLBACK source_on (int panel, int control, int event,
		void *callbackData, int eventData1, int eventData2)
{
	unsigned char state;
	switch (event)
	{
			/*
		case EVENT_LEFT_DOUBLE_CLICK:
		case EVENT_LEFT_CLICK:
		//	GetCtrlVal (HPSPANEL, HPSPANEL_SOURCE_RING, &state);
		//	SetCtrlVal (HPSPANEL, HPSPANEL_SOURCE_RING, !state);
			buffout[2] = 0x11;
	        buffout[3] = 0x02;
	        buffout[4] = 0x02;
			buffout[5] = buffout[2]^buffout[3]^buffout[4];
			
			ComWrt (comPort, buffout, 8); //��Դ��ѹ
			*/
			break;
	}
	return 0;
}

int CVICALLBACK load_on (int panel, int control, int event,
		void *callbackData, int eventData1, int eventData2)
{
	unsigned char state;
	switch (event)
	{
			/*
		case EVENT_LEFT_DOUBLE_CLICK:
		case EVENT_LEFT_CLICK:
			GetCtrlVal (HPSPanelHandle, HPSPANEL_LOAD_RING, &state);
		//	SetCtrlVal (HPSPANEL, HPSPANEL_LOAD_RING, !state);
			buffout[2] = 0x11;
	        buffout[3] = 0x03;
	        buffout[4] = 0x03;
			buffout[5] = buffout[2]^buffout[3]^buffout[4];
			
			
			ComWrt (comPort, buffout, 8); //���ع�ѹ
			*/
			break;
	}
	return 0;
}

int CVICALLBACK quit (int panel, int control, int event,
		void *callbackData, int eventData1, int eventData2)
{
	switch (event)
	{
		case EVENT_COMMIT:
			HidePanel(HPSPanelHandle);
			QuitUserInterface(0);  
			break;
	}
	return 0;
}

int CVICALLBACK HPStimer (int panel, int control, int event,
		void *callbackData, int eventData1, int eventData2)
{
	
	int i;
	int rc;
	char cbit;

	switch (event)
	{
		case EVENT_TIMER_TICK:
			if(mb != NULL)
			{
				/*
				rc = modbus_read_registers(mb, 19, 6, tag_reg);
				//rc = modbus_receive(mb, &req);
				if (rc == -1) { 
			         sprintf(error_str, "%s\n", modbus_strerror(errno));
					 //MessagePopup("����", error_str);
					 rc = 0; 
				}  
				else
				{
					source_p= ((float)tag_reg[1])/ 27648.0 *21;
					SetCtrlVal(HPSPanelHandle, HPSPANEL_SYS_P_DIAL, source_p);
				}
				*/ 

				rc =  modbus_receive(mb, query);
				if (rc >= 0)
				{
					/*rc is the query size*/
					//modbus_reply(mb, query, rc, mb_mapping);
					
					offset = modbus_get_header_length(mb);
				
					 switch(query[offset])
			         {
			           case 0x03:      
						    /* �����ּĴ��� */
			                /* ������յ�����,��Ҫ���͵����ݷ���mb_mapping�Ĵ�������*/
			                for(i = 0; i < 3; i++)
			                {
			                    mb_mapping->tab_registers[19+i] = send_reg[i];
			                }
			                 /* ��Ӧ */
			                modbus_reply(mb, query, rc, mb_mapping);
			                break;
			            case 0x10:      
							/* д����Ĵ��� */
			                /* ��Ӧ */
			                modbus_reply(mb, query, rc, mb_mapping);
			                for(i = 0; i < 6; i++)
			                 {
			                    tag_reg[i] = mb_mapping->tab_registers[i];
			                 }
			               break;
			             default:
			                 break;
			        }
					
				}
				else
				{
					//connection closed by the client or error
					//MessagePopup("����", "Connection Closed");
				}
				
				sys_temp= ((float)tag_reg[0]-5530)/ (27648.0-5530.0) *200-50;	   //�¶ȷ�Χ
				SetCtrlVal(HPSPanelHandle, HPSPANEL_TEMP_METER, sys_temp);
				
				source_p1= ((float)tag_reg[1]-5530)/  (27648.0-5530.0) *40;
				SetCtrlVal(HPSPanelHandle, HPSPANEL_SRC1_P_GAUGE, source_p1);
				source_p2= ((float)tag_reg[2]-5530)/  (27648.0-5530.0) *40;
				SetCtrlVal(HPSPanelHandle, HPSPANEL_SRC2_P_GAUGE, source_p2);
				using_p= ((float)tag_reg[3]-5530)/  (27648.0-5530.0) *40;
				SetCtrlVal(HPSPanelHandle, HPSPANEL_LOAD_P_GAUGE, using_p);
				return_p= ((float)tag_reg[4]-5530)/  (27648.0-5530.0) *6;
				SetCtrlVal(HPSPanelHandle, HPSPANEL_TANK_P_METER, return_p);
				
				cbit = tag_reg[5] &0x01<<8;  //����1����
				SetCtrlVal(HPSPanelHandle, HPSPANEL_SRC1_P_ALARM_LED, cbit);
				
				cbit = tag_reg[5] &0x01<<9;  //����2����
				SetCtrlVal(HPSPanelHandle, HPSPANEL_SRC2_P_ALARM_LED, cbit);
				
				cbit = tag_reg[5] &0x01<<10;  //����1ѹ�Ͷ���
				SetCtrlVal(HPSPanelHandle, HPSPANEL_SRC1_PACKER_LED, cbit);
				
				cbit = tag_reg[5] &0x01<<11;  //����2ѹ�Ͷ���
				SetCtrlVal(HPSPanelHandle, HPSPANEL_SRC2_PACKER_LED, cbit);
				
				cbit = tag_reg[5] &0x01<<12;  //���Ͷ�������
				SetCtrlVal(HPSPanelHandle, HPSPANEL_RETURN_P_ALARM_LED, cbit);
				
				cbit = tag_reg[5] &0x01<<13;  //���¸߱���
				SetCtrlVal(HPSPanelHandle, HPSPANEL_HT_ALARM_LED, cbit);
				
				cbit = tag_reg[5] &0x01<<14;  //���µͱ���
				SetCtrlVal(HPSPanelHandle, HPSPANEL_LT_ALARM_LED, cbit);
				
				cbit = tag_reg[5] &0x01<<15;  //Һλ�߱���
				SetCtrlVal(HPSPanelHandle, HPSPANEL_HLEVEL_ALARM_LED, cbit);
				
				cbit = tag_reg[5] &0x01<<0;  //Һλ�ͱ���
				SetCtrlVal(HPSPanelHandle, HPSPANEL_LLEVEL_ALARM_LED, cbit);
				
				cbit = tag_reg[5] &0x01<<3;  //Զ�̼�ͣ
				SetCtrlVal(HPSPanelHandle, HPSPANEL_REMOTE_STOP_LED, cbit);
				
				cbit = tag_reg[5] &0x01<<4;  //����1���з���
				SetCtrlVal(HPSPanelHandle, HPSPANEL_SRC1_P_RUN_LED, cbit);
				//SetCtrlVal (HPSPANEL, HPSPANEL_MOTOR_RING, cbit);    
			
				
				cbit = tag_reg[5] &0x01<<5;  //����2���з���
				SetCtrlVal(HPSPanelHandle, HPSPANEL_SRC2_P_RUN_LED, cbit);
				//SetCtrlVal (HPSPANEL, HPSPANEL_MOTOR_RING_2, cbit);  
				
				modbus_flush(mb);//
			}
			
			/*
			if(comState >= 0)
			{
				
				InQLength = GetInQLen (comPort);
				if(InQLength>512)
		            InQLength = 512;
				ComRd (comPort, buffin, InQLength);
				for(i = 0; i < InQLength-1;)
				{
					if(buffin[i] == 0xDD && buffin[i+1] == 0xDD) //��ͷ��ȷ
					{
						if(i+7 < InQLength) //����������
						{
							checkSum = buffin[i+2]^buffin[i+3]^buffin[i+4];
					
							if(checkSum == buffin[i+5]) //������ȷ
							{	
								if ((buffin[i+6] == 0xCC) && (buffin[i+7] == 0xCC))		 //��β��ȷ
							   {
								comValue= (buffin[i+3]<<8)+buffin[i+4];	 //�����ݰ�����comValue
								
								if(buffin[i+2] == 0x11) //�Ƿ�ϵͳ״̬��
								{
									if((comValue & 0x0001)!=0 )//�Ƿ�����
									{
										
										GetCtrlVal (HPSPANEL, HPSPANEL_MOTOR_RING, &start_sf);
										SetCtrlVal (HPSPANEL, HPSPANEL_MOTOR_RING, MotorON);
										if (!start_sf)
											InsertTextBoxLine (HPSPANEL, HPSPANEL_STATES_TEXTBOX, -1, "���������"); 
										
									}
									if ((comValue & 0x0001)==0 )
									{  
										
										GetCtrlVal (HPSPANEL, HPSPANEL_MOTOR_RING, &start_sf);
										SetCtrlVal (HPSPANEL, HPSPANEL_MOTOR_RING, !MotorON);
										if (start_sf)
											InsertTextBoxLine (HPSPANEL, HPSPANEL_STATES_TEXTBOX, -1, "���ֹͣ��");
										
									}
									if((comValue & 0x0002)!=0 )//�Ƿ��Դ��ѹ
									{	
										
										GetCtrlVal (HPSPANEL, HPSPANEL_SOURCE_RING, &source_sf);
										SetCtrlVal (HPSPANEL, HPSPANEL_SOURCE_RING, SourceON);  
										if (!source_sf)	 
											InsertTextBoxLine (HPSPANEL, HPSPANEL_STATES_TEXTBOX, -1, "��Դ��ѹ��"); 
										
									}
									if((comValue & 0x0002)==0 ) 
									{	
										
										GetCtrlVal (HPSPANEL, HPSPANEL_SOURCE_RING, &source_sf);
										SetCtrlVal (HPSPANEL, HPSPANEL_SOURCE_RING, !SourceON);  
										if (source_sf)	 
											InsertTextBoxLine (HPSPANEL, HPSPANEL_STATES_TEXTBOX, -1, "��Դж�ɣ�");
										
									}
									if((comValue & 0x0004)!=0 )//�Ƿ��ع�ѹ
									{	
									
										GetCtrlVal (HPSPANEL, HPSPANEL_LOAD_RING, &load_sf);
										SetCtrlVal (HPSPANEL, HPSPANEL_LOAD_RING, LoadON);  
										if (!load_sf)	 
											InsertTextBoxLine (HPSPANEL, HPSPANEL_STATES_TEXTBOX, -1, "���ع�ѹ��"); 
										
									}
									if((comValue & 0x0004)==0 )
									{	
										
										GetCtrlVal (HPSPANEL, HPSPANEL_LOAD_RING, &load_sf);
										SetCtrlVal (HPSPANEL, HPSPANEL_LOAD_RING, !LoadON);  
										if (load_sf)	 
											InsertTextBoxLine (HPSPANEL, HPSPANEL_STATES_TEXTBOX, -1, "����ж�ɣ�"); 
										
								    }
									if((comValue & 0x0008)!=0 )//�Ƿ���ȴ
									{
										GetCtrlVal (HPSPANEL, HPSPANEL_COOL_RING, &cool_sf);
										SetCtrlVal (HPSPANEL, HPSPANEL_COOL_RING, CoolON);  
										if (!cool_sf)
											 InsertTextBoxLine (HPSPANEL, HPSPANEL_STATES_TEXTBOX, -1, "������ȴ��"); 
										 
									}
									if((comValue & 0x0008)==0 ) 
									{
										GetCtrlVal (HPSPANEL, HPSPANEL_COOL_RING, &cool_sf);
										SetCtrlVal (HPSPANEL, HPSPANEL_COOL_RING, !CoolON);  
										if (cool_sf)
											 InsertTextBoxLine (HPSPANEL, HPSPANEL_STATES_TEXTBOX, -1, "ֹͣ��ȴ��"); 
										 
									}
									
									if((comValue & 0x0100)!=0 )//�Ƿ��ͱ�ѹ������
									{
										GetCtrlVal (HPSPANEL, HPSPANEL_SRC_P_ALARM_LED, &pump_sf);
										SetCtrlVal(HPSPANEL, HPSPANEL_SRC_P_ALARM_LED, 1);
										if (!pump_sf) 
										 InsertTextBoxLine (HPSPANEL, HPSPANEL_STATES_TEXTBOX, -1, "�ͱ�ѹ��ƫ�ߣ�����Ҫ�����ͱ�ж�ɣ�");
									}
									else 
									{
										 GetCtrlVal (HPSPANEL, HPSPANEL_SRC_P_ALARM_LED, &pump_sf); 
										 SetCtrlVal(HPSPANEL, HPSPANEL_SRC_P_ALARM_LED, 0);
										 if (pump_sf)
											 InsertTextBoxLine (HPSPANEL, HPSPANEL_STATES_TEXTBOX, -1, "�ͱ�ѹ���ָ����������£�"); 
										
									}
									
									if((comValue & 0x0200)!=0)//�Ƿ�������ѹ������
									{
										GetCtrlVal (HPSPANEL, HPSPANEL_ACCU_P_ALARM_LED, &accu_sf); 
										SetCtrlVal(HPSPANEL, HPSPANEL_ACCU_P_ALARM_LED, 1);
										if (!accu_sf)
										 InsertTextBoxLine (HPSPANEL, HPSPANEL_STATES_TEXTBOX, -1, "������ѹ��ƫ�ߣ�����Ҫ����������ж�ɣ�");
									}
									else 
									{
										GetCtrlVal (HPSPANEL, HPSPANEL_ACCU_P_ALARM_LED, &accu_sf); 
										SetCtrlVal(HPSPANEL, HPSPANEL_ACCU_P_ALARM_LED, 0);
										if (accu_sf)
											InsertTextBoxLine (HPSPANEL, HPSPANEL_STATES_TEXTBOX, -1, "������ѹ���ָ����������£�"); 
											
									}
									 
									if((comValue & 0x0400)!=0)//�Ƿ���ѹ������
									{
										GetCtrlVal (HPSPANEL, HPSPANEL_USING_P_ALARM_LED, &using_sf);  
										SetCtrlVal(HPSPANEL, HPSPANEL_USING_P_ALARM_LED, 1);
										if(!using_sf)
										 InsertTextBoxLine (HPSPANEL, HPSPANEL_STATES_TEXTBOX, -1, "����ѹ��ƫ�ߣ�����Ҫ���и���ж�ɣ�");
								
									}
									else 
									{
										GetCtrlVal (HPSPANEL, HPSPANEL_USING_P_ALARM_LED, &using_sf);  
										SetCtrlVal(HPSPANEL, HPSPANEL_USING_P_ALARM_LED, 0);
										if (using_sf)
											InsertTextBoxLine (HPSPANEL, HPSPANEL_STATES_TEXTBOX, -1, "����ѹ���ָ����������£�"); 
									}
									
									if((comValue & 0x0800)!=0)//�Ƿ��¶ȱ���
									{
										GetCtrlVal (HPSPANEL, HPSPANEL_TEMP_ALARM_LED, &temp_sf); 
										SetCtrlVal(HPSPANEL, HPSPANEL_TEMP_ALARM_LED, 1);
										if (!temp_sf)
										 InsertTextBoxLine (HPSPANEL, HPSPANEL_STATES_TEXTBOX, -1, "�¶�ƫ�ߣ���������ȴϵͳ��");
										
									}
									else 
									{																										  
										GetCtrlVal (HPSPANEL, HPSPANEL_TEMP_ALARM_LED, &temp_sf); 
										SetCtrlVal(HPSPANEL, HPSPANEL_TEMP_ALARM_LED, 0);
										if(temp_sf)
											 InsertTextBoxLine (HPSPANEL, HPSPANEL_STATES_TEXTBOX, -1, "�¶Ȼָ����������£�"); 
										 
									}
									 
									if((comValue & 0x1000)!=0)//Һλ����
								    {
										GetCtrlVal (HPSPANEL, HPSPANEL_LEVEL_ALARM_LED, &level_sf); 
										SetCtrlVal(HPSPANEL, HPSPANEL_LEVEL_ALARM_LED, 1);
										if(!level_sf)
										 InsertTextBoxLine (HPSPANEL, HPSPANEL_STATES_TEXTBOX, -1, "Һλƫ�ͣ���ע��Һѹ�ͣ�");
									}
									else 
									{
										GetCtrlVal (HPSPANEL, HPSPANEL_LEVEL_ALARM_LED, &level_sf);  
										SetCtrlVal(HPSPANEL, HPSPANEL_LEVEL_ALARM_LED, 0);
										 if(level_sf)
										 InsertTextBoxLine (HPSPANEL, HPSPANEL_STATES_TEXTBOX, -1, "Һλ�ָ�����ˮƽ��");
									}
									 
									if((comValue & 0x2000)!=0)//��ѹ������Ⱦ����
									{
										GetCtrlVal (HPSPANEL, HPSPANEL_HP_ALARM__LED, &HF_sf);  
										SetCtrlVal(HPSPANEL, HPSPANEL_HP_ALARM__LED, 1);
										if(!HF_sf)
										 InsertTextBoxLine (HPSPANEL, HPSPANEL_STATES_TEXTBOX, -1, "��ѹ���˶���������������о��");
									}
									else 
									{
										GetCtrlVal (HPSPANEL, HPSPANEL_HP_ALARM__LED, &HF_sf);   
										SetCtrlVal(HPSPANEL, HPSPANEL_HP_ALARM__LED, 0);
										if(HF_sf)
											InsertTextBoxLine (HPSPANEL, HPSPANEL_STATES_TEXTBOX, -1, "��ѹ���˻ָ�������");
											
									}
									
									if((comValue & 0x4000)!=0)//��ѹ������Ⱦ����
									{
										GetCtrlVal (HPSPANEL, HPSPANEL_LP_ALARM_LED, &LF_sf); 
										SetCtrlVal(HPSPANEL, HPSPANEL_LP_ALARM_LED, 1);
										if(!LF_sf)
										 InsertTextBoxLine (HPSPANEL, HPSPANEL_STATES_TEXTBOX, -1, "��ѹ���˶���������������о��");
									}
									else 
									{
										GetCtrlVal (HPSPANEL, HPSPANEL_LP_ALARM_LED, &LF_sf); 
										SetCtrlVal(HPSPANEL, HPSPANEL_LP_ALARM_LED, 0);
										if(LF_sf)
										 InsertTextBoxLine (HPSPANEL, HPSPANEL_STATES_TEXTBOX, -1, "��ѹ���˶»ָ�������");
									}
								}
								
								else if(buffin[i+2] == 0x22)//�Ƿ��г̿��غ�Զ�̿�����Ч��־
								{
								   	
									if((buffin[i+3] & 0x01)!=0)//�г̿����Ƿ���Ч
									{
										GetCtrlVal (HPSPANEL, HPSPANEL_TRAVEL_SW_LED, &travel_sf);  
										SetCtrlVal(HPSPANEL, HPSPANEL_TRAVEL_SW_LED, 1);
										if(!travel_sf)
										 InsertTextBoxLine (HPSPANEL, HPSPANEL_STATES_TEXTBOX, -1, "�г̿��رպϣ��޷�������");
									}
									else 
									{
										GetCtrlVal (HPSPANEL, HPSPANEL_TRAVEL_SW_LED, &travel_sf);  
										SetCtrlVal(HPSPANEL, HPSPANEL_TRAVEL_SW_LED, 0);
										if(travel_sf)
										InsertTextBoxLine (HPSPANEL, HPSPANEL_STATES_TEXTBOX, -1, "�г̿��ضϿ������Կ�����");
									}
									
									if((buffin[i+4] & 0x01)!=0)//Զ�̿����Ƿ��
									{
										GetCtrlVal (HPSPANEL, HPSPANEL_REMOTE_LED, &remote_sf);  
										SetCtrlVal(HPSPANEL, HPSPANEL_REMOTE_LED, 1);
										if(!remote_sf)
										InsertTextBoxLine (HPSPANEL, HPSPANEL_STATES_TEXTBOX, -1, "Զ�̿�����Ч��");
									}
									else 
								    {
										GetCtrlVal (HPSPANEL, HPSPANEL_REMOTE_LED, &remote_sf); 
										SetCtrlVal(HPSPANEL, HPSPANEL_REMOTE_LED, 0);
										if (remote_sf)
										InsertTextBoxLine (HPSPANEL, HPSPANEL_STATES_TEXTBOX, -1, "Զ�̿�����Ч��");
									}
								}
								
								else if(buffin[i+2] == 0x33)//�Ƿ�ʹ��ѹ������
								{
									 using_p=(float)comValue/1023*40;
									 SetCtrlVal (HPSPANEL, HPSPANEL_LOAD_P_GAUGE, using_p);
								}

								else if(buffin[i+2] == 0x44)//�Ƿ���Һ�¶� 
								{
									 sys_temp=(float)comValue/1023*120-20;
									 SetCtrlVal (HPSPANEL, HPSPANEL_TEMP_METER, sys_temp);
								}
								else if(buffin[i+2] == 0x55)//�Ƿ��Դѹ�� 
								{
									 source_p=(float)comValue/1023*40;
									 SetCtrlVal (HPSPANEL, HPSPANEL_SRC_P_GAUGE, source_p);
								}
								else if(buffin[i+2] == 0x66)//�Ƿ�������ѹ�� 
								{
									accu_p =(float)comValue/1023*40; 
									SetCtrlVal (HPSPANEL, HPSPANEL_ACCU_P_GAUGE, accu_p);
								}
								else if(buffin[i+2] == 0x77)//�Ƿ�����Һλ 
								{
									sys_level=(float)comValue/1023;
									SetCtrlVal (HPSPANEL, HPSPANEL_LEVEL_METER, sys_level);
								}
								
								GetCtrlVal(HPSPANEL, HPSPANEL_LOAD_P_GAUGE,&fOilPress);
								GetCtrlVal(HPSPANEL, HPSPANEL_TEMP_METER,&fOilTemp);
								GetCtrlVal(HPSPANEL, HPSPANEL_TEMP_ALARM_LED,&iTempWarn);
								GetCtrlVal(HPSPANEL, HPSPANEL_USING_P_ALARM_LED,&iPressWarn);
							   }
							}
						     else //���ݲ���ȷ
							 {
								InsertTextBoxLine (HPSPANEL, HPSPANEL_STATES_TEXTBOX, 0, "״̬���ݴ����޷����в���");
							 }  
						   
							i = i+7;//����һ����
						 }
						else
						{
							i = InQLength-1; //��ͷ֮��û��6���ֽڣ�����
						}
					}
					else //��ͷ����ȷ������һ���ֽڼ���
					{
						i++;
					}
				}
				
			}
			else
				InsertTextBoxLine (HPSPANEL, HPSPANEL_STATES_TEXTBOX, 0, "���ڴ����޷����в��������ѯ�������ã�");
			*/
			break;
	}
	return 0;
}

int CVICALLBACK com_select (int panel, int control, int event,
		void *callbackData, int eventData1, int eventData2)
{
	switch (event)
	{
		case EVENT_COMMIT:
			
		   /*
		  GetCtrlVal(HPSPanelHandle, HPSPANEL_COM_RING, &comPort);
	      //���ô���
	      comState = OpenComConfig (comPort, "", 9600, 0, 8, 1, 512, 8);
	
	      if(comState < 0)
	      {
		    MessagePopup("����", "���ڴ򿪴������ѯ�������ã�");
	    	return -1;
	      }
	      InQLength = GetInQLen (comPort);
	      if(InQLength>512)
		  InQLength = 512;
	      ComRd (comPort, buffin, InQLength);   
		   
		   */
	      //DisplayPanel(HPSPANEL);
	      //RunUserInterface();
		break;
	}
	return 0;
}

int CVICALLBACK clear_box (int panel, int control, int event,
		void *callbackData, int eventData1, int eventData2)
{
	switch (event)
	{
		case EVENT_COMMIT:
			ResetTextBox (HPSPANEL, HPSPANEL_STATES_TEXTBOX, "");
			break;
	}
	return 0;
}

int CVICALLBACK valve_ctrl_select (int panel, int control, int event,
		void *callbackData, int eventData1, int eventData2)
{
	switch (event)
	{
		case EVENT_COMMIT:
			/*
			GetCtrlVal(HPSPANEL, HPSPANEL_VALVE_SEL_RING, &valve_ctrl);
			buffout[2] = 0x03;
	        buffout[3] = 0x00;
	        buffout[4] = valve_ctrl;
			buffout[5] = buffout[2]^buffout[3]^buffout[4];
			
			
			ComWrt (comPort, buffout, 8); 
			*/
			
			break;
	}
	return 0;
}

int CVICALLBACK reset (int panel, int control, int event,
		void *callbackData, int eventData1, int eventData2)
{
	switch (event)
	{
		case EVENT_COMMIT:
			/*
			buffout[2] = 0x11;
	        buffout[3] = 0x07;
	        buffout[4] = 0x07;
			buffout[5] = buffout[2]^buffout[3]^buffout[4];
			
			
			ComWrt (comPort, buffout, 8); 
			*/
			break;
	}
	return 0;
}

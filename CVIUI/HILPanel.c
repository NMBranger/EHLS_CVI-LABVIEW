#include "HPS_Controller.h"
#include <userint.h>
#include "HILPanel.h"
#include "ExtraVar.h"
#include <udpsupp.h>
#include <tcpsupp.h>
#include "Functions.h"
#include <formatio.h>
#include <utility.h>
#include <ansi_c.h>

#define ANY_CHART_TRACE         1
#define TORQUE_CHART_TRACE      5
#define PROPORTION_TRACE        4
#define VMIC_TEST_TRACE			9

extern char DftPathSign; //Ĭ��·����־λ
extern char pathpass[];  //·���������
extern char originalkey[] ;
extern int keyerr = 1;
static int HILPanelHandle;

 /*
#define ANY_CHART_TRACE              1
#define FRE_SCAN_TEST     	         2
#define ANALOGY_FRE_SCAN_TEST     	 9   
  */
#define  READBUFFERLEN	  1024    

TESTDATA ReadDataMessage;
//TESTDATANEW  ReadDataMessageBuff[READBUFFERLEN];  //�������µķ������ݷ�ʽ������16λ���ֽ���
//int BufferWriteIndex=0;
//int BufferReadIndex=0;
 
MessageHead sendMessageHead; 
MessageHead readMessageHead;  
CommandMessage sendCommandMessage; 
CommandMessage readCommandMessage; 

extern unsigned int gTCPConnection;
//static unsigned int 	gCommandConnection				= TCP_INVALID_CONNECTION;  
static int 				gLock							= 0;

static int CVICALLBACK TCPDataCallback (unsigned handle, int xType, int errCode, void *cbData);   
int transdatasizeH = 42;

static unsigned int		gUDPChannel			= 0;

#define UDP_PORT   20000
#define MAX_IP4_STRING_LENGTH		15
#define MAX_CLIENT_NAME_LENGTH		30

int CVICALLBACK	UDPChatCallback (unsigned channel, int eventType, int errCode, void *callbackData);   
typedef unsigned char IPAddr[MAX_IP4_STRING_LENGTH + 1]; 
typedef unsigned char ClientName[MAX_CLIENT_NAME_LENGTH + 1]; 

//��Դ����������
extern uint16_t send_reg[32]; 
extern uint16_t tag_reg[32];   
extern unsigned short control_word;  
float source_p1; 
float source_p2;
float sys_temp;

extern int CVICALLBACK HPStimer(int panel, int control, int event, void *callbackData, int eventData1, int eventData2);



typedef struct
{
	short command;
	short commandvalue1;
	short commandvalue2;   
	short reserve;
	short checksum;
}UDPRECMessage;

UDPRECMessage udpmsg;

typedef struct
{
	short command;
	short PressureStateValue1;
	short PressureStateValue2; 
	short TemperatureValue;
	short SourceState;
	short LoadState;
	short checksum;
}UDPSENDMessage;

UDPSENDMessage udpsendmsg; 
#define SEND_COMPUTER_ADDR		"192.168.1.100"	



//HIL����ʼ��
int HILPanelInit(void)
{
	if ((HILPanelHandle = LoadPanel (0, "HILPanel.uir", HILPNL)) < 0)
	  return -1;
	
	if (ConnectToTCPServer (&gTCPConnection, HOST_PORT, HOST_ADDRESS, TCPDataCallback, 0, 2000) < 0)
	{
		//SetCtrlVal(runningpanel,RUNPANEL_MSGSTRING,"����ʵʱ����ʧ�ܣ���ȷ��ʵʱ�����Ѿ����У�����������������");	
		MessagePopup ("Error", "����ʵʱ����ʧ�ܣ���ȷ��ʵʱ�����Ѿ����У�����������������");    
		goto Error;
	}
	else
	{
		//SetCtrlVal(StaticPanelHandle,RUNPANEL_MSGSTRING,"ʵʱ��������������");	       
	}
	
	
		
	//HIL��ʼ��
	InitHILCtrlPara();
	InitHILDisIndex();   
	DisplayPanel (HILPanelHandle);
	
	//UDPԶ��ͨ��
	CreateUDPChannel(UDP_PORT, &gUDPChannel);
	SetUDPAttribute(gUDPChannel, ATTR_UDP_CALLBACK, UDPChatCallback);
	
	
	RunUserInterface ();
	

	DiscardPanel (HILPanelHandle);
	
	if (gUDPChannel)
		DisposeUDPChannel(gUDPChannel);



	return 1;
Error:
    /* error operation */
	return -1;
	

Done: 
	/* clean up */   
	if (gTCPConnection != TCP_INVALID_CONNECTION)
		DisconnectFromTCPServer (gTCPConnection);
}

void Hctrlparadown(void)		//��ɲ���������
{
	int i;
	double dTemp;
	GetPanelHandleFromTabPage (HILPanelHandle, HILPNL_TAB,1,&tabpanel1);
	
	//����ͨ����ȫ�����Ʋ���ѭ����ȡ
	for (i=0; i < SIDE_NO ; i++ )
	{
		GetPanelHandleFromTabPage(tabpanel1, PARA_TAB_CTRL_PARA_TAB,i,&tabpanel2);
		GetCtrlVal(tabpanel2,CH1_P_TAB_NUM_HIL_KTP,&dTemp);
		pExMem->ctrlPara[i+1].Ktp = dTemp;
		GetCtrlVal(tabpanel2,CH1_P_TAB_NUM_HIL_KTI,&dTemp);
		pExMem->ctrlPara[i+1].Kti = dTemp;
		GetCtrlVal(tabpanel2,CH1_P_TAB_NUM_HIL_KTD,&dTemp);
		pExMem->ctrlPara[i+1].Ktd = dTemp;
		GetCtrlVal(tabpanel2,CH1_P_TAB_NUM_HIL_FT1,&dTemp);
		pExMem->ctrlPara[i+1].Ft1 = dTemp;
		GetCtrlVal(tabpanel2,CH1_P_TAB_NUM_HIL_FT2,&dTemp);
		pExMem->ctrlPara[i+1].Ft2 = dTemp;
		GetCtrlVal(tabpanel2,CH1_P_TAB_NUM_HIL_FTK,&dTemp);
		pExMem->ctrlPara[i+1].Ftk = dTemp;

		GetCtrlVal(tabpanel2,CH1_P_TAB_NUM_HIL_KEP,&dTemp);
		pExMem->ctrlPara[i+1].Kep = dTemp;
		GetCtrlVal(tabpanel2,CH1_P_TAB_NUM_HIL_KED,&dTemp);
		pExMem->ctrlPara[i+1].Ked = dTemp;
		GetCtrlVal(tabpanel2,CH1_P_TAB_NUM_HIL_TED,&dTemp);
		pExMem->ctrlPara[i+1].Ted = dTemp;
		GetCtrlVal(tabpanel2,CH1_P_TAB_NUM_HIL_X1,&dTemp);
		pExMem->ctrlPara[i+1].X1 = dTemp;
		GetCtrlVal(tabpanel2,CH1_P_TAB_NUM_HIL_X2,&dTemp);
		pExMem->ctrlPara[i+1].X2 = dTemp;
		GetCtrlVal(tabpanel2,CH1_P_TAB_NUM_HIL_X3,&dTemp);
		pExMem->ctrlPara[i+1].X3 = dTemp;

	}
			

	
	//����ͨ���Ŀ��Ʋ���
	for (i=SIDE_NO; i < SIDE_NO+AXIS_NO ; i++ )
	{
		GetPanelHandleFromTabPage(tabpanel1, PARA_TAB_CTRL_PARA_TAB, i, &tabpanel2);
		GetCtrlVal(tabpanel2,CH1_A_TAB_NUM_HIL_KTP,&dTemp);
		pExMem->ctrlPara[i+1].Ktp = dTemp;
		GetCtrlVal(tabpanel2,CH1_A_TAB_NUM_HIL_KTI,&dTemp);
		pExMem->ctrlPara[i+1].Kti = dTemp;
		GetCtrlVal(tabpanel2,CH1_A_TAB_NUM_HIL_KTD,&dTemp);
		pExMem->ctrlPara[i+1].Ktd = dTemp;
		GetCtrlVal(tabpanel2,CH1_A_TAB_NUM_HIL_FT1,&dTemp);
		pExMem->ctrlPara[i+1].Ft1 = dTemp;
		GetCtrlVal(tabpanel2,CH1_A_TAB_NUM_HIL_FT2,&dTemp);
		pExMem->ctrlPara[i+1].Ft2 = dTemp;
		GetCtrlVal(tabpanel2,CH1_A_TAB_NUM_HIL_FTK,&dTemp);
		pExMem->ctrlPara[i+1].Ftk = dTemp;
	}
}	

void InitHILCtrlSet(void)				//��ɲ���������
{
	int i;
	GetPanelHandleFromTabPage (HILPanelHandle, HILPNL_TAB,1,&tabpanel1);  
	
	//ͨ��1�Ŀ��Ʋ���
	
	for (i=0; i<SIDE_NO; i++)
	{
		GetPanelHandleFromTabPage(tabpanel1, PARA_TAB_CTRL_PARA_TAB,i,&tabpanel2);
		SetCtrlVal(tabpanel2,CH1_P_TAB_NUM_HIL_KTP,pExMem->ctrlPara[i+1].Ktp);
		SetCtrlVal(tabpanel2,CH1_P_TAB_NUM_HIL_KTI,pExMem->ctrlPara[i+1].Kti);
		SetCtrlVal(tabpanel2,CH1_P_TAB_NUM_HIL_KTD,pExMem->ctrlPara[i+1].Ktd);
		SetCtrlVal(tabpanel2,CH1_P_TAB_NUM_HIL_FT1,pExMem->ctrlPara[i+1].Ft1);
		SetCtrlVal(tabpanel2,CH1_P_TAB_NUM_HIL_FT2,pExMem->ctrlPara[i+1].Ft2);
		SetCtrlVal(tabpanel2,CH1_P_TAB_NUM_HIL_FTK,pExMem->ctrlPara[i+1].Ftk);
		SetCtrlVal(tabpanel2,CH1_P_TAB_NUM_HIL_KEP,pExMem->ctrlPara[i+1].Kep);
		SetCtrlVal(tabpanel2,CH1_P_TAB_NUM_HIL_KED,pExMem->ctrlPara[i+1].Ked);
		SetCtrlVal(tabpanel2,CH1_P_TAB_NUM_HIL_TED,pExMem->ctrlPara[i+1].Ted);
		SetCtrlVal(tabpanel2,CH1_P_TAB_NUM_HIL_X1,pExMem->ctrlPara[i+1].X1);
		SetCtrlVal(tabpanel2,CH1_P_TAB_NUM_HIL_X2,pExMem->ctrlPara[i+1].X2);
		SetCtrlVal(tabpanel2,CH1_P_TAB_NUM_HIL_X3,pExMem->ctrlPara[i+1].X3);
	}
	
	

	for (i=SIDE_NO; i<SIDE_NO+AXIS_NO ; i++) 
	{
		//����1�Ŀ��Ʋ���
		GetPanelHandleFromTabPage(tabpanel1, PARA_TAB_CTRL_PARA_TAB,i,&tabpanel2);
		SetCtrlVal(tabpanel2,CH1_A_TAB_NUM_HIL_KTP,pExMem->ctrlPara[i+1].Ktp);
		SetCtrlVal(tabpanel2,CH1_A_TAB_NUM_HIL_KTI,pExMem->ctrlPara[i+1].Kti);
		SetCtrlVal(tabpanel2,CH1_A_TAB_NUM_HIL_KTD,pExMem->ctrlPara[i+1].Ktd);
		SetCtrlVal(tabpanel2,CH1_A_TAB_NUM_HIL_FT1,pExMem->ctrlPara[i+1].Ft1);
		SetCtrlVal(tabpanel2,CH1_A_TAB_NUM_HIL_FT2,pExMem->ctrlPara[i+1].Ft2);
		SetCtrlVal(tabpanel2,CH1_A_TAB_NUM_HIL_FTK,pExMem->ctrlPara[i+1].Ftk);
	}

	
	
}

void InitHILTestPara(void)			 //����˲���������
{
	double dTemp;
	int tempInt;
	int i;
	
	//ͨ���Ĳ�������
	GetCtrlVal(HILPanelHandle, HILPNL_CH_TEST_TYPE, &tempInt);
	for (i=0; i<SIDE_NO+AXIS_NO ; i++)
	{
		pExMem->testPara[i+1].iTestType = tempInt;
	
	}
	GetCtrlVal(HILPanelHandle, HILPNL_CH_TEST_TIME, &dTemp);    
	for (i=0; i<SIDE_NO+AXIS_NO ; i++)
	{
		pExMem->testPara[i+1].dTestTime = dTemp;
	}

	
	GetPanelHandleFromTabPage (HILPanelHandle, HILPNL_TAB, 0, &tabpanel1);
	
	for (i=0 ;i < SIDE_NO + AXIS_NO; i++)
	{
		GetPanelHandleFromTabPage(tabpanel1, SET_TAB_CH_TAB,i,&tabpanel2);
	
		GetCtrlVal(tabpanel2, 7 ,&dTemp);
		pExMem->testPara[i+1].dAmp = dTemp;
	
		GetCtrlVal(tabpanel2, 6 ,&dTemp);
		pExMem->testPara[i+1].dOffset = dTemp;
	
		GetCtrlVal(tabpanel2, 5 ,&dTemp);    
		pExMem->testPara[i+1].dFreq = dTemp;  
		//GetCtrlVal(tabpanel2,CH1_TAB_CH_START_FREQ,&dTemp);    
		//pExMem->testPara[1].dFreq = dTemp; 
	
		GetCtrlVal(tabpanel2, 4 ,&dTemp);    
		pExMem->testPara[i+1].dStopFreq = dTemp;

		GetCtrlVal(tabpanel2, 2 ,&tempInt); 
		if(tempInt == 1)
		 {
			pExMem->testPara[i+1].iEnable = 1;
		 	pExMem->testPara[i+1].iStatus = 2;  		  //1��ʾλ��ͨ����2��ʾ����ͨ��
		 }
		 GetCtrlVal(tabpanel2, 11 ,&tempInt); 
		pExMem->testPara[i+1].iWave = tempInt; 
	}

}

void HILSetScreenCycNum(void)
{
	double dfreq;	   //Ԥ��Ƶ��
	double dring_val;  //��������
	long N;   //ÿ����ʾ����
	int i,ringindex;
	
	for(i=1;i<9;i++)
	{
		if(pExMem->testPara[i].iStatus && (chart1_ch == i))
		{
			N = (long)(2000 / (pExMem->testPara[i].dFreq) * CYCLE_NUM_PER_SCREEN);  //��Ļ��ʾ�������㹫ʽ
			if(N >= 3 && N <= 10000)
			{
				SetCtrlAttribute(HILPanelHandle,HILPNL_STRIPCHART_1,ATTR_POINTS_PER_SCREEN,N);
				SetCtrlAttribute(HILPanelHandle,HILPNL_STRIPCHART_2,ATTR_POINTS_PER_SCREEN,N);
				SetCtrlAttribute(HILPanelHandle,HILPNL_STRIPCHART_3,ATTR_POINTS_PER_SCREEN,N);
				SetCtrlAttribute(HILPanelHandle,HILPNL_STRIPCHART_4,ATTR_POINTS_PER_SCREEN,N);
			}
			 /*
			for(ringindex = 0;ringindex < 11;ringindex++)  //����ֵ������Χ������y����
			{
				//GetValueFromIndex(HILPanelHandle,HILPNL_CHART1_YRANGE_RING,ringindex,&dring_val);
				if(pExMem->testPara[i].dAmp < dring_val)
				{
					SetCtrlIndex(HILPanelHandle,HILPNL_CHART1_YRANGE_RING,ringindex);
					SetCtrlIndex(HILPanelHandle,HILPNL_CHART2_YRANGE_RING,ringindex);
					SetAxisRange(HILPanelHandle,HILPNL_STRIPCHART_1,VAL_NO_CHANGE,0.0,1.0,VAL_MANUAL,-dring_val,dring_val);
					SetAxisRange(HILPanelHandle,HILPNL_STRIPCHART_2,VAL_NO_CHANGE,0.0,1.0,VAL_MANUAL,-dring_val,dring_val);
					break;
				}
			}
			 */
		}
	}
}

void InitHILDisIndex(void)
{
	char tempLabel[64];
	//��ʼ����ʾ����
	//tempLabel = " ";
	
	GetCtrlVal(HILPanelHandle,HILPNL_CHART1_CH_RING,&chart1_ch); 
	/*
	GetCtrlVal(HILPanelHandle,HILPNL_CHART1_CONTENT_RING,&chart1_content);
	strcpy(tempLabel,""); 
	strcat(tempLabel,contentLabel[chart1_content]);
	SetCtrlAttribute(HILPanelHandle,HILPNL_STRIPCHART_1,ATTR_LABEL_TEXT,tempLabel);
	SetCtrlAttribute(HILPanelHandle,HILPNL_STRIPCHART_1,ATTR_LABEL_JUSTIFY,VAL_LEFT_JUSTIFIED); 
	*/
	
	GetCtrlVal(HILPanelHandle,HILPNL_CHART2_CH_RING,&chart2_ch);
	GetCtrlVal(HILPanelHandle,HILPNL_CHART2_CONTENT_RING,&chart2_content);
	strcpy(tempLabel,""); 
	strcat(tempLabel,contentLabel[chart2_content]);
	SetCtrlAttribute(HILPanelHandle,HILPNL_STRIPCHART_2,ATTR_LABEL_TEXT,tempLabel);
	//SetCtrlAttribute(HILPanelHandle,HILPNL_STRIPCHART_2,ATTR_LABEL_JUSTIFY,VAL_LEFT_JUSTIFIED); 

	GetCtrlVal(HILPanelHandle,HILPNL_CHART3_CH_RING,&chart3_ch); 
	/*
	GetCtrlVal(HILPanelHandle,HILPNL_CHART3_CONTENT_RING,&chart3_content);
	strcpy(tempLabel,""); 
	strcat(tempLabel,contentLabel[chart3_content]);
	SetCtrlAttribute(HILPanelHandle,HILPNL_STRIPCHART_3,ATTR_LABEL_TEXT,tempLabel);
	SetCtrlAttribute(HILPanelHandle,HILPNL_STRIPCHART_3,ATTR_LABEL_JUSTIFY,VAL_LEFT_JUSTIFIED); 
	*/

	GetCtrlVal(HILPanelHandle,HILPNL_CHART4_CH_RING,&chart4_ch);
	GetCtrlVal(HILPanelHandle,HILPNL_CHART4_CONTENT_RING,&chart4_content);
	strcpy(tempLabel,""); 
	strcat(tempLabel,contentLabel[chart4_content==5?chart4_content+1:chart4_content]);
	SetCtrlAttribute(HILPanelHandle,HILPNL_STRIPCHART_4,ATTR_LABEL_TEXT,tempLabel);
	//SetCtrlAttribute(HILPanelHandle,HILPNL_STRIPCHART_4,ATTR_LABEL_JUSTIFY,VAL_LEFT_JUSTIFIED); 
}

//��ʼ�����Ʋ���
void InitHILCtrlPara(void)	   //����������
{
	int i;
	double dTemp;
	char sectionNam[512];
	//��ȡini�ļ����
	inihandle = Ini_New(0);
	//��ȡini�ļ�
	//GetWindowsDirs (windowsDirectory, systemDirectory);
	//strcpy( pathname ,windowsDirectory);
	
	GetDir (currentDir);
	strcpy( pathname ,currentDir);
	strcat(pathname,"\\HILPara.ini");
	
	inierr = Ini_ReadFromFile(inihandle, pathname); 
	if( inierr < 0)
	{
		sprintf(msgBuff,"�򿪲����ļ�����err = %d",inierr);
		MessagePopup ("Err",msgBuff);  
		//���ļ�ʧ��
	}
	else
	{
		for (i=0; i<SIDE_NO+AXIS_NO ; i++)
		{
			
			//ͨ��1 ----KTP/KTI/KTD
			strcpy(sectionNam,"0CHANNEL");
			sectionNam[0] = i+ '0'; 
			if(Ini_GetDouble (inihandle, sectionNam,"Ktp", &dTemp)>0)
			{
				pExMem->ctrlPara[i+1].Ktp = dTemp;
			}
			else 
			{
				 pExMem->ctrlPara[i+1].Ktp = 0.0;
			}
	
			if(Ini_GetDouble (inihandle, sectionNam,"Kti", &dTemp)>0)
			{
				pExMem->ctrlPara[i+1].Kti = dTemp;
			}
			else 
			{
				 pExMem->ctrlPara[i+1].Kti = 0.0;
			}
	
			if(Ini_GetDouble (inihandle, sectionNam,"Ktd", &dTemp)>0)
			{
				pExMem->ctrlPara[i+1].Ktd = dTemp;
			}
			else 
			{
				 pExMem->ctrlPara[i+1].Ktd = 0.0;
			}
			//����ģ������
			if(Ini_GetDouble (inihandle, sectionNam,"Ft1", &dTemp)>0)
			{
				pExMem->ctrlPara[i+1].Ft1 = dTemp;
			}
			else 
			{
				 pExMem->ctrlPara[i+1].Ft1 = 0.0;
			}
	
			if(Ini_GetDouble (inihandle, sectionNam,"Ft2", &dTemp)>0)
			{
				pExMem->ctrlPara[i+1].Ft2 = dTemp;
			}
			else 
			{
				 pExMem->ctrlPara[i+1].Ft2 = 0.0;
			}
	
			if(Ini_GetDouble (inihandle, sectionNam,"Ftk", &dTemp)>0)
			{
				pExMem->ctrlPara[i+1].Ftk = dTemp;
			}
			else 
			{
				 pExMem->ctrlPara[i+1].Ftk = 0.0;
			}
			//---------kep/ked/ted
			if(Ini_GetDouble (inihandle, sectionNam,"Kep", &dTemp)>0)
			{
				pExMem->ctrlPara[i+1].Kep = dTemp;
			}
			else 
			{
				 pExMem->ctrlPara[i+1].Kep = 0.0;
			}		
			if(Ini_GetDouble (inihandle, sectionNam,"Ked", &dTemp)>0)
			{
				pExMem->ctrlPara[i+1].Ked = dTemp;
			}
			else 
			{
				 pExMem->ctrlPara[i+1].Ked = 0.0;
			}
			if(Ini_GetDouble (inihandle, sectionNam,"Ted", &dTemp)>0)
			{
				pExMem->ctrlPara[i+1].Ted = dTemp;
			}
			else 
			{
				 pExMem->ctrlPara[i+1].Ted = 0.0;
			}
			//-------------x1/x2/x3
			if(Ini_GetDouble (inihandle, sectionNam,"X1", &dTemp)>0)
			{
				pExMem->ctrlPara[i+1].X1 = dTemp;
			}
			else 
			{
				 pExMem->ctrlPara[i+1].X1 = 0.0;
			}
			if(Ini_GetDouble (inihandle, sectionNam,"X2", &dTemp)>0)
			{
				pExMem->ctrlPara[i+1].X2 = dTemp;
			}
			else 
			{
				 pExMem->ctrlPara[i+1].X2 = 0.0;
			}
			if(Ini_GetDouble (inihandle, sectionNam,"X3", &dTemp)>0)
			{
				pExMem->ctrlPara[i+1].X3 = dTemp;
			}
			else 
			{
				 pExMem->ctrlPara[i+1].X3 = 0.0;
			}
		}
 

			//����ͨ��4
	//	if(Ini_GetDouble (inihandle, "CtrlPara","CH9_Ktp", &dTemp)>0)
//		{
//			pExMem->ctrlPara[9].Ktp = dTemp;
//		}
//		else 
//		{
//			 pExMem->ctrlPara[9].Ktp = 0.0;
//		}
	
	
	}
	
	InitHILCtrlSet();
}

//ʹ�ܿ��Ʋ�������
void LightHILParaSet(void)			  //���������������
{
	int i;
	GetPanelHandleFromTabPage (HILPanelHandle, HILPNL_TAB,1,&tabpanel1);
	
	for (i=0 ;i < SIDE_NO  ; i++)
	{
		GetPanelHandleFromTabPage(tabpanel1, PARA_TAB_CTRL_PARA_TAB,i,&tabpanel2);
		SetCtrlAttribute(tabpanel2, CH1_P_TAB_NUM_HIL_KTP ,ATTR_DIMMED,0);
		SetCtrlAttribute(tabpanel2,CH1_P_TAB_NUM_HIL_KTI,ATTR_DIMMED,0);
		SetCtrlAttribute(tabpanel2,CH1_P_TAB_NUM_HIL_KTD,ATTR_DIMMED,0);
		SetCtrlAttribute(tabpanel2,CH1_P_TAB_NUM_HIL_FT1,ATTR_DIMMED,0);
		SetCtrlAttribute(tabpanel2,CH1_P_TAB_NUM_HIL_FT2,ATTR_DIMMED,0);
		SetCtrlAttribute(tabpanel2,CH1_P_TAB_NUM_HIL_FTK,ATTR_DIMMED,0);
		SetCtrlAttribute(tabpanel2,CH1_P_TAB_NUM_HIL_KEP,ATTR_DIMMED,0);
		SetCtrlAttribute(tabpanel2,CH1_P_TAB_NUM_HIL_KED,ATTR_DIMMED,0);
		SetCtrlAttribute(tabpanel2,CH1_P_TAB_NUM_HIL_TED,ATTR_DIMMED,0);
		SetCtrlAttribute(tabpanel2,CH1_P_TAB_NUM_HIL_X1,ATTR_DIMMED,0);  
		SetCtrlAttribute(tabpanel2,CH1_P_TAB_NUM_HIL_X2,ATTR_DIMMED,0);  
		SetCtrlAttribute(tabpanel2,CH1_P_TAB_NUM_HIL_X3,ATTR_DIMMED,0);
	}
	
	for (i=SIDE_NO ;i < AXIS_NO+SIDE_NO ; i++)   
	{
		GetPanelHandleFromTabPage(tabpanel1, PARA_TAB_CTRL_PARA_TAB,i ,&tabpanel2);   
		SetCtrlAttribute(tabpanel2,CH4_A_TAB_NUM_HIL_KTP,ATTR_DIMMED,0);
		SetCtrlAttribute(tabpanel2,CH4_A_TAB_NUM_HIL_KTI,ATTR_DIMMED,0);
		SetCtrlAttribute(tabpanel2,CH4_A_TAB_NUM_HIL_KTD,ATTR_DIMMED,0);
		SetCtrlAttribute(tabpanel2,CH4_A_TAB_NUM_HIL_FT1,ATTR_DIMMED,0);
		SetCtrlAttribute(tabpanel2,CH4_A_TAB_NUM_HIL_FT2,ATTR_DIMMED,0);
		SetCtrlAttribute(tabpanel2,CH4_A_TAB_NUM_HIL_FTK,ATTR_DIMMED,0);
	}
	

}

////////////////////////////////////////These are Callback Functions
int CVICALLBACK HILPanelCallback (int panel, int event, void *callbackData,
		int eventData1, int eventData2)
{
	switch (event)
	{
		case EVENT_GOT_FOCUS:

			break;
		case EVENT_LOST_FOCUS:

			break;
		case EVENT_CLOSE:

			break;
	}
	return 0;
}

int CVICALLBACK HILYRangeChangeCallback (int panel, int control, int event,
		void *callbackData, int eventData1, int eventData2)
{
	double dTemp;
	switch (event)
	{
		case EVENT_COMMIT:
			
			switch(control)
			{
				case  HILPNL_CHART1_YRANGE_RING:
					GetCtrlVal(panel,control,&dTemp);
					SetAxisRange(panel,HILPNL_STRIPCHART_1,VAL_NO_CHANGE,0.0,1.0,VAL_MANUAL,-dTemp,dTemp);
					break;
				case  HILPNL_CHART2_YRANGE_RING:
					GetCtrlVal(panel,control,&dTemp);
					SetAxisRange(panel,HILPNL_STRIPCHART_2,VAL_NO_CHANGE,0.0,1.0,VAL_MANUAL,-dTemp,dTemp);
					break;
				case  HILPNL_CHART3_YRANGE_RING:
					GetCtrlVal(panel,control,&dTemp);
					SetAxisRange(panel,HILPNL_STRIPCHART_3,VAL_NO_CHANGE,0.0,1.0,VAL_MANUAL,-dTemp,dTemp);
					break;
				case  HILPNL_CHART4_YRANGE_RING:
					GetCtrlVal(panel,control,&dTemp);
					SetAxisRange(panel,HILPNL_STRIPCHART_4,VAL_NO_CHANGE,0.0,1.0,VAL_MANUAL,-dTemp,dTemp);
					break;
			} 
			
			break;
	}
	return 0;
}

int CVICALLBACK HILSaveCallback (int panel, int control, int event,
		void *callbackData, int eventData1, int eventData2)
{
	switch (event)
	{
		case EVENT_COMMIT:
		strcpy(saveDir,"D:\\��������" );
		//err = MakeDir (saveDir);
		strcat(saveDir,"\\" );
		strcat(saveDir, DateStr());
		//err = MakeDir (saveDir);

		if(FileSelectPopup (saveDir, "*.dat", "*.dat", "���ݱ���", VAL_SAVE_BUTTON, 0, 1, 1, 0,savepath ) != 0)
		{
			savefilehandle = OpenFile (savepath , VAL_WRITE_ONLY, VAL_TRUNCATE, VAL_BINARY);
			WriteFile (savefilehandle, (char*)saveBuf, saveIndex * dataStructSize ); 
			CloseFile (savefilehandle);
		}

		break;
	}
	return 0;
}

int CVICALLBACK HILRecordCallback (int panel, int control, int event,
		void *callbackData, int eventData1, int eventData2)
{
	switch (event)
	{
		case EVENT_COMMIT:
			if(recording == 0)
			{
				recording = 1;
				saveIndex = 0;
				SetCtrlAttribute(panel,control,ATTR_LABEL_TEXT,"ֹͣ��¼");
			}
			else
			{
				recording = 0;
				SetCtrlAttribute(panel,control,ATTR_LABEL_TEXT,"��ʼ��¼");  
			}
			break;
	}
	return 0;
}

int CVICALLBACK HILDisplayChangeCallback (int panel, int control, int event,
		void *callbackData, int eventData1, int eventData2)
{
	switch (event)
	{
		case EVENT_COMMIT:
			InitHILDisIndex();         
			break;
	}
	return 0;
}

int CVICALLBACK AHILDisplayChangeCallback (int panel, int control, int event,
		void *callbackData, int eventData1, int eventData2)
{
	long N;
	int iTemp,ringindex;
	double dring_val;
	
	switch (event)
	{
		case EVENT_COMMIT:
			
			GetCtrlVal(panel,control,&iTemp);								//�����ĸ�strips��ʾͨ����ͬ
			SetCtrlVal(HILPanelHandle,HILPNL_CHART1_CH_RING,iTemp);
			SetCtrlVal(HILPanelHandle,HILPNL_CHART2_CH_RING,iTemp);
			SetCtrlVal(HILPanelHandle,HILPNL_CHART3_CH_RING,iTemp);
			SetCtrlVal(HILPanelHandle,HILPNL_CHART4_CH_RING,iTemp);
			
			InitHILDisIndex();
			
			if(pExMem->testPara[chart1_ch].iStatus) //�����ѡͨ����״̬Ϊ��
			{
				N = (long)(2000 / (pExMem->testPara[chart1_ch].dFreq) * CYCLE_NUM_PER_SCREEN);  //��Ļ��ʾ�������㹫ʽ
				if(N >= 3 && N <= 10000)
				{
					SetCtrlAttribute(HILPanelHandle,HILPNL_STRIPCHART_1,ATTR_POINTS_PER_SCREEN,N);
					SetCtrlAttribute(HILPanelHandle,HILPNL_STRIPCHART_2,ATTR_POINTS_PER_SCREEN,N);
					SetCtrlAttribute(HILPanelHandle,HILPNL_STRIPCHART_3,ATTR_POINTS_PER_SCREEN,N);
					SetCtrlAttribute(HILPanelHandle,HILPNL_STRIPCHART_4,ATTR_POINTS_PER_SCREEN,N);
				}
				
				/*
				for(ringindex = 0;ringindex < 11;ringindex++)  //����ֵ������Χ������y����
				{
					GetValueFromIndex(HILPanelHandle,HILPNL_CHART1_YRANGE_RING,ringindex,&dring_val);
					if(pExMem->testPara[chart1_ch].dAmp < dring_val)
					{
						SetCtrlIndex(HILPanelHandle,HILPNL_CHART1_YRANGE_RING,ringindex);
						SetCtrlIndex(HILPanelHandle,HILPNL_CHART2_YRANGE_RING,ringindex);
						SetAxisRange(HILPanelHandle,HILPNL_STRIPCHART_1,VAL_NO_CHANGE,0.0,1.0,VAL_MANUAL,-dring_val,dring_val);
						SetAxisRange(HILPanelHandle,HILPNL_STRIPCHART_2,VAL_NO_CHANGE,0.0,1.0,VAL_MANUAL,-dring_val,dring_val);
						break;
					}
				}
				*/
			}
			
			break;
	}
	return 0;
}

int CVICALLBACK HILExitCallback (int panel, int control, int event,
		void *callbackData, int eventData1, int eventData2)
{
	switch (event)
	{
		case EVENT_COMMIT:
			if(ConfirmPopup ("ȷ���˳�", "ȷ���˳���")) 
			{
				//��ָ����Ϣ���͸�ʵʱϵͳ����Ϣͷ
				sendMessageHead.iCommand = PAUSE_PROCESS;
				sendMessageHead.iMessageLen = 0;
				if(gTCPConnection != 0)
					ClientTCPWrite(gTCPConnection, &sendMessageHead, MESSAGEHEADSIZE, 1000);         //��������ͷ
				QuitUserInterface(0);  
				DisconnectFromTCPServer(gTCPConnection);    
			}
			break;
	}
	return 0;
}

//���ظ�λ
int CVICALLBACK HILResetCallback (int panel, int control, int event,
		void *callbackData, int eventData1, int eventData2)
{
	switch (event)
	{
		case EVENT_COMMIT:
			 sendMessageHead.iCommand = RESET_PROCESS;
			 sendMessageHead.iMessageLen = COMMANDSIZE;
			 ClientTCPWrite(gTCPConnection, &sendMessageHead, MESSAGEHEADSIZE, 1000);         //��������ͷ
			 Sleep(10);  
			 
			 Hctrlparadown();     
			 InitHILTestPara();
			 ClientTCPWrite(gTCPConnection, pExMem, COMMANDSIZE, 5000);		    //��������   
			 SetCtrlAttribute(HILPanelHandle,HILPNL_PB_START,ATTR_LABEL_TEXT,"����");
			break;
	}
	return 0;
}

//λ�û���
/*int CVICALLBACK HILReturnZeroCallback (int panel, int control, int event,
		void *callbackData, int eventData1, int eventData2)
{
	double dTemp;
	switch (event)
	{
		case EVENT_COMMIT:
			InitHILTestPara();  					  
			
			GetPanelHandleFromTabPage (HILPanelHandle, HILPNL_TAB,2,&tabpanel1);    
			
			GetCtrlVal(tabpanel1,POS_TAB_CH1_ZERO_NUMERIC,&dTemp);
			pExMem->testPara[1].dOffset = dTemp;
										
			GetCtrlVal(tabpanel1,POS_TAB_CH2_ZERO_NUMERIC,&dTemp);
			pExMem->testPara[2].dOffset = dTemp;
			
			GetCtrlVal(tabpanel1,POS_TAB_CH3_ZERO_NUMERIC,&dTemp);
			pExMem->testPara[3].dOffset = dTemp;
			
			GetCtrlVal(tabpanel1,POS_TAB_CH4_ZERO_NUMERIC,&dTemp);
			pExMem->testPara[4].dOffset = dTemp;
			break;
	}
	return 0;
}*/

int CVICALLBACK HILStartCallback (int panel, int control, int event,
		void *callbackData, int eventData1, int eventData2)
{
	double dTemp;
	int tempInt;
	int SendBuffLen;
	switch (event)
	{
		case EVENT_COMMIT:
		
			HILSetScreenCycNum();  //������Ļ��ʾ����  	
			
			//��ָ����Ϣ���͸�ʵʱϵͳ����Ϣͷ������ָ���Լ���������ָ����ĳ���
			
			sendMessageHead.iCommand = START_PROCESS;
			sendMessageHead.iMessageLen = COMMANDSIZE;
			ClientTCPWrite(gTCPConnection, &sendMessageHead, MESSAGEHEADSIZE, 1000);         //��������ͷ
			
			Sleep(10);
			
			Hctrlparadown();     
			InitHILTestPara();	   //��ʼ�����Բ��� 
			ClientTCPWrite(gTCPConnection, pExMem, COMMANDSIZE, 5000);		    //��������  
			SetCtrlAttribute(panel,control,ATTR_LABEL_TEXT,"����");
			break;
	}
	return 0;
}

int CVICALLBACK HILChTesttypeChange (int panel, int control, int event,
		void *callbackData, int eventData1, int eventData2)			  //��ɲ���������
{
	int iTemp;
	int i;
	switch (event)
	{
		case EVENT_COMMIT:
			GetCtrlVal(panel,control,&iTemp);
			
			switch(iTemp)
			{
				case ANY_CHART_TRACE:
					GetPanelHandleFromTabPage (HILPanelHandle,HILPNL_TAB,0,&tabpanel1);
					
					for (i=0 ;i < SIDE_NO + AXIS_NO  ; i++)   
					{
						GetPanelHandleFromTabPage(tabpanel1, SET_TAB_CH_TAB, i ,&tabpanel2);
						
						SetCtrlVal(tabpanel2,CH1_TAB_CH_WAVE,1); 
						SetCtrlAttribute(tabpanel2,CH1_TAB_CH_WAVE,ATTR_DIMMED,0);
						SetCtrlAttribute(tabpanel2,CH1_TAB_CH_AMP,ATTR_DIMMED,0); 
						SetCtrlAttribute(tabpanel2,CH1_TAB_CH_FREQ,ATTR_DIMMED,0); 
						SetCtrlAttribute(tabpanel2,CH1_TAB_CH_AMP,ATTR_LABEL_TEXT,"ָ���ֵ");
						SetCtrlAttribute(tabpanel2,CH1_TAB_TEXTMSG_15,ATTR_CTRL_VAL,"Nm");
						SetCtrlAttribute(tabpanel2,CH1_TAB_CH_START_FREQ,ATTR_DIMMED,1); 
						SetCtrlAttribute(tabpanel2,CH1_TAB_CH_STOP_FREQ,ATTR_DIMMED,1);
					}
					  
					break;
					
				case TORQUE_CHART_TRACE:
					GetPanelHandleFromTabPage (HILPanelHandle,HILPNL_TAB,0,&tabpanel1);
					
					for (i=0 ;i < SIDE_NO + AXIS_NO  ; i++)  
					{
						GetPanelHandleFromTabPage(tabpanel1, SET_TAB_CH_TAB,i,&tabpanel2);
						
						SetCtrlAttribute(tabpanel2,CH1_TAB_CH_WAVE,ATTR_DIMMED,1);
						SetCtrlAttribute(tabpanel2,CH1_TAB_CH_AMP,ATTR_DIMMED,1); 
						SetCtrlAttribute(tabpanel2,CH1_TAB_CH_FREQ,ATTR_DIMMED,1); 
						SetCtrlAttribute(tabpanel2,CH1_TAB_CH_AMP,ATTR_LABEL_TEXT,"ָ���ֵ");
						SetCtrlAttribute(tabpanel2,CH1_TAB_TEXTMSG_15,ATTR_CTRL_VAL,"Nm");
						SetCtrlAttribute(tabpanel2,CH1_TAB_CH_START_FREQ,ATTR_DIMMED,1); 
						SetCtrlAttribute(tabpanel2,CH1_TAB_CH_STOP_FREQ,ATTR_DIMMED,1);
					}
					
					break;
					
				case PROPORTION_TRACE:
					GetPanelHandleFromTabPage (HILPanelHandle,HILPNL_TAB,0,&tabpanel1);
					
					for (i=0 ;i < SIDE_NO + AXIS_NO  ; i++)   
					{
						GetPanelHandleFromTabPage(tabpanel1, SET_TAB_CH_TAB,i,&tabpanel2);
						
						SetCtrlAttribute(tabpanel2,CH1_TAB_CH_WAVE,ATTR_DIMMED,1);
						SetCtrlAttribute(tabpanel2,CH1_TAB_CH_AMP,ATTR_DIMMED,0); 
						SetCtrlAttribute(tabpanel2,CH1_TAB_CH_FREQ,ATTR_DIMMED,1); 
						SetCtrlAttribute(tabpanel2,CH1_TAB_CH_AMP,ATTR_LABEL_TEXT,"�����ݶ�");
						SetCtrlAttribute(tabpanel2,CH1_TAB_TEXTMSG_15,ATTR_CTRL_VAL,"Nm/��"); 
						SetCtrlAttribute(tabpanel2,CH1_TAB_CH_START_FREQ,ATTR_DIMMED,1); 
						SetCtrlAttribute(tabpanel2,CH1_TAB_CH_STOP_FREQ,ATTR_DIMMED,1);
					}
					
					break;
					
				case VMIC_TEST_TRACE:
					GetPanelHandleFromTabPage (HILPanelHandle,HILPNL_TAB,0,&tabpanel1);
					
					for (i=0 ;i < SIDE_NO + AXIS_NO  ; i++) 
					{
						GetPanelHandleFromTabPage(tabpanel1, SET_TAB_CH_TAB,i,&tabpanel2);
						
						SetCtrlVal(tabpanel2,CH1_TAB_CH_WAVE,VMIC_TEST_TRACE);
						SetCtrlAttribute(tabpanel2,CH1_TAB_CH_WAVE,ATTR_DIMMED,1); 
						SetCtrlAttribute(tabpanel2,CH1_TAB_CH_AMP,ATTR_DIMMED,1); 
						SetCtrlAttribute(tabpanel2,CH1_TAB_CH_FREQ,ATTR_DIMMED,1); 
						SetCtrlAttribute(tabpanel2,CH1_TAB_CH_AMP,ATTR_LABEL_TEXT,"ָ���ֵ");
						SetCtrlAttribute(tabpanel2,CH1_TAB_TEXTMSG_15,ATTR_CTRL_VAL,"Nm");
						SetCtrlAttribute(tabpanel2,CH1_TAB_CH_START_FREQ,ATTR_DIMMED,1); 
						SetCtrlAttribute(tabpanel2,CH1_TAB_CH_STOP_FREQ,ATTR_DIMMED,1);
					}
	
					break;
			}

			break;
	}
	return 0;
}
/*
int CVICALLBACK HILCh2TesttypeChange (int panel, int control, int event,
		void *callbackData, int eventData1, int eventData2)
{
	int iTemp; 
	switch (event)
	{
		case EVENT_COMMIT:
			GetCtrlVal(panel,control,&iTemp);
			switch(iTemp)
			{
				case ANY_CHART_TRACE:
					SetCtrlAttribute(panel,CH2_TAB_CH_WAVE,ATTR_DIMMED,0);
					SetCtrlAttribute(panel,CH2_TAB_CH_AMP,ATTR_DIMMED,0); 
					SetCtrlAttribute(panel,CH2_TAB_CH_FREQ,ATTR_DIMMED,0); 
					SetCtrlAttribute(panel,CH2_TAB_CH_AMP,ATTR_LABEL_TEXT,"ָ���ֵ");
					SetCtrlAttribute(panel,CH1_TAB_CH_START_FREQ,ATTR_DIMMED,1); 
					SetCtrlAttribute(panel,CH1_TAB_CH_STOP_FREQ,ATTR_DIMMED,1);
					break;
				case TORQUE_CHART_TRACE:
					SetCtrlAttribute(panel,CH2_TAB_CH_WAVE,ATTR_DIMMED,1);
					SetCtrlAttribute(panel,CH2_TAB_CH_AMP,ATTR_DIMMED,1); 
					SetCtrlAttribute(panel,CH2_TAB_CH_FREQ,ATTR_DIMMED,1); 
					SetCtrlAttribute(panel,CH2_TAB_CH_AMP,ATTR_LABEL_TEXT,"ָ���ֵ");
					SetCtrlAttribute(panel,CH1_TAB_CH_START_FREQ,ATTR_DIMMED,0); 
					SetCtrlAttribute(panel,CH1_TAB_CH_STOP_FREQ,ATTR_DIMMED,0);
					break;
				case PROPORTION_TRACE:
					SetCtrlAttribute(panel,CH2_TAB_CH_WAVE,ATTR_DIMMED,1);
					
					SetCtrlAttribute(panel,CH2_TAB_CH_AMP,ATTR_DIMMED,0); 
					SetCtrlAttribute(panel,CH2_TAB_CH_FREQ,ATTR_DIMMED,1); 
					SetCtrlAttribute(panel,CH2_TAB_CH_AMP,ATTR_LABEL_TEXT,"�����ݶ�");
					SetCtrlAttribute(panel,CH1_TAB_CH_START_FREQ,ATTR_DIMMED,1); 
					SetCtrlAttribute(panel,CH1_TAB_CH_STOP_FREQ,ATTR_DIMMED,1);
					break;
					
			}
			break;
	}
	return 0;
}

int CVICALLBACK HILCh3TesttypeChange (int panel, int control, int event,
		void *callbackData, int eventData1, int eventData2)
{
	int iTemp; 
	switch (event)
	{
		case EVENT_COMMIT:
			GetCtrlVal(panel,control,&iTemp);
			switch(iTemp)
			{
				case ANY_CHART_TRACE:
					SetCtrlAttribute(panel,CH3_TAB_CH_WAVE,ATTR_DIMMED,0);
					SetCtrlAttribute(panel,CH3_TAB_CH_AMP,ATTR_DIMMED,0); 
					SetCtrlAttribute(panel,CH3_TAB_CH_FREQ,ATTR_DIMMED,0); 
					SetCtrlAttribute(panel,CH3_TAB_CH_AMP,ATTR_LABEL_TEXT,"ָ���ֵ");
					SetCtrlAttribute(panel,CH1_TAB_CH_START_FREQ,ATTR_DIMMED,1); 
					SetCtrlAttribute(panel,CH1_TAB_CH_STOP_FREQ,ATTR_DIMMED,1);
					break;
				case TORQUE_CHART_TRACE:
					SetCtrlAttribute(panel,CH3_TAB_CH_WAVE,ATTR_DIMMED,1);
					SetCtrlAttribute(panel,CH3_TAB_CH_AMP,ATTR_DIMMED,1); 
					SetCtrlAttribute(panel,CH3_TAB_CH_FREQ,ATTR_DIMMED,1); 
					SetCtrlAttribute(panel,CH3_TAB_CH_AMP,ATTR_LABEL_TEXT,"ָ���ֵ");
					SetCtrlAttribute(panel,CH1_TAB_CH_START_FREQ,ATTR_DIMMED,0); 
					SetCtrlAttribute(panel,CH1_TAB_CH_STOP_FREQ,ATTR_DIMMED,0);
					break;
				case PROPORTION_TRACE:
					SetCtrlAttribute(panel,CH3_TAB_CH_WAVE,ATTR_DIMMED,1);
					
					SetCtrlAttribute(panel,CH3_TAB_CH_AMP,ATTR_DIMMED,0); 
					SetCtrlAttribute(panel,CH3_TAB_CH_FREQ,ATTR_DIMMED,1); 
					SetCtrlAttribute(panel,CH3_TAB_CH_AMP,ATTR_LABEL_TEXT,"�����ݶ�");
					SetCtrlAttribute(panel,CH1_TAB_CH_START_FREQ,ATTR_DIMMED,1); 
					SetCtrlAttribute(panel,CH1_TAB_CH_STOP_FREQ,ATTR_DIMMED,1);
					break;
					
			}
			break;
	}
	return 0;
}

int CVICALLBACK HILCh4TesttypeChange (int panel, int control, int event,
		void *callbackData, int eventData1, int eventData2)
{
	int iTemp;      
	switch (event)
	{
		case EVENT_COMMIT:
			GetCtrlVal(panel,control,&iTemp);
			switch(iTemp)
			{
				case ANY_CHART_TRACE:
					SetCtrlAttribute(panel,CH4_TAB_CH_WAVE,ATTR_DIMMED,0);
					SetCtrlAttribute(panel,CH4_TAB_CH_AMP,ATTR_DIMMED,0); 
					SetCtrlAttribute(panel,CH4_TAB_CH_FREQ,ATTR_DIMMED,0); 
					SetCtrlAttribute(panel,CH4_TAB_CH_AMP,ATTR_LABEL_TEXT,"ָ���ֵ");
					SetCtrlAttribute(panel,CH1_TAB_CH_START_FREQ,ATTR_DIMMED,1); 
					SetCtrlAttribute(panel,CH1_TAB_CH_STOP_FREQ,ATTR_DIMMED,1);
					break;
				case TORQUE_CHART_TRACE:
					SetCtrlAttribute(panel,CH4_TAB_CH_WAVE,ATTR_DIMMED,1);
					SetCtrlAttribute(panel,CH4_TAB_CH_AMP,ATTR_DIMMED,1); 
					SetCtrlAttribute(panel,CH4_TAB_CH_FREQ,ATTR_DIMMED,1); 
					SetCtrlAttribute(panel,CH4_TAB_CH_AMP,ATTR_LABEL_TEXT,"ָ���ֵ");
					SetCtrlAttribute(panel,CH1_TAB_CH_START_FREQ,ATTR_DIMMED,0); 
					SetCtrlAttribute(panel,CH1_TAB_CH_STOP_FREQ,ATTR_DIMMED,0);
					break;
				case PROPORTION_TRACE:
					SetCtrlAttribute(panel,CH4_TAB_CH_WAVE,ATTR_DIMMED,1);
					SetCtrlAttribute(panel,CH4_TAB_CH_AMP,ATTR_DIMMED,0); 
					SetCtrlAttribute(panel,CH4_TAB_CH_FREQ,ATTR_DIMMED,1); 
					SetCtrlAttribute(panel,CH4_TAB_CH_AMP,ATTR_LABEL_TEXT,"�����ݶ�");
					SetCtrlAttribute(panel,CH1_TAB_CH_START_FREQ,ATTR_DIMMED,1); 
					SetCtrlAttribute(panel,CH1_TAB_CH_STOP_FREQ,ATTR_DIMMED,1);
					break;
					
			}
			break;
	}
	return 0;
}
*/
int CVICALLBACK HILSaveParaCallback (int panel, int control, int event,
		void *callbackData, int eventData1, int eventData2)
{
	int i;
	double dTemp; 
	char sectionNam[512];
	switch (event)
	{
		case EVENT_COMMIT:
			if(ConfirmPopup("Confirm Popup","�����������������滻ԭ�ڴ��в�����"))
			{
				GetPanelHandleFromTabPage (HILPanelHandle, HILPNL_TAB, 1, &tabpanel1);    
			
				for (i=0 ;i < SIDE_NO  ; i++)
				{
					GetPanelHandleFromTabPage(tabpanel1, PARA_TAB_CTRL_PARA_TAB, i,&tabpanel2);
					strcpy(sectionNam,"0CHANNEL");
					sectionNam[0] = i+ '0'; 
					GetCtrlVal(tabpanel2,12,&dTemp);
					Ini_PutDouble(inihandle,sectionNam,"Ktp",dTemp);
					GetCtrlVal(tabpanel2,11,&dTemp);
					Ini_PutDouble(inihandle,sectionNam,"Kti",dTemp); 
					GetCtrlVal(tabpanel2,13,&dTemp);
					Ini_PutDouble(inihandle,sectionNam,"Ktd",dTemp);
					GetCtrlVal(tabpanel2,10,&dTemp);
					Ini_PutDouble(inihandle,sectionNam,"Ft1",dTemp);
					GetCtrlVal(tabpanel2,9,&dTemp);
					Ini_PutDouble(inihandle,sectionNam,"Ft2",dTemp); 
					GetCtrlVal(tabpanel2,8,&dTemp);
					Ini_PutDouble(inihandle,sectionNam,"Ftk",dTemp); 
					GetCtrlVal(tabpanel2,7,&dTemp);
					Ini_PutDouble(inihandle,sectionNam,"Kep",dTemp);
					GetCtrlVal(tabpanel2,6,&dTemp);
					Ini_PutDouble(inihandle,sectionNam,"Ked",dTemp); 
					GetCtrlVal(tabpanel2,5,&dTemp);
					Ini_PutDouble(inihandle,sectionNam,"Ted",dTemp);
					GetCtrlVal(tabpanel2,4,&dTemp);
					Ini_PutDouble(inihandle,sectionNam,"X1",dTemp);
					GetCtrlVal(tabpanel2,3,&dTemp);
					Ini_PutDouble(inihandle,sectionNam,"X2",dTemp); 
					GetCtrlVal(tabpanel2,2,&dTemp);
					Ini_PutDouble(inihandle,sectionNam,"X3",dTemp);
					//Ini_WriteToFile (inihandle, pathname);  	
				} 
				for (i=SIDE_NO ;i < AXIS_NO+SIDE_NO  ; i++)
				{
					GetPanelHandleFromTabPage(tabpanel1, PARA_TAB_CTRL_PARA_TAB, i,&tabpanel2);
					strcpy(sectionNam,"0CHANNEL");
					sectionNam[0] = i+ '0'; 

				    //����ͨ���Ŀ��Ʋ���
			
					GetCtrlVal(tabpanel2,CH1_A_TAB_NUM_HIL_KTP,&dTemp);
					Ini_PutDouble(inihandle,sectionNam,"Ktp",dTemp);
					GetCtrlVal(tabpanel2,CH1_A_TAB_NUM_HIL_KTI,&dTemp);
					Ini_PutDouble(inihandle,sectionNam,"Kti",dTemp); 
					GetCtrlVal(tabpanel2,CH1_A_TAB_NUM_HIL_KTD,&dTemp);
					Ini_PutDouble(inihandle,sectionNam,"Ktd",dTemp);
					GetCtrlVal(tabpanel2,CH1_A_TAB_NUM_HIL_FT1,&dTemp);
					Ini_PutDouble(inihandle,sectionNam,"Ft1",dTemp);
					GetCtrlVal(tabpanel2,CH1_A_TAB_NUM_HIL_FT2,&dTemp);
					Ini_PutDouble(inihandle,sectionNam,"Ft2",dTemp); 
					GetCtrlVal(tabpanel2,CH1_A_TAB_NUM_HIL_FTK,&dTemp);
					Ini_PutDouble(inihandle,sectionNam,"Ftk",dTemp); 
				}

			
				Ini_WriteToFile (inihandle, pathname);   
			
			}
			else
			{}
			
			
			break;
	}
	return 0;
}

int CVICALLBACK HILSetParaCallback (int panel, int control, int event,
		void *callbackData, int eventData1, int eventData2)			//����������
{
	double dTemp;
	int selbutton;
	int i;
	char keyinputbuffer[65];
	
	switch (event)
	{
		case EVENT_COMMIT:
			if(keyerr)  //��������־λ
			{
				selbutton = GenericMessagePopup("Generic Message Popup","���Ʋ����޸���Ҫ����ԱȨ�ޣ����������룺",
												"ȷ��","ȡ��",0,keyinputbuffer,64,0,VAL_GENERIC_POPUP_INPUT_STRING,
												VAL_GENERIC_POPUP_BTN1,VAL_GENERIC_POPUP_BTN2);
				switch (selbutton)
				{
					case VAL_GENERIC_POPUP_BTN1:
						keyerr = strcmp(keyinputbuffer,originalkey); //����Ƚ�
						if(!keyerr)									 //����Ǻ�
						{
							//ȷ��
							if(ConfirmPopup("Confirm Popup","������ȷ���޸Ĳ�����Ӱ��ϵͳ�������ܣ������޸ģ�"))
							{
								keyerr = 0;
								LightHILParaSet();	//light the paraset
							}
							else
							{
								MessagePopup("Message Popup","���Ѿ�ȡ���˿��Ʋ������޸Ĳ�����");
							}
						}
						else										 //������Ǻ�
						{
							MessagePopup("Message Popup","�������������˶�������ٽ��в�����");
							keyerr = 1;
						}
						break;
					case VAL_GENERIC_POPUP_BTN2:
						break;
					default:
						break;
							
				}
				
			}
			else
			{
				LightHILParaSet();
				GetPanelHandleFromTabPage (HILPanelHandle, HILPNL_TAB,1,&tabpanel1); 
				
				for (i=0 ;i < SIDE_NO  ; i++) 
				{
				//ͨ��1�Ŀ��Ʋ���
					GetPanelHandleFromTabPage(tabpanel1, PARA_TAB_CTRL_PARA_TAB,i,&tabpanel2);
					GetCtrlVal(tabpanel2,CH1_P_TAB_NUM_HIL_KTP,&dTemp);
					pExMem->ctrlPara[i+1].Ktp = dTemp;
					GetCtrlVal(tabpanel2,CH1_P_TAB_NUM_HIL_KTI,&dTemp);
					pExMem->ctrlPara[i+1].Kti = dTemp;
					GetCtrlVal(tabpanel2,CH1_P_TAB_NUM_HIL_KTD,&dTemp);
					pExMem->ctrlPara[i+1].Ktd = dTemp;
					GetCtrlVal(tabpanel2,CH1_P_TAB_NUM_HIL_FT1,&dTemp);
					pExMem->ctrlPara[i+1].Ft1 = dTemp;
					GetCtrlVal(tabpanel2,CH1_P_TAB_NUM_HIL_FT2,&dTemp);
					pExMem->ctrlPara[i+1].Ft2 = dTemp;
					GetCtrlVal(tabpanel2,CH1_P_TAB_NUM_HIL_FTK,&dTemp);
					pExMem->ctrlPara[i+1].Ftk = dTemp;
					GetCtrlVal(tabpanel2,CH1_P_TAB_NUM_HIL_KEP,&dTemp);
					pExMem->ctrlPara[i+1].Kep = dTemp;
					GetCtrlVal(tabpanel2,CH1_P_TAB_NUM_HIL_KED,&dTemp);
					pExMem->ctrlPara[i+1].Ked = dTemp;
					GetCtrlVal(tabpanel2,CH1_P_TAB_NUM_HIL_TED,&dTemp);
					pExMem->ctrlPara[i+1].Ted = dTemp;
					GetCtrlVal(tabpanel2,CH1_P_TAB_NUM_HIL_X1,&dTemp);
					pExMem->ctrlPara[i+1].X1 = dTemp;
					GetCtrlVal(tabpanel2,CH1_P_TAB_NUM_HIL_X2,&dTemp);
					pExMem->ctrlPara[i+1].X2 = dTemp;
					GetCtrlVal(tabpanel2,CH1_P_TAB_NUM_HIL_X3,&dTemp);
					pExMem->ctrlPara[i+1].X3 = dTemp;
				}
				//ͨ��2�Ĳ�������
		
				
				/*
				pExMem->requestType = SETPARA_PROCESS; 
				pExMem->request  = IS_REQUEST; 
				*/
			}
			
			break;
	}
	return 0;
}

int CVICALLBACK HILOilCtrlCallback (int panel, int control, int event,
		void *callbackData, int eventData1, int eventData2)
{
	switch (event)
	{
		case EVENT_COMMIT:
			DisplayHPSPanel();             
			break;
	}
	return 0;
}

int CVICALLBACK HILDataReDisplay (int panel, int control, int event,
		void *callbackData, int eventData1, int eventData2)
{
	
	int selectstatus = 0;
	static int confirm = 0;
	
	switch (event)
	{
		case EVENT_COMMIT:
			if(DftPathSign == 0)
			{
				selectstatus = FileSelectPopup("d:\\","*.exe",".exe;.dat;.dll","ѡ�����ݻط����",
												VAL_SELECT_BUTTON,0,1,0,0,pathname);
				if(selectstatus > 0)
				{
					confirm = ConfirmPopup("Confirm Popup","����ѡ����ȷ��ΪĬ�ϵ����ݻطų���");
					
					if(confirm)
					{
						DftPathSign = 1; 
						strcpy(pathpass,"");
						strcat(pathpass,pathname);
						LaunchExecutable (pathpass);
					} 
					else
						DftPathSign = 0;
				}
				
			}
			else
			{
				strcpy(pathpass,"");
				strcat(pathpass,pathname);
				LaunchExecutable (pathpass);	//�����ⲿ���ݻط����
			}
			
			break;
	}
	return 0;
}

int CVICALLBACK HILTabChanelSelect (int panel, int control, int event,
		void *callbackData, int eventData1, int eventData2)
{
	long N;
	int iTemp,ringindex;
	double dring_val;
	
	switch (event)
	{
		case EVENT_ACTIVE_TAB_CHANGE:
			
			if(eventData1 != eventData2)
			{
				eventData2 += 1;
				SetCtrlVal(HILPanelHandle,HILPNL_CHART1_CH_RING,eventData2);
				SetCtrlVal(HILPanelHandle,HILPNL_CHART2_CH_RING,eventData2);
				SetCtrlVal(HILPanelHandle,HILPNL_CHART3_CH_RING,eventData2);
				SetCtrlVal(HILPanelHandle,HILPNL_CHART4_CH_RING,eventData2);
				
				InitHILDisIndex();
				
				if(pExMem->testPara[chart1_ch].iStatus) //�����ѡͨ����״̬Ϊ��
				{
					N = (long)(2000 / (pExMem->testPara[chart1_ch].dFreq) * CYCLE_NUM_PER_SCREEN);  //��Ļ��ʾ�������㹫ʽ
					if(N >= 3 && N <= 10000)
					{
						SetCtrlAttribute(HILPanelHandle,HILPNL_STRIPCHART_1,ATTR_POINTS_PER_SCREEN,N);
						SetCtrlAttribute(HILPanelHandle,HILPNL_STRIPCHART_2,ATTR_POINTS_PER_SCREEN,N);
						SetCtrlAttribute(HILPanelHandle,HILPNL_STRIPCHART_3,ATTR_POINTS_PER_SCREEN,N);
						SetCtrlAttribute(HILPanelHandle,HILPNL_STRIPCHART_4,ATTR_POINTS_PER_SCREEN,N);
					}
					/*
					for(ringindex = 0;ringindex < 11;ringindex++)  //����ֵ������Χ������y����
					{
						//GetValueFromIndex(HILPanelHandle,HILPNL_CHART1_YRANGE_RING,ringindex,&dring_val);
						if(pExMem->testPara[chart1_ch].dAmp < dring_val)
						{
							SetCtrlIndex(HILPanelHandle,HILPNL_CHART1_YRANGE_RING,ringindex);
							SetCtrlIndex(HILPanelHandle,HILPNL_CHART2_YRANGE_RING,ringindex);
							SetAxisRange(HILPanelHandle,HILPNL_STRIPCHART_1,VAL_NO_CHANGE,0.0,1.0,VAL_MANUAL,-dring_val,dring_val);
							SetAxisRange(HILPanelHandle,HILPNL_STRIPCHART_2,VAL_NO_CHANGE,0.0,1.0,VAL_MANUAL,-dring_val,dring_val);
							break;
						}
					}
					*/
				}
			}
			break;
	}
	return 0;
}
int CVICALLBACK HILParaTabChanelSelect (int panel, int control, int event,
		void *callbackData, int eventData1, int eventData2)
{
	long N;
	int iTemp,ringindex;
	double dring_val;
	
	switch (event)
	{
		case EVENT_ACTIVE_TAB_CHANGE:
			
			if(eventData1 != eventData2)
			{
				eventData2 += 1;
				SetCtrlVal(HILPanelHandle,HILPNL_CHART1_CH_RING,eventData2);
				SetCtrlVal(HILPanelHandle,HILPNL_CHART2_CH_RING,eventData2);
				SetCtrlVal(HILPanelHandle,HILPNL_CHART3_CH_RING,eventData2);
				SetCtrlVal(HILPanelHandle,HILPNL_CHART4_CH_RING,eventData2);
				
				InitHILDisIndex();
				
				if(pExMem->testPara[chart1_ch].iStatus) //�����ѡͨ����״̬Ϊ��
				{
					N = (long)(2000 / (pExMem->testPara[chart1_ch].dFreq) * CYCLE_NUM_PER_SCREEN);  //��Ļ��ʾ�������㹫ʽ
					if(N >= 3 && N <= 10000)
					{
						SetCtrlAttribute(HILPanelHandle,HILPNL_STRIPCHART_1,ATTR_POINTS_PER_SCREEN,N);
						SetCtrlAttribute(HILPanelHandle,HILPNL_STRIPCHART_2,ATTR_POINTS_PER_SCREEN,N);
						SetCtrlAttribute(HILPanelHandle,HILPNL_STRIPCHART_3,ATTR_POINTS_PER_SCREEN,N);
						SetCtrlAttribute(HILPanelHandle,HILPNL_STRIPCHART_4,ATTR_POINTS_PER_SCREEN,N);
					}
					/*
					for(ringindex = 0;ringindex < 11;ringindex++)  //����ֵ������Χ������y����
					{
						//GetValueFromIndex(HILPanelHandle,HILPNL_CHART1_YRANGE_RING,ringindex,&dring_val);
						if(pExMem->testPara[chart1_ch].dAmp < dring_val)
						{
							SetCtrlIndex(HILPanelHandle,HILPNL_CHART1_YRANGE_RING,ringindex);
							SetCtrlIndex(HILPanelHandle,HILPNL_CHART2_YRANGE_RING,ringindex);
							SetAxisRange(HILPanelHandle,HILPNL_STRIPCHART_1,VAL_NO_CHANGE,0.0,1.0,VAL_MANUAL,-dring_val,dring_val);
							SetAxisRange(HILPanelHandle,HILPNL_STRIPCHART_2,VAL_NO_CHANGE,0.0,1.0,VAL_MANUAL,-dring_val,dring_val);
							break;
						}
					}
					*/
				}
			}
			break;
	}
	return 0;
}


  //����λ�����ص�����
static int CVICALLBACK TCPDataCallback (unsigned handle, int xType, int errCode, void *cbData)
{
	int iCommand;
	int iDataLen;
	int iChannels;
	
	char msgBuf[512];
	char tempBuf1[32];
	char tempValueBuf[128];
	char *tempBuf;
	char path[MAX_PATHNAME_LEN];  
	char datestr[16];
	char datestr1[16];
	char timestr[16];
	int i,j;
	short shortBuf[2];
	int iDataNum;
	
	
	int DataSize; 
	int bytesToRead;
	int bytesRead;
	int messageLen;
	int dataLen;
	int iDataLoops; 
	
	double * readData = NULL;
	char * readBuff = NULL;
	double * chart1 = NULL;
	double * chart2 = NULL;
	double * chart3 = NULL;
	double * chart4 = NULL;
	
	
    switch (xType)
    {
        case TCP_DISCONNECT:
			//CmtGetLock (gLock);
            gTCPConnection = TCP_INVALID_CONNECTION;
			//CmtReleaseLock (gLock);
            break;
        case TCP_DATAREADY: // Client executable sent a command.	  //�õ�����
			// ClientTCPRead does not guarantee that all data will be transferred in
			// a single call, but this code assumes that it does for simplicity.
			
			// ClientTCPRead does not guarantee that all data will be transferred in
			// a single call, but this code assumes that it does for simplicity.
			//InsertTextBoxLine(runningpanel, RUNPANEL_MSGSTRING,0,"�յ��������");   
			bytesToRead = MESSAGEHEADSIZE;
			
			//printf("address1 = %x,address2=%x", &readCommandMessage, (char*)(&readCommandMessage) +1);
			
			while (bytesToRead > 0)		 //��֡ͷ
			{
				bytesRead = ClientTCPRead (gTCPConnection, (char*)(&readMessageHead) + (MESSAGEHEADSIZE - bytesToRead), bytesToRead, 0);
				bytesToRead -= bytesRead;
			}
			
			iCommand = readMessageHead.iCommand;
			iDataLen = readMessageHead.iMessageLen;
			iDataLoops =  iDataLen /transdatasizeH; //ÿ�����ݰ��а����Ŀ��Ʋ���ѭ������
		
			if(iCommand == 6)	   //��������ݣ��Ͷ������е�����
			{
				bytesToRead =  iDataLen*sizeof(double);  
				DataSize = bytesToRead;
				
				readData = (double *)malloc(iDataLen*sizeof(double));
				readBuff = (char *) malloc(iDataLen*sizeof(double));
				
				chart1 = (double *)malloc((sizeof(double)*iDataLoops) * 2);
				chart2 = (double *)malloc(sizeof(double)*iDataLoops);
				chart3 = (double *)malloc((sizeof(double)*iDataLoops) * 1);
				chart4 = (double *)malloc(sizeof(double)*iDataLoops);
				
				while (bytesToRead > 0)		 //������
				{
					bytesRead = ClientTCPRead (gTCPConnection, &readBuff[DataSize - bytesToRead], bytesToRead, 0);
					bytesToRead -= bytesRead;
				}
				
				memcpy(readData,readBuff, DataSize);
				
				if(recording == 1)
				{
					//memcpy((saveBuf + saveIndex ),readBuff,dataStructSize);
					memcpy((saveBuf + saveIndex),readData,DataSize); 
					saveIndex += iDataLoops;
				}
				
				for(i=0;i<iDataLoops;i++)
				{
					//��λ������������ÿ��������42��double�͵�������Ϊ4�飬
					//0-8����ָ��ǽǶ�ָ�������ָ���ɲ�����Ŀ����
					//9-13Ϊ5���������ز�����
					//14-17Ϊ4��������������
					//18-26Ϊ9�����������
					//27-31��5������λ�Ʋ�����
					//32-36��5������ǶȲ�����
					//37-41��5��������ٶȲ�����ÿ�����͵���ռ8�������ͨ��������8����ʣ��ľ���������
					
					chart1[2*i] = readData[transdatasizeH*i+0+chart3_ch-1];
					chart1[2*i+1] = readData[transdatasizeH*i+9+chart3_ch-1]; 	   //���ز���
					
				//	chart2[i] = readData[transdatasizeH*i+0+chart1_ch-1] - readData[transdatasizeH*i+8+chart1_ch-1];
					
					chart3[i] = readData[transdatasizeH*i+32+chart1_ch-1];   //���̽ǶȲ���
					
				//	chart4[i] = readData[transdatasizeH*i+16+chart1_ch-1];
					switch(chart2_content)
					{
						case POSITION_SAMPLE_INDEX:
								chart2[i] = readData[transdatasizeH*i+27+chart2_ch-1];
							break;
						case ACCELEROMETER_SAMPLE_INDEX:
								chart2[i] = readData[transdatasizeH*i+37+chart2_ch-1];  
							break;
						case TORQUE_SAMPLE_INDEX:
								chart2[i] = readData[transdatasizeH*i+9+chart2_ch-1];  
							break;
						case TORQUE_GIVEN_INDEX:
								chart2[i] = readData[transdatasizeH*i+0+chart2_ch-1];      
							break;
						case SERVO_OUT_INDEX:
								chart2[i] = readData[transdatasizeH*i+18+chart2_ch-1];      
							break;
						case ANGLE_SAMPLE_INDEX:
								//chart2[i] = chart1[2*i]- chart1[2*i+1];   ///////////////
								chart2[i] = readData[transdatasizeH*i+32+chart2_ch-1];
							break;
						default:
								chart2[i] = 0.0;
							break;
					}
				
					switch(chart4_content)
					{
						/*case ANGLE_SAMPLE_INDEX:
								chart4[i] = readData[transdatasizeH*i+8+chart4_ch-1];
							break;
						case ANGLE_GIVEN_INDEX:
								chart4[i] = readData[transdatasizeH*i+0+chart4_ch-1]; 
							break;*/
						case TORQUE_SAMPLE_INDEX:
								chart4[i] = readData[transdatasizeH*i+9+chart4_ch-1]; 
							break;
						case TORQUE_GIVEN_INDEX:
								chart4[i] = readData[transdatasizeH*i+0+chart4_ch-1];;
							break;
						case SERVO_OUT_INDEX:
								chart4[i] = readData[transdatasizeH*i+18+chart4_ch-1]; 
							break;
						case TORQUE_ERROR_INDEX:
								chart4[i] = chart1[2*i]- chart1[2*i+1];   ///////////////
							break;
						default:
								chart4[i] = 0.0;
							break;
					
				
					}
				
					
				}
				
				if(iDataLoops>0)
				{
					PlotStripChart(HILPanelHandle, HILPNL_STRIPCHART_1,chart3,1*iDataLoops,0,0,VAL_DOUBLE);
					PlotStripChart(HILPanelHandle, HILPNL_STRIPCHART_2,chart2,iDataLoops,0,0,VAL_DOUBLE);  
					PlotStripChart(HILPanelHandle, HILPNL_STRIPCHART_3,chart1,2*iDataLoops,0,0,VAL_DOUBLE);
					PlotStripChart(HILPanelHandle, HILPNL_STRIPCHART_4,chart4,iDataLoops,0,0,VAL_DOUBLE); 
				}
				
				if(readData != NULL)
				{
					SetCtrlVal(HILPanelHandle, HILPNL_NUMERIC, readData[9]);  
					SetCtrlVal(HILPanelHandle, HILPNL_NUMERIC_2, readData[10]);    
					SetCtrlVal(HILPanelHandle, HILPNL_NUMERIC_3, readData[11]);    
					SetCtrlVal(HILPanelHandle, HILPNL_NUMERIC_4, readData[12]); 
					SetCtrlVal(HILPanelHandle, HILPNL_NUMERIC_9, readData[13]); 
										
					SetCtrlVal(HILPanelHandle, HILPNL_NUMERIC_5, readData[14]);    					
					SetCtrlVal(HILPanelHandle, HILPNL_NUMERIC_6, readData[15]);    
					SetCtrlVal(HILPanelHandle, HILPNL_NUMERIC_7, readData[16]);
					
					SetCtrlVal(HILPanelHandle, HILPNL_NUMERIC_8, readData[17]); 

				}
				
			}
		
            break;
    }
	
	
	if(readData!= NULL) free(readData);
	if(readBuff!=NULL) free(readBuff);
	if(chart1 != NULL) free(chart1);
	if(chart2 != NULL) free(chart2);  
	if(chart3 != NULL) free(chart3); 
	if(chart4 != NULL) free(chart4);  
    
    return 0;
}

int CVICALLBACK NetWatchDog3 (int panel, int control, int event,
		void *callbackData, int eventData1, int eventData2)
{
	
	switch (event)
	{
		case EVENT_TIMER_TICK:
			
			//��ָ����Ϣ���͸�ʵʱϵͳ����Ϣͷ������ָ���Լ���������ָ����ĳ���
			sendMessageHead.iCommand = WATCHDOG;   
			
			sendMessageHead.iMessageLen = 0;		 //Ҳû��ʵ�ʵ�����
		
			//ClientTCPWrite(gTCPConnection, &sendMessageHead, MESSAGEHEADSIZE, 1000);         //��������ͷ
			
			udpsendmsg.command = udpmsg.command;//����������
			
			source_p1= ((float)tag_reg[1]-5530)/  (27648.0-5530.0) *40;
			source_p2= ((float)tag_reg[2]-5530)/  (27648.0-5530.0) *40;
			
			udpsendmsg.PressureStateValue1 = (short)(source_p1*100);
			udpsendmsg.PressureStateValue2 = (short)(source_p2*100);
			
			sys_temp= ((float)tag_reg[0]-5530)/ (27648.0-5530.0) *200-50;	   //�¶ȷ�Χ   
			
			udpsendmsg.TemperatureValue = (short)(sys_temp*100);
			
			udpsendmsg.SourceState = (short)tag_reg[5];
			
			
			if(gUDPChannel != NULL)
				UDPWrite(gUDPChannel, 20000, SEND_COMPUTER_ADDR, &udpsendmsg, 14);
			
			//������Դ�ĸ��³���
			//HPStimer(0, 0, EVENT_TIMER_TICK, 0, 0, 0);
			
			break;
	}
	return 0;
}


int CVICALLBACK	UDPChatCallback (unsigned channel, int eventType, int errCode, void *callbackData)
{
	int				error = 0,
					size;
	unsigned char	*msg = NULL;
	float value;   
	
	if (eventType == UDP_DATAREADY)
	{
		IPAddr srcAddr;
		
		// Pass NULL as the input buffer to determine the size of the arrived data.
		size = UDPRead(channel, NULL, 0, UDP_DO_NOT_WAIT, NULL, NULL);
		
		msg = malloc(size);
		
		// Read the waiting message into the allocated buffer.
		size = UDPRead(channel, msg, size, UDP_DO_NOT_WAIT, NULL, (char*)srcAddr);
		
		sprintf(msgBuff,"size = %d",size);
		//MessagePopup("UDP", msgBuff);  
		
		
		// Process the message.
		if(size == 10)
		{
			memcpy(&udpmsg, msg, 10);
			if(udpmsg.command == 0x01)   //Һѹ�õ�ѹ
			{
				value = udpmsg.commandvalue1 / 100.0 / 21.0  * (13824);
				send_reg[0]  = (unsigned short int)value;
				value = udpmsg.commandvalue2 / 100.0 / 21.0  * (13824);
				send_reg[1]  = (unsigned short int)value;
			}
			
			if(udpmsg.command == 0x02)	  //��ͣ��
			{
				if(udpmsg.commandvalue1 == 0 )
				{
					control_word = control_word & 0xF700;    
					control_word = control_word | 0x1000;
					//SetCtrlVal (HPSPANEL, HPSPANEL_MOTOR_RING, !state); 
				}
				else
				{
					control_word = control_word & 0xEF00;    
					control_word = control_word | 0x0800;
					//SetCtrlVal(HPSPANEL, HPSPANEL_MOTOR_RING, !state); 
			
				}
				
				if(udpmsg.commandvalue2 == 0)
				{
					control_word = control_word & 0xFD00;    
					control_word = control_word | 0x0400;
					//SetCtrlVal (HPSPANEL, HPSPANEL_MOTOR_RING_2, !state); 
				}
				else
				{
					control_word = control_word & 0xFB00;    
					control_word = control_word | 0x0200;
					//SetCtrlVal(HPSPANEL, HPSPANEL_MOTOR_RING_2, !state); 
				}
			
				send_reg[2] = control_word;
				
			}
			if(udpmsg.command == 0x03)		//��������
			{
				//��ָ����Ϣ���͸�ʵʱϵͳ����Ϣͷ������ָ���Լ���������ָ����ĳ���
			
				sendMessageHead.iCommand = START_PROCESS;
				sendMessageHead.iMessageLen = COMMANDSIZE;
				ClientTCPWrite(gTCPConnection, &sendMessageHead, MESSAGEHEADSIZE, 1000);         //��������ͷ
			
				Sleep(10);
			
				Hctrlparadown();     
				InitHILTestPara();	   //��ʼ�����Բ��� 
				ClientTCPWrite(gTCPConnection, pExMem, COMMANDSIZE, 5000);		    //��������  
				SetCtrlAttribute(HILPanelHandle,HILPNL_PB_START,ATTR_LABEL_TEXT,"����");
				
			}
			
			if(udpmsg.command == 0x04)   //���ظ�λ
			{
				//��ָ����Ϣ���͸�ʵʱϵͳ����Ϣͷ������ָ���Լ���������ָ����ĳ���
			
				 sendMessageHead.iCommand = RESET_PROCESS;
				 sendMessageHead.iMessageLen = COMMANDSIZE;
				 ClientTCPWrite(gTCPConnection, &sendMessageHead, MESSAGEHEADSIZE, 1000);         //��������ͷ
				 Sleep(10);  
		 
				 Hctrlparadown();     
				 InitHILTestPara();
				 ClientTCPWrite(gTCPConnection, pExMem, COMMANDSIZE, 5000);		    //��������   
				 SetCtrlAttribute(HILPanelHandle,HILPNL_PB_START,ATTR_LABEL_TEXT,"����");
				
			}
			
		}
	}
Error:	
	
	return 0;
}

 
int CVICALLBACK HILDownParaCallback (int panel, int control, int event,
		void *callbackData, int eventData1, int eventData2)
{
	switch (event)
	{
		case EVENT_COMMIT:
			sendMessageHead.iCommand = SETPARA_PROCESS; 
			sendMessageHead.iMessageLen = COMMANDSIZE;
			ClientTCPWrite(gTCPConnection, &sendMessageHead, MESSAGEHEADSIZE, 1000);         //��������ͷ
			Hctrlparadown(); 
			ClientTCPWrite(gTCPConnection, pExMem, COMMANDSIZE, 5000);		    //��������Ͳ���
			
			break;
	}
	return 0;
}

int CVICALLBACK HILOliDisplay (int panel, int control, int event,
							   void *callbackData, int eventData1, int eventData2)
{
	switch (event)
	{
		case EVENT_COMMIT:
			DisplayHPSPanel();   
			break;
	}
	return 0;
}

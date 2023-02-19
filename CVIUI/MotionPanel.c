#include <cvirte.h>		
#include <userint.h>
#include "MotionPanel.h"
#include "Functions.h"
#include "ExtraVar.h"
//#include <rtutil.h>
#include <tcpsupp.h> 
#include <formatio.h>
#include <utility.h>
#include <ansi_c.h>

extern char DftPathSign; //Ĭ��·����־λ
extern char pathpass[];  //·���������
extern char originalkey[] ;
extern int keyerr ;
extern int nm=0;   
static int MotionPanelHandle;

#define ANY_CHART_TRACE              1
#define FRE_SCAN_TEST     	         2
#define ANALOGY_FRE_SCAN_TEST     	 9   
#define  READBUFFERLEN	  1024    

TESTDATA ReadDataMessage;
//TESTDATANEW  ReadDataMessageBuff[READBUFFERLEN];  //�������µķ������ݷ�ʽ������16λ���ֽ���
//int BufferWriteIndex=0;
//int BufferReadIndex=0;
 
MessageHead sendMessageHead; 
MessageHead readMessageHead;  
CommandMessage sendCommandMessage; 
CommandMessage readCommandMessage; 

//static unsigned int 	gCommandConnection				= TCP_INVALID_CONNECTION;  
static int 				gLock							= 0;

static int CVICALLBACK TCPDataCallback (unsigned handle, int xType, int errCode, void *cbData);   
int transdatasizeM = 36;


//�˶���ʾ����ʼ��
int MotionPanelInit(void)
{
	if ((MotionPanelHandle = LoadPanel (0, "MotionPanel.uir", MOTIONPNL)) < 0)
	  return -1;
	
	if (ConnectToTCPServer (&gTCPConnection, HOST_PORT, HOST_ADDRESS, TCPDataCallback, 0, 2000) < 0)
	{
		//SetCtrlVal(runningpanel,RUNPANEL_MSGSTRING,"����ʵʱ����ʧ�ܣ���ȷ��ʵʱ�����Ѿ����У�����������������");	
		MessagePopup ("Error", "����ʵʱ����ʧ�ܣ���ȷ��ʵʱ�����Ѿ����У�����������������");    
		goto Error;
	}
	else
	{
		//MessagePopup ("OK", "����ʵʱ����ok");       	       
	}

	//�˶���ʾ��ʼ��
	InitMotionCtrlPara();
	InitMotionDisIndex();   
	DisplayPanel (MotionPanelHandle);
	RunUserInterface ();
	
	DiscardPanel (MotionPanelHandle);
	
	if (gTCPConnection != TCP_INVALID_CONNECTION)
		DisconnectFromTCPServer (gTCPConnection);
	
	return 1;
Error:
	return -1;
}

void Mctrlparadown(void)
{
	double dTemp;
	int i;
	GetPanelHandleFromTabPage (MotionPanelHandle, MOTIONPNL_MOT_TAB,1,&tabpanel1);
	for (i=0; i<SIDE_NO; i++)
	{
		//ͨ���Ŀ��Ʋ���
		GetPanelHandleFromTabPage(tabpanel1, PARA_TAB_CTRL_PARA_TAB,i,&tabpanel2);
		GetCtrlVal(tabpanel2,CH1_P_TAB_NUM_MOTION_KAP,&dTemp);
		pExMem->ctrlPara[i+1].Kap = dTemp;
		GetCtrlVal(tabpanel2,CH1_P_TAB_NUM_MOTION_KAI,&dTemp);
		pExMem->ctrlPara[i+1].Kai = dTemp;
		GetCtrlVal(tabpanel2,CH1_P_TAB_NUM_MOTION_KAD,&dTemp);
		pExMem->ctrlPara[i+1].Kad = dTemp;
		GetCtrlVal(tabpanel2,CH1_P_TAB_NUM_MOTION_FA1,&dTemp);
		pExMem->ctrlPara[i+1].Fa1 = dTemp;
		GetCtrlVal(tabpanel2,CH1_P_TAB_NUM_MOTION_FA2,&dTemp);
		pExMem->ctrlPara[i+1].Fa2 = dTemp;
		GetCtrlVal(tabpanel2,CH1_P_TAB_NUM_MOTION_FAK,&dTemp);
		pExMem->ctrlPara[i+1].Fak = dTemp;
	}			
			
}

void InitMotionCtrlSet(void)
{
    GetPanelHandleFromTabPage (MotionPanelHandle, MOTIONPNL_MOT_TAB, 1, &tabpanel1);
	int i;
	for (i = 0; i < SIDE_NO; i++)
	{
		GetPanelHandleFromTabPage (tabpanel1, PARA_TAB_CTRL_PARA_TAB, i, &tabpanel2);
		SetCtrlVal(tabpanel2,CH1_P_TAB_NUM_MOTION_KAP,pExMem->ctrlPara[i+1].Kap);
		SetCtrlVal(tabpanel2,CH1_P_TAB_NUM_MOTION_KAI,pExMem->ctrlPara[i+1].Kai);
		SetCtrlVal(tabpanel2,CH1_P_TAB_NUM_MOTION_KAD,pExMem->ctrlPara[i+1].Kad);
		SetCtrlVal(tabpanel2,CH1_P_TAB_NUM_MOTION_FA1,pExMem->ctrlPara[i+1].Fa1);
		SetCtrlVal(tabpanel2,CH1_P_TAB_NUM_MOTION_FA2,pExMem->ctrlPara[i+1].Fa2);
		SetCtrlVal(tabpanel2,CH1_P_TAB_NUM_MOTION_FAK,pExMem->ctrlPara[i+1].Fak);
	}
	
  //2022/11/25	Replace enumeration with a for loop. lxl
}   

void InitMotionTestPara(void)
{
	double dTemp;
	int tempInt;
	int i;
	
	
	GetCtrlVal(MotionPanelHandle,MOTIONPNL_CH_TEST_TIME,&dTemp); 
	for (i=0; i<SIDE_NO; i++)
	{
		pExMem->testPara[i+1].dTestTime = dTemp;
	}
	GetCtrlVal(MotionPanelHandle,MOTIONPNL_CH_TEST_TYPE,&tempInt); 
	for (i=0; i<SIDE_NO; i++)
	{
		pExMem->testPara[i+1].iTestType = tempInt; 
	}
	
	GetPanelHandleFromTabPage (MotionPanelHandle, MOTIONPNL_MOT_TAB,0,&tabpanel1);
    for (i=0; i<SIDE_NO; i++)
	{
		GetPanelHandleFromTabPage(tabpanel1, SET_TAB_CH_TAB,i,&tabpanel2);
	
		GetCtrlVal(tabpanel2,CH1_TAB_CH_AMP,&dTemp);
		pExMem->testPara[i+1].dAmp = dTemp;
		GetCtrlVal(tabpanel2,CH1_TAB_CH_OFFSET,&dTemp);
		pExMem->testPara[i+1].dOffset = dTemp;
		GetCtrlVal(tabpanel2,CH1_TAB_CH_FREQ,&dTemp);    
		pExMem->testPara[i+1].dFreq = dTemp; 
		//GetCtrlVal(tabpanel2,CH1_TAB_CH_START_FREQ,&dTemp);    
		//pExMem->testPara[i+1].dStartFreq = dTemp; 
		GetCtrlVal(tabpanel2,CH1_TAB_CH_STOP_FREQ,&dTemp);    
		pExMem->testPara[i+1].dStopFreq = dTemp;
		GetCtrlVal(tabpanel2,CH1_TAB_CH_ON_OFF,&tempInt); 
		if(tempInt == 1)
		{
			pExMem->testPara[i+1].iStatus = 1;  		  //1��ʾλ��ͨ����2��ʾ����ͨ��
		}
		else
		{
			pExMem->testPara[i+1].iStatus = 0;  		  //1��ʾλ��ͨ����2��ʾ����ͨ��
		}	
		GetCtrlVal(tabpanel2,CH1_TAB_CH_WAVE,&tempInt); 
		pExMem->testPara[i+1].iWave = tempInt; 

	}

	
}

//������Ļ��ʾ������
void MotionSetScreenCycNum(void)
{
	double dfreq;	   //Ԥ��Ƶ��
	double dring_val;  //��������
	long N;   //ÿ����ʾ����
	int i,ringindex;
	
	for(i=1;i<5;i++)
	{
		if(pExMem->testPara[i].iStatus && (chart1_ch == i))
		{
			N = (long)(2000 / (pExMem->testPara[i].dFreq) * CYCLE_NUM_PER_SCREEN);  //��Ļ��ʾ�������㹫ʽ
			if(N >= 3 && N <= 10000)
			{
				SetCtrlAttribute(MotionPanelHandle,MOTIONPNL_STRIPCHART_1,ATTR_POINTS_PER_SCREEN,N);
				SetCtrlAttribute(MotionPanelHandle,MOTIONPNL_STRIPCHART_2,ATTR_POINTS_PER_SCREEN,N);
				SetCtrlAttribute(MotionPanelHandle,MOTIONPNL_STRIPCHART_3,ATTR_POINTS_PER_SCREEN,N);
				SetCtrlAttribute(MotionPanelHandle,MOTIONPNL_STRIPCHART_4,ATTR_POINTS_PER_SCREEN,N);
			}
			
			for(ringindex = 0;ringindex < 11;ringindex++)  //����ֵ������Χ������y����
			{
				GetValueFromIndex(MotionPanelHandle,MOTIONPNL_CHART1_YRANGE_RING,ringindex,&dring_val);
				if(pExMem->testPara[i].dAmp < dring_val)
				{
					SetCtrlIndex(MotionPanelHandle,MOTIONPNL_CHART1_YRANGE_RING,ringindex);
					//SetCtrlIndex(MotionPanelHandle,MOTIONPNL_CHART2_YRANGE_RING,ringindex);
					SetAxisRange(MotionPanelHandle,MOTIONPNL_STRIPCHART_1,VAL_NO_CHANGE,0.0,1.0,VAL_MANUAL,-dring_val,dring_val);
					SetAxisRange(MotionPanelHandle,MOTIONPNL_STRIPCHART_2,VAL_NO_CHANGE,0.0,1.0,VAL_MANUAL,-dring_val,dring_val);
					break;
				}
			}
		}
	}
	
	
}

 //��ʼ����ʾ����
void InitMotionDisIndex(void)
{
	char tempLabel[64];
	//��ʼ����ʾ����
	//tempLabel = " ";
	
	GetCtrlVal(MotionPanelHandle,MOTIONPNL_CHART1_CH_RING,&chart1_ch);
	/*
	GetCtrlVal(MotionPanelHandle,MOTIONPNL_CHART1_CONTENT_RING,&chart1_content);
	strcpy(tempLabel,""); 
	strcat(tempLabel,contentLabel[chart1_content]);
	SetCtrlAttribute(MotionPanelHandle,MOTIONPNL_STRIPCHART_1,ATTR_LABEL_TEXT,tempLabel);
	SetCtrlAttribute(MotionPanelHandle,MOTIONPNL_STRIPCHART_1,ATTR_LABEL_JUSTIFY,VAL_LEFT_JUSTIFIED); 
	*/

	GetCtrlVal(MotionPanelHandle,MOTIONPNL_CHART2_CH_RING,&chart2_ch);
	GetCtrlVal(MotionPanelHandle,MOTIONPNL_CHART2_CONTENT_RING,&chart2_content);
	strcpy(tempLabel,""); 
	strcat(tempLabel,contentLabel[chart2_content]);
	SetCtrlAttribute(MotionPanelHandle,MOTIONPNL_STRIPCHART_2,ATTR_LABEL_TEXT,tempLabel);
	//SetCtrlAttribute(MotionPanelHandle,MOTIONPNL_STRIPCHART_2,ATTR_LABEL_JUSTIFY,VAL_LEFT_JUSTIFIED); 
	
	GetCtrlVal(MotionPanelHandle,MOTIONPNL_CHART3_CH_RING,&chart3_ch);
	/*
	GetCtrlVal(MotionPanelHandle,MOTIONPNL_CHART3_CONTENT_RING,&chart3_content);
	strcpy(tempLabel,""); 
	strcat(tempLabel,contentLabel[chart3_content]);
	SetCtrlAttribute(MotionPanelHandle,MOTIONPNL_STRIPCHART_3,ATTR_LABEL_TEXT,tempLabel);
	SetCtrlAttribute(MotionPanelHandle,MOTIONPNL_STRIPCHART_3,ATTR_LABEL_JUSTIFY,VAL_LEFT_JUSTIFIED); 
	*/
	
	GetCtrlVal(MotionPanelHandle,MOTIONPNL_CHART4_CH_RING,&chart4_ch);
	GetCtrlVal(MotionPanelHandle,MOTIONPNL_CHART4_CONTENT_RING,&chart4_content);
	strcpy(tempLabel,""); 
	strcat(tempLabel,contentLabel[chart4_content==5?chart4_content+1:chart4_content]);
	SetCtrlAttribute(MotionPanelHandle,MOTIONPNL_STRIPCHART_4,ATTR_LABEL_TEXT,tempLabel);
	//SetCtrlAttribute(MotionPanelHandle,MOTIONPNL_STRIPCHART_4,ATTR_LABEL_JUSTIFY,VAL_LEFT_JUSTIFIED);
	
	
}

//��ʼ�����Ʋ���
void InitMotionCtrlPara(void)
{
	double dTemp;
	int i;
	char sectionNam[512];  
	//��ȡini�ļ����
	inihandle = Ini_New(0);
	//��ȡini�ļ�
	//GetWindowsDirs (windowsDirectory, systemDirectory);
	//strcpy( pathname ,windowsDirectory);
	
	GetDir (currentDir);
	strcpy( pathname ,currentDir);
	strcat(pathname,"\\MotionPara.ini");
	
	inierr = Ini_ReadFromFile(inihandle, pathname); 
	if( inierr < 0)
	{
		sprintf(msgBuff,"�󿪲����ļ�����err = %d",inierr);
		MessagePopup ("Err",msgBuff);  
		//���ļ�ʧ��
	}
	else
	{
		for (i=0; i<SIDE_NO+AXIS_NO ; i++)
		{
			//ͨ��1---kap\kai\kadλ��PID����
			strcpy(sectionNam,"0CHANNEL");
			sectionNam[0] = i+ '0'; 
			if(Ini_GetDouble (inihandle,sectionNam,"Kap", &dTemp)>0)
			{
				pExMem->ctrlPara[i+1].Kap = dTemp;
			}
			else 						 
			{
				 pExMem->ctrlPara[i+1].Kap = 0.0;
			}
	
			if(Ini_GetDouble (inihandle, sectionNam,"Kai", &dTemp)>0)
			{
				pExMem->ctrlPara[i+1].Kai = dTemp;
			}
			else 
			{
				 pExMem->ctrlPara[i+1].Kai = 0.0;
			}
	
			if(Ini_GetDouble (inihandle, sectionNam,"Kad", &dTemp)>0)
			{
				pExMem->ctrlPara[i+1].Kad = dTemp;
			}
			else 
			{
				 pExMem->ctrlPara[i+1].Kad = 0.0;
			}
			//����ģ������-----fa1\fa2\fak
			if(Ini_GetDouble (inihandle, sectionNam,"Fa1", &dTemp)>0)
			{
				pExMem->ctrlPara[i+1].Fa1 = dTemp;
			}
			else 
			{
				 pExMem->ctrlPara[i+1].Fa1 = 0.0;
			}
	
			if(Ini_GetDouble (inihandle, sectionNam,"Fa2", &dTemp)>0)
			{
				pExMem->ctrlPara[i+1].Fa2 = dTemp;
			}
			else 
			{
				 pExMem->ctrlPara[i+1].Fa2 = 0.0;
			}
	
			if(Ini_GetDouble (inihandle, sectionNam,"Fak", &dTemp)>0)
			{
				pExMem->ctrlPara[i+1].Fak = dTemp;
			}
			else 
			{
				 pExMem->ctrlPara[i+1].Fak = 0.0;
			}
		}
		
		
		//ͨ��2
	
	}
	
	InitMotionCtrlSet();
}

//ʹ�ܿ��Ʋ�������
void LightMotionParaSet(void)
{
	int i;
	GetPanelHandleFromTabPage (MotionPanelHandle, MOTIONPNL_MOT_TAB,1,&tabpanel1);
	 
	//ͨ��1�Ŀ��Ʋ���
	for (i=0;i<SIDE_NO; i++)
	{
		GetPanelHandleFromTabPage(tabpanel1, PARA_TAB_CTRL_PARA_TAB,i,&tabpanel2);
		SetCtrlAttribute(tabpanel2,CH1_P_TAB_NUM_MOTION_KAP,ATTR_DIMMED,0);
		SetCtrlAttribute(tabpanel2,CH1_P_TAB_NUM_MOTION_KAI,ATTR_DIMMED,0);
		SetCtrlAttribute(tabpanel2,CH1_P_TAB_NUM_MOTION_KAD,ATTR_DIMMED,0);
		SetCtrlAttribute(tabpanel2,CH1_P_TAB_NUM_MOTION_FA1,ATTR_DIMMED,0);
		SetCtrlAttribute(tabpanel2,CH1_P_TAB_NUM_MOTION_FA2,ATTR_DIMMED,0);
		SetCtrlAttribute(tabpanel2,CH1_P_TAB_NUM_MOTION_FAK,ATTR_DIMMED,0);
	}
	

}
 
///////////////////////////////�����ǻص�����

int CVICALLBACK MotionPanelCallback (int panel, int event, void *callbackData,
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

int CVICALLBACK MotionYRangeChangeCallback (int panel, int control, int event,
		void *callbackData, int eventData1, int eventData2)
{
	double dTemp;
	switch (event)
	{
		case EVENT_COMMIT:

			switch(control)
			{
				case MOTIONPNL_CHART1_YRANGE_RING:
					GetCtrlVal(panel,control,&dTemp);
					SetAxisRange(panel,MOTIONPNL_STRIPCHART_1,VAL_NO_CHANGE,0.0,1.0,VAL_MANUAL,-dTemp,dTemp);
					break;
				case MOTIONPNL_CHART2_YRANGE_RING:
					GetCtrlVal(panel,control,&dTemp);
					SetAxisRange(panel,MOTIONPNL_STRIPCHART_2,VAL_NO_CHANGE,0.0,1.0,VAL_MANUAL,-dTemp,dTemp);
					break;
				case MOTIONPNL_CHART3_YRANGE_RING:
					GetCtrlVal(panel,control,&dTemp);
					SetAxisRange(panel,MOTIONPNL_STRIPCHART_3,VAL_NO_CHANGE,0.0,1.0,VAL_MANUAL,-dTemp,dTemp);
					break;
				case MOTIONPNL_CHART4_YRANGE_RING:
					GetCtrlVal(panel,control,&dTemp);
					SetAxisRange(panel,MOTIONPNL_STRIPCHART_4,VAL_NO_CHANGE,0.0,1.0,VAL_MANUAL,-dTemp,dTemp);
					break;
				
			}  
			
			break;
	}
	return 0;
}

int CVICALLBACK MotionSaveCallback (int panel, int control, int event,
		void *callbackData, int eventData1, int eventData2)
{
	switch (event)
	{
		case EVENT_COMMIT:
			strcpy(saveDir,"D:\\��������" );
			err = MakeDir (saveDir);
			strcat(saveDir,"\\" );
			strcat(saveDir, DateStr());
			err = MakeDir (saveDir);
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

int CVICALLBACK MotionRecordCallback (int panel, int control, int event,
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

int CVICALLBACK MotionExitCallback (int panel, int control, int event,
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
				ClientTCPWrite(gTCPConnection, &sendMessageHead, MESSAGEHEADSIZE, 1000);         //��������ͷ
				
				//DisconnectTCPClient (gTCPConnection);
				
				/*
				pExMem->requestType = EXIT_PROCESS; 
				pExMem->request  = IS_REQUEST;
				*/
				QuitUserInterface(0);  
			}
			break;
	}
	return 0;
}
 
int CVICALLBACK MotionResetCallback (int panel, int control, int event,
		void *callbackData, int eventData1, int eventData2)
{
	switch (event)
	{
		
		
		case EVENT_COMMIT:
			//��ָ����Ϣ���͸�ʵʱϵͳ����Ϣͷ
			sendMessageHead.iCommand = RESET_PROCESS;
			sendMessageHead.iMessageLen = COMMANDSIZE;
			ClientTCPWrite(gTCPConnection, &sendMessageHead, MESSAGEHEADSIZE, 1000);         //��������ͷ
			Sleep(10);  
			 
			Mctrlparadown();     
			InitMotionTestPara();
			ClientTCPWrite(gTCPConnection, pExMem, COMMANDSIZE, 5000);		    //��������   
			
			
			break;
	}
	return 0;
}

int MotionStartCallback  (int panel, int control, int event,
		void *callbackData, int eventData1, int eventData2)
{
	double dTemp;
	int tempInt;
	int SendBuffLen;
	
	switch (event)
	{
		case EVENT_COMMIT:
			if(ConfirmPopup ("ȷ������", "ȷ��������"))
			{
				MotionSetScreenCycNum();  //������Ļ��ʾ���� 
				 //��ָ����Ϣ���͸�ʵʱϵͳ����Ϣͷ������ָ���Լ���������ָ����ĳ��� 
			
			
				sendMessageHead.iCommand = START_PROCESS;
				sendMessageHead.iMessageLen = COMMANDSIZE;
				ClientTCPWrite(gTCPConnection, &sendMessageHead, MESSAGEHEADSIZE, 1000);         //��������ͷ
				
				Sleep(10); 
				
				Mctrlparadown();   
				InitMotionTestPara();	  //��ʼ�����Բ���
				ClientTCPWrite(gTCPConnection, pExMem, COMMANDSIZE, 5000);		    //�������� 
				}

			break;
	}
	return 0;
}

int CVICALLBACK MotionCh1TesttypeChange (int panel, int control, int event,
		void *callbackData, int eventData1, int eventData2)
{
	switch (event)
	{
		int iTemp;
		int i;
		
		case EVENT_COMMIT:
			GetCtrlVal(panel,control,&iTemp);
			
			switch(iTemp)
			{
				 
				case FRE_SCAN_TEST:
					for (i=0 ;i < SIDE_NO  ; i++)   
					{
						GetPanelHandleFromTabPage (MotionPanelHandle, MOTIONPNL_MOT_TAB,0,&tabpanel1);
						GetPanelHandleFromTabPage(tabpanel1, SET_TAB_CH_TAB,i,&tabpanel2);
						SetCtrlAttribute(tabpanel2,CH1_TAB_CH_WAVE,ATTR_DIMMED,1); 
						SetCtrlAttribute(tabpanel2,CH1_TAB_CH_FREQ,ATTR_DIMMED,1);
						SetCtrlAttribute(tabpanel2,CH1_TAB_CH_AMP,ATTR_DIMMED,0);
						SetCtrlAttribute(tabpanel2,CH1_TAB_CH_SCAN_TYPE,ATTR_DIMMED,0); 
						SetCtrlAttribute(tabpanel2,CH1_TAB_CH_START_FREQ,ATTR_DIMMED,0); 
						SetCtrlAttribute(tabpanel2,CH1_TAB_CH_STOP_FREQ,ATTR_DIMMED,0);
					}
					break;
				case ANY_CHART_TRACE:
					for (i=0 ;i < SIDE_NO  ; i++)   
					{
						GetPanelHandleFromTabPage (MotionPanelHandle, MOTIONPNL_MOT_TAB,0,&tabpanel1);
						GetPanelHandleFromTabPage(tabpanel1, SET_TAB_CH_TAB,i,&tabpanel2);
						SetCtrlAttribute(tabpanel2,CH1_TAB_CH_WAVE,ATTR_DIMMED,0); 
						SetCtrlAttribute(tabpanel2,CH1_TAB_CH_FREQ,ATTR_DIMMED,0);
						SetCtrlAttribute(tabpanel2,CH1_TAB_CH_AMP,ATTR_DIMMED,0);
						SetCtrlAttribute(tabpanel2,CH1_TAB_CH_SCAN_TYPE,ATTR_DIMMED,1); 
						SetCtrlAttribute(tabpanel2,CH1_TAB_CH_START_FREQ,ATTR_DIMMED,1); 
						SetCtrlAttribute(tabpanel2,CH1_TAB_CH_STOP_FREQ,ATTR_DIMMED,1);
					}
					break; 
			}
			 
			break;
	}
	return 0;
}

int CVICALLBACK MotionCh2TesttypeChange (int panel, int control, int event,
		void *callbackData, int eventData1, int eventData2)
{
	switch (event)
	{
		int iTemp;
		
		case EVENT_COMMIT:
			GetCtrlVal(panel,control,&iTemp);
			
			switch(iTemp)
			{
				case FRE_SCAN_TEST:
					SetCtrlAttribute(panel,CH2_TAB_CH_WAVE,ATTR_DIMMED,1); 
					SetCtrlAttribute(panel,CH2_TAB_CH_FREQ,ATTR_DIMMED,1);
					SetCtrlAttribute(panel,CH2_TAB_CH_AMP,ATTR_DIMMED,0);
					SetCtrlAttribute(panel,CH2_TAB_CH_SCAN_TYPE,ATTR_DIMMED,0); 
					SetCtrlAttribute(panel,CH2_TAB_CH_START_FREQ,ATTR_DIMMED,0); 
					SetCtrlAttribute(panel,CH2_TAB_CH_STOP_FREQ,ATTR_DIMMED,0);
					break;
				case ANY_CHART_TRACE:
					SetCtrlAttribute(panel,CH2_TAB_CH_WAVE,ATTR_DIMMED,0); 
					SetCtrlAttribute(panel,CH2_TAB_CH_FREQ,ATTR_DIMMED,0);
					SetCtrlAttribute(panel,CH2_TAB_CH_AMP,ATTR_DIMMED,0);
					SetCtrlAttribute(panel,CH2_TAB_CH_SCAN_TYPE,ATTR_DIMMED,1); 
					SetCtrlAttribute(panel,CH2_TAB_CH_START_FREQ,ATTR_DIMMED,1); 
					SetCtrlAttribute(panel,CH2_TAB_CH_STOP_FREQ,ATTR_DIMMED,1);
					break; 
			}
			 
			break;
	}
	return 0;
}

int CVICALLBACK MotionCh3TesttypeChange (int panel, int control, int event,
		void *callbackData, int eventData1, int eventData2)
{
	switch (event)
	{
		int iTemp;
		
		case EVENT_COMMIT:
			GetCtrlVal(panel,control,&iTemp);
			
			switch(iTemp)
			{
				case FRE_SCAN_TEST:
					SetCtrlAttribute(panel,CH3_TAB_CH_WAVE,ATTR_DIMMED,1); 
					SetCtrlAttribute(panel,CH3_TAB_CH_FREQ,ATTR_DIMMED,1);
					SetCtrlAttribute(panel,CH3_TAB_CH_AMP,ATTR_DIMMED,0);
					SetCtrlAttribute(panel,CH3_TAB_CH_SCAN_TYPE,ATTR_DIMMED,0); 
					SetCtrlAttribute(panel,CH3_TAB_CH_START_FREQ,ATTR_DIMMED,0); 
					SetCtrlAttribute(panel,CH3_TAB_CH_STOP_FREQ,ATTR_DIMMED,0);
					break;
				case ANY_CHART_TRACE:
					SetCtrlAttribute(panel,CH3_TAB_CH_WAVE,ATTR_DIMMED,0); 
					SetCtrlAttribute(panel,CH3_TAB_CH_FREQ,ATTR_DIMMED,0);
					SetCtrlAttribute(panel,CH3_TAB_CH_AMP,ATTR_DIMMED,0);
					SetCtrlAttribute(panel,CH3_TAB_CH_SCAN_TYPE,ATTR_DIMMED,1); 
					SetCtrlAttribute(panel,CH3_TAB_CH_START_FREQ,ATTR_DIMMED,1); 
					SetCtrlAttribute(panel,CH3_TAB_CH_STOP_FREQ,ATTR_DIMMED,1);
					break; 
			}
			 
			break;
	}
	return 0;
}

int CVICALLBACK MotionCh4TesttypeChange (int panel, int control, int event,
		void *callbackData, int eventData1, int eventData2)
{
	switch (event)
	{
		int iTemp;
		
		case EVENT_COMMIT:
			GetCtrlVal(panel,control,&iTemp);
			
			switch(iTemp)
			{
				case FRE_SCAN_TEST:
					SetCtrlAttribute(panel,CH4_TAB_CH_WAVE,ATTR_DIMMED,1); 
					SetCtrlAttribute(panel,CH4_TAB_CH_FREQ,ATTR_DIMMED,1);
					SetCtrlAttribute(panel,CH4_TAB_CH_AMP,ATTR_DIMMED,0);
					SetCtrlAttribute(panel,CH4_TAB_CH_SCAN_TYPE,ATTR_DIMMED,0); 
					SetCtrlAttribute(panel,CH4_TAB_CH_START_FREQ,ATTR_DIMMED,0); 
					SetCtrlAttribute(panel,CH4_TAB_CH_STOP_FREQ,ATTR_DIMMED,0);
					break;
				case ANY_CHART_TRACE:
					SetCtrlAttribute(panel,CH4_TAB_CH_WAVE,ATTR_DIMMED,0); 
					SetCtrlAttribute(panel,CH4_TAB_CH_FREQ,ATTR_DIMMED,0);
					SetCtrlAttribute(panel,CH4_TAB_CH_AMP,ATTR_DIMMED,0);
					SetCtrlAttribute(panel,CH4_TAB_CH_SCAN_TYPE,ATTR_DIMMED,1); 
					SetCtrlAttribute(panel,CH4_TAB_CH_START_FREQ,ATTR_DIMMED,1); 
					SetCtrlAttribute(panel,CH4_TAB_CH_STOP_FREQ,ATTR_DIMMED,1);
					break; 
			}
			 
			break;
	}
	return 0;
}

int CVICALLBACK MotionSaveParaCallback (int panel, int control, int event,
		void *callbackData, int eventData1, int eventData2)
{
	double dTemp;
	char sectionNam[512];  
	int i;
	
	switch (event)
	{
		
		case EVENT_COMMIT:
			
				if(ConfirmPopup("Confirm Popup","�����������������滻ԭ�ڴ��в�����"))
				{
					GetPanelHandleFromTabPage (MotionPanelHandle, MOTIONPNL_MOT_TAB,1,&tabpanel1);
					for (i=0 ;i < SIDE_NO  ; i++)
					{
						strcpy(sectionNam,"0CHANNEL");
						sectionNam[0] = i+ '0'; 
						//ͨ���Ŀ��Ʋ���
						GetPanelHandleFromTabPage(tabpanel1, PARA_TAB_CTRL_PARA_TAB,i,&tabpanel2);
						GetCtrlVal(tabpanel2,CH1_P_TAB_NUM_MOTION_KAP,&dTemp);
						Ini_PutDouble(inihandle,sectionNam,"Kap",dTemp);
						GetCtrlVal(tabpanel2,CH1_P_TAB_NUM_MOTION_KAI,&dTemp);
						Ini_PutDouble(inihandle,sectionNam,"Kai",dTemp); 
						GetCtrlVal(tabpanel2,CH1_P_TAB_NUM_MOTION_KAD,&dTemp);
						Ini_PutDouble(inihandle,sectionNam,"Kad",dTemp);
						GetCtrlVal(tabpanel2,CH1_P_TAB_NUM_MOTION_FA1,&dTemp);
						Ini_PutDouble(inihandle,sectionNam,"Fa1",dTemp);
						GetCtrlVal(tabpanel2,CH1_P_TAB_NUM_MOTION_FA2,&dTemp);
						Ini_PutDouble(inihandle,sectionNam,"Fa2",dTemp); 
						GetCtrlVal(tabpanel2,CH1_P_TAB_NUM_MOTION_FAK,&dTemp);
						Ini_PutDouble(inihandle,sectionNam,"Fak",dTemp); 
					}
					Ini_WriteToFile (inihandle, pathname);
				}
				else
				{}
			
			break;
	}
	return 0;
}

int CVICALLBACK MotionSetParaCallback (int panel, int control, int event,
		void *callbackData, int eventData1, int eventData2)
{
	double dTemp;
	int selbutton;
	char keyinputbuffer[65];
	
	switch (event)
	{
		case EVENT_COMMIT:
			/*if(keyerr)  //��������־λ
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
							if(ConfirmPopup("Confirm Popup","������ȷ���޸Ŀ��Ʋ�����Ӱ��ϵͳ�������ܣ������޸ģ�"))
							{
								keyerr = 0;
								LightMotionParaSet();	//light the paraset
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
			
			else
			{
				LightMotionParaSet();
				 	 
			}		 */ 
			
			LightMotionParaSet();	//light the paraset 
			break;
	}
	return 0;
}

 int CVICALLBACK MotionOilControlCallback (int panel, int control, int event,
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

int CVICALLBACK MotionDataReDisplay (int panel, int control, int event,
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
					confirm = ConfirmPopup("Confirm Popup","����ѡ����ȷ��ΪĬ�ϵ����ݻطų���?");
					
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
				//strcpy(pathpass,"");
				//strcat(pathpass,pathname);
				LaunchExecutable (pathpass);	//�����ⲿ���ݻط����
			}
			
			break;
	}
	return 0;
}

int CVICALLBACK MotionDisplayChangeCallback (int panel, int control, int event,
		void *callbackData, int eventData1, int eventData2)
{
	switch (event)
	{
		case EVENT_COMMIT:
			InitMotionDisIndex();
			break;
	}
	return 0;
}

int CVICALLBACK AMotionDisplayChangeCallback (int panel, int control, int event,
		void *callbackData, int eventData1, int eventData2)
{
	long N;
	int iTemp,ringindex;
	double dring_val;
	
	switch (event)
	{
		case EVENT_COMMIT:
			
			GetCtrlVal(panel,control,&iTemp);								//�����ĸ�strips��ʾͨ����ͬ
			SetCtrlVal(MotionPanelHandle,MOTIONPNL_CHART1_CH_RING,iTemp);
			SetCtrlVal(MotionPanelHandle,MOTIONPNL_CHART2_CH_RING,iTemp);
			SetCtrlVal(MotionPanelHandle,MOTIONPNL_CHART3_CH_RING,iTemp);
			SetCtrlVal(MotionPanelHandle,MOTIONPNL_CHART4_CH_RING,iTemp);
			
			InitMotionDisIndex();
			
			if(pExMem->testPara[chart1_ch].iStatus) //�����ѡͨ����״̬Ϊ��
			{
				N = (long)(2000 / (pExMem->testPara[chart1_ch].dFreq) * CYCLE_NUM_PER_SCREEN);  //��Ļ��ʾ�������㹫ʽ
				if(N >= 3 && N <= 10000)
				{
					SetCtrlAttribute(MotionPanelHandle,MOTIONPNL_STRIPCHART_1,ATTR_POINTS_PER_SCREEN,N);
					SetCtrlAttribute(MotionPanelHandle,MOTIONPNL_STRIPCHART_2,ATTR_POINTS_PER_SCREEN,N);
					SetCtrlAttribute(MotionPanelHandle,MOTIONPNL_STRIPCHART_3,ATTR_POINTS_PER_SCREEN,N);
					SetCtrlAttribute(MotionPanelHandle,MOTIONPNL_STRIPCHART_4,ATTR_POINTS_PER_SCREEN,N);
				}
				
				for(ringindex = 0;ringindex < 11;ringindex++)  //����ֵ������Χ������y����
				{
					GetValueFromIndex(MotionPanelHandle,MOTIONPNL_CHART1_YRANGE_RING,ringindex,&dring_val);
					if(pExMem->testPara[chart1_ch].dAmp < dring_val)
					{
						SetCtrlIndex(MotionPanelHandle,MOTIONPNL_CHART1_YRANGE_RING,ringindex);
						SetCtrlIndex(MotionPanelHandle,MOTIONPNL_CHART2_YRANGE_RING,ringindex);
						SetAxisRange(MotionPanelHandle,MOTIONPNL_STRIPCHART_1,VAL_NO_CHANGE,0.0,1.0,VAL_MANUAL,-dring_val,dring_val);
						SetAxisRange(MotionPanelHandle,MOTIONPNL_STRIPCHART_2,VAL_NO_CHANGE,0.0,1.0,VAL_MANUAL,-dring_val,dring_val);
						break;
					}
				}
			}
			
			break;
	}
	return 0;
}

int CVICALLBACK MotionTabChanelSelect (int panel, int control, int event,
		void *callbackData, int eventData1, int eventData2)
{
	long N;
	int iTemp,ringindex;
	double dring_val;
	
	switch (event)
	{
		case EVENT_ACTIVE_TAB_CHANGE:
										//eventData1 = tab index of the tab that was previously active.
										//eventData2 = tab index of the tab that the user selected.

			if(eventData1 != eventData2)
			{
				eventData2 += 1;
				SetCtrlVal(MotionPanelHandle,MOTIONPNL_CHART1_CH_RING,eventData2);
				SetCtrlVal(MotionPanelHandle,MOTIONPNL_CHART2_CH_RING,eventData2);
				SetCtrlVal(MotionPanelHandle,MOTIONPNL_CHART3_CH_RING,eventData2);
				SetCtrlVal(MotionPanelHandle,MOTIONPNL_CHART4_CH_RING,eventData2);
				
				InitMotionDisIndex();
				
				if(pExMem->testPara[chart1_ch].iStatus) //�����ѡͨ����״̬Ϊ��
				{
					N = (long)(2000 / (pExMem->testPara[chart1_ch].dFreq) * CYCLE_NUM_PER_SCREEN);  //��Ļ��ʾ�������㹫ʽ
					if(N >= 3 && N <= 10000)
					{
						SetCtrlAttribute(MotionPanelHandle,MOTIONPNL_STRIPCHART_1,ATTR_POINTS_PER_SCREEN,N);
						SetCtrlAttribute(MotionPanelHandle,MOTIONPNL_STRIPCHART_2,ATTR_POINTS_PER_SCREEN,N);
						SetCtrlAttribute(MotionPanelHandle,MOTIONPNL_STRIPCHART_3,ATTR_POINTS_PER_SCREEN,N);
						SetCtrlAttribute(MotionPanelHandle,MOTIONPNL_STRIPCHART_4,ATTR_POINTS_PER_SCREEN,N);
					}
				
					for(ringindex = 0;ringindex < 5;ringindex++)  //����ֵ������Χ������y����
					{
						GetValueFromIndex(MotionPanelHandle,MOTIONPNL_CHART1_YRANGE_RING,ringindex,&dring_val);
						if(pExMem->testPara[chart1_ch].dAmp < dring_val)
						{
							SetCtrlIndex(MotionPanelHandle,MOTIONPNL_CHART1_YRANGE_RING,ringindex);
							SetCtrlIndex(MotionPanelHandle,MOTIONPNL_CHART2_YRANGE_RING,ringindex);
							SetAxisRange(MotionPanelHandle,MOTIONPNL_STRIPCHART_1,VAL_NO_CHANGE,0.0,1.0,VAL_MANUAL,-dring_val,dring_val);
							SetAxisRange(MotionPanelHandle,MOTIONPNL_STRIPCHART_2,VAL_NO_CHANGE,0.0,1.0,VAL_MANUAL,-dring_val,dring_val);
							break;
						}
					}
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
			iDataLoops =  iDataLen /transdatasizeM;
		
			if(iCommand == 6)	   //��������ݣ��Ͷ������е�����
			{
				bytesToRead =  iDataLen*sizeof(double);  
				DataSize = bytesToRead;
				
				readData = (double *)malloc(iDataLen*sizeof(double));
				readBuff = (char *) malloc(iDataLen*sizeof(double));
				
				chart1 = (double *)malloc((sizeof(double)*iDataLoops) * 2);
				chart2 = (double *)malloc(sizeof(double)*iDataLoops);
				chart3 = (double *)malloc((sizeof(double)*iDataLoops) * 2);
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
					//��λ������������ÿ��������32��double�͵�������Ϊ4�飬0-3����ָ��ǽǶ�ָ�������ָ���ɲ�����Ŀ����
					//4-7��λ���ز�����8-11��Ϊ�ǶȲ�����12-15��λ���������ÿ�����͵���ռ4�������ͨ��������8����ʣ��ľ���������
					
					chart1[2*i] = readData[transdatasizeM*i+chart1_ch-1];	 //λ��ָ��
					chart1[2*i+1] = readData[transdatasizeM*i + 24+chart1_ch-1]; //λ�Ʋ���
					
					chart3[2*i] = 0;	 //����ָ��    
					chart3[2*i+1] = readData[transdatasizeM*i + 8+chart3_ch-1];	 //������       
				
					switch(chart2_content)
					{
						case ANGLE_SAMPLE_INDEX:
								chart2[i] = readData[transdatasizeM*i+28+chart2_ch-1];
							break;
						case ANGLE_GIVEN_INDEX:
								chart2[i] = readData[transdatasizeM*i+0+chart2_ch-1];  
							break;
						case TORQUE_SAMPLE_INDEX:
								chart2[i] = readData[transdatasizeM*i+8+chart2_ch-1];  
							break;
						case TORQUE_GIVEN_INDEX:
								chart2[i] = readData[transdatasizeM*i+0+chart2_ch-1];      
							break;
						case SERVO_OUT_INDEX:
								chart2[i] = readData[transdatasizeM*i+16+chart2_ch-1];      
							break;
						case ERROR_INDEX:
								chart2[i] = chart1[2*i]- chart1[2*i+1];   ///////////////
							break;
						default:
								chart2[i] = 0.0;
							break;
					}
				
					switch(chart4_content)
					{
						case ANGLE_SAMPLE_INDEX:
								chart4[i] = readData[transdatasizeM*i+28+chart4_ch-1];
							break;
						case ANGLE_GIVEN_INDEX:
								chart4[i] = readData[transdatasizeM*i+0+chart4_ch-1]; 
							break;
						case TORQUE_SAMPLE_INDEX:
								chart4[i] = readData[transdatasizeM*i+8+chart4_ch-1]; 
							break;
						case TORQUE_GIVEN_INDEX:
								chart4[i] = readData[transdatasizeM*i+0+chart4_ch-1];;
							break;
						case SERVO_OUT_INDEX:
								chart4[i] = readData[transdatasizeM*i+16+chart4_ch-1]; 
							break;
						case ERROR_INDEX:
								chart4[i] = chart3[2*i]- chart3[2*i+1];   ///////////////
							break;
						default:
								chart4[i] = 0.0;
							break;
					
				
					}
				}
				
				if(iDataLoops>0)
				{
					PlotStripChart(MotionPanelHandle,MOTIONPNL_STRIPCHART_1,chart1,2*iDataLoops,0,0,VAL_DOUBLE);
					PlotStripChart(MotionPanelHandle,MOTIONPNL_STRIPCHART_2,chart2,iDataLoops,0,0,VAL_DOUBLE);  
					PlotStripChart(MotionPanelHandle,MOTIONPNL_STRIPCHART_3,chart3,2*iDataLoops,0,0,VAL_DOUBLE);
					PlotStripChart(MotionPanelHandle,MOTIONPNL_STRIPCHART_4,chart4,iDataLoops,0,0,VAL_DOUBLE); 
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


int CVICALLBACK NetWatchDog1 (int panel, int control, int event,
		void *callbackData, int eventData1, int eventData2)
{
	switch (event)
	{
		case EVENT_TIMER_TICK:
			
			//��ָ����Ϣ���͸�ʵʱϵͳ����Ϣͷ������ָ���Լ���������ָ����ĳ���
			sendMessageHead.iCommand = WATCHDOG;   
			
			sendMessageHead.iMessageLen = 0;		 //Ҳû��ʵ�ʵ�����
		
			ClientTCPWrite(gTCPConnection, &sendMessageHead, MESSAGEHEADSIZE, 1000);         //��������ͷ

			break;
	}
	return 0;
}
 

int CVICALLBACK MotionDownParaCallback (int panel, int control, int event,
		void *callbackData, int eventData1, int eventData2)
{
	switch (event)
	{
		case EVENT_COMMIT:
			//��ָ����Ϣ���͸�ʵʱϵͳ����Ϣͷ������ָ���Լ���������ָ����ĳ���
			sendMessageHead.iCommand = SETPARA_PROCESS;  
			sendMessageHead.iMessageLen = COMMANDSIZE;
			ClientTCPWrite(gTCPConnection, &sendMessageHead, MESSAGEHEADSIZE, 1000);         //��������ͷ
			Mctrlparadown(); 
			ClientTCPWrite(gTCPConnection, pExMem, COMMANDSIZE, 5000);		    //��������Ͳ���
			
			break;
	}
	return 0;
}

int CVICALLBACK MEnablecallback (int panel, int control, int event,
		void *callbackData, int eventData1, int eventData2)
{
	double dTemp;
	int tempInt;
	int SendBuffLen;
	
	
	switch (event)
	{
		case EVENT_COMMIT:
			sendMessageHead.iCommand = ENABLE_PROCESS;
			sendMessageHead.iMessageLen = COMMANDSIZE; 
			ClientTCPWrite(gTCPConnection, &sendMessageHead, MESSAGEHEADSIZE, 1000);         //��������ͷ
			InitMotionTestPara();	  //��ʼ�����Բ���
			if (nm > 1)	   //��������ݣ��Ͷ������е�����
			{
				nm=nm-2;
			}
			//GetCtrlVal(tabpanel2,STAPANEL_LED,&n);
			/* 
			pExMem->testPara[1].iEnable = 1;//tempInt;  
			pExMem->testPara[2].iEnable = 2;//tempInt; 
			pExMem->testPara[3].iEnable = 2;//tempInt;  
			pExMem->testPara[4].iEnable = 2;//tempInt;     */  
			
			if (nm == 0)	   //��������ݣ��Ͷ������е�����
			{
				SetCtrlAttribute(panel,control,ATTR_LABEL_TEXT,"ֹͣʹ��"); 
				GetPanelHandleFromTabPage (MotionPanelHandle, MOTIONPNL_MOT_TAB,0,&tabpanel1);
	
				 //ͨ��1�Ĳ��Բ���
				 GetPanelHandleFromTabPage(tabpanel1, SET_TAB_CH_TAB,0,&tabpanel2);
				 GetCtrlVal(tabpanel2,CH1_TAB_CH_ON_OFF,&tempInt); 
				 pExMem->testPara[1].iEnable = tempInt+1; 
				  
				 //ͨ��2�Ĳ��Բ���
 				 GetPanelHandleFromTabPage(tabpanel1, SET_TAB_CH_TAB,1,&tabpanel2);
				 GetCtrlVal(tabpanel2,CH2_TAB_CH_ON_OFF,&tempInt); 
				 pExMem->testPara[2].iEnable = tempInt+1;
				 
				 //ͨ��3�Ĳ��Բ���
				 GetPanelHandleFromTabPage(tabpanel1, SET_TAB_CH_TAB,2,&tabpanel2);
				 GetCtrlVal(tabpanel2,CH3_TAB_CH_ON_OFF,&tempInt); 
				 pExMem->testPara[3].iEnable = tempInt+1;
				 
				 //ͨ��4�Ĳ��Բ���
				 GetPanelHandleFromTabPage(tabpanel1, SET_TAB_CH_TAB,3,&tabpanel2);
				 GetCtrlVal(tabpanel2,CH4_TAB_CH_ON_OFF,&tempInt); 
				 pExMem->testPara[4].iEnable = tempInt+1; 
				 
				 //InitStaticTestPara();			 //��ʼ�����Բ���   	 
				}
			 else
			 {
				 SetCtrlAttribute(panel,control,ATTR_LABEL_TEXT,"ʹ��");
				 GetPanelHandleFromTabPage (MotionPanelHandle, MOTIONPNL_MOT_TAB,0,&tabpanel1); 
				 //ͨ��1�Ĳ��Բ���
				 GetPanelHandleFromTabPage(tabpanel1, SET_TAB_CH_TAB,0,&tabpanel2);
				 GetCtrlVal(tabpanel2,CH1_TAB_CH_ON_OFF,&tempInt); 
				 pExMem->testPara[1].iEnable = 1; 
				  
				 //ͨ��2�Ĳ��Բ���
				 GetPanelHandleFromTabPage(tabpanel1, SET_TAB_CH_TAB,1,&tabpanel2);
				 GetCtrlVal(tabpanel2,CH2_TAB_CH_ON_OFF,&tempInt); 
				 pExMem->testPara[2].iEnable = 1; 
				 
				 //ͨ��3�Ĳ��Բ���
				 GetPanelHandleFromTabPage(tabpanel1, SET_TAB_CH_TAB,2,&tabpanel2);
				 GetCtrlVal(tabpanel2,CH3_TAB_CH_ON_OFF,&tempInt); 
				 pExMem->testPara[3].iEnable = 1; 
				 
				 //ͨ��4�Ĳ��Բ���
				 GetPanelHandleFromTabPage(tabpanel1, SET_TAB_CH_TAB,3,&tabpanel2);
				 GetCtrlVal(tabpanel2,CH4_TAB_CH_ON_OFF,&tempInt); 
				 pExMem->testPara[4].iEnable = 1; 
						
				// InitStaticTestPara();			 //��ʼ�����Բ���
			 }
			 
			ClientTCPWrite(gTCPConnection, pExMem, COMMANDSIZE, 5000);		    //�������� 
			nm=nm+1;

			break;
	}
	return 0;
}

#include <cvirte.h>		
#include <userint.h>
#include "StaticPanel.h"
#include "Functions.h"
#include "ExtraVar.h"
#include <tcpsupp.h>
#include <formatio.h>
#include <utility.h>
#include <ansi_c.h>

extern char DftPathSign; //Ĭ��·����־λ
extern char pathpass[];  //·���������
extern char originalkey[];
extern int keyerr;
extern int ns=0; 
static int StaticPanelHandle;

#define ANY_CHART_TRACE              1
#define FRE_SCAN_TEST     	         2
#define ANALOGY_FRE_SCAN_TEST     	 9   

#define  READBUFFERLEN	  1024    

TESTDATA ReadDataMessage;
//TESTDATANEW  ReadDataMessageBuff[READBUFFERLEN];  //�������µķ������ݷ�ʽ������16λ���ֽ���
int BufferWriteIndex=0;
int BufferReadIndex=0;

MessageHead sendMessageHead; 
MessageHead readMessageHead;  
CommandMessage sendCommandMessage; 
CommandMessage readCommandMessage; 
 
//static unsigned int 	gCommandConnection				= TCP_INVALID_CONNECTION;  
static int 				gLock							= 0;

static int CVICALLBACK TCPDataCallback (unsigned handle, int xType, int errCode, void *cbData);   
int transdatasizeS = 36;


//��ֹ������������
int StaticPanelInit(void)
{
	
	if ((StaticPanelHandle = LoadPanel (0, "StaticPanel.uir", STAPANEL)) < 0)
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
	
	
	InitStaticCtrlPara();
	InitStaticDisIndex();   
	DisplayPanel (StaticPanelHandle);
	RunUserInterface ();
	
	

	

    DiscardPanel (StaticPanelHandle);      
	return 1;
Error:
    /* error operation */
	

Done: 
	/* clean up */   
	if (gTCPConnection != TCP_INVALID_CONNECTION)
		DisconnectFromTCPServer (gTCPConnection);
}

//��ֹ���ؿ��Ʋ�����ʼ��
void InitStaticCtrlSet(void)
{
    GetPanelHandleFromTabPage (StaticPanelHandle, STAPANEL_STA_TAB, 1, &tabpanel1);
	
	//ͨ��1�Ŀ��Ʋ���
	GetPanelHandleFromTabPage (tabpanel1, PARA_TAB_CTRL_PARA_TAB, 0, &tabpanel2);
	SetCtrlVal(tabpanel2,CH1_P_TAB_NUM_STATIC_KTP,pExMem->ctrlPara[1].Ktp);
	SetCtrlVal(tabpanel2,CH1_P_TAB_NUM_STATIC_KTI,pExMem->ctrlPara[1].Kti);
	SetCtrlVal(tabpanel2,CH1_P_TAB_NUM_STATIC_KTD,pExMem->ctrlPara[1].Ktd);
	SetCtrlVal(tabpanel2,CH1_P_TAB_NUM_STATIC_FT1,pExMem->ctrlPara[1].Ft1);
	SetCtrlVal(tabpanel2,CH1_P_TAB_NUM_STATIC_FT2,pExMem->ctrlPara[1].Ft2);
	SetCtrlVal(tabpanel2,CH1_P_TAB_NUM_STATIC_FTK,pExMem->ctrlPara[1].Ftk);
	SetCtrlVal(tabpanel2,CH1_P_TAB_NUM_STATIC_X1,pExMem->ctrlPara[1].X1);
	SetCtrlVal(tabpanel2,CH1_P_TAB_NUM_STATIC_X2,pExMem->ctrlPara[1].X2);
	SetCtrlVal(tabpanel2,CH1_P_TAB_NUM_STATIC_X3,pExMem->ctrlPara[1].X3);
	
	//ͨ��2�Ŀ��Ʋ���
	GetPanelHandleFromTabPage(tabpanel1, PARA_TAB_CTRL_PARA_TAB,1,&tabpanel2);
	SetCtrlVal(tabpanel2,CH2_P_TAB_NUM_STATIC_KTP,pExMem->ctrlPara[2].Ktp);
	SetCtrlVal(tabpanel2,CH2_P_TAB_NUM_STATIC_KTI,pExMem->ctrlPara[2].Kti);
	SetCtrlVal(tabpanel2,CH2_P_TAB_NUM_STATIC_KTD,pExMem->ctrlPara[2].Ktd);
	SetCtrlVal(tabpanel2,CH2_P_TAB_NUM_STATIC_FT1,pExMem->ctrlPara[2].Ft1);
	SetCtrlVal(tabpanel2,CH2_P_TAB_NUM_STATIC_FT2,pExMem->ctrlPara[2].Ft2);
	SetCtrlVal(tabpanel2,CH2_P_TAB_NUM_STATIC_FTK,pExMem->ctrlPara[2].Ftk);
	SetCtrlVal(tabpanel2,CH2_P_TAB_NUM_STATIC_X1,pExMem->ctrlPara[2].X1);
	SetCtrlVal(tabpanel2,CH2_P_TAB_NUM_STATIC_X2,pExMem->ctrlPara[2].X2);
	SetCtrlVal(tabpanel2,CH2_P_TAB_NUM_STATIC_X3,pExMem->ctrlPara[2].X3);
	
	//ͨ��3�Ŀ��Ʋ���	
	GetPanelHandleFromTabPage(tabpanel1, PARA_TAB_CTRL_PARA_TAB,2,&tabpanel2);
	SetCtrlVal(tabpanel2,CH3_P_TAB_NUM_STATIC_KTP,pExMem->ctrlPara[3].Ktp);
	SetCtrlVal(tabpanel2,CH3_P_TAB_NUM_STATIC_KTI,pExMem->ctrlPara[3].Kti);
	SetCtrlVal(tabpanel2,CH3_P_TAB_NUM_STATIC_KTD,pExMem->ctrlPara[3].Ktd);
	SetCtrlVal(tabpanel2,CH3_P_TAB_NUM_STATIC_FT1,pExMem->ctrlPara[3].Ft1);
	SetCtrlVal(tabpanel2,CH3_P_TAB_NUM_STATIC_FT2,pExMem->ctrlPara[3].Ft2);
	SetCtrlVal(tabpanel2,CH3_P_TAB_NUM_STATIC_FTK,pExMem->ctrlPara[3].Ftk);
	SetCtrlVal(tabpanel2,CH3_P_TAB_NUM_STATIC_X1,pExMem->ctrlPara[3].X1);
	SetCtrlVal(tabpanel2,CH3_P_TAB_NUM_STATIC_X2,pExMem->ctrlPara[3].X2);
	SetCtrlVal(tabpanel2,CH3_P_TAB_NUM_STATIC_X3,pExMem->ctrlPara[3].X3);
	
	//ͨ��4�Ŀ��Ʋ���
	GetPanelHandleFromTabPage(tabpanel1, PARA_TAB_CTRL_PARA_TAB,3,&tabpanel2);
	SetCtrlVal(tabpanel2,CH4_P_TAB_NUM_STATIC_KTP,pExMem->ctrlPara[4].Ktp);
	SetCtrlVal(tabpanel2,CH4_P_TAB_NUM_STATIC_KTI,pExMem->ctrlPara[4].Kti);
	SetCtrlVal(tabpanel2,CH4_P_TAB_NUM_STATIC_KTD,pExMem->ctrlPara[4].Ktd);
	SetCtrlVal(tabpanel2,CH4_P_TAB_NUM_STATIC_FT1,pExMem->ctrlPara[4].Ft1);
	SetCtrlVal(tabpanel2,CH4_P_TAB_NUM_STATIC_FT2,pExMem->ctrlPara[4].Ft2);
	SetCtrlVal(tabpanel2,CH4_P_TAB_NUM_STATIC_FTK,pExMem->ctrlPara[4].Ftk);
	SetCtrlVal(tabpanel2,CH4_P_TAB_NUM_STATIC_X1,pExMem->ctrlPara[4].X1);
	SetCtrlVal(tabpanel2,CH4_P_TAB_NUM_STATIC_X2,pExMem->ctrlPara[4].X2);
	SetCtrlVal(tabpanel2,CH4_P_TAB_NUM_STATIC_X3,pExMem->ctrlPara[4].X3);
	
}   

void Sctrlparadown(void)
{
	double dTemp;
	GetPanelHandleFromTabPage (StaticPanelHandle, STAPANEL_STA_TAB,1,&tabpanel1);
			
				//ͨ��1�Ŀ��Ʋ���
				GetPanelHandleFromTabPage(tabpanel1, PARA_TAB_CTRL_PARA_TAB,0,&tabpanel2);
				GetCtrlVal(tabpanel2,CH1_P_TAB_NUM_STATIC_KTP,&dTemp);
				pExMem->ctrlPara[1].Ktp = dTemp;
				GetCtrlVal(tabpanel2,CH1_P_TAB_NUM_STATIC_KTI,&dTemp);
				pExMem->ctrlPara[1].Kti = dTemp;
				GetCtrlVal(tabpanel2,CH1_P_TAB_NUM_STATIC_KTD,&dTemp);
				pExMem->ctrlPara[1].Ktd = dTemp;
				GetCtrlVal(tabpanel2,CH1_P_TAB_NUM_STATIC_FT1,&dTemp);
				pExMem->ctrlPara[1].Ft1 = dTemp;
				GetCtrlVal(tabpanel2,CH1_P_TAB_NUM_STATIC_FT2,&dTemp);
				pExMem->ctrlPara[1].Ft2 = dTemp;
				GetCtrlVal(tabpanel2,CH1_P_TAB_NUM_STATIC_FTK,&dTemp);
				pExMem->ctrlPara[1].Ftk = dTemp;
				GetCtrlVal(tabpanel2,CH1_P_TAB_NUM_STATIC_X1,&dTemp);
				pExMem->ctrlPara[1].X1 = dTemp;
				GetCtrlVal(tabpanel2,CH1_P_TAB_NUM_STATIC_X2,&dTemp);
				pExMem->ctrlPara[1].X2 = dTemp;
				GetCtrlVal(tabpanel2,CH1_P_TAB_NUM_STATIC_X3,&dTemp);
				pExMem->ctrlPara[1].X3 = dTemp;
			
				//ͨ��2�Ŀ��Ʋ���
				GetPanelHandleFromTabPage(tabpanel1, PARA_TAB_CTRL_PARA_TAB,1,&tabpanel2);
				GetCtrlVal(tabpanel2,CH2_P_TAB_NUM_STATIC_KTP,&dTemp);
				pExMem->ctrlPara[2].Ktp = dTemp;
				GetCtrlVal(tabpanel2,CH2_P_TAB_NUM_STATIC_KTI,&dTemp);
				pExMem->ctrlPara[2].Kti = dTemp;
				GetCtrlVal(tabpanel2,CH2_P_TAB_NUM_STATIC_KTD,&dTemp);
				pExMem->ctrlPara[2].Ktd = dTemp;
				GetCtrlVal(tabpanel2,CH2_P_TAB_NUM_STATIC_FT1,&dTemp);
				pExMem->ctrlPara[2].Ft1 = dTemp;
				GetCtrlVal(tabpanel2,CH2_P_TAB_NUM_STATIC_FT2,&dTemp);
				pExMem->ctrlPara[2].Ft2 = dTemp;
				GetCtrlVal(tabpanel2,CH2_P_TAB_NUM_STATIC_FTK,&dTemp);
				pExMem->ctrlPara[2].Ftk = dTemp;
				GetCtrlVal(tabpanel2,CH2_P_TAB_NUM_STATIC_X1,&dTemp);
				pExMem->ctrlPara[2].X1 = dTemp;
				GetCtrlVal(tabpanel2,CH2_P_TAB_NUM_STATIC_X2,&dTemp);
				pExMem->ctrlPara[2].X2 = dTemp;
				GetCtrlVal(tabpanel2,CH2_P_TAB_NUM_STATIC_X3,&dTemp);
				pExMem->ctrlPara[2].X3 = dTemp;
			
				//ͨ��3�Ŀ��Ʋ���
				GetPanelHandleFromTabPage(tabpanel1, PARA_TAB_CTRL_PARA_TAB,2,&tabpanel2);
				GetCtrlVal(tabpanel2,CH3_P_TAB_NUM_STATIC_KTP,&dTemp);
				pExMem->ctrlPara[3].Ktp = dTemp;
				GetCtrlVal(tabpanel2,CH3_P_TAB_NUM_STATIC_KTI,&dTemp);
				pExMem->ctrlPara[3].Kti = dTemp;
				GetCtrlVal(tabpanel2,CH3_P_TAB_NUM_STATIC_KTD,&dTemp);
				pExMem->ctrlPara[3].Ktd = dTemp;
				GetCtrlVal(tabpanel2,CH3_P_TAB_NUM_STATIC_FT1,&dTemp);
				pExMem->ctrlPara[3].Ft1 = dTemp;
				GetCtrlVal(tabpanel2,CH3_P_TAB_NUM_STATIC_FT2,&dTemp);
				pExMem->ctrlPara[3].Ft2 = dTemp;
				GetCtrlVal(tabpanel2,CH3_P_TAB_NUM_STATIC_FTK,&dTemp);
				pExMem->ctrlPara[3].Ftk = dTemp;
				GetCtrlVal(tabpanel2,CH3_P_TAB_NUM_STATIC_X1,&dTemp);
				pExMem->ctrlPara[3].X1 = dTemp;
				GetCtrlVal(tabpanel2,CH3_P_TAB_NUM_STATIC_X2,&dTemp);
				pExMem->ctrlPara[3].X2 = dTemp;
				GetCtrlVal(tabpanel2,CH3_P_TAB_NUM_STATIC_X3,&dTemp);
				pExMem->ctrlPara[3].X3 = dTemp;
		
				//ͨ��4�Ŀ��Ʋ���
				GetPanelHandleFromTabPage(tabpanel1, PARA_TAB_CTRL_PARA_TAB,3,&tabpanel2);
				GetCtrlVal(tabpanel2,CH4_P_TAB_NUM_STATIC_KTP,&dTemp);
				pExMem->ctrlPara[4].Ktp = dTemp;
				GetCtrlVal(tabpanel2,CH4_P_TAB_NUM_STATIC_KTI,&dTemp);
				pExMem->ctrlPara[4].Kti = dTemp;
				GetCtrlVal(tabpanel2,CH4_P_TAB_NUM_STATIC_KTD,&dTemp);
				pExMem->ctrlPara[4].Ktd = dTemp;
				GetCtrlVal(tabpanel2,CH4_P_TAB_NUM_STATIC_FT1,&dTemp);
				pExMem->ctrlPara[4].Ft1 = dTemp;
				GetCtrlVal(tabpanel2,CH4_P_TAB_NUM_STATIC_FT2,&dTemp);
				pExMem->ctrlPara[4].Ft2 = dTemp;
				GetCtrlVal(tabpanel2,CH4_P_TAB_NUM_STATIC_FTK,&dTemp);
				pExMem->ctrlPara[4].Ftk = dTemp;
				GetCtrlVal(tabpanel2,CH4_P_TAB_NUM_STATIC_X1,&dTemp);
				pExMem->ctrlPara[4].X1 = dTemp;
				GetCtrlVal(tabpanel2,CH4_P_TAB_NUM_STATIC_X2,&dTemp);
				pExMem->ctrlPara[4].X2 = dTemp;
				GetCtrlVal(tabpanel2,CH4_P_TAB_NUM_STATIC_X3,&dTemp);
				pExMem->ctrlPara[4].X3 = dTemp;
		}
//��ʼ����̬���ز��Բ���
void InitStaticTestPara(void)
{
	double dTemp;
	int tempInt;
	
	GetPanelHandleFromTabPage (StaticPanelHandle, STAPANEL_STA_TAB,0,&tabpanel1);
	
	GetCtrlVal(StaticPanelHandle,STAPANEL_CH_TEST_TIME,&dTemp);    
	pExMem->testPara[1].dTestTime = dTemp;
	pExMem->testPara[2].dTestTime = dTemp;    
	pExMem->testPara[3].dTestTime = dTemp;    
	pExMem->testPara[4].dTestTime = dTemp;    
			
	//ͨ��1�Ĳ��Բ���
	GetPanelHandleFromTabPage(tabpanel1, SET_TAB_CH_TAB,0,&tabpanel2);
	
	GetCtrlVal(tabpanel2,CH1_TAB_CH_AMP,&dTemp);
	pExMem->testPara[1].dAmp = dTemp;
	GetCtrlVal(tabpanel2,CH1_TAB_CH_OFFSET,&dTemp);
	pExMem->testPara[1].dOffset = dTemp;
	GetCtrlVal(tabpanel2,CH1_TAB_CH_FREQ,&dTemp);    
	pExMem->testPara[1].dFreq = dTemp;  
	//GetCtrlVal(tabpanel2,CH1_TAB_CH_START_FREQ,&dTemp);    
	//pExMem->testPara[1].dStartFreq = dTemp; 
	GetCtrlVal(tabpanel2,CH1_TAB_CH_STOP_FREQ,&dTemp);    
	pExMem->testPara[1].dStopFreq = dTemp;

	GetCtrlVal(tabpanel2,CH1_TAB_CH_ON_OFF,&tempInt); 
	if(tempInt == 1)
	{
		pExMem->testPara[1].iStatus = 2;  		  //1��ʾλ��ͨ����2��ʾ����ͨ��
	}
	else
	{
		pExMem->testPara[1].iStatus = 0;  		  //1��ʾλ��ͨ����2��ʾ����ͨ��
	}
	GetCtrlVal(tabpanel2,CH1_TAB_CH_WAVE,&tempInt); 
	pExMem->testPara[1].iWave = tempInt; 
	GetCtrlVal(tabpanel2,CH1_TAB_CH_TEST_TYPE,&tempInt); 
	pExMem->testPara[1].iTestType = tempInt;

	
	//ͨ��2�Ĳ��Բ��� 
	GetPanelHandleFromTabPage(tabpanel1, SET_TAB_CH_TAB,1,&tabpanel2);
	GetCtrlVal(tabpanel2,CH2_TAB_CH_AMP,&dTemp);
	pExMem->testPara[2].dAmp = dTemp;
	GetCtrlVal(tabpanel2,CH2_TAB_CH_OFFSET,&dTemp);
	pExMem->testPara[2].dOffset = dTemp;
	GetCtrlVal(tabpanel2,CH2_TAB_CH_FREQ,&dTemp);    
	pExMem->testPara[2].dFreq = dTemp;  
	//GetCtrlVal(tabpanel2,CH2_TAB_CH_START_FREQ,&dTemp);    
	//pExMem->testPara[2].dStartFreq = dTemp; 
	GetCtrlVal(tabpanel2,CH2_TAB_CH_STOP_FREQ,&dTemp);    
	pExMem->testPara[2].dStopFreq = dTemp;

	GetCtrlVal(tabpanel2,CH2_TAB_CH_ON_OFF,&tempInt);
	//pExMem->testPara[2].iEnable = tempInt;
	if(tempInt == 1)
	{
		pExMem->testPara[2].iStatus = 2;  		  //1��ʾλ��ͨ����2��ʾ����ͨ��
	}
	else
	{
		pExMem->testPara[2].iStatus = 0;  		  //1��ʾλ��ͨ����2��ʾ����ͨ��
	}
	GetCtrlVal(tabpanel2,CH2_TAB_CH_WAVE,&tempInt); 
	pExMem->testPara[2].iWave = tempInt; 
	GetCtrlVal(tabpanel2,CH2_TAB_CH_TEST_TYPE,&tempInt); 
	pExMem->testPara[2].iTestType = tempInt; 

	
	//ͨ��3�Ĳ��Բ��� 
	GetPanelHandleFromTabPage(tabpanel1, SET_TAB_CH_TAB,2,&tabpanel2);
	GetCtrlVal(tabpanel2,CH3_TAB_CH_AMP,&dTemp);
	pExMem->testPara[3].dAmp = dTemp;
	GetCtrlVal(tabpanel2,CH3_TAB_CH_OFFSET,&dTemp);
	pExMem->testPara[3].dOffset = dTemp;
	GetCtrlVal(tabpanel2,CH3_TAB_CH_FREQ,&dTemp);    
	pExMem->testPara[3].dFreq = dTemp;  
	//GetCtrlVal(tabpanel2,CH3_TAB_CH_START_FREQ,&dTemp);    
	//pExMem->testPara[3].dStartFreq = dTemp; 
	GetCtrlVal(tabpanel2,CH3_TAB_CH_STOP_FREQ,&dTemp);    
	pExMem->testPara[3].dStopFreq = dTemp;

	GetCtrlVal(tabpanel2,CH3_TAB_CH_ON_OFF,&tempInt); 
	//pExMem->testPara[3].iEnable = tempInt;
	if(tempInt == 1)
	{
		pExMem->testPara[3].iStatus = 2;  		  //1��ʾλ��ͨ����2��ʾ����ͨ��
	}
	else
	{
		pExMem->testPara[3].iStatus = 0;  		  //1��ʾλ��ͨ����2��ʾ����ͨ��
	}
	GetCtrlVal(tabpanel2,CH3_TAB_CH_WAVE,&tempInt); 
	pExMem->testPara[3].iWave = tempInt; 
	GetCtrlVal(tabpanel2,CH3_TAB_CH_TEST_TYPE,&tempInt); 
	pExMem->testPara[3].iTestType = tempInt; 

	
	//ͨ��4�Ĳ��Բ��� 
	GetPanelHandleFromTabPage(tabpanel1, SET_TAB_CH_TAB,3,&tabpanel2);
	GetCtrlVal(tabpanel2,CH4_TAB_CH_AMP,&dTemp);
	pExMem->testPara[4].dAmp = dTemp;
	GetCtrlVal(tabpanel2,CH4_TAB_CH_OFFSET,&dTemp);
	pExMem->testPara[4].dOffset = dTemp;
	GetCtrlVal(tabpanel2,CH4_TAB_CH_FREQ,&dTemp);    
	pExMem->testPara[4].dFreq = dTemp;  
	//GetCtrlVal(tabpanel2,CH4_TAB_CH_START_FREQ,&dTemp);    
	//pExMem->testPara[4].dStartFreq = dTemp; 
	GetCtrlVal(tabpanel2,CH4_TAB_CH_STOP_FREQ,&dTemp);    
	pExMem->testPara[4].dStopFreq = dTemp;

	GetCtrlVal(tabpanel2,CH4_TAB_CH_ON_OFF,&tempInt);
	//pExMem->testPara[4].iEnable = tempInt;
	if(tempInt == 1)
	{
		pExMem->testPara[4].iStatus = 2;  		  //1��ʾλ��ͨ����2��ʾ����ͨ��
	}
	else
	{
		pExMem->testPara[4].iStatus = 0;  		  //1��ʾλ��ͨ����2��ʾ����ͨ��
	}
	
	GetCtrlVal(tabpanel2,CH4_TAB_CH_WAVE,&tempInt); 
	pExMem->testPara[4].iWave = tempInt; 
	
	GetCtrlVal(tabpanel2,CH4_TAB_CH_TEST_TYPE,&tempInt); 
	pExMem->testPara[4].iTestType = tempInt; 
	
		
}

void StaticSetScreenCycNum(void)
{
	double dfreq;	   //Ԥ��Ƶ��
	double dring_val;  //����ֶ�ֵ
	long N;   //ÿ����ʾ����
	int i,ringindex;
	
	for(i=1;i<5;i++)
	{
		if(pExMem->testPara[i].iStatus && (chart3_ch == i))
		{
			N = (long)(2000 / (pExMem->testPara[i].dFreq) * CYCLE_NUM_PER_SCREEN);  //��Ļ��ʾ�������㹫ʽ
			if(N >= 3 && N <= 10000)
			{
				SetCtrlAttribute(StaticPanelHandle,STAPANEL_STRIPCHART_3,ATTR_POINTS_PER_SCREEN,N);
				SetCtrlAttribute(StaticPanelHandle,STAPANEL_STRIPCHART_4,ATTR_POINTS_PER_SCREEN,N);
				SetCtrlAttribute(StaticPanelHandle,STAPANEL_STRIPCHART_1,ATTR_POINTS_PER_SCREEN,N);
				SetCtrlAttribute(StaticPanelHandle,STAPANEL_STRIPCHART_2,ATTR_POINTS_PER_SCREEN,N);
			}
			
			for(ringindex = 0;ringindex < 11;ringindex++)  //����ֵ������Χ������y����
			{
				GetValueFromIndex(StaticPanelHandle,STAPANEL_CHART3_YRANGE_RING,ringindex,&dring_val);  //�õ�����ķֶ�ֵ
				if(pExMem->testPara[i].dAmp < dring_val)
				{
					SetCtrlIndex(StaticPanelHandle,STAPANEL_CHART3_YRANGE_RING,ringindex);
					SetCtrlIndex(StaticPanelHandle,STAPANEL_CHART4_YRANGE_RING,ringindex);
					SetAxisRange(StaticPanelHandle,STAPANEL_STRIPCHART_3,VAL_NO_CHANGE,0.0,1.0,VAL_MANUAL,-dring_val,dring_val);
					SetAxisRange(StaticPanelHandle,STAPANEL_STRIPCHART_4,VAL_NO_CHANGE,0.0,1.0,VAL_MANUAL,-dring_val,dring_val);
					break;
				}
			}
		}
	}
}

void InitStaticDisIndex(void)
{
	char tempLabel[64];
	//��ʼ����ʾ����
	//tempLabel = " ";
	
	GetCtrlVal(StaticPanelHandle,STAPANEL_CHART1_CH_RING,&chart1_ch);
	/*
	GetCtrlVal(StaticPanelHandle,STAPANEL_CHART1_CONTENT_RING,&chart1_content);
	strcpy(tempLabel,"");    
	strcat(tempLabel,contentLabel[chart1_content]);
	SetCtrlAttribute(StaticPanelHandle,STAPANEL_STRIPCHART_1,ATTR_LABEL_TEXT,tempLabel);
	SetCtrlAttribute(StaticPanelHandle,STAPANEL_STRIPCHART_1,ATTR_LABEL_JUSTIFY,VAL_LEFT_JUSTIFIED); 
	*/
	
	GetCtrlVal(StaticPanelHandle,STAPANEL_CHART2_CH_RING,&chart2_ch);
	GetCtrlVal(StaticPanelHandle,STAPANEL_CHART2_CONTENT_RING,&chart2_content);
	strcpy(tempLabel,""); 
	strcat(tempLabel,contentLabel[chart2_content]);
	SetCtrlAttribute(StaticPanelHandle,STAPANEL_STRIPCHART_2,ATTR_LABEL_TEXT,tempLabel);
	//SetCtrlAttribute(StaticPanelHandle,STAPANEL_STRIPCHART_2,ATTR_LABEL_JUSTIFY,VAL_LEFT_JUSTIFIED); 
	
	
	GetCtrlVal(StaticPanelHandle,STAPANEL_CHART3_CH_RING,&chart3_ch);
	/*
	GetCtrlVal(StaticPanelHandle,STAPANEL_CHART3_CONTENT_RING,&chart3_content);
	strcpy(tempLabel,""); 
	strcat(tempLabel,contentLabel[chart3_content]);
	SetCtrlAttribute(StaticPanelHandle,STAPANEL_STRIPCHART_3,ATTR_LABEL_TEXT,tempLabel);
	SetCtrlAttribute(StaticPanelHandle,STAPANEL_STRIPCHART_3,ATTR_LABEL_JUSTIFY,VAL_LEFT_JUSTIFIED); 
	*/
	
	
	GetCtrlVal(StaticPanelHandle,STAPANEL_CHART4_CH_RING,&chart4_ch);
	GetCtrlVal(StaticPanelHandle,STAPANEL_CHART4_CONTENT_RING,&chart4_content);
	strcpy(tempLabel,""); 
	strcat(tempLabel,contentLabel[chart4_content==5?chart4_content+1:chart4_content]);
	SetCtrlAttribute(StaticPanelHandle,STAPANEL_STRIPCHART_4,ATTR_LABEL_TEXT,tempLabel);
	//SetCtrlAttribute(StaticPanelHandle,STAPANEL_STRIPCHART_4,ATTR_LABEL_JUSTIFY,VAL_LEFT_JUSTIFIED); 
}

//��ʼ�����Ʋ���
void InitStaticCtrlPara(void)
{
	double dTemp;
	//��ȡini�ļ����
	inihandle = Ini_New(0);
	//��ȡini�ļ�
	//GetWindowsDirs (windowsDirectory, systemDirectory);
	//strcpy( pathname ,windowsDirectory);
	
	GetDir (currentDir);
	strcpy( pathname ,currentDir);
	strcat(pathname,"\\StaticPara.ini");
	
	inierr = Ini_ReadFromFile(inihandle, pathname); 
	if( inierr < 0)
	{
		sprintf(msgBuff,"�󿪲����ļ�����err = %d",inierr);
		MessagePopup ("Err",msgBuff);  
		//���ļ�ʧ��
	}
	else
	{
		//ͨ��1---kap\kai\kadλ��PID����
		if(Ini_GetDouble (inihandle, "CtrlPara","CH1_Ktp", &dTemp)>0)
		{
			pExMem->ctrlPara[1].Ktp = dTemp;
		}
		else 						 
		{
			 pExMem->ctrlPara[1].Ktp = 0.0;
		}
	
		if(Ini_GetDouble (inihandle, "CtrlPara","CH1_Kti", &dTemp)>0)
		{
			pExMem->ctrlPara[1].Kti = dTemp;
		}
		else 
		{
			 pExMem->ctrlPara[1].Kti = 0.0;
		}
	
		if(Ini_GetDouble (inihandle, "CtrlPara","CH1_Ktd", &dTemp)>0)
		{
			pExMem->ctrlPara[1].Ktd = dTemp;
		}
		else 
		{
			 pExMem->ctrlPara[1].Ktd = 0.0;
		}
		//����ģ������-----fa1\fa2\fak
		if(Ini_GetDouble (inihandle, "CtrlPara","CH1_Ft1", &dTemp)>0)
		{
			pExMem->ctrlPara[1].Ft1 = dTemp;
		}
		else 
		{
			 pExMem->ctrlPara[1].Ft1 = 0.0;
		}
	
		if(Ini_GetDouble (inihandle, "CtrlPara","CH1_Ft2", &dTemp)>0)
		{
			pExMem->ctrlPara[1].Ft2 = dTemp;
		}
		else 
		{
			 pExMem->ctrlPara[1].Ft2 = 0.0;
		}
	
		if(Ini_GetDouble (inihandle, "CtrlPara","CH1_Ftk", &dTemp)>0)
		{
			pExMem->ctrlPara[1].Ftk = dTemp;
		}
		else 
		{
			 pExMem->ctrlPara[1].Ftk = 0.0;
		}
		//�������ò���-----X1\X2\X3
		if(Ini_GetDouble (inihandle, "CtrlPara","CH1_X1", &dTemp)>0)
		{
			pExMem->ctrlPara[1].X1 = dTemp;
		}
		else 
		{
			 pExMem->ctrlPara[1].X1 = 0.0;
		}
	
		if(Ini_GetDouble (inihandle, "CtrlPara","CH1_X2", &dTemp)>0)
		{
			pExMem->ctrlPara[1].X2 = dTemp;
		}
		else 
		{
			 pExMem->ctrlPara[1].X2 = 0.0;
		}
	
		if(Ini_GetDouble (inihandle, "CtrlPara","CH1_X3", &dTemp)>0)
		{
			pExMem->ctrlPara[1].X3 = dTemp;
		}
		else 
		{
			 pExMem->ctrlPara[1].X3 = 0.0;
		}
		
		//ͨ��2
		if(Ini_GetDouble (inihandle, "CtrlPara","CH2_Ktp", &dTemp)>0)
		{
			pExMem->ctrlPara[2].Ktp = dTemp;
		}
		else 
		{
			 pExMem->ctrlPara[2].Ktp = 0.0;
		}
	
		if(Ini_GetDouble (inihandle, "CtrlPara","CH2_Kti", &dTemp)>0)
		{
			pExMem->ctrlPara[2].Kti = dTemp;
		}
		else 
		{
			 pExMem->ctrlPara[2].Kti = 0;
		}
	
		if(Ini_GetDouble (inihandle, "CtrlPara","CH2_Ktd", &dTemp)>0)
		{
			pExMem->ctrlPara[2].Ktd = dTemp;
		}
		else 
		{
			 pExMem->ctrlPara[2].Ktd = 0.0;
		}
		//����ģ������
		if(Ini_GetDouble (inihandle, "CtrlPara","CH2_Ft1", &dTemp)>0)
		{
			pExMem->ctrlPara[2].Ft1 = dTemp;
		}
		else 
		{
			 pExMem->ctrlPara[2].Ft1 = 0.0;
		}
	
		if(Ini_GetDouble (inihandle, "CtrlPara","CH2_Ft2", &dTemp)>0)
		{
			pExMem->ctrlPara[2].Ft2 = dTemp;
		}
		else 
		{
			 pExMem->ctrlPara[2].Ft2 = 0.0;
		}
	
		if(Ini_GetDouble (inihandle, "CtrlPara","CH2_Ftk", &dTemp)>0)
		{
			pExMem->ctrlPara[2].Ftk = dTemp;
		}
		else 
		{
			 pExMem->ctrlPara[2].Ftk = 0.0;
		}
		//�������ò���-----X1\X2\X3
		if(Ini_GetDouble (inihandle, "CtrlPara","CH2_X1", &dTemp)>0)
		{
			pExMem->ctrlPara[2].X1 = dTemp;
		}
		else 
		{
			 pExMem->ctrlPara[2].X1 = 0.0;
		}
	
		if(Ini_GetDouble (inihandle, "CtrlPara","CH2_X2", &dTemp)>0)
		{
			pExMem->ctrlPara[2].X2 = dTemp;
		}
		else 
		{
			 pExMem->ctrlPara[2].X2 = 0.0;
		}
	
		if(Ini_GetDouble (inihandle, "CtrlPara","CH2_X3", &dTemp)>0)
		{
			pExMem->ctrlPara[2].X3 = dTemp;
		}
		else 
		{
			 pExMem->ctrlPara[2].X3 = 0.0;
		}
		
		//ͨ��3
		if(Ini_GetDouble (inihandle, "CtrlPara","CH3_Ktp", &dTemp)>0)
		{
			pExMem->ctrlPara[3].Ktp = dTemp;
		}
		else 
		{
			 pExMem->ctrlPara[3].Ktp = 0.0;
		}
	
		if(Ini_GetDouble (inihandle, "CtrlPara","CH3_Kti", &dTemp)>0)
		{
			pExMem->ctrlPara[3].Kti = dTemp;
		}
		else 
		{
			 pExMem->ctrlPara[3].Kti = 0;
		}
	
		if(Ini_GetDouble (inihandle, "CtrlPara","CH3_Ktd", &dTemp)>0)
		{
			pExMem->ctrlPara[3].Ktd = dTemp;
		}
		else 
		{
			 pExMem->ctrlPara[3].Ktd = 0.0;
		}
		//����ģ������
		if(Ini_GetDouble (inihandle, "CtrlPara","CH3_Ft1", &dTemp)>0)
		{
			pExMem->ctrlPara[3].Ft1 = dTemp;
		}
		else 
		{
			 pExMem->ctrlPara[3].Ft1 = 0.0;
		}
	
		if(Ini_GetDouble (inihandle, "CtrlPara","CH3_Ft2", &dTemp)>0)
		{
			pExMem->ctrlPara[3].Ft2 = dTemp;
		}
		else 
		{
			 pExMem->ctrlPara[3].Ft2 = 0.0;
		}
	
		if(Ini_GetDouble (inihandle, "CtrlPara","CH3_Ftk", &dTemp)>0)
		{
			pExMem->ctrlPara[3].Ftk = dTemp;
		}
		else 
		{
			 pExMem->ctrlPara[3].Ftk = 0.0;
		}
		//�������ò���-----X1\X2\X3
		if(Ini_GetDouble (inihandle, "CtrlPara","CH3_X1", &dTemp)>0)
		{
			pExMem->ctrlPara[3].X1 = dTemp;
		}
		else 
		{
			 pExMem->ctrlPara[3].X1 = 0.0;
		}
	
		if(Ini_GetDouble (inihandle, "CtrlPara","CH3_X2", &dTemp)>0)
		{
			pExMem->ctrlPara[3].X2 = dTemp;
		}
		else 
		{
			 pExMem->ctrlPara[3].X2 = 0.0;
		}
	
		if(Ini_GetDouble (inihandle, "CtrlPara","CH3_X3", &dTemp)>0)
		{
			pExMem->ctrlPara[3].X3 = dTemp;
		}
		else 
		{
			 pExMem->ctrlPara[3].X3 = 0.0;
		}
		
		//ͨ��4
		if(Ini_GetDouble (inihandle, "CtrlPara","CH4_Ktp", &dTemp)>0)
		{
			pExMem->ctrlPara[4].Ktp = dTemp;
		}
		else 
		{
			 pExMem->ctrlPara[4].Ktp = 0.0;
		}
	
		if(Ini_GetDouble (inihandle, "CtrlPara","CH4_Kti", &dTemp)>0)
		{
			pExMem->ctrlPara[4].Kti = dTemp;
		}
		else 
		{
			 pExMem->ctrlPara[4].Kti = 0.0;
		}
	
		if(Ini_GetDouble (inihandle, "CtrlPara","CH4_Ktd", &dTemp)>0)
		{
			pExMem->ctrlPara[4].Ktd = dTemp;
		}
		else 
		{
			 pExMem->ctrlPara[4].Ktd = 0.0;
		}
		//����ģ������
		if(Ini_GetDouble (inihandle, "CtrlPara","CH4_Ft1", &dTemp)>0)
		{
			pExMem->ctrlPara[4].Ft1 = dTemp;
		}
		else 
		{
			 pExMem->ctrlPara[4].Ft1 = 0.0;
		}
	
		if(Ini_GetDouble (inihandle, "CtrlPara","CH4_Ft2", &dTemp)>0)
		{
			pExMem->ctrlPara[4].Ft2 = dTemp;
		}
		else 
		{
			 pExMem->ctrlPara[4].Ft2 = 0.0;
		}
	
		if(Ini_GetDouble (inihandle, "CtrlPara","CH4_Ftk", &dTemp)>0)
		{
			pExMem->ctrlPara[4].Ftk = dTemp;
		}
		else 
		{
			 pExMem->ctrlPara[4].Ftk = 0.0;
		}
		//�������ò���-----X1\X2\X3
		if(Ini_GetDouble (inihandle, "CtrlPara","CH4_X1", &dTemp)>0)
		{
			pExMem->ctrlPara[4].X1 = dTemp;
		}
		else 
		{
			 pExMem->ctrlPara[4].X1 = 0.0;
		}
	
		if(Ini_GetDouble (inihandle, "CtrlPara","CH4_X2", &dTemp)>0)
		{
			pExMem->ctrlPara[4].X2 = dTemp;
		}
		else 
		{
			 pExMem->ctrlPara[4].X2 = 0.0;
		}
	
		if(Ini_GetDouble (inihandle, "CtrlPara","CH4_X3", &dTemp)>0)
		{
			pExMem->ctrlPara[4].X3 = dTemp;
		}
		else 
		{
			 pExMem->ctrlPara[4].X3 = 0.0;
		}
	}
	
	InitStaticCtrlSet();
}

//ʹ�ܿ��Ʋ�������
void LightStaticParaSet(void)
{
	GetPanelHandleFromTabPage (StaticPanelHandle, STAPANEL_STA_TAB, 1, &tabpanel1);
	
	//ͨ��1�Ŀ��Ʋ���
	GetPanelHandleFromTabPage (tabpanel1, PARA_TAB_CTRL_PARA_TAB, 0, &tabpanel2);
	SetCtrlAttribute(tabpanel2,CH1_P_TAB_NUM_STATIC_KTP,ATTR_DIMMED,0);
	SetCtrlAttribute(tabpanel2,CH1_P_TAB_NUM_STATIC_KTI,ATTR_DIMMED,0);
	SetCtrlAttribute(tabpanel2,CH1_P_TAB_NUM_STATIC_KTD,ATTR_DIMMED,0);
	SetCtrlAttribute(tabpanel2,CH1_P_TAB_NUM_STATIC_FT1,ATTR_DIMMED,0);
	SetCtrlAttribute(tabpanel2,CH1_P_TAB_NUM_STATIC_FT2,ATTR_DIMMED,0);
	SetCtrlAttribute(tabpanel2,CH1_P_TAB_NUM_STATIC_FTK,ATTR_DIMMED,0);
	SetCtrlAttribute(tabpanel2,CH1_P_TAB_NUM_STATIC_X1,ATTR_DIMMED,0);
	SetCtrlAttribute(tabpanel2,CH1_P_TAB_NUM_STATIC_X2,ATTR_DIMMED,0);
	SetCtrlAttribute(tabpanel2,CH1_P_TAB_NUM_STATIC_X3,ATTR_DIMMED,0);
	
	
	GetPanelHandleFromTabPage(tabpanel1, PARA_TAB_CTRL_PARA_TAB,1,&tabpanel2);
	SetCtrlAttribute(tabpanel2,CH2_P_TAB_NUM_STATIC_KTP,ATTR_DIMMED,0);
	SetCtrlAttribute(tabpanel2,CH2_P_TAB_NUM_STATIC_KTI,ATTR_DIMMED,0);
	SetCtrlAttribute(tabpanel2,CH2_P_TAB_NUM_STATIC_KTD,ATTR_DIMMED,0);
	SetCtrlAttribute(tabpanel2,CH2_P_TAB_NUM_STATIC_FT1,ATTR_DIMMED,0);
	SetCtrlAttribute(tabpanel2,CH2_P_TAB_NUM_STATIC_FT2,ATTR_DIMMED,0);
	SetCtrlAttribute(tabpanel2,CH2_P_TAB_NUM_STATIC_FTK,ATTR_DIMMED,0);
	SetCtrlAttribute(tabpanel2,CH2_P_TAB_NUM_STATIC_X1,ATTR_DIMMED,0);
	SetCtrlAttribute(tabpanel2,CH2_P_TAB_NUM_STATIC_X2,ATTR_DIMMED,0);
	SetCtrlAttribute(tabpanel2,CH2_P_TAB_NUM_STATIC_X3,ATTR_DIMMED,0);

	
	GetPanelHandleFromTabPage(tabpanel1, PARA_TAB_CTRL_PARA_TAB,2,&tabpanel2);
	SetCtrlAttribute(tabpanel2,CH3_P_TAB_NUM_STATIC_KTP,ATTR_DIMMED,0);
	SetCtrlAttribute(tabpanel2,CH3_P_TAB_NUM_STATIC_KTI,ATTR_DIMMED,0);
	SetCtrlAttribute(tabpanel2,CH3_P_TAB_NUM_STATIC_KTD,ATTR_DIMMED,0);
	SetCtrlAttribute(tabpanel2,CH3_P_TAB_NUM_STATIC_FT1,ATTR_DIMMED,0);
	SetCtrlAttribute(tabpanel2,CH3_P_TAB_NUM_STATIC_FT2,ATTR_DIMMED,0);
	SetCtrlAttribute(tabpanel2,CH3_P_TAB_NUM_STATIC_FTK,ATTR_DIMMED,0);
	SetCtrlAttribute(tabpanel2,CH3_P_TAB_NUM_STATIC_X1,ATTR_DIMMED,0);
	SetCtrlAttribute(tabpanel2,CH3_P_TAB_NUM_STATIC_X2,ATTR_DIMMED,0);
	SetCtrlAttribute(tabpanel2,CH3_P_TAB_NUM_STATIC_X3,ATTR_DIMMED,0);
	
	
	GetPanelHandleFromTabPage(tabpanel1, PARA_TAB_CTRL_PARA_TAB,3,&tabpanel2);
	SetCtrlAttribute(tabpanel2,CH4_P_TAB_NUM_STATIC_KTP,ATTR_DIMMED,0);
	SetCtrlAttribute(tabpanel2,CH4_P_TAB_NUM_STATIC_KTI,ATTR_DIMMED,0);
	SetCtrlAttribute(tabpanel2,CH4_P_TAB_NUM_STATIC_KTD,ATTR_DIMMED,0);
	SetCtrlAttribute(tabpanel2,CH4_P_TAB_NUM_STATIC_FT1,ATTR_DIMMED,0);
	SetCtrlAttribute(tabpanel2,CH4_P_TAB_NUM_STATIC_FT2,ATTR_DIMMED,0);
	SetCtrlAttribute(tabpanel2,CH4_P_TAB_NUM_STATIC_FTK,ATTR_DIMMED,0);
	SetCtrlAttribute(tabpanel2,CH4_P_TAB_NUM_STATIC_X1,ATTR_DIMMED,0);
	SetCtrlAttribute(tabpanel2,CH4_P_TAB_NUM_STATIC_X2,ATTR_DIMMED,0);
	SetCtrlAttribute(tabpanel2,CH4_P_TAB_NUM_STATIC_X3,ATTR_DIMMED,0);

}

////////////////////�����ǻص�����////////////////////////
//////**********���ص�����************////////

int CVICALLBACK StaticPanelCallback (int panel, int event, void *callbackData,
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
  
int CVICALLBACK StaticRecordCallback (int panel, int control, int event,
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

 ///////**********�������ݻص�����************////////  
int CVICALLBACK StaticSaveCallback (int panel, int control, int event,
		void *callbackData, int eventData1, int eventData2)
{
	switch (event)
	{
		case EVENT_COMMIT:
			strcpy(saveDir,"D:\\��������" );
		//	err = MakeDir (saveDir);
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

int CVICALLBACK StaticStartCallback (int panel, int control, int event,
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
				
				StaticSetScreenCycNum();         //������Ļ��ʾ����
				
				//��ָ����Ϣ���͸�ʵʱϵͳ����Ϣͷ������ָ���Լ���������ָ����ĳ���
				sendMessageHead.iCommand = SETPARA_PROCESS;
				sendMessageHead.iMessageLen = COMMANDSIZE;
				ClientTCPWrite(gTCPConnection, &sendMessageHead, MESSAGEHEADSIZE, 1000);         //��������ͷ
				Sctrlparadown();
				ClientTCPWrite(gTCPConnection, pExMem, COMMANDSIZE, 5000);
				
				sendMessageHead.iCommand = START_PROCESS;
				sendMessageHead.iMessageLen = COMMANDSIZE;
				ClientTCPWrite(gTCPConnection, &sendMessageHead, MESSAGEHEADSIZE, 1000);         //��������ͷ
				InitStaticTestPara();			 //��ʼ�����Բ���   
				ClientTCPWrite(gTCPConnection, pExMem, COMMANDSIZE, 5000);		    //��������    
				
			}
			break;
	}
	return 0;
}

int CVICALLBACK StaticResetCallback (int panel, int control, int event,
		void *callbackData, int eventData1, int eventData2)
{
	int SendBuffLen;  
	
	switch (event)
	{
		case EVENT_COMMIT:
			
			//��ָ����Ϣ���͸�ʵʱϵͳ����Ϣͷ
			sendMessageHead.iCommand = RESET_PROCESS;
			sendMessageHead.iMessageLen = 0;
			ClientTCPWrite(gTCPConnection, &sendMessageHead, MESSAGEHEADSIZE, 1000);         //��������ͷ
			 
			break;
	}
	return 0;
}

int CVICALLBACK StaticExitCallback (int panel, int control, int event,
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
				
				QuitUserInterface(0);  
			}
			break;
	}
	return 0;
}

int CVICALLBACK StaDisplayChangeCallback (int panel, int control, int event,
		void *callbackData, int eventData1, int eventData2)
{
	switch (event)
	{
		case EVENT_COMMIT:
			InitStaticDisIndex();            
			break;
	}
	return 0;
}

int CVICALLBACK TStaDisplayChangeCallback (int panel, int control, int event,
		void *callbackData, int eventData1, int eventData2)
{
	long N;
	int iTemp,ringindex;
	double dring_val;
	
	switch (event)
	{
		case EVENT_COMMIT:
			GetCtrlVal(panel,control,&iTemp);								//�����ĸ�strips��ʾͨ����ͬ
			SetCtrlVal(StaticPanelHandle,STAPANEL_CHART1_CH_RING,iTemp);
			SetCtrlVal(StaticPanelHandle,STAPANEL_CHART2_CH_RING,iTemp);
			SetCtrlVal(StaticPanelHandle,STAPANEL_CHART3_CH_RING,iTemp);
			SetCtrlVal(StaticPanelHandle,STAPANEL_CHART4_CH_RING,iTemp);
			
			InitStaticDisIndex();
			
			if(pExMem->testPara[chart3_ch].iStatus) //�����ѡͨ����״̬Ϊ��
			{
				N = (long)(2000 / (pExMem->testPara[chart3_ch].dFreq) * CYCLE_NUM_PER_SCREEN);  //��Ļ��ʾ�������㹫ʽ
				if(N >= 3 && N <= 10000)
				{
					SetCtrlAttribute(StaticPanelHandle,STAPANEL_STRIPCHART_1,ATTR_POINTS_PER_SCREEN,N);
					SetCtrlAttribute(StaticPanelHandle,STAPANEL_STRIPCHART_2,ATTR_POINTS_PER_SCREEN,N);
					SetCtrlAttribute(StaticPanelHandle,STAPANEL_STRIPCHART_3,ATTR_POINTS_PER_SCREEN,N);
					SetCtrlAttribute(StaticPanelHandle,STAPANEL_STRIPCHART_4,ATTR_POINTS_PER_SCREEN,N);
				}
				
				for(ringindex = 0;ringindex < 11;ringindex++)  //����ֵ������Χ������y����
				{
					GetValueFromIndex(StaticPanelHandle,STAPANEL_CHART3_YRANGE_RING,ringindex,&dring_val);
					if(pExMem->testPara[chart1_ch].dAmp < dring_val)
					{
						SetCtrlIndex(StaticPanelHandle,STAPANEL_CHART3_YRANGE_RING,ringindex);
						SetCtrlIndex(StaticPanelHandle,STAPANEL_CHART4_YRANGE_RING,ringindex);
						SetAxisRange(StaticPanelHandle,STAPANEL_STRIPCHART_3,VAL_NO_CHANGE,0.0,1.0,VAL_MANUAL,-dring_val,dring_val);
						SetAxisRange(StaticPanelHandle,STAPANEL_STRIPCHART_4,VAL_NO_CHANGE,0.0,1.0,VAL_MANUAL,-dring_val,dring_val);
						break;
					}
				}
			}
			
			break;
	}
	return 0;
}

///////**********Y���귶Χ�ص�����************//////// 
int CVICALLBACK StaYRangeChangeCallback (int panel, int control, int event,
		void *callbackData, int eventData1, int eventData2)
{
	double dTemp;
	switch (event)
	{
		case EVENT_COMMIT:
			
			switch(control)
			{
				case STAPANEL_CHART1_YRANGE_RING:
					GetCtrlVal(panel,control,&dTemp);
					SetAxisRange(panel,STAPANEL_STRIPCHART_1,VAL_NO_CHANGE,0.0,1.0,VAL_MANUAL,-dTemp,dTemp);
					break;
				case STAPANEL_CHART2_YRANGE_RING:
					GetCtrlVal(panel,control,&dTemp);
					SetAxisRange(panel,STAPANEL_STRIPCHART_2,VAL_NO_CHANGE,0.0,1.0,VAL_MANUAL,-dTemp,dTemp);	
					break;
				case STAPANEL_CHART3_YRANGE_RING:
					GetCtrlVal(panel,control,&dTemp);
					SetAxisRange(panel,STAPANEL_STRIPCHART_3,VAL_NO_CHANGE,0.0,1.0,VAL_MANUAL,-dTemp,dTemp);	
					break; 
				case STAPANEL_CHART4_YRANGE_RING:
					GetCtrlVal(panel,control,&dTemp);
					SetAxisRange(panel,STAPANEL_STRIPCHART_4,VAL_NO_CHANGE,0.0,1.0,VAL_MANUAL,-dTemp,dTemp);		
					break;
						
			} 
			
			break;
	}
	return 0;
}

 ///////**********���ò����ص�����************////////  
int CVICALLBACK StaticSetParaCallback (int panel, int control, int event,
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
								LightStaticParaSet();	//light the paraset
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
				LightStaticParaSet();
			}  */
			LightStaticParaSet();
			break;
	}
	return 0;
}

//�´����Ʋ���
int CVICALLBACK StaticDownParaCallback (int panel, int control, int event,
		void *callbackData, int eventData1, int eventData2)
{
	double dTemp;
	switch (event)
	{
		case EVENT_COMMIT:
				//��ָ����Ϣ���͸�ʵʱϵͳ����Ϣͷ������ָ���Լ���������ָ����ĳ���
				sendMessageHead.iCommand = SETPARA_PROCESS;
				
				sendMessageHead.iMessageLen = COMMANDSIZE;
			
				ClientTCPWrite(gTCPConnection, &sendMessageHead, MESSAGEHEADSIZE, 1000);         //��������ͷ
				Sctrlparadown();
				
				ClientTCPWrite(gTCPConnection, pExMem, COMMANDSIZE, 5000);		    //��������Ͳ���  
				
			
			break;
	}
	return 0;
}


int CVICALLBACK StaticSaveParaCallback (int panel, int control, int event,
		void *callbackData, int eventData1, int eventData2)
{
	double dTemp;
	switch (event)
	{
		case EVENT_COMMIT:
			if(ConfirmPopup("Confirm Popup","�����������������滻ԭ�ڴ��в�����"))
			{
			GetPanelHandleFromTabPage (StaticPanelHandle, STAPANEL_STA_TAB,1,&tabpanel1);
			
			//ͨ��1�Ŀ��Ʋ���
			GetPanelHandleFromTabPage(tabpanel1, PARA_TAB_CTRL_PARA_TAB,0,&tabpanel2);
			GetCtrlVal(tabpanel2,CH1_P_TAB_NUM_STATIC_KTP,&dTemp);
			Ini_PutDouble(inihandle,"CtrlPara","CH1_Ktp",dTemp);
			GetCtrlVal(tabpanel2,CH1_P_TAB_NUM_STATIC_KTI,&dTemp);
			Ini_PutDouble(inihandle,"CtrlPara","CH1_Kti",dTemp); 
			GetCtrlVal(tabpanel2,CH1_P_TAB_NUM_STATIC_KTD,&dTemp);
			Ini_PutDouble(inihandle,"CtrlPara","CH1_Ktd",dTemp);
			GetCtrlVal(tabpanel2,CH1_P_TAB_NUM_STATIC_FT1,&dTemp);
			Ini_PutDouble(inihandle,"CtrlPara","CH1_Ft1",dTemp);
			GetCtrlVal(tabpanel2,CH1_P_TAB_NUM_STATIC_FT2,&dTemp);
			Ini_PutDouble(inihandle,"CtrlPara","CH1_Ft2",dTemp); 
			GetCtrlVal(tabpanel2,CH1_P_TAB_NUM_STATIC_FTK,&dTemp);
			Ini_PutDouble(inihandle,"CtrlPara","CH1_Ftk",dTemp); 
			GetCtrlVal(tabpanel2,CH1_P_TAB_NUM_STATIC_X1,&dTemp);
			Ini_PutDouble(inihandle,"CtrlPara","CH1_X1",dTemp);
			GetCtrlVal(tabpanel2,CH1_P_TAB_NUM_STATIC_X2,&dTemp);
			Ini_PutDouble(inihandle,"CtrlPara","CH1_X2",dTemp); 
			GetCtrlVal(tabpanel2,CH1_P_TAB_NUM_STATIC_X3,&dTemp);
			Ini_PutDouble(inihandle,"CtrlPara","CH1_X3",dTemp); 
			
			//ͨ��2�Ŀ��Ʋ���
			GetPanelHandleFromTabPage(tabpanel1, PARA_TAB_CTRL_PARA_TAB,1,&tabpanel2);
			GetCtrlVal(tabpanel2,CH2_P_TAB_NUM_STATIC_KTP,&dTemp);
			Ini_PutDouble(inihandle,"CtrlPara","CH2_Ktp",dTemp);
			GetCtrlVal(tabpanel2,CH2_P_TAB_NUM_STATIC_KTI,&dTemp);
			Ini_PutDouble(inihandle,"CtrlPara","CH2_Kti",dTemp); 
			GetCtrlVal(tabpanel2,CH2_P_TAB_NUM_STATIC_KTD,&dTemp);
			Ini_PutDouble(inihandle,"CtrlPara","CH2_Ktd",dTemp);
			GetCtrlVal(tabpanel2,CH2_P_TAB_NUM_STATIC_FT1,&dTemp);
			Ini_PutDouble(inihandle,"CtrlPara","CH2_Ft1",dTemp);
			GetCtrlVal(tabpanel2,CH2_P_TAB_NUM_STATIC_FT2,&dTemp);
			Ini_PutDouble(inihandle,"CtrlPara","CH2_Ft2",dTemp); 
			GetCtrlVal(tabpanel2,CH2_P_TAB_NUM_STATIC_FTK,&dTemp);
			Ini_PutDouble(inihandle,"CtrlPara","CH2_Ftk",dTemp);  
			GetCtrlVal(tabpanel2,CH2_P_TAB_NUM_STATIC_X1,&dTemp);
			Ini_PutDouble(inihandle,"CtrlPara","CH2_X1",dTemp);
			GetCtrlVal(tabpanel2,CH2_P_TAB_NUM_STATIC_X2,&dTemp);
			Ini_PutDouble(inihandle,"CtrlPara","CH2_X2",dTemp); 
			GetCtrlVal(tabpanel2,CH2_P_TAB_NUM_STATIC_X3,&dTemp);
			Ini_PutDouble(inihandle,"CtrlPara","CH2_X3",dTemp); 
			
			//ͨ��3�Ŀ��Ʋ���
			GetPanelHandleFromTabPage(tabpanel1, PARA_TAB_CTRL_PARA_TAB,2,&tabpanel2);
			GetCtrlVal(tabpanel2,CH3_P_TAB_NUM_STATIC_KTP,&dTemp);
			Ini_PutDouble(inihandle,"CtrlPara","CH3_Ktp",dTemp);
			GetCtrlVal(tabpanel2,CH3_P_TAB_NUM_STATIC_KTI,&dTemp);
			Ini_PutDouble(inihandle,"CtrlPara","CH3_Kti",dTemp); 
			GetCtrlVal(tabpanel2,CH3_P_TAB_NUM_STATIC_KTD,&dTemp);
			Ini_PutDouble(inihandle,"CtrlPara","CH3_Ktd",dTemp);
			GetCtrlVal(tabpanel2,CH3_P_TAB_NUM_STATIC_FT1,&dTemp);
			Ini_PutDouble(inihandle,"CtrlPara","CH3_Ft1",dTemp);
			GetCtrlVal(tabpanel2,CH3_P_TAB_NUM_STATIC_FT2,&dTemp);
			Ini_PutDouble(inihandle,"CtrlPara","CH3_Ft2",dTemp); 
			GetCtrlVal(tabpanel2,CH3_P_TAB_NUM_STATIC_FTK,&dTemp);
			Ini_PutDouble(inihandle,"CtrlPara","CH3_Ftk",dTemp); 
			GetCtrlVal(tabpanel2,CH3_P_TAB_NUM_STATIC_X1,&dTemp);
			Ini_PutDouble(inihandle,"CtrlPara","CH3_X1",dTemp);
			GetCtrlVal(tabpanel2,CH3_P_TAB_NUM_STATIC_X2,&dTemp);
			Ini_PutDouble(inihandle,"CtrlPara","CH3_X2",dTemp); 
			GetCtrlVal(tabpanel2,CH3_P_TAB_NUM_STATIC_X3,&dTemp);
			Ini_PutDouble(inihandle,"CtrlPara","CH3_X3",dTemp); 
			
			//ͨ��4�Ŀ��Ʋ���
			GetPanelHandleFromTabPage(tabpanel1, PARA_TAB_CTRL_PARA_TAB,3,&tabpanel2);
			GetCtrlVal(tabpanel2,CH4_P_TAB_NUM_STATIC_KTP,&dTemp);
			Ini_PutDouble(inihandle,"CtrlPara","CH4_Ktp",dTemp);
			GetCtrlVal(tabpanel2,CH4_P_TAB_NUM_STATIC_KTI,&dTemp);
			Ini_PutDouble(inihandle,"CtrlPara","CH4_Kti",dTemp); 
			GetCtrlVal(tabpanel2,CH4_P_TAB_NUM_STATIC_KTD,&dTemp);
			Ini_PutDouble(inihandle,"CtrlPara","CH4_Ktd",dTemp);
			GetCtrlVal(tabpanel2,CH4_P_TAB_NUM_STATIC_FT1,&dTemp);
			Ini_PutDouble(inihandle,"CtrlPara","CH4_Ft1",dTemp);
			GetCtrlVal(tabpanel2,CH4_P_TAB_NUM_STATIC_FT2,&dTemp);
			Ini_PutDouble(inihandle,"CtrlPara","CH4_Ft2",dTemp); 
			GetCtrlVal(tabpanel2,CH4_P_TAB_NUM_STATIC_FTK,&dTemp);
			Ini_PutDouble(inihandle,"CtrlPara","CH4_Ftk",dTemp);
			GetCtrlVal(tabpanel2,CH4_P_TAB_NUM_STATIC_X1,&dTemp);
			Ini_PutDouble(inihandle,"CtrlPara","CH4_X1",dTemp);
			GetCtrlVal(tabpanel2,CH4_P_TAB_NUM_STATIC_X2,&dTemp);
			Ini_PutDouble(inihandle,"CtrlPara","CH4_X2",dTemp); 
			GetCtrlVal(tabpanel2,CH4_P_TAB_NUM_STATIC_X3,&dTemp);
			Ini_PutDouble(inihandle,"CtrlPara","CH4_X3",dTemp); 
			
		    Ini_WriteToFile (inihandle, pathname);  	
			}
			else
			{}
			break;
	}
	return 0;
}

int CVICALLBACK StaCh1TesttypeChange (int panel, int control, int event,
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
					/*
					SetCtrlAttribute(panel,CH1_TAB_CH_WAVE,ATTR_DIMMED,1); 
					SetCtrlAttribute(panel,CH1_TAB_CH_FREQ,ATTR_DIMMED,1);
					SetCtrlAttribute(panel,CH1_TAB_CH_AMP,ATTR_DIMMED,0);
					SetCtrlAttribute(panel,CH1_TAB_CH_SCAN_TYPE,ATTR_DIMMED,0); 
					SetCtrlAttribute(panel,CH1_TAB_CH_START_FREQ,ATTR_DIMMED,0); 
					SetCtrlAttribute(panel,CH1_TAB_CH_STOP_FREQ,ATTR_DIMMED,0);
					*/
					break;
				case ANY_CHART_TRACE:
					/*
					SetCtrlAttribute(panel,CH1_TAB_CH_WAVE,ATTR_DIMMED,0); 
					SetCtrlAttribute(panel,CH1_TAB_CH_FREQ,ATTR_DIMMED,0);
					SetCtrlAttribute(panel,CH1_TAB_CH_AMP,ATTR_DIMMED,0);
					SetCtrlAttribute(panel,CH1_TAB_CH_SCAN_TYPE,ATTR_DIMMED,1); 
					SetCtrlAttribute(panel,CH1_TAB_CH_START_FREQ,ATTR_DIMMED,1); 
					SetCtrlAttribute(panel,CH1_TAB_CH_STOP_FREQ,ATTR_DIMMED,1);
					*/
					break; 
			}
			 
			break;
	}
	return 0;
}

int CVICALLBACK StaCh2TesttypeChange (int panel, int control, int event,
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
					/*
					SetCtrlAttribute(panel,CH2_TAB_CH_WAVE,ATTR_DIMMED,1); 
					SetCtrlAttribute(panel,CH2_TAB_CH_FREQ,ATTR_DIMMED,1);
					SetCtrlAttribute(panel,CH2_TAB_CH_AMP,ATTR_DIMMED,0);
					SetCtrlAttribute(panel,CH2_TAB_CH_SCAN_TYPE,ATTR_DIMMED,0); 
					SetCtrlAttribute(panel,CH2_TAB_CH_START_FREQ,ATTR_DIMMED,0); 
					SetCtrlAttribute(panel,CH2_TAB_CH_STOP_FREQ,ATTR_DIMMED,0);
					*/
					break;
				case ANY_CHART_TRACE:
					/*
					SetCtrlAttribute(panel,CH2_TAB_CH_WAVE,ATTR_DIMMED,0); 
					SetCtrlAttribute(panel,CH2_TAB_CH_FREQ,ATTR_DIMMED,0);
					SetCtrlAttribute(panel,CH2_TAB_CH_AMP,ATTR_DIMMED,0);
					SetCtrlAttribute(panel,CH2_TAB_CH_SCAN_TYPE,ATTR_DIMMED,1); 
					SetCtrlAttribute(panel,CH2_TAB_CH_START_FREQ,ATTR_DIMMED,1); 
					SetCtrlAttribute(panel,CH2_TAB_CH_STOP_FREQ,ATTR_DIMMED,1);
					*/
					break; 
			}
			 
			break;
	}
	return 0;
}

int CVICALLBACK StaCh3TesttypeChange (int panel, int control, int event,
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
					/*
					SetCtrlAttribute(panel,CH3_TAB_CH_WAVE,ATTR_DIMMED,1); 
					SetCtrlAttribute(panel,CH3_TAB_CH_FREQ,ATTR_DIMMED,1);
					SetCtrlAttribute(panel,CH3_TAB_CH_AMP,ATTR_DIMMED,0);
					SetCtrlAttribute(panel,CH3_TAB_CH_SCAN_TYPE,ATTR_DIMMED,0); 
					SetCtrlAttribute(panel,CH3_TAB_CH_START_FREQ,ATTR_DIMMED,0); 
					SetCtrlAttribute(panel,CH3_TAB_CH_STOP_FREQ,ATTR_DIMMED,0);
					*/
					break;
				case ANY_CHART_TRACE:
					/*
					SetCtrlAttribute(panel,CH3_TAB_CH_WAVE,ATTR_DIMMED,0); 
					SetCtrlAttribute(panel,CH3_TAB_CH_FREQ,ATTR_DIMMED,0);
					SetCtrlAttribute(panel,CH3_TAB_CH_AMP,ATTR_DIMMED,0);
					SetCtrlAttribute(panel,CH3_TAB_CH_SCAN_TYPE,ATTR_DIMMED,1); 
					SetCtrlAttribute(panel,CH3_TAB_CH_START_FREQ,ATTR_DIMMED,1); 
					SetCtrlAttribute(panel,CH3_TAB_CH_STOP_FREQ,ATTR_DIMMED,1);
					*/
					break; 
			}
			 
			break;
	}
	return 0;
}

int CVICALLBACK StaCh4TesttypeChange (int panel, int control, int event,
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
					/*
					SetCtrlAttribute(panel,CH4_TAB_CH_WAVE,ATTR_DIMMED,1); 
					SetCtrlAttribute(panel,CH4_TAB_CH_FREQ,ATTR_DIMMED,1);
					SetCtrlAttribute(panel,CH4_TAB_CH_AMP,ATTR_DIMMED,0);
					SetCtrlAttribute(panel,CH4_TAB_CH_SCAN_TYPE,ATTR_DIMMED,0); 
					SetCtrlAttribute(panel,CH4_TAB_CH_START_FREQ,ATTR_DIMMED,0); 
					SetCtrlAttribute(panel,CH4_TAB_CH_STOP_FREQ,ATTR_DIMMED,0);
					*/
					break;
				case ANY_CHART_TRACE:
					/*
					SetCtrlAttribute(panel,CH4_TAB_CH_WAVE,ATTR_DIMMED,0); 
					SetCtrlAttribute(panel,CH4_TAB_CH_FREQ,ATTR_DIMMED,0);
					SetCtrlAttribute(panel,CH4_TAB_CH_AMP,ATTR_DIMMED,0);
					SetCtrlAttribute(panel,CH4_TAB_CH_SCAN_TYPE,ATTR_DIMMED,1); 
					SetCtrlAttribute(panel,CH4_TAB_CH_START_FREQ,ATTR_DIMMED,1); 
					SetCtrlAttribute(panel,CH4_TAB_CH_STOP_FREQ,ATTR_DIMMED,1);
					*/
					break; 
				case ANALOGY_FRE_SCAN_TEST:
					/*
					SetCtrlAttribute(panel,CH4_TAB_CH_WAVE,ATTR_DIMMED,1); 
					SetCtrlAttribute(panel,CH4_TAB_CH_FREQ,ATTR_DIMMED,1);
					SetCtrlAttribute(panel,CH4_TAB_CH_AMP,ATTR_DIMMED,1);
					SetCtrlAttribute(panel,CH4_TAB_CH_SCAN_TYPE,ATTR_DIMMED,1); 
					SetCtrlAttribute(panel,CH4_TAB_CH_START_FREQ,ATTR_DIMMED,1); 
					SetCtrlAttribute(panel,CH4_TAB_CH_STOP_FREQ,ATTR_DIMMED,1);
					*/
					break;
					
			}
			 
			break;
	}
	return 0;
}

 ///////**********��Դ���ƻص�����************//////// 
int CVICALLBACK StaticOilCtrlCallback (int panel, int control, int event,
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
 
int CVICALLBACK StaticDataReDisplay (int panel, int control, int event,
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
  
int CVICALLBACK StaTabChanelSelect (int panel, int control, int event,
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
				SetCtrlVal(StaticPanelHandle,STAPANEL_CHART1_CH_RING,eventData2);
				SetCtrlVal(StaticPanelHandle,STAPANEL_CHART2_CH_RING,eventData2);
				SetCtrlVal(StaticPanelHandle,STAPANEL_CHART3_CH_RING,eventData2);
				SetCtrlVal(StaticPanelHandle,STAPANEL_CHART4_CH_RING,eventData2);
				
				InitStaticDisIndex();
			
				if(pExMem->testPara[chart3_ch].iStatus) //�����ѡͨ����״̬Ϊ��
				{
					N = (long)(2000 / (pExMem->testPara[chart3_ch].dFreq) * CYCLE_NUM_PER_SCREEN);  //��Ļ��ʾ�������㹫ʽ
					if(N >= 3 && N <= 10000)
					{
						SetCtrlAttribute(StaticPanelHandle,STAPANEL_STRIPCHART_1,ATTR_POINTS_PER_SCREEN,N);
						SetCtrlAttribute(StaticPanelHandle,STAPANEL_STRIPCHART_2,ATTR_POINTS_PER_SCREEN,N);
						SetCtrlAttribute(StaticPanelHandle,STAPANEL_STRIPCHART_3,ATTR_POINTS_PER_SCREEN,N);
						SetCtrlAttribute(StaticPanelHandle,STAPANEL_STRIPCHART_4,ATTR_POINTS_PER_SCREEN,N);
					}
				
					for(ringindex = 0;ringindex < 11;ringindex++)  //����ֵ������Χ������y����
					{
						GetValueFromIndex(StaticPanelHandle,STAPANEL_CHART3_YRANGE_RING,ringindex,&dring_val);
						if(pExMem->testPara[chart1_ch].dAmp < dring_val)
						{
							SetCtrlIndex(StaticPanelHandle,STAPANEL_CHART3_YRANGE_RING,ringindex);
							SetCtrlIndex(StaticPanelHandle,STAPANEL_CHART4_YRANGE_RING,ringindex);
							SetAxisRange(StaticPanelHandle,STAPANEL_STRIPCHART_3,VAL_NO_CHANGE,0.0,1.0,VAL_MANUAL,-dring_val,dring_val);
							SetAxisRange(StaticPanelHandle,STAPANEL_STRIPCHART_4,VAL_NO_CHANGE,0.0,1.0,VAL_MANUAL,-dring_val,dring_val);
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
			iDataLoops =  iDataLen / transdatasizeS;
		
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
					memcpy((saveBuf + saveIndex ),readBuff,DataSize); 
					saveIndex += iDataLoops;
				}
					
				
				for(i=0;i<iDataLoops;i++)
				{
					//��λ������������ÿ��������32��double�͵�������Ϊ4�飬0-7����ָ��ǽǶ�ָ�������ָ���ɲ�����Ŀ����
					//8-15��λ���ز�����15-23��Ϊ�ǶȲ�����23-31��λ���������ÿ�����͵���ռ8�������ͨ��������8����ʣ��ľ���������
					chart1[2*i] = 0;	 //�Ƕ�ָ��
					chart1[2*i+1] = readData[transdatasizeS*i + 24 +chart1_ch-1]; //�ǶȲ���
					
					chart3[2*i] = readData[transdatasizeS*i + chart3_ch-1];	 //��ָ��    
					chart3[2*i+1] = readData[transdatasizeS*i + 8 + chart3_ch-1];	 //������       
				
					switch(chart2_content)
					{
						case ANGLE_SAMPLE_INDEX:
								chart2[i] = readData[transdatasizeS*i+28+chart2_ch-1];
							break;
						case ANGLE_GIVEN_INDEX:
								chart2[i] = readData[transdatasizeS*i+0+chart2_ch-1];  
							break;
						case TORQUE_SAMPLE_INDEX:
								chart2[i] = readData[transdatasizeS*i+8+chart2_ch-1];  
							break;
						case TORQUE_GIVEN_INDEX:
								chart2[i] = readData[transdatasizeS*i+0+chart2_ch-1];      
							break;
						case SERVO_OUT_INDEX:
								chart2[i] = readData[transdatasizeS*i+16+chart2_ch-1];      
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
								chart4[i] = readData[transdatasizeS*i+28+chart4_ch-1];
							break;
						case ANGLE_GIVEN_INDEX:
								chart4[i] = readData[transdatasizeS*i+0+chart4_ch-1]; 
							break;
						case TORQUE_SAMPLE_INDEX:
								chart4[i] = readData[transdatasizeS*i+8+chart4_ch-1]; 
							break;
						case TORQUE_GIVEN_INDEX:
								chart4[i] = readData[transdatasizeS*i+0+chart4_ch-1];
							break;
						case SERVO_OUT_INDEX:
								chart4[i] = readData[transdatasizeS*i+16+chart4_ch-1]; 
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
					PlotStripChart(StaticPanelHandle,STAPANEL_STRIPCHART_1,chart1,2*iDataLoops,0,0,VAL_DOUBLE);
					PlotStripChart(StaticPanelHandle,STAPANEL_STRIPCHART_2,chart2,iDataLoops,0,0,VAL_DOUBLE);  
					PlotStripChart(StaticPanelHandle,STAPANEL_STRIPCHART_3,chart3,2*iDataLoops,0,0,VAL_DOUBLE);
					PlotStripChart(StaticPanelHandle,STAPANEL_STRIPCHART_4,chart4,iDataLoops,0,0,VAL_DOUBLE); 
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

int CVICALLBACK NetWatchDog (int panel, int control, int event,
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

int CVICALLBACK Enablecallback (int panel, int control, int event,
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
			InitStaticTestPara(); 
			if (ns > 1)	   //��������ݣ��Ͷ������е�����
			{
				ns=ns-2;
			}
			//GetCtrlVal(tabpanel2,STAPANEL_LED,&n);
		
			if (ns == 0)	   //��������ݣ��Ͷ������е�����
			{
				SetCtrlAttribute(panel,control,ATTR_LABEL_TEXT,"ֹͣʹ��"); 
				GetPanelHandleFromTabPage (StaticPanelHandle, STAPANEL_STA_TAB,0,&tabpanel1);
	
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
				 GetPanelHandleFromTabPage (StaticPanelHandle, STAPANEL_STA_TAB,0,&tabpanel1); 
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
			ns=ns+1;
			
			break;
	}
	return 0;
}

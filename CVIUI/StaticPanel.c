#include <cvirte.h>		
#include <userint.h>
#include "StaticPanel.h"
#include "Functions.h"
#include "ExtraVar.h"
#include <tcpsupp.h>
#include <formatio.h>
#include <utility.h>
#include <ansi_c.h>

extern char DftPathSign; //默认路径标志位
extern char pathpass[];  //路径存放数组
extern char originalkey[];
extern int keyerr;
extern int ns=0; 
static int StaticPanelHandle;

#define ANY_CHART_TRACE              1
#define FRE_SCAN_TEST     	         2
#define ANALOGY_FRE_SCAN_TEST     	 9   

#define  READBUFFERLEN	  1024    

TESTDATA ReadDataMessage;
//TESTDATANEW  ReadDataMessageBuff[READBUFFERLEN];  //采用了新的发送数据方式，发送16位的字节数
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


//静止加载启动界面
int StaticPanelInit(void)
{
	
	if ((StaticPanelHandle = LoadPanel (0, "StaticPanel.uir", STAPANEL)) < 0)
	  return -1;
	
	
	if (ConnectToTCPServer (&gTCPConnection, HOST_PORT, HOST_ADDRESS, TCPDataCallback, 0, 2000) < 0)
	{
		//SetCtrlVal(runningpanel,RUNPANEL_MSGSTRING,"连接实时主机失败，请确认实时程序已经运行，且网络连接正常！");	
		MessagePopup ("Error", "连接实时主机失败，请确认实时程序已经运行，且网络连接正常！");    
		goto Error;
	}
	else
	{
		//SetCtrlVal(StaticPanelHandle,RUNPANEL_MSGSTRING,"实时主机连接正常！");	       
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

//静止加载控制参数初始化
void InitStaticCtrlSet(void)
{
    GetPanelHandleFromTabPage (StaticPanelHandle, STAPANEL_STA_TAB, 1, &tabpanel1);
	
	//通道1的控制参数
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
	
	//通道2的控制参数
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
	
	//通道3的控制参数	
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
	
	//通道4的控制参数
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
			
				//通道1的控制参数
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
			
				//通道2的控制参数
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
			
				//通道3的控制参数
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
		
				//通道4的控制参数
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
//初始化静态加载测试参数
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
			
	//通道1的测试参数
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
		pExMem->testPara[1].iStatus = 2;  		  //1表示位置通道，2表示加载通道
	}
	else
	{
		pExMem->testPara[1].iStatus = 0;  		  //1表示位置通道，2表示加载通道
	}
	GetCtrlVal(tabpanel2,CH1_TAB_CH_WAVE,&tempInt); 
	pExMem->testPara[1].iWave = tempInt; 
	GetCtrlVal(tabpanel2,CH1_TAB_CH_TEST_TYPE,&tempInt); 
	pExMem->testPara[1].iTestType = tempInt;

	
	//通道2的测试参数 
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
		pExMem->testPara[2].iStatus = 2;  		  //1表示位置通道，2表示加载通道
	}
	else
	{
		pExMem->testPara[2].iStatus = 0;  		  //1表示位置通道，2表示加载通道
	}
	GetCtrlVal(tabpanel2,CH2_TAB_CH_WAVE,&tempInt); 
	pExMem->testPara[2].iWave = tempInt; 
	GetCtrlVal(tabpanel2,CH2_TAB_CH_TEST_TYPE,&tempInt); 
	pExMem->testPara[2].iTestType = tempInt; 

	
	//通道3的测试参数 
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
		pExMem->testPara[3].iStatus = 2;  		  //1表示位置通道，2表示加载通道
	}
	else
	{
		pExMem->testPara[3].iStatus = 0;  		  //1表示位置通道，2表示加载通道
	}
	GetCtrlVal(tabpanel2,CH3_TAB_CH_WAVE,&tempInt); 
	pExMem->testPara[3].iWave = tempInt; 
	GetCtrlVal(tabpanel2,CH3_TAB_CH_TEST_TYPE,&tempInt); 
	pExMem->testPara[3].iTestType = tempInt; 

	
	//通道4的测试参数 
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
		pExMem->testPara[4].iStatus = 2;  		  //1表示位置通道，2表示加载通道
	}
	else
	{
		pExMem->testPara[4].iStatus = 0;  		  //1表示位置通道，2表示加载通道
	}
	
	GetCtrlVal(tabpanel2,CH4_TAB_CH_WAVE,&tempInt); 
	pExMem->testPara[4].iWave = tempInt; 
	
	GetCtrlVal(tabpanel2,CH4_TAB_CH_TEST_TYPE,&tempInt); 
	pExMem->testPara[4].iTestType = tempInt; 
	
		
}

void StaticSetScreenCycNum(void)
{
	double dfreq;	   //预设频率
	double dring_val;  //坐标分段值
	long N;   //每屏显示点数
	int i,ringindex;
	
	for(i=1;i<5;i++)
	{
		if(pExMem->testPara[i].iStatus && (chart3_ch == i))
		{
			N = (long)(2000 / (pExMem->testPara[i].dFreq) * CYCLE_NUM_PER_SCREEN);  //屏幕显示点数计算公式
			if(N >= 3 && N <= 10000)
			{
				SetCtrlAttribute(StaticPanelHandle,STAPANEL_STRIPCHART_3,ATTR_POINTS_PER_SCREEN,N);
				SetCtrlAttribute(StaticPanelHandle,STAPANEL_STRIPCHART_4,ATTR_POINTS_PER_SCREEN,N);
				SetCtrlAttribute(StaticPanelHandle,STAPANEL_STRIPCHART_1,ATTR_POINTS_PER_SCREEN,N);
				SetCtrlAttribute(StaticPanelHandle,STAPANEL_STRIPCHART_2,ATTR_POINTS_PER_SCREEN,N);
			}
			
			for(ringindex = 0;ringindex < 11;ringindex++)  //检测幅值所处范围并设置y坐标
			{
				GetValueFromIndex(StaticPanelHandle,STAPANEL_CHART3_YRANGE_RING,ringindex,&dring_val);  //得到坐标的分段值
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
	//初始化显示索引
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

//初始化控制参数
void InitStaticCtrlPara(void)
{
	double dTemp;
	//获取ini文件句柄
	inihandle = Ini_New(0);
	//读取ini文件
	//GetWindowsDirs (windowsDirectory, systemDirectory);
	//strcpy( pathname ,windowsDirectory);
	
	GetDir (currentDir);
	strcpy( pathname ,currentDir);
	strcat(pathname,"\\StaticPara.ini");
	
	inierr = Ini_ReadFromFile(inihandle, pathname); 
	if( inierr < 0)
	{
		sprintf(msgBuff,"大开参数文件错误！err = %d",inierr);
		MessagePopup ("Err",msgBuff);  
		//打开文件失败
	}
	else
	{
		//通道1---kap\kai\kad位置PID参数
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
		//三个模糊参数-----fa1\fa2\fak
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
		//三个备用参数-----X1\X2\X3
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
		
		//通道2
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
		//三个模糊参数
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
		//三个备用参数-----X1\X2\X3
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
		
		//通道3
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
		//三个模糊参数
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
		//三个备用参数-----X1\X2\X3
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
		
		//通道4
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
		//三个模糊参数
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
		//三个备用参数-----X1\X2\X3
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

//使能控制参数设置
void LightStaticParaSet(void)
{
	GetPanelHandleFromTabPage (StaticPanelHandle, STAPANEL_STA_TAB, 1, &tabpanel1);
	
	//通道1的控制参数
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

////////////////////以下是回调函数////////////////////////
//////**********面板回调函数************////////

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
				SetCtrlAttribute(panel,control,ATTR_LABEL_TEXT,"停止记录");
			}
			else
			{
				recording = 0;
				SetCtrlAttribute(panel,control,ATTR_LABEL_TEXT,"开始记录");  
			}
			break;
	}
	return 0;
}

 ///////**********保存数据回调函数************////////  
int CVICALLBACK StaticSaveCallback (int panel, int control, int event,
		void *callbackData, int eventData1, int eventData2)
{
	switch (event)
	{
		case EVENT_COMMIT:
			strcpy(saveDir,"D:\\测试数据" );
		//	err = MakeDir (saveDir);
			strcat(saveDir,"\\" );
			strcat(saveDir, DateStr());
			//err = MakeDir (saveDir);
			if(FileSelectPopup (saveDir, "*.dat", "*.dat", "数据保存", VAL_SAVE_BUTTON, 0, 1, 1, 0,savepath ) != 0)
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
			if(ConfirmPopup ("确定启动", "确定启动？"))
			{
				
				StaticSetScreenCycNum();         //设置屏幕显示点数
				
				//将指令信息发送给实时系统，消息头，包括指令以及后续具体指令包的长度
				sendMessageHead.iCommand = SETPARA_PROCESS;
				sendMessageHead.iMessageLen = COMMANDSIZE;
				ClientTCPWrite(gTCPConnection, &sendMessageHead, MESSAGEHEADSIZE, 1000);         //发送命令头
				Sctrlparadown();
				ClientTCPWrite(gTCPConnection, pExMem, COMMANDSIZE, 5000);
				
				sendMessageHead.iCommand = START_PROCESS;
				sendMessageHead.iMessageLen = COMMANDSIZE;
				ClientTCPWrite(gTCPConnection, &sendMessageHead, MESSAGEHEADSIZE, 1000);         //发送命令头
				InitStaticTestPara();			 //初始化测试参数   
				ClientTCPWrite(gTCPConnection, pExMem, COMMANDSIZE, 5000);		    //发送命令    
				
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
			
			//将指令信息发送给实时系统，消息头
			sendMessageHead.iCommand = RESET_PROCESS;
			sendMessageHead.iMessageLen = 0;
			ClientTCPWrite(gTCPConnection, &sendMessageHead, MESSAGEHEADSIZE, 1000);         //发送命令头
			 
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
			if(ConfirmPopup ("确定退出", "确定退出？"))
			{

				//将指令信息发送给实时系统，消息头
				sendMessageHead.iCommand = PAUSE_PROCESS;
			
				sendMessageHead.iMessageLen = 0;
		
				ClientTCPWrite(gTCPConnection, &sendMessageHead, MESSAGEHEADSIZE, 1000);         //发送命令头
				
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
			GetCtrlVal(panel,control,&iTemp);								//设置四个strips显示通道相同
			SetCtrlVal(StaticPanelHandle,STAPANEL_CHART1_CH_RING,iTemp);
			SetCtrlVal(StaticPanelHandle,STAPANEL_CHART2_CH_RING,iTemp);
			SetCtrlVal(StaticPanelHandle,STAPANEL_CHART3_CH_RING,iTemp);
			SetCtrlVal(StaticPanelHandle,STAPANEL_CHART4_CH_RING,iTemp);
			
			InitStaticDisIndex();
			
			if(pExMem->testPara[chart3_ch].iStatus) //如果所选通道的状态为开
			{
				N = (long)(2000 / (pExMem->testPara[chart3_ch].dFreq) * CYCLE_NUM_PER_SCREEN);  //屏幕显示点数计算公式
				if(N >= 3 && N <= 10000)
				{
					SetCtrlAttribute(StaticPanelHandle,STAPANEL_STRIPCHART_1,ATTR_POINTS_PER_SCREEN,N);
					SetCtrlAttribute(StaticPanelHandle,STAPANEL_STRIPCHART_2,ATTR_POINTS_PER_SCREEN,N);
					SetCtrlAttribute(StaticPanelHandle,STAPANEL_STRIPCHART_3,ATTR_POINTS_PER_SCREEN,N);
					SetCtrlAttribute(StaticPanelHandle,STAPANEL_STRIPCHART_4,ATTR_POINTS_PER_SCREEN,N);
				}
				
				for(ringindex = 0;ringindex < 11;ringindex++)  //检测幅值所处范围并设置y坐标
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

///////**********Y坐标范围回调函数************//////// 
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

 ///////**********设置参数回调函数************////////  
int CVICALLBACK StaticSetParaCallback (int panel, int control, int event,
		void *callbackData, int eventData1, int eventData2)
{
	double dTemp;
	int selbutton;          
	char keyinputbuffer[65];
	
	switch (event)
	{
		case EVENT_COMMIT:
			/*if(keyerr)  //检查密码标志位
			{
				selbutton = GenericMessagePopup("Generic Message Popup","控制参数修改需要管理员权限，请输入密码：",
												"确定","取消",0,keyinputbuffer,64,0,VAL_GENERIC_POPUP_INPUT_STRING,
												VAL_GENERIC_POPUP_BTN1,VAL_GENERIC_POPUP_BTN2);
				switch (selbutton)
				{
					case VAL_GENERIC_POPUP_BTN1:
						keyerr = strcmp(keyinputbuffer,originalkey); //密码比较
						if(!keyerr)									 //如果吻合
						{
							//确认
							if(ConfirmPopup("Confirm Popup","密码正确！修改控制参数将影响系统控制性能，进行修改？"))
							{
								keyerr = 0;
								LightStaticParaSet();	//light the paraset
							}
							else
							{
								MessagePopup("Message Popup","您已经取消了控制参数的修改操作！");
							}
						}
						else										 //如果不吻合
						{
							MessagePopup("Message Popup","输入密码错误！请核对密码后再进行操作！");
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

//下传控制参数
int CVICALLBACK StaticDownParaCallback (int panel, int control, int event,
		void *callbackData, int eventData1, int eventData2)
{
	double dTemp;
	switch (event)
	{
		case EVENT_COMMIT:
				//将指令信息发送给实时系统，消息头，包括指令以及后续具体指令包的长度
				sendMessageHead.iCommand = SETPARA_PROCESS;
				
				sendMessageHead.iMessageLen = COMMANDSIZE;
			
				ClientTCPWrite(gTCPConnection, &sendMessageHead, MESSAGEHEADSIZE, 1000);         //发送命令头
				Sctrlparadown();
				
				ClientTCPWrite(gTCPConnection, pExMem, COMMANDSIZE, 5000);		    //发送命令和参数  
				
			
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
			if(ConfirmPopup("Confirm Popup","保存此组参数？它将替换原内存中参数！"))
			{
			GetPanelHandleFromTabPage (StaticPanelHandle, STAPANEL_STA_TAB,1,&tabpanel1);
			
			//通道1的控制参数
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
			
			//通道2的控制参数
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
			
			//通道3的控制参数
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
			
			//通道4的控制参数
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

 ///////**********油源控制回调函数************//////// 
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
				selectstatus = FileSelectPopup("d:\\","*.exe",".exe;.dat;.dll","选择数据回放软件",
												VAL_SELECT_BUTTON,0,1,0,0,pathname);
				if(selectstatus > 0)
				{
					confirm = ConfirmPopup("Confirm Popup","将所选程序确定为默认的数据回放程序?");
					
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
				LaunchExecutable (pathpass);	//加载外部数据回放软件
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
			
				if(pExMem->testPara[chart3_ch].iStatus) //如果所选通道的状态为开
				{
					N = (long)(2000 / (pExMem->testPara[chart3_ch].dFreq) * CYCLE_NUM_PER_SCREEN);  //屏幕显示点数计算公式
					if(N >= 3 && N <= 10000)
					{
						SetCtrlAttribute(StaticPanelHandle,STAPANEL_STRIPCHART_1,ATTR_POINTS_PER_SCREEN,N);
						SetCtrlAttribute(StaticPanelHandle,STAPANEL_STRIPCHART_2,ATTR_POINTS_PER_SCREEN,N);
						SetCtrlAttribute(StaticPanelHandle,STAPANEL_STRIPCHART_3,ATTR_POINTS_PER_SCREEN,N);
						SetCtrlAttribute(StaticPanelHandle,STAPANEL_STRIPCHART_4,ATTR_POINTS_PER_SCREEN,N);
					}
				
					for(ringindex = 0;ringindex < 11;ringindex++)  //检测幅值所处范围并设置y坐标
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

//读下位机返回的数据
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
        case TCP_DATAREADY: // Client executable sent a command.	  //得到数据
			// ClientTCPRead does not guarantee that all data will be transferred in
			// a single call, but this code assumes that it does for simplicity.
			
			// ClientTCPRead does not guarantee that all data will be transferred in
			// a single call, but this code assumes that it does for simplicity.
			//InsertTextBoxLine(runningpanel, RUNPANEL_MSGSTRING,0,"收到命令反馈！");   
			
			bytesToRead = MESSAGEHEADSIZE;
			
			//printf("address1 = %x,address2=%x", &readCommandMessage, (char*)(&readCommandMessage) +1);
			
			while (bytesToRead > 0)		 //读帧头
			{
				bytesRead = ClientTCPRead (gTCPConnection, (char*)(&readMessageHead) + (MESSAGEHEADSIZE - bytesToRead), bytesToRead, 0);
				bytesToRead -= bytesRead;
			}
			
			
			iCommand = readMessageHead.iCommand;
			iDataLen = readMessageHead.iMessageLen;
			iDataLoops =  iDataLen / transdatasizeS;
		
			if(iCommand == 6)	   //如果是数据，就读出所有的数据
			{
				bytesToRead =  iDataLen*sizeof(double);  
				DataSize = bytesToRead;
				
				readData = (double *)malloc(iDataLen*sizeof(double));
				readBuff = (char *) malloc(iDataLen*sizeof(double));
				
				chart1 = (double *)malloc((sizeof(double)*iDataLoops) * 2);
				chart2 = (double *)malloc(sizeof(double)*iDataLoops);
				chart3 = (double *)malloc((sizeof(double)*iDataLoops) * 2);
				chart4 = (double *)malloc(sizeof(double)*iDataLoops);
				
				while (bytesToRead > 0)		 //读数据
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
					//下位机发上来的数每个周期是32个double型的数，分为4组，0-7个是指令，是角度指令还是力矩指令由测试项目决定
					//8-15个位力矩采样，15-23个为角度采样，23-31个位控制输出，每种类型的数占8个，如果通道数不到8个，剩余的就留作备用
					chart1[2*i] = 0;	 //角度指令
					chart1[2*i+1] = readData[transdatasizeS*i + 24 +chart1_ch-1]; //角度采样
					
					chart3[2*i] = readData[transdatasizeS*i + chart3_ch-1];	 //力指令    
					chart3[2*i+1] = readData[transdatasizeS*i + 8 + chart3_ch-1];	 //力采样       
				
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
			
			//将指令信息发送给实时系统，消息头，包括指令以及后续具体指令包的长度
			sendMessageHead.iCommand = WATCHDOG;   
			
			sendMessageHead.iMessageLen = 0;		 //也没有实际的内容
		
			ClientTCPWrite(gTCPConnection, &sendMessageHead, MESSAGEHEADSIZE, 1000);         //发送命令头

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
			ClientTCPWrite(gTCPConnection, &sendMessageHead, MESSAGEHEADSIZE, 1000);         //发送命令头
			InitStaticTestPara(); 
			if (ns > 1)	   //如果是数据，就读出所有的数据
			{
				ns=ns-2;
			}
			//GetCtrlVal(tabpanel2,STAPANEL_LED,&n);
		
			if (ns == 0)	   //如果是数据，就读出所有的数据
			{
				SetCtrlAttribute(panel,control,ATTR_LABEL_TEXT,"停止使能"); 
				GetPanelHandleFromTabPage (StaticPanelHandle, STAPANEL_STA_TAB,0,&tabpanel1);
	
				 //通道1的测试参数
				 GetPanelHandleFromTabPage(tabpanel1, SET_TAB_CH_TAB,0,&tabpanel2);
				 GetCtrlVal(tabpanel2,CH1_TAB_CH_ON_OFF,&tempInt); 
				 pExMem->testPara[1].iEnable = tempInt+1; 
				  
				 //通道2的测试参数
				 GetPanelHandleFromTabPage(tabpanel1, SET_TAB_CH_TAB,1,&tabpanel2);
				 GetCtrlVal(tabpanel2,CH2_TAB_CH_ON_OFF,&tempInt); 
				 pExMem->testPara[2].iEnable = tempInt+1;
				 
				 //通道3的测试参数
				 GetPanelHandleFromTabPage(tabpanel1, SET_TAB_CH_TAB,2,&tabpanel2);
				 GetCtrlVal(tabpanel2,CH3_TAB_CH_ON_OFF,&tempInt); 
				 pExMem->testPara[3].iEnable = tempInt+1;
				 
				 //通道4的测试参数
				 GetPanelHandleFromTabPage(tabpanel1, SET_TAB_CH_TAB,3,&tabpanel2);
				 GetCtrlVal(tabpanel2,CH4_TAB_CH_ON_OFF,&tempInt); 
				 pExMem->testPara[4].iEnable = tempInt+1; 
				 
				 //InitStaticTestPara();			 //初始化测试参数   	 
				}
			 else
			 {
				 SetCtrlAttribute(panel,control,ATTR_LABEL_TEXT,"使能");
				 GetPanelHandleFromTabPage (StaticPanelHandle, STAPANEL_STA_TAB,0,&tabpanel1); 
				 //通道1的测试参数
				 GetPanelHandleFromTabPage(tabpanel1, SET_TAB_CH_TAB,0,&tabpanel2);
				 GetCtrlVal(tabpanel2,CH1_TAB_CH_ON_OFF,&tempInt); 
				 pExMem->testPara[1].iEnable = 1; 
				  
				 //通道2的测试参数
				 GetPanelHandleFromTabPage(tabpanel1, SET_TAB_CH_TAB,1,&tabpanel2);
				 GetCtrlVal(tabpanel2,CH2_TAB_CH_ON_OFF,&tempInt); 
				 pExMem->testPara[2].iEnable = 1; 
				 
				 //通道3的测试参数
				 GetPanelHandleFromTabPage(tabpanel1, SET_TAB_CH_TAB,2,&tabpanel2);
				 GetCtrlVal(tabpanel2,CH3_TAB_CH_ON_OFF,&tempInt); 
				 pExMem->testPara[3].iEnable = 1; 
				 
				 //通道4的测试参数
				 GetPanelHandleFromTabPage(tabpanel1, SET_TAB_CH_TAB,3,&tabpanel2);
				 GetCtrlVal(tabpanel2,CH4_TAB_CH_ON_OFF,&tempInt); 
				 pExMem->testPara[4].iEnable = 1; 
						
				// InitStaticTestPara();			 //初始化测试参数
			 }
			 
			ClientTCPWrite(gTCPConnection, pExMem, COMMANDSIZE, 5000);		    //发送命令 
			ns=ns+1;
			
			break;
	}
	return 0;
}

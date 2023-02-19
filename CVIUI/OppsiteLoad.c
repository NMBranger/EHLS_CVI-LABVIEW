#include <userint.h> 
#include "ExtraVar.h"   
#include <tcpsupp.h>
#include <formatio.h>
#include <utility.h>
#include "OppsiteLoad.h"
#include "Functions.h"
#include "inifile.h" 

#define REMOVE_EXTRA_TORQUE  1
#define ANY_CHART_TRACE      2
#define PROPORTION_TRACE     3
#define TORQUE_TRACE         4  //�غ��׸���

const int OppsiteChannel[5]= {0,2,1,4,3};
extern char DftPathSign; //Ĭ��·����־λ
extern char pathpass[];  //·���������
extern char originalkey[] ;
extern int keyerr;
extern int no=0; 
static int oppstpanel;

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
int transdatasizeO = 36;

 /*    


int main (int argc, char *argv[])
{
	if (InitCVIRTE (0, argv, 0) == 0)
		return -1;	// out of memory 
	if ((oppstpanel = LoadPanel (0, "OppsiteLoad.uir", OPPSITEPNL)) < 0)
		return -1;
	DisplayPanel (oppstpanel);
	RunUserInterface ();
	DiscardPanel (oppstpanel);
	return 0;
}
*/

//��ʼ�����Բ���
void InitOppsiteTestPara(void)
{
		double dTemp;
		int tempInt;
		int iAngleChannel;
		int iTorqueChannel;
		int i;
		
		GetPanelHandleFromTabPage (OppsitePanelHandle, OPPSITEPNL_OPP_TAB,0,&tabpanel1);
		
		GetCtrlVal(tabpanel1,SET_TAB_ANGLE_CH_RING,&iAngleChannel); 
		GetCtrlVal(tabpanel1,SET_TAB_TORQUE_CH_RING,&iTorqueChannel);
		
		//GetPanelHandleFromTabPage (OppsitePanelHandle, OPPSITEPNL_STA_TAB,0,&tabpanel1);
	
		GetCtrlVal(OppsitePanelHandle,OPPSITEPNL_CH_TEST_TIME,&dTemp);   
		pExMem->testPara[1].dTestTime = dTemp;
		pExMem->testPara[2].dTestTime = dTemp;    		 
		pExMem->testPara[3].dTestTime = dTemp;    
		pExMem->testPara[4].dTestTime = dTemp;  
		
		
		for(i= 1; i<5; i++)
		{
			if(i == iAngleChannel)
			{
				pExMem->testPara[i].iStatus = 1; 				   //1��ʾλ��ͨ����2��ʾ����ͨ��         
			}
			else if(i == iTorqueChannel)
			{
				pExMem->testPara[i].iStatus = 2; 
			}
			else 
				pExMem->testPara[i].iStatus = 0;      
		}
		
		GetCtrlVal(tabpanel1,SET_TAB_ANGLE_GIVEN_AMP,&dTemp); 
		pExMem->testPara[iAngleChannel].dAmp = dTemp; 
		GetCtrlVal(tabpanel1,SET_TAB_ANGLE_GIVEN_FREQ,&dTemp); 
		pExMem->testPara[iAngleChannel].dFreq = dTemp; 
		GetCtrlVal(tabpanel1,SET_TAB_ANGLE_GIVEN_WAVE,&tempInt); 
		pExMem->testPara[iAngleChannel].iWave = tempInt; 
		GetCtrlVal(tabpanel1,SET_TAB_TORQUE_GIVEN_WAVE,&tempInt); 
		pExMem->testPara[iAngleChannel].iTestType= tempInt;  
		GetCtrlVal(tabpanel1,SET_TAB_CH_OFFSET_2,&dTemp);
	    pExMem->testPara[iAngleChannel].dOffset = dTemp;
		
		GetCtrlVal(tabpanel1,SET_TAB_CH_OFFSET,&dTemp);
	    pExMem->testPara[iTorqueChannel].dOffset = dTemp;
		GetCtrlVal(tabpanel1,SET_TAB_TORQUE_GIVEN_AMP,&dTemp); 
		pExMem->testPara[iTorqueChannel].dAmp = dTemp; 
		GetCtrlVal(tabpanel1,SET_TAB_TORQUE_GIVEN_FREQ,&dTemp); 
		pExMem->testPara[iTorqueChannel].dFreq = dTemp; 
		GetCtrlVal(tabpanel1,SET_TAB_TORQUE_GIVEN_WAVE,&tempInt); 
		pExMem->testPara[iTorqueChannel].iWave = tempInt; 
		GetCtrlVal(tabpanel1,SET_TAB_ANGLE_GIVEN_WAVE,&tempInt); 
		pExMem->testPara[iTorqueChannel].iTestType= tempInt; 
		
		if (tempInt == 8)
			{
				  SetCtrlAttribute(tabpanel1,SET_TAB_CH_OFFSET,ATTR_DIMMED,1); 
				  SetCtrlAttribute(tabpanel1,SET_TAB_TORQUE_GIVEN_FREQ,ATTR_DIMMED,1); 
				//SetCtrlVal(runningpanel,RUNPANEL_MSGSTRING,"����ʵʱ����ʧ�ܣ���ȷ��ʵʱ�����Ѿ����У�����������������");	
				
			}
		else
			{
				SetCtrlAttribute(tabpanel1,SET_TAB_CH_OFFSET,ATTR_DIMMED,0); 
				SetCtrlAttribute(tabpanel1,SET_TAB_TORQUE_GIVEN_FREQ,ATTR_DIMMED,0);
				//SetCtrlVal(StaticPanelHandle,RUNPANEL_MSGSTRING,"ʵʱ��������������");
			}
}

//�Զ���������ʼ��
int OppsitePanelInit(void)
{
	if ((OppsitePanelHandle = LoadPanel (0, "OppsiteLoad.uir", OPPSITEPNL)) < 0)
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
	
	 
	//�Զ����س�ʼ��
	
	InitOppsiteCtrlPara();	  //��ʼ�����Ʋ��� �������ʼ�� (��ini�ļ��������ݵ������ڴ棬�ٽ������ڴ�����ݶ����ؼ���) 
	InitOppsiteDisIndex();    //��ʼ����ʾ����
	DisplayPanel (OppsitePanelHandle);
	RunUserInterface ();

	
	DiscardPanel (OppsitePanelHandle);
	return 1;
	
Error:
    /* error operation */
	

Done: 
	/* clean up */   
	if (gTCPConnection != TCP_INVALID_CONNECTION)
		DisconnectFromTCPServer (gTCPConnection);
	
	return 0;
}

void OppositeSetScreenCycNum(void)
{
	double dfreq;	   //Ԥ��Ƶ��
	double dring_val;  //��������
	long N;   //ÿ����ʾ����
	int i,ringindex;
	
	for(i=1;i<5;i++)
	{
		if(pExMem->testPara[i].iStatus == 1)  //�����λ��ͨ��
		{
			N = (long)(2000 / (pExMem->testPara[i].dFreq) * CYCLE_NUM_PER_SCREEN);  //��Ļ��ʾ�������㹫ʽ
			if(N >= 3 && N <= 10000)
			{
				SetCtrlAttribute(OppsitePanelHandle,OPPSITEPNL_STRIPCHART_1,ATTR_POINTS_PER_SCREEN,N);//ֻ���ýǶ����chart
				SetCtrlAttribute(OppsitePanelHandle,OPPSITEPNL_STRIPCHART_2,ATTR_POINTS_PER_SCREEN,N);
			}
			
			for(ringindex = 0;ringindex < 11;ringindex++)  //����ֵ������Χ������y����
			{
				GetValueFromIndex(OppsitePanelHandle,OPPSITEPNL_YRANGE_RING_1,ringindex,&dring_val);
				if(pExMem->testPara[i].dAmp < dring_val)
				{
					//SetCtrlIndex(OppsitePanelHandle,OPPSITEPNL_YRANGE_RING_1,ringindex);
					//SetCtrlIndex(OppsitePanelHandle,OPPSITEPNL_YRANGE_RING_2,ringindex);
					//SetAxisRange(OppsitePanelHandle,OPPSITEPNL_STRIPCHART_1,VAL_NO_CHANGE,0.0,1.0,VAL_MANUAL,-dring_val,dring_val);
					//SetAxisRange(OppsitePanelHandle,OPPSITEPNL_STRIPCHART_2,VAL_NO_CHANGE,0.0,1.0,VAL_MANUAL,-dring_val,dring_val);
					break;
				}
			}
		}
		else if(pExMem->testPara[i].iStatus == 2)  //���������ͨ��
		{
			N = (long)(2000 / (pExMem->testPara[i].dFreq) * CYCLE_NUM_PER_SCREEN);  //��Ļ��ʾ�������㹫ʽ
			if(N >= 3 && N <= 10000)
			{
				SetCtrlAttribute(OppsitePanelHandle,OPPSITEPNL_STRIPCHART_3,ATTR_POINTS_PER_SCREEN,N);//ֻ���ýǶ����chart
				SetCtrlAttribute(OppsitePanelHandle,OPPSITEPNL_STRIPCHART_4,ATTR_POINTS_PER_SCREEN,N);
			}
			
			for(ringindex = 0;ringindex < 11;ringindex++)  //����ֵ������Χ������y����
			{
				GetValueFromIndex(OppsitePanelHandle,OPPSITEPNL_YRANGE_RING_3,ringindex,&dring_val);
				if(pExMem->testPara[i].dAmp < dring_val)
				{
					//SetCtrlIndex(OppsitePanelHandle,OPPSITEPNL_YRANGE_RING_3,ringindex);
					//SetCtrlIndex(OppsitePanelHandle,OPPSITEPNL_YRANGE_RING_4,ringindex);
					//SetAxisRange(OppsitePanelHandle,OPPSITEPNL_STRIPCHART_3,VAL_NO_CHANGE,0.0,1.0,VAL_MANUAL,-dring_val,dring_val);
					//SetAxisRange(OppsitePanelHandle,OPPSITEPNL_STRIPCHART_4,VAL_NO_CHANGE,0.0,1.0,VAL_MANUAL,-dring_val,dring_val);
					break;
				}
			}
		}
		else
		{}
		
		
	}
	
	
}

//��ʼ����ʾ����
void InitOppsiteDisIndex(void)
{
	char tempLabel[128];
	
	GetPanelHandleFromTabPage (OppsitePanelHandle, OPPSITEPNL_OPP_TAB,0,&tabpanel1);
	

		
	GetCtrlVal(tabpanel1,SET_TAB_ANGLE_CH_RING,&chart1_ch); 
	
	///////////////////////////////////////////////////////////
	
	if(chart1_ch==2||chart1_ch==4) 
	{
		SetCtrlAttribute(tabpanel1,SET_TAB_TEXTMSG_5_1,ATTR_VISIBLE,1);
		//SetCtrlAttribute(tabpanel1,SET_TAB_TEXTMSG_13_1,ATTR_VISIBLE,1);
		SetCtrlAttribute(tabpanel1,SET_TAB_TEXTMSG_13,ATTR_VISIBLE,0);
      	SetCtrlAttribute(tabpanel1,SET_TAB_TEXTMSG_5,ATTR_VISIBLE,0);
	}
	else
	{
		//SetCtrlAttribute(tabpanel1,SET_TAB_TEXTMSG_13_1,ATTR_VISIBLE,0);
		SetCtrlAttribute(tabpanel1,SET_TAB_TEXTMSG_13,ATTR_VISIBLE,1);
		SetCtrlAttribute(tabpanel1,SET_TAB_TEXTMSG_5_1,ATTR_VISIBLE,0); 
		SetCtrlAttribute(tabpanel1,SET_TAB_TEXTMSG_5,ATTR_VISIBLE,1); 
	}
	/////////////////////////////////////////////////////////////
	
	
	
	//GetCtrlVal(OppsitePanelHandle,OPPSITEPNL_CHART1_CH_RING,&chart1_ch);
	strcpy(tempLabel,""); 
	strcat(tempLabel,channelLabel[chart1_ch]);
	SetCtrlVal(OppsitePanelHandle,OPPSITEPNL_TEXTMSG_25,tempLabel);
	/*
	//��chart1��label��ǩ��ʾΪ��ring������һ��
	//GetCtrlVal(OppsitePanelHandle,OPPSITEPNL_CHART1_CH_RING,&chart1_ch); 
	GetCtrlVal(OppsitePanelHandle,OPPSITEPNL_CHART1_CONTENT_RING,&chart1_content);
	strcpy(tempLabel,""); 
	strcat(tempLabel,contentLabel[chart1_content]);
	SetCtrlAttribute(OppsitePanelHandle,OPPSITEPNL_STRIPCHART_1,ATTR_LABEL_TEXT,tempLabel);
	SetCtrlAttribute(OppsitePanelHandle,OPPSITEPNL_STRIPCHART_1,ATTR_LABEL_JUSTIFY,VAL_LEFT_JUSTIFIED);
	*/
	
	//��chart2��label��ǩ��ʾΪ��ring������һ��
	GetCtrlVal(OppsitePanelHandle,OPPSITEPNL_CHART2_CH_RING,&chart2_ch);
	GetCtrlVal(OppsitePanelHandle,OPPSITEPNL_CHART2_CONTENT_RING,&chart2_content);
	strcpy(tempLabel,""); 
	strcat(tempLabel,contentLabel[chart2_content]);
	SetCtrlAttribute(OppsitePanelHandle,OPPSITEPNL_STRIPCHART_2,ATTR_LABEL_TEXT,tempLabel);
	//SetCtrlAttribute(OppsitePanelHandle,OPPSITEPNL_STRIPCHART_2,ATTR_LABEL_JUSTIFY,VAL_LEFT_JUSTIFIED);
	
	
	GetCtrlVal(tabpanel1,SET_TAB_TORQUE_CH_RING,&chart3_ch);
	//GetCtrlVal(OppsitePanelHandle,OPPSITEPNL_CHART3_CH_RING,&chart3_ch);
	strcpy(tempLabel,""); 
	strcat(tempLabel,channelLabel[chart3_ch]);
	SetCtrlVal(OppsitePanelHandle,OPPSITEPNL_TEXTMSG_24,tempLabel);
	/*
	//��chart3��label��ǩ��ʾΪ��ring������һ��
	GetCtrlVal(OppsitePanelHandle,OPPSITEPNL_CHART3_CONTENT_RING,&chart3_content);
	strcpy(tempLabel,""); 
	strcat(tempLabel,contentLabel[chart3_content]);
	SetCtrlAttribute(OppsitePanelHandle,OPPSITEPNL_STRIPCHART_3,ATTR_LABEL_TEXT,tempLabel);
	SetCtrlAttribute(OppsitePanelHandle,OPPSITEPNL_STRIPCHART_3,ATTR_LABEL_JUSTIFY,VAL_LEFT_JUSTIFIED);
	*/
	
	//��chart4��label��ǩ��ʾΪ��ring������һ��
	GetCtrlVal(OppsitePanelHandle,OPPSITEPNL_CHART4_CH_RING,&chart4_ch);
	GetCtrlVal(OppsitePanelHandle,OPPSITEPNL_CHART4_CONTENT_RING,&chart4_content);
	strcpy(tempLabel,""); 
	strcat(tempLabel,contentLabel[chart4_content==5?chart4_content+1:chart4_content]);
	SetCtrlAttribute(OppsitePanelHandle,OPPSITEPNL_STRIPCHART_4,ATTR_LABEL_TEXT,tempLabel);
	//SetCtrlAttribute(OppsitePanelHandle,OPPSITEPNL_STRIPCHART_4,ATTR_LABEL_JUSTIFY,VAL_LEFT_JUSTIFIED); 
}

void Octrlparadown(void)
{
	double dTemp;
	//GetPanelHandleFromTabPage (MotionPanelHandle, MOTIONPNL_MOT_TAB,1,&tabpanel1);
			
	GetPanelHandleFromTabPage (OppsitePanelHandle, OPPSITEPNL_OPP_TAB,1,&tabpanel1);
			
				//ͨ��1�Ŀ��Ʋ���
				GetPanelHandleFromTabPage(tabpanel1, PARA_TAB_CTRL_PARA_TAB,0,&tabpanel2);
				GetCtrlVal(tabpanel2,CH1_P_TAB_NUM_OPPSITE_KAP,&dTemp);
				pExMem->ctrlPara[1].Kap = dTemp;
				GetCtrlVal(tabpanel2,CH1_P_TAB_NUM_OPPSITE_KAI,&dTemp);
				pExMem->ctrlPara[1].Kai = dTemp;
				GetCtrlVal(tabpanel2,CH1_P_TAB_NUM_OPPSITE_KAD,&dTemp);
				pExMem->ctrlPara[1].Kad = dTemp;
				GetCtrlVal(tabpanel2,CH1_P_TAB_NUM_OPPSITE_FA1,&dTemp);
				pExMem->ctrlPara[1].Fa1 = dTemp;
				GetCtrlVal(tabpanel2,CH1_P_TAB_NUM_OPPSITE_FA2,&dTemp);
				pExMem->ctrlPara[1].Fa2 = dTemp;
				GetCtrlVal(tabpanel2,CH1_P_TAB_NUM_OPPSITE_FAK,&dTemp);
				pExMem->ctrlPara[1].Fak = dTemp;
				
				GetCtrlVal(tabpanel2,CH1_P_TAB_NUM_OPPSITE_KTP,&dTemp);
				pExMem->ctrlPara[1].Ktp = dTemp;
				GetCtrlVal(tabpanel2,CH1_P_TAB_NUM_OPPSITE_KTI,&dTemp);
				pExMem->ctrlPara[1].Kti = dTemp;
				GetCtrlVal(tabpanel2,CH1_P_TAB_NUM_OPPSITE_KTD,&dTemp);
				pExMem->ctrlPara[1].Ktd = dTemp;
				
				GetCtrlVal(tabpanel2,CH1_P_TAB_NUM_OPPSITE_FT1,&dTemp);
				pExMem->ctrlPara[1].Ft1 = dTemp;
				GetCtrlVal(tabpanel2,CH1_P_TAB_NUM_OPPSITE_FT2,&dTemp);
				pExMem->ctrlPara[1].Ft2 = dTemp;
				GetCtrlVal(tabpanel2,CH1_P_TAB_NUM_OPPSITE_FTK,&dTemp);
				pExMem->ctrlPara[1].Ftk = dTemp;
				
				GetCtrlVal(tabpanel2,CH1_P_TAB_NUM_OPPSITE_KEP,&dTemp);
				pExMem->ctrlPara[1].Kep = dTemp;
				GetCtrlVal(tabpanel2,CH1_P_TAB_NUM_OPPSITE_KED,&dTemp);
				pExMem->ctrlPara[1].Ked = dTemp;
				GetCtrlVal(tabpanel2,CH1_P_TAB_NUM_OPPSITE_TED,&dTemp);
				pExMem->ctrlPara[1].Ted = dTemp;
				
				GetCtrlVal(tabpanel2,CH1_P_TAB_NUM_OPPSITE_T1,&dTemp);
				pExMem->ctrlPara[1].T1 = dTemp;
				GetCtrlVal(tabpanel2,CH1_P_TAB_NUM_OPPSITE_T2,&dTemp);
				pExMem->ctrlPara[1].T2 = dTemp;
				GetCtrlVal(tabpanel2,CH1_P_TAB_NUM_OPPSITE_X1,&dTemp);
				pExMem->ctrlPara[1].X1 = dTemp;
				GetCtrlVal(tabpanel2,CH1_P_TAB_NUM_OPPSITE_X2,&dTemp);
				pExMem->ctrlPara[1].X2 = dTemp;
				GetCtrlVal(tabpanel2,CH1_P_TAB_NUM_OPPSITE_X3,&dTemp);
				pExMem->ctrlPara[1].X3 = dTemp;

			
				//ͨ��2�Ŀ��Ʋ���
				GetPanelHandleFromTabPage(tabpanel1, PARA_TAB_CTRL_PARA_TAB,1,&tabpanel2);
				GetCtrlVal(tabpanel2,CH2_P_TAB_NUM_OPPSITE_KAP,&dTemp);
				pExMem->ctrlPara[2].Kap = dTemp;
				GetCtrlVal(tabpanel2,CH2_P_TAB_NUM_OPPSITE_KAI,&dTemp);
				pExMem->ctrlPara[2].Kai = dTemp;
				GetCtrlVal(tabpanel2,CH2_P_TAB_NUM_OPPSITE_KAD,&dTemp);
				pExMem->ctrlPara[2].Kad = dTemp;
				GetCtrlVal(tabpanel2,CH2_P_TAB_NUM_OPPSITE_FA1,&dTemp);
				pExMem->ctrlPara[2].Fa1 = dTemp;
				GetCtrlVal(tabpanel2,CH2_P_TAB_NUM_OPPSITE_FA2,&dTemp);
				pExMem->ctrlPara[2].Fa2 = dTemp;
				GetCtrlVal(tabpanel2,CH2_P_TAB_NUM_OPPSITE_FAK,&dTemp);
				pExMem->ctrlPara[2].Fak = dTemp;
				GetCtrlVal(tabpanel2,CH2_P_TAB_NUM_OPPSITE_KTP,&dTemp);
				pExMem->ctrlPara[2].Ktp = dTemp;
				GetCtrlVal(tabpanel2,CH2_P_TAB_NUM_OPPSITE_KTI,&dTemp);
				pExMem->ctrlPara[2].Kti = dTemp;
				GetCtrlVal(tabpanel2,CH2_P_TAB_NUM_OPPSITE_KTD,&dTemp);
				pExMem->ctrlPara[2].Ktd = dTemp;
				
				GetCtrlVal(tabpanel2,CH2_P_TAB_NUM_OPPSITE_FT1,&dTemp);
				pExMem->ctrlPara[2].Ft1 = dTemp;
				GetCtrlVal(tabpanel2,CH2_P_TAB_NUM_OPPSITE_FT2,&dTemp);
				pExMem->ctrlPara[2].Ft2 = dTemp;
				GetCtrlVal(tabpanel2,CH2_P_TAB_NUM_OPPSITE_FTK,&dTemp);
				pExMem->ctrlPara[2].Ftk = dTemp;
			
				GetCtrlVal(tabpanel2,CH2_P_TAB_NUM_OPPSITE_KEP,&dTemp);
				pExMem->ctrlPara[2].Kep = dTemp;
				GetCtrlVal(tabpanel2,CH2_P_TAB_NUM_OPPSITE_KED,&dTemp);
				pExMem->ctrlPara[2].Ked = dTemp;
				GetCtrlVal(tabpanel2,CH2_P_TAB_NUM_OPPSITE_TED,&dTemp);
				pExMem->ctrlPara[2].Ted = dTemp;
			
				GetCtrlVal(tabpanel2,CH2_P_TAB_NUM_OPPSITE_T1,&dTemp);
				pExMem->ctrlPara[2].T1 = dTemp;
				GetCtrlVal(tabpanel2,CH2_P_TAB_NUM_OPPSITE_T2,&dTemp);
				pExMem->ctrlPara[2].T2 = dTemp;
				GetCtrlVal(tabpanel2,CH2_P_TAB_NUM_OPPSITE_X1,&dTemp);
				pExMem->ctrlPara[2].X1 = dTemp;
				GetCtrlVal(tabpanel2,CH2_P_TAB_NUM_OPPSITE_X2,&dTemp);
				pExMem->ctrlPara[2].X2 = dTemp;
				GetCtrlVal(tabpanel2,CH2_P_TAB_NUM_OPPSITE_X3,&dTemp);
				pExMem->ctrlPara[2].X3 = dTemp;
				
				//ͨ��3�Ŀ��Ʋ���
				GetPanelHandleFromTabPage(tabpanel1, PARA_TAB_CTRL_PARA_TAB,2,&tabpanel2);
				GetCtrlVal(tabpanel2,CH3_P_TAB_NUM_OPPSITE_KAP,&dTemp);
				pExMem->ctrlPara[3].Kap = dTemp;
				GetCtrlVal(tabpanel2,CH3_P_TAB_NUM_OPPSITE_KAI,&dTemp);
				pExMem->ctrlPara[3].Kai = dTemp;
				GetCtrlVal(tabpanel2,CH3_P_TAB_NUM_OPPSITE_KAD,&dTemp);
				pExMem->ctrlPara[3].Kad = dTemp;
				GetCtrlVal(tabpanel2,CH3_P_TAB_NUM_OPPSITE_FA1,&dTemp);
				pExMem->ctrlPara[3].Fa1 = dTemp;
				GetCtrlVal(tabpanel2,CH3_P_TAB_NUM_OPPSITE_FA2,&dTemp);
				pExMem->ctrlPara[3].Fa2 = dTemp;
				GetCtrlVal(tabpanel2,CH3_P_TAB_NUM_OPPSITE_FAK,&dTemp);
				pExMem->ctrlPara[3].Fak = dTemp;
				GetCtrlVal(tabpanel2,CH3_P_TAB_NUM_OPPSITE_KTP,&dTemp);
				pExMem->ctrlPara[3].Ktp = dTemp;
				GetCtrlVal(tabpanel2,CH3_P_TAB_NUM_OPPSITE_KTI,&dTemp);
				pExMem->ctrlPara[3].Kti = dTemp;
				GetCtrlVal(tabpanel2,CH3_P_TAB_NUM_OPPSITE_KTD,&dTemp);
				pExMem->ctrlPara[3].Ktd = dTemp;
				
				GetCtrlVal(tabpanel2,CH3_P_TAB_NUM_OPPSITE_FT1,&dTemp);
				pExMem->ctrlPara[3].Ft1 = dTemp;
				GetCtrlVal(tabpanel2,CH3_P_TAB_NUM_OPPSITE_FT2,&dTemp);
				pExMem->ctrlPara[3].Ft2 = dTemp;
				GetCtrlVal(tabpanel2,CH3_P_TAB_NUM_OPPSITE_FTK,&dTemp);
				pExMem->ctrlPara[3].Ftk = dTemp;
				
				GetCtrlVal(tabpanel2,CH3_P_TAB_NUM_OPPSITE_KEP,&dTemp);
				pExMem->ctrlPara[3].Kep = dTemp;
				GetCtrlVal(tabpanel2,CH3_P_TAB_NUM_OPPSITE_KED,&dTemp);
				pExMem->ctrlPara[3].Ked = dTemp;
				GetCtrlVal(tabpanel2,CH3_P_TAB_NUM_OPPSITE_TED,&dTemp);
				pExMem->ctrlPara[3].Ted = dTemp;
				
				GetCtrlVal(tabpanel2,CH3_P_TAB_NUM_OPPSITE_T1,&dTemp);
				pExMem->ctrlPara[3].T1 = dTemp;
				GetCtrlVal(tabpanel2,CH3_P_TAB_NUM_OPPSITE_T2,&dTemp);
				pExMem->ctrlPara[3].T2 = dTemp;
				GetCtrlVal(tabpanel2,CH3_P_TAB_NUM_OPPSITE_X1,&dTemp);
				pExMem->ctrlPara[3].X1 = dTemp;
				GetCtrlVal(tabpanel2,CH3_P_TAB_NUM_OPPSITE_X2,&dTemp);
				pExMem->ctrlPara[3].X2 = dTemp;
				GetCtrlVal(tabpanel2,CH3_P_TAB_NUM_OPPSITE_X3,&dTemp);
				pExMem->ctrlPara[3].X3 = dTemp;
				
				//ͨ��4�Ŀ��Ʋ���
				GetPanelHandleFromTabPage(tabpanel1, PARA_TAB_CTRL_PARA_TAB,3,&tabpanel2);
				GetCtrlVal(tabpanel2,CH4_P_TAB_NUM_OPPSITE_KAP,&dTemp);
				pExMem->ctrlPara[4].Kap = dTemp;
				GetCtrlVal(tabpanel2,CH4_P_TAB_NUM_OPPSITE_KAI,&dTemp);
				pExMem->ctrlPara[4].Kai = dTemp;
				GetCtrlVal(tabpanel2,CH4_P_TAB_NUM_OPPSITE_KAD,&dTemp);
				pExMem->ctrlPara[4].Kad = dTemp;
				GetCtrlVal(tabpanel2,CH4_P_TAB_NUM_OPPSITE_FA1,&dTemp);
				pExMem->ctrlPara[4].Fa1 = dTemp;
				GetCtrlVal(tabpanel2,CH4_P_TAB_NUM_OPPSITE_FA2,&dTemp);
				pExMem->ctrlPara[4].Fa2 = dTemp;
				GetCtrlVal(tabpanel2,CH4_P_TAB_NUM_OPPSITE_FAK,&dTemp);
				pExMem->ctrlPara[4].Fak = dTemp;
				GetCtrlVal(tabpanel2,CH4_P_TAB_NUM_OPPSITE_KTP,&dTemp);
				pExMem->ctrlPara[4].Ktp = dTemp;
				GetCtrlVal(tabpanel2,CH4_P_TAB_NUM_OPPSITE_KTI,&dTemp);
				pExMem->ctrlPara[4].Kti = dTemp;
				GetCtrlVal(tabpanel2,CH4_P_TAB_NUM_OPPSITE_KTD,&dTemp);
				pExMem->ctrlPara[4].Ktd = dTemp;
				
				GetCtrlVal(tabpanel2,CH4_P_TAB_NUM_OPPSITE_FT1,&dTemp);
				pExMem->ctrlPara[4].Ft1 = dTemp;
				GetCtrlVal(tabpanel2,CH4_P_TAB_NUM_OPPSITE_FT2,&dTemp);
				pExMem->ctrlPara[4].Ft2 = dTemp;
				GetCtrlVal(tabpanel2,CH4_P_TAB_NUM_OPPSITE_FTK,&dTemp);
				pExMem->ctrlPara[4].Ftk = dTemp;
				
				GetCtrlVal(tabpanel2,CH4_P_TAB_NUM_OPPSITE_KEP,&dTemp);
				pExMem->ctrlPara[4].Kep = dTemp;
				GetCtrlVal(tabpanel2,CH4_P_TAB_NUM_OPPSITE_KED,&dTemp);
				pExMem->ctrlPara[4].Ked = dTemp;
				GetCtrlVal(tabpanel2,CH4_P_TAB_NUM_OPPSITE_TED,&dTemp);
				pExMem->ctrlPara[4].Ted = dTemp;
			
				GetCtrlVal(tabpanel2,CH4_P_TAB_NUM_OPPSITE_T1,&dTemp);
				pExMem->ctrlPara[4].T1 = dTemp;
				GetCtrlVal(tabpanel2,CH4_P_TAB_NUM_OPPSITE_T2,&dTemp);
				pExMem->ctrlPara[4].T2 = dTemp;
				GetCtrlVal(tabpanel2,CH4_P_TAB_NUM_OPPSITE_X1,&dTemp);
				pExMem->ctrlPara[4].X1 = dTemp;
				GetCtrlVal(tabpanel2,CH4_P_TAB_NUM_OPPSITE_X2,&dTemp);
				pExMem->ctrlPara[4].X2 = dTemp;
				GetCtrlVal(tabpanel2,CH4_P_TAB_NUM_OPPSITE_X3,&dTemp);
				pExMem->ctrlPara[4].X3 = dTemp;  
}

//��ʼ�����Ʋ����ؼ���ֵ   
void InitOppsiteCtrlSet(void)
{
	
	GetPanelHandleFromTabPage (OppsitePanelHandle, OPPSITEPNL_OPP_TAB,1,&tabpanel1);
			
	//ͨ��1�Ŀ��Ʋ���
	GetPanelHandleFromTabPage(tabpanel1, PARA_TAB_CTRL_PARA_TAB,0,&tabpanel2);
	SetCtrlVal(tabpanel2,CH1_P_TAB_NUM_OPPSITE_KAP,pExMem->ctrlPara[1].Kap);
	SetCtrlVal(tabpanel2,CH1_P_TAB_NUM_OPPSITE_KAI,pExMem->ctrlPara[1].Kai);
	SetCtrlVal(tabpanel2,CH1_P_TAB_NUM_OPPSITE_KAD,pExMem->ctrlPara[1].Kad);
	SetCtrlVal(tabpanel2,CH1_P_TAB_NUM_OPPSITE_FA1,pExMem->ctrlPara[1].Fa1);
	SetCtrlVal(tabpanel2,CH1_P_TAB_NUM_OPPSITE_FA2,pExMem->ctrlPara[1].Fa2);
	SetCtrlVal(tabpanel2,CH1_P_TAB_NUM_OPPSITE_FAK,pExMem->ctrlPara[1].Fak);
	SetCtrlVal(tabpanel2,CH1_P_TAB_NUM_OPPSITE_KTP,pExMem->ctrlPara[1].Ktp);
	SetCtrlVal(tabpanel2,CH1_P_TAB_NUM_OPPSITE_KTI,pExMem->ctrlPara[1].Kti);
	SetCtrlVal(tabpanel2,CH1_P_TAB_NUM_OPPSITE_KTD,pExMem->ctrlPara[1].Ktd);
	SetCtrlVal(tabpanel2,CH1_P_TAB_NUM_OPPSITE_FT1,pExMem->ctrlPara[1].Ft1);
	SetCtrlVal(tabpanel2,CH1_P_TAB_NUM_OPPSITE_FT2,pExMem->ctrlPara[1].Ft2);
	SetCtrlVal(tabpanel2,CH1_P_TAB_NUM_OPPSITE_FTK,pExMem->ctrlPara[1].Ftk);
	SetCtrlVal(tabpanel2,CH1_P_TAB_NUM_OPPSITE_KEP,pExMem->ctrlPara[1].Kep);
	SetCtrlVal(tabpanel2,CH1_P_TAB_NUM_OPPSITE_KED,pExMem->ctrlPara[1].Ked);
	SetCtrlVal(tabpanel2,CH1_P_TAB_NUM_OPPSITE_TED,pExMem->ctrlPara[1].Ted);
	SetCtrlVal(tabpanel2,CH1_P_TAB_NUM_OPPSITE_T1,pExMem->ctrlPara[1].T1);
	SetCtrlVal(tabpanel2,CH1_P_TAB_NUM_OPPSITE_T2,pExMem->ctrlPara[1].T2);
	SetCtrlVal(tabpanel2,CH1_P_TAB_NUM_OPPSITE_X1,pExMem->ctrlPara[1].X1);
	SetCtrlVal(tabpanel2,CH1_P_TAB_NUM_OPPSITE_X2,pExMem->ctrlPara[1].X2);
	SetCtrlVal(tabpanel2,CH1_P_TAB_NUM_OPPSITE_X3,pExMem->ctrlPara[1].X3);

	//ͨ��2�Ŀ��Ʋ���
	GetPanelHandleFromTabPage(tabpanel1, PARA_TAB_CTRL_PARA_TAB,1,&tabpanel2);
	SetCtrlVal(tabpanel2,CH2_P_TAB_NUM_OPPSITE_KAP,pExMem->ctrlPara[2].Kap);
	SetCtrlVal(tabpanel2,CH2_P_TAB_NUM_OPPSITE_KAI,pExMem->ctrlPara[2].Kai);
	SetCtrlVal(tabpanel2,CH2_P_TAB_NUM_OPPSITE_KAD,pExMem->ctrlPara[2].Kad);
	SetCtrlVal(tabpanel2,CH2_P_TAB_NUM_OPPSITE_FA1,pExMem->ctrlPara[2].Fa1);
	SetCtrlVal(tabpanel2,CH2_P_TAB_NUM_OPPSITE_FA2,pExMem->ctrlPara[2].Fa2);
	SetCtrlVal(tabpanel2,CH2_P_TAB_NUM_OPPSITE_FAK,pExMem->ctrlPara[2].Fak);
	SetCtrlVal(tabpanel2,CH2_P_TAB_NUM_OPPSITE_KTP,pExMem->ctrlPara[2].Ktp);
	SetCtrlVal(tabpanel2,CH2_P_TAB_NUM_OPPSITE_KTI,pExMem->ctrlPara[2].Kti);
	SetCtrlVal(tabpanel2,CH2_P_TAB_NUM_OPPSITE_KTD,pExMem->ctrlPara[2].Ktd);
	SetCtrlVal(tabpanel2,CH2_P_TAB_NUM_OPPSITE_FT1,pExMem->ctrlPara[2].Ft1);
	SetCtrlVal(tabpanel2,CH2_P_TAB_NUM_OPPSITE_FT2,pExMem->ctrlPara[2].Ft2);
	SetCtrlVal(tabpanel2,CH2_P_TAB_NUM_OPPSITE_FTK,pExMem->ctrlPara[2].Ftk);
	SetCtrlVal(tabpanel2,CH2_P_TAB_NUM_OPPSITE_KEP,pExMem->ctrlPara[2].Kep);
	SetCtrlVal(tabpanel2,CH2_P_TAB_NUM_OPPSITE_KED,pExMem->ctrlPara[2].Ked);
	SetCtrlVal(tabpanel2,CH2_P_TAB_NUM_OPPSITE_TED,pExMem->ctrlPara[2].Ted);
	SetCtrlVal(tabpanel2,CH2_P_TAB_NUM_OPPSITE_T1,pExMem->ctrlPara[2].T1);
	SetCtrlVal(tabpanel2,CH2_P_TAB_NUM_OPPSITE_T2,pExMem->ctrlPara[2].T2);
	SetCtrlVal(tabpanel2,CH2_P_TAB_NUM_OPPSITE_X1,pExMem->ctrlPara[2].X1);
	SetCtrlVal(tabpanel2,CH2_P_TAB_NUM_OPPSITE_X2,pExMem->ctrlPara[2].X2);
	SetCtrlVal(tabpanel2,CH2_P_TAB_NUM_OPPSITE_X3,pExMem->ctrlPara[2].X3);

	//ͨ��3�Ŀ��Ʋ���
	GetPanelHandleFromTabPage(tabpanel1, PARA_TAB_CTRL_PARA_TAB,2,&tabpanel2);
	SetCtrlVal(tabpanel2,CH3_P_TAB_NUM_OPPSITE_KAP,pExMem->ctrlPara[3].Kap);
	SetCtrlVal(tabpanel2,CH3_P_TAB_NUM_OPPSITE_KAI,pExMem->ctrlPara[3].Kai);
	SetCtrlVal(tabpanel2,CH3_P_TAB_NUM_OPPSITE_KAD,pExMem->ctrlPara[3].Kad);
	SetCtrlVal(tabpanel2,CH3_P_TAB_NUM_OPPSITE_FA1,pExMem->ctrlPara[3].Fa1);
	SetCtrlVal(tabpanel2,CH3_P_TAB_NUM_OPPSITE_FA2,pExMem->ctrlPara[3].Fa2);
	SetCtrlVal(tabpanel2,CH3_P_TAB_NUM_OPPSITE_FAK,pExMem->ctrlPara[3].Fak);
	SetCtrlVal(tabpanel2,CH3_P_TAB_NUM_OPPSITE_KTP,pExMem->ctrlPara[3].Ktp);
	SetCtrlVal(tabpanel2,CH3_P_TAB_NUM_OPPSITE_KTI,pExMem->ctrlPara[3].Kti);
	SetCtrlVal(tabpanel2,CH3_P_TAB_NUM_OPPSITE_KTD,pExMem->ctrlPara[3].Ktd);
	SetCtrlVal(tabpanel2,CH3_P_TAB_NUM_OPPSITE_FT1,pExMem->ctrlPara[3].Ft1);
	SetCtrlVal(tabpanel2,CH3_P_TAB_NUM_OPPSITE_FT2,pExMem->ctrlPara[3].Ft2);
	SetCtrlVal(tabpanel2,CH3_P_TAB_NUM_OPPSITE_FTK,pExMem->ctrlPara[3].Ftk);
	SetCtrlVal(tabpanel2,CH3_P_TAB_NUM_OPPSITE_KEP,pExMem->ctrlPara[3].Kep);
	SetCtrlVal(tabpanel2,CH3_P_TAB_NUM_OPPSITE_KED,pExMem->ctrlPara[3].Ked);
	SetCtrlVal(tabpanel2,CH3_P_TAB_NUM_OPPSITE_TED,pExMem->ctrlPara[3].Ted);
	SetCtrlVal(tabpanel2,CH3_P_TAB_NUM_OPPSITE_T1,pExMem->ctrlPara[3].T1);
	SetCtrlVal(tabpanel2,CH3_P_TAB_NUM_OPPSITE_T2,pExMem->ctrlPara[3].T2);
	SetCtrlVal(tabpanel2,CH3_P_TAB_NUM_OPPSITE_X1,pExMem->ctrlPara[3].X1);
	SetCtrlVal(tabpanel2,CH3_P_TAB_NUM_OPPSITE_X2,pExMem->ctrlPara[3].X2);
	SetCtrlVal(tabpanel2,CH3_P_TAB_NUM_OPPSITE_X3,pExMem->ctrlPara[3].X3);

	//ͨ��4�Ŀ��Ʋ���
	GetPanelHandleFromTabPage(tabpanel1, PARA_TAB_CTRL_PARA_TAB,3,&tabpanel2);
	SetCtrlVal(tabpanel2,CH4_P_TAB_NUM_OPPSITE_KAP,pExMem->ctrlPara[4].Kap);
	SetCtrlVal(tabpanel2,CH4_P_TAB_NUM_OPPSITE_KAI,pExMem->ctrlPara[4].Kai);
	SetCtrlVal(tabpanel2,CH4_P_TAB_NUM_OPPSITE_KAD,pExMem->ctrlPara[4].Kad);
	SetCtrlVal(tabpanel2,CH4_P_TAB_NUM_OPPSITE_FA1,pExMem->ctrlPara[4].Fa1);
	SetCtrlVal(tabpanel2,CH4_P_TAB_NUM_OPPSITE_FA2,pExMem->ctrlPara[4].Fa2);
	SetCtrlVal(tabpanel2,CH4_P_TAB_NUM_OPPSITE_FAK,pExMem->ctrlPara[4].Fak);
	SetCtrlVal(tabpanel2,CH4_P_TAB_NUM_OPPSITE_KTP,pExMem->ctrlPara[4].Ktp);
	SetCtrlVal(tabpanel2,CH4_P_TAB_NUM_OPPSITE_KTI,pExMem->ctrlPara[4].Kti);
	SetCtrlVal(tabpanel2,CH4_P_TAB_NUM_OPPSITE_KTD,pExMem->ctrlPara[4].Ktd);
	SetCtrlVal(tabpanel2,CH4_P_TAB_NUM_OPPSITE_FT1,pExMem->ctrlPara[4].Ft1);
	SetCtrlVal(tabpanel2,CH4_P_TAB_NUM_OPPSITE_FT2,pExMem->ctrlPara[4].Ft2);
	SetCtrlVal(tabpanel2,CH4_P_TAB_NUM_OPPSITE_FTK,pExMem->ctrlPara[4].Ftk);
	SetCtrlVal(tabpanel2,CH4_P_TAB_NUM_OPPSITE_KEP,pExMem->ctrlPara[4].Kep);
	SetCtrlVal(tabpanel2,CH4_P_TAB_NUM_OPPSITE_KED,pExMem->ctrlPara[4].Ked);
	SetCtrlVal(tabpanel2,CH4_P_TAB_NUM_OPPSITE_TED,pExMem->ctrlPara[4].Ted);
	SetCtrlVal(tabpanel2,CH4_P_TAB_NUM_OPPSITE_T1,pExMem->ctrlPara[4].T1);
	SetCtrlVal(tabpanel2,CH4_P_TAB_NUM_OPPSITE_T2,pExMem->ctrlPara[4].T2);
	SetCtrlVal(tabpanel2,CH4_P_TAB_NUM_OPPSITE_X1,pExMem->ctrlPara[4].X1);
	SetCtrlVal(tabpanel2,CH4_P_TAB_NUM_OPPSITE_X2,pExMem->ctrlPara[4].X2);
	SetCtrlVal(tabpanel2,CH4_P_TAB_NUM_OPPSITE_X3,pExMem->ctrlPara[4].X3);
}

//��ʼ�����Ʋ���
void InitOppsiteCtrlPara(void)
{
	double dTemp;
	//��ȡini�ļ����
	inihandle = Ini_New(0); //This function creates an object which can contain an in-memory list of tag/value pairs within sections.
	//��ȡini�ļ�
	//GetWindowsDirs (windowsDirectory, systemDirectory);
	//strcpy( pathname ,windowsDirectory);  
	
	GetDir(currentDir);
	strcpy(pathname,currentDir);
	strcat(pathname,"\\OppsitePara.ini");
	
	inierr = Ini_ReadFromFile(inihandle, pathname); 
	if( inierr < 0)
	{
		//sprintf(msgBuff,"�򿪲����ļ�����err = %d",inierr);
		//MessagePopup ("Err",msgBuff);  
		//���ļ�ʧ��
	}
	else
	{
		//ͨ��1�Ƕȿ��Ʋ���
		if(Ini_GetDouble (inihandle, "CtrlPara","CH1_Kap", &dTemp)>0)
		{
			pExMem->ctrlPara[1].Kap = dTemp;
		}
		else 
		{
			 pExMem->ctrlPara[1].Kap = 0.0;
		}
	
		if(Ini_GetDouble (inihandle, "CtrlPara","CH1_Kai", &dTemp)>0)
		{
			pExMem->ctrlPara[1].Kai = dTemp;
		}
		else 
		{
			 pExMem->ctrlPara[1].Kai = 0.0;
		}
	
		if(Ini_GetDouble (inihandle, "CtrlPara","CH1_Kad", &dTemp)>0)
		{
			pExMem->ctrlPara[1].Kad = dTemp;
		}
		else 
		{
			 pExMem->ctrlPara[1].Kad = 0.0;
		}
		//�Ƕȿ���ģ������
		if(Ini_GetDouble (inihandle, "CtrlPara","CH1_Fa1", &dTemp)>0)
		{
			pExMem->ctrlPara[1].Fa1 = dTemp;
		}
		else 
		{
			 pExMem->ctrlPara[1].Fa1 = 0.0;
		}
	
		if(Ini_GetDouble (inihandle, "CtrlPara","CH1_Fa2", &dTemp)>0)
		{
			pExMem->ctrlPara[1].Fa2 = dTemp;
		}
		else 
		{
			 pExMem->ctrlPara[1].Fa2 = 0.0;
		}
	
		if(Ini_GetDouble (inihandle, "CtrlPara","CH1_Fak", &dTemp)>0)
		{
			pExMem->ctrlPara[1].Fak = dTemp;
		}
		else 
		{
			 pExMem->ctrlPara[1].Fak = 0.0;
		}
		//ͨ��1���ؿ���PID����
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
		//ͨ��1����ģ������
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
		//ͨ��1�Զ�����
		if(Ini_GetDouble (inihandle, "CtrlPara","CH1_Kep", &dTemp)>0)
		{
			pExMem->ctrlPara[1].Kep = dTemp;
		}
		else 
		{
			 pExMem->ctrlPara[1].Kep = 0.0;
		}
	
		if(Ini_GetDouble (inihandle, "CtrlPara","CH1_Ked", &dTemp)>0)
		{
			pExMem->ctrlPara[1].Ked = dTemp;
		}
		else 
		{
			 pExMem->ctrlPara[1].Ked = 0.0;
		}
	
		if(Ini_GetDouble (inihandle, "CtrlPara","CH1_Ted", &dTemp)>0)
		{
			pExMem->ctrlPara[1].Ted = dTemp;
		}
		else 
		{
			 pExMem->ctrlPara[1].Ted = 0.0;
		}
		//ͨ��1��չ����
		if(Ini_GetDouble (inihandle, "CtrlPara","CH1_T1", &dTemp)>0)
		{
			pExMem->ctrlPara[1].T1 = dTemp;
		}
		else 
		{
			 pExMem->ctrlPara[1].T1 = 999.0;
		}
	
		if(Ini_GetDouble (inihandle, "CtrlPara","CH1_T2", &dTemp)>0)
		{
			pExMem->ctrlPara[1].T2 = dTemp;
		}
		else 
		{
			 pExMem->ctrlPara[1].T2 = 9999.0;
		}
	
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
		
		
		//ͨ��2 �Ƕ�PID����
		if(Ini_GetDouble (inihandle, "CtrlPara","CH2_Kap", &dTemp)>0)
		{
			pExMem->ctrlPara[2].Kap = dTemp;
		}
		else 
		{
			 pExMem->ctrlPara[2].Kap = 0.0;
		}
	
		if(Ini_GetDouble (inihandle, "CtrlPara","CH2_Kai", &dTemp)>0)
		{
			pExMem->ctrlPara[2].Kai = dTemp;
		}
		else 
		{
			 pExMem->ctrlPara[2].Kai = 0;
		}
	
		if(Ini_GetDouble (inihandle, "CtrlPara","CH2_Kad", &dTemp)>0)
		{
			pExMem->ctrlPara[2].Kad = dTemp;
		}
		else 
		{
			 pExMem->ctrlPara[2].Kad = 0.0;
		}
		//ͨ��2�Ƕ�ģ������
		if(Ini_GetDouble (inihandle, "CtrlPara","CH2_Fa1", &dTemp)>0)
		{
			pExMem->ctrlPara[2].Fa1 = dTemp;
		}
		else 
		{
			 pExMem->ctrlPara[2].Fa1 = 0.0;
		}
	
		if(Ini_GetDouble (inihandle, "CtrlPara","CH2_Fa2", &dTemp)>0)
		{
			pExMem->ctrlPara[2].Fa2 = dTemp;
		}
		else 
		{
			 pExMem->ctrlPara[2].Fa2 = 0.0;
		}
	
		if(Ini_GetDouble (inihandle, "CtrlPara","CH2_Fak", &dTemp)>0)
		{
			pExMem->ctrlPara[2].Fak = dTemp;
		}
		else 
		{
			 pExMem->ctrlPara[2].Fak = 0.0;
		}
		//ͨ��2���ؿ���PID����
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
			 pExMem->ctrlPara[2].Kti = 0.0;
		}
	
		if(Ini_GetDouble (inihandle, "CtrlPara","CH2_Ktd", &dTemp)>0)
		{
			pExMem->ctrlPara[2].Ktd = dTemp;
		}
		else 
		{
			 pExMem->ctrlPara[2].Ktd = 0.0;
		}
		//ͨ��2����ģ������
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
		//ͨ��2�Զ�����
		if(Ini_GetDouble (inihandle, "CtrlPara","CH2_Kep", &dTemp)>0)
		{
			pExMem->ctrlPara[2].Kep = dTemp;
		}
		else 
		{
			 pExMem->ctrlPara[2].Kep = 0.0;
		}
	
		if(Ini_GetDouble (inihandle, "CtrlPara","CH2_Ked", &dTemp)>0)
		{
			pExMem->ctrlPara[2].Ked = dTemp;
		}
		else 
		{
			 pExMem->ctrlPara[2].Ked = 0.0;
		}
	
		if(Ini_GetDouble (inihandle, "CtrlPara","CH2_Ted", &dTemp)>0)
		{
			pExMem->ctrlPara[2].Ted = dTemp;
		}
		else 
		{
			 pExMem->ctrlPara[2].Ted = 0.0;
		}
		//ͨ��2��չ����
		if(Ini_GetDouble (inihandle, "CtrlPara","CH2_T1", &dTemp)>0)
		{
			pExMem->ctrlPara[2].T1 = dTemp;
		}
		else 
		{
			 pExMem->ctrlPara[2].T1 = 999.0;
		}
	
		if(Ini_GetDouble (inihandle, "CtrlPara","CH2_T2", &dTemp)>0)
		{
			pExMem->ctrlPara[2].T2 = dTemp;
		}
		else 
		{
			 pExMem->ctrlPara[2].T2 = 9999.0;
		}
	
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
		if(Ini_GetDouble (inihandle, "CtrlPara","CH3_Kap", &dTemp)>0)
		{
			pExMem->ctrlPara[3].Kap = dTemp;
		}
		else 
		{
			 pExMem->ctrlPara[3].Kap = 0.0;
		}
	
		if(Ini_GetDouble (inihandle, "CtrlPara","CH3_Kai", &dTemp)>0)
		{
			pExMem->ctrlPara[3].Kai = dTemp;
		}
		else 
		{
			 pExMem->ctrlPara[3].Kai = 0;
		}
	
		if(Ini_GetDouble (inihandle, "CtrlPara","CH3_Kad", &dTemp)>0)
		{
			pExMem->ctrlPara[3].Kad = dTemp;
		}
		else 
		{
			 pExMem->ctrlPara[3].Kad = 0.0;
		}
		//ͨ��3�Ƕ�ģ������
		if(Ini_GetDouble (inihandle, "CtrlPara","CH3_Fa1", &dTemp)>0)
		{
			pExMem->ctrlPara[3].Fa1 = dTemp;
		}
		else 
		{
			 pExMem->ctrlPara[3].Fa1 = 0.0;
		}
	
		if(Ini_GetDouble (inihandle, "CtrlPara","CH3_Fa2", &dTemp)>0)
		{
			pExMem->ctrlPara[3].Fa2 = dTemp;
		}
		else 
		{
			 pExMem->ctrlPara[3].Fa2 = 0.0;
		}
	
		if(Ini_GetDouble (inihandle, "CtrlPara","CH3_Fak", &dTemp)>0)
		{
			pExMem->ctrlPara[3].Fak = dTemp;
		}
		else 
		{
			 pExMem->ctrlPara[3].Fak = 0.0;
		}
		//ͨ��3���ؿ���PID����
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
			 pExMem->ctrlPara[3].Kti = 0.0;
		}
	
		if(Ini_GetDouble (inihandle, "CtrlPara","CH3_Ktd", &dTemp)>0)
		{
			pExMem->ctrlPara[3].Ktd = dTemp;
		}
		else 
		{
			 pExMem->ctrlPara[3].Ktd = 0.0;
		}
		//ͨ��3����ģ������
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
		//ͨ��3�Զ�����
		if(Ini_GetDouble (inihandle, "CtrlPara","CH3_Kep", &dTemp)>0)
		{
			pExMem->ctrlPara[3].Kep = dTemp;
		}
		else 
		{
			 pExMem->ctrlPara[3].Kep = 0.0;
		}
	
		if(Ini_GetDouble (inihandle, "CtrlPara","CH3_Ked", &dTemp)>0)
		{
			pExMem->ctrlPara[3].Ked = dTemp;
		}
		else 
		{
			 pExMem->ctrlPara[3].Ked = 0.0;
		}
	
		if(Ini_GetDouble (inihandle, "CtrlPara","CH3_Ted", &dTemp)>0)
		{
			pExMem->ctrlPara[3].Ted = dTemp;
		}
		else 
		{
			 pExMem->ctrlPara[3].Ted = 0.0;
		}
		//ͨ��1��չ����
		if(Ini_GetDouble (inihandle, "CtrlPara","CH3_T1", &dTemp)>0)
		{
			pExMem->ctrlPara[3].T1 = dTemp;
		}
		else 
		{
			 pExMem->ctrlPara[3].T1 = 999.0;
		}
	
		if(Ini_GetDouble (inihandle, "CtrlPara","CH3_T2", &dTemp)>0)
		{
			pExMem->ctrlPara[3].T2 = dTemp;
		}
		else 
		{
			 pExMem->ctrlPara[3].T2 = 9999.0;
		}
	
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
		
		//ͨ��4 �Ƕȿ���PID����
		if(Ini_GetDouble (inihandle, "CtrlPara","CH4_Kap", &dTemp)>0)
		{
			pExMem->ctrlPara[4].Kap = dTemp;
		}
		else 
		{
			 pExMem->ctrlPara[4].Kap = 0.0;
		}
	
		if(Ini_GetDouble (inihandle, "CtrlPara","CH4_Kai", &dTemp)>0)
		{
			pExMem->ctrlPara[4].Kai = 0;
		}
		else 
		{
			 pExMem->ctrlPara[4].Kai = 0.0;
		}
	
		if(Ini_GetDouble (inihandle, "CtrlPara","CH4_Kad", &dTemp)>0)
		{
			pExMem->ctrlPara[4].Kad = dTemp;
		}
		else 
		{
			 pExMem->ctrlPara[4].Kad = 0.0;
		}
		//ͨ��4�Ƕȿ���ģ������
		if(Ini_GetDouble (inihandle, "CtrlPara","CH4_Fa1", &dTemp)>0)
		{
			pExMem->ctrlPara[4].Fa1 = dTemp;
		}
		else 
		{
			 pExMem->ctrlPara[4].Fa1 = 0.0;
		}
	
		if(Ini_GetDouble (inihandle, "CtrlPara","CH4_Fa2", &dTemp)>0)
		{
			pExMem->ctrlPara[4].Fa2 = dTemp;
		}
		else 
		{
			 pExMem->ctrlPara[4].Fa2 = 0.0;
		}
	
		if(Ini_GetDouble (inihandle, "CtrlPara","CH4_Fak", &dTemp)>0)
		{
			pExMem->ctrlPara[4].Fak = dTemp;
		}
		else 
		{
			 pExMem->ctrlPara[4].Fak = 0.0;
		}
		//ͨ��4���ؿ���PID����
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
		//ͨ��4����ģ������
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
		//ͨ��4�Զ�����
		if(Ini_GetDouble (inihandle, "CtrlPara","CH4_Kep", &dTemp)>0)
		{
			pExMem->ctrlPara[4].Kep = dTemp;
		}
		else 
		{
			 pExMem->ctrlPara[4].Kep = 0.0;
		}
	
		if(Ini_GetDouble (inihandle, "CtrlPara","CH4_Ked", &dTemp)>0)
		{
			pExMem->ctrlPara[4].Ked = dTemp;
		}
		else 
		{
			 pExMem->ctrlPara[4].Ked = 0.0;
		}
	
		if(Ini_GetDouble (inihandle, "CtrlPara","CH4_Ted", &dTemp)>0)
		{
			pExMem->ctrlPara[4].Ted = dTemp;
		}
		else 
		{
			 pExMem->ctrlPara[4].Ted = 0.0;
		}
		//ͨ��4��չ����
		if(Ini_GetDouble (inihandle, "CtrlPara","CH4_T1", &dTemp)>0)
		{
			pExMem->ctrlPara[4].T1 = dTemp;
		}
		else 
		{
			 pExMem->ctrlPara[4].T1 = 999.0;
		}
	
		if(Ini_GetDouble (inihandle, "CtrlPara","CH4_T2", &dTemp)>0)
		{
			pExMem->ctrlPara[4].T2 = dTemp;
		}
		else 
		{
			 pExMem->ctrlPara[4].T2 = 9999.0;
		}
	
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
	
	InitOppsiteCtrlSet();
}

//ʹ�ܿ��Ʋ�������
void LightOppositeParaSet(void)
{
	GetPanelHandleFromTabPage (OppsitePanelHandle, OPPSITEPNL_OPP_TAB,1,&tabpanel1);
			

	GetPanelHandleFromTabPage(tabpanel1, PARA_TAB_CTRL_PARA_TAB,0,&tabpanel2);
	SetCtrlAttribute(tabpanel2,CH1_P_TAB_NUM_OPPSITE_KAP,ATTR_DIMMED,0);
	SetCtrlAttribute(tabpanel2,CH1_P_TAB_NUM_OPPSITE_KAI,ATTR_DIMMED,0);
	SetCtrlAttribute(tabpanel2,CH1_P_TAB_NUM_OPPSITE_KAD,ATTR_DIMMED,0);
	SetCtrlAttribute(tabpanel2,CH1_P_TAB_NUM_OPPSITE_FA1,ATTR_DIMMED,0);
	SetCtrlAttribute(tabpanel2,CH1_P_TAB_NUM_OPPSITE_FA2,ATTR_DIMMED,0);
	SetCtrlAttribute(tabpanel2,CH1_P_TAB_NUM_OPPSITE_FAK,ATTR_DIMMED,0);
	SetCtrlAttribute(tabpanel2,CH1_P_TAB_NUM_OPPSITE_KTP,ATTR_DIMMED,0);
	SetCtrlAttribute(tabpanel2,CH1_P_TAB_NUM_OPPSITE_KTI,ATTR_DIMMED,0);
	SetCtrlAttribute(tabpanel2,CH1_P_TAB_NUM_OPPSITE_KTD,ATTR_DIMMED,0);
	SetCtrlAttribute(tabpanel2,CH1_P_TAB_NUM_OPPSITE_FT1,ATTR_DIMMED,0);
	SetCtrlAttribute(tabpanel2,CH1_P_TAB_NUM_OPPSITE_FT2,ATTR_DIMMED,0);
	SetCtrlAttribute(tabpanel2,CH1_P_TAB_NUM_OPPSITE_FTK,ATTR_DIMMED,0);
	SetCtrlAttribute(tabpanel2,CH1_P_TAB_NUM_OPPSITE_KEP,ATTR_DIMMED,0);
	SetCtrlAttribute(tabpanel2,CH1_P_TAB_NUM_OPPSITE_KED,ATTR_DIMMED,0);
	SetCtrlAttribute(tabpanel2,CH1_P_TAB_NUM_OPPSITE_TED,ATTR_DIMMED,0);
	SetCtrlAttribute(tabpanel2,CH1_P_TAB_NUM_OPPSITE_T1, ATTR_DIMMED,0);
	SetCtrlAttribute(tabpanel2,CH1_P_TAB_NUM_OPPSITE_T2, ATTR_DIMMED,0);
	SetCtrlAttribute(tabpanel2,CH1_P_TAB_NUM_OPPSITE_X1, ATTR_DIMMED,0);
	SetCtrlAttribute(tabpanel2,CH1_P_TAB_NUM_OPPSITE_X2, ATTR_DIMMED,0);
	SetCtrlAttribute(tabpanel2,CH1_P_TAB_NUM_OPPSITE_X3, ATTR_DIMMED,0);
	
	GetPanelHandleFromTabPage(tabpanel1, PARA_TAB_CTRL_PARA_TAB,1,&tabpanel2);
	SetCtrlAttribute(tabpanel2,CH2_P_TAB_NUM_OPPSITE_KAP,ATTR_DIMMED,0);
	SetCtrlAttribute(tabpanel2,CH2_P_TAB_NUM_OPPSITE_KAI,ATTR_DIMMED,0);
	SetCtrlAttribute(tabpanel2,CH2_P_TAB_NUM_OPPSITE_KAD,ATTR_DIMMED,0);
	SetCtrlAttribute(tabpanel2,CH2_P_TAB_NUM_OPPSITE_FA1,ATTR_DIMMED,0);
	SetCtrlAttribute(tabpanel2,CH2_P_TAB_NUM_OPPSITE_FA2,ATTR_DIMMED,0);
	SetCtrlAttribute(tabpanel2,CH2_P_TAB_NUM_OPPSITE_FAK,ATTR_DIMMED,0);
	SetCtrlAttribute(tabpanel2,CH2_P_TAB_NUM_OPPSITE_KTP,ATTR_DIMMED,0);
	SetCtrlAttribute(tabpanel2,CH2_P_TAB_NUM_OPPSITE_KTI,ATTR_DIMMED,0);
	SetCtrlAttribute(tabpanel2,CH2_P_TAB_NUM_OPPSITE_KTD,ATTR_DIMMED,0);
	SetCtrlAttribute(tabpanel2,CH2_P_TAB_NUM_OPPSITE_FT1,ATTR_DIMMED,0);
	SetCtrlAttribute(tabpanel2,CH2_P_TAB_NUM_OPPSITE_FT2,ATTR_DIMMED,0);
	SetCtrlAttribute(tabpanel2,CH2_P_TAB_NUM_OPPSITE_FTK,ATTR_DIMMED,0);
	SetCtrlAttribute(tabpanel2,CH2_P_TAB_NUM_OPPSITE_KEP,ATTR_DIMMED,0);
	SetCtrlAttribute(tabpanel2,CH2_P_TAB_NUM_OPPSITE_KED,ATTR_DIMMED,0);
	SetCtrlAttribute(tabpanel2,CH2_P_TAB_NUM_OPPSITE_TED,ATTR_DIMMED,0);
	SetCtrlAttribute(tabpanel2,CH2_P_TAB_NUM_OPPSITE_T1, ATTR_DIMMED,0);
	SetCtrlAttribute(tabpanel2,CH2_P_TAB_NUM_OPPSITE_T2, ATTR_DIMMED,0);
	SetCtrlAttribute(tabpanel2,CH2_P_TAB_NUM_OPPSITE_X1, ATTR_DIMMED,0);
	SetCtrlAttribute(tabpanel2,CH2_P_TAB_NUM_OPPSITE_X2, ATTR_DIMMED,0);
	SetCtrlAttribute(tabpanel2,CH2_P_TAB_NUM_OPPSITE_X3, ATTR_DIMMED,0);
	
	GetPanelHandleFromTabPage(tabpanel1, PARA_TAB_CTRL_PARA_TAB,2,&tabpanel2);
	SetCtrlAttribute(tabpanel2,CH3_P_TAB_NUM_OPPSITE_KAP,ATTR_DIMMED,0);
	SetCtrlAttribute(tabpanel2,CH3_P_TAB_NUM_OPPSITE_KAI,ATTR_DIMMED,0);
	SetCtrlAttribute(tabpanel2,CH3_P_TAB_NUM_OPPSITE_KAD,ATTR_DIMMED,0);
	SetCtrlAttribute(tabpanel2,CH3_P_TAB_NUM_OPPSITE_FA1,ATTR_DIMMED,0);
	SetCtrlAttribute(tabpanel2,CH3_P_TAB_NUM_OPPSITE_FA2,ATTR_DIMMED,0);
	SetCtrlAttribute(tabpanel2,CH3_P_TAB_NUM_OPPSITE_FAK,ATTR_DIMMED,0);
	SetCtrlAttribute(tabpanel2,CH3_P_TAB_NUM_OPPSITE_KTP,ATTR_DIMMED,0);
	SetCtrlAttribute(tabpanel2,CH3_P_TAB_NUM_OPPSITE_KTI,ATTR_DIMMED,0);
	SetCtrlAttribute(tabpanel2,CH3_P_TAB_NUM_OPPSITE_KTD,ATTR_DIMMED,0);
	SetCtrlAttribute(tabpanel2,CH3_P_TAB_NUM_OPPSITE_FT1,ATTR_DIMMED,0);
	SetCtrlAttribute(tabpanel2,CH3_P_TAB_NUM_OPPSITE_FT2,ATTR_DIMMED,0);
	SetCtrlAttribute(tabpanel2,CH3_P_TAB_NUM_OPPSITE_FTK,ATTR_DIMMED,0);
	SetCtrlAttribute(tabpanel2,CH3_P_TAB_NUM_OPPSITE_KEP,ATTR_DIMMED,0);
	SetCtrlAttribute(tabpanel2,CH3_P_TAB_NUM_OPPSITE_KED,ATTR_DIMMED,0);
	SetCtrlAttribute(tabpanel2,CH3_P_TAB_NUM_OPPSITE_TED,ATTR_DIMMED,0);
	SetCtrlAttribute(tabpanel2,CH3_P_TAB_NUM_OPPSITE_T1, ATTR_DIMMED,0);
	SetCtrlAttribute(tabpanel2,CH3_P_TAB_NUM_OPPSITE_T2, ATTR_DIMMED,0);
	SetCtrlAttribute(tabpanel2,CH3_P_TAB_NUM_OPPSITE_X1, ATTR_DIMMED,0);
	SetCtrlAttribute(tabpanel2,CH3_P_TAB_NUM_OPPSITE_X2, ATTR_DIMMED,0);
	SetCtrlAttribute(tabpanel2,CH3_P_TAB_NUM_OPPSITE_X3, ATTR_DIMMED,0);
	
	GetPanelHandleFromTabPage(tabpanel1, PARA_TAB_CTRL_PARA_TAB,3,&tabpanel2);
	SetCtrlAttribute(tabpanel2,CH4_P_TAB_NUM_OPPSITE_KAP,ATTR_DIMMED,0);
	SetCtrlAttribute(tabpanel2,CH4_P_TAB_NUM_OPPSITE_KAI,ATTR_DIMMED,0);
	SetCtrlAttribute(tabpanel2,CH4_P_TAB_NUM_OPPSITE_KAD,ATTR_DIMMED,0);
	SetCtrlAttribute(tabpanel2,CH4_P_TAB_NUM_OPPSITE_FA1,ATTR_DIMMED,0);
	SetCtrlAttribute(tabpanel2,CH4_P_TAB_NUM_OPPSITE_FA2,ATTR_DIMMED,0);
	SetCtrlAttribute(tabpanel2,CH4_P_TAB_NUM_OPPSITE_FAK,ATTR_DIMMED,0);
	SetCtrlAttribute(tabpanel2,CH4_P_TAB_NUM_OPPSITE_KTP,ATTR_DIMMED,0);
	SetCtrlAttribute(tabpanel2,CH4_P_TAB_NUM_OPPSITE_KTI,ATTR_DIMMED,0);
	SetCtrlAttribute(tabpanel2,CH4_P_TAB_NUM_OPPSITE_KTD,ATTR_DIMMED,0);
	SetCtrlAttribute(tabpanel2,CH4_P_TAB_NUM_OPPSITE_FT1,ATTR_DIMMED,0);
	SetCtrlAttribute(tabpanel2,CH4_P_TAB_NUM_OPPSITE_FT2,ATTR_DIMMED,0);
	SetCtrlAttribute(tabpanel2,CH4_P_TAB_NUM_OPPSITE_FTK,ATTR_DIMMED,0);
	SetCtrlAttribute(tabpanel2,CH4_P_TAB_NUM_OPPSITE_KEP,ATTR_DIMMED,0);
	SetCtrlAttribute(tabpanel2,CH4_P_TAB_NUM_OPPSITE_KED,ATTR_DIMMED,0);
	SetCtrlAttribute(tabpanel2,CH4_P_TAB_NUM_OPPSITE_TED,ATTR_DIMMED,0);
	SetCtrlAttribute(tabpanel2,CH4_P_TAB_NUM_OPPSITE_T1, ATTR_DIMMED,0);
	SetCtrlAttribute(tabpanel2,CH4_P_TAB_NUM_OPPSITE_T2, ATTR_DIMMED,0);
	SetCtrlAttribute(tabpanel2,CH4_P_TAB_NUM_OPPSITE_X1, ATTR_DIMMED,0);
	SetCtrlAttribute(tabpanel2,CH4_P_TAB_NUM_OPPSITE_X2, ATTR_DIMMED,0);
	SetCtrlAttribute(tabpanel2,CH4_P_TAB_NUM_OPPSITE_X3, ATTR_DIMMED,0);

}

/////////////////////////�����ǻص�����/////////////////////

///////**********���ص�����************////////
int CVICALLBACK OppsitePanelCallback (int panel, int event, void *callbackData,
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

///////**********Y���귶Χ�ص�����************////////
int CVICALLBACK YRangeChange (int panel, int control, int event,
		void *callbackData, int eventData1, int eventData2)
{
	switch (event)
	{
		double dTemp;
		
		case EVENT_COMMIT:
			
			switch(control)
			{
				case OPPSITEPNL_YRANGE_RING_1:
					GetCtrlVal(panel,control, &dTemp);
					SetAxisRange(panel,OPPSITEPNL_STRIPCHART_1,VAL_NO_CHANGE,0.0,1.0,VAL_MANUAL,-dTemp,dTemp);
					break;
				case OPPSITEPNL_YRANGE_RING_2:
					GetCtrlVal(panel,control,&dTemp);
					SetAxisRange(panel,OPPSITEPNL_STRIPCHART_2,VAL_NO_CHANGE,0.0,1.0,VAL_MANUAL,-dTemp,dTemp);
					break;
				case OPPSITEPNL_YRANGE_RING_3:
					GetCtrlVal(panel,control, &dTemp);
					SetAxisRange(panel,OPPSITEPNL_STRIPCHART_3,VAL_NO_CHANGE,0.0,1.0,VAL_MANUAL,-dTemp,dTemp);
					break;
				case OPPSITEPNL_YRANGE_RING_4:
					GetCtrlVal(panel,control,&dTemp);
					SetAxisRange(panel,OPPSITEPNL_STRIPCHART_4,VAL_NO_CHANGE,0.0,1.0,VAL_MANUAL,-dTemp,dTemp);
					break;
			} 
			
			break;
	}
	return 0;
}
 
///////**********�������ݻص�����************////////
int CVICALLBACK OppsiteSaveCallback (int panel, int control, int event,
		void *callbackData, int eventData1, int eventData2)
{
	switch (event)
	{
		case EVENT_COMMIT:
			strcpy(saveDir,"D:\\��������" );
			if (FileExists(saveDir,0)<1)
			{
				err = MakeDir (saveDir);
				strcat(saveDir,"\\" );
				strcat(saveDir, DateStr());  
				err = MakeDir (saveDir);
			}
			else
			{
				strcat(saveDir,"\\" );  
				strcat(saveDir, DateStr());  
				if (FileExists(saveDir,0)<1) 
				{
					err = MakeDir (saveDir); 
				}
			}
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

///////**********��¼���ݻص�����************////////
int CVICALLBACK OppsiteRecordCallback (int panel, int control, int event,
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

///////**********�˳��ص�����************////////
int CVICALLBACK OppsiteExitCallback (int panel, int control, int event,
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
				
				DisconnectFromTCPServer (gTCPConnection);   
				
				QuitUserInterface(0);  
			}
			break;
	}
	return 0;
}

///////**********��ʼ�ص�����************////////
int CVICALLBACK OppsiteStartCallback (int panel, int control, int event,
		void *callbackData, int eventData1, int eventData2)
{
	switch (event)
	{
		case EVENT_COMMIT:
			if(ConfirmPopup ("ȷ������", "ȷ��������"))
			{
				OppositeSetScreenCycNum();	//������Ļ��ʾ����  
			
				Octrlparadown();
			
				//��ָ����Ϣ���͸�ʵʱϵͳ����Ϣͷ������ָ���Լ���������ָ����ĳ���
				sendMessageHead.iCommand = START_PROCESS;
				sendMessageHead.iMessageLen = COMMANDSIZE;
				ClientTCPWrite(gTCPConnection, &sendMessageHead, MESSAGEHEADSIZE, 1000);         //��������ͷ
				 
				InitOppsiteTestPara();		//��ʼ�����Բ���
				ClientTCPWrite(gTCPConnection, pExMem, COMMANDSIZE, 5000);		    //�������� 
				/*
				pExMem->requestType = START_PROCESS; 
				pExMem->request  = IS_REQUEST;
				*/
			}
			break;
	}
	return 0;
}

///////**********��λ�ص�����************////////
int CVICALLBACK OppsiteResetCallback (int panel, int control, int event,
		void *callbackData, int eventData1, int eventData2)
{
	int iAngleChannel,iTorqueChannel;
	int i;
	switch (event)
	{
		case EVENT_COMMIT:
			
			sendMessageHead.iCommand = RESET_PROCESS;
			sendMessageHead.iMessageLen = COMMANDSIZE;
			ClientTCPWrite(gTCPConnection, &sendMessageHead, MESSAGEHEADSIZE, 1000);         //��������ͷ
			Sleep(10);  
			Octrlparadown();
			InitOppsiteTestPara();
			ClientTCPWrite(gTCPConnection, pExMem, COMMANDSIZE, 5000);		    //��������
			break;
	}
	return 0;
}

int CVICALLBACK OppDisChangeCallback (int panel, int control, int event,
		void *callbackData, int eventData1, int eventData2)
{
	switch (event)
	{
		case EVENT_COMMIT:
			InitOppsiteDisIndex();
			break;
	}
	return 0;
}

///////**********����ͨ���ص�����************////////
int CVICALLBACK TChannelChooseCallback (int panel, int control, int event,
		void *callbackData, int eventData1, int eventData2)
{
	switch (event)
	{
		int iTemp;
		
		case EVENT_COMMIT:
			GetCtrlVal(panel,control,&iTemp);
			SetCtrlVal(panel,SET_TAB_ANGLE_CH_RING,OppsiteChannel[iTemp]);
			//SetCtrlVal(OppsitePanelHandle,OPPSITEPNL_CHART3_CH_RING,iTemp);
			SetCtrlVal(OppsitePanelHandle,OPPSITEPNL_CHART4_CH_RING,iTemp);
			//SetCtrlVal(OppsitePanelHandle,OPPSITEPNL_CHART1_CH_RING,OppsiteChannel[iTemp]);
			SetCtrlVal(OppsitePanelHandle,OPPSITEPNL_CHART2_CH_RING,OppsiteChannel[iTemp]);
			
			InitOppsiteDisIndex();
			break;
	}
	return 0;
}

///////**********λ��ͨ���ص�����************////////
int CVICALLBACK AChannelChooseCallback (int panel, int control, int event,
		void *callbackData, int eventData1, int eventData2)
{
	switch (event)
	{
		int iTemp;
		
		case EVENT_COMMIT:
			GetCtrlVal(panel,control,&iTemp);
			SetCtrlVal(panel,SET_TAB_TORQUE_CH_RING,OppsiteChannel[iTemp]);
			//SetCtrlVal(OppsitePanelHandle,OPPSITEPNL_CHART1_CH_RING,iTemp);
			SetCtrlVal(OppsitePanelHandle,OPPSITEPNL_CHART2_CH_RING,iTemp);
			//SetCtrlVal(OppsitePanelHandle,OPPSITEPNL_CHART3_CH_RING,OppsiteChannel[iTemp]);
			SetCtrlVal(OppsitePanelHandle,OPPSITEPNL_CHART4_CH_RING,OppsiteChannel[iTemp]);
			
			InitOppsiteDisIndex();
			break;
	}
	return 0;
}

///////**********�������ͻص�����************////////
int CVICALLBACK TestTypeChangeCallback (int panel, int control, int event,
		void *callbackData, int eventData1, int eventData2)
{
	switch (event)
	{
		int iTemp;
		int handle_A;
		int i;
		
		case EVENT_COMMIT:

			GetCtrlVal(panel,control,&iTemp);
			switch(iTemp)
			{
				case REMOVE_EXTRA_TORQUE:
					SetCtrlAttribute(panel,SET_TAB_TORQUE_GIVEN_WAVE,ATTR_DIMMED,0);
					SetCtrlVal(panel,SET_TAB_TORQUE_GIVEN_WAVE,4);
					SetCtrlAttribute(panel,SET_TAB_TORQUE_GIVEN_AMP,ATTR_DIMMED,0);
					SetCtrlAttribute(panel,SET_TAB_TORQUE_GIVEN_FREQ,ATTR_DIMMED,0); 
					SetCtrlAttribute(panel,SET_TAB_TORQUE_GIVEN_AMP,ATTR_LABEL_TEXT,"����ָ���ֵ");
					SetCtrlAttribute(panel,SET_TAB_TEXTMSG_13,ATTR_CTRL_VAL,"Nm"); //��ʾ��λ
					SetCtrlAttribute(panel,SET_TAB_ANGLE_GIVEN_WAVE,ATTR_DIMMED,0);
					SetCtrlAttribute(panel,SET_TAB_ANGLE_GIVEN_AMP,ATTR_DIMMED,0); 
					SetCtrlAttribute(panel,SET_TAB_ANGLE_GIVEN_FREQ,ATTR_DIMMED,0);
					break;
				case ANY_CHART_TRACE:
					SetCtrlAttribute(panel,SET_TAB_TORQUE_GIVEN_WAVE,ATTR_DIMMED,0);
					SetCtrlAttribute(panel,SET_TAB_TORQUE_GIVEN_AMP,ATTR_DIMMED,0); 
					SetCtrlAttribute(panel,SET_TAB_TORQUE_GIVEN_FREQ,ATTR_DIMMED,0);
					SetCtrlVal(panel,SET_TAB_TORQUE_GIVEN_WAVE,1); 
					SetCtrlAttribute(panel,SET_TAB_TORQUE_GIVEN_AMP,ATTR_LABEL_TEXT,"����ָ���ֵ");
					SetCtrlAttribute(panel,SET_TAB_TEXTMSG_13,ATTR_CTRL_VAL,"Nm");
					SetCtrlAttribute(panel,SET_TAB_ANGLE_GIVEN_WAVE,ATTR_DIMMED,0);
					SetCtrlAttribute(panel,SET_TAB_ANGLE_GIVEN_AMP,ATTR_DIMMED,0); 
					SetCtrlAttribute(panel,SET_TAB_ANGLE_GIVEN_FREQ,ATTR_DIMMED,0);
					break;
				case PROPORTION_TRACE:
					SetCtrlAttribute(panel,SET_TAB_TORQUE_GIVEN_WAVE,ATTR_DIMMED,1);
					SetCtrlAttribute(panel,SET_TAB_TORQUE_GIVEN_AMP,ATTR_DIMMED,0); 
					SetCtrlAttribute(panel,SET_TAB_TORQUE_GIVEN_AMP,ATTR_LABEL_TEXT,"���ؼ����ݶ�");
					SetCtrlAttribute(panel,SET_TAB_TEXTMSG_13,ATTR_CTRL_VAL,"��/Nm"); 
					SetCtrlVal(panel,SET_TAB_TORQUE_GIVEN_WAVE,3); 
					SetCtrlAttribute(panel,SET_TAB_TORQUE_GIVEN_FREQ,ATTR_DIMMED,1); 
					SetCtrlAttribute(panel,SET_TAB_ANGLE_GIVEN_WAVE,ATTR_DIMMED,0);
					SetCtrlAttribute(panel,SET_TAB_ANGLE_GIVEN_AMP,ATTR_DIMMED,0); 
					SetCtrlAttribute(panel,SET_TAB_ANGLE_GIVEN_FREQ,ATTR_DIMMED,0);
					break;
				case  TORQUE_TRACE:
				    SetCtrlAttribute(panel,SET_TAB_TORQUE_GIVEN_WAVE,ATTR_DIMMED,1);
					SetCtrlVal(panel,SET_TAB_TORQUE_GIVEN_WAVE,4);
					SetCtrlAttribute(panel,SET_TAB_TORQUE_GIVEN_AMP,ATTR_DIMMED,1); 
					SetCtrlAttribute(panel,SET_TAB_TORQUE_GIVEN_FREQ,ATTR_DIMMED,1); 
			    	SetCtrlAttribute(panel,SET_TAB_TORQUE_GIVEN_AMP,ATTR_LABEL_TEXT,"����ָ���ֵ");
					SetCtrlAttribute(panel,SET_TAB_ANGLE_GIVEN_WAVE,ATTR_DIMMED,1);
					SetCtrlAttribute(panel,SET_TAB_ANGLE_GIVEN_AMP,ATTR_DIMMED,1); 
					SetCtrlAttribute(panel,SET_TAB_ANGLE_GIVEN_FREQ,ATTR_DIMMED,1); 
					
			
				FileToArray ("E:\\150Nmcontrol programV2(zaihepu)\\tor.dat", pTraceMem->Tor, VAL_DOUBLE, 453031, 1, VAL_GROUPS_TOGETHER, VAL_GROUPS_AS_ROWS, VAL_ASCII);
					
					
			   if(FileToArray ("E:\\150Nmcontrol programV2(zaihepu)\\pos.dat", pTraceMem->Pos, VAL_DOUBLE, 453031, 1, VAL_GROUPS_TOGETHER, VAL_GROUPS_AS_ROWS,VAL_ASCII)==0)
				
					{
					  MessagePopup ("��ϲ", "�ɹ������غ���");	
					}
			    else			 
					{
					  MessagePopup ("����", "δ�ܳɹ������غ���");	 
					} 
				
					

					break;  
					
			}
			
			break;
	}
	return 0;
}

///////**********���ò����ص�����************////////
int CVICALLBACK OppsiteSetParaCallback (int panel, int control, int event,
		void *callbackData, int eventData1, int eventData2)
{
	double dTemp;
	int selbutton;          
	char keyinputbuffer[65];
	
	switch (event)
	{
		case EVENT_COMMIT:
			
		/*	if(keyerr)  //��������־λ
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
								LightOppositeParaSet();	//light the paraset
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
				LightOppositeParaSet();
				
			} */
			LightOppositeParaSet();
			break;
	}
	return 0;
}

///////**********��������ص�����************////////
int CVICALLBACK OppsiteSaveParaCallback (int panel, int control, int event,
		void *callbackData, int eventData1, int eventData2)
{
	double dTemp;
	
	switch (event)
	{
		case EVENT_COMMIT:
			if(ConfirmPopup("Confirm Popup","�����������������滻ԭ�ڴ��в�����"))
			{
			 GetPanelHandleFromTabPage (OppsitePanelHandle, OPPSITEPNL_OPP_TAB,1,&tabpanel1);
			
			//ͨ��1�Ŀ��Ʋ���
			GetPanelHandleFromTabPage(tabpanel1, PARA_TAB_CTRL_PARA_TAB,0,&tabpanel2);
			GetCtrlVal(tabpanel2,CH1_P_TAB_NUM_OPPSITE_KAP,&dTemp);
			Ini_PutDouble(inihandle,"CtrlPara","CH1_Kap",dTemp);
			GetCtrlVal(tabpanel2,CH1_P_TAB_NUM_OPPSITE_KAI,&dTemp);
			Ini_PutDouble(inihandle,"CtrlPara","CH1_Kai",dTemp); 
			GetCtrlVal(tabpanel2,CH1_P_TAB_NUM_OPPSITE_KAD,&dTemp);
			Ini_PutDouble(inihandle,"CtrlPara","CH1_Kad",dTemp);
			GetCtrlVal(tabpanel2,CH1_P_TAB_NUM_OPPSITE_FA1,&dTemp);
			Ini_PutDouble(inihandle,"CtrlPara","CH1_Fa1",dTemp);
			GetCtrlVal(tabpanel2,CH1_P_TAB_NUM_OPPSITE_FA2,&dTemp);
			Ini_PutDouble(inihandle,"CtrlPara","CH1_Fa2",dTemp); 
			GetCtrlVal(tabpanel2,CH1_P_TAB_NUM_OPPSITE_FAK,&dTemp);
			Ini_PutDouble(inihandle,"CtrlPara","CH1_Fak",dTemp); 
			
		
			GetCtrlVal(tabpanel2,CH1_P_TAB_NUM_OPPSITE_KTP,&dTemp);
			Ini_PutDouble(inihandle,"CtrlPara","CH1_Ktp",dTemp);
			GetCtrlVal(tabpanel2,CH1_P_TAB_NUM_OPPSITE_KTI,&dTemp);
			Ini_PutDouble(inihandle,"CtrlPara","CH1_Kti",dTemp); 
			GetCtrlVal(tabpanel2,CH1_P_TAB_NUM_OPPSITE_KTD,&dTemp);
			Ini_PutDouble(inihandle,"CtrlPara","CH1_Ktd",dTemp);
			GetCtrlVal(tabpanel2,CH1_P_TAB_NUM_OPPSITE_FT1,&dTemp);
			Ini_PutDouble(inihandle,"CtrlPara","CH1_Ft1",dTemp);
			GetCtrlVal(tabpanel2,CH1_P_TAB_NUM_OPPSITE_FT2,&dTemp);
			Ini_PutDouble(inihandle,"CtrlPara","CH1_Ft2",dTemp); 
			GetCtrlVal(tabpanel2,CH1_P_TAB_NUM_OPPSITE_FTK,&dTemp);
			Ini_PutDouble(inihandle,"CtrlPara","CH1_Ftk",dTemp);
			
			GetCtrlVal(tabpanel2,CH1_P_TAB_NUM_OPPSITE_KEP,&dTemp);
			Ini_PutDouble(inihandle,"CtrlPara","CH1_Kep",dTemp);
			GetCtrlVal(tabpanel2,CH1_P_TAB_NUM_OPPSITE_KED,&dTemp);
			Ini_PutDouble(inihandle,"CtrlPara","CH1_Ked",dTemp); 
			GetCtrlVal(tabpanel2,CH1_P_TAB_NUM_OPPSITE_TED,&dTemp);
			Ini_PutDouble(inihandle,"CtrlPara","CH1_Ted",dTemp);
			
			GetCtrlVal(tabpanel2,CH1_P_TAB_NUM_OPPSITE_T1,&dTemp);
			Ini_PutDouble(inihandle,"CtrlPara","CH1_T1",dTemp);
			GetCtrlVal(tabpanel2,CH1_P_TAB_NUM_OPPSITE_T2,&dTemp);
			Ini_PutDouble(inihandle,"CtrlPara","CH1_T2",dTemp); 
			GetCtrlVal(tabpanel2,CH1_P_TAB_NUM_OPPSITE_X1,&dTemp);
			Ini_PutDouble(inihandle,"CtrlPara","CH1_X1",dTemp);
			GetCtrlVal(tabpanel2,CH1_P_TAB_NUM_OPPSITE_X2,&dTemp);
			Ini_PutDouble(inihandle,"CtrlPara","CH1_X2",dTemp);
			GetCtrlVal(tabpanel2,CH1_P_TAB_NUM_OPPSITE_X3,&dTemp);
			Ini_PutDouble(inihandle,"CtrlPara","CH1_X3",dTemp); 
		
			
			//ͨ��2�Ŀ��Ʋ���
			GetPanelHandleFromTabPage(tabpanel1, PARA_TAB_CTRL_PARA_TAB,1,&tabpanel2);
			GetCtrlVal(tabpanel2,CH2_P_TAB_NUM_OPPSITE_KAP,&dTemp);
			Ini_PutDouble(inihandle,"CtrlPara","CH2_Kap",dTemp); 
			GetCtrlVal(tabpanel2,CH2_P_TAB_NUM_OPPSITE_KAI,&dTemp);
			Ini_PutDouble(inihandle,"CtrlPara","CH2_Kai",dTemp);  
			GetCtrlVal(tabpanel2,CH2_P_TAB_NUM_OPPSITE_KAD,&dTemp);
			Ini_PutDouble(inihandle,"CtrlPara","CH2_Kad",dTemp);
			GetCtrlVal(tabpanel2,CH2_P_TAB_NUM_OPPSITE_FA1,&dTemp);
			Ini_PutDouble(inihandle,"CtrlPara","CH2_Fa1",dTemp);
			GetCtrlVal(tabpanel2,CH2_P_TAB_NUM_OPPSITE_FA2,&dTemp);
			Ini_PutDouble(inihandle,"CtrlPara","CH2_Fa2",dTemp); 
			GetCtrlVal(tabpanel2,CH2_P_TAB_NUM_OPPSITE_FAK,&dTemp);
			Ini_PutDouble(inihandle,"CtrlPara","CH2_Fak",dTemp); 
			
			GetCtrlVal(tabpanel2,CH2_P_TAB_NUM_OPPSITE_KTP,&dTemp);
			Ini_PutDouble(inihandle,"CtrlPara","CH2_Ktp",dTemp); 
			GetCtrlVal(tabpanel2,CH2_P_TAB_NUM_OPPSITE_KTI,&dTemp);
			Ini_PutDouble(inihandle,"CtrlPara","CH2_Kti",dTemp);  
			GetCtrlVal(tabpanel2,CH2_P_TAB_NUM_OPPSITE_KTD,&dTemp);
			Ini_PutDouble(inihandle,"CtrlPara","CH2_Ktd",dTemp);
			
			GetCtrlVal(tabpanel2,CH2_P_TAB_NUM_OPPSITE_FT1,&dTemp);
			Ini_PutDouble(inihandle,"CtrlPara","CH2_Ft1",dTemp);
			GetCtrlVal(tabpanel2,CH2_P_TAB_NUM_OPPSITE_FT2,&dTemp);
			Ini_PutDouble(inihandle,"CtrlPara","CH2_Ft2",dTemp); 
			GetCtrlVal(tabpanel2,CH2_P_TAB_NUM_OPPSITE_FTK,&dTemp);
			Ini_PutDouble(inihandle,"CtrlPara","CH2_Ftk",dTemp);
			
			GetCtrlVal(tabpanel2,CH2_P_TAB_NUM_OPPSITE_KEP,&dTemp);
			Ini_PutDouble(inihandle,"CtrlPara","CH2_Kep",dTemp);
			GetCtrlVal(tabpanel2,CH2_P_TAB_NUM_OPPSITE_KED,&dTemp);
			Ini_PutDouble(inihandle,"CtrlPara","CH2_Ked",dTemp); 
			GetCtrlVal(tabpanel2,CH2_P_TAB_NUM_OPPSITE_TED,&dTemp);
			Ini_PutDouble(inihandle,"CtrlPara","CH2_Ted",dTemp);
			
			GetCtrlVal(tabpanel2,CH2_P_TAB_NUM_OPPSITE_T1,&dTemp);
			Ini_PutDouble(inihandle,"CtrlPara","CH2_T1",dTemp);
			GetCtrlVal(tabpanel2,CH2_P_TAB_NUM_OPPSITE_T2,&dTemp);
			Ini_PutDouble(inihandle,"CtrlPara","CH2_T2",dTemp); 
			GetCtrlVal(tabpanel2,CH2_P_TAB_NUM_OPPSITE_X1,&dTemp);
			Ini_PutDouble(inihandle,"CtrlPara","CH2_X1",dTemp);
			GetCtrlVal(tabpanel2,CH2_P_TAB_NUM_OPPSITE_X2,&dTemp);
			Ini_PutDouble(inihandle,"CtrlPara","CH2_X2",dTemp);
			GetCtrlVal(tabpanel2,CH2_P_TAB_NUM_OPPSITE_X3,&dTemp);
			Ini_PutDouble(inihandle,"CtrlPara","CH2_X3",dTemp); 
			
			//ͨ��3�Ŀ��Ʋ���
			GetPanelHandleFromTabPage(tabpanel1, PARA_TAB_CTRL_PARA_TAB,2,&tabpanel2);
			GetCtrlVal(tabpanel2,CH3_P_TAB_NUM_OPPSITE_KAP,&dTemp);
			Ini_PutDouble(inihandle,"CtrlPara","CH3_Kap",dTemp);
			GetCtrlVal(tabpanel2,CH3_P_TAB_NUM_OPPSITE_KAI,&dTemp);
			Ini_PutDouble(inihandle,"CtrlPara","CH3_Kai",dTemp);
			GetCtrlVal(tabpanel2,CH3_P_TAB_NUM_OPPSITE_KAD,&dTemp);
			Ini_PutDouble(inihandle,"CtrlPara","CH3_Kad",dTemp);
			GetCtrlVal(tabpanel2,CH3_P_TAB_NUM_OPPSITE_FA1,&dTemp);
			Ini_PutDouble(inihandle,"CtrlPara","CH3_Fa1",dTemp);
			GetCtrlVal(tabpanel2,CH3_P_TAB_NUM_OPPSITE_FA2,&dTemp);
			Ini_PutDouble(inihandle,"CtrlPara","CH3_Fa2",dTemp); 
			GetCtrlVal(tabpanel2,CH3_P_TAB_NUM_OPPSITE_FAK,&dTemp);
			Ini_PutDouble(inihandle,"CtrlPara","CH3_Fak",dTemp); 
			
			GetCtrlVal(tabpanel2,CH3_P_TAB_NUM_OPPSITE_KTP,&dTemp);
			Ini_PutDouble(inihandle,"CtrlPara","CH3_Ktp",dTemp);
			GetCtrlVal(tabpanel2,CH3_P_TAB_NUM_OPPSITE_KTI,&dTemp);
			Ini_PutDouble(inihandle,"CtrlPara","CH3_Kti",dTemp);
			GetCtrlVal(tabpanel2,CH3_P_TAB_NUM_OPPSITE_KTD,&dTemp);
			Ini_PutDouble(inihandle,"CtrlPara","CH3_Ktd",dTemp);
			
			GetCtrlVal(tabpanel2,CH3_P_TAB_NUM_OPPSITE_FT1,&dTemp);
			Ini_PutDouble(inihandle,"CtrlPara","CH3_Ft1",dTemp);
			GetCtrlVal(tabpanel2,CH3_P_TAB_NUM_OPPSITE_FT2,&dTemp);
			Ini_PutDouble(inihandle,"CtrlPara","CH3_Ft2",dTemp); 
			GetCtrlVal(tabpanel2,CH3_P_TAB_NUM_OPPSITE_FTK,&dTemp);
			Ini_PutDouble(inihandle,"CtrlPara","CH3_Ftk",dTemp);
			
			GetCtrlVal(tabpanel2,CH3_P_TAB_NUM_OPPSITE_KEP,&dTemp);
			Ini_PutDouble(inihandle,"CtrlPara","CH3_Kep",dTemp);
			GetCtrlVal(tabpanel2,CH3_P_TAB_NUM_OPPSITE_KED,&dTemp);
			Ini_PutDouble(inihandle,"CtrlPara","CH3_Ked",dTemp); 
			GetCtrlVal(tabpanel2,CH3_P_TAB_NUM_OPPSITE_TED,&dTemp);
			Ini_PutDouble(inihandle,"CtrlPara","CH3_Ted",dTemp);
			
			GetCtrlVal(tabpanel2,CH3_P_TAB_NUM_OPPSITE_T1,&dTemp);
			Ini_PutDouble(inihandle,"CtrlPara","CH3_T1",dTemp);
			GetCtrlVal(tabpanel2,CH3_P_TAB_NUM_OPPSITE_T2,&dTemp);
			Ini_PutDouble(inihandle,"CtrlPara","CH3_T2",dTemp); 
			GetCtrlVal(tabpanel2,CH3_P_TAB_NUM_OPPSITE_X1,&dTemp);
			Ini_PutDouble(inihandle,"CtrlPara","CH3_X1",dTemp);
			GetCtrlVal(tabpanel2,CH3_P_TAB_NUM_OPPSITE_X2,&dTemp);
			Ini_PutDouble(inihandle,"CtrlPara","CH3_X2",dTemp);
			GetCtrlVal(tabpanel2,CH3_P_TAB_NUM_OPPSITE_X3,&dTemp);
			Ini_PutDouble(inihandle,"CtrlPara","CH3_X3",dTemp); 
			
			//ͨ��4�Ŀ��Ʋ���
			GetPanelHandleFromTabPage(tabpanel1, PARA_TAB_CTRL_PARA_TAB,3,&tabpanel2);
			GetCtrlVal(tabpanel2,CH4_P_TAB_NUM_OPPSITE_KAP,&dTemp);
			Ini_PutDouble(inihandle,"CtrlPara","CH4_Kap",dTemp); 
			GetCtrlVal(tabpanel2,CH4_P_TAB_NUM_OPPSITE_KAI,&dTemp);
			Ini_PutDouble(inihandle,"CtrlPara","CH4_Kai",dTemp); 
			GetCtrlVal(tabpanel2,CH4_P_TAB_NUM_OPPSITE_KAD,&dTemp);
			Ini_PutDouble(inihandle,"CtrlPara","CH4_Kad",dTemp);
			GetCtrlVal(tabpanel2,CH4_P_TAB_NUM_OPPSITE_FA1,&dTemp);
			Ini_PutDouble(inihandle,"CtrlPara","CH4_Fa1",dTemp);
			GetCtrlVal(tabpanel2,CH4_P_TAB_NUM_OPPSITE_FA2,&dTemp);
			Ini_PutDouble(inihandle,"CtrlPara","CH4_Fa2",dTemp); 
			GetCtrlVal(tabpanel2,CH4_P_TAB_NUM_OPPSITE_FAK,&dTemp);
			Ini_PutDouble(inihandle,"CtrlPara","CH4_Fak",dTemp); 
			
			GetCtrlVal(tabpanel2,CH4_P_TAB_NUM_OPPSITE_KTP,&dTemp);
			Ini_PutDouble(inihandle,"CtrlPara","CH4_Ktp",dTemp); 
			GetCtrlVal(tabpanel2,CH4_P_TAB_NUM_OPPSITE_KTI,&dTemp);
			Ini_PutDouble(inihandle,"CtrlPara","CH4_Kti",dTemp); 
			GetCtrlVal(tabpanel2,CH4_P_TAB_NUM_OPPSITE_KAD,&dTemp);
			Ini_PutDouble(inihandle,"CtrlPara","CH4_Ktd",dTemp);
			
			GetCtrlVal(tabpanel2,CH4_P_TAB_NUM_OPPSITE_FT1,&dTemp);
			Ini_PutDouble(inihandle,"CtrlPara","CH4_Ft1",dTemp);
			GetCtrlVal(tabpanel2,CH4_P_TAB_NUM_OPPSITE_FT2,&dTemp);
			Ini_PutDouble(inihandle,"CtrlPara","CH4_Ft2",dTemp); 
			GetCtrlVal(tabpanel2,CH4_P_TAB_NUM_OPPSITE_FTK,&dTemp);
			Ini_PutDouble(inihandle,"CtrlPara","CH4_Ftk",dTemp);
			
			GetCtrlVal(tabpanel2,CH4_P_TAB_NUM_OPPSITE_KEP,&dTemp);
			Ini_PutDouble(inihandle,"CtrlPara","CH4_Kep",dTemp);
			GetCtrlVal(tabpanel2,CH4_P_TAB_NUM_OPPSITE_KED,&dTemp);
			Ini_PutDouble(inihandle,"CtrlPara","CH4_Ked",dTemp); 
			GetCtrlVal(tabpanel2,CH4_P_TAB_NUM_OPPSITE_TED,&dTemp);
			Ini_PutDouble(inihandle,"CtrlPara","CH4_Ted",dTemp);
			
			GetCtrlVal(tabpanel2,CH4_P_TAB_NUM_OPPSITE_T1,&dTemp);
			Ini_PutDouble(inihandle,"CtrlPara","CH4_T1",dTemp);
			GetCtrlVal(tabpanel2,CH4_P_TAB_NUM_OPPSITE_T2,&dTemp);
			Ini_PutDouble(inihandle,"CtrlPara","CH4_T2",dTemp); 
			GetCtrlVal(tabpanel2,CH4_P_TAB_NUM_OPPSITE_X1,&dTemp);
			Ini_PutDouble(inihandle,"CtrlPara","CH4_X1",dTemp);
			GetCtrlVal(tabpanel2,CH4_P_TAB_NUM_OPPSITE_X2,&dTemp);
			Ini_PutDouble(inihandle,"CtrlPara","CH4_X2",dTemp);
			GetCtrlVal(tabpanel2,CH4_P_TAB_NUM_OPPSITE_X3,&dTemp);
			Ini_PutDouble(inihandle,"CtrlPara","CH4_X3",dTemp); 
			
			Ini_WriteToFile (inihandle, pathname);
			}
			else
			{}
			
			break;
	}
	return 0;
}

///////**********��Դ���ƻص�����************////////
int CVICALLBACK OilControlCallback (int panel, int control, int event,
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

int CVICALLBACK OppDataReDisplay (int panel, int control, int event,
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

int CVICALLBACK TOppDisChangeCallback (int panel, int control, int event,
		void *callbackData, int eventData1, int eventData2)
{
	long N;
	int iTemp,ringindex;
	double dring_val;
	
	switch (event)
	{
		case EVENT_COMMIT:
			
			/*GetCtrlVal(panel,control,&iTemp);									//�����ĸ�strips��ʾͨ��
			SetCtrlVal(OppsitePanelHandle,OPPSITEPNL_CHART3_CH_RING,iTemp);
			SetCtrlVal(OppsitePanelHandle,OPPSITEPNL_CHART4_CH_RING,iTemp);
			SetCtrlVal(OppsitePanelHandle,OPPSITEPNL_CHART1_CH_RING,OppsiteChannel[iTemp]);
			SetCtrlVal(OppsitePanelHandle,OPPSITEPNL_CHART2_CH_RING,OppsiteChannel[iTemp]);
			*/
			InitOppsiteDisIndex();
			
			/*if(pExMem->testPara[chart3_ch].iStatus == 2)         //�����ѡͨ����״̬Ϊ��
			{
				N = (long)(2000 / (pExMem->testPara[chart3_ch].dFreq) * CYCLE_NUM_PER_SCREEN);  //��Ļ��ʾ�������㹫ʽ
				
				if(N >= 3 && N <= 10000)
				{
				
					SetCtrlAttribute(OppsitePanelHandle,OPPSITEPNL_STRIPCHART_3,ATTR_POINTS_PER_SCREEN,N);
					SetCtrlAttribute(OppsitePanelHandle,OPPSITEPNL_STRIPCHART_4,ATTR_POINTS_PER_SCREEN,N);
				}
				
				for(ringindex = 0;ringindex < 11;ringindex++)  //����ֵ������Χ������y����
				{
					GetValueFromIndex(OppsitePanelHandle,OPPSITEPNL_YRANGE_RING_3,ringindex,&dring_val);
					if(pExMem->testPara[chart3_ch].dAmp < dring_val)
					{
						SetCtrlIndex(OppsitePanelHandle,OPPSITEPNL_YRANGE_RING_3,ringindex);
						SetCtrlIndex(OppsitePanelHandle,OPPSITEPNL_YRANGE_RING_4,ringindex);
						SetAxisRange(OppsitePanelHandle,OPPSITEPNL_STRIPCHART_3,VAL_NO_CHANGE,0.0,1.0,VAL_MANUAL,-dring_val,dring_val);
						SetAxisRange(OppsitePanelHandle,OPPSITEPNL_STRIPCHART_4,VAL_NO_CHANGE,0.0,1.0,VAL_MANUAL,-dring_val,dring_val);
						break;
					}
				}
			}*/
			
			break;
	}
	return 0;
}

int CVICALLBACK AOppDisChangeCallback (int panel, int control, int event,
		void *callbackData, int eventData1, int eventData2)
{
	long N;
	int iTemp,ringindex;
	double dring_val;
	
	
	switch (event)
	{
		case EVENT_COMMIT:
			
			/*GetCtrlVal(panel,control,&iTemp);								//�����ĸ�strips��ʾͨ����ͬ
			SetCtrlVal(OppsitePanelHandle,OPPSITEPNL_CHART1_CH_RING,iTemp);
			SetCtrlVal(OppsitePanelHandle,OPPSITEPNL_CHART2_CH_RING,iTemp);
			SetCtrlVal(OppsitePanelHandle,OPPSITEPNL_CHART3_CH_RING,OppsiteChannel[iTemp]);
			SetCtrlVal(OppsitePanelHandle,OPPSITEPNL_CHART4_CH_RING,OppsiteChannel[iTemp]);
			*/
			InitOppsiteDisIndex();
			
			/*if(pExMem->testPara[chart1_ch].iStatus == 1)         //�����ѡͨ����״̬Ϊ��
			{
				N = (long)(2000 / (pExMem->testPara[chart1_ch].dFreq) * CYCLE_NUM_PER_SCREEN);  //��Ļ��ʾ�������㹫ʽ
				if(N >= 3 && N <= 10000)
				{
				
					SetCtrlAttribute(OppsitePanelHandle,OPPSITEPNL_STRIPCHART_1,ATTR_POINTS_PER_SCREEN,N);
					SetCtrlAttribute(OppsitePanelHandle,OPPSITEPNL_STRIPCHART_2,ATTR_POINTS_PER_SCREEN,N);
				}
				
				for(ringindex = 0;ringindex < 11;ringindex++)  //����ֵ������Χ������y����
				{
					GetValueFromIndex(OppsitePanelHandle,OPPSITEPNL_YRANGE_RING_1,ringindex,&dring_val);
					if(pExMem->testPara[chart1_ch].dAmp < dring_val)
					{
						SetCtrlIndex(OppsitePanelHandle,OPPSITEPNL_YRANGE_RING_1,ringindex);
						SetCtrlIndex(OppsitePanelHandle,OPPSITEPNL_YRANGE_RING_2,ringindex);
						SetAxisRange(OppsitePanelHandle,OPPSITEPNL_STRIPCHART_1,VAL_NO_CHANGE,0.0,1.0,VAL_MANUAL,-dring_val,dring_val);
						SetAxisRange(OppsitePanelHandle,OPPSITEPNL_STRIPCHART_2,VAL_NO_CHANGE,0.0,1.0,VAL_MANUAL,-dring_val,dring_val);
						break;
					}
				}
			}*/
			break;
	}
	return 0;
}

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
			iDataLoops =  iDataLen /transdatasizeO;
		
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
					chart1[2*i] = readData[transdatasizeO*i+ chart1_ch-1];	 //λ��ָ��		
					chart1[2*i+1] = readData[transdatasizeO*i + 24 + chart1_ch-1]; //λ�ò���
					
					chart3[2*i] = readData[transdatasizeO*i+ chart3_ch-1];	 //��ָ��    
					chart3[2*i+1] = readData[transdatasizeO*i + 8 + chart3_ch-1];	 //������       
				
					switch(chart2_content)
					{
						case ANGLE_SAMPLE_INDEX:
								chart2[i] = readData[transdatasizeO*i+28+chart2_ch-1];
							break;
						case ANGLE_GIVEN_INDEX:
								chart2[i] = readData[transdatasizeO*i+0+chart2_ch-1];  
							break;
						case TORQUE_SAMPLE_INDEX:
								chart2[i] = readData[transdatasizeO*i+8+chart2_ch-1];  
							break;
						case TORQUE_GIVEN_INDEX:
								chart2[i] = readData[transdatasizeO*i+0+chart2_ch-1];      
							break;
						case SERVO_OUT_INDEX:
								chart2[i] = readData[transdatasizeO*i+16+chart2_ch-1];      
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
								chart4[i] = readData[transdatasizeO*i+28+chart4_ch-1];
							break;
						case ANGLE_GIVEN_INDEX:
								chart4[i] = readData[transdatasizeO*i+0+chart4_ch-1]; 
							break;
						case TORQUE_SAMPLE_INDEX:
								chart4[i] = readData[transdatasizeO*i+8+chart4_ch-1]; 
							break;
						case TORQUE_GIVEN_INDEX:
								chart4[i] = readData[transdatasizeO*i+0+chart4_ch-1];;
							break;
						case SERVO_OUT_INDEX:
								chart4[i] = readData[transdatasizeO*i+16+chart4_ch-1]; 
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
					PlotStripChart(OppsitePanelHandle,OPPSITEPNL_STRIPCHART_1,chart1,2*iDataLoops,0,0,VAL_DOUBLE);
					PlotStripChart(OppsitePanelHandle,OPPSITEPNL_STRIPCHART_2,chart2,iDataLoops,0,0,VAL_DOUBLE);  	     
					PlotStripChart(OppsitePanelHandle,OPPSITEPNL_STRIPCHART_3,chart3,2*iDataLoops,0,0,VAL_DOUBLE);
					PlotStripChart(OppsitePanelHandle,OPPSITEPNL_STRIPCHART_4,chart4,iDataLoops,0,0,VAL_DOUBLE); 
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

 int CVICALLBACK NetWatchDog2 (int panel, int control, int event,
		void *callbackData, int eventData1, int eventData2)
{
	switch (event)
	{
		case EVENT_TIMER_TICK:
			
			//��ָ����Ϣ���͸�ʵʱϵͳ����Ϣͷ������ָ���Լ���������ָ����ĳ���
			sendMessageHead.iCommand = WATCHDOG;   
			
			sendMessageHead.iMessageLen = 0;		 //Ҳû��ʵ�ʵ�����
		
			//ClientTCPWrite(gTCPConnection, &sendMessageHead, MESSAGEHEADSIZE, 1000);         //��������ͷ

			break;
	}
	return 0;
}

int CVICALLBACK OEnablecallback (int panel, int control, int event,
		void *callbackData, int eventData1, int eventData2)
{
	double dTemp;
	int tempInt;
	int SendBuffLen;
	int iAngleChannel,iTorqueChannel;
	int i; 	
	
	switch (event)
	{
		case EVENT_COMMIT:
			GetPanelHandleFromTabPage (OppsitePanelHandle, OPPSITEPNL_OPP_TAB,0,&tabpanel1); 
			GetCtrlVal(tabpanel1,SET_TAB_ANGLE_CH_RING,&iAngleChannel); 
			GetCtrlVal(tabpanel1,SET_TAB_TORQUE_CH_RING,&iTorqueChannel);
			
			sendMessageHead.iCommand = ENABLE_PROCESS;
			sendMessageHead.iMessageLen = COMMANDSIZE; 
			ClientTCPWrite(gTCPConnection, &sendMessageHead, MESSAGEHEADSIZE, 1000);         //��������ͷ
			
			InitOppsiteTestPara();		//��ʼ�����Բ��� 
			
			if (no > 1)	   //��������ݣ��Ͷ������е�����
			{
				no=no-2;
			}
			//GetCtrlVal(tabpanel2,STAPANEL_LED,&n);
		
			if (no== 0)	   //��������ݣ��Ͷ������е�����
			{
				SetCtrlAttribute(panel,control,ATTR_LABEL_TEXT,"ֹͣʹ��"); 
				//GetPanelHandleFromTabPage (OppsitePanelHandle, OPPSITEPNL_STA_TAB,0,&tabpanel1);
					   
				 for(i= 1; i<5; i++)
				 
				 {
					 if(i == iAngleChannel)
						 {
							 pExMem->testPara[i].iEnable = 2; 		   //1��ʾλ��ͨ����2��ʾ����ͨ�� 
							 }
					 else if(i == iTorqueChannel)
						 {
							 pExMem->testPara[i].iEnable = 2; 
						 }
					 else 
						 { 
							 pExMem->testPara[i].iEnable = 1;   
						}
				 
				     //InitStaticTestPara();			 //��ʼ�����Բ���   
				 }
			}
			 else
			 {
				 SetCtrlAttribute(panel,control,ATTR_LABEL_TEXT,"ʹ��");
				 //GetPanelHandleFromTabPage (OppsitePanelHandle, OPPSITEPNL_STA_TAB,0,&tabpanel1);  
				 
				 //ͨ��1�Ĳ��Բ���
				 pExMem->testPara[1].iEnable = 1; 
				  
				 //ͨ��2�Ĳ��Բ���
				 pExMem->testPara[2].iEnable = 1; 
				 
				 //ͨ��3�Ĳ��Բ���
				 pExMem->testPara[3].iEnable = 1; 
				 
				 //ͨ��4�Ĳ��Բ���
				 pExMem->testPara[4].iEnable = 1; 
						
				// InitStaticTestPara();			 //��ʼ�����Բ���
			 }
			ClientTCPWrite(gTCPConnection, pExMem, COMMANDSIZE, 5000);		    //�������� 
			
			no=no+1;
			
			break;
	}
	return 0;
}

int CVICALLBACK OppsiteDownParaCallback (int panel, int control, int event,
		void *callbackData, int eventData1, int eventData2)
{
	switch (event)
	{
		case EVENT_COMMIT:
			//��ָ����Ϣ���͸�ʵʱϵͳ����Ϣͷ������ָ���Լ���������ָ����ĳ���
			sendMessageHead.iCommand = SETPARA_PROCESS;
			sendMessageHead.iMessageLen = COMMANDSIZE;
			ClientTCPWrite(gTCPConnection, &sendMessageHead, MESSAGEHEADSIZE, 1000);         //��������ͷ
				
			Octrlparadown();
			ClientTCPWrite(gTCPConnection, pExMem, COMMANDSIZE, 5000);		    //��������Ͳ���
		break;
	}
	return 0;
}

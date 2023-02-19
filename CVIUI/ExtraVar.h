//���������ⲿ�����ͱ���
#ifndef EXTVAR
#define EXTVAR

#include "DataMemory.h"
#include "ExchangeMemory.h"
#include <windows.h>
#include "inifile.h"  

//�豸ͨ������Ԥ����
#define AXIS_NO 4
#define SIDE_NO 5  
#define STAGE_NO 2	  //��������


//ͨ��״̬Ԥ����
#define CHANNEL_OFF 0
#define CHANNEL_ON 1

//�������ӦԤ����
#define IS_REQUEST 1
#define NO_REQUEST  0
#define IS_RESPOND 1
#define NO_RESPOND 0

//��������Ԥ����

#define EXIT_PROCESS -1
#define START_PROCESS 1
#define RESET_PROCESS 2  
#define SETPARA_PROCESS 3
#define PAUSE_PROCESS 4
#define RETURN_ZERO   5
#define ENABLE_PROCESS  7
#define WATCHDOG      88

//�˶���ʾ��������Ԥ����
#define MOTION_FOLLOW 1		//�˶���ʾ����
#define MOTION_SACN_FREQ  2  //ӿ����ʾɨƵ 

//����Ԥ����
#define ANGLE_SAMPLE_INDEX 7   
#define ANGLE_GIVEN_INDEX  22  

#define TORQUE_SAMPLE_INDEX 2   
#define TORQUE_GIVEN_INDEX  3  
#define AXIS_FORCE_SAMPLE_INDEX 9   
#define AXIS_FORCE_GIVEN_INDEX  10  
#define POSITION_SAMPLE_INDEX  0
#define ACCELEROMETER_SAMPLE_INDEX  8  
  

#define SERVO_OUT_INDEX     4   
#define ERROR_INDEX         5
#define TORQUE_ERROR_INDEX   6


//��岨����ʾ
#define CYCLE_NUM_PER_SCREEN 2

//#define HOST_ADDRESS 			"127.0.0.1"
#define HOST_ADDRESS 			"192.168.7.109"  
#define HOST_PORT 				10001
//#define HOST_COMMOND_PORT       10009
#define TCP_INVALID_CONNECTION 	((unsigned int)-1)

//���������
static int MainPanelHandle;		//�����   
static int MotionPanelHandle;    //�˶���ʾ���
static int HILPanelHandle;		//��ʵ��������
static int HPSPanelHandle;    //��Դ�������
static int OppsitePanelHandle;   //�Զ��������

//extern int MainPanelHandle;		//�����   
//extern int MotionPanelHandle;    //�˶���ʾ���
//extern int HILPanelHandle;		//��ʵ��������
//extern int HPSPanelHandle;  //��Դ�������
//extern int OppsitePanelHandle;   //�Զ��������

//�����ⲿ����
extern char msgBuff[256];		//��Ϣ����
extern int initShareMemStatus;  //��ʼ�����ڴ�״̬
extern const int DATASIZE;
extern const int COMMANDSIZE;
extern const int MESSAGEHEADSIZE;
extern const int DATAFRAMESIZE;

//����rtss���̱���
extern int rtssStatus;
extern PROCESS_INFORMATION rtssInfo;
extern HANDLE hProcess;
extern TESTDATA *pDataMem;		//���ݹ����ڴ�ṹ��
extern EXCHANGE *pExMem;	    	//���������ڴ�ṹ��
extern TRACEDATA *pTraceMem; 
extern HANDLE hDataMem;	 			//�����ڴ������
extern HANDLE hExMem;				//�����ڴ������
extern HANDLE hTraceMem; 
extern int ExMemSize ;
extern int DataMemSize;
extern int TraceMemSize;
extern const int DATA_LENGTH;
extern const int TRACE_LENGTH;
extern int initShareMemStatus;  
extern int recording  ;


extern unsigned int gTCPConnection;



//������ʾ���ñ���
//��ʾ��ͨ��
extern int iDisChannel[5];
//��ʾ������
extern int iDisContent[5];
extern WAVESTRUCT sWaveStruct[64];
extern int iWaveIndex;
extern int iWaveCount;  //��������
extern int iChooseCh;
extern int lngPoint[5];  
extern const char channelLabel[5][16];
extern const char contentLabel[9][16]; 

//��������ı���
extern TESTDATA * saveBuf;
extern int saveBufLength;
extern int saveIndex;
extern const int dataStructSize ;
extern char savepath[512];
extern char saveDir[512];
extern int savefilehandle;
extern int err;
char pathpass[512];  //���ݵ�ַ                    
char DftPathSign;	//Ĭ�����ݻط����·����־


//����ָʾtab��壬�ҵ�tab�еĿؼ�
extern int tabpanel1,tabpanel2;

 //���Ʋ����ļ�
 //������д������ini�ļ�����ʽ
extern IniText inihandle;
extern int inierr;
extern char windowsDirectory[512];
extern char systemDirectory[512];
extern char currentDir[512]; 
extern char pathname[512];
extern char sectionNam[512];

//������ʾ����  
extern int chart1_ch,chart1_content;
extern int chart2_ch,chart2_content;   
extern int chart3_ch,chart3_content;
extern int chart4_ch,chart4_content;

//��Դѹ�����¶ȱ���ָʾ 
float fOilPress;
float fOilTemp;
int   iTempWarn;
int   iPressWarn;

//���Ʋ����޸�Ȩ��
char  originalkey[65];
int   keyerr;
#endif

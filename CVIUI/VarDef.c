//����������Ҫ���õĺ���������
#include "DataMemory.h"
#include "ExchangeMemory.h"
#include <windows.h> 
#include "inifile.h"

const int DATASIZE = sizeof(DataMessage);
const int MESSAGEHEADSIZE = sizeof(MessageHead);
const int COMMANDSIZE = sizeof(EXCHANGE);
const int DATAFRAMESIZE = sizeof(DataFrameHead); 


const char channelLabel[5][16] = {"","ͨ��һ","ͨ����","ͨ����","ͨ����"};
const char contentLabel[9][16] = {"λ�ò���","λ��ָ��","��(��)����","��(��)ָ��","�ŷ����","λ�����","��(��)���","���̽Ƕ�","���ٶ�"}; 


TESTDATA *pDataMem;		//���ݹ����ڴ�ṹ��
EXCHANGE *pExMem;	    //���������ڴ�ṹ��
TRACEDATA *pTraceMem;
HANDLE hDataMem;	 //�����ڴ������
HANDLE hExMem;		//�����ڴ������
HANDLE hTraceMem;

WAVESTRUCT sWaveStruct[64];
int iWaveIndex;
int iWaveCount;  //��������
int iChooseCh;
int lngPoint[5]; 

int ExMemSize = sizeof(EXCHANGE);
int DataMemSize = sizeof(TESTDATA);
int TraceMemSize = sizeof(TRACEDATA);

const int DATA_LENGTH = 65536;
//const int TRACE_LENGTH = 65536*3;
const int TRACE_LENGTH = 1; 
int initShareMemStatus;  
int recording = 0 ;

//����rtss���̱���
int rtssStatus = 0;
PROCESS_INFORMATION rtssInfo;
HANDLE hProcess;

//��Ϣ������  
char msgBuff[256];	

//���ݱ������
TESTDATA * saveBuf = NULL;
int saveBufLength;
int saveIndex = 0;
const int dataStructSize = sizeof(TESTDATA);
char savepath[512];
char saveDir[512];
int savefilehandle = 0;
int err;

//����ָʾtab��壬�ҵ�tab�еĿؼ�
int tabpanel1,tabpanel2,tabpanel3;
 
 //���Ʋ����ļ�
 //������д������ini�ļ�����ʽ
IniText inihandle;
int inierr;
char windowsDirectory[512];
char systemDirectory[512];
char currentDir[512]; //���ڴ��·��
char pathname[512];


//������ʾ����
int chart1_ch,chart1_content=1;
int chart2_ch,chart2_content;   
int chart3_ch,chart3_content=1;
int chart4_ch,chart4_content;


unsigned int 	gTCPConnection	= -1;    









//用来定义需要调用的函数及变量
#include "DataMemory.h"
#include "ExchangeMemory.h"
#include <windows.h> 
#include "inifile.h"

const int DATASIZE = sizeof(DataMessage);
const int MESSAGEHEADSIZE = sizeof(MessageHead);
const int COMMANDSIZE = sizeof(EXCHANGE);
const int DATAFRAMESIZE = sizeof(DataFrameHead); 


const char channelLabel[5][16] = {"","通道一","通道二","通道三","通道四"};
const char contentLabel[9][16] = {"位置采样","位置指令","力(矩)采样","力(矩)指令","伺服输出","位置误差","力(矩)误差","码盘角度","加速度"}; 


TESTDATA *pDataMem;		//数据共享内存结构体
EXCHANGE *pExMem;	    //交互共享内存结构体
TRACEDATA *pTraceMem;
HANDLE hDataMem;	 //数据内存区句柄
HANDLE hExMem;		//交互内存区句柄
HANDLE hTraceMem;

WAVESTRUCT sWaveStruct[64];
int iWaveIndex;
int iWaveCount;  //波形数量
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

//启动rtss进程变量
int rtssStatus = 0;
PROCESS_INFORMATION rtssInfo;
HANDLE hProcess;

//消息缓冲区  
char msgBuff[256];	

//数据保存变量
TESTDATA * saveBuf = NULL;
int saveBufLength;
int saveIndex = 0;
const int dataStructSize = sizeof(TESTDATA);
char savepath[512];
char saveDir[512];
int savefilehandle = 0;
int err;

//用来指示tab面板，找到tab中的控件
int tabpanel1,tabpanel2,tabpanel3;
 
 //控制参数文件
 //参数读写，采用ini文件的形式
IniText inihandle;
int inierr;
char windowsDirectory[512];
char systemDirectory[512];
char currentDir[512]; //用于存放路径
char pathname[512];


//曲线显示索引
int chart1_ch,chart1_content=1;
int chart2_ch,chart2_content;   
int chart3_ch,chart3_content=1;
int chart4_ch,chart4_content;


unsigned int 	gTCPConnection	= -1;    









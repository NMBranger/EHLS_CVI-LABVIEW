//用来声明外部函数和变量
#ifndef EXTVAR
#define EXTVAR

#include "DataMemory.h"
#include "ExchangeMemory.h"
#include <windows.h>
#include "inifile.h"  

//设备通道数量预定义
#define AXIS_NO 4
#define SIDE_NO 5  
#define STAGE_NO 2	  //几级加载


//通道状态预定义
#define CHANNEL_OFF 0
#define CHANNEL_ON 1

//请求与回应预定义
#define IS_REQUEST 1
#define NO_REQUEST  0
#define IS_RESPOND 1
#define NO_RESPOND 0

//测试类型预定义

#define EXIT_PROCESS -1
#define START_PROCESS 1
#define RESET_PROCESS 2  
#define SETPARA_PROCESS 3
#define PAUSE_PROCESS 4
#define RETURN_ZERO   5
#define ENABLE_PROCESS  7
#define WATCHDOG      88

//运动演示测试类型预定义
#define MOTION_FOLLOW 1		//运动演示跟踪
#define MOTION_SACN_FREQ  2  //涌动演示扫频 

//索引预定义
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


//面板波形显示
#define CYCLE_NUM_PER_SCREEN 2

//#define HOST_ADDRESS 			"127.0.0.1"
#define HOST_ADDRESS 			"192.168.7.109"  
#define HOST_PORT 				10001
//#define HOST_COMMOND_PORT       10009
#define TCP_INVALID_CONNECTION 	((unsigned int)-1)

//面板句柄声明
static int MainPanelHandle;		//主面板   
static int MotionPanelHandle;    //运动演示面板
static int HILPanelHandle;		//半实物仿真面板
static int HPSPanelHandle;    //油源控制面板
static int OppsitePanelHandle;   //对顶加载面板

//extern int MainPanelHandle;		//主面板   
//extern int MotionPanelHandle;    //运动演示面板
//extern int HILPanelHandle;		//半实物仿真面板
//extern int HPSPanelHandle;  //油源控制面板
//extern int OppsitePanelHandle;   //对顶加载面板

//声明外部变量
extern char msgBuff[256];		//信息缓存
extern int initShareMemStatus;  //初始共享内存状态
extern const int DATASIZE;
extern const int COMMANDSIZE;
extern const int MESSAGEHEADSIZE;
extern const int DATAFRAMESIZE;

//启动rtss进程变量
extern int rtssStatus;
extern PROCESS_INFORMATION rtssInfo;
extern HANDLE hProcess;
extern TESTDATA *pDataMem;		//数据共享内存结构体
extern EXCHANGE *pExMem;	    	//交互共享内存结构体
extern TRACEDATA *pTraceMem; 
extern HANDLE hDataMem;	 			//数据内存区句柄
extern HANDLE hExMem;				//交互内存区句柄
extern HANDLE hTraceMem; 
extern int ExMemSize ;
extern int DataMemSize;
extern int TraceMemSize;
extern const int DATA_LENGTH;
extern const int TRACE_LENGTH;
extern int initShareMemStatus;  
extern int recording  ;


extern unsigned int gTCPConnection;



//曲线显示所用变量
//显示的通道
extern int iDisChannel[5];
//显示的数据
extern int iDisContent[5];
extern WAVESTRUCT sWaveStruct[64];
extern int iWaveIndex;
extern int iWaveCount;  //波形数量
extern int iChooseCh;
extern int lngPoint[5];  
extern const char channelLabel[5][16];
extern const char contentLabel[9][16]; 

//用来保存的变量
extern TESTDATA * saveBuf;
extern int saveBufLength;
extern int saveIndex;
extern const int dataStructSize ;
extern char savepath[512];
extern char saveDir[512];
extern int savefilehandle;
extern int err;
char pathpass[512];  //传递地址                    
char DftPathSign;	//默认数据回放软件路径标志


//用来指示tab面板，找到tab中的控件
extern int tabpanel1,tabpanel2;

 //控制参数文件
 //参数读写，采用ini文件的形式
extern IniText inihandle;
extern int inierr;
extern char windowsDirectory[512];
extern char systemDirectory[512];
extern char currentDir[512]; 
extern char pathname[512];
extern char sectionNam[512];

//曲线显示索引  
extern int chart1_ch,chart1_content;
extern int chart2_ch,chart2_content;   
extern int chart3_ch,chart3_content;
extern int chart4_ch,chart4_content;

//油源压力和温度报警指示 
float fOilPress;
float fOilTemp;
int   iTempWarn;
int   iPressWarn;

//控制参数修改权限
char  originalkey[65];
int   keyerr;
#endif

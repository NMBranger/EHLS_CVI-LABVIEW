#pragma once

typedef struct
{
	//位置PID参数
	double Kap;   //0
	double Kai;
	double Kad;
	
	//位置模糊参数
	double Fa1;  //3
	double Fa2;
	double Fak;
	
	//加载PID参数
	double Ktp;   //6
	double Kti;
	double Ktd;
	
    //加载模糊参数
	double Ft1;	   //9
	double Ft2;
	double Ftk;
	
	//对顶消扰参数
	double Kep;	   //12	   伺服指令、采样差，比例后加到控制上
	double Ked;			  //13 前馈滤波频宽
	double Ted;		//14    未滤波的前馈信号比例
	
	//扩展备用参数 
	double T1;   //15
	double T2;   //16
	double T3;	  //17
	double T4;	  //18
	double X1;	  //19
	double X2;	  //20		 滤波后的前馈信号比例
	double X3;	  //21
	double X4;	  //22
	double X5;    //23
	
}CONTROLPARA;

typedef struct
{
	int  iStatus;//开、关或者是加载通道还是位置通道（对顶加载）
	int  iTestType;//测试类型，如消除多余力，任意波形跟踪，载荷谱跟踪
	int  iWave;//波形 
	int iEnable; 
	
	double dAmp;//幅值，梯度（梯度加载）
	double dOffset;  //偏置
	double dFreq;//频率,扫频时起始频率    
	double dStopFreq;//扫频时终止频率
	double dTestTime;//测试时间，可以实现定周期停止测试   
}TESTPARA;


typedef struct
{
	TESTPARA    testPara[10];
	CONTROLPARA ctrlPara[10];
}EXCHANGE;


typedef struct
{
	int iCommand;
	int iMessageLen;
	int ienable1;
	int ienable2;

}MessageHead;  

typedef union
{
	int intdata;
	unsigned short shortdata[2];
} IntorShort;


// Message structure sent from host executable to RT project
// 一个任务结构体是一个时间段的任务，有多少个段就建立多少个这个结构体，完成一个之后初始化下一个任务，再开始
typedef struct
{
	 TESTPARA testpara[10];
}CommandMessage;


#pragma once

typedef struct
{
   	double AngleCom[9];
	double TorqueSample[5]; 
	double AxisForceSample[4]
	double ServoOut[9];
	double DisSample[5]; 
	double AngleSample[5]; 
	double Acceleration[5];  
}TESTDATA;

typedef struct
{
	int AngleSample[5];
	int AngleGiven[5];
	int TorqueSample[5];
	int TorqueGiven[5];
	int ServoOut[5];
}DataMessage;

typedef struct
{
   	int AngleSample[5];
	int AngleGiven[5];
	int TorqueSample[5];
	int TorqueGiven[5];
	int ServoOut[5];
}TESTDATANEW;


typedef struct
{
	//��ָ�����
   double dTrace[5];
   //����Ƕ�ָ��
   double dTrace_A[5];
   
   double Pos[453031];
   double Tor[453031];
   
}TRACEDATA;

typedef struct
{
   	int iWave;
	double dAmp;
	double dFreq;
	double dRamp;
	int    iCycle;
	double distance;
}WAVESTRUCT;



//��λ������λ���������ݵ�ͷ
typedef struct
{
	int iCh;
	int iLen; 
}DataFrameHead;


 

#pragma once

typedef struct
{
	//λ��PID����
	double Kap;   //0
	double Kai;
	double Kad;
	
	//λ��ģ������
	double Fa1;  //3
	double Fa2;
	double Fak;
	
	//����PID����
	double Ktp;   //6
	double Kti;
	double Ktd;
	
    //����ģ������
	double Ft1;	   //9
	double Ft2;
	double Ftk;
	
	//�Զ����Ų���
	double Kep;	   //12	   �ŷ�ָ������������ӵ�������
	double Ked;			  //13 ǰ���˲�Ƶ��
	double Ted;		//14    δ�˲���ǰ���źű���
	
	//��չ���ò��� 
	double T1;   //15
	double T2;   //16
	double T3;	  //17
	double T4;	  //18
	double X1;	  //19
	double X2;	  //20		 �˲����ǰ���źű���
	double X3;	  //21
	double X4;	  //22
	double X5;    //23
	
}CONTROLPARA;

typedef struct
{
	int  iStatus;//�����ػ����Ǽ���ͨ������λ��ͨ�����Զ����أ�
	int  iTestType;//�������ͣ������������������Ⲩ�θ��٣��غ��׸���
	int  iWave;//���� 
	int iEnable; 
	
	double dAmp;//��ֵ���ݶȣ��ݶȼ��أ�
	double dOffset;  //ƫ��
	double dFreq;//Ƶ��,ɨƵʱ��ʼƵ��    
	double dStopFreq;//ɨƵʱ��ֹƵ��
	double dTestTime;//����ʱ�䣬����ʵ�ֶ�����ֹͣ����   
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
// һ������ṹ����һ��ʱ��ε������ж��ٸ��ξͽ������ٸ�����ṹ�壬���һ��֮���ʼ����һ�������ٿ�ʼ
typedef struct
{
	 TESTPARA testpara[10];
}CommandMessage;


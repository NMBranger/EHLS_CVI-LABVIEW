//�����ⲿ����
#ifndef FUNCTIONS
#define FUNCTIONS

//������ʼ��    
extern int MainPanelInit(void);

//�˶���ʾ����ʼ�� 
extern int MotionPanelInit(void);
void InitMotionTestPara(void);
void InitMotionDisIndex(void);
void InitMotionCtrlSet(void);
void InitMotionCtrlPara(void);
void MotionSetScreenCycNum(void);
void LightMotionParaSet(void);

//�Զ���������ʼ��
extern int OppsitePanelInit(void);
void InitOppsiteTestPara(void);
void InitOppsiteDisIndex(void);
void InitOppsiteCtrlSet(void);
void InitOppsiteCtrlPara(void);
void OppositeSetScreenCycNum(void); 
void LightOppositeParaSet(void);

//��ֹ���ز�������ʼ��
extern int StaticPanelInit(void);
void InitStaticTestPara(void);
void InitStaticDisIndex(void);
void InitStaticCtrlSet(void);
void InitStaticCtrlPara(void);
void StaticSetScreenCycNum(void);
void LightStaticParaSet(void);

//��ʵ���������ʼ��
extern int HILPanelInit(void);
void InitHILTestPara(void);
void InitHILDisIndex(void);
void InitHILCtrlSet(void);
void InitHILCtrlPara(void);
void HILSetScreenCycNum(void); 
void LightHILParaSet(void);


//�˳�������
extern void ExitPoint(void);

//��Դ�������ĺ���
extern int HPSPanelInit(void);
extern int HPSPanelClose(void);
extern int HPSClose(void);  


void HySupplyPanelExit(void); 

void DisplayHPSPanel(void);  


#endif







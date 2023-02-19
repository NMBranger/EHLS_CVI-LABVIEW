//声明外部函数
#ifndef FUNCTIONS
#define FUNCTIONS

//主面板初始化    
extern int MainPanelInit(void);

//运动演示面板初始化 
extern int MotionPanelInit(void);
void InitMotionTestPara(void);
void InitMotionDisIndex(void);
void InitMotionCtrlSet(void);
void InitMotionCtrlPara(void);
void MotionSetScreenCycNum(void);
void LightMotionParaSet(void);

//对顶加载面板初始化
extern int OppsitePanelInit(void);
void InitOppsiteTestPara(void);
void InitOppsiteDisIndex(void);
void InitOppsiteCtrlSet(void);
void InitOppsiteCtrlPara(void);
void OppositeSetScreenCycNum(void); 
void LightOppositeParaSet(void);

//静止加载测试面板初始化
extern int StaticPanelInit(void);
void InitStaticTestPara(void);
void InitStaticDisIndex(void);
void InitStaticCtrlSet(void);
void InitStaticCtrlPara(void);
void StaticSetScreenCycNum(void);
void LightStaticParaSet(void);

//半实物仿真面板初始化
extern int HILPanelInit(void);
void InitHILTestPara(void);
void InitHILDisIndex(void);
void InitHILCtrlSet(void);
void InitHILCtrlPara(void);
void HILSetScreenCycNum(void); 
void LightHILParaSet(void);


//退出清理函数
extern void ExitPoint(void);

//油源控制器的函数
extern int HPSPanelInit(void);
extern int HPSPanelClose(void);
extern int HPSClose(void);  


void HySupplyPanelExit(void); 

void DisplayHPSPanel(void);  


#endif







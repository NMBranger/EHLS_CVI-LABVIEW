#include <cvirte.h> 
#include <userint.h> 
#include <ansi_c.h>
#include "ExtraVar.h"
#include <tcpsupp.h>
#include <utility.h>
#include "Functions.h"
#include "MainPanel.h"



static int MainPanelHandle;

MessageHead sendMessageHead; 
MessageHead readMessageHead;  
  

int CVICALLBACK TCPDataCallback1 (unsigned handle, int xType, int errCode, void *cbData);     


int __stdcall WinMain (HINSTANCE hInstance, HINSTANCE hPrevInstance,
                       LPSTR lpszCmdLine, int nCmdShow)
{
	if (InitCVIRTE (hInstance, 0, 0) == 0)
		return -1;	/* out of memory */
	
	//装载油源控制面板，但不显示
	HPSPanelInit() ;  
	
	//初始化指令内存去，pExMem现在作为指令内存区
	pExMem = (EXCHANGE *)malloc(sizeof(EXCHANGE));
	
	Delay(1); 
	
	saveBufLength = 1024*1024;
	saveBuf = (TESTDATA *)malloc(dataStructSize * saveBufLength);
	if(saveBuf == NULL)
	{
		sprintf(msgBuff,"开辟保存内存错误，错误号-%d",initShareMemStatus);
		MessagePopup ("开辟保存内存错误！", msgBuff);
		return -1;
	}
	else
	{
		//InsertTextBoxLine (MainPanelHandle,LOGOPANEL_LOGO_MESSAGE , 0 , "开辟保存内存成功");       
	}
	
	//主界面初始化
	MainPanelInit();
	
	HPSClose();   
	
	

	return 0;  
	
Error:
    /* error operation */
	return -1;
}

//主面板初始化
int MainPanelInit(void)
{
    if ((MainPanelHandle = LoadPanel (0, "MainPanel.uir", MAINPANEL)) < 0)
		return -1;
	DisplayPanel (MainPanelHandle);
	RunUserInterface ();
	DiscardPanel (MainPanelHandle);
    return 1;
}

//主面板回调
int CVICALLBACK MainPanelCallback (int panel, int event, void *callbackData,
		int eventData1, int eventData2)
{
	switch (event)
	{
		case EVENT_GOT_FOCUS:

			break;
		case EVENT_LOST_FOCUS:

			break;
		case EVENT_CLOSE:
			//执行退出点函数
			//HPSPanelExit();	//油源控制界面退出
			ExitPoint();			//退出点函数
			QuitUserInterface(0);   //退出用户界面
			break;
	}
	return 0;
}

/***********   网络检测回调  ************/ 
int CVICALLBACK NetDetectCallback (int panel, int control, int event,
		void *callbackData, int eventData1, int eventData2)
{
	switch (event)
	{
		case EVENT_COMMIT:
			HidePanel (MainPanelHandle);//隐藏主界面  
			 
			//启动rtss进程
			
			//NetDetectPanelInit();   //程序初始化    
			
			DisplayPanel (MainPanelHandle);   //显示主界面
			break;
	}
	return 0;
}

/***********   静止加载回调  ************/ 
int CVICALLBACK StaticTestCallback (int panel, int control, int event,
		void *callbackData, int eventData1, int eventData2)
{
	switch (event)
	{
		case EVENT_COMMIT:
			HidePanel (MainPanelHandle);//隐藏主界面  
			 
			StaticPanelInit();   //程序初始化    
			
			DisplayPanel(MainPanelHandle);   //显示主界面
			break;
	}
	return 0;
}


/***********   对顶加载回调  ************/
int CVICALLBACK OppositeTestCallback (int panel, int control, int event,
		void *callbackData, int eventData1, int eventData2)
{
	switch (event)
	{
		case EVENT_COMMIT:
			HidePanel (MainPanelHandle);//隐藏主界面  
			 
			OppsitePanelInit();     
			
			DisplayPanel (MainPanelHandle);   //显示主界面
			break;
	}
	return 0;
}

/***********   运动演示回调  ************/ 
int CVICALLBACK MotionTestCallback (int panel, int control, int event,
		void *callbackData, int eventData1, int eventData2)
{
	switch (event)
	{
		case EVENT_COMMIT:
	
			
			HidePanel (MainPanelHandle); //隐藏主界面
			
	
			MotionPanelInit();  //程序初始化
			
			
			DisplayPanel (MainPanelHandle); //显示主界面
		
			
			break;
	}
	return 0;
Error:
	return -1;
	
}

/***********   半实物仿真回调  ************/
int CVICALLBACK HILTestCallback (int panel, int control, int event,
		void *callbackData, int eventData1, int eventData2)
{
	switch (event)
	{
		case EVENT_COMMIT:
			HidePanel (MainPanelHandle); //隐藏主界面
			
			
			HILPanelInit();  //程序初始化
			
			DisplayPanel (MainPanelHandle); //显示主界面
			break;
	}
	return 0;
}

/***********   系统设置回调  ************/
int CVICALLBACK SystemSetCallback (int panel, int control, int event,
		void *callbackData, int eventData1, int eventData2)
{
	switch (event)
	{
		case EVENT_COMMIT:

			break;
	}
	return 0;
}

/***********   油源控制回调  ************/
int CVICALLBACK HySupplyCallback (int panel, int control, int event,
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

/***********   帮助回调  ************/ 
int CVICALLBACK HelpCallback (int panel, int control, int event,
		void *callbackData, int eventData1, int eventData2)
{
	switch (event)
	{
		case EVENT_COMMIT:

			break;
	}
	return 0;
}

/***********   退出回调  ************/
int CVICALLBACK ExitCallback (int panel, int control, int event,
		void *callbackData, int eventData1, int eventData2)
{
	switch (event)
	{
		case EVENT_COMMIT:
			if(ConfirmPopup ("确定退出", "确定退出？"))
			{
				HPSPanelClose() ;
				QuitUserInterface(0);  
			}
			break;
	}
	return 0;
}

 

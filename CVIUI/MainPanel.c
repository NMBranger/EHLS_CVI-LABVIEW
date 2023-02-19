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
	
	//װ����Դ������壬������ʾ
	HPSPanelInit() ;  
	
	//��ʼ��ָ���ڴ�ȥ��pExMem������Ϊָ���ڴ���
	pExMem = (EXCHANGE *)malloc(sizeof(EXCHANGE));
	
	Delay(1); 
	
	saveBufLength = 1024*1024;
	saveBuf = (TESTDATA *)malloc(dataStructSize * saveBufLength);
	if(saveBuf == NULL)
	{
		sprintf(msgBuff,"���ٱ����ڴ���󣬴����-%d",initShareMemStatus);
		MessagePopup ("���ٱ����ڴ����", msgBuff);
		return -1;
	}
	else
	{
		//InsertTextBoxLine (MainPanelHandle,LOGOPANEL_LOGO_MESSAGE , 0 , "���ٱ����ڴ�ɹ�");       
	}
	
	//�������ʼ��
	MainPanelInit();
	
	HPSClose();   
	
	

	return 0;  
	
Error:
    /* error operation */
	return -1;
}

//������ʼ��
int MainPanelInit(void)
{
    if ((MainPanelHandle = LoadPanel (0, "MainPanel.uir", MAINPANEL)) < 0)
		return -1;
	DisplayPanel (MainPanelHandle);
	RunUserInterface ();
	DiscardPanel (MainPanelHandle);
    return 1;
}

//�����ص�
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
			//ִ���˳��㺯��
			//HPSPanelExit();	//��Դ���ƽ����˳�
			ExitPoint();			//�˳��㺯��
			QuitUserInterface(0);   //�˳��û�����
			break;
	}
	return 0;
}

/***********   ������ص�  ************/ 
int CVICALLBACK NetDetectCallback (int panel, int control, int event,
		void *callbackData, int eventData1, int eventData2)
{
	switch (event)
	{
		case EVENT_COMMIT:
			HidePanel (MainPanelHandle);//����������  
			 
			//����rtss����
			
			//NetDetectPanelInit();   //�����ʼ��    
			
			DisplayPanel (MainPanelHandle);   //��ʾ������
			break;
	}
	return 0;
}

/***********   ��ֹ���ػص�  ************/ 
int CVICALLBACK StaticTestCallback (int panel, int control, int event,
		void *callbackData, int eventData1, int eventData2)
{
	switch (event)
	{
		case EVENT_COMMIT:
			HidePanel (MainPanelHandle);//����������  
			 
			StaticPanelInit();   //�����ʼ��    
			
			DisplayPanel(MainPanelHandle);   //��ʾ������
			break;
	}
	return 0;
}


/***********   �Զ����ػص�  ************/
int CVICALLBACK OppositeTestCallback (int panel, int control, int event,
		void *callbackData, int eventData1, int eventData2)
{
	switch (event)
	{
		case EVENT_COMMIT:
			HidePanel (MainPanelHandle);//����������  
			 
			OppsitePanelInit();     
			
			DisplayPanel (MainPanelHandle);   //��ʾ������
			break;
	}
	return 0;
}

/***********   �˶���ʾ�ص�  ************/ 
int CVICALLBACK MotionTestCallback (int panel, int control, int event,
		void *callbackData, int eventData1, int eventData2)
{
	switch (event)
	{
		case EVENT_COMMIT:
	
			
			HidePanel (MainPanelHandle); //����������
			
	
			MotionPanelInit();  //�����ʼ��
			
			
			DisplayPanel (MainPanelHandle); //��ʾ������
		
			
			break;
	}
	return 0;
Error:
	return -1;
	
}

/***********   ��ʵ�����ص�  ************/
int CVICALLBACK HILTestCallback (int panel, int control, int event,
		void *callbackData, int eventData1, int eventData2)
{
	switch (event)
	{
		case EVENT_COMMIT:
			HidePanel (MainPanelHandle); //����������
			
			
			HILPanelInit();  //�����ʼ��
			
			DisplayPanel (MainPanelHandle); //��ʾ������
			break;
	}
	return 0;
}

/***********   ϵͳ���ûص�  ************/
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

/***********   ��Դ���ƻص�  ************/
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

/***********   �����ص�  ************/ 
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

/***********   �˳��ص�  ************/
int CVICALLBACK ExitCallback (int panel, int control, int event,
		void *callbackData, int eventData1, int eventData2)
{
	switch (event)
	{
		case EVENT_COMMIT:
			if(ConfirmPopup ("ȷ���˳�", "ȷ���˳���"))
			{
				HPSPanelClose() ;
				QuitUserInterface(0);  
			}
			break;
	}
	return 0;
}

 

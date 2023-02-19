/**************************************************************************/
/* LabWindows/CVI User Interface Resource (UIR) Include File              */
/*                                                                        */
/* WARNING: Do not add to, delete from, or otherwise modify the contents  */
/*          of this include file.                                         */
/**************************************************************************/

#include <userint.h>

#ifdef __cplusplus
    extern "C" {
#endif

     /* Panels and Controls: */

#define  MAINPANEL                        1       /* callback function: MainPanelCallback */
#define  MAINPANEL_PB_EXIT                2       /* control type: command, callback function: ExitCallback */
#define  MAINPANEL_PB_HELP                3       /* control type: command, callback function: HelpCallback */
#define  MAINPANEL_PB_HYSUPPLY            4       /* control type: command, callback function: HySupplyCallback */
#define  MAINPANEL_PB_SYSSET              5       /* control type: command, callback function: SystemSetCallback */
#define  MAINPANEL_PB_HIL                 6       /* control type: command, callback function: HILTestCallback */
#define  MAINPANEL_PB_MOTION              7       /* control type: command, callback function: MotionTestCallback */
#define  MAINPANEL_PB_OPPOSITE            8       /* control type: command, callback function: OppositeTestCallback */
#define  MAINPANEL_PB_STATIC              9       /* control type: command, callback function: StaticTestCallback */
#define  MAINPANEL_PB_NETDETECT           10      /* control type: command, callback function: NetDetectCallback */
#define  MAINPANEL_TEXTMSG                11      /* control type: textMsg, callback function: (none) */
#define  MAINPANEL_PICTURE                12      /* control type: picture, callback function: (none) */
#define  MAINPANEL_DECORATION             13      /* control type: deco, callback function: (none) */
#define  MAINPANEL_PICTURE_2              14      /* control type: picture, callback function: (none) */
#define  MAINPANEL_TEXTMSG_2              15      /* control type: textMsg, callback function: (none) */


     /* Control Arrays: */

          /* (no control arrays in the resource file) */


     /* Menu Bars, Menus, and Menu Items: */

          /* (no menu bars in the resource file) */


     /* Callback Prototypes: */

int  CVICALLBACK ExitCallback(int panel, int control, int event, void *callbackData, int eventData1, int eventData2);
int  CVICALLBACK HelpCallback(int panel, int control, int event, void *callbackData, int eventData1, int eventData2);
int  CVICALLBACK HILTestCallback(int panel, int control, int event, void *callbackData, int eventData1, int eventData2);
int  CVICALLBACK HySupplyCallback(int panel, int control, int event, void *callbackData, int eventData1, int eventData2);
int  CVICALLBACK MainPanelCallback(int panel, int event, void *callbackData, int eventData1, int eventData2);
int  CVICALLBACK MotionTestCallback(int panel, int control, int event, void *callbackData, int eventData1, int eventData2);
int  CVICALLBACK NetDetectCallback(int panel, int control, int event, void *callbackData, int eventData1, int eventData2);
int  CVICALLBACK OppositeTestCallback(int panel, int control, int event, void *callbackData, int eventData1, int eventData2);
int  CVICALLBACK StaticTestCallback(int panel, int control, int event, void *callbackData, int eventData1, int eventData2);
int  CVICALLBACK SystemSetCallback(int panel, int control, int event, void *callbackData, int eventData1, int eventData2);


#ifdef __cplusplus
    }
#endif

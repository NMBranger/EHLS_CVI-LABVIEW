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

#define  STAPANEL                         1       /* callback function: StaticPanelCallback */
#define  STAPANEL_TEXTMSG_8               2       /* control type: textMsg, callback function: (none) */
#define  STAPANEL_CH_TEST_TIME            3       /* control type: numeric, callback function: (none) */
#define  STAPANEL_DECORATION_6            4       /* control type: deco, callback function: (none) */
#define  STAPANEL_STRIPCHART_2            5       /* control type: strip, callback function: (none) */
#define  STAPANEL_STRIPCHART_1            6       /* control type: strip, callback function: (none) */
#define  STAPANEL_CHART2_YRANGE_RING      7       /* control type: ring, callback function: StaYRangeChangeCallback */
#define  STAPANEL_CHART1_YRANGE_RING      8       /* control type: ring, callback function: StaYRangeChangeCallback */
#define  STAPANEL_CHART4_YRANGE_RING      9       /* control type: ring, callback function: StaYRangeChangeCallback */
#define  STAPANEL_CHART3_YRANGE_RING      10      /* control type: ring, callback function: StaYRangeChangeCallback */
#define  STAPANEL_STRIPCHART_4            11      /* control type: strip, callback function: (none) */
#define  STAPANEL_CHART2_CONTENT_RING     12      /* control type: ring, callback function: StaDisplayChangeCallback */
#define  STAPANEL_CHART4_CONTENT_RING     13      /* control type: ring, callback function: StaDisplayChangeCallback */
#define  STAPANEL_CHART2_CH_RING          14      /* control type: ring, callback function: TStaDisplayChangeCallback */
#define  STAPANEL_PB_SAVE                 15      /* control type: command, callback function: StaticSaveCallback */
#define  STAPANEL_PB_RECORD               16      /* control type: command, callback function: StaticRecordCallback */
#define  STAPANEL_CHART1_CH_RING          17      /* control type: ring, callback function: TStaDisplayChangeCallback */
#define  STAPANEL_CHART3_CH_RING          18      /* control type: ring, callback function: TStaDisplayChangeCallback */
#define  STAPANEL_CHART4_CH_RING          19      /* control type: ring, callback function: TStaDisplayChangeCallback */
#define  STAPANEL_STA_TAB                 20      /* control type: tab, callback function: (none) */
#define  STAPANEL_PB_EXIT                 21      /* control type: command, callback function: StaticExitCallback */
#define  STAPANEL_PB_RESET                22      /* control type: command, callback function: StaticResetCallback */
#define  STAPANEL_PB_START                23      /* control type: command, callback function: StaticStartCallback */
#define  STAPANEL_TEXTMSG_22              24      /* control type: textMsg, callback function: (none) */
#define  STAPANEL_TEXTMSG_23              25      /* control type: textMsg, callback function: (none) */
#define  STAPANEL_TEXTMSG                 26      /* control type: textMsg, callback function: (none) */
#define  STAPANEL_TEXTMSG_21              27      /* control type: textMsg, callback function: (none) */
#define  STAPANEL_COMMANDBUTTON           28      /* control type: command, callback function: StaticDataReDisplay */
#define  STAPANEL_DECORATION_9            29      /* control type: deco, callback function: (none) */
#define  STAPANEL_STRIPCHART_3            30      /* control type: strip, callback function: (none) */
#define  STAPANEL_MEnable                 31      /* control type: command, callback function: Enablecallback */
#define  STAPANEL_TIMER                   32      /* control type: timer, callback function: NetWatchDog */

     /* tab page panel controls */
#define  CH1_P_TAB_NUM_STATIC_X3          2       /* control type: numeric, callback function: (none) */
#define  CH1_P_TAB_NUM_STATIC_X2          3       /* control type: numeric, callback function: (none) */
#define  CH1_P_TAB_NUM_STATIC_X1          4       /* control type: numeric, callback function: (none) */
#define  CH1_P_TAB_NUM_STATIC_FTK         5       /* control type: numeric, callback function: (none) */
#define  CH1_P_TAB_NUM_STATIC_FT2         6       /* control type: numeric, callback function: (none) */
#define  CH1_P_TAB_NUM_STATIC_FT1         7       /* control type: numeric, callback function: (none) */
#define  CH1_P_TAB_NUM_STATIC_KTI         8       /* control type: numeric, callback function: (none) */
#define  CH1_P_TAB_NUM_STATIC_KTP         9       /* control type: numeric, callback function: (none) */
#define  CH1_P_TAB_NUM_STATIC_KTD         10      /* control type: numeric, callback function: (none) */

     /* tab page panel controls */
#define  CH1_TAB_CH_ON_OFF                2       /* control type: LED, callback function: (none) */
#define  CH1_TAB_CH_TEST_TYPE             3       /* control type: ring, callback function: StaCh1TesttypeChange */
#define  CH1_TAB_CH_START_FREQ            4       /* control type: numeric, callback function: (none) */
#define  CH1_TAB_CH_STOP_FREQ             5       /* control type: numeric, callback function: (none) */
#define  CH1_TAB_CH_OFFSET                6       /* control type: numeric, callback function: (none) */
#define  CH1_TAB_CH_FREQ                  7       /* control type: numeric, callback function: (none) */
#define  CH1_TAB_CH_AMP                   8       /* control type: numeric, callback function: (none) */
#define  CH1_TAB_TEXTMSG_20               9       /* control type: textMsg, callback function: (none) */
#define  CH1_TAB_TEXTMSG_15               10      /* control type: textMsg, callback function: (none) */
#define  CH1_TAB_TEXTMSG_16               11      /* control type: textMsg, callback function: (none) */
#define  CH1_TAB_CH_SCAN_TYPE             12      /* control type: ring, callback function: (none) */
#define  CH1_TAB_CH_WAVE                  13      /* control type: ring, callback function: (none) */
#define  CH1_TAB_TEXTMSG_17               14      /* control type: textMsg, callback function: (none) */
#define  CH1_TAB_TEXTMSG_18               15      /* control type: textMsg, callback function: (none) */

     /* tab page panel controls */
#define  CH2_P_TAB_NUM_STATIC_X3          2       /* control type: numeric, callback function: (none) */
#define  CH2_P_TAB_NUM_STATIC_X2          3       /* control type: numeric, callback function: (none) */
#define  CH2_P_TAB_NUM_STATIC_X1          4       /* control type: numeric, callback function: (none) */
#define  CH2_P_TAB_NUM_STATIC_FTK         5       /* control type: numeric, callback function: (none) */
#define  CH2_P_TAB_NUM_STATIC_FT2         6       /* control type: numeric, callback function: (none) */
#define  CH2_P_TAB_NUM_STATIC_FT1         7       /* control type: numeric, callback function: (none) */
#define  CH2_P_TAB_NUM_STATIC_KTI         8       /* control type: numeric, callback function: (none) */
#define  CH2_P_TAB_NUM_STATIC_KTP         9       /* control type: numeric, callback function: (none) */
#define  CH2_P_TAB_NUM_STATIC_KTD         10      /* control type: numeric, callback function: (none) */

     /* tab page panel controls */
#define  CH2_TAB_CH_ON_OFF                2       /* control type: LED, callback function: (none) */
#define  CH2_TAB_CH_TEST_TYPE             3       /* control type: ring, callback function: StaCh2TesttypeChange */
#define  CH2_TAB_CH_START_FREQ            4       /* control type: numeric, callback function: (none) */
#define  CH2_TAB_CH_STOP_FREQ             5       /* control type: numeric, callback function: (none) */
#define  CH2_TAB_CH_OFFSET                6       /* control type: numeric, callback function: (none) */
#define  CH2_TAB_CH_FREQ                  7       /* control type: numeric, callback function: (none) */
#define  CH2_TAB_CH_AMP                   8       /* control type: numeric, callback function: (none) */
#define  CH2_TAB_TEXTMSG_20               9       /* control type: textMsg, callback function: (none) */
#define  CH2_TAB_TEXTMSG_15               10      /* control type: textMsg, callback function: (none) */
#define  CH2_TAB_TEXTMSG_16               11      /* control type: textMsg, callback function: (none) */
#define  CH2_TAB_CH_SCAN_TYPE             12      /* control type: ring, callback function: (none) */
#define  CH2_TAB_CH_WAVE                  13      /* control type: ring, callback function: (none) */
#define  CH2_TAB_TEXTMSG_17               14      /* control type: textMsg, callback function: (none) */
#define  CH2_TAB_TEXTMSG_18               15      /* control type: textMsg, callback function: (none) */

     /* tab page panel controls */
#define  CH3_P_TAB_NUM_STATIC_X3          2       /* control type: numeric, callback function: (none) */
#define  CH3_P_TAB_NUM_STATIC_X2          3       /* control type: numeric, callback function: (none) */
#define  CH3_P_TAB_NUM_STATIC_X1          4       /* control type: numeric, callback function: (none) */
#define  CH3_P_TAB_NUM_STATIC_FTK         5       /* control type: numeric, callback function: (none) */
#define  CH3_P_TAB_NUM_STATIC_FT2         6       /* control type: numeric, callback function: (none) */
#define  CH3_P_TAB_NUM_STATIC_FT1         7       /* control type: numeric, callback function: (none) */
#define  CH3_P_TAB_NUM_STATIC_KTI         8       /* control type: numeric, callback function: (none) */
#define  CH3_P_TAB_NUM_STATIC_KTP         9       /* control type: numeric, callback function: (none) */
#define  CH3_P_TAB_NUM_STATIC_KTD         10      /* control type: numeric, callback function: (none) */

     /* tab page panel controls */
#define  CH3_TAB_CH_ON_OFF                2       /* control type: LED, callback function: (none) */
#define  CH3_TAB_CH_TEST_TYPE             3       /* control type: ring, callback function: StaCh2TesttypeChange */
#define  CH3_TAB_CH_START_FREQ            4       /* control type: numeric, callback function: (none) */
#define  CH3_TAB_CH_STOP_FREQ             5       /* control type: numeric, callback function: (none) */
#define  CH3_TAB_CH_OFFSET                6       /* control type: numeric, callback function: (none) */
#define  CH3_TAB_CH_FREQ                  7       /* control type: numeric, callback function: (none) */
#define  CH3_TAB_CH_AMP                   8       /* control type: numeric, callback function: (none) */
#define  CH3_TAB_TEXTMSG_20               9       /* control type: textMsg, callback function: (none) */
#define  CH3_TAB_TEXTMSG_15               10      /* control type: textMsg, callback function: (none) */
#define  CH3_TAB_TEXTMSG_16               11      /* control type: textMsg, callback function: (none) */
#define  CH3_TAB_CH_SCAN_TYPE             12      /* control type: ring, callback function: (none) */
#define  CH3_TAB_CH_WAVE                  13      /* control type: ring, callback function: (none) */
#define  CH3_TAB_TEXTMSG_17               14      /* control type: textMsg, callback function: (none) */
#define  CH3_TAB_TEXTMSG_18               15      /* control type: textMsg, callback function: (none) */

     /* tab page panel controls */
#define  CH4_P_TAB_NUM_STATIC_X3          2       /* control type: numeric, callback function: (none) */
#define  CH4_P_TAB_NUM_STATIC_X2          3       /* control type: numeric, callback function: (none) */
#define  CH4_P_TAB_NUM_STATIC_X1          4       /* control type: numeric, callback function: (none) */
#define  CH4_P_TAB_NUM_STATIC_FTK         5       /* control type: numeric, callback function: (none) */
#define  CH4_P_TAB_NUM_STATIC_FT2         6       /* control type: numeric, callback function: (none) */
#define  CH4_P_TAB_NUM_STATIC_FT1         7       /* control type: numeric, callback function: (none) */
#define  CH4_P_TAB_NUM_STATIC_KTI         8       /* control type: numeric, callback function: (none) */
#define  CH4_P_TAB_NUM_STATIC_KTP         9       /* control type: numeric, callback function: (none) */
#define  CH4_P_TAB_NUM_STATIC_KTD         10      /* control type: numeric, callback function: (none) */

     /* tab page panel controls */
#define  CH4_TAB_CH_ON_OFF                2       /* control type: LED, callback function: (none) */
#define  CH4_TAB_CH_TEST_TYPE             3       /* control type: ring, callback function: StaCh4TesttypeChange */
#define  CH4_TAB_CH_START_FREQ            4       /* control type: numeric, callback function: (none) */
#define  CH4_TAB_CH_STOP_FREQ             5       /* control type: numeric, callback function: (none) */
#define  CH4_TAB_CH_OFFSET                6       /* control type: numeric, callback function: (none) */
#define  CH4_TAB_CH_FREQ                  7       /* control type: numeric, callback function: (none) */
#define  CH4_TAB_CH_AMP                   8       /* control type: numeric, callback function: (none) */
#define  CH4_TAB_TEXTMSG_20               9       /* control type: textMsg, callback function: (none) */
#define  CH4_TAB_TEXTMSG_15               10      /* control type: textMsg, callback function: (none) */
#define  CH4_TAB_TEXTMSG_16               11      /* control type: textMsg, callback function: (none) */
#define  CH4_TAB_CH_SCAN_TYPE             12      /* control type: ring, callback function: (none) */
#define  CH4_TAB_CH_WAVE                  13      /* control type: ring, callback function: (none) */
#define  CH4_TAB_TEXTMSG_17               14      /* control type: textMsg, callback function: (none) */
#define  CH4_TAB_TEXTMSG_18               15      /* control type: textMsg, callback function: (none) */

     /* tab page panel controls */
#define  PARA_TAB_CTRL_PARA_TAB           2       /* control type: tab, callback function: (none) */
#define  PARA_TAB_PB_SAVE2                3       /* control type: command, callback function: StaticSaveParaCallback */
#define  PARA_TAB_PB_DOWNLOAD_PARA        4       /* control type: command, callback function: StaticDownParaCallback */
#define  PARA_TAB_PB_SAVE                 5       /* control type: command, callback function: StaticSetParaCallback */

     /* tab page panel controls */
#define  SET_TAB_CH_TAB                   2       /* control type: tab, callback function: StaTabChanelSelect */


     /* Control Arrays: */

          /* (no control arrays in the resource file) */


     /* Menu Bars, Menus, and Menu Items: */

          /* (no menu bars in the resource file) */


     /* Callback Prototypes: */

int  CVICALLBACK Enablecallback(int panel, int control, int event, void *callbackData, int eventData1, int eventData2);
int  CVICALLBACK NetWatchDog(int panel, int control, int event, void *callbackData, int eventData1, int eventData2);
int  CVICALLBACK StaCh1TesttypeChange(int panel, int control, int event, void *callbackData, int eventData1, int eventData2);
int  CVICALLBACK StaCh2TesttypeChange(int panel, int control, int event, void *callbackData, int eventData1, int eventData2);
int  CVICALLBACK StaCh4TesttypeChange(int panel, int control, int event, void *callbackData, int eventData1, int eventData2);
int  CVICALLBACK StaDisplayChangeCallback(int panel, int control, int event, void *callbackData, int eventData1, int eventData2);
int  CVICALLBACK StaTabChanelSelect(int panel, int control, int event, void *callbackData, int eventData1, int eventData2);
int  CVICALLBACK StaticDataReDisplay(int panel, int control, int event, void *callbackData, int eventData1, int eventData2);
int  CVICALLBACK StaticDownParaCallback(int panel, int control, int event, void *callbackData, int eventData1, int eventData2);
int  CVICALLBACK StaticExitCallback(int panel, int control, int event, void *callbackData, int eventData1, int eventData2);
int  CVICALLBACK StaticPanelCallback(int panel, int event, void *callbackData, int eventData1, int eventData2);
int  CVICALLBACK StaticRecordCallback(int panel, int control, int event, void *callbackData, int eventData1, int eventData2);
int  CVICALLBACK StaticResetCallback(int panel, int control, int event, void *callbackData, int eventData1, int eventData2);
int  CVICALLBACK StaticSaveCallback(int panel, int control, int event, void *callbackData, int eventData1, int eventData2);
int  CVICALLBACK StaticSaveParaCallback(int panel, int control, int event, void *callbackData, int eventData1, int eventData2);
int  CVICALLBACK StaticSetParaCallback(int panel, int control, int event, void *callbackData, int eventData1, int eventData2);
int  CVICALLBACK StaticStartCallback(int panel, int control, int event, void *callbackData, int eventData1, int eventData2);
int  CVICALLBACK StaYRangeChangeCallback(int panel, int control, int event, void *callbackData, int eventData1, int eventData2);
int  CVICALLBACK TStaDisplayChangeCallback(int panel, int control, int event, void *callbackData, int eventData1, int eventData2);


#ifdef __cplusplus
    }
#endif

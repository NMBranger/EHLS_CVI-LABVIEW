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

#define  MOTIONPNL                        1       /* callback function: MotionPanelCallback */
#define  MOTIONPNL_DECORATION_6           2       /* control type: deco, callback function: (none) */
#define  MOTIONPNL_CH_TEST_TYPE           3       /* control type: ring, callback function: MotionCh1TesttypeChange */
#define  MOTIONPNL_CH_TEST_TIME           4       /* control type: numeric, callback function: (none) */
#define  MOTIONPNL_STRIPCHART_4           5       /* control type: strip, callback function: (none) */
#define  MOTIONPNL_STRIPCHART_1           6       /* control type: strip, callback function: (none) */
#define  MOTIONPNL_CHART4_YRANGE_RING     7       /* control type: ring, callback function: MotionYRangeChangeCallback */
#define  MOTIONPNL_CHART3_YRANGE_RING     8       /* control type: ring, callback function: MotionYRangeChangeCallback */
#define  MOTIONPNL_CHART2_YRANGE_RING     9       /* control type: ring, callback function: MotionYRangeChangeCallback */
#define  MOTIONPNL_CHART1_YRANGE_RING     10      /* control type: ring, callback function: MotionYRangeChangeCallback */
#define  MOTIONPNL_MOTION_TIMER           11      /* control type: timer, callback function: NetWatchDog1 */
#define  MOTIONPNL_CHART4_CONTENT_RING    12      /* control type: ring, callback function: MotionDisplayChangeCallback */
#define  MOTIONPNL_CHART2_CONTENT_RING    13      /* control type: ring, callback function: MotionDisplayChangeCallback */
#define  MOTIONPNL_CHART4_CH_RING         14      /* control type: ring, callback function: AMotionDisplayChangeCallback */
#define  MOTIONPNL_PB_SAVE                15      /* control type: command, callback function: MotionSaveCallback */
#define  MOTIONPNL_PB_RECORD              16      /* control type: command, callback function: MotionRecordCallback */
#define  MOTIONPNL_CHART3_CH_RING         17      /* control type: ring, callback function: AMotionDisplayChangeCallback */
#define  MOTIONPNL_CHART2_CH_RING         18      /* control type: ring, callback function: AMotionDisplayChangeCallback */
#define  MOTIONPNL_CHART1_CH_RING         19      /* control type: ring, callback function: AMotionDisplayChangeCallback */
#define  MOTIONPNL_MOT_TAB                20      /* control type: tab, callback function: (none) */
#define  MOTIONPNL_DECORATION_15          21      /* control type: deco, callback function: (none) */
#define  MOTIONPNL_DECORATION_9           22      /* control type: deco, callback function: (none) */
#define  MOTIONPNL_PB_EXIT                23      /* control type: command, callback function: MotionExitCallback */
#define  MOTIONPNL_PB_RESET               24      /* control type: command, callback function: MotionResetCallback */
#define  MOTIONPNL_PB_START               25      /* control type: command, callback function: MotionStartCallback */
#define  MOTIONPNL_TEXTMSG                26      /* control type: textMsg, callback function: (none) */
#define  MOTIONPNL_TEXTMSG_22             27      /* control type: textMsg, callback function: (none) */
#define  MOTIONPNL_TEXTMSG_21             28      /* control type: textMsg, callback function: (none) */
#define  MOTIONPNL_MEnable                29      /* control type: command, callback function: MEnablecallback */
#define  MOTIONPNL_STRIPCHART_2           30      /* control type: strip, callback function: (none) */
#define  MOTIONPNL_test1                  31      /* control type: textMsg, callback function: (none) */
#define  MOTIONPNL_TEXTMSG_20             32      /* control type: textMsg, callback function: (none) */
#define  MOTIONPNL_COMMANDBUTTON          33      /* control type: command, callback function: MotionDataReDisplay */
#define  MOTIONPNL_STRIPCHART_3           34      /* control type: strip, callback function: (none) */

     /* tab page panel controls */
#define  CH1_P_TAB_NUM_MOTION_FAK         2       /* control type: numeric, callback function: (none) */
#define  CH1_P_TAB_NUM_MOTION_FA2         3       /* control type: numeric, callback function: (none) */
#define  CH1_P_TAB_NUM_MOTION_FA1         4       /* control type: numeric, callback function: (none) */
#define  CH1_P_TAB_NUM_MOTION_KAI         5       /* control type: numeric, callback function: (none) */
#define  CH1_P_TAB_NUM_MOTION_KAP         6       /* control type: numeric, callback function: (none) */
#define  CH1_P_TAB_NUM_MOTION_KAD         7       /* control type: numeric, callback function: (none) */

     /* tab page panel controls */
#define  CH1_TAB_CH_ON_OFF                2       /* control type: LED, callback function: (none) */
#define  CH1_TAB_CH_START_FREQ            3       /* control type: numeric, callback function: (none) */
#define  CH1_TAB_CH_STOP_FREQ             4       /* control type: numeric, callback function: (none) */
#define  CH1_TAB_CH_OFFSET                5       /* control type: numeric, callback function: (none) */
#define  CH1_TAB_CH_FREQ                  6       /* control type: numeric, callback function: (none) */
#define  CH1_TAB_CH_AMP                   7       /* control type: numeric, callback function: (none) */
#define  CH1_TAB_TEXTMSG_19               8       /* control type: textMsg, callback function: (none) */
#define  CH1_TAB_TEXTMSG_15               9       /* control type: textMsg, callback function: (none) */
#define  CH1_TAB_TEXTMSG_16               10      /* control type: textMsg, callback function: (none) */
#define  CH1_TAB_CH_SCAN_TYPE             11      /* control type: ring, callback function: (none) */
#define  CH1_TAB_CH_WAVE                  12      /* control type: ring, callback function: (none) */
#define  CH1_TAB_TEXTMSG_17               13      /* control type: textMsg, callback function: (none) */
#define  CH1_TAB_TEXTMSG_18               14      /* control type: textMsg, callback function: (none) */

     /* tab page panel controls */
#define  CH2_P_TAB_NUM_MOTION_FAK         2       /* control type: numeric, callback function: (none) */
#define  CH2_P_TAB_NUM_MOTION_FA2         3       /* control type: numeric, callback function: (none) */
#define  CH2_P_TAB_NUM_MOTION_FA1         4       /* control type: numeric, callback function: (none) */
#define  CH2_P_TAB_NUM_MOTION_KAI         5       /* control type: numeric, callback function: (none) */
#define  CH2_P_TAB_NUM_MOTION_KAP         6       /* control type: numeric, callback function: (none) */
#define  CH2_P_TAB_NUM_MOTION_KAD         7       /* control type: numeric, callback function: (none) */

     /* tab page panel controls */
#define  CH2_TAB_CH_ON_OFF                2       /* control type: LED, callback function: (none) */
#define  CH2_TAB_CH_START_FREQ            3       /* control type: numeric, callback function: (none) */
#define  CH2_TAB_CH_STOP_FREQ             4       /* control type: numeric, callback function: (none) */
#define  CH2_TAB_CH_OFFSET                5       /* control type: numeric, callback function: (none) */
#define  CH2_TAB_CH_FREQ                  6       /* control type: numeric, callback function: (none) */
#define  CH2_TAB_CH_AMP                   7       /* control type: numeric, callback function: (none) */
#define  CH2_TAB_TEXTMSG_19               8       /* control type: textMsg, callback function: (none) */
#define  CH2_TAB_TEXTMSG_15               9       /* control type: textMsg, callback function: (none) */
#define  CH2_TAB_TEXTMSG_16               10      /* control type: textMsg, callback function: (none) */
#define  CH2_TAB_CH_SCAN_TYPE             11      /* control type: ring, callback function: (none) */
#define  CH2_TAB_CH_WAVE                  12      /* control type: ring, callback function: (none) */
#define  CH2_TAB_TEXTMSG_17               13      /* control type: textMsg, callback function: (none) */
#define  CH2_TAB_TEXTMSG_18               14      /* control type: textMsg, callback function: (none) */

     /* tab page panel controls */
#define  CH3_P_TAB_NUM_MOTION_FAK         2       /* control type: numeric, callback function: (none) */
#define  CH3_P_TAB_NUM_MOTION_FA2         3       /* control type: numeric, callback function: (none) */
#define  CH3_P_TAB_NUM_MOTION_FA1         4       /* control type: numeric, callback function: (none) */
#define  CH3_P_TAB_NUM_MOTION_KAI         5       /* control type: numeric, callback function: (none) */
#define  CH3_P_TAB_NUM_MOTION_KAP         6       /* control type: numeric, callback function: (none) */
#define  CH3_P_TAB_NUM_MOTION_KAD         7       /* control type: numeric, callback function: (none) */

     /* tab page panel controls */
#define  CH3_TAB_CH_ON_OFF                2       /* control type: LED, callback function: (none) */
#define  CH3_TAB_CH_START_FREQ            3       /* control type: numeric, callback function: (none) */
#define  CH3_TAB_CH_STOP_FREQ             4       /* control type: numeric, callback function: (none) */
#define  CH3_TAB_CH_OFFSET                5       /* control type: numeric, callback function: (none) */
#define  CH3_TAB_CH_FREQ                  6       /* control type: numeric, callback function: (none) */
#define  CH3_TAB_CH_AMP                   7       /* control type: numeric, callback function: (none) */
#define  CH3_TAB_TEXTMSG_19               8       /* control type: textMsg, callback function: (none) */
#define  CH3_TAB_TEXTMSG_15               9       /* control type: textMsg, callback function: (none) */
#define  CH3_TAB_TEXTMSG_16               10      /* control type: textMsg, callback function: (none) */
#define  CH3_TAB_CH_SCAN_TYPE             11      /* control type: ring, callback function: (none) */
#define  CH3_TAB_CH_WAVE                  12      /* control type: ring, callback function: (none) */
#define  CH3_TAB_TEXTMSG_17               13      /* control type: textMsg, callback function: (none) */
#define  CH3_TAB_TEXTMSG_18               14      /* control type: textMsg, callback function: (none) */

     /* tab page panel controls */
#define  CH4_P_TAB_NUM_MOTION_FAK         2       /* control type: numeric, callback function: (none) */
#define  CH4_P_TAB_NUM_MOTION_FA2         3       /* control type: numeric, callback function: (none) */
#define  CH4_P_TAB_NUM_MOTION_FA1         4       /* control type: numeric, callback function: (none) */
#define  CH4_P_TAB_NUM_MOTION_KAI         5       /* control type: numeric, callback function: (none) */
#define  CH4_P_TAB_NUM_MOTION_KAP         6       /* control type: numeric, callback function: (none) */
#define  CH4_P_TAB_NUM_MOTION_KAD         7       /* control type: numeric, callback function: (none) */

     /* tab page panel controls */
#define  CH4_TAB_CH_ON_OFF                2       /* control type: LED, callback function: (none) */
#define  CH4_TAB_CH_START_FREQ            3       /* control type: numeric, callback function: (none) */
#define  CH4_TAB_CH_STOP_FREQ             4       /* control type: numeric, callback function: (none) */
#define  CH4_TAB_CH_OFFSET                5       /* control type: numeric, callback function: (none) */
#define  CH4_TAB_CH_FREQ                  6       /* control type: numeric, callback function: (none) */
#define  CH4_TAB_CH_AMP                   7       /* control type: numeric, callback function: (none) */
#define  CH4_TAB_TEXTMSG_19               8       /* control type: textMsg, callback function: (none) */
#define  CH4_TAB_TEXTMSG_15               9       /* control type: textMsg, callback function: (none) */
#define  CH4_TAB_TEXTMSG_16               10      /* control type: textMsg, callback function: (none) */
#define  CH4_TAB_CH_SCAN_TYPE             11      /* control type: ring, callback function: (none) */
#define  CH4_TAB_CH_WAVE                  12      /* control type: ring, callback function: (none) */
#define  CH4_TAB_TEXTMSG_17               13      /* control type: textMsg, callback function: (none) */
#define  CH4_TAB_TEXTMSG_18               14      /* control type: textMsg, callback function: (none) */

     /* tab page panel controls */
#define  PARA_TAB_CTRL_PARA_TAB           2       /* control type: tab, callback function: (none) */
#define  PARA_TAB_PB_SAVE2                3       /* control type: command, callback function: MotionSaveParaCallback */
#define  PARA_TAB_PB_DOWNLOAD_PARA        4       /* control type: command, callback function: MotionDownParaCallback */
#define  PARA_TAB_PB_SAVE                 5       /* control type: command, callback function: MotionSetParaCallback */

     /* tab page panel controls */
#define  SET_TAB_CH_TAB                   2       /* control type: tab, callback function: MotionTabChanelSelect */

     /* tab page panel controls */
#define  TABPANEL_CH_ON_OFF               2       /* control type: LED, callback function: (none) */
#define  TABPANEL_CH_START_FREQ           3       /* control type: numeric, callback function: (none) */
#define  TABPANEL_CH_STOP_FREQ            4       /* control type: numeric, callback function: (none) */
#define  TABPANEL_CH_OFFSET               5       /* control type: numeric, callback function: (none) */
#define  TABPANEL_CH_FREQ                 6       /* control type: numeric, callback function: (none) */
#define  TABPANEL_CH_AMP                  7       /* control type: numeric, callback function: (none) */
#define  TABPANEL_TEXTMSG_19              8       /* control type: textMsg, callback function: (none) */
#define  TABPANEL_TEXTMSG_15              9       /* control type: textMsg, callback function: (none) */
#define  TABPANEL_TEXTMSG_16              10      /* control type: textMsg, callback function: (none) */
#define  TABPANEL_CH_SCAN_TYPE            11      /* control type: ring, callback function: (none) */
#define  TABPANEL_CH_WAVE                 12      /* control type: ring, callback function: (none) */
#define  TABPANEL_TEXTMSG_17              13      /* control type: textMsg, callback function: (none) */
#define  TABPANEL_TEXTMSG_18              14      /* control type: textMsg, callback function: (none) */

     /* tab page panel controls */
#define  TABPANEL_6_NUM_MOTION_FAK        2       /* control type: numeric, callback function: (none) */
#define  TABPANEL_6_NUM_MOTION_FA2        3       /* control type: numeric, callback function: (none) */
#define  TABPANEL_6_NUM_MOTION_FA1        4       /* control type: numeric, callback function: (none) */
#define  TABPANEL_6_NUM_MOTION_KAI        5       /* control type: numeric, callback function: (none) */
#define  TABPANEL_6_NUM_MOTION_KAP        6       /* control type: numeric, callback function: (none) */
#define  TABPANEL_6_NUM_MOTION_KAD        7       /* control type: numeric, callback function: (none) */


     /* Control Arrays: */

#define  CTRLARRAY                        1

     /* Menu Bars, Menus, and Menu Items: */

          /* (no menu bars in the resource file) */


     /* Callback Prototypes: */

int  CVICALLBACK AMotionDisplayChangeCallback(int panel, int control, int event, void *callbackData, int eventData1, int eventData2);
int  CVICALLBACK MEnablecallback(int panel, int control, int event, void *callbackData, int eventData1, int eventData2);
int  CVICALLBACK MotionCh1TesttypeChange(int panel, int control, int event, void *callbackData, int eventData1, int eventData2);
int  CVICALLBACK MotionDataReDisplay(int panel, int control, int event, void *callbackData, int eventData1, int eventData2);
int  CVICALLBACK MotionDisplayChangeCallback(int panel, int control, int event, void *callbackData, int eventData1, int eventData2);
int  CVICALLBACK MotionDownParaCallback(int panel, int control, int event, void *callbackData, int eventData1, int eventData2);
int  CVICALLBACK MotionExitCallback(int panel, int control, int event, void *callbackData, int eventData1, int eventData2);
int  CVICALLBACK MotionPanelCallback(int panel, int event, void *callbackData, int eventData1, int eventData2);
int  CVICALLBACK MotionRecordCallback(int panel, int control, int event, void *callbackData, int eventData1, int eventData2);
int  CVICALLBACK MotionResetCallback(int panel, int control, int event, void *callbackData, int eventData1, int eventData2);
int  CVICALLBACK MotionSaveCallback(int panel, int control, int event, void *callbackData, int eventData1, int eventData2);
int  CVICALLBACK MotionSaveParaCallback(int panel, int control, int event, void *callbackData, int eventData1, int eventData2);
int  CVICALLBACK MotionSetParaCallback(int panel, int control, int event, void *callbackData, int eventData1, int eventData2);
int  CVICALLBACK MotionStartCallback(int panel, int control, int event, void *callbackData, int eventData1, int eventData2);
int  CVICALLBACK MotionTabChanelSelect(int panel, int control, int event, void *callbackData, int eventData1, int eventData2);
int  CVICALLBACK MotionYRangeChangeCallback(int panel, int control, int event, void *callbackData, int eventData1, int eventData2);
int  CVICALLBACK NetWatchDog1(int panel, int control, int event, void *callbackData, int eventData1, int eventData2);


#ifdef __cplusplus
    }
#endif

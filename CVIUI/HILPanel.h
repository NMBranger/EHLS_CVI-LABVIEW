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

#define  HILPNL                           1       /* callback function: HILPanelCallback */
#define  HILPNL_TEXTMSG_8                 2       /* control type: textMsg, callback function: (none) */
#define  HILPNL_CH_TEST_TYPE              3       /* control type: ring, callback function: HILChTesttypeChange */
#define  HILPNL_CH_TEST_TIME              4       /* control type: numeric, callback function: (none) */
#define  HILPNL_STRIPCHART_2              5       /* control type: strip, callback function: (none) */
#define  HILPNL_STRIPCHART_1              6       /* control type: strip, callback function: (none) */
#define  HILPNL_STRIPCHART_3              7       /* control type: strip, callback function: (none) */
#define  HILPNL_CHART2_YRANGE_RING        8       /* control type: ring, callback function: HILYRangeChangeCallback */
#define  HILPNL_CHART1_YRANGE_RING        9       /* control type: ring, callback function: HILYRangeChangeCallback */
#define  HILPNL_CHART4_YRANGE_RING        10      /* control type: ring, callback function: HILYRangeChangeCallback */
#define  HILPNL_CHART2_CONTENT_RING       11      /* control type: ring, callback function: HILDisplayChangeCallback */
#define  HILPNL_CHART4_CONTENT_RING       12      /* control type: ring, callback function: HILDisplayChangeCallback */
#define  HILPNL_CHART3_YRANGE_RING        13      /* control type: ring, callback function: HILYRangeChangeCallback */
#define  HILPNL_PB_SAVE                   14      /* control type: command, callback function: HILSaveCallback */
#define  HILPNL_PB_RECORD                 15      /* control type: command, callback function: HILRecordCallback */
#define  HILPNL_CHART1_CH_RING            16      /* control type: ring, callback function: AHILDisplayChangeCallback */
#define  HILPNL_CHART2_CH_RING            17      /* control type: ring, callback function: AHILDisplayChangeCallback */
#define  HILPNL_CHART4_CH_RING            18      /* control type: ring, callback function: AHILDisplayChangeCallback */
#define  HILPNL_CHART3_CH_RING            19      /* control type: ring, callback function: AHILDisplayChangeCallback */
#define  HILPNL_STRIPCHART_4              20      /* control type: strip, callback function: (none) */
#define  HILPNL_HIL_TIMER                 21      /* control type: timer, callback function: NetWatchDog3 */
#define  HILPNL_TAB                       22      /* control type: tab, callback function: (none) */
#define  HILPNL_DECORATION_15             23      /* control type: deco, callback function: (none) */
#define  HILPNL_DECORATION_9              24      /* control type: deco, callback function: (none) */
#define  HILPNL_PB_EXIT                   25      /* control type: command, callback function: HILExitCallback */
#define  HILPNL_PB_RESET                  26      /* control type: command, callback function: HILResetCallback */
#define  HILPNL_PB_START                  27      /* control type: command, callback function: HILStartCallback */
#define  HILPNL_COMMANDBUTTON_2           28      /* control type: command, callback function: HILOliDisplay */
#define  HILPNL_COMMANDBUTTON             29      /* control type: command, callback function: HILDataReDisplay */
#define  HILPNL_NUMERIC_5                 30      /* control type: numeric, callback function: (none) */
#define  HILPNL_NUMERIC_6                 31      /* control type: numeric, callback function: (none) */
#define  HILPNL_NUMERIC_7                 32      /* control type: numeric, callback function: (none) */
#define  HILPNL_NUMERIC_8                 33      /* control type: numeric, callback function: (none) */
#define  HILPNL_NUMERIC_9                 34      /* control type: numeric, callback function: (none) */
#define  HILPNL_NUMERIC_4                 35      /* control type: numeric, callback function: (none) */
#define  HILPNL_NUMERIC_3                 36      /* control type: numeric, callback function: (none) */
#define  HILPNL_NUMERIC                   37      /* control type: numeric, callback function: (none) */
#define  HILPNL_NUMERIC_2                 38      /* control type: numeric, callback function: (none) */

     /* tab page panel controls */
#define  AXICH1_TAB_CH_ON_OFF             2       /* control type: LED, callback function: (none) */
#define  AXICH1_TAB_CH_START_FREQ         3       /* control type: numeric, callback function: (none) */
#define  AXICH1_TAB_CH_STOP_FREQ          4       /* control type: numeric, callback function: (none) */
#define  AXICH1_TAB_CH_FREQ               5       /* control type: numeric, callback function: (none) */
#define  AXICH1_TAB_CH_OFFSET             6       /* control type: numeric, callback function: (none) */
#define  AXICH1_TAB_CH_AMP                7       /* control type: numeric, callback function: (none) */
#define  AXICH1_TAB_TEXTMSG_19            8       /* control type: textMsg, callback function: (none) */
#define  AXICH1_TAB_TEXTMSG_15            9       /* control type: textMsg, callback function: (none) */
#define  AXICH1_TAB_TEXTMSG_16            10      /* control type: textMsg, callback function: (none) */
#define  AXICH1_TAB_CH_WAVE               11      /* control type: ring, callback function: (none) */
#define  AXICH1_TAB_DECORATION            12      /* control type: deco, callback function: (none) */
#define  AXICH1_TAB_TEXTMSG_17            13      /* control type: textMsg, callback function: (none) */
#define  AXICH1_TAB_TEXTMSG_18            14      /* control type: textMsg, callback function: (none) */
#define  AXICH1_TAB_TEXTMSG               15      /* control type: textMsg, callback function: (none) */

     /* tab page panel controls */
#define  AXICH2_TAB_CH_ON_OFF             2       /* control type: LED, callback function: (none) */
#define  AXICH2_TAB_CH_START_FREQ         3       /* control type: numeric, callback function: (none) */
#define  AXICH2_TAB_CH_STOP_FREQ          4       /* control type: numeric, callback function: (none) */
#define  AXICH2_TAB_CH_FREQ               5       /* control type: numeric, callback function: (none) */
#define  AXICH2_TAB_CH_OFFSET             6       /* control type: numeric, callback function: (none) */
#define  AXICH2_TAB_CH_AMP                7       /* control type: numeric, callback function: (none) */
#define  AXICH2_TAB_TEXTMSG_19            8       /* control type: textMsg, callback function: (none) */
#define  AXICH2_TAB_TEXTMSG_15            9       /* control type: textMsg, callback function: (none) */
#define  AXICH2_TAB_TEXTMSG_16            10      /* control type: textMsg, callback function: (none) */
#define  AXICH2_TAB_CH_WAVE               11      /* control type: ring, callback function: (none) */
#define  AXICH2_TAB_DECORATION            12      /* control type: deco, callback function: (none) */
#define  AXICH2_TAB_TEXTMSG_17            13      /* control type: textMsg, callback function: (none) */
#define  AXICH2_TAB_TEXTMSG_18            14      /* control type: textMsg, callback function: (none) */
#define  AXICH2_TAB_TEXTMSG               15      /* control type: textMsg, callback function: (none) */

     /* tab page panel controls */
#define  AXICH3_TAB_CH_ON_OFF             2       /* control type: LED, callback function: (none) */
#define  AXICH3_TAB_CH_START_FREQ         3       /* control type: numeric, callback function: (none) */
#define  AXICH3_TAB_CH_STOP_FREQ          4       /* control type: numeric, callback function: (none) */
#define  AXICH3_TAB_CH_FREQ               5       /* control type: numeric, callback function: (none) */
#define  AXICH3_TAB_CH_OFFSET             6       /* control type: numeric, callback function: (none) */
#define  AXICH3_TAB_CH_AMP                7       /* control type: numeric, callback function: (none) */
#define  AXICH3_TAB_TEXTMSG_19            8       /* control type: textMsg, callback function: (none) */
#define  AXICH3_TAB_TEXTMSG_15            9       /* control type: textMsg, callback function: (none) */
#define  AXICH3_TAB_TEXTMSG_16            10      /* control type: textMsg, callback function: (none) */
#define  AXICH3_TAB_CH_WAVE               11      /* control type: ring, callback function: (none) */
#define  AXICH3_TAB_DECORATION            12      /* control type: deco, callback function: (none) */
#define  AXICH3_TAB_TEXTMSG_17            13      /* control type: textMsg, callback function: (none) */
#define  AXICH3_TAB_TEXTMSG_18            14      /* control type: textMsg, callback function: (none) */
#define  AXICH3_TAB_TEXTMSG               15      /* control type: textMsg, callback function: (none) */

     /* tab page panel controls */
#define  AXICH4_TAB_CH_ON_OFF             2       /* control type: LED, callback function: (none) */
#define  AXICH4_TAB_CH_START_FREQ         3       /* control type: numeric, callback function: (none) */
#define  AXICH4_TAB_CH_STOP_FREQ          4       /* control type: numeric, callback function: (none) */
#define  AXICH4_TAB_CH_FREQ               5       /* control type: numeric, callback function: (none) */
#define  AXICH4_TAB_CH_OFFSET             6       /* control type: numeric, callback function: (none) */
#define  AXICH4_TAB_CH_AMP                7       /* control type: numeric, callback function: (none) */
#define  AXICH4_TAB_TEXTMSG_19            8       /* control type: textMsg, callback function: (none) */
#define  AXICH4_TAB_TEXTMSG_15            9       /* control type: textMsg, callback function: (none) */
#define  AXICH4_TAB_TEXTMSG_16            10      /* control type: textMsg, callback function: (none) */
#define  AXICH4_TAB_CH_WAVE               11      /* control type: ring, callback function: (none) */
#define  AXICH4_TAB_DECORATION            12      /* control type: deco, callback function: (none) */
#define  AXICH4_TAB_TEXTMSG_17            13      /* control type: textMsg, callback function: (none) */
#define  AXICH4_TAB_TEXTMSG_18            14      /* control type: textMsg, callback function: (none) */
#define  AXICH4_TAB_TEXTMSG               15      /* control type: textMsg, callback function: (none) */

     /* tab page panel controls */
#define  CH1_A_TAB_NUM_HIL_FTK            2       /* control type: numeric, callback function: (none) */
#define  CH1_A_TAB_NUM_HIL_FT2            3       /* control type: numeric, callback function: (none) */
#define  CH1_A_TAB_NUM_HIL_FT1            4       /* control type: numeric, callback function: (none) */
#define  CH1_A_TAB_NUM_HIL_KTI            5       /* control type: numeric, callback function: (none) */
#define  CH1_A_TAB_NUM_HIL_KTP            6       /* control type: numeric, callback function: (none) */
#define  CH1_A_TAB_NUM_HIL_KTD            7       /* control type: numeric, callback function: (none) */

     /* tab page panel controls */
#define  CH1_P_TAB_NUM_HIL_X3             2       /* control type: numeric, callback function: (none) */
#define  CH1_P_TAB_NUM_HIL_X2             3       /* control type: numeric, callback function: (none) */
#define  CH1_P_TAB_NUM_HIL_X1             4       /* control type: numeric, callback function: (none) */
#define  CH1_P_TAB_NUM_HIL_TED            5       /* control type: numeric, callback function: (none) */
#define  CH1_P_TAB_NUM_HIL_KED            6       /* control type: numeric, callback function: (none) */
#define  CH1_P_TAB_NUM_HIL_KEP            7       /* control type: numeric, callback function: (none) */
#define  CH1_P_TAB_NUM_HIL_FTK            8       /* control type: numeric, callback function: (none) */
#define  CH1_P_TAB_NUM_HIL_FT2            9       /* control type: numeric, callback function: (none) */
#define  CH1_P_TAB_NUM_HIL_FT1            10      /* control type: numeric, callback function: (none) */
#define  CH1_P_TAB_NUM_HIL_KTI            11      /* control type: numeric, callback function: (none) */
#define  CH1_P_TAB_NUM_HIL_KTP            12      /* control type: numeric, callback function: (none) */
#define  CH1_P_TAB_NUM_HIL_KTD            13      /* control type: numeric, callback function: (none) */

     /* tab page panel controls */
#define  CH1_TAB_CH_ON_OFF                2       /* control type: LED, callback function: (none) */
#define  CH1_TAB_CH_START_FREQ            3       /* control type: numeric, callback function: (none) */
#define  CH1_TAB_CH_STOP_FREQ             4       /* control type: numeric, callback function: (none) */
#define  CH1_TAB_CH_FREQ                  5       /* control type: numeric, callback function: (none) */
#define  CH1_TAB_CH_OFFSET                6       /* control type: numeric, callback function: (none) */
#define  CH1_TAB_CH_AMP                   7       /* control type: numeric, callback function: (none) */
#define  CH1_TAB_TEXTMSG_19               8       /* control type: textMsg, callback function: (none) */
#define  CH1_TAB_TEXTMSG_15               9       /* control type: textMsg, callback function: (none) */
#define  CH1_TAB_TEXTMSG_16               10      /* control type: textMsg, callback function: (none) */
#define  CH1_TAB_CH_WAVE                  11      /* control type: ring, callback function: (none) */
#define  CH1_TAB_DECORATION               12      /* control type: deco, callback function: (none) */
#define  CH1_TAB_TEXTMSG_17               13      /* control type: textMsg, callback function: (none) */
#define  CH1_TAB_TEXTMSG_18               14      /* control type: textMsg, callback function: (none) */
#define  CH1_TAB_TEXTMSG                  15      /* control type: textMsg, callback function: (none) */

     /* tab page panel controls */
#define  CH2_A_TAB_NUM_HIL_FTK            2       /* control type: numeric, callback function: (none) */
#define  CH2_A_TAB_NUM_HIL_FT2            3       /* control type: numeric, callback function: (none) */
#define  CH2_A_TAB_NUM_HIL_FT1            4       /* control type: numeric, callback function: (none) */
#define  CH2_A_TAB_NUM_HIL_KTI            5       /* control type: numeric, callback function: (none) */
#define  CH2_A_TAB_NUM_HIL_KTP            6       /* control type: numeric, callback function: (none) */
#define  CH2_A_TAB_NUM_HIL_KTD            7       /* control type: numeric, callback function: (none) */

     /* tab page panel controls */
#define  CH2_P_TAB_NUM_HIL_X3             2       /* control type: numeric, callback function: (none) */
#define  CH2_P_TAB_NUM_HIL_X2             3       /* control type: numeric, callback function: (none) */
#define  CH2_P_TAB_NUM_HIL_X1             4       /* control type: numeric, callback function: (none) */
#define  CH2_P_TAB_NUM_HIL_TED            5       /* control type: numeric, callback function: (none) */
#define  CH2_P_TAB_NUM_HIL_KED            6       /* control type: numeric, callback function: (none) */
#define  CH2_P_TAB_NUM_HIL_KEP            7       /* control type: numeric, callback function: (none) */
#define  CH2_P_TAB_NUM_HIL_FTK            8       /* control type: numeric, callback function: (none) */
#define  CH2_P_TAB_NUM_HIL_FT2            9       /* control type: numeric, callback function: (none) */
#define  CH2_P_TAB_NUM_HIL_FT1            10      /* control type: numeric, callback function: (none) */
#define  CH2_P_TAB_NUM_HIL_KTI            11      /* control type: numeric, callback function: (none) */
#define  CH2_P_TAB_NUM_HIL_KTP            12      /* control type: numeric, callback function: (none) */
#define  CH2_P_TAB_NUM_HIL_KTD            13      /* control type: numeric, callback function: (none) */

     /* tab page panel controls */
#define  CH2_TAB_CH_ON_OFF                2       /* control type: LED, callback function: (none) */
#define  CH2_TAB_CH_START_FREQ            3       /* control type: numeric, callback function: (none) */
#define  CH2_TAB_CH_STOP_FREQ             4       /* control type: numeric, callback function: (none) */
#define  CH2_TAB_CH_FREQ                  5       /* control type: numeric, callback function: (none) */
#define  CH2_TAB_CH_OFFSET                6       /* control type: numeric, callback function: (none) */
#define  CH2_TAB_CH_AMP                   7       /* control type: numeric, callback function: (none) */
#define  CH2_TAB_TEXTMSG_19               8       /* control type: textMsg, callback function: (none) */
#define  CH2_TAB_TEXTMSG_15               9       /* control type: textMsg, callback function: (none) */
#define  CH2_TAB_TEXTMSG_16               10      /* control type: textMsg, callback function: (none) */
#define  CH2_TAB_CH_WAVE                  11      /* control type: ring, callback function: (none) */
#define  CH2_TAB_DECORATION               12      /* control type: deco, callback function: (none) */
#define  CH2_TAB_TEXTMSG_17               13      /* control type: textMsg, callback function: (none) */
#define  CH2_TAB_TEXTMSG_18               14      /* control type: textMsg, callback function: (none) */
#define  CH2_TAB_TEXTMSG                  15      /* control type: textMsg, callback function: (none) */

     /* tab page panel controls */
#define  CH3_A_TAB_NUM_HIL_FTK            2       /* control type: numeric, callback function: (none) */
#define  CH3_A_TAB_NUM_HIL_FT2            3       /* control type: numeric, callback function: (none) */
#define  CH3_A_TAB_NUM_HIL_FT1            4       /* control type: numeric, callback function: (none) */
#define  CH3_A_TAB_NUM_HIL_KTI            5       /* control type: numeric, callback function: (none) */
#define  CH3_A_TAB_NUM_HIL_KTP            6       /* control type: numeric, callback function: (none) */
#define  CH3_A_TAB_NUM_HIL_KTD            7       /* control type: numeric, callback function: (none) */

     /* tab page panel controls */
#define  CH3_P_TAB_NUM_HIL_X3             2       /* control type: numeric, callback function: (none) */
#define  CH3_P_TAB_NUM_HIL_X2             3       /* control type: numeric, callback function: (none) */
#define  CH3_P_TAB_NUM_HIL_X1             4       /* control type: numeric, callback function: (none) */
#define  CH3_P_TAB_NUM_HIL_TED            5       /* control type: numeric, callback function: (none) */
#define  CH3_P_TAB_NUM_HIL_KED            6       /* control type: numeric, callback function: (none) */
#define  CH3_P_TAB_NUM_HIL_KEP            7       /* control type: numeric, callback function: (none) */
#define  CH3_P_TAB_NUM_HIL_FTK            8       /* control type: numeric, callback function: (none) */
#define  CH3_P_TAB_NUM_HIL_FT2            9       /* control type: numeric, callback function: (none) */
#define  CH3_P_TAB_NUM_HIL_FT1            10      /* control type: numeric, callback function: (none) */
#define  CH3_P_TAB_NUM_HIL_KTI            11      /* control type: numeric, callback function: (none) */
#define  CH3_P_TAB_NUM_HIL_KTP            12      /* control type: numeric, callback function: (none) */
#define  CH3_P_TAB_NUM_HIL_KTD            13      /* control type: numeric, callback function: (none) */

     /* tab page panel controls */
#define  CH3_TAB_CH_ON_OFF                2       /* control type: LED, callback function: (none) */
#define  CH3_TAB_CH_START_FREQ            3       /* control type: numeric, callback function: (none) */
#define  CH3_TAB_CH_STOP_FREQ             4       /* control type: numeric, callback function: (none) */
#define  CH3_TAB_CH_FREQ                  5       /* control type: numeric, callback function: (none) */
#define  CH3_TAB_CH_OFFSET                6       /* control type: numeric, callback function: (none) */
#define  CH3_TAB_CH_AMP                   7       /* control type: numeric, callback function: (none) */
#define  CH3_TAB_TEXTMSG_19               8       /* control type: textMsg, callback function: (none) */
#define  CH3_TAB_TEXTMSG_15               9       /* control type: textMsg, callback function: (none) */
#define  CH3_TAB_TEXTMSG_16               10      /* control type: textMsg, callback function: (none) */
#define  CH3_TAB_CH_WAVE                  11      /* control type: ring, callback function: (none) */
#define  CH3_TAB_DECORATION               12      /* control type: deco, callback function: (none) */
#define  CH3_TAB_TEXTMSG_17               13      /* control type: textMsg, callback function: (none) */
#define  CH3_TAB_TEXTMSG_18               14      /* control type: textMsg, callback function: (none) */
#define  CH3_TAB_TEXTMSG                  15      /* control type: textMsg, callback function: (none) */

     /* tab page panel controls */
#define  CH4_A_TAB_NUM_HIL_FTK            2       /* control type: numeric, callback function: (none) */
#define  CH4_A_TAB_NUM_HIL_FT2            3       /* control type: numeric, callback function: (none) */
#define  CH4_A_TAB_NUM_HIL_FT1            4       /* control type: numeric, callback function: (none) */
#define  CH4_A_TAB_NUM_HIL_KTI            5       /* control type: numeric, callback function: (none) */
#define  CH4_A_TAB_NUM_HIL_KTP            6       /* control type: numeric, callback function: (none) */
#define  CH4_A_TAB_NUM_HIL_KTD            7       /* control type: numeric, callback function: (none) */

     /* tab page panel controls */
#define  CH4_P_TAB_NUM_HIL_X3             2       /* control type: numeric, callback function: (none) */
#define  CH4_P_TAB_NUM_HIL_X2             3       /* control type: numeric, callback function: (none) */
#define  CH4_P_TAB_NUM_HIL_X1             4       /* control type: numeric, callback function: (none) */
#define  CH4_P_TAB_NUM_HIL_TED            5       /* control type: numeric, callback function: (none) */
#define  CH4_P_TAB_NUM_HIL_KED            6       /* control type: numeric, callback function: (none) */
#define  CH4_P_TAB_NUM_HIL_KEP            7       /* control type: numeric, callback function: (none) */
#define  CH4_P_TAB_NUM_HIL_FTK            8       /* control type: numeric, callback function: (none) */
#define  CH4_P_TAB_NUM_HIL_FT2            9       /* control type: numeric, callback function: (none) */
#define  CH4_P_TAB_NUM_HIL_FT1            10      /* control type: numeric, callback function: (none) */
#define  CH4_P_TAB_NUM_HIL_KTI            11      /* control type: numeric, callback function: (none) */
#define  CH4_P_TAB_NUM_HIL_KTP            12      /* control type: numeric, callback function: (none) */
#define  CH4_P_TAB_NUM_HIL_KTD            13      /* control type: numeric, callback function: (none) */

     /* tab page panel controls */
#define  CH4_TAB_CH_ON_OFF                2       /* control type: LED, callback function: (none) */
#define  CH4_TAB_CH_START_FREQ            3       /* control type: numeric, callback function: (none) */
#define  CH4_TAB_CH_STOP_FREQ             4       /* control type: numeric, callback function: (none) */
#define  CH4_TAB_CH_FREQ                  5       /* control type: numeric, callback function: (none) */
#define  CH4_TAB_CH_OFFSET                6       /* control type: numeric, callback function: (none) */
#define  CH4_TAB_CH_AMP                   7       /* control type: numeric, callback function: (none) */
#define  CH4_TAB_TEXTMSG_19               8       /* control type: textMsg, callback function: (none) */
#define  CH4_TAB_TEXTMSG_15               9       /* control type: textMsg, callback function: (none) */
#define  CH4_TAB_TEXTMSG_16               10      /* control type: textMsg, callback function: (none) */
#define  CH4_TAB_CH_WAVE                  11      /* control type: ring, callback function: (none) */
#define  CH4_TAB_DECORATION               12      /* control type: deco, callback function: (none) */
#define  CH4_TAB_TEXTMSG_17               13      /* control type: textMsg, callback function: (none) */
#define  CH4_TAB_TEXTMSG_18               14      /* control type: textMsg, callback function: (none) */
#define  CH4_TAB_TEXTMSG                  15      /* control type: textMsg, callback function: (none) */

     /* tab page panel controls */
#define  CH5_P_TAB_NUM_HIL_X3             2       /* control type: numeric, callback function: (none) */
#define  CH5_P_TAB_NUM_HIL_X2             3       /* control type: numeric, callback function: (none) */
#define  CH5_P_TAB_NUM_HIL_X1             4       /* control type: numeric, callback function: (none) */
#define  CH5_P_TAB_NUM_HIL_TED            5       /* control type: numeric, callback function: (none) */
#define  CH5_P_TAB_NUM_HIL_KED            6       /* control type: numeric, callback function: (none) */
#define  CH5_P_TAB_NUM_HIL_KEP            7       /* control type: numeric, callback function: (none) */
#define  CH5_P_TAB_NUM_HIL_FTK            8       /* control type: numeric, callback function: (none) */
#define  CH5_P_TAB_NUM_HIL_FT2            9       /* control type: numeric, callback function: (none) */
#define  CH5_P_TAB_NUM_HIL_FT1            10      /* control type: numeric, callback function: (none) */
#define  CH5_P_TAB_NUM_HIL_KTI            11      /* control type: numeric, callback function: (none) */
#define  CH5_P_TAB_NUM_HIL_KTP            12      /* control type: numeric, callback function: (none) */
#define  CH5_P_TAB_NUM_HIL_KTD            13      /* control type: numeric, callback function: (none) */

     /* tab page panel controls */
#define  CH5_TAB_CH_ON_OFF                2       /* control type: LED, callback function: (none) */
#define  CH5_TAB_CH_START_FREQ            3       /* control type: numeric, callback function: (none) */
#define  CH5_TAB_CH_STOP_FREQ             4       /* control type: numeric, callback function: (none) */
#define  CH5_TAB_CH_FREQ                  5       /* control type: numeric, callback function: (none) */
#define  CH5_TAB_CH_OFFSET                6       /* control type: numeric, callback function: (none) */
#define  CH5_TAB_CH_AMP                   7       /* control type: numeric, callback function: (none) */
#define  CH5_TAB_TEXTMSG_19               8       /* control type: textMsg, callback function: (none) */
#define  CH5_TAB_TEXTMSG_15               9       /* control type: textMsg, callback function: (none) */
#define  CH5_TAB_TEXTMSG_16               10      /* control type: textMsg, callback function: (none) */
#define  CH5_TAB_CH_WAVE                  11      /* control type: ring, callback function: (none) */
#define  CH5_TAB_DECORATION               12      /* control type: deco, callback function: (none) */
#define  CH5_TAB_TEXTMSG_17               13      /* control type: textMsg, callback function: (none) */
#define  CH5_TAB_TEXTMSG_18               14      /* control type: textMsg, callback function: (none) */
#define  CH5_TAB_TEXTMSG                  15      /* control type: textMsg, callback function: (none) */

     /* tab page panel controls */
#define  PARA_TAB_CTRL_PARA_TAB           2       /* control type: tab, callback function: HILParaTabChanelSelect */
#define  PARA_TAB_PB_SAVE2                3       /* control type: command, callback function: HILSaveParaCallback */
#define  PARA_TAB_PB_DOWNLOAD_PARA        4       /* control type: command, callback function: HILDownParaCallback */
#define  PARA_TAB_PB_SAVE                 5       /* control type: command, callback function: HILSetParaCallback */

     /* tab page panel controls */
#define  SET_TAB_CH_TAB                   2       /* control type: tab, callback function: HILTabChanelSelect */


     /* Control Arrays: */

          /* (no control arrays in the resource file) */


     /* Menu Bars, Menus, and Menu Items: */

          /* (no menu bars in the resource file) */


     /* Callback Prototypes: */

int  CVICALLBACK AHILDisplayChangeCallback(int panel, int control, int event, void *callbackData, int eventData1, int eventData2);
int  CVICALLBACK HILChTesttypeChange(int panel, int control, int event, void *callbackData, int eventData1, int eventData2);
int  CVICALLBACK HILDataReDisplay(int panel, int control, int event, void *callbackData, int eventData1, int eventData2);
int  CVICALLBACK HILDisplayChangeCallback(int panel, int control, int event, void *callbackData, int eventData1, int eventData2);
int  CVICALLBACK HILDownParaCallback(int panel, int control, int event, void *callbackData, int eventData1, int eventData2);
int  CVICALLBACK HILExitCallback(int panel, int control, int event, void *callbackData, int eventData1, int eventData2);
int  CVICALLBACK HILOliDisplay(int panel, int control, int event, void *callbackData, int eventData1, int eventData2);
int  CVICALLBACK HILPanelCallback(int panel, int event, void *callbackData, int eventData1, int eventData2);
int  CVICALLBACK HILParaTabChanelSelect(int panel, int control, int event, void *callbackData, int eventData1, int eventData2);
int  CVICALLBACK HILRecordCallback(int panel, int control, int event, void *callbackData, int eventData1, int eventData2);
int  CVICALLBACK HILResetCallback(int panel, int control, int event, void *callbackData, int eventData1, int eventData2);
int  CVICALLBACK HILSaveCallback(int panel, int control, int event, void *callbackData, int eventData1, int eventData2);
int  CVICALLBACK HILSaveParaCallback(int panel, int control, int event, void *callbackData, int eventData1, int eventData2);
int  CVICALLBACK HILSetParaCallback(int panel, int control, int event, void *callbackData, int eventData1, int eventData2);
int  CVICALLBACK HILStartCallback(int panel, int control, int event, void *callbackData, int eventData1, int eventData2);
int  CVICALLBACK HILTabChanelSelect(int panel, int control, int event, void *callbackData, int eventData1, int eventData2);
int  CVICALLBACK HILYRangeChangeCallback(int panel, int control, int event, void *callbackData, int eventData1, int eventData2);
int  CVICALLBACK NetWatchDog3(int panel, int control, int event, void *callbackData, int eventData1, int eventData2);


#ifdef __cplusplus
    }
#endif

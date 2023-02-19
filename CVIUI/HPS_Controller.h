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

#define  HPSPANEL                         1
#define  HPSPANEL_VALVE_SEL_RING          2       /* control type: ring, callback function: valve_ctrl_select */
#define  HPSPANEL_COM_RING                3       /* control type: ring, callback function: com_select */
#define  HPSPANEL_LOAD_P_GAUGE            4       /* control type: scale, callback function: (none) */
#define  HPSPANEL_SRC2_P_GAUGE            5       /* control type: scale, callback function: (none) */
#define  HPSPANEL_COMMANDBUTTON           6       /* control type: command, callback function: reset */
#define  HPSPANEL_SRC1_P_GAUGE            7       /* control type: scale, callback function: (none) */
#define  HPSPANEL_SYS_P_CONFIRM_BUTTON    8       /* control type: command, callback function: set_sys_p */
#define  HPSPANEL_STATES_TEXTBOX          9       /* control type: textBox, callback function: (none) */
#define  HPSPANEL_LLEVEL_ALARM_LED        10      /* control type: LED, callback function: (none) */
#define  HPSPANEL_HLEVEL_ALARM_LED        11      /* control type: LED, callback function: (none) */
#define  HPSPANEL_HT_ALARM_LED            12      /* control type: LED, callback function: (none) */
#define  HPSPANEL_SRC2_P_ALARM_LED        13      /* control type: LED, callback function: (none) */
#define  HPSPANEL_LT_ALARM_LED            14      /* control type: LED, callback function: (none) */
#define  HPSPANEL_SRC2_PACKER_LED         15      /* control type: LED, callback function: (none) */
#define  HPSPANEL_SRC2_P_RUN_LED          16      /* control type: LED, callback function: (none) */
#define  HPSPANEL_SRC1_PACKER_LED         17      /* control type: LED, callback function: (none) */
#define  HPSPANEL_REMOTE_STOP_LED         18      /* control type: LED, callback function: (none) */
#define  HPSPANEL_SRC1_P_RUN_LED          19      /* control type: LED, callback function: (none) */
#define  HPSPANEL_SRC1_P_ALARM_LED        20      /* control type: LED, callback function: (none) */
#define  HPSPANEL_RETURN_P_ALARM_LED      21      /* control type: LED, callback function: (none) */
#define  HPSPANEL_SYS_P_DIAL_2            22      /* control type: scale, callback function: (none) */
#define  HPSPANEL_SYS_P_DIAL              23      /* control type: scale, callback function: (none) */
#define  HPSPANEL_QUIT                    24      /* control type: command, callback function: quit */
#define  HPSPANEL_TIMER                   25      /* control type: timer, callback function: HPStimer */
#define  HPSPANEL_MOTOR_RING_2            26      /* control type: pictRing, callback function: sys_start2 */
#define  HPSPANEL_MOTOR_RING              27      /* control type: pictRing, callback function: sys_start */
#define  HPSPANEL_STATUS_COMMANDBUTTON    28      /* control type: command, callback function: clear_box */
#define  HPSPANEL_TANK_P_METER            29      /* control type: scale, callback function: (none) */
#define  HPSPANEL_TEMP_METER              30      /* control type: scale, callback function: (none) */
#define  HPSPANEL_DECORATION              31      /* control type: deco, callback function: (none) */
#define  HPSPANEL_DECORATION_2            32      /* control type: deco, callback function: (none) */


     /* Control Arrays: */

          /* (no control arrays in the resource file) */


     /* Menu Bars, Menus, and Menu Items: */

          /* (no menu bars in the resource file) */


     /* Callback Prototypes: */

int  CVICALLBACK clear_box(int panel, int control, int event, void *callbackData, int eventData1, int eventData2);
int  CVICALLBACK com_select(int panel, int control, int event, void *callbackData, int eventData1, int eventData2);
int  CVICALLBACK HPStimer(int panel, int control, int event, void *callbackData, int eventData1, int eventData2);
int  CVICALLBACK quit(int panel, int control, int event, void *callbackData, int eventData1, int eventData2);
int  CVICALLBACK reset(int panel, int control, int event, void *callbackData, int eventData1, int eventData2);
int  CVICALLBACK set_sys_p(int panel, int control, int event, void *callbackData, int eventData1, int eventData2);
int  CVICALLBACK sys_start(int panel, int control, int event, void *callbackData, int eventData1, int eventData2);
int  CVICALLBACK sys_start2(int panel, int control, int event, void *callbackData, int eventData1, int eventData2);
int  CVICALLBACK valve_ctrl_select(int panel, int control, int event, void *callbackData, int eventData1, int eventData2);


#ifdef __cplusplus
    }
#endif

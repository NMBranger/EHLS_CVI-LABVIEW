#pragma pack(4)
typedef struct {char *name; void *address; unsigned long isFunction:1; unsigned long reserved:31;} ExeSymbol;
int __cdecl AChannelChooseCallback (int panel, int control, int event, void *callbackData, int eventData1, int eventData2);
int __cdecl AHILDisplayChangeCallback (int panel, int control, int event, void *callbackData, int eventData1, int eventData2);
int __cdecl AMotionDisplayChangeCallback (int panel, int control, int event, void *callbackData, int eventData1, int eventData2);
int __cdecl AOppDisChangeCallback (int panel, int control, int event, void *callbackData, int eventData1, int eventData2);
int __cdecl Enablecallback (int panel, int control, int event, void *callbackData, int eventData1, int eventData2);
int __cdecl ExitCallback (int panel, int control, int event, void *callbackData, int eventData1, int eventData2);
int __cdecl HILChTesttypeChange (int panel, int control, int event, void *callbackData, int eventData1, int eventData2);
int __cdecl HILDataReDisplay (int panel, int control, int event, void *callbackData, int eventData1, int eventData2);
int __cdecl HILDisplayChangeCallback (int panel, int control, int event, void *callbackData, int eventData1, int eventData2);
int __cdecl HILDownParaCallback (int panel, int control, int event, void *callbackData, int eventData1, int eventData2);
int __cdecl HILExitCallback (int panel, int control, int event, void *callbackData, int eventData1, int eventData2);
int __cdecl HILOliDisplay (int panel, int control, int event, void *callbackData, int eventData1, int eventData2);
int __cdecl HILPanelCallback (int panel, int event, void *callbackData, int eventData1, int eventData2);
int __cdecl HILParaTabChanelSelect (int panel, int control, int event, void *callbackData, int eventData1, int eventData2);
int __cdecl HILRecordCallback (int panel, int control, int event, void *callbackData, int eventData1, int eventData2);
int __cdecl HILResetCallback (int panel, int control, int event, void *callbackData, int eventData1, int eventData2);
int __cdecl HILSaveCallback (int panel, int control, int event, void *callbackData, int eventData1, int eventData2);
int __cdecl HILSaveParaCallback (int panel, int control, int event, void *callbackData, int eventData1, int eventData2);
int __cdecl HILSetParaCallback (int panel, int control, int event, void *callbackData, int eventData1, int eventData2);
int __cdecl HILStartCallback (int panel, int control, int event, void *callbackData, int eventData1, int eventData2);
int __cdecl HILTabChanelSelect (int panel, int control, int event, void *callbackData, int eventData1, int eventData2);
int __cdecl HILTestCallback (int panel, int control, int event, void *callbackData, int eventData1, int eventData2);
int __cdecl HILYRangeChangeCallback (int panel, int control, int event, void *callbackData, int eventData1, int eventData2);
int __cdecl HPStimer (int panel, int control, int event, void *callbackData, int eventData1, int eventData2);
int __cdecl HelpCallback (int panel, int control, int event, void *callbackData, int eventData1, int eventData2);
int __cdecl HySupplyCallback (int panel, int control, int event, void *callbackData, int eventData1, int eventData2);
int __cdecl MEnablecallback (int panel, int control, int event, void *callbackData, int eventData1, int eventData2);
int __cdecl MainPanelCallback (int panel, int event, void *callbackData, int eventData1, int eventData2);
int __cdecl MotionCh1TesttypeChange (int panel, int control, int event, void *callbackData, int eventData1, int eventData2);
int __cdecl MotionDataReDisplay (int panel, int control, int event, void *callbackData, int eventData1, int eventData2);
int __cdecl MotionDisplayChangeCallback (int panel, int control, int event, void *callbackData, int eventData1, int eventData2);
int __cdecl MotionDownParaCallback (int panel, int control, int event, void *callbackData, int eventData1, int eventData2);
int __cdecl MotionExitCallback (int panel, int control, int event, void *callbackData, int eventData1, int eventData2);
int __cdecl MotionPanelCallback (int panel, int event, void *callbackData, int eventData1, int eventData2);
int __cdecl MotionRecordCallback (int panel, int control, int event, void *callbackData, int eventData1, int eventData2);
int __cdecl MotionResetCallback (int panel, int control, int event, void *callbackData, int eventData1, int eventData2);
int __cdecl MotionSaveCallback (int panel, int control, int event, void *callbackData, int eventData1, int eventData2);
int __cdecl MotionSaveParaCallback (int panel, int control, int event, void *callbackData, int eventData1, int eventData2);
int __cdecl MotionSetParaCallback (int panel, int control, int event, void *callbackData, int eventData1, int eventData2);
int __cdecl MotionStartCallback (int panel, int control, int event, void *callbackData, int eventData1, int eventData2);
int __cdecl MotionTabChanelSelect (int panel, int control, int event, void *callbackData, int eventData1, int eventData2);
int __cdecl MotionTestCallback (int panel, int control, int event, void *callbackData, int eventData1, int eventData2);
int __cdecl MotionYRangeChangeCallback (int panel, int control, int event, void *callbackData, int eventData1, int eventData2);
int __cdecl NetDetectCallback (int panel, int control, int event, void *callbackData, int eventData1, int eventData2);
int __cdecl NetWatchDog (int panel, int control, int event, void *callbackData, int eventData1, int eventData2);
int __cdecl NetWatchDog1 (int panel, int control, int event, void *callbackData, int eventData1, int eventData2);
int __cdecl NetWatchDog2 (int panel, int control, int event, void *callbackData, int eventData1, int eventData2);
int __cdecl NetWatchDog3 (int panel, int control, int event, void *callbackData, int eventData1, int eventData2);
int __cdecl OEnablecallback (int panel, int control, int event, void *callbackData, int eventData1, int eventData2);
int __cdecl OppDataReDisplay (int panel, int control, int event, void *callbackData, int eventData1, int eventData2);
int __cdecl OppDisChangeCallback (int panel, int control, int event, void *callbackData, int eventData1, int eventData2);
int __cdecl OppositeTestCallback (int panel, int control, int event, void *callbackData, int eventData1, int eventData2);
int __cdecl OppsiteDownParaCallback (int panel, int control, int event, void *callbackData, int eventData1, int eventData2);
int __cdecl OppsiteExitCallback (int panel, int control, int event, void *callbackData, int eventData1, int eventData2);
int __cdecl OppsitePanelCallback (int panel, int event, void *callbackData, int eventData1, int eventData2);
int __cdecl OppsiteRecordCallback (int panel, int control, int event, void *callbackData, int eventData1, int eventData2);
int __cdecl OppsiteResetCallback (int panel, int control, int event, void *callbackData, int eventData1, int eventData2);
int __cdecl OppsiteSaveCallback (int panel, int control, int event, void *callbackData, int eventData1, int eventData2);
int __cdecl OppsiteSaveParaCallback (int panel, int control, int event, void *callbackData, int eventData1, int eventData2);
int __cdecl OppsiteSetParaCallback (int panel, int control, int event, void *callbackData, int eventData1, int eventData2);
int __cdecl OppsiteStartCallback (int panel, int control, int event, void *callbackData, int eventData1, int eventData2);
int __cdecl StaCh1TesttypeChange (int panel, int control, int event, void *callbackData, int eventData1, int eventData2);
int __cdecl StaCh2TesttypeChange (int panel, int control, int event, void *callbackData, int eventData1, int eventData2);
int __cdecl StaCh4TesttypeChange (int panel, int control, int event, void *callbackData, int eventData1, int eventData2);
int __cdecl StaDisplayChangeCallback (int panel, int control, int event, void *callbackData, int eventData1, int eventData2);
int __cdecl StaTabChanelSelect (int panel, int control, int event, void *callbackData, int eventData1, int eventData2);
int __cdecl StaYRangeChangeCallback (int panel, int control, int event, void *callbackData, int eventData1, int eventData2);
int __cdecl StaticDataReDisplay (int panel, int control, int event, void *callbackData, int eventData1, int eventData2);
int __cdecl StaticDownParaCallback (int panel, int control, int event, void *callbackData, int eventData1, int eventData2);
int __cdecl StaticExitCallback (int panel, int control, int event, void *callbackData, int eventData1, int eventData2);
int __cdecl StaticPanelCallback (int panel, int event, void *callbackData, int eventData1, int eventData2);
int __cdecl StaticRecordCallback (int panel, int control, int event, void *callbackData, int eventData1, int eventData2);
int __cdecl StaticResetCallback (int panel, int control, int event, void *callbackData, int eventData1, int eventData2);
int __cdecl StaticSaveCallback (int panel, int control, int event, void *callbackData, int eventData1, int eventData2);
int __cdecl StaticSaveParaCallback (int panel, int control, int event, void *callbackData, int eventData1, int eventData2);
int __cdecl StaticSetParaCallback (int panel, int control, int event, void *callbackData, int eventData1, int eventData2);
int __cdecl StaticStartCallback (int panel, int control, int event, void *callbackData, int eventData1, int eventData2);
int __cdecl StaticTestCallback (int panel, int control, int event, void *callbackData, int eventData1, int eventData2);
int __cdecl SystemSetCallback (int panel, int control, int event, void *callbackData, int eventData1, int eventData2);
int __cdecl TChannelChooseCallback (int panel, int control, int event, void *callbackData, int eventData1, int eventData2);
int __cdecl TOppDisChangeCallback (int panel, int control, int event, void *callbackData, int eventData1, int eventData2);
int __cdecl TStaDisplayChangeCallback (int panel, int control, int event, void *callbackData, int eventData1, int eventData2);
int __cdecl TestTypeChangeCallback (int panel, int control, int event, void *callbackData, int eventData1, int eventData2);
int __cdecl YRangeChange (int panel, int control, int event, void *callbackData, int eventData1, int eventData2);
int __cdecl clear_box (int panel, int control, int event, void *callbackData, int eventData1, int eventData2);
int __cdecl com_select (int panel, int control, int event, void *callbackData, int eventData1, int eventData2);
int __cdecl quit (int panel, int control, int event, void *callbackData, int eventData1, int eventData2);
int __cdecl reset (int panel, int control, int event, void *callbackData, int eventData1, int eventData2);
int __cdecl set_sys_p (int panel, int control, int event, void *callbackData, int eventData1, int eventData2);
int __cdecl sys_start (int panel, int control, int event, void *callbackData, int eventData1, int eventData2);
int __cdecl sys_start2 (int panel, int control, int event, void *callbackData, int eventData1, int eventData2);
int __cdecl valve_ctrl_select (int panel, int control, int event, void *callbackData, int eventData1, int eventData2);
int __UICallbackSymbolCount = 92;
ExeSymbol __UICallbackSymbols [92] =
{
 {"_AChannelChooseCallback", (void*)AChannelChooseCallback, 1, 0},
 {"_AHILDisplayChangeCallback", (void*)AHILDisplayChangeCallback, 1, 0},
 {"_AMotionDisplayChangeCallback", (void*)AMotionDisplayChangeCallback, 1, 0},
 {"_AOppDisChangeCallback", (void*)AOppDisChangeCallback, 1, 0},
 {"_Enablecallback", (void*)Enablecallback, 1, 0},
 {"_ExitCallback", (void*)ExitCallback, 1, 0},
 {"_HILChTesttypeChange", (void*)HILChTesttypeChange, 1, 0},
 {"_HILDataReDisplay", (void*)HILDataReDisplay, 1, 0},
 {"_HILDisplayChangeCallback", (void*)HILDisplayChangeCallback, 1, 0},
 {"_HILDownParaCallback", (void*)HILDownParaCallback, 1, 0},
 {"_HILExitCallback", (void*)HILExitCallback, 1, 0},
 {"_HILOliDisplay", (void*)HILOliDisplay, 1, 0},
 {"_HILPanelCallback", (void*)HILPanelCallback, 1, 0},
 {"_HILParaTabChanelSelect", (void*)HILParaTabChanelSelect, 1, 0},
 {"_HILRecordCallback", (void*)HILRecordCallback, 1, 0},
 {"_HILResetCallback", (void*)HILResetCallback, 1, 0},
 {"_HILSaveCallback", (void*)HILSaveCallback, 1, 0},
 {"_HILSaveParaCallback", (void*)HILSaveParaCallback, 1, 0},
 {"_HILSetParaCallback", (void*)HILSetParaCallback, 1, 0},
 {"_HILStartCallback", (void*)HILStartCallback, 1, 0},
 {"_HILTabChanelSelect", (void*)HILTabChanelSelect, 1, 0},
 {"_HILTestCallback", (void*)HILTestCallback, 1, 0},
 {"_HILYRangeChangeCallback", (void*)HILYRangeChangeCallback, 1, 0},
 {"_HPStimer", (void*)HPStimer, 1, 0},
 {"_HelpCallback", (void*)HelpCallback, 1, 0},
 {"_HySupplyCallback", (void*)HySupplyCallback, 1, 0},
 {"_MEnablecallback", (void*)MEnablecallback, 1, 0},
 {"_MainPanelCallback", (void*)MainPanelCallback, 1, 0},
 {"_MotionCh1TesttypeChange", (void*)MotionCh1TesttypeChange, 1, 0},
 {"_MotionDataReDisplay", (void*)MotionDataReDisplay, 1, 0},
 {"_MotionDisplayChangeCallback", (void*)MotionDisplayChangeCallback, 1, 0},
 {"_MotionDownParaCallback", (void*)MotionDownParaCallback, 1, 0},
 {"_MotionExitCallback", (void*)MotionExitCallback, 1, 0},
 {"_MotionPanelCallback", (void*)MotionPanelCallback, 1, 0},
 {"_MotionRecordCallback", (void*)MotionRecordCallback, 1, 0},
 {"_MotionResetCallback", (void*)MotionResetCallback, 1, 0},
 {"_MotionSaveCallback", (void*)MotionSaveCallback, 1, 0},
 {"_MotionSaveParaCallback", (void*)MotionSaveParaCallback, 1, 0},
 {"_MotionSetParaCallback", (void*)MotionSetParaCallback, 1, 0},
 {"_MotionStartCallback", (void*)MotionStartCallback, 1, 0},
 {"_MotionTabChanelSelect", (void*)MotionTabChanelSelect, 1, 0},
 {"_MotionTestCallback", (void*)MotionTestCallback, 1, 0},
 {"_MotionYRangeChangeCallback", (void*)MotionYRangeChangeCallback, 1, 0},
 {"_NetDetectCallback", (void*)NetDetectCallback, 1, 0},
 {"_NetWatchDog", (void*)NetWatchDog, 1, 0},
 {"_NetWatchDog1", (void*)NetWatchDog1, 1, 0},
 {"_NetWatchDog2", (void*)NetWatchDog2, 1, 0},
 {"_NetWatchDog3", (void*)NetWatchDog3, 1, 0},
 {"_OEnablecallback", (void*)OEnablecallback, 1, 0},
 {"_OppDataReDisplay", (void*)OppDataReDisplay, 1, 0},
 {"_OppDisChangeCallback", (void*)OppDisChangeCallback, 1, 0},
 {"_OppositeTestCallback", (void*)OppositeTestCallback, 1, 0},
 {"_OppsiteDownParaCallback", (void*)OppsiteDownParaCallback, 1, 0},
 {"_OppsiteExitCallback", (void*)OppsiteExitCallback, 1, 0},
 {"_OppsitePanelCallback", (void*)OppsitePanelCallback, 1, 0},
 {"_OppsiteRecordCallback", (void*)OppsiteRecordCallback, 1, 0},
 {"_OppsiteResetCallback", (void*)OppsiteResetCallback, 1, 0},
 {"_OppsiteSaveCallback", (void*)OppsiteSaveCallback, 1, 0},
 {"_OppsiteSaveParaCallback", (void*)OppsiteSaveParaCallback, 1, 0},
 {"_OppsiteSetParaCallback", (void*)OppsiteSetParaCallback, 1, 0},
 {"_OppsiteStartCallback", (void*)OppsiteStartCallback, 1, 0},
 {"_StaCh1TesttypeChange", (void*)StaCh1TesttypeChange, 1, 0},
 {"_StaCh2TesttypeChange", (void*)StaCh2TesttypeChange, 1, 0},
 {"_StaCh4TesttypeChange", (void*)StaCh4TesttypeChange, 1, 0},
 {"_StaDisplayChangeCallback", (void*)StaDisplayChangeCallback, 1, 0},
 {"_StaTabChanelSelect", (void*)StaTabChanelSelect, 1, 0},
 {"_StaYRangeChangeCallback", (void*)StaYRangeChangeCallback, 1, 0},
 {"_StaticDataReDisplay", (void*)StaticDataReDisplay, 1, 0},
 {"_StaticDownParaCallback", (void*)StaticDownParaCallback, 1, 0},
 {"_StaticExitCallback", (void*)StaticExitCallback, 1, 0},
 {"_StaticPanelCallback", (void*)StaticPanelCallback, 1, 0},
 {"_StaticRecordCallback", (void*)StaticRecordCallback, 1, 0},
 {"_StaticResetCallback", (void*)StaticResetCallback, 1, 0},
 {"_StaticSaveCallback", (void*)StaticSaveCallback, 1, 0},
 {"_StaticSaveParaCallback", (void*)StaticSaveParaCallback, 1, 0},
 {"_StaticSetParaCallback", (void*)StaticSetParaCallback, 1, 0},
 {"_StaticStartCallback", (void*)StaticStartCallback, 1, 0},
 {"_StaticTestCallback", (void*)StaticTestCallback, 1, 0},
 {"_SystemSetCallback", (void*)SystemSetCallback, 1, 0},
 {"_TChannelChooseCallback", (void*)TChannelChooseCallback, 1, 0},
 {"_TOppDisChangeCallback", (void*)TOppDisChangeCallback, 1, 0},
 {"_TStaDisplayChangeCallback", (void*)TStaDisplayChangeCallback, 1, 0},
 {"_TestTypeChangeCallback", (void*)TestTypeChangeCallback, 1, 0},
 {"_YRangeChange", (void*)YRangeChange, 1, 0},
 {"_clear_box", (void*)clear_box, 1, 0},
 {"_com_select", (void*)com_select, 1, 0},
 {"_quit", (void*)quit, 1, 0},
 {"_reset", (void*)reset, 1, 0},
 {"_set_sys_p", (void*)set_sys_p, 1, 0},
 {"_sys_start", (void*)sys_start, 1, 0},
 {"_sys_start2", (void*)sys_start2, 1, 0},
 {"_valve_ctrl_select", (void*)valve_ctrl_select, 1, 0}
};

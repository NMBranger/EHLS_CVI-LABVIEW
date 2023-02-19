/* -*-C++-*-
 *
 *      Copyright (c) 1996-2005 Ardence, Inc.  All rights reserved.
 *
 *      Rtapi.h -- Real-time API data types and function prototypes.
 *
 *      $Header: /RtxNt/include/Rtapi.h 29    2/20/03 9:19a Yanyi $
 */

#ifndef _RTAPI_H_
#define _RTAPI_H_

#ifdef __cplusplus
extern "C" {
#endif

// Specify a RTAPI version unless an override is in effect.
#ifndef RTAPIVER
#define RTAPIVER 30
#endif 

// Define API decoration for direct importing of DLL references.
#if !defined(_RTBASEAPI_) && !defined(MIDL_PASS)
#define RTBASEAPI __declspec(dllimport)
#else
#define RTBASEAPI
#endif

// Define RTAPI function calling convention unless an override is in effect.
#ifndef RTAPI
#       if (defined(_MSC_VER)&&(_MSC_VER >= 800)) || defined(_STDCALL_SUPPORTED)
#              define RTAPI __stdcall
#       else
#              define RTAPI
#       endif
#endif

// Max length used for names by RtCreate[Objtype], RtOpen[Objtype]
#define RTX_MAX_OBJECT_NAME_LENGTH	128

// Define the declaration for application provided functions.
#define RTFCNDCL RTAPI

//
// Provide backward (source level) compatability with pre-release RTX 4.1 functions
// to their current equivalent Win32 functions.
//
// NOTE: These are obsolete and will be removed in a future version.
//
#define	RtGetLastError		GetLastError
#define	RtSetLastError		SetLastError
#define RtCreateThread		CreateThread
#define	RtExitThread		ExitThread
#define	RtGetCurrentThread	GetCurrentThread
#define	RtResumeThread		ResumeThread
#define	RtSuspendThread		SuspendThread
#define	RtTerminateThread	TerminateThread
#define	RtExitProcess		ExitProcess
#define	RtGetStdHandle		GetStdHandle
#define	RtSleep			Sleep
#define	RtSleepEx		SleepEx
#ifdef UNICODE
#define	RtWriteConsole		WriteConsoleW
#define	RtGetCommandLine	GetCommandLineW
#else
#define	RtWriteConsole		WriteConsoleA
#define	RtGetCommandLine	GetCommandLineA
#endif // !UNICODE


#if RTAPIVER >= 20

RTBASEAPI
BOOL RTAPI RtIsInRtss(VOID);

RTBASEAPI
int _cdecl RtAtoi(LPCSTR string);

RTBASEAPI
int _cdecl RtWtoi(LPCWSTR string);

RTBASEAPI
int _cdecl RtPrintf(LPCSTR fmt, ...);

RTBASEAPI
int _cdecl RtWprintf(LPCWSTR fmt, ...);

//
// Real-time priority defines.
//
#define RT_PRIORITY_MAX 127
#define RT_PRIORITY_MIN 0

RTBASEAPI
int RTAPI RtGetThreadPriority(HANDLE hThread);

RTBASEAPI
BOOL RTAPI RtSetThreadPriority(
			HANDLE hThread,
			int nPriority);

RTBASEAPI
DWORD RTAPI RtGetThreadTimeQuantum(HANDLE hThread);

RTBASEAPI
BOOL RTAPI RtSetThreadTimeQuantum(
			HANDLE hThread,
			DWORD dwQuantumInMS);

//
// Shutdown Handler reason defines.
//
#define RT_SHUTDOWN_OPERATOR_REQUEST    1
#define RT_SHUTDOWN_NT_SYSTEM_SHUTDOWN  2
#define RT_SHUTDOWN_NT_STOP             3

RTBASEAPI
HANDLE RTAPI RtAttachShutdownHandler(
			PSECURITY_ATTRIBUTES  pThreadAttributes,
			ULONG StackSize,                        
			VOID (RTFCNDCL *Routine)(PVOID context, LONG reason),
			PVOID Context,
			ULONG Priority);

RTBASEAPI
BOOL RTAPI RtReleaseShutdownHandler(HANDLE hShutdown);

RTBASEAPI
DWORD RTAPI RtWaitForSingleObject(
    			HANDLE hHandle,
			DWORD dwMilliseconds);

//
// Data structures for threads waiting (WFSO/WFMO) on synchronization objects.
//
#define	MAX_WFMO	16				// Maximum number of objects per WFMO

RTBASEAPI
DWORD RTAPI RtWaitForMultipleObjects(
	DWORD			dwCount,
    CONST HANDLE	*lpHandles,
	BOOL			bWaitAll,
    DWORD			dwMilliseconds);

RTBASEAPI
BOOL RTAPI RtCloseHandle(HANDLE hObject);

RTBASEAPI
HANDLE RTAPI RtCreateSharedMemoryA(
			DWORD flProtect,
			DWORD dwMaximumSizeHigh,
			DWORD dwMaximumSizeLow,
			LPCSTR lpName,
			VOID ** location);
RTBASEAPI
HANDLE RTAPI RtCreateSharedMemoryW(
			DWORD flProtect,
			DWORD dwMaximumSizeHigh,
			DWORD dwMaximumSizeLow,
			LPCWSTR lpName,
			VOID ** location);
#ifdef UNICODE
#define RtCreateSharedMemory  RtCreateSharedMemoryW
#else
#define RtCreateSharedMemory  RtCreateSharedMemoryA
#endif // !UNICODE

// Open shared memory flags

#define SHM_MAP_WRITE 2
#define SHM_MAP_READ 1
#define SHM_MAP_ALL_ACCESS (SHM_MAP_WRITE + SHM_MAP_READ)

RTBASEAPI
HANDLE RTAPI RtOpenSharedMemoryA(
			DWORD dwDesiredAccess,
			BOOL bInheritHandle,
			LPCSTR lpName,
			VOID ** location);
RTBASEAPI
HANDLE RTAPI RtOpenSharedMemoryW(
			DWORD dwDesiredAccess,
			BOOL bInheritHandle,
			LPCWSTR lpName,
			VOID ** location);
#ifdef UNICODE
#define RtOpenSharedMemory  RtOpenSharedMemoryW
#else
#define RtOpenSharedMemory  RtOpenSharedMemoryA
#endif // !UNICODE

RTBASEAPI
BOOL RTAPI RtUnmapSharedMemory(VOID * location);

RTBASEAPI
HANDLE RTAPI RtCreateMutexA(
			LPSECURITY_ATTRIBUTES lpMutexAttributes,
			BOOL bInitialOwner,
			LPCSTR lpName);
RTBASEAPI
HANDLE RTAPI RtCreateMutexW(
			LPSECURITY_ATTRIBUTES lpMutexAttributes,
			BOOL bInitialOwner,
			LPCWSTR lpName);
#ifdef UNICODE
#define RtCreateMutex  RtCreateMutexW
#else
#define RtCreateMutex  RtCreateMutexA
#endif // !UNICODE

RTBASEAPI
HANDLE RTAPI RtOpenMutexA(
			DWORD dwDesiredAccess,
			BOOL bInheritHandle,
			LPCSTR lpName);
RTBASEAPI
HANDLE RTAPI RtOpenMutexW(
			DWORD dwDesiredAccess,
			BOOL bInheritHandle,
			LPCWSTR lpName);
#ifdef UNICODE
#define RtOpenMutex  RtOpenMutexW
#else
#define RtOpenMutex  RtOpenMutexA
#endif // !UNICODE

RTBASEAPI
BOOL RTAPI RtReleaseMutex(HANDLE hMutex);

RTBASEAPI
HANDLE RTAPI RtCreateSemaphoreA(
			LPSECURITY_ATTRIBUTES lpSemaphoreAttributes,
			LONG lInitialCount,
			LONG lMaximumCount,
			LPCSTR lpName);
RTBASEAPI
HANDLE RTAPI RtCreateSemaphoreW(
			LPSECURITY_ATTRIBUTES lpSemaphoreAttributes,
			LONG lInitialCount,
			LONG lMaximumCount,
			LPCWSTR lpName);
#ifdef UNICODE
#define RtCreateSemaphore  RtCreateSemaphoreW
#else
#define RtCreateSemaphore  RtCreateSemaphoreA
#endif // !UNICODE

RTBASEAPI
HANDLE RTAPI RtOpenSemaphoreA(
			DWORD dwDesiredAccess,
			BOOL bInheritHandle,
			LPCSTR lpName);
RTBASEAPI
HANDLE RTAPI RtOpenSemaphoreW(
			DWORD dwDesiredAccess,
			BOOL bInheritHandle,
			LPCWSTR lpName);
#ifdef UNICODE
#define RtOpenSemaphore  RtOpenSemaphoreW
#else
#define RtOpenSemaphore  RtOpenSemaphoreA
#endif // !UNICODE

RTBASEAPI
BOOL RTAPI RtReleaseSemaphore(HANDLE hSemaphore,
			LONG lReleaseCount,
			LPLONG lpPreviousCount);

RTBASEAPI
HANDLE RTAPI RtCreateEventA(
			LPSECURITY_ATTRIBUTES lpEventAttributes,
			BOOL bManualReset,
			BOOL bInitialState,
			LPCSTR lpName);
RTBASEAPI
HANDLE RTAPI RtCreateEventW(
			LPSECURITY_ATTRIBUTES lpEventAttributes,
			BOOL bManualReset,
			BOOL bInitialState,
			LPCWSTR lpName);
#ifdef UNICODE
#define RtCreateEvent  RtCreateEventW
#else
#define RtCreateEvent  RtCreateEventA
#endif // !UNICODE

RTBASEAPI
HANDLE RTAPI RtOpenEventA(
			DWORD dwDesiredAccess,
			BOOL bInheritHandle,
			LPCSTR lpName);
RTBASEAPI
HANDLE RTAPI RtOpenEventW(
			DWORD dwDesiredAccess,
			BOOL bInheritHandle,
			LPCWSTR lpName);
#ifdef UNICODE
#define RtOpenEvent  RtOpenEventW
#else
#define RtOpenEvent  RtOpenEventA
#endif // !UNICODE

RTBASEAPI
BOOL RTAPI RtPulseEvent(HANDLE hEvent);

RTBASEAPI
BOOL RTAPI RtResetEvent(HANDLE hEvent);

RTBASEAPI
BOOL RTAPI RtSetEvent(HANDLE hEvent);

typedef
VOID
(RTFCNDCL *PTIMERAPCROUTINE)(
    LPVOID lpArgToCompletionRoutine,
    DWORD dwTimerLowValue,
    DWORD dwTimerHighValue
    );

RTBASEAPI
HANDLE RTAPI RtCreateWaitableTimerA(
			LPSECURITY_ATTRIBUTES lpTimerAttributes,
		    BOOL bManualReset,
			LPCSTR lpTimerName);
RTBASEAPI
HANDLE RTAPI RtCreateWaitableTimerW(
			LPSECURITY_ATTRIBUTES lpTimerAttributes,
			BOOL bManualReset,
			LPCWSTR lpTimerName);
#ifdef UNICODE
#define RtCreateWaitableTimer  RtCreateWaitableTimerW
#else
#define RtCreateWaitableTimer  RtCreateWaitableTimerA
#endif // !UNICODE

RTBASEAPI
HANDLE RTAPI RtOpenWaitableTimerA(
			DWORD dwDesiredAccess,
			BOOL bInheritHandle,
			LPCSTR lpTimerName);
RTBASEAPI 
HANDLE RTAPI RtOpenWaitableTimerW(
			DWORD dwDesiredAccess,
			BOOL bInheritHandle,
			LPCWSTR lpTimerName);
#ifdef UNICODE
#define RtOpenWaitableTimer  RtOpenWaitableTimerW
#else
#define RtOpenWaitableTimer  RtOpenWaitableTimerA
#endif // !UNICODE

RTBASEAPI
BOOL RTAPI RtSetWaitableTimer(
			HANDLE hWaitableTimer,
			const LARGE_INTEGER *lpDueTime,
			LONG lPeriod,
			PTIMERAPCROUTINE pfnCompletionRoutine,
			LPVOID lpArgToCompletionRoutine,
			BOOL fResume);

RTBASEAPI
BOOL RTAPI RtCancelWaitableTimer(HANDLE hWaitableTimer);

RTBASEAPI
BOOL RTAPI RtSetWaitableTimerFt(
			HANDLE hWaitableTimer,
			const LARGE_INTEGER *lpDueTime,
			const LARGE_INTEGER *lpPeriod,
			PTIMERAPCROUTINE pfnCompletionRoutine,
			LPVOID lpArgToCompletionRoutine,
			BOOL fResume);

#endif // RTAPIVER >= 20

//
// Clocks and Timers
//
#define CLOCK_1		1	// System Clock
#define CLOCK_2		2	// Real-time HAL Clock
#define CLOCK_3		3
#define CLOCK_4		4
#define CLOCK_FASTEST	0xFFFF	// Fastest available clock & timer

#define CLOCK_SYSTEM	CLOCK_1

typedef unsigned long CLOCK, *PCLOCK;

RTBASEAPI
BOOL RTAPI RtGetClockTime(
			CLOCK Clock,
			PLARGE_INTEGER pTime);

RTBASEAPI
BOOL RTAPI RtSetClockTime(
			CLOCK Clock,
			PLARGE_INTEGER pTime);

RTBASEAPI
BOOL RTAPI RtGetClockResolution(
			CLOCK Clock,
		   	PLARGE_INTEGER pTime);

RTBASEAPI
BOOL RTAPI RtGetClockTimerPeriod(
			CLOCK Clock,
			PLARGE_INTEGER pTime);

RTBASEAPI
HANDLE RTAPI RtCreateTimer(
			PSECURITY_ATTRIBUTES pThreadAttributes,      
			ULONG StackSize,            
			VOID (RTFCNDCL *pRoutine)(PVOID context),
			PVOID Context,
			ULONG Priority,
			CLOCK Clock);

RTBASEAPI
BOOL RTAPI RtDeleteTimer(HANDLE Handle);

RTBASEAPI
BOOL RTAPI RtCancelTimer(
			HANDLE hTimer,
			PLARGE_INTEGER pRemain);

RTBASEAPI
BOOL RTAPI RtGetTimer(
			HANDLE Handle,
			PLARGE_INTEGER pRemain);

RTBASEAPI
BOOL RTAPI RtSetTimer(
			HANDLE hTimer,
			PLARGE_INTEGER pExpiration,
			PLARGE_INTEGER pInterval);

RTBASEAPI
BOOL RTAPI RtSetTimerRelative(
			HANDLE hTimer,
			PLARGE_INTEGER pExpiration,
			PLARGE_INTEGER pInterval);

RTBASEAPI
void RTAPI RtGetRtssTimeAsFileTime(LPFILETIME pTime);

RTBASEAPI
void RTAPI RtSetRtssTimeAsFileTime(LPFILETIME pTime);

RTBASEAPI
VOID RTAPI RtSleepFt(PLARGE_INTEGER pSleepTime);

RTBASEAPI
ULONG RTAPI RtSleepFtEx(
			PLARGE_INTEGER pSleepTime,
			BOOL bAlertable);

RTBASEAPI
BOOL RTAPI RtEnablePortIo(
			PUCHAR Port,
			ULONG Count);

RTBASEAPI
BOOL RTAPI RtDisablePortIo(
			PUCHAR Port,
			ULONG Count);

RTBASEAPI
UCHAR RTAPI RtReadPortUchar(PUCHAR Port);

RTBASEAPI
USHORT RTAPI RtReadPortUshort(PUSHORT Port);

RTBASEAPI
ULONG RTAPI RtReadPortUlong(PULONG Port);

RTBASEAPI
VOID RTAPI RtReadPortBufferUchar(
			PUCHAR Port,
			PUCHAR Buffer,
			ULONG Count);

RTBASEAPI
VOID RTAPI RtReadPortBufferUshort(
			PUSHORT Port,
			PUSHORT Buffer,
			ULONG Count);

RTBASEAPI
VOID RTAPI RtReadPortBufferUlong(
			PULONG Port,
			PULONG Buffer,
			ULONG Count);

RTBASEAPI
VOID RTAPI RtWritePortUchar(
			PUCHAR Port,
			UCHAR Value);

RTBASEAPI
VOID RTAPI RtWritePortUshort(
			PUSHORT Port,
			USHORT Value);

RTBASEAPI
VOID RTAPI RtWritePortUlong(
			PULONG Port,
			ULONG Value);

RTBASEAPI
VOID RTAPI RtWritePortBufferUchar(
			PUCHAR Port,
			PUCHAR Buffer,
			ULONG Count);

RTBASEAPI
VOID RTAPI RtWritePortBufferUshort(
			PUSHORT Port,
			PUSHORT Buffer,
			ULONG Count);

RTBASEAPI
VOID RTAPI RtWritePortBufferUlong(
			PULONG Port,
			PULONG Buffer,
			ULONG Count);

RTBASEAPI
PVOID RTAPI RtMapMemory(
			LARGE_INTEGER PhysAddr,
			ULONG Length,
			BOOL CacheEnable);

RTBASEAPI
BOOL RTAPI RtUnmapMemory(PVOID VirtAddr);

RTBASEAPI
PVOID RTAPI RtAllocateLockedMemory(ULONG Size);

RTBASEAPI
BOOL RTAPI RtFreeLockedMemory(PVOID pVirtualAddress);

RTBASEAPI
PVOID RTAPI RtAllocateContiguousMemory(
			ULONG nNumberOfBytes,
			LARGE_INTEGER HighestAddress);

RTBASEAPI
BOOL RTAPI RtFreeContiguousMemory(PVOID pVirtualAddress);

RTBASEAPI
LARGE_INTEGER RTAPI RtGetPhysicalAddress(PVOID pVirtualAddress);

RTBASEAPI
PVOID RTAPI RtAllocateLocalMemory(ULONG Size);

RTBASEAPI
BOOL RTAPI RtFreeLocalMemory(PVOID pVirtualAddress);

RTBASEAPI
BOOL RTAPI 
RtQueryLocalMemory(PULONG MemSize, PULONG MemAvail, PULONG MemContig);

//
// RtLockProcess Flags
//
#define RT_PLOCK_BASIC	0x0001
#define RT_PLOCK_ALL	0xFFFF

RTBASEAPI
BOOL RTAPI RtLockProcess(ULONG Sections);

RTBASEAPI
BOOL RTAPI RtUnlockProcess(ULONG Sections);

//
// RtLockKernel Flags
//
#define RT_KLOCK_DATA		0x0001
#define RT_KLOCK_CODE		0x0002
#define RT_KLOCK_EXECUTIVE	(RT_KLOCK_DATA | RT_KLOCK_DATA)
#define RT_KLOCK_WIN32K		0x0004
#define RT_KLOCK_DRIVERS	0x0008
#define RT_KLOCK_ALL		0xFFFF

RTBASEAPI
BOOL RTAPI RtLockKernel(ULONG Sections);

RTBASEAPI
BOOL RTAPI RtUnlockKernel(ULONG Sections);

RTBASEAPI
BOOL RTAPI RtCommitLockStack(ULONG nNumberOfBytes);

RTBASEAPI
BOOL RTAPI RtCommitLockProcessHeap(
			ULONG nNumberOfBytes,
			VOID (RTFCNDCL *pExceptionRoutine)(HANDLE));

RTBASEAPI
BOOL RTAPI RtCommitLockHeap(
			HANDLE hHeap,
			ULONG nNumberOfBytes,
			VOID (RTFCNDCL *pExceptionRoutine)(HANDLE));


#if ( !defined(_NTDDK_) && !defined(_NTHAL_H_) && !defined(__RTX_H__) )
//
// Define the I/O bus interface types.
//
typedef enum _INTERFACE_TYPE {
    InterfaceTypeUndefined = -1,
    Internal,
    Isa,
    Eisa,
    MicroChannel,
    TurboChannel,
    PCIBus,
    VMEBus,
    NuBus,
    PCMCIABus,
    CBus,
    MPIBus,
    MPSABus,
    ProcessorInternal,
    InternalPowerBus,
    PNPISABus,
    MaximumInterfaceType
}INTERFACE_TYPE, *PINTERFACE_TYPE;

//
// Interrupt modes.
//
typedef enum _KINTERRUPT_MODE {
    LevelSensitive,
    Latched
    } KINTERRUPT_MODE;
#endif 


#if ( !defined(_NTDDK_) )

//
// Define types of bus information.
//
typedef enum _BUS_DATA_TYPE {
    ConfigurationSpaceUndefined = -1,
    Cmos,
    EisaConfiguration,
    Pos,
    CbusConfiguration,
    PCIConfiguration,
    VMEConfiguration,
    NuBusConfiguration,
    PCMCIAConfiguration,
    MPIConfiguration,
    MPSAConfiguration,
    PNPISAConfiguration,
    MaximumBusDataType
} BUS_DATA_TYPE, *PBUS_DATA_TYPE;


//
// Define the format of the PCI Slot parameter.
//
typedef struct _PCI_SLOT_NUMBER {
    union {
        struct {
            ULONG   DeviceNumber:5;
            ULONG   FunctionNumber:3;
            ULONG   Reserved:24;
        } bits;
        ULONG   AsULONG;
    } u;
} PCI_SLOT_NUMBER, *PPCI_SLOT_NUMBER;

#define PCI_TYPE0_ADDRESSES             6
#define PCI_TYPE1_ADDRESSES             2

//
// Define the standard PCI configuration information.
//
typedef struct _PCI_COMMON_CONFIG {
    USHORT  VendorID;                   // (ro)
    USHORT  DeviceID;                   // (ro)
    USHORT  Command;                    // Device control
    USHORT  Status;
    UCHAR   RevisionID;                 // (ro)
    UCHAR   ProgIf;                     // (ro)
    UCHAR   SubClass;                   // (ro)
    UCHAR   BaseClass;                  // (ro)
    UCHAR   CacheLineSize;              // (ro+)
    UCHAR   LatencyTimer;               // (ro+)
    UCHAR   HeaderType;                 // (ro)
    UCHAR   BIST;                       // Built in self test

    union {
        struct _PCI_HEADER_TYPE_0 {
            ULONG   BaseAddresses[PCI_TYPE0_ADDRESSES];
            ULONG   CIS;
            USHORT  SubVendorID;
            USHORT  SubSystemID;
            ULONG   ROMBaseAddress;
            ULONG   Reserved2[2];

            UCHAR   InterruptLine;      //
            UCHAR   InterruptPin;       // (ro)
            UCHAR   MinimumGrant;       // (ro)
            UCHAR   MaximumLatency;     // (ro)
        } type0;


    } u;

    UCHAR   DeviceSpecific[192];

} PCI_COMMON_CONFIG, *PPCI_COMMON_CONFIG;


#define PCI_COMMON_HDR_LENGTH (FIELD_OFFSET (PCI_COMMON_CONFIG, DeviceSpecific))

#define PCI_MAX_DEVICES                     32
#define PCI_MAX_FUNCTION                    8

#define PCI_INVALID_VENDORID                0xFFFF

//
// Bit encodings for  PCI_COMMON_CONFIG.HeaderType
//
#define PCI_MULTIFUNCTION                   0x80
#define PCI_DEVICE_TYPE                     0x00
#define PCI_BRIDGE_TYPE                     0x01

//
// Bit encodings for PCI_COMMON_CONFIG.Command
//
#define PCI_ENABLE_IO_SPACE                 0x0001
#define PCI_ENABLE_MEMORY_SPACE             0x0002
#define PCI_ENABLE_BUS_MASTER               0x0004
#define PCI_ENABLE_SPECIAL_CYCLES           0x0008
#define PCI_ENABLE_WRITE_AND_INVALIDATE     0x0010
#define PCI_ENABLE_VGA_COMPATIBLE_PALETTE   0x0020
#define PCI_ENABLE_PARITY                   0x0040  // (ro+)
#define PCI_ENABLE_WAIT_CYCLE               0x0080  // (ro+)
#define PCI_ENABLE_SERR                     0x0100  // (ro+)
#define PCI_ENABLE_FAST_BACK_TO_BACK        0x0200  // (ro)

//
// Bit encodings for PCI_COMMON_CONFIG.Status
//
#define PCI_STATUS_FAST_BACK_TO_BACK        0x0080  // (ro)
#define PCI_STATUS_DATA_PARITY_DETECTED     0x0100
#define PCI_STATUS_DEVSEL                   0x0600  // 2 bits wide
#define PCI_STATUS_SIGNALED_TARGET_ABORT    0x0800
#define PCI_STATUS_RECEIVED_TARGET_ABORT    0x1000
#define PCI_STATUS_RECEIVED_MASTER_ABORT    0x2000
#define PCI_STATUS_SIGNALED_SYSTEM_ERROR    0x4000
#define PCI_STATUS_DETECTED_PARITY_ERROR    0x8000

//
// Bit encodes for PCI_COMMON_CONFIG.u.type0.BaseAddresses
//
#define PCI_ADDRESS_IO_SPACE                0x00000001  // (ro)
#define PCI_ADDRESS_MEMORY_TYPE_MASK        0x00000006  // (ro)
#define PCI_ADDRESS_MEMORY_PREFETCHABLE     0x00000008  // (ro)

#define PCI_TYPE_32BIT      0
#define PCI_TYPE_20BIT      2
#define PCI_TYPE_64BIT      4

//
// Bit encodes for PCI_COMMON_CONFIG.u.type0.ROMBaseAddresses
//
#define PCI_ROMADDRESS_ENABLED              0x00000001

//
// Reference notes for PCI configuration fields:
//
// ro   these field are read only.  changes to these fields are ignored
//
// ro+  these field are intended to be read only and should be initialized
//      by the system to their proper values.  However, driver may change
//      these settings.
//

#endif // _NTDDK_

/* wsa
/*
/* Define the CE-style PnP ISA data structures/defines
 */
 typedef struct _ISA_PNP_LOGICAL_DEVICE_INFO
          {
		   DWORD LogicalDeviceID;
		   DWORD CompatibleIDs[8];
		  } ISA_PNP_LOGICAL_DEVICE_INFO, *PISA_PNP_LOGICAL_DEVICE_INFO;

 typedef struct _ISA_PNP_CONFIG
          {
		   DWORD VendorID;
		   DWORD SerialNumber;
		   DWORD NumberLogicalDevices;
		   ISA_PNP_LOGICAL_DEVICE_INFO LogicalDeviceInfo[8];
		  } ISA_PNP_CONFIG, *PISA_PNP_CONFIG;

 typedef struct _ISA_PNP_RESOURCES
          {
		   USHORT Flags;
		   struct
		    {
			 USHORT MemoryBase;
			 USHORT MemoryUpperLimit;
			 UCHAR MemoryControl;
			} Memory24Descriptors[4];
		   struct
		    {
			 DWORD MemoryBase;
			 DWORD MemoryUpperLimit;
			 UCHAR MemoryControl;
			} Memory32Descriptors[4];
           USHORT IoPortDescriptors[8];
		   struct
		    {
			 UCHAR IRQLevel;
			 UCHAR IRQType;
			} IRQDescriptors[2];
		   UCHAR DMADescriptors[2];
		  } ISA_PNP_RESOURCES, *PISA_PNP_RESOURCES;

#define ISA_PNP_RESOURCE_FLAG_ACTIVE 0x00000001

#if ( !defined(__RTX_H__) )
//
// Interrupt states.
// Return types for interrupt level routine.
//
typedef enum _INTERRUPT_DISPOSITION {
    PassToNextDevice,
    Dismiss,
    CallInterruptThread
    } INTERRUPT_DISPOSITION, *PINTERRUPT_DISPOSITION;
#endif 


RTBASEAPI
HANDLE RTAPI RtAttachInterruptVector(
                        PSECURITY_ATTRIBUTES pThreadAttributes,
                        ULONG StackSize,                        
                        VOID (RTFCNDCL *pRoutine)(PVOID context),
                        PVOID Context,
                        ULONG Priority,
                        INTERFACE_TYPE InterfaceType,
                        ULONG BusNumber,
                        ULONG BusInterruptLevel,
                        ULONG BusInterruptVector);

RTBASEAPI
HANDLE RTAPI RtAttachInterruptVectorEx(
                        PSECURITY_ATTRIBUTES pThreadAttributes,
                        ULONG StackSize,                        
                        BOOLEAN (RTFCNDCL *pRoutine)(PVOID context),
                        PVOID Context,
                        ULONG Priority,
                        INTERFACE_TYPE InterfaceType,
                        ULONG BusNumber,
                        ULONG BusInterruptLevel,
                        ULONG BusInterruptVector,
                        BOOLEAN Shared,
                        KINTERRUPT_MODE InterruptMode,
                        INTERRUPT_DISPOSITION (RTFCNDCL *MyInterrupt)(PVOID context));

RTBASEAPI
BOOL RTAPI RtReleaseInterruptVector(HANDLE hInterrupt);

RTBASEAPI
BOOL RTAPI RtDisableInterrupts(VOID);

RTBASEAPI
VOID RTAPI RtEnableInterrupts(VOID);

RTBASEAPI
ULONG RTAPI RtGetBusDataByOffset(
                        BUS_DATA_TYPE BusDataType,
                        ULONG BusNumber,
                        ULONG SlotNumber,
                        PVOID pBuffer,
                        ULONG Offset,
                        ULONG Length);

RTBASEAPI
ULONG RTAPI RtSetBusDataByOffset(
                        BUS_DATA_TYPE BusDataType,
                        ULONG BusNumber,
                        ULONG SlotNumber,
                        PVOID pBuffer,
                        ULONG Offset,
                        ULONG Length);

RTBASEAPI
BOOL RTAPI RtTranslateBusAddress(
                        INTERFACE_TYPE InterfaceType,
                        ULONG BusNumber,
                        LARGE_INTEGER BusAddress,
                        PULONG pAddressSpace,
                        PLARGE_INTEGER pTranslatedAddress);

RTBASEAPI
HANDLE RTAPI RtOpenProcess(
						DWORD dwAccess,			// ignored
						BOOL bInherit,			// ignored
						DWORD dwProcessId);

RTBASEAPI
BOOL RTAPI RtGetExitCodeProcess(
						HANDLE hProcess,
						LPDWORD lpExitCode);

RTBASEAPI
BOOL RTAPI RtTerminateProcess(
						HANDLE hProcess,
						UINT uExitCode);


#if !defined(_WINDEF_)
typedef struct _PROCESS_INFORMATION {
    HANDLE  hProcess;
    HANDLE  hThread;
    DWORD   dwProcessId;
    DWORD   dwThreadId;
} PROCESS_INFORMATION, *LPPROCESS_INFORMATION;

typedef struct _STARTUPINFOA {
    DWORD   cb;
    LPSTR   lpReserved;
    LPSTR   lpDesktop;
    LPSTR   lpTitle;
    DWORD   dwX;
    DWORD   dwY;
    DWORD   dwXSize;
    DWORD   dwYSize;
    DWORD   dwXCountChars;
    DWORD   dwYCountChars;
    DWORD   dwFillAttribute;
    DWORD   dwFlags;
    WORD    wShowWindow;
    WORD    cbReserved2;
    LPBYTE  lpReserved2;
    HANDLE  hStdInput;
    HANDLE  hStdOutput;
    HANDLE  hStdError;
} STARTUPINFOA, *LPSTARTUPINFOA;

typedef struct _STARTUPINFOW {
    DWORD   cb;
    LPWSTR  lpReserved;
    LPWSTR  lpDesktop;
    LPWSTR  lpTitle;
    DWORD   dwX;
    DWORD   dwY;
    DWORD   dwXSize;
    DWORD   dwYSize;
    DWORD   dwXCountChars;
    DWORD   dwYCountChars;
    DWORD   dwFillAttribute;
    DWORD   dwFlags;
    WORD    wShowWindow;
    WORD    cbReserved2;
    LPBYTE  lpReserved2;
    HANDLE  hStdInput;
    HANDLE  hStdOutput;
    HANDLE  hStdError;
} STARTUPINFOW, *LPSTARTUPINFOW;

#ifdef UNICODE
typedef STARTUPINFOW STARTUPINFO;
typedef LPSTARTUPINFOW LPSTARTUPINFO;
#else
typedef STARTUPINFOA STARTUPINFO;
typedef LPSTARTUPINFOA LPSTARTUPINFO;
#endif // UNICODE

#endif //!defined(_WINDEF_)

//
// dwCreationFlag values
//
#define CREATE_USE_LOCAL_MEMORY               0x00004000
#define CREATE_2003NET_DEBUG                  0x00008000

RTBASEAPI
BOOL RTAPI RtCreateProcessA(
						LPCSTR lpApplicationName,
						LPSTR lpCommandLine,
						LPSECURITY_ATTRIBUTES lpProcessAttributes,	// ignored 
						LPSECURITY_ATTRIBUTES lpThreadAttributes,	// ignored 
						BOOL bInheritHandles,						// ignored 
						DWORD dwCreationFlags,						// support CREATE_SUSPENDED
						LPVOID lpEnvironment,						// ignored 
						LPCSTR lpCurrentDirectory,					// ignored
						LPSTARTUPINFO lpStartupInfo,				// ignored
						LPPROCESS_INFORMATION lpProcessInformation);
RTBASEAPI
BOOL RTAPI RtCreateProcessW(
						LPCWSTR lpApplicationName,
						LPWSTR lpCommandLine,
						LPSECURITY_ATTRIBUTES lpProcessAttributes,	// ignored 
						LPSECURITY_ATTRIBUTES lpThreadAttributes,	// ignored 
						BOOL bInheritHandles,						// ignored 
						DWORD dwCreationFlags,						// support CREATE_SUSPENDED
						LPVOID lpEnvironment,						// ignored 
						LPCWSTR lpCurrentDirectory,					// ignored
						LPSTARTUPINFO lpStartupInfo,				// ignored
						LPPROCESS_INFORMATION lpProcessInformation);
#ifdef UNICODE
#define RtCreateProcess  RtCreateProcessW
#else
#define RtCreateProcess  RtCreateProcessA
#endif // !UNICODE

//
// support RTX application level logging
//
RTBASEAPI
BOOL RTAPI RtTraceEvent(
			ULONG TraceEventID, 
			PVOID arg1,
			PVOID arg2);

//
// support RTX application level perf measurement
//
#define PERF_MEASURE_HALISR_ENTEREXIT     0x00000001
#define PERF_MEASURE_HAL_CLOCKTICK        0x00000002
#define PERF_MEASURE_HAL_INTERNAL         0x00000004
#define PERF_MEASURE_RTSS_TIMERTICK       0x00000008
#define PLATFORM_CONFIG_UP_PIC            0x00000001
#define PLATFORM_CONFIG_UP_APIC           0x00000002
#define PLATFORM_CONFIG_MP_DEDICATED      0x00000004
#define PLATFORM_CONFIG_MP_SHARED         0x00000008


RTBASEAPI
ULONG
RTAPI
RtStartPerfMeasure(
	ULONG perfMeasureType,  //control code of operation to perform (input)
	ULONG *ioBuffer,        //buffer to pass data (output)
	ULONG ioBufferSize,     //buffer size (input)
	ULONG *ioBufferWrite,   //pointer to write index (output)
	LONGLONG *lPerfCyclesPerSecond, //pointer to pass PerfCyclesPerSecond (output)
	ULONG *platformConfig,   //pointer to pass platform configuration (output)
	ULONG *rtssProcessorNumber // pointer to pass RTSS processor number (output)
  );

RTBASEAPI
ULONG
RTAPI
RtStopPerfMeasure(
	ULONG perfMeasureType,
	ULONG *ioBuffer
  );

#ifdef __cplusplus
}
#endif

//
// Define RTX supported Win32 calls that are missing on Windows CE.
//
#if defined(UNDER_CE)
#	include <rtapice.h>
#endif // UNDER_CE

#endif // _RTAPI_H_

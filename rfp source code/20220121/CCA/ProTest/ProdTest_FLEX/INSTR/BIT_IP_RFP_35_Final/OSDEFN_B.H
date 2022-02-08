/******************************************************************************
* Project   : Pro 61                                                           *
*                                                                             *
* Module    : Kernel                                                          *
*                                                                             *
* Reference :                                                                 *
*                                                                             *
* File      : osdefn_b.h                                                      *
*                                                                             *
* Type      :                                                                 *
*                                                                             *
* Function  :                                                                 *
*                                                                             *
* Systems   :                                                                 *
*-----------------------------------------------------------------------------*
$Header:   M:/PROJECTS/PRO_71/DESIGN/SW/MODULE/KERNEL/SOURCE/vcs/osdefn_b.h_v   1.7   18 Jul 2000 10:45:34   khk  $
******************************************************************************/

#ifndef __OSDEFN_B_H
#define __OSDEFN_B_H

#ifndef __STDDEF_H
//  #include "header\stddef.h"
#endif

/****************************************************************************
* This header files defines constants for use with the operating system.    *
* Please use:                                                               *
*   #ifdef __TURBOC__  .... #endif                                          *
*   #ifndef __TURBOC__ .... #endif                                          *
* whenever applicable.                                                      *
****************************************************************************/

/* Public routines ***********************************************************/

extern void InternalIdleFunction(void);
extern void InternalTimerFunction(void);

/******************************************************************************
* Definitions of the identifiers for the different tasks in the system.       *
******************************************************************************/
typedef enum {
  // Insert task-identifiers here.
  // same in both handset and base
  TB_HANDLE_TASK=0x00,         /* MUST NOT BE MOVED, LHJ */
  BS_CCF = 0x01,
  BS_LLME = 0x02,
  BS_CSF = 0x03,
  BS_BBC_TASK = 0x04,
  LC_TASK=0x05, /* Dlc task */
  LAPC_TASK=0x06, /* Dlc Task */
  LCE_TASK=0x07,
  CC_TASK,
  MMTASK,
  CISS_TASK = 0x0A,
  BASE_CTRL_TASK = 0x0B,
  BASE_MENU_TASK,
  ISDNL1_TASK=0x0D,
  ISDNL2_TASK,
  ISDNL2M_TASK,
  ISDNL3_TASK,
  ABLINE_TASK,
  CTRL417_TASK,
  DSP_TASK,
  AUTH_TASK,
  AUTHFF_TASK,
  RFPCTRL_TASK,
  DLCME_TASK = 0x17, /* Dlc task */
  WATCH_TASK,
  ERROR_TASK,
  ISDNL3_LCETASK,
  V24_CALL_TASK = 0x1B,
  LU3_TASK = 0x1C, /* Dlc U-plane task */
  LU2_TASK = 0x1D, /* Dlc U-plane task */
  BASEKEY_TASK=0x1E,
  BASELED_TASK=0x1F,
  LISA_DATA_TASK=0x20,
  X75_TASK=0x21,
  GAP_IWU_TASK=0x22,
  X31_TASK=0x23,
  RECEIVER_NOT_DEFINED_TASK,    /* must be defined to receive mails to non existing tasks */
  NUMBER_OF_TASKS,
  TIMER_INTERRUPT_TASK,
  TESTBUS_INTERRUPT_TASK,
  MAC_INTERRUPT_TASK,
  ISDNL1_INT_TASK,
  NULL_TASK,
  NO_TASK=0xff
} TaskIdentifier;

/******************************************************************************
* Definitions of the identifiers for the different timers in the system.      *
******************************************************************************/
typedef enum {
  // Insert timer-identifiers here.
  BS_BBC_TIMER,
  ERROR_TASK_TIMER0,
  ERROR_TASK_TIMER1,
  ERROR_TASK_TIMER2,
  ERROR_TASK_TIMER3,
  ISDN_T200_TIMER0,
  ISDN_T200_TIMER1,
  ISDN_T200_TIMER2,
  ISDN_T200_TIMER3,
  ISDN_T200_TIMER4,
  ISDN_T200_TIMER5,
  ISDN_T200_TIMER6,
  ISDN_T200_TIMER7,
  ISDN_T202_TIMER0,
  ISDN_T202_TIMER1,
  ISDN_T202_TIMER2,
  ISDN_T202_TIMER3,
  ISDN_T202_TIMER4,
  ISDN_T202_TIMER5,
  ISDN_T202_TIMER6,
  ISDN_T202_TIMER7,
  ISDN_T203_TIMER0,
  ISDN_T203_TIMER1,
  ISDN_T203_TIMER2,
  ISDN_T203_TIMER3,
  ISDN_T203_TIMER4,
  ISDN_T203_TIMER5,
  ISDN_T203_TIMER6,
  ISDN_T203_TIMER7,
  ISDN_T3XX_TIMER0,
  ISDN_T3XX_TIMER1,
  ISDN_T3XX_TIMER2,
  ISDN_T3XX_TIMER3,
  ISDN_T3XX_TIMER4,
  ISDN_T3XX_TIMER5,
  ISDN_T3XX_TIMER6,
  ISDN_T3XX_TIMER7,
  ISDN_T3XX_TIMER8,
  ISDN_T3XX_TIMER9,
  ISDN_T3XX_TIMER10,
  ISDN_T3XX_TIMER11,
  ISDN_T3XX_TIMER12,
  ISDN_T3XX_TIMER13,
  ISDN_T3XX_TIMER14,
  ISDN_T3XX_TIMER15,
  ISDN_T322_TIMER0,
  ISDN_T322_TIMER1,
  ISDN_T322_TIMER2,
  ISDN_T322_TIMER3,
  ISDN_T322_TIMER4,
  ISDN_T322_TIMER5,
  ISDN_T322_TIMER6,
  ISDN_T322_TIMER7,
  ISDN_T322_TIMER8,
  ISDN_T322_TIMER9,
  ISDN_T322_TIMER10,
  ISDN_T322_TIMER11,
  ISDN_T322_TIMER12,
  ISDN_T322_TIMER13,
  ISDN_T322_TIMER14,
  ISDN_T322_TIMER15,

  ABLINE_START_RING_TIMER0,
  ABLINE_START_RING_TIMER1,
  ABLINE_STOP_RING_TIMER0,
  ABLINE_STOP_RING_TIMER1,

  CC_TIMER_0_0,
  CC_TIMER_0_1,
  CC_TIMER_0_2,
  CC_TIMER_1_0,
  CC_TIMER_1_1,
  CC_TIMER_1_2,
  CC_TIMER_2_0,
  CC_TIMER_2_1,
  CC_TIMER_2_2,
  CC_TIMER_3_0,
  CC_TIMER_3_1,
  CC_TIMER_3_2,
  CC_TIMER_4_0,
  CC_TIMER_4_1,
  CC_TIMER_4_2,
  CC_TIMER_5_0,
  CC_TIMER_5_1,
  CC_TIMER_5_2,
  CC_TIMER_6_0,
  CC_TIMER_6_1,
  CC_TIMER_6_2,
  CC_TIMER_7_0,
  CC_TIMER_7_1,
  CC_TIMER_7_2,
  CC_TIMER_8_0,
  CC_TIMER_8_1,
  CC_TIMER_8_2,
  CC_TIMER_9_0,
  CC_TIMER_9_1,
  CC_TIMER_9_2,
  CC_TIMER_10_0,
  CC_TIMER_10_1,
  CC_TIMER_10_2,
  CC_TIMER_11_0,
  CC_TIMER_11_1,
  CC_TIMER_11_2,
  CC_TIMER_12_0,
  CC_TIMER_12_1,
  CC_TIMER_12_2,
  CC_TIMER_13_0,
  CC_TIMER_13_1,
  CC_TIMER_13_2,
  CC_TIMER_14_0,
  CC_TIMER_14_1,
  CC_TIMER_14_2,
  CC_TIMER_15_0,
  CC_TIMER_15_1,
  CC_TIMER_15_2,
  CC_TIMER_16_0,
  CC_TIMER_16_1,
  CC_TIMER_16_2,
  CC_TIMER_17_0,
  CC_TIMER_17_1,
  CC_TIMER_17_2,
  CC_TIMER_18_0,
  CC_TIMER_18_1,
  CC_TIMER_18_2,
  CC_TIMER_19_0,
  CC_TIMER_19_1,
  CC_TIMER_19_2,

  MM_TIMER_UPDATE_TIME_0,
  MM_TIMER_0,
  MM_TIMER_1,
  MM_TIMER_2,
  MM_TIMER_3,
  MM_TIMER_4,
  MM_TIMER_5,
  MM_TIMER_6,
  MM_TIMER_7,
  MM_TIMER_8,
  MM_TIMER_9,
  MM_TIMER_10,
  MM_TIMER_11,

  DLCME_TIMER_1_0,
  DLCME_TIMER_1_1,
  DLCME_TIMER_1_2,
  DLCME_TIMER_1_3,
  DLCME_TIMER_1_4,
  DLCME_TIMER_1_5,
  DLCME_TIMER_1_6,
  DLCME_TIMER_1_7,
  DLCME_TIMER_1_8,
  DLCME_TIMER_1_9,
  DLCME_TIMER_1_10,
  DLCME_TIMER_1_11,

  DSP_TONE_PAUSE_TIMER,
  DSP_ANSWERTONE_SIGNAL_TIMER,

  LCE_TIMER_0,
  LCE_TIMER_1,
  LCE_TIMER_2,
  LCE_TIMER_3,
  LCE_TIMER_4,
  LCE_TIMER_5,
  LCE_TIMER_6,
  LCE_TIMER_7,
  LCE_TIMER_8,
  LCE_TIMER_9,
  LCE_TIMER_10,
  LCE_TIMER_11,
  LCE_TIMER_12,
  LCE_TIMER_13,
  LCE_TIMER_14,
  LCE_TIMER_15,
  LCE_TIMER_16,
  LCE_TIMER_17,
  LCE_TIMER_18,
  LCE_TIMER_19,

  LC_TIMER_1_0,
  LC_TIMER_1_1,
  LC_TIMER_1_2,
  LC_TIMER_1_3,
  LC_TIMER_1_4,
  LC_TIMER_1_5,
  LC_TIMER_1_6,
  LC_TIMER_1_7,
  LC_TIMER_1_8,
  LC_TIMER_1_9,
  LC_TIMER_1_10,
  LC_TIMER_1_11,

  LAPC_TIMER_1_0,
  LAPC_TIMER_1_1,
  LAPC_TIMER_1_2,
  LAPC_TIMER_1_3,
  LAPC_TIMER_1_4,
  LAPC_TIMER_1_5,
  LAPC_TIMER_1_6,
  LAPC_TIMER_1_7,
  LAPC_TIMER_1_8,
  LAPC_TIMER_1_9,
  LAPC_TIMER_1_10,
  LAPC_TIMER_1_11,

  LU3_TIMER_1_0,
  LU3_TIMER_1_1,
  LU2_TIMER_1_0,
  LU2_TIMER_1_1,

  MM_TIMER_RESET_TO_FAC_0,

  WATCH_MINUTE_TIMER,

  DSP_CALLWAITING_PAUSE_TIMER,

  ISDNL1_TIMER0,
  ISDNL1_TIMER1,

  BASEKEY_SCAN_TIMER,
  LED_BLINK_TIMER,

  MM_TIMER_FLUSH_ENROL_ARRAY_0,

  BC_CL_ANSWER_TIMER,
  BC_CALLBACK_TIMER_0,
  BC_CALLBACK_TIMER_1,
  BC_CALLBACK_TIMER_2,
  BC_CALLBACK_TIMER_3,
  BC_CALLBACK_TIMER_4,
  BC_HANDOVER_TIMER,

  GAP_IWU_TIMER_0,
  GAP_IWU_TIMER_1,
  GAP_IWU_TIMER_2,
  GAP_IWU_TIMER_3,
  GAP_IWU_TIMER_4,
  GAP_IWU_TIMER_5,
  GAP_IWU_TIMER_6,
  GAP_IWU_TIMER_7,
  GAP_IWU_TIMER_8,

  BASE_MENU_TIMER,

  NUMBER_OF_TIMERS,
  NO_TIMER
} TimerIdentifier;

/* Public variables **********************************************************/

#endif

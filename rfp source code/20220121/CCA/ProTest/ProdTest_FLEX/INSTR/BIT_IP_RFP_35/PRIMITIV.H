/******************************************************************************
* Project   : PRO-61                                                          *
*                                                                             *
* Module    : None                                                            *
*                                                                             *
* Reference : None                                                            *
*                                                                             *
* File      : primitiv.h                                                      *
*                                                                             *
* Type      : C Header file                                                   *
*                                                                             *
* Function  : Contains all primitives defined in system                       *
*                                                                             *
* Systems   : PRO-61                                                          *
*-----------------------------------------------------------------------------*
* Owner     : MS                                                              *
*******************************************************************************
$Header:   M:/PROJECTS/PRO_71/DESIGN/SW/HEADER/vcs/primitiv.h_v   1.19   24 Jul 2000 13:50:40   khk  $
******************************************************************************/

#ifndef __PRIMITIV_H
#define __PRIMITIV_H

typedef enum {
   DUMMY_DATA=0,
   TIMEOUT,

   /* Special ID procedure prim for RFP */
   RPF_ID_PROCEDURE_PRIM=0x08,


   /* testbus test primitives */
   TESTBUS_VERSION_READ_REQ=0x100,
   TESTBUS_VERSION_READ_CONFIRM,
   TESTBUS_LISA_CTRL,
   TESTBUS_LISA_BCHANNEL_DATA,
   /* TAPI PRIMITIVES */
   TAPI_REQ=0x180,
   TAPI_IND,

   /* modultst */
   INSTANCE_REQ=0x200,
   INSTANCE_CFM,
   STATE_REQ,
   STATE_CFM,

   /* IWF <-> LU3 primitives */
   DLU_LU3_DTR_GO_REQ=0x0600,
   DLU_LU3_DTR_GO_IND,
   DLU_LU3_DTR_STOP_REQ,
   DLU_LU3_DTR_STOP_IND,
   DLU_LU3_DATA_REQ,
   DLU_LU3_DATA_IND,

   /* LU3 <-> LU2 primitives */
   LU2_ESTABLISH_REQ=0x0620,
   LU2_ESTABLISH_CFM,
   LU2_DATA_REQ,
   LU2_DATA_IND,
   LU2_RELEASE_REQ,
   LU2_ERROR_IND,

   /* DLCME <-> LU3 */
   LU3_ESTABLISH_REQ=0x0640,
   LU3_ESTABLISH_CFM,
   LU3_RELEASE_REQ,

   /* LAPC <-> DLCME */
   LAPC_ESTABLISH_REQ=0x0660,
   LAPC_ESTABLISH_CFM,
   LAPC_HANDOVER_IND,
   LAPC_HANDOVER_COM_IND,
   LAPC_RELEASE_REQ,
   LAPC_RELEASE_CFM,
   LAPC_RELEASE_IND,
   LAPC_SUSPEND_REQ,
   LAPC_RESUME_REQ,
   LAPC_RESUME_CFM,


   /* Primitives to DLC task from 0x0700 */
   /* PRIMITIVES LCE <-> LAPC */
   DLC_ESTABLISH_REQ=0x0700,
   DLC_ESTABLISH_CFM,
   DLC_ESTABLISH_IND,

   DLC_DATA_REQ=0x0703,
   DLC_DATA_IND=0x0704,

   DLC_RELEASE_REQ,
   DLC_RELEASE_IND,
   DLC_RELEASE_CFM,

   DLC_SUSPEND_REQ=0x0710,
   DLC_SUSPEND_IND,
   DLC_SUSPEND_CFM,
   DLC_SUSPEND_RES,

   DLC_RESUME_REQ,
   DLC_RESUME_IND,
   DLC_RESUME_CFM,
   DLC_RESUME_RES,

   /* PRIMITIVES LCE <-> DLCME */
   DLCME_ENC_KEY_REQ=0x0720,
   DLCME_ENC_EKS_REQ,
   DLCME_ENC_EKS_CFM,
   DLCME_ENC_EKS_IND,

   DLCME_ESTABLISH_REQ,
   DLCME_ESTABLISH_CFM,
   DLCME_ESTABLISH_IND,

   DLCME_RELEASE_REQ,
   DLCME_RELEASE_CFM,
   DLCME_RELEASE_IND,
   DLCME_SET_PARAMETERS_REQ,
   DLCME_MOD_REQ,
   DLCME_MOD_CFM,
   DLCME_MOD_IND,

   DLCME_CON_UPLANE_REQ,
   DLCME_DIS_UPLANE_REQ,

   /* input Primitives to lce (NWL) task from 0x0800 */
   /* PRIMITIVES  CallControl <-> LCE */
   CC_DATA_REQ=0x0800,
   CC_DATA_IND,
   CC_RELEASE_LINK_REQ,
   CC_RELEASE_LINK_IND,
   CC_RELEASE_LINK_PAR_REQ,
   CC_SUSPEND_LINK_REQ,
   CC_SUSPEND_LINK_ALLOWED_REQ,
   CC_MODIFY_REQ,
   CC_MODIFY_IND,
   CC_MODIFY_CFM,
   CC_SET_PARAMETERS_REQ,
   CC_ESTABLISH_REQ,
   CC_ESTABLISH_CFM,
   CC_ESTABLISH_IND,
   CC_RELEASE_REQ,
   CC_RELEASE_CFM,
   CC_RELEASE_IND,
   /* Timeout LCE handset */
   LCE_TIMEOUT,

   /* App. layer -> LCE */
   LCE_CONN_UPLANE_REQ,
   LCE_DISCONN_UPLANE_REQ,

    /* input Primitives to lce (NWL) task from 0x0880 */
    /* PRIMITIVES  SS <-> LCE */
    SS_DATA_REQ=0x0880,
    SS_DATA_IND,
    SS_RELEASE_LINK_PAR_REQ,
    SS_RELEASE_LINK_IND,


    /* input Primitives to lce (NWL) task from 0x0900 */
    /* PRIMITIVES  MM <-> LCE */
    MM_DATA_REQ=0x0900,
    MM_DATA_IND,
    MM_RELEASE_LINK_REQ,
    MM_RELEASE_LINK_PAR_REQ,
    MM_RELEASE_LINK_IND,
    MM_ENC_KEY_REQ,
    MM_ENC_EKS_REQ,
    MM_MODULE_TEST_SETUP,
    MM_ENC_EKS_IND,

   /* V24_CALL_TASK -> USER_TASK */
   TAPI_SETUP_REQ,
   TAPI_RELEASE_REQ,
   TAPI_3PTY_BEGIN_REQ,
   TAPI_3PTY_END_REQ,
   TAPI_HOLD_REQ,
   TAPI_RETRIEVE_REQ,
   TAPI_PARK_REQ,
   TAPI_UNPARK_REQ,
   TAPI_ACCEPT_REQ,
   TAPI_DIAL_REQ,

   /* USER_TASK -> V24_CALL_TASK */
   TAPI_SETUP_ACK,
   TAPI_SETUP_REJ,
   TAPI_SETUP_IND,
   TAPI_ALERT_IND,
   TAPI_CONNECT_IND,
   TAPI_RELEASE_IND,
   TAPI_3PTY_BEGIN_IND,
   TAPI_3PTY_BEGIN_ACK,
   TAPI_3PTY_BEGIN_REJ,
   TAPI_3PTY_END_IND,
   TAPI_3PTY_END_ACK,
   TAPI_3PTY_END_REJ,
   TAPI_HOLD_IND,
   TAPI_HOLD_ACK,
   TAPI_HOLD_REJ,
   TAPI_RETRIEVE_IND,
   TAPI_RETRIEVE_ACK,
   TAPI_RETRIEVE_REJ,
   TAPI_PARK_IND,
   TAPI_PARK_ACK,
   TAPI_PARK_REJ,
   TAPI_UNPARK_IND,
   TAPI_UNPARK_ACK,
   TAPI_UNPARK_REJ,
   TAPI_STATEINFO_IND,
   TAPI_ACCEPT_ACK,
   TAPI_ACCEPT_REJ,
   TAPI_CHARGE_IND,

   /* LISA testbus -> V24_CALL_TASK */
   TAPI_DATA_CONNECT_IND,
   TAPI_DATA_DISCONNECT_IND,



   /* input Primitives to sessiontask from 0x0A00 */
   /* PRIMITIVES SESSION-TASK  <-> USERTASK */
   SESS_NO_SERVICE_IND=0x0A00,
   SESS_SERVICE_IND,
   SESS_SERVICE_REQ,
   SESS_ENROL_SEARCH_BASE_REQ,
   SESS_ENROL_SEARCH_BASE_IND,
   SESS_ENROL_START_REQ,
   SESS_ENROL_SUCCESS_IND,
   SESS_ENROL_REJECT_IND,
   SESS_DELETE_SYSTEM_IND,
   SESS_TERM_ACCESS_RIGHTS_REQ,
   SESS_GET_LOCAL_NUMBER_REQ,
   SESS_LOCAL_NUMBER_IND,
   SESS_MSN_NUMBER_IND,
   SESS_TIME_DATE_INFO_IND,
   SESS_ATTACH_REQ,
   SESS_DETACH_REQ,
   SESS_USER_AUTH_RES,
   SESS_USER_AUTH_IND,
   SESS_USER_AUTH_REJECT_IND,
   SESS_MAC_COVERAGE_LEVEL_IND,
   SESS_POWER_OFF_REQ,
   SESS_CONN_AUDIO_REQ,
   SESS_NEXT_ENROLLED_IND,
   SESS_RESET_TERM_CAP_REQ,
   SESS_EMERGENCY_NUMBER_IND,

   /* PRIMITIVES TO SESSION-TASK  (TIMERS)*/
   SESS_SEND_DETACH_TIMEOUT=0x0A80,
   SESS_OUT_OF_SERVICE_TIMEOUT,
   SESS_LOCATION_UPDATE_TIMEOUT,
   SESS_EXTERNAL_HANDOVER_TIMEOUT,
   SESS_FP_INVOKED_ENROL_START_REQ,

   /* input Primitives to cc (NWL) task from 0x0B00 */
   /* PRIMITIVES application layer <-> CallControl */
   MNCC_ALERT_IND=0x0B00,
   MNCC_ALERT_REQ,
   MNCC_CALL_PROC_IND,
   MNCC_CALL_PROC_REQ,
   MNCC_SETUP_ACK_IND,
   MNCC_SETUP_ACK_REQ,
   MNCC_CONNECT_REQ,
   MNCC_CONNECT_IND,
   MNCC_CONNECT_CFM,
   MNCC_CONNECT_ACK_REQ,
   MNCC_FACILITY_REQ,
   MNCC_FACILITY_IND,
   MNCC_HOLD_CFM,
   MNCC_HOLD_IND,
   MNCC_HOLD_RES,
   MNCC_HOLD_REQ,
   MNCC_INFO_IND,
   MNCC_INFO_REQ,
   MNCC_MODIFY_REQ,
   MNCC_MODIFY_IND,
   MNCC_MODIFY_CFM,
   MNCC_NOTIFY_IND,
   MNCC_NOTIFY_REQ,
   MNCC_REJECT_REQ,
   MNCC_REJECT_IND,
   MNCC_RELEASE_REQ,
   MNCC_RELEASE_IND,
   MNCC_RELEASE_CFM,
   MNCC_RELEASE_RES,
   MNCC_RETRIEVE_CFM,
   MNCC_RETRIEVE_IND,
   MNCC_RETRIEVE_RES,
   MNCC_RETRIEVE_REQ,
   MNCC_SERVICE_CHANGE_REQ,
   MNCC_SERVICE_ACC_CFM,
   MNCC_SERVICE_REJ_CFM,
   MNCC_SETUP_REQ,
   MNCC_SETUP_IND,
   CC_TIMEOUT,
   MNCC_IWU_INFO_IND,
   MNCC_IWU_INFO_REQ,
   MNCC_RELEASE_LINK_IND,
   MNCC_RELEASE_LINK_REQ,

    /* input Primitives to MM (NWL) task from 0x0C00 */
    /* PRIMITIVES application layer <-> MM */
    /* MM -> Userinterface */
    MNMM_FIRST_PRIMITIVE=0x0C00,  /* Shall be the first MNMM-primitive (AnJ 16/4-99) */
    MNMM_ACC_RIGHTS_TERM_ACC,
    MNMM_ACC_RIGHTS_TERM_REJ,
    MNMM_ACCESS_RIGHTS_ACC,
    MNMM_ACCESS_RIGHTS_REJ,
    MNMM_DELETE_SYSTEM_IND,
    MNMM_INFO_SUG,
    MNMM_INFO_IND,
    MNMM_INFO_REJ,
    MNMM_AUTH_PP_REQ,
    MNMM_AUTH_PP_REJ,
    MNMM_AUTH_FP_REJ,
    MNMM_AUTH_FP_ACC,
    MNMM_LOC_ACC,
    MNMM_LOC_REJ,
    MNMM_LOC_REJ_ABNORMAL,
    MNMM_ATTACH_SUC_IND,
    /* Userinterface -> MM */
    MNMM_ACC_RIGHTS_TERM_REQ,
    MNMM_ACCESS_RIGHTS_REQ,
    MNMM_ACC_RIGHTS_MODIFY_REQ,
    MNMM_INFO_REQ,
    MNMM_AUTH_PP_REP,
    MNMM_DETACH_REQ,
    MNMM_LOC_REQ,
    MNMM_RELEASE_REQ,
    MNMM_AUTH_FP_REQ,
    /*** MM -> BaseCtrl ***/
    MNMM_CIPHER_ACCEPT_CFM,
    MNMM_CIPHER_REJECT_CFM,
    MNMM_ENROLMENT_SUCCES_IND,
    MNMM_ATTACHED_SUCCES_IND,
    /*** BaseCtrl -> MM ***/
    MNMM_CIPHER_REQ,
    MNMM_TIME_UPDATED,
    MNMM_RESET_TO_FAC_SET,
    /*** BaseMenu -> MM ***/
    MNMM_ACCESS_RIGHTS_TERMINATE_REQ,
    MNMM_INFO_SUGGEST_REQ,
    MNMM_LAST_PRIMITIVE,  /* Shall be the last MNMM-primitive (AnJ 16/4-99) */

    /* Timeout MM */
    MM_TIMEOUT,
    MM_TIMEOUT_UPDATE_TIME,
    MM_TIMEOUT_RESET_TO_FAC_SET,
    MM_TIMEOUT_FLUSH_ENROL_ARRAY,

    /*Primitives between MM and authtasks*/
    AUTH_STOP,
    AUTH_AUTHENTICATE_REP,
    AUTH_AUTHENTICATE_REQ,
    AUTH_AUTHENTCIPHER_REQ,
    AUTH_AUTHDSAA_REQ,
    AUTHFF_REQ,
    AUTHFF_CONT,
    AUTHFF_STOP,
    AUTH_AUTH_NEXT,


    /* PRIMITIVES application layer <-> Ciss from 0x0D00 */
    /* Ciss -> App. layer */
    MNSS_SETUP_IND=0x0D00,
    MNSS_FACILITY_IND,
    MNSS_RELEASE_IND,
    MNSS_CL_FACILITY_IND,
    /* App. layer -> Ciss */
    MNSS_SETUP_REQ,
    MNSS_RELEASE_REQ,
    MNSS_FACILITY_REQ,
    MNSS_CL_FACILITY_REQ,

   /* input Primitives to Audio-buzzer task from 0x0E00 */
   AUDIO_OPEN=0x0E00,
   AUDIO_CLOSE,
   AUDIO_STEP_VOLUME_UP,
   AUDIO_STEP_VOLUME_DOWN,
   AUDIO_START_TONE,
   AUDIO_STOP_TONE,
   AUDIO_MUTE_MICROPHONE,
   AUDIO_UNMUTE_MICROPHONE,
   AUDIO_MUTE_EARPIECE,
   AUDIO_UNMUTE_EARPIECE,
   AUDIO_HANDSFREE_ENABLED,
   AUDIO_HANDSFREE_DISABLED,

   AUDIO_START_BUZZER_MELODY,
   AUDIO_STOP_BUZZER_MELODY,
   AUDIO_TEST_BUZZER_MELODY,
   AUDIO_SET_BUZZER_VOLUME,
   AUDIO_GENERATE_KEYCLICK,
   AUDIO_GENERATE_NOTIFY_BEEP,
   AUDIO_GENERATE_ERROR_BEEP,

   AUDIO_HEADSET_PRESENT,
   AUDIO_HEADSET_NOT_PRESENT,

   AUDIO_START_TREMBLER,
   AUDIO_STOP_TREMBLER,
   AUDIO_TREMBLER_TIMEOUT,

   /* input Primitives to Usertask (HS) from 0x0E50 */
   USER_NO_ACTION_TIMEOUT=0x0E50,
   USER_KEYPRESS_TIMEOUT,
   USER_ISDN_TIMEOUT,
   USER_UNMUTE_MICROPHONE,
   USER_AUTOTEST_TIMEOUT,
   USER_AUTOTEST_OFF_TIMEOUT,
   USER_AUTOTEST_ON_TIMEOUT,
   USER_CALLTIME_TIMEOUT,
   USER_LRMS_TIMEOUT,
   USER_SITESURVEY_TIMEOUT,
   USER_CALL_MISSED_TIMEOUT,

   USER_LAST_PRIMITIVE=0x0E7F,

   /* input Primitives to Listtask (HS) from 0x0E80 */

   /* Primitive UserTask->ListTask */
   LIST_ADD_REQ=0x0E80,
   LIST_EDIT_REQ,
   LIST_DELETE_REQ,
   LIST_DELETE_ALL_REQ,
   LIST_SEARCH_REQ,
   LIST_GET_REQ,
   LIST_SET_REQ,
   LIST_RESET_REQ,
   LIST_MASTER_RESET_REQ,

   /* Primitive ListTask->UserTask */
   LIST_ADD_IND,
   LIST_EDIT_IND,
   LIST_DELETE_IND,
   LIST_DELETE_ALL_IND,
   LIST_SEARCH_IND,
   LIST_GET_IND,
   LIST_SET_IND,
   LIST_RESET_IND,
   LIST_MASTER_RESET_IND,

   /* Primitive ListTask internal*/
   LIST_MOVE_INT,
   LIST_MASTER_RESET_INT,

   LIST_ADD_QUICKCALL_REQ,
   LIST_ADD_QUICKCALL_IND,
   LIST_SET_PROPERTY_REQ,
   LIST_SET_PROPERTY_IND,

   LIST_LAST_PRIMITIVE=0x0EFF,

   /* input Primitives to Application Layer (HS) from 0x0F00 */
   SESS_SETUP_REQ=0x0F00,
   SESS_ALERT_REQ,
   SESS_INFO_REQ,
   SESS_CONNECT_REQ,
   SESS_RELEASE_REQ,
   SESS_RELEASE_RES,
   SESS_REJECT_REQ,
   SESS_HOLD_REQ,
   SESS_RETRIEVE_REQ,
   SESS_SETUP_IND,
   SESS_SETUP_ACK_IND,
   SESS_CALL_PROC_IND,
   SESS_ALERT_IND,
   SESS_CONNECT_IND,
   SESS_CONNECT_CFM,
   SESS_HOLD_CFM,
   SESS_RETRIEVE_CFM,
   SESS_INFO_IND,
   SESS_IWU_INFO_IND,
   SESS_RELEASE_IND,
   SESS_RELEASE_CFM,
   SESS_REJECT_IND,
   SESS_CR_FACILITY_REQ,
   SESS_CI_FACILITY_REQ,
   SESS_ISDN_INFO_IND,
   SESS_CI_DISPLAY_IND,
   SESS_IWU_INFO_REQ,
   SESS_IIP_C_PLANE_IWU_INFO_REQ,
   SESS_IIP_C_PLANE_IWU_INFO_IND,
   SESS_IIP_C_PLANE_RELEASE_REQ,
   SESS_IIP_C_PLANE_RELEASE_IND,
   SESS_MODIFY_REQ,
   SESS_MODIFY_IND,
   SESS_MODIFY_CFM,

   /* input primitives to Call Task (HS) from 0x0F30 */
   CALL_SETUP_IND=0x0F30,
   CALL_CCBS_SETUP_REQ,
   CALL_CCBS_SETUP_IND,
   CALL_INFO_IND,
   CALL_CI_DISPLAY_IND,
   CALL_STATE_IND,
   CALL_SETUP_REQ,
   CALL_CONNECT_REQ,
   CALL_RELEASE_REQ,
   CALL_RELEASE_ALL_REQ,
   CALL_RECEIVE_REQ,
   CALL_REJECT_REQ,
   CALL_HOLD_REQ,
   CALL_RETRIEVE_REQ,
   CALL_INFO_REQ,
   CALL_AUTO_ANSWER_TIMEOUT,

   /* input primitives to Isdn Task from User Task (HS) from 0x0F50 */
   ISDN_INVOKE_MCID_REQ=0x0F50,
   ISDN_BEGIN_3PTY_REQ,
   ISDN_END_3PTY_REQ,
   ISDN_INVOKE_CD_REQ,
   ISDN_INVOKE_ECT_REQ,
   ISDN_ACTIVATE_CF_REQ,
   ISDN_DEACTIVATE_CF_REQ,
   ISDN_CF_STATUS_REQ,
   ISDN_INVOKE_CCBS_REQ,
   ISDN_INVOKE_CCNR_REQ,
   ISDN_CCBS_CALL_REQ,
   ISDN_TP_SUSPEND_REQ,
   ISDN_TP_RESUME_REQ,

   /* input primitives to User Task from Isdn Task from 0x0F70 */
   ISDN_MCID_SUCCESS_IND,
   ISDN_MCID_ERROR_IND,
   ISDN_CD_SUCCESS_IND,
   ISDN_CD_ERROR_IND,
   ISDN_ECT_SUCCESS_IND,
   ISDN_ECT_ERROR_IND,
   ISDN_BEGIN_3PTY_SUCCESS_IND,
   ISDN_BEGIN_3PTY_ERROR_IND,
   ISDN_END_3PTY_SUCCESS_IND,
   ISDN_END_3PTY_ERROR_IND,
   ISDN_ACTIVATE_CF_SUCCESS_IND,
   ISDN_ACTIVATE_CF_ERROR_IND,
   ISDN_DEACTIVATE_CF_SUCCESS_IND,
   ISDN_DEACTIVATE_CF_ERROR_IND,
   ISDN_INTERROGATE_CF_SUCCESS_IND,
   ISDN_INTERROGATE_CF_ERROR_IND,
   ISDN_CONNECTED_NO_IND,
   ISDN_MESSAGE_WAITING_IND,
   ISDN_CC_SUCCESS_IND,
   ISDN_CC_ERROR_IND,
   ISDN_CCBS_CALL_SETUP_IND,
   ISDN_SUSPEND_SUCCESS_IND,
   ISDN_SUSPEND_ERROR_IND,
   ISDN_RESUME_SUCCESS_IND,
   ISDN_RESUME_ERROR_IND,
   ISDN_NOTIFICATION_IND,
   ISDN_ACTIVE_CLEARING_IND,
   ISDN_HELD_CLEARING_IND,

   ISDN_T_ACTIVATE_TIMEOUT,
   ISDN_T_DEACTIVATE_TIMEOUT,
   ISDN_T_INTERROGATE_TIMEOUT,
   ISDN_AOC_IND,

   /* input primitives to Service Task from User Task (HS) from 0x0FA0 */
   SERVICE_SETUP_REQ=0x0FA0,
   SERVICE_INFO_REQ,

   /* input primitives to User Task from Service Task */
   SERVICE_SETUP_IND,
   SERVICE_RELEASE_IND,
   SERVICE_INFO_IND,

   /* Input primitives from the User Task to the Lrms Task */
   LRMS_URGENT_MSG_REQ=0x0FB0,
   LRMS_NORMAL_MSG_REQ,
   LRMS_MSG_ACK_REQ,
   LRMS_MSG_NACK_REQ,
   LRMS_MSG_USER_ACK_REQ,

   /* Input primitives to the User Task from the Lrms Task */
   LRMS_URGENT_MSG_IND,
   LRMS_NORMAL_MSG_IND,
   LRMS_MSG_PROGRESS_IND,
   LRMS_MSG_FAILED_IND,
   LRMS_MSG_SUCCESS_IND,

   /* Timeouts to the Lrms Task*/
   LRMS_MMS_SEND_RPY_TIMEOUT,
   LRMS_MMS_STATUS_RPY_TIMEOUT,

   NULLMSG=0x1000,
   /* TIMEOUT,  do not use find another name for your primitiv */
   DUMMY_PRIMITIVE,
   TIMEOUT_ERROR_TASK,
   TIMEOUT_NULL,


   /* Primitives to clear and exit testmode */
   SIM_TESTBUS_ENTER_TESTMODE,
   SIM_TESTBUS_EXIT_TESTMODE,

   /* Test primitives on mactest program cannot change value */
   SIM_MAC_PAGE_REQ=0x10C0,
   SIM_MAC_PAGE_IND,
   SIM_MAC_CON_REQ=0x10D0,
   SIM_MAC_CON_CFM,
   SIM_MAC_CON_IND,

   SIM_MAC_DIS_REQ,
   SIM_MAC_DIS_IND,

   SIM_MAC_CS_DATA_REQ,
   SIM_MAC_CS_DTR_IND,
   SIM_MAC_CS_DATA_IND,

   SIM_MAC_ENC_KEY_REQ,
   SIM_MAC_ENC_EKS_ENABLE_REQ,
   SIM_MAC_ENC_EKS_ENABLE_CFM,
   SIM_MAC_ENC_EKS_ENABLE_IND,
   SIM_MAC_ENC_EKS_DISABLE_REQ,
   SIM_MAC_ENC_EKS_DISABLE_CFM,
   SIM_MAC_ENC_EKS_DISABLE_IND,

   SIM_MAC_CONN_AUDIO_REQ,
   SIM_MAC_DISCONN_AUDIO_REQ,


   /* PRIMITIVES DLC <-> MAC */
   MAC_CON_REQ=0x1100,
   MAC_CON_IND,
   MAC_CON_CFM,
   MAC_CON_RES,

   MAC_MOD_REQ,
   MAC_MOD_IND,
   MAC_MOD_CFM,
   MAC_MOD_RES,

   MAC_CO_DATA_REQ,
   MAC_CO_DATA_IND,
   MAC_CO_DTR_IND,

   MAC_DIS_REQ,
   MAC_DIS_IND,
   MAC_DIS_CFM,
   MAC_DIS_RES,
   MAC_RES_DLC_IND,

   MAC_CONN_AUDIO_REQ,
   MAC_DISCONN_AUDIO_REQ,

   MAC_VOLUNTARY_CO_HO_IND,

   MAC_ENC_KEY_REQ,
   MAC_ENC_EKS_ENABLE_IND,
   MAC_ENC_EKS_IND,

   /* PRIMITIVES LCE <-> MAC */
   MAC_PAGE_IND=0x1200,
   MAC_PAGE_REQ,

   MAC_ACTIVATE_REQ,
   MAC_DEACTIVATE_REQ,    /* Added by FPG (RFP boot control) */

   /* Below primitive added by JBA 22-09-1999 for PRO_61 */
   /* This should exclude the need of differentation of crypted and clear */
   MAC_ENC_EKS_CFM,
   MAC_ENC_EKS_REQ,
   MAC_ENC_EKS_CRYPTED_REQ,
   MAC_ENC_EKS_CRYPTED_CFM,
   MAC_ENC_EKS_CRYPTED_IND,
   MAC_ENC_EKS_CRYPTED_RES,

   MAC_ENC_EKS_CLEAR_REQ,
   MAC_ENC_EKS_CLEAR_CFM,
   MAC_ENC_EKS_CLEAR_IND,
   MAC_ENC_EKS_CLEAR_RES,

   MAC_SERVICE_IND,
   MAC_SERVICE_REQ,
   MAC_ENROLL_REQ,
   MAC_ENROLL_CFM,
   MAC_MEASUREMENT_REQ,
   MAC_NO_SERVICE_IND,
   MAC_SET_PLI_REQ,
   MAC_EXT_HOV_IND,

   /* PRIMITIVES MAC <-> USER TASK */
   MAC_COVERAGE_LEVEL_IND=0x1280,

   /*---Internal MAC--------------------------------------*/
   MAC_ME_REL_RPT_IND=0x1300,

   MAC_BEARER_REQ,
   MAC_BEARER_IND,
   /*MAC_BEARER_RES,*/
   MAC_BEARER_HOV_IND,
   MAC_CON_HOV_IND,
   MAC_CON_HOV_REP,
   MAC_BEARER_ESTABLISHED_IND,
   MAC_BEARER_SETUP_FAILED_IND,
   MAC_BEARER_RELEASE_REQ,
   MAC_BEARER_RELEASE_IND,

   MAC_ENC_ENABLE_IND, /* encryption bearer enable ind */
   MAC_ENC_CHAN_REQ,   /* encryption key bearer   */

   MAC_BEARER_AUDIO_REQ,

   MAC_UNMUTE_AUDIOLINE_REQ,
   MAC_MUTE_AUDIOLINE_REQ,

   MAC_RSSI_SCAN_IND,
   MAC_RSSI_SCAN_REQ,
   MAC_STOP_RSSI_SCAN_REQ,

   MAC_ACTUAL_RSSI_IND,

   MAC_IDLE_LOCKED_IND,
   MAC_IDLE_UNLOCKED_IND,

   MAC_BROADCAST_CHANNEL_REQ,
   MAC_BROADCAST_CHANNEL_IND,
   MAC_RECOMMENDED_OTHER_BEARER_IND,
   MAC_OTHER_BEARER_IND,
   MAC_CORRUPT_BROADCAST_CHANNEL,

   MAC_MULTIFRAME_SYNC_IND,

   TIMEOUT_BBC_DUMMY,

   MAC_START_SEARCH_REQ,
   MAC_STOP_SEARCH_REQ,
   MAC_START_RESCUE,
   MAC_LOAD_INFO_IND,
   MAC_IDLE_RELOCKED_IND,
   MAC_NEW_BASE_IND,
   MAC_FORCE_BEARER_HANDOVER,

   TIMEOUT_CCF_HANDOVER,
   TIMEOUT_MAC_BASEINFO,
   TIMEOUT_MAC_LOADINFO,
   TIMEOUT_MAC_HANDOVER,
   TIMEOUT_CCF_RSSI_SCAN,
   TIMEOUT_HSCCF_T200,
   TIMEOUT_HSCCF_FREE_CHANNEL,
   TIMEOUT_HSCCF_STATE_TIMER,
   TIMEOUT_HSCCC_WAIT_FOR_SERVICE,
   MAC_BEARER_HANDOVER_REQ,
   MAC_ENC_EKS_ENABLE_REQ,
   MAC_ENC_EKS_ENABLE_CFM,
   MAC_ENC_ENABLE_REQ,
   MAC_ENC_ENABLE_CFM,
   MAC_ENC_DISABLE_REQ,
   MAC_ENC_DISABLE_CFM,
   MAC_ENC_EKS_DISABLE_CFM,
   MAC_ENC_EKS_DISABLE_REQ,
   TIMEOUT_SERVICE_RESCUE,
   TIMEOUT_HSCCF_BROADCAST,
   MAC_FORCED_TRANSMIT,

   /* Below primitive added by JBA 27-09-1999 for PRO_61 */
   MAC_CONNECT_UPLANE_REQ,
   MAC_DIS_UPLANE_REQ,

   MAC_DIS_SLOT_AUDIO_REQ,
   MAC_CONNECT_SLOT_AUDIO_REQ,

   MAC_START_CODEC_REQ,
   MAC_STOP_CODEC_REQ,

   MAC_CS_DATA_SEGMENT_REQ,
   MAC_CS_DATA_REQ,
   MAC_CS_DATA_IND,
   MAC_CS_DATA_DTR_IND,
   MAC_CS_DATA_SEGMENT_IND,
   MAC_CS_DTR_IND,
   MAC_CS_DATA_DTR_IND_ERROR,
   MAC_CS_DATA_DTR_IND_OK,

   MAC_NEW_RFPI,
   MAC_CORRUPT_CHANNEL,

   LC_DTR_TIMEOUT,

   /*-------------------------------------------------*/

   /* LC PRIMITIVES from 0x1400 */
   LC_RELEASE_REQ=0x1404,
   LC_RELEASE_CFM,

   LC_DATA_REQ=0x1408,
   LC_DATA_IND=0x140A,

   /* DLC timers */
   /* Changed by JBA 18-10-1999 */
   LAPC_TIMER_1_TIMEOUT,
   LAPC_TIMER_2_TIMEOUT,
   DLCME_TIMER_1_TIMEOUT,
   LC_TIMER_1_TIMEOUT,
   LU2_TIMER_1_TIMEOUT,
   LU3_TIMER_1_TIMEOUT,

   CCF_IDENTITY_RELEASE,

   PUT_KEY=0x16F0,
   KEY_PRESS,
/*   PUT_KEY=0x3000,*/
   TOGGLE_KEY_CLICK,
   TIMEOUT_KEYPAD,


    /* PRIMITIVES application layer <-> Llme from 0x1700 */
    /* App. layer -> Llme */
    LLME_ENROLMENT_ENABLE_REQ=0x1700,
    LLME_ENROLMENT_DISABLE_REQ,

    /* PRIMITIVES application layer <-> V.24 from 0x1740 */
    /* App. layer -> V.24 */
    V24_FIRST_PRIMITIVE=0x1740,   /* Shall be the first V24 primitive (Anj 27/10-99) */
    V24_CONNECT_REQ,
    V24_REJECT_REQ,
    V24_RELEASE_RES,
    V24_SETUP_REQ,
    V24_CONNECT_RES,
    V24_RELEASE_REQ,
    V24_SETUP_ACK_REQ,
    V24_ALERT_REQ,
    V24_CONNECT_AUDIO_REQ,
    V24_DISCONNECT_AUDIO_REQ,
    /* V.24 -> App. layer */
    V24_SETUP_IND,
    V24_INFO_IND,
    V24_REJECT_IND,
    V24_ALERT_IND,
    V24_CONNECT_IND,
    V24_RELEASE_IND,
    V24_LAST_PRIMITIVE,          /* Shall be the last V24-primitive (AnJ 27/10-99) */

    /* PRIMITIVES application layer <-> a_b lines from 0x1780 */
    /* App. layer -> a_b lines */
    AB_SETUP_ACK_REQ=0x1780,
    AB_ALERT_REQ,
    AB_CONNECT_REQ,
    AB_REJECT_REQ,
    AB_RELEASE_RES,
    AB_RELEASE_REQ,
    AB_SETUP_REQ,
    AB_CONNECT_ACK_REQ,
    AB_INFO_REQ,
    /* a_b lines -> App. layer */
    AB_SETUP_IND,
    AB_RELEASE_IND,
    AB_INFO_IND,
    AB_ALERT_IND,
    AB_CONNECT_IND,
    AB_REJECT_IND,
    AB_RELEASE_CFM,


    /* a_b lines -> DSP */
    DSP_DTMF_RECEIVER_ENABLE=0x1800,
    DSP_DTMF_RECEIVER_DISABLE,
    DSP_GENERATE_TONE,
    DSP_STOP_TONE,
    DSP_CONNECT_LINE_AUDIO,
    DSP_DISCONNECT_LINE_AUDIO,
    DSP_RX_AUDIO_MUTE,
    DSP_RX_AUDIO_UNMUTE,

    /* Hardware  ->  a_b lines*/
    AB_HOOK_OFF,
    AB_HOOK_ON,
    AB_R_KEY,
    AB_FRI,
    AB_OCCUPIED,
    ABLINE_START_RING_TIMEOUT,
    ABLINE_STOP_RING_TIMEOUT,

    /*AB_Line's Test Primitives  */
    AB_WRITE_EEPROM_RAP_MODE_TEST,           /*EEPROM VALUE */
    AB_WRITE_EEPROM_NOT_RAP_MODE_TEST,       /*EEPROM VALUE */
    AB_HW_HOOK_OFF_TEST,
    AB_HW_HOOK_ON_TEST,
    AB_HW_HOOK_ON_1_TEST,
    AB_HW_HOOK_OFF_1_TEST,
    AB_HW_HOOK_ON_2_TEST,
    AB_HW_HOOK_OFF_2_TEST,

    /* PRIMITIVES application layer <-> DSP from 0x1900*/
    /* App. layer -> DSP */
    DSP_ECHO_CONTROL_ENABLE=0x1900,
    DSP_ECHO_CONTROL_DISABLE,
    DSP_ANSWERTONE_DETECT_ENABLE,
    DSP_GENERATE_DTMF_TONE,
    DSP_CONNECT_AUDIO_REQ,
    DSP_DISCONNECT_AUDIO_REQ,
    DSP_CONNECT_3PTY_REQ,
    DSP_DISCONNECT_3PTY_REQ,

    /* DSP -> App. layer */
    DSP_DIALED_DIGIT_IND,
    DSP_ANSWERTONE_DETECTED,

    /* DSP -> DSP */
    DSP_GENERATE_DTMF_TIMEOUT_INT,

    /* DSP TASK Timeout Primitives*/
    DSP_TONE_PAUSE_TIMEOUT,
    DSP_ANSWERTONE_SIGNAL_TIMEOUT,
    DSP_CALLWAITING_PAUSE_TIMEOUT,

    /* DSP TASK TEST PRIMITIVES*/
    DSP_READ_RAM_REQ_TEST,
    DSP_READ_RAM_ACK_TEST,
    DSP_WRITE_ECP_REG_TEST,
    DSP_WRITE_DTMF_INDEX_REG_TEST,

    /*  */
    DSP_ECHO_SUPP_1_ENABLE,
    DSP_ECHO_SUPP_1_DISABLE,
    DSP_ECHO_SUPP_2_ENABLE,
    DSP_ECHO_SUPP_2_DISABLE,
    DSP_ART_ECHO_LOSS_1_24DB,
    DSP_ART_ECHO_LOSS_1_OFF,
    DSP_ART_ECHO_LOSS_2_24DB,
    DSP_ART_ECHO_LOSS_2_OFF,

    /*****************************/
    /* BaseCtrl -> ISDN */
    /*****************************/

    /* PRIMITIVES application layer <-> CTRL417 task from 0x1B80 */
    CTRL417_CONN_UPLANE_REQ=0x1B80,
    CTRL417_DISCONN_UPLANE_REQ,
    CTRL417_INIT,

    /**********************************************************************/
    /*   The order of the primitives between                              */
    /*   ISDN_FIRST_PRIMITIVE and ISDN_LAST_PRIMITIVE MUST not be changed */
    /**********************************************************************/
    ISDN_FIRST_PRIMITIVE=0x1bff,  /* Shall be the first ISDN-primitive (AnJ 10/6-99) */
    ISDN_ESCAPE_REQ,                        /*0x00*/
    ISDN_ALERTING_REQ,                      /*0x01*/
    ISDN_CALL_PROC_REQ,                     /*0x02*/
    ISDN_PROGRESS_REQ,                      /*0x03*/
    ISDN_SETUP_REQ=0x1c05,                  /*0x05*/
    ISDN_CONNECT_REQ=0x1c07,                /*0x07*/
    ISDN_SETUP_ACK_REQ=0x1c0d,              /*0x0D*/
    ISDN_CONNECT_ACKNOWLEDGE_REQ=0x1c0f,    /*0x0F*/
    ISDN_USER_INFO_REQ=0x1c20,              /*0x20*/
    ISDN_SUSPEND_REJECT_REQ,                /*0x21*/
    ISDN_RESUME_REJECT_REQ,                 /*0x22*/
    ISDN_HOLD_REQ=0x1c24,                   /*0x24*/
    ISDN_SUSPEND_REQ,                       /*0x25*/
    ISDN_RESUME_REQ,                        /*0x26*/
    ISDN_HOLD_ACKNOWLEDGE_REQ=0x1c28,       /*0x28*/
    ISDN_SUSPEND_ACKNOWLEDGE_REQ=0x1c2d,    /*0x2D*/
    ISDN_RESUME_ACKNOWLEDGE_REQ,            /*0x2E*/
    ISDN_HOLD_REJECT_REQ=0x1c30,            /*0x30*/
    ISDN_RETRIEVE_REQ,                      /*0x31*/
    ISDN_RETRIEVE_ACKNOWLEDGE_REQ=0x1c33,   /*0x33*/
    ISDN_RETRIEVE_REJECT_REQ=0x1c37,        /*0x37*/
    ISDN_DISCONNECT_REQ=0x1c45,             /*0x45*/
    ISDN_RESTART_REQ,                       /*0x46*/
    ISDN_RELEASE_REQ=0x1c4d,                /*0x4D*/
    ISDN_RESTART_ACKNOWLEDGE_REQ,           /*0x4E*/
    ISDN_RELEASE_RES=0x1c5a,                /*0x5A*/
    ISDN_SEGMENT_REQ=0x1c60,                /*0x60*/
    ISDN_FACILITY_REQ=0x1c62,               /*0x62*/
    ISDN_REGISTER_REQ=0x1c64,               /*0x64*/
    ISDN_STATUS_ENQUIRY_REQ=0x1c75,         /*0x75*/
    ISDN_CONGESTION_CONTROL_REQ=0x1c79,     /*0x79*/
    ISDN_INFO_REQ=0x1c7b,                   /*0x7B*/
    ISDN_STATUS_REQ=0x1c7d,                 /*0x7D*/
    ISDN_MORE_INFO_REQ=0x1ca0,              /*0xA0*/

    /*****************************/
    /* ISDN -> BaseCtrl */
    /*****************************/
    ISDN_ESCAPE_IND=0x1d00,                 /*0x00*/
    ISDN_ALERTING_IND,                      /*0x01*/
    ISDN_CALL_PROC_IND,                     /*0x02*/
    ISDN_PROGRESS_IND,                      /*0x03*/
    ISDN_SETUP_IND=0x1d05,                  /*0x05*/
    ISDN_CONNECT_IND=0x1d07,                /*0x07*/
    ISDN_SETUP_ACK_IND=0x1d0d,              /*0x0D*/
    ISDN_CONNECT_ACK_IND=0x1d0f,            /*0x0F*/
    ISDN_USER_INFO_IND=0x1d20,              /*0x20*/
    ISDN_SUSPEND_REJECT_IND,                /*0x21*/
    ISDN_RESUME_REJECT_IND,                 /*0x22*/
    ISDN_HOLD_IND=0x1d24,                   /*0x24*/
    ISDN_SUSPEND_IND,                       /*0x25*/
    ISDN_RESUME_IND,                        /*0x26*/
    ISDN_HOLD_ACK_CFM=0x1d28,               /*0x28*/
    ISDN_SUSPEND_ACK_IND=0x1d2d,            /*0x2D*/
    ISDN_RESUME_ACK_IND,                    /*0x2E*/
    ISDN_HOLD_REJECT_CFM=0x1d30,            /*0x30*/
    ISDN_RETRIEVE_IND,                      /*0x31*/
    ISDN_RETRIEVE_ACK_CFM=0x1d33,           /*0x33*/
    ISDN_RETRIEVE_REJECT_CFM=0x1d37,        /*0x37*/
    ISDN_DISCONNECT_IND=0x1d45,             /*0x45*/
    ISDN_RESTART_IND,                       /*0x46*/
    ISDN_RELEASE_IND=0x1d4d,                /*0x4D*/
    ISDN_RESTART_ACK_IND,                   /*0x4E*/
    ISDN_RELEASE_CFM=0x1d5a,                /*0x5A*/
    ISDN_SEGMENT_IND=0x1d60,                /*0x60*/
    ISDN_FACILITY_IND=0x1d62,               /*0x62*/
    ISDN_REGISTER_IND=0x1d64,               /*0x64*/
    ISDN_NOTIFY_IND=0x1d6e,                 /*0x6E*/
    ISDN_STATUS_ENQUIRY_IND=0x1d75,         /*0x75*/
    ISDN_CONGESTION_CONTROL_IND=0x1d79,     /*0x79*/
    ISDN_INFO_IND=0x1d7b,                   /*0x7B*/
    ISDN_STATUS_IND=0x1d7d,                 /*0x7D*/
    ISDN_MORE_INFO_IND=0x1da0,              /*0xA0*/

    ISDN_NONCONV_TABLE_PRIMITIVE,  /* Following primitives is not used in the "table conversion in isdn l3" */

    ISDN_SAPI12_SETUP_REQ,
    ISDN_IIP_SAPI12_SETUP_REQ,
    ISDN_SAPI12_SETUP_IND,
    ISDN_SAPI16_SETUP_REQ,
    ISDN_IIP_SAPI16_SETUP_REQ,
    ISDN_SAPI16_SETUP_IND,
    ISDN_REJECT_IND,
    ISDN_REJECT_REQ,
    ISDN_IIP_SETUP_REQ,
    ISDN_IIP_MORE_INFO_REQ,

    ISDN_CL_FACILITY_REQ=0x1dc5,
    ISDN_NOTIFY_REQ=0x1dd1,

    ISDN_CL_FACILITY_IND=0x1dd2,
    ISDN_CL_NOTIFY_IND=0x1dde,

    ISDN_IIP_ALERTING_REQ=0x1de1,
    ISDN_IIP_CALL_PROC_REQ=0x1de2,
    ISDN_IIP_CONNECT_REQ=0x1de7,
    ISDN_IIP_SETUP_ACK_REQ=0x1ded,
    ISDN_IIP_REJECT_REQ,
    ISDN_ENTER_IIP_REQ,

    ISDN_BUS_CONNECTED_IND,

    X31_CONFIG_REQ,          /* X31 primitives have to be placed between FIRST- and LAST ISDN primitive */
    X31_SETUP_REQ,
    X31_CLEAR_REQ,
    X31_CLEAR_RES,
    X31_CONNECT_IND,
    X31_CLEAR_CFM,
    X31_CLEAR_IND,
    ISDN_LAST_PRIMITIVE,  /* Shall be the last ISDN-primitive (AnJ 10/6-99) */

    /**********************************************************************/
    /*   The order of the primitives between                              */
    /*   ISDN_FIRST_PRIMITIVE and ISDN_LAST_PRIMITIVE MUST not be changed */
    /**********************************************************************/
    /*************************/
    /* BaseCtrl <-> BaseCtrl */
    /*************************/


   BC_CIPHER_REJECT_CFM=0x1E00,
   BC_ATTACHED_IND,
   BC_ENROLL_REQ,

   BC_CIPHER_REQ,
   BC_INFO_SUGGEST_REQ,

   /* Requests */
   BC_SETUP_HANDSET,
   BC_REJECT_HANDSET,
   BC_RELEASE_HANDSET,
   BC_FINISH_KNOCK_ON,
   BC_KEYPAD_INFO,
   /* Responses */
   BC_CALL_ACCEPTED,
   BC_CALL_FAILED,
   BC_CALL_REJECTED,
   BC_CALLEE_HOOK_OFF,
   BC_TAKE_OVER_EXT_CONV,

   BC_CISS_RELEASE_HANDSET,
   BC_CL_ANSWER_TIMEOUT,
   BC_CALLBACK_TIMEOUT,
   BC_HANDOVER_TIMEOUT,

    /***************************************************************/
    /* Primitives betweent BaseKey and BaseCtrl from 0x1E20       */
    /***************************************************************/

   BASE_KEY_PAGING=0x1E20,

    /***************************************************************/
    /* Primitives betweent BaseCtrl and BaseMenu from 0x1E30       */
    /***************************************************************/

      /************************/
      /* BaseCtrl -> BaseMenu */
      /************************/
   BSMENU_BASE_MENU_INIT=0x1E30,
   BSMENU_PP_KEY_INPUT,
   BSMENU_RESET_BASE_MENU,

      /************************/
      /* BaseMenu -> BaseCtrl */
      /************************/
   BSMENU_BASE_MENU_FINISHED,
      /************************/
      /* Timer ->    BaseMenu */
      /************************/
   BSMENU_TIMEOUT,




    /***************************************************************/
    /* Primitives betweent BaseCtrl and abLines                    */
    /***************************************************************/

      /**********************************/
      /* BaseCtrl -> abLine */
      /**********************************/
   /* See above */

      /**********************************/
      /* abLine -> BaseCtrl */
      /**********************************/
   /* See above */

    /***********************************************************/
    /* Primitives betweent BaseCtrl and Dsp                    */
    /***********************************************************/

      /**********************************/
      /* BaseCtrl -> Dsp */
      /**********************************/
   /* See above */

      /**********************************/
      /* Dsp -> BaseCtrl */
      /**********************************/
   /* See above */

    /***************************************************************/
    /* Primitives between BaseKey, BaseLed and BaseMenu            */
    /***************************************************************/
   BASELED_ENROLMENT_ENABLED=0x1E40,
   BASEKEY_RESET_TO_FACTORY_SETTINGS,
   SIM_BASEKEY_ON,
   SIM_BASEKEY_OFF,

    /***************************************************************/
    /* Timer-Primitives; BaseKey and BaseLed                       */
    /***************************************************************/
   BASEKEY_SCAN_TIMER_TIMEOUT,
   BASELED_BLINK_TIMER_TIMEOUT,

    /***************************************************************/
    /* Timer-Primitives; GapIwu                                    */
    /***************************************************************/
   GAP_IWU_TIMEOUT=0x1E50,

   /**********************************/
   /* Watch task                     */
   /**********************************/

   WATCH_INCREMENT_MINUTE=0x1F00,


   ERROR_PRIM_00=0x2030,
/*   ERROR_PRIM_00=0x4000, */
   ERROR_PRIM_01,
   ERROR_PRIM_02,
   ERROR_PRIM_03,
   ERROR_PRIM_04,
   ERROR_PRIM_05,
   ERROR_PRIM_06,
   ERROR_PRIM_07,
   ERROR_PRIM_08,
   ERROR_PRIM_09,
   ERROR_PRIM_10,
   ERROR_PRIM_11,
   ERROR_PRIM_12,
   ERROR_PRIM_13,
   ERROR_PRIM_14,
   ERROR_PRIM_15,
   ERROR_PRIM_16,
   ERROR_PRIM_17,
   ERROR_PRIM_18,
   ERROR_PRIM_19,
   ERROR_PRIM_20,
   ERROR_PRIM_21,
   ERROR_PRIM_22,
   ERROR_PRIM_23,
   ERROR_PRIM_24,
   ERROR_PRIM_25,
   ERROR_PRIM_26,
   ERROR_PRIM_27,
   ERROR_PRIM_28,
   ERROR_PRIM_29,
   ERROR_PRIM_30,
   ERROR_PRIM_31,

   MODULTEST_TIMEOUT=0x2200,



   ERROR_LAST_ERROR_PRIMITIVE=0x2fff,
/*   ERROR_LAST_ERROR_PRIMITIVE=0x5000,*/


   TEST_EEPROM_STATUS_REQ=0x3702,
   TEST_EEPROM_STATUS_REP=0x3703,
/*   TEST_EEPROM_STATUS_REQ=0x3702,*/
/*   TEST_EEPROM_STATUS_REP=0x3703,*/

   TEST_FORCED_DUMMYBEARER=0x3800,
   TEST_BMC_SWITCHANTENNA=0x3801,
   TEST_MAC_SET_FORCED_TRANSMIT=0x3802,
   TEST_MAC_SET_LOOP_BACK=0x3803,
   TEST_MAC_SET_FORCE_BEARER_HANDOVER=0x3804,

/* * Following are primitives for the Isdn protocol stack **/

/* Primitives between Isdn layer 2 and Application layer */
    ISDN_APP_RNR=0x4000,
    ISDN_APP_RR,

/* Primitives between Isdn layer 2 and layer 3 */
    ISDN_DL_ESTABLISH_REQ,
    ISDN_DL_ESTABLISH_CFM,
    ISDN_DL_ESTABLISH_IND,
    ISDN_DL_RELEASE_REQ,
    ISDN_DL_RELEASE_CFM,
    ISDN_DL_RELEASE_IND,
    ISDN_DL_DATA_REQ,
    ISDN_DL_DATA_IND,
    ISDN_DL_UNIT_DATA_REQ,
    ISDN_DL_UNIT_DATA_IND,
    ISDN_L2_ESTABLISH_IND,
    ISDN_L2_ESTABLISH_CFM,
    ISDN_L2_RELEASE_IND,
    ISDN_IIP_L2_RELEASE_REQ,
    ISDN_IIP_L2_RELEASE_IND,
/* Primitives between Isdn layer 1 and layer 2 */
    ISDNL1_ACT_TIMEOUT,
    ISDNL1_DEACT_TIMEOUT,
    ISDN_PH_TESTLOOP_DATA,
    ISDN_PH_RESET_REQ,
    ISDN_PH_DATA_REQ,
    ISDN_PH_DATA_IND,
    ISDN_PH_ACTIVATE_REQ,
    ISDN_PH_ACTIVATE_IND,
    ISDN_PH_DEACTIVATE_IND,
    ISDN_PH_DEACTIVATE_REQ,
    ISDN_PH_SEND_DATA,
    ISDN_PH_RECEIVE_DATA,
/* Primitives between Isdn layer 2 and layer 2 management*/
    ISDN_MDL_ASSIGN_IND,
    ISDN_MDL_ASSIGN_REQ,
    ISDN_MDL_ERROR_IND,
    ISDN_MDL_ERROR_RES,
    ISDN_MDL_REMOVE_REQ,
/* Primitives for layer 2 Timerout*/
    ISDN_T200_TIMEOUT,
    ISDN_T202_TIMEOUT,
    ISDN_T203_TIMEOUT,
/* Primitives for layer 2 Timerout*/
    ISDN_T3XX_TIMEOUT,
    ISDN_T322_TIMEOUT,
/* Primitives internal at Isdn layer 2 */
    ISDN_DL_DEACTIVATE_IND,

/* * Isdn primitive end **/

/* Primitives for timeout at the Rci control*/
    RCIMMI_TIMEOUT,
    RCIMMI_LED_TIMEOUT,

    RCIMMI_LED_ON,
    RCIMMI_LED_OFF,


/***************************************************************/
/* Primitives between Display Task and User Task               */
/***************************************************************/


    /* User Task  ->   Display Task   */
     DISPLAY_SET_LANGUAGE=0x4100,
     DISPLAY_SHOW_TEXT,
     DISPLAY_SHOW_STRING,
     DISPLAY_SHOW_STRING_AND_CURSOR,
     DISPLAY_SHOW_LARGE_STRING,
     DISPLAY_SHOW_LARGE_STRING_AND_CURSOR,
     DISPLAY_SHOW_MULTI_LINE_STRING,
     DISPLAY_SHOW_MULTI_LINE_STRING_AND_CURSOR,
     DISPLAY_SET_CURSOR,
     DISPLAY_SET_LARGE_STRING_CURSOR,
     DISPLAY_SET_MULTI_LINE_CURSOR,
     DISPLAY_CLEAR_CURSOR,
     DISPLAY_SET_SOFTICON,
     DISPLAY_SET_COVERAGE_ICON,
     DISPLAY_SET_BATTERY_ICON,
     DISPLAY_SHOW_SOFT_ICON_LINE,
     DISPLAY_SHOW_SOFTKEY_LINE,
     DISPLAY_SHOW_ROLL_STRING,
     DISPLAY_SHOW_BITMAP_PIC,
     DISPLAY_OFF,
     DISPLAY_SHOW_INTRO,
     DISPLAY_REMOVE_INTRO,
     DISPLAY_SET_LIGHT_ON,
     DISPLAY_SET_LIGHT_OFF,
     DISPLAY_SET_LED,
     DISPLAY_CLEAR_ALL,
     DISPLAY_SET_UNDERLINE_HEADERLINE,
     DISPLAY_RESET_UNDERLINE_HEADERLINE,


   /*Display Task Timeout Primitives*/
     DISPLAY_TIMEOUT,

   /*Additional Display Test Primitives*/
     DISPLAY_DBG_USE_TEST_FONTS,
     DISPLAY_DBG_NOT_USE_TEST_FONTS,
     DISPLAY_DBG_PAINT_BLACK,
     DISPLAY_DBG_PAINT_WHITE,
     DISPLAY_DBG_SET_BLINK_FLAG,
     DISPLAY_DBG_RESET_BLINK_FLAG,
     DISPLAY_DBG_SEND_TEXT_REQ,
     DISPLAY_DBG_SEND_TEXT_RES,
     DISPLAY_DBG_SEND_LANGUAGE_REQ,
     DISPLAY_DBG_SEND_LANGUAGE_RES,
     DISPLAY_DBG_SET_TO_MANUAL_CONTROL_REQ,
     DISPLAY_DBG_RESET_MANUAL_CONTROL_REQ,
     DISPLAY_DBG_BANK_DATA_REQ,
     DISPLAY_DBG_BANK_DATA_RES,
     DISPLAY_DBG_BANK_DATA_EXTRACT_REQ,
     DISPLAY_DBG_BANK_DATA_EXTRACT_RES,
     DISPLAY_DBG_BLINK_FLAG_AUT_CNTR,
     DISPLAY_DBG_BLINK_FLAG_MAN_CNTR,
     DISPLAY_DBG_SHIFTED_AND_MASKED_DATA_REQ,
     DISPLAY_DBG_SHIFTED_AND_MASKED_DATA_RES,
     DISPLAY_DBG_SHIFTED_AND_MASKED_DATA_EXTRACT_REQ,
     DISPLAY_DBG_SHIFTED_AND_MASKED_DATA_EXTRACT_RES,

/***************************************************************/
/* Primitives between Display Task and IIC Driver Task         */
/***************************************************************/

    /* Display Task ->   IIC Driver Task  */
     DISPLAY_DATA_READY,
     DISPLAY_FIELD_UPDATE,


/***************************************************************/
/* Primitives for testing and adjusting IIC Driver Task        */
/***************************************************************/

    /* RSX  ->   IIC Driver Task  */
     DISPLAY_SET_TEMP_COEF,
     DISPLAY_SET_BIAS,
     DISPLAY_SET_VOP,
     DISPLAY_SET_CONTROLS,

    /* ??? Task  ->   IIC Driver Task  */
     DISPLAY_INIT_DISPLAY,

/***************************************************************/
/* Battery Management Task Interface Primitives                */
/***************************************************************/

    /*User Task  ->   Battery Management Task  */
     CHARGER_PRESENT=0x4200,
     CHARGER_NOT_PRESENT,
   /*  CHARGER_PP_START_HANDSFREE,*/
  /*   CHARGER_PP_END_HANDSFREE,   */
     CHARGER_PP_START_CHARGE_LED_ON,
     CHARGER_PP_END_CHARGE_LED_ON,
     CHARGER_PP_START_OUT_OF_SERVICE,
     CHARGER_PP_END_OUT_OF_SERVICE,
     CHARGER_PP_START_CONVERSATION,
     CHARGER_PP_END_CONVERSATION,
     CHARGER_PP_START_DATA_FULL_SLOT,
     CHARGER_PP_END_DATA_FULL_SLOT,
     CHARGER_PP_START_DATA_DOUBLE_SLOT,
     CHARGER_PP_END_DATA_DOUBLE_SLOT,
     CHARGER_PP_START_DEFAULT_COND,
     CHARGER_PP_END_DEFAULT_COND,


    /* Battery Management Task -> User Task    */
     CHARGER_CAPACITY_STATUS_IND,

    /*Power Management Task  ->   Battery Management Task  */
   /*  CHARGER_PP_STATE_TIME_INFO_IND,*/

    /* Battery Management Task Charger Switch Timeout */
     CHARGER_TURN_OFF_IND,
     CHARGER_PP_TURN_OFF_REQ,

     CHARGER_UPDATE_TIMEOUT,
     CHARGER_SWITCH_OFF_TIMEOUT,

    /* LF department primitive for testing Battery Management Task */
     CHARGER_TEST_STATE_STATUS_REQ,
     CHARGER_TEST_STATE_STATUS_RES,

    /* Battery Management Task Test Primitives available if DISP_TEST is defined*/
/*     CHARGER_TEST_BAT_SIM_INIT_AND_RESTART_REQ,*/
     CHARGER_TEST_BAT_REPLACED_FLAG_REQ,
     CHARGER_TEST_SET_BAT_VOLT_REQ,
     CHARGER_TEST_RESTART_REQ,
     CHARGER_TEST_VERIFY_STATE_MACHINE_REQ,
     CHARGER_TEST_UPDATE_FREERUNCOUNTER,
     CHARGER_TEST_SET_EEPROM_ERC_REQ,
     CHARGER_TEST_SET_EEPROM_UNK_CAP_ERC_REQ,
     CHARGER_TEST_START_TIMER,
     CHARGER_TEST_START_SWITCH_TIMER,
     CHARGER_TEST_START_TIMER_LOOPED,
     CHARGER_TEST_USEC_DELAY,


/*****************************************************************/
/* RFP SYNC primitives                                           */
/*****************************************************************/
    SYNC_SYNCSTART_REQ,
    SYNC_MULTIFRAME_ERROR_IND,

/*****************************************************************/
/* RFP LED primitives                                            */
/*****************************************************************/
    LED_TIMEOUT,
    LED_CHANGE_STATE,

/*****************************************************************/
/* RFP UpN primitives                                            */
/*****************************************************************/

    UPN_SPI_PACKET,
    UPN_INIT,
    UPN_FRAME,
    UPN_DEBUG,
    UPN_PLUGNPLAY_TIMEOUT,

/*****************************************************************/
/* RFP control primitives                                        */
/*****************************************************************/

    /* Upn interface primitives  */

    RFP_UPN_ESTABLISH_IND,
    UPN_ESTABLISH_IND,
    RFP_UPN_RELEASE_IND,
    UPN_RELEASE_IND,
    RFP_UPN_RELEASE_REQ,
    UPN_RELEASE_REQ,

   /* RFP control timer primitives...defined in Timers.H  */

    RFP_IDLE_TIMEOUT,
    RFP_DOWNLOAD_TIMEOUT,
    RFP_UPN_TIMEOUT,
    RFP_DEACTIVATED_TIMEOUT,
    RFP_POWERMEASURE_TIMEOUT,
    RFP_SYNC_TIMEOUT,
    RFP_UPN_RELEASE_TIMEOUT,
    RFP_T_BIT_TIMEOUT,
    RFP_UPN2_TIMEOUT,

    /* RFP internal sync primitives */

    SYNC_START_REQ,
    SYNC_STOP_REQ,
    SYNC_STATUS_IND,

    /* RFP - PABX primitives */

    RFP_READY_IND=0x7801,
    RFP_INIT_REQ=0x7802,
    RFP_INIT_CFM=0x7803,
    RFP_NEW_UPN_DETECTED_IND=0x7804,
    RFP_SARI_LIST_REQ=0x7805,
    RFP_CHANGE_HIGHER_LAYER_CAP_REQ=0x7806,
    RFP_CHANGE_HIGHER_LAYER_CAP_CFM=0x7807,
    RFP_CHANGE_STATUS_INFO_REQ=0x7808,
    RFP_CHANGE_STATUS_INFO_CFM=0x7809,
    RFP_CHANGE_CABLE_DELAY_REQ=0x780A,
    RFP_CHANGE_CABLE_DELAY_CFM=0x780B,
    RFP_MULTIFRAME_NUMBER_REQ=0x780C,
    RFP_SYNC_STARTED_IND=0x780D,
    RFP_SYNCHRONIZED_IND=0x780E,
    RFP_ACTIVATE_REQ=0x780F,
    RFP_ACTIVATE_CFM=0x7810,
    RFP_DEACTIVATE_REQ=0x7811,
    RFP_DEACTIVATE_CFM=0x7812,
    RFP_RESET_REQ=0x7813,
    RFP_ECHO_ID_REQ=0x7814,
    RFP_ECHO_ID_CFM=0x7815,
    RFP_STATISTICS_DATA_REQ=0x7816,
    RFP_STATISTICS_DATA_CFM=0x7817,
    RFP_ERROR_IND=0x7818,
    RFP_BOOT_READY_IND=0x7819,
    RFP_LEAVE_BOOT_REQ=0x781A,

    /* RFP download primitives  */

    RFP_DOWNLOAD_FIRST_REQ=0x781B,
    RFP_DOWNLOAD_NEXT_REQ=0x781C,
    RFP_DOWNLOAD_END_REQ=0x781D,
    RFP_DOWNLOAD_END_CFM=0x781E,
    RFP_DOWNLOAD_CFM=0x781F,
    RFP_CHANGE_MAC_LAYER_CAP_REQ=0x7820,
    RFP_CHANGE_MAC_LAYER_CAP_CFM=0x7821,

    /* 7900-79FF / reserved for PABX-LC primitives */
    RFP_LC_DATA_REQ=0x7905,
    RFP_LC_DATA_IND=0x7906,
    RFP_LC_DTR_IND =0x7907,

    /* 7a00-7aFF / reserved for PABX-MAC primitives */
    RFP_MAC_CON_IND=0x7A01,
    RFP_MAC_DIS_REQ=0x7A02,
    RFP_MAC_DIS_IND=0x7A03,
    RFP_MAC_MODEM_DETECT_IND=0x7A04,
    RFP_MAC_PAGE_REQ=0x7A08,
    RFP_MAC_ENC_KEY_REQ=0x7A09,
    RFP_MAC_ENC_EKS_IND=0x7A0A,
    RFP_MAC_HO_IN_PROGRESS_IND=0x7A0B,
    RFP_MAC_HO_IN_PROGRESS_RES=0x7A0C,
    RFP_MAC_HO_FAILED_IND=0x7A0D,
    RFP_MAC_HO_FAILED_REQ=0x7A0E,
    RFP_MAC_ASSIGN_UPN_CHANNEL_REQ=0x7A0F,
    RFP_MAC_ASSIGN_UPN_CHANNEL_CFM=0x7A10,
    RFP_MAC_SWITCH_UPN_CHANNEL_REQ=0x7A11,
    RFP_MAC_SWITCH_UPN_CHANNEL_CFM=0x7A12,
    RFP_MAC_SWITCH_UPN_CHANNEL_COMPLETE_REQ=0x7A13,
    RFP_MAC_RFP_ERROR_IND=0x7A14,

    /* 7b00-7bFF / reserved for PABX-RFP debug primitives */

    RFP_DEBUG_REQ=0x7B00,

    /* End RFP control primitives */

   TEST_PRIMITIVE,

/*   NUMBER_OF_PRIMITIVES,*/

      /* Primitives used by remote mail trace */
      SIM_MAILTRACE_START=0x8010,
      SIM_MAILTRACE_START_CONFIRM,
      SIM_MAILTRACE_STOP,
      SIM_MAILTRACE_STOP_CONFIRM,
      SIM_MAILTRACE_DOWNLOAD_OPTIONS_1,
      SIM_MAILTRACE_DOWNLOAD_OPTIONS_2,
      SIM_MAILTRACE_DOWNLOAD_OPTIONS_CONFIRM,
      SIM_MAILTRACE_TRACE_INPUT_MAIL,
      SIM_MAILTRACE_TRACE_OUTPUT_MAIL,
      SIM_MAILTRACE_TRACE_TIMER,
      SIM_MAILTRACE_TRACE_DATA,
      SIM_MAILTRACE_TRACE_EVENT,
      SIM_MAILTRACE_DOWNLOAD_OPTIONS_3,
      /* Primitives used by the eeprom editor */
      SIM_EEPROM_WRITE_DATA=0x8020,
      SIM_EEPROM_WRITE_DATA_CONFIRM,
      SIM_EEPROM_REQUEST_DATA,
      SIM_EEPROM_REQUEST_DATA_CONFIRM,

      /* Module test primitives */
      SIM_MODULE_TEST_START=0x8030,
      SIM_MODULE_TEST_STOP,
      SIM_MODULE_MAIL,
      RESET_TASK,

      /* mactest program primitives */
      SIM_MAC_DOWNLOAD_OPTIONS=0x8050,
      SIM_MAC_DOWNLOAD_OPTIONS_CONFIRM,

      /* Send from the debug environment to a task, use it instead of getch etc. */
      KEY_MESSAGE=0x8060,

      /* Remote memory monitor */
      SIM_MEMORY_WRITE=0x8070,
      SIM_MEMORY_READ,
      SIM_MEMORY_READ_CONFIRM,
      SIM_MEMORY_W32_WRITE,
      SIM_MEMORY_W32_READ,
      SIM_MEMORY_W32_READ_CONFIRM,
      /* The bmc status, rssi, crc, phase etc., trace program */
      SIM_BMC_TRACE_START_REQ=0x80A0,
      SIM_BMC_TRACE_START_CFM,
      SIM_BMC_TRACE_STOP_REQ,
      SIM_BMC_TRACE_STOP_CFM,
      SIM_BMC_TRACE_MSG,
      /* Bmc a fields logging */
      SIM_AFIELD_START_REQ=0x80B0,
      SIM_AFIELD_START_CFM,
      SIM_AFIELD_STOP_REQ,
      SIM_AFIELD_STOP_CFM,
      SIM_AFIELD_DATA_INPUT,
      SIM_AFIELD_DATA_OUTPUT,


      /* Trap primitives */

      SIM_TRAP=0x80C0,
      SIM_SET_WRITE_TRAP,



/* Temporary primitives for Lu7Test */
      T417_CALLTESTFUNC=0xeff0,



/* TEST PRIMITIVES USED FOR PRODUCTION TEST */
      /* General Test Primitives from 0xF000 */
      TEST_INITIALIZATION_FINISHED=0xF000,
      TEST_GET_COMP_TIME_REQ,
      TEST_GET_COMP_TIME_CFM,
      TEST_ENTER_TESTMODE=0xF004,
      TEST_EXIT_TESTMODE,
      TEST_GET_VERSION_REQ=0xF008,
      TEST_GET_VERSION_CFM,
      TEST_READ_LAST_ERROR_REQ,
      TEST_READ_LAST_ERROR_CFM,
      TEST_CLEAR_LAST_ERROR_REQ,
      TEST_CLEAR_LAST_ERROR_CFM,
      TEST_PRINTF,
      TEST_SW_RUN_TIME_ERROR,


      /* Read_Write RAM Primitives from 0xF020 */
      TEST_READ_RAM_REQ=0xF020,
      TEST_READ_RAM_CFM,
      TEST_WRITE_RAM_REQ,
      TEST_WRITE_RAM_CFM,
      TEST_MODIFY_RAM_REQ,
      TEST_MODIFY_RAM_CFM,

      /* Read_Write EEPROM Primitives from 0xF040 */
      TEST_READ_EEPROM_REQ=0xF040,
      TEST_READ_EEPROM_CFM,
      TEST_WRITE_EEPROM_REQ,
      TEST_WRITE_EEPROM_CFM,

      /* RF Test Primitives from 0xF100 */
      TEST_BMC_DEFAULT_RX=0xF100,
      TEST_BMC_DEFAULT_TX,
      TEST_BMC_SET_TX_CHANNEL,
      TEST_BMC_SET_RX_CHANNEL,
      TEST_SETUP_DUMMY_BEARER,  /* For FP */
      TEST_FORCED_TRANSMIT,     /* For PP */
      TEST_BMC_SET_A_FIELD,
      TEST_BMC_SET_B_FIELD,
      TEST_BMC_SET_TX_CHANNEL_NO_MOD,
      TEST_BMC_ENTER_LOWLEVEL_TESTMODE,
      TEST_BMC_RSSI_INIT,
      TEST_BMC_GET_RSSI_REQ,
      TEST_BMC_GET_RSSI_CFM,
      TEST_BMC_SET_DIVERSITY,
      TEST_BMC_SWITCH_ANTENNA,
      TEST_BMC_SET_QUADRATURE,
      TEST_BMC_SET_VCO_SWITCH,
      TEST_BMC_INIT_QUADRATURE,
      TEST_BMC_SET_QUADRATURE_DC_OFFSET,
      TEST_BMC_SET_DEVIATION,
      TEST_BMC_READ_EEPROM_QUADRATURE_REQ,
      TEST_BMC_READ_EEPROM_QUADRATURE_CFM,
      TEST_BMC_WRITE_EEPROM_QUADRATURE_REQ,
      TEST_BMC_WRITE_EEPROM_QUADRATURE_CFM,
      TEST_BMC_READ_EEPROM_QUAD_DC_OFFSET_REQ,
      TEST_BMC_READ_EEPROM_QUAD_DC_OFFSET_CFM,
      TEST_BMC_WRITE_EEPROM_QUAD_DC_OFFSET_REQ,
      TEST_BMC_WRITE_EEPROM_QUAD_DC_OFFSET_CFM,
      TEST_BMC_READ_EEPROM_DEVIATION_REQ,
      TEST_BMC_READ_EEPROM_DEVIATION_CFM,
      TEST_BMC_WRITE_EEPROM_DEVIATION_REQ,
      TEST_BMC_WRITE_EEPROM_DEVIATION_CFM,
      TEST_BMC_RF_SETLING_RX_TX,
      TEST_MAC_CONNECT_UPLANE,

      /* LF Test Primitives from 0xF140 */
      TEST_LOOP_ADPCM_AUDIO=0xF140,
      TEST_READ_ADC_REQ,
      TEST_READ_ADC_CFM,
      TEST_AUDIO_START_TONE,
      TEST_AUDIO_STOP_TONE,
      TEST_AUDIO_START_MELODY,
      TEST_AUDIO_STOP_MELODY,
      TEST_AUDIO_MUTE_MIC,
      TEST_AUDIO_UNMUTE_MIC,
      TEST_AUDIO_MUTE_AUDIO,
      TEST_AUDIO_UNMUTE_AUDIO,
      TEST_SET_VCC1,
      TEST_SET_VREF_CODEC,
      TEST_AUDIO_TREMBLER_ON,
      TEST_AUDIO_TREMBLER_OFF,
      TEST_KEY_PUT_KEY,
      TEST_KEY_PRESSED_REQ,
      TEST_KEY_PRESSED_CFM,
      TEST_APP_SERVICE_STATE_REQ,
      TEST_APP_SERVICE_STATE_CFM,
      TEST_APP_AUDIO_STATE_REQ,
      TEST_APP_AUDIO_STATE_CFM,
      TEST_SET_CLOCK,
      TEST_DISP_LED_ON,
      TEST_DISP_LED_OFF,
      TEST_DISP_BACKLIGHT_ON,
      TEST_DISP_BACKLIGHT_OFF,
      TEST_ENABLE_CHARGER,
      TEST_DISABLE_CHARGER,
      TEST_READ_EEPROM_CLOCK_REQ,
      TEST_READ_EEPROM_CLOCK_CFM,
      TEST_WRITE_EEPROM_CLOCK_REQ,
      TEST_WRITE_EEPROM_CLOCK_CFM,
      TEST_RFP_GET_SYNC_STATE_REQ,
      TEST_RFP_GET_SYNC_STATE_CFM,
      TEST_CHECK_417_REQ,
      TEST_CHECK_417_CFM,
      TEST_DISABLE_ANTENNAS_REQ,
      TEST_DISABLE_ANTENNAS_CFM,

      PRIM_NO_LAST=0xfff0               /* RH 20.05.99 TSE does not eccept the value 0xffe0!! */
#ifdef PC_SIMULATION
       ,                                 /* this trick is makes emulator mailtrace conversion work */
#endif

} PrimitiveType;

#endif


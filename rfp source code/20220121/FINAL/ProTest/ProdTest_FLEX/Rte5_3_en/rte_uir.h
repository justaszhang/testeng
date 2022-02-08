/**************************************************************************/
/* LabWindows/CVI User Interface Resource (UIR) Include File              */
/* Copyright (c) National Instruments 2021. All Rights Reserved.          */
/*                                                                        */
/* WARNING: Do not add to, delete from, or otherwise modify the contents  */
/*          of this include file.                                         */
/**************************************************************************/

#include <userint.h>

#ifdef __cplusplus
    extern "C" {
#endif

     /* Panels and Controls: */

#define  EDITSEQU                         1
#define  EDITSEQU_SEQUENCEDISPLAY         2       /* control type: listBox, callback function: displseqcallb */
#define  EDITSEQU_SEQNAME                 3       /* control type: string, callback function: (none) */
#define  EDITSEQU_NUMTESTS                4       /* control type: numeric, callback function: (none) */
#define  EDITSEQU_SAVE                    5       /* control type: command, callback function: edit_sequ */
#define  EDITSEQU_CANCEL                  6       /* control type: command, callback function: edit_sequ */
#define  EDITSEQU_REMOVE                  7       /* control type: command, callback function: edit_sequ */
#define  EDITSEQU_INSERTB                 8       /* control type: command, callback function: edit_sequ */
#define  EDITSEQU_INSERT                  9       /* control type: command, callback function: edit_sequ */
#define  EDITSEQU_EDITHEAD                10      /* control type: command, callback function: edit_sequglob */
#define  EDITSEQU_REMHEAD                 11      /* control type: command, callback function: edit_sequ */
#define  EDITSEQU_ADDHEAD                 12      /* control type: command, callback function: edit_sequ */
#define  EDITSEQU_MODULES                 13      /* control type: listBox, callback function: edit_sequ */
#define  EDITSEQU_PROTOTYPES              14      /* control type: listBox, callback function: edit_sequ */
#define  EDITSEQU_DECORATION              15      /* control type: deco, callback function: (none) */

#define  EDITTEST                         2
#define  EDITTEST_PRECOND                 2       /* control type: ring, callback function: (none) */
#define  EDITTEST_PRECISION               3       /* control type: ring, callback function: (none) */
#define  EDITTEST_RUNMODE                 4       /* control type: ring, callback function: (none) */
#define  EDITTEST_INDEX                   5       /* control type: numeric, callback function: (none) */
#define  EDITTEST_RESOFFSET               6       /* control type: numeric, callback function: (none) */
#define  EDITTEST_UPLIMIT                 7       /* control type: numeric, callback function: (none) */
#define  EDITTEST_LOWLIMIT                8       /* control type: numeric, callback function: (none) */
#define  EDITTEST_LOOPCOUNT               9       /* control type: numeric, callback function: (none) */
#define  EDITTEST_MESUNIT                 10      /* control type: string, callback function: (none) */
#define  EDITTEST_STRING4                 11      /* control type: string, callback function: (none) */
#define  EDITTEST_STRING3                 12      /* control type: string, callback function: (none) */
#define  EDITTEST_STRING2                 13      /* control type: string, callback function: (none) */
#define  EDITTEST_STRING1                 14      /* control type: string, callback function: (none) */
#define  EDITTEST_ERRORMESSAGE            15      /* control type: string, callback function: (none) */
#define  EDITTEST_NAME                    16      /* control type: string, callback function: (none) */
#define  EDITTEST_MODULE                  17      /* control type: string, callback function: (none) */
#define  EDITTEST_LISTDBL                 18      /* control type: listBox, callback function: edit_callb */
#define  EDITTEST_LISTINT                 19      /* control type: listBox, callback function: edit_callb */
#define  EDITTEST_CANCEL                  20      /* control type: command, callback function: edit_callb */
#define  EDITTEST_GETNEW                  21      /* control type: command, callback function: edit_callb */
#define  EDITTEST_DECORATION              22      /* control type: deco, callback function: (none) */
#define  EDITTEST_TEXTMSG                 23      /* control type: textMsg, callback function: (none) */

#define  LOGIN                            3
#define  LOGIN_OKBUTTON                   2       /* control type: command, callback function: login_callb */
#define  LOGIN_USERNAME                   3       /* control type: string, callback function: (none) */
#define  LOGIN_LOGINLEVEL                 4       /* control type: string, callback function: (none) */
#define  LOGIN_DECORATION                 5       /* control type: deco, callback function: (none) */
#define  LOGIN_ICON                       6       /* control type: picture, callback function: (none) */
#define  LOGIN_TESTVERSION                7       /* control type: textMsg, callback function: (none) */
#define  LOGIN_TEXT2                      8       /* control type: textMsg, callback function: (none) */
#define  LOGIN_CVITESTEXECUTIVE           9       /* control type: textMsg, callback function: (none) */

#define  PANEL1                           4       /* callback function: panel1 */
#define  PANEL1_CLRRESULTD                2       /* control type: command, callback function: panel1_callb */
#define  PANEL1_RUNSINGLEPR               3       /* control type: command, callback function: panel1_callb */
#define  PANEL1_RUNABORTR                 4       /* control type: command, callback function: panel1_callb */
#define  PANEL1_RUNLOOPR                  5       /* control type: command, callback function: panel1_callb */
#define  PANEL1_RUNSINGLEPL               6       /* control type: command, callback function: panel1_callb */
#define  PANEL1_RUNABORTL                 7       /* control type: command, callback function: panel1_callb */
#define  PANEL1_RUNLOOPL                  8       /* control type: command, callback function: panel1_callb */
#define  PANEL1_PRODUCTNUMBERR            9       /* control type: string, callback function: (none) */
#define  PANEL1_PRODUCTNUMBERL            10      /* control type: string, callback function: (none) */
#define  PANEL1_CURRENTLOGIN              11      /* control type: string, callback function: panel1_callb */
#define  PANEL1_SEQNAMER                  12      /* control type: string, callback function: (none) */
#define  PANEL1_SEQNAMEL                  13      /* control type: string, callback function: (none) */
#define  PANEL1_SEQUENCEDISPLAYR          14      /* control type: listBox, callback function: displseqcallb */
#define  PANEL1_SEQUENCEDISPLAYL          15      /* control type: listBox, callback function: displseqcallb */
#define  PANEL1_ERRORRATER                16      /* control type: scale, callback function: (none) */
#define  PANEL1_NUMTESTSR                 17      /* control type: numeric, callback function: (none) */
#define  PANEL1_NUMTESTSL                 18      /* control type: numeric, callback function: (none) */
#define  PANEL1_RESULTDISPLAY             19      /* control type: textBox, callback function: (none) */
#define  PANEL1_ERRORRATEL                20      /* control type: scale, callback function: (none) */
#define  PANEL1_SERNUML                   21      /* control type: string, callback function: (none) */
#define  PANEL1_SERNUMR                   22      /* control type: string, callback function: (none) */
#define  PANEL1_CURRENTSTATUSR            23      /* control type: textMsg, callback function: (none) */
#define  PANEL1_CURRENTSTATUSL            24      /* control type: textMsg, callback function: (none) */
#define  PANEL1_TIMER                     25      /* control type: timer, callback function: timer_callb */
#define  PANEL1_HINTLEFT                  26      /* control type: textMsg, callback function: (none) */
#define  PANEL1_HINTRIGHT                 27      /* control type: textMsg, callback function: (none) */
#define  PANEL1_ONLYMEASUREMSG            28      /* control type: textMsg, callback function: (none) */

#define  SEQU_HEAD                        5
#define  SEQU_HEAD_SEQNAME                2       /* control type: string, callback function: (none) */
#define  SEQU_HEAD_CLEANUP                3       /* control type: string, callback function: (none) */
#define  SEQU_HEAD_IOERR                  4       /* control type: string, callback function: (none) */
#define  SEQU_HEAD_FLOW                   5       /* control type: string, callback function: (none) */
#define  SEQU_HEAD_STARTUP                6       /* control type: string, callback function: (none) */
#define  SEQU_HEAD_DLL_LIB                7       /* control type: string, callback function: (none) */
#define  SEQU_HEAD_WORKDIR                8       /* control type: string, callback function: (none) */
#define  SEQU_HEAD_LOGFILE                9       /* control type: string, callback function: (none) */
#define  SEQU_HEAD_FIX_ID                 10      /* control type: string, callback function: (none) */
#define  SEQU_HEAD_IOERRFCT               11      /* control type: string, callback function: (none) */
#define  SEQU_HEAD_FLOWFCT                12      /* control type: string, callback function: (none) */
#define  SEQU_HEAD_CLEANFCT               13      /* control type: string, callback function: (none) */
#define  SEQU_HEAD_STARTFCT               14      /* control type: string, callback function: (none) */
#define  SEQU_HEAD_PRINTPORT              15      /* control type: string, callback function: (none) */
#define  SEQU_HEAD_CANCEL                 16      /* control type: command, callback function: edit_sequhead */
#define  SEQU_HEAD_GETNEW                 17      /* control type: command, callback function: edit_sequhead */
#define  SEQU_HEAD_MAXERROR               18      /* control type: numeric, callback function: (none) */

#define  SEQU_SEL                         6
#define  SEQU_SEL_SEQU_SELWIND            2       /* control type: listBox, callback function: sequ_selectwindow */
#define  SEQU_SEL_QUIT                    3       /* control type: command, callback function: sequ_selcallb */
#define  SEQU_SEL_OKAY                    4       /* control type: command, callback function: sequ_selcallb */
#define  SEQU_SEL_PRODUCTNUMBER           5       /* control type: string, callback function: (none) */
#define  SEQU_SEL_STATUS                  6       /* control type: textMsg, callback function: (none) */

#define  SYSTEM                           7
#define  SYSTEM_PRINTMODE                 2       /* control type: ring, callback function: (none) */
#define  SYSTEM_RESULTDCLR                3       /* control type: ring, callback function: (none) */
#define  SYSTEM_SYSEVENT                  4       /* control type: ring, callback function: (none) */
#define  SYSTEM_RESULTD                   5       /* control type: ring, callback function: (none) */
#define  SYSTEM_DATALOG                   6       /* control type: radioButton, callback function: (none) */
#define  SYSTEM_MAXERROR                  7       /* control type: numeric, callback function: (none) */
#define  SYSTEM_SHOWRATE                  8       /* control type: radioButton, callback function: (none) */


     /* Control Arrays: */

          /* (no control arrays in the resource file) */


     /* Menu Bars, Menus, and Menu Items: */

#define  MENU1                            1
#define  MENU1_FILE                       2       /* callback function: menu1 */
#define  MENU1_FILE_LOGIN                 3       /* callback function: menu1 */
#define  MENU1_FILE_LOADSEQUENCE          4       /* callback function: menu1 */
#define  MENU1_FILE_LOADSEQUENCE_SUBMENU  5
#define  MENU1_FILE_LOADSEQUENCE_SEQUL    6       /* callback function: menu1 */
#define  MENU1_FILE_LOADSEQUENCE_SEQUR    7       /* callback function: menu1 */
#define  MENU1_FILE_EDITSEQUENCE          8       /* callback function: menu1 */
#define  MENU1_FILE_EDITSEQUENCE_SUBMENU  9
#define  MENU1_FILE_EDITSEQUENCE_SEQUL    10      /* callback function: menu1 */
#define  MENU1_FILE_EDITSEQUENCE_SEQUR    11      /* callback function: menu1 */
#define  MENU1_FILE_EDITSEQUENCE_NEWSEQU  12      /* callback function: menu1 */
#define  MENU1_ERRORRATE                  13      /* callback function: menu1 */
#define  MENU1_ERRORRATE_SHOWER           14      /* callback function: menu1 */
#define  MENU1_ERRORRATE_RESETER          15      /* callback function: menu1 */
#define  MENU1_PRINT                      16      /* callback function: menu1 */
#define  MENU1_PRINT_PRINTON              17      /* callback function: menu1 */
#define  MENU1_PRINT_SEPARATOR            18
#define  MENU1_PRINT_PRINTERROR           19      /* callback function: menu1 */
#define  MENU1_PRINT_PRINTALL             20      /* callback function: menu1 */
#define  MENU1_SPECIAL                    21      /* callback function: menu1 */
#define  MENU1_SPECIAL_REFERENCE          22      /* callback function: menu1 */
#define  MENU1_SPECIAL_REPAIR             23      /* callback function: menu1 */
#define  MENU1_SPECIAL_DEBUG              24      /* callback function: menu1 */
#define  MENU1_SPECIAL_SEPARATOR          25
#define  MENU1_SPECIAL_MAXERROR           26      /* callback function: menu1 */
#define  MENU1_SETS                       27      /* callback function: menu1 */
#define  MENU1_SETS_RDISP                 28      /* callback function: menu1 */
#define  MENU1_SETS_RDISP_SUBMENU         29
#define  MENU1_SETS_RDISP_CLREVERYTEST    30      /* callback function: menu1 */
#define  MENU1_SETS_RDISP_CLREVERYLOOP    31      /* callback function: menu1 */
#define  MENU1_SETS_RDISP_SEPERATOR1      32
#define  MENU1_SETS_RDISP_ONLYERROR       33      /* callback function: menu1 */
#define  MENU1_SETS_SYSEVENT              34      /* callback function: menu1 */
#define  MENU1_SETS_SYSEVENT_SUBMENU      35
#define  MENU1_SETS_SYSEVENT_SRVEVERYTEST 36      /* callback function: menu1 */
#define  MENU1_SETS_SYSEVENT_SRVEVERYLOOP 37      /* callback function: menu1 */
#define  MENU1_SETS_DATALOG               38      /* callback function: menu1 */
#define  MENU1_SETS_DATALOG_SUBMENU       39
#define  MENU1_SETS_DATALOG_DATALOGON     40      /* callback function: menu1 */
#define  MENU1_SETS_SEPERATOR2            41
#define  MENU1_SETS_SERVICE               42      /* callback function: menu1 */
#define  MENU1_SETS_SERVICE_SUBMENU       43
#define  MENU1_SETS_SERVICE_SYSTEM        44      /* callback function: menu1 */
#define  MENU1_SETS_SERVICE_FIXTURE       45      /* callback function: menu1 */
#define  MENU1_LEFTSPEC                   46      /* callback function: menu1 */
#define  MENU1_RIGHTSPEC                  47      /* callback function: menu1 */


     /* Callback Prototypes: */

int  CVICALLBACK displseqcallb(int panel, int control, int event, void *callbackData, int eventData1, int eventData2);
int  CVICALLBACK edit_callb(int panel, int control, int event, void *callbackData, int eventData1, int eventData2);
int  CVICALLBACK edit_sequ(int panel, int control, int event, void *callbackData, int eventData1, int eventData2);
int  CVICALLBACK edit_sequglob(int panel, int control, int event, void *callbackData, int eventData1, int eventData2);
int  CVICALLBACK edit_sequhead(int panel, int control, int event, void *callbackData, int eventData1, int eventData2);
int  CVICALLBACK login_callb(int panel, int control, int event, void *callbackData, int eventData1, int eventData2);
void CVICALLBACK menu1(int menubar, int menuItem, void *callbackData, int panel);
int  CVICALLBACK panel1(int panel, int event, void *callbackData, int eventData1, int eventData2);
int  CVICALLBACK panel1_callb(int panel, int control, int event, void *callbackData, int eventData1, int eventData2);
int  CVICALLBACK sequ_selcallb(int panel, int control, int event, void *callbackData, int eventData1, int eventData2);
int  CVICALLBACK sequ_selectwindow(int panel, int control, int event, void *callbackData, int eventData1, int eventData2);
int  CVICALLBACK timer_callb(int panel, int control, int event, void *callbackData, int eventData1, int eventData2);


#ifdef __cplusplus
    }
#endif

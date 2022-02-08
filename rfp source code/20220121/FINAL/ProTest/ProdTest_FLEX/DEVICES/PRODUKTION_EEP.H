/*~+:***************************************************************** */
/*~+:* Project   : DeTeWe DECT Phones                                  */
/*~+:* Module    : prod_eep.h                                          */
/*~+:* Function  : Defines EEPROM Values used by Testdepartment        */
/*~+:* Owner     : 			                               */
/*~+:***************************************************************** */
/*~+:* $Date: 1999/04/17  $                                            */
/*~+:* $Revision: 1.0 $                                                */
//															            */
/*~+:***************************************************************** */

/*~+:Eeprom defines*/
/*~A*/
/*~+:Production parameters in the range 0x0000-0x001F*/
#define EEPROM_TESTFLOW_DATA             (0x0001)  /* Production -1 byte BCD */
#define EEPROM_UNIT_NUMBER               (0x0002)  /* Production -3 byte BCD */

#define EEPROM_SERIAL_NO                 (0x0005)  /* Production -3 byte BCD */
#define EEPROM_LAYOUT_INFO               (0x0009)  /* Production -1 byte BCD */
#ifndef EEPROM_HW_REVISION 
#define EEPROM_HW_REVISION               (0x000a)  /* Production -1 byte BCD */
#endif
#ifndef EEPROM_HF_PRO_X 
#define EEPROM_HF_PRO_X                  (0x000b)  /* Production -1 byte BCD */
#endif
#ifndef EEPROM_HF_ADJUST    
#define EEPROM_HF_ADJUST                 (0x000c)  /* Production -1 byte BCD */
#endif  
#define EEPROM_PARTLIST_NUMBER           (0x000d)  /* Production -4 byte BCD */

#define EEPROM_FUNC_DATE                 (0x0011)  /* Production -3 byte BCD */
#ifndef EEPROM_FINAL_DATE
#define EEPROM_FINAL_DATE                (0x0014)  /* Production -3 byte BCD */
#endif
#define EEPROM_SERVICE_DATE              (0x0017)  /* Production -3 byte BCD */
#define EEPROM_SERVICE_PARTLIST_NUMBER   (0x001a)  /* Production -4 byte BCD */
#ifndef EEPROM_TEST_SPARE  
#define EEPROM_TEST_SPARE                (0x001e)  /* Production -1 byte  */
#endif 
#ifndef EEPROM_FLASH_INDEX  
#define EEPROM_FLASH_INDEX               (0x001f)  /* Production -1 byte  */
#endif  

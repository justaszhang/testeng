        ??  ??                  ?       ?? ??     0 	        <?xml version="1.0" encoding="UTF-8" standalone="yes"?>
<assembly xmlns="urn:schemas-microsoft-com:asm.v1" manifestVersion="1.0" />?      ?? ??     0 	        ?4   V S _ V E R S I O N _ I N F O     ???                ?                         D   S t r i n g F i l e I n f o       0 4 0 9 0 4 b 0   P   C o m p a n y N a m e     p r o t e c h   r o s e n b e r g ,   1 9 9 9   X   F i l e D e s c r i p t i o n     I P _ R f p _ 3 5   ( R e l e a s e   x 8 6 )   ,   F i l e V e r s i o n     1 . 0 . 5   4 
  I n t e r n a l N a m e   I P _ R f p _ 3 5   V   L e g a l C o p y r i g h t   C o p y r i g h t   ?   p r o t e c h   1 9 9 9     D   O r i g i n a l F i l e n a m e   I P _ R f p _ 3 5 . d l l   6   P r o d u c t N a m e       I P _ R f p _ 3 5     ,   P r o d u c t V e r s i o n   1 . 0   D    V a r F i l e I n f o     $    T r a n s l a t i o n     	??8  X   C V I D L L P R O T O T Y P E S   C V I D L L E X P O R T S         0 	        int CMD60_addon_ClearError(int InstrSession);
int CMD60_addon_Connect(int InstrSession, int type, char *cErrorInfo);
int CMD60_addon_GetFirmwareVersion(int InstrSession, char *firmwareVersion);
int CMD60_addon_GetOption(int InstrSession, char *sysOption);
int CMD60_addon_ReadAfmCoun(int InstrSession, double *freq);
int CMD60_addon_ReadFpRfpi(int InstrSession, char *RfpiValue);
int CMD60_addon_ReadNormalTransmitPower(int InstrSession, double *transPower);
int CMD60_addon_Register(int InstrSession, char *buf);
int CMD60_addon_SendCommand(int InstrSession, unsigned char *cCmd);
int CMD60_addon_SetPacketTypePP32Z(int InstrSession);
int CMD60_addon_activate_SW_Demod(int InstrSession);
int CMD60_addon_deactivate_SW_Demod(int InstrSession);
int CheckBootFlashImage(struct struct_test *act_structptr);
int CheckDECTSwVersion(struct struct_test *act_structptr);
int CheckLED_blue(struct struct_test *act_structptr);
int CheckLED_green(struct struct_test *act_structptr);
int CheckLED_red(struct struct_test *act_structptr);
int CheckSoftwareVersion(struct struct_test *act_structptr);
int Check_MAC_Serial(struct struct_test *act_structptr);
int Check_button(struct struct_test *act_structptr);
int DateSelect(int iSelect, char *cReturn);
int DisableTestfunktionIfValid(struct struct_test *act_structptr);
int Disconnect_V2(struct struct_test *act_structptr);
int FactoryDeliveryState(struct struct_test *act_structptr);
int GetBarCode(char *strMAC, char *strSerial);
int GetCalWeek(int iDay, int iMonth, int iYear, int *iKwYear);
int IP_PortConnect(struct struct_test *act_structptr);
int InputBarCode(int panel, int control, int event, void *callbackData, int eventData1, int eventData2);
int InputSerial(int panel, int control, int event, void *callbackData, int eventData1, int eventData2);
int LogDUTSerialNumber(struct struct_test *act_structptr);
int LogMAC(struct struct_test *act_structptr);
int LogScannedSerialNumber(struct struct_test *act_structptr);
void LogShortResult(int mode);
int MiniSysClose();
int MiniSysOpen();
int MiniSysReSetOC5();
int MiniSysReSetOC6();
int MiniSysReSetOC8();
int MiniSysSetOC5();
int MiniSysSetOC6();
int MiniSysSetOC8();
int MiniSys_ADC_InpS(int gnd, int channel);
int MiniSys_ADC_RaSe(int DcAc, int range);
int MiniSys_ADC_getVac(double *result);
int MiniSys_ADC_getVdc(double *result);
void MiniSys_close();
int MiniSys_getLightSensor();
int MiniSys_get_AdapterID(char *adapterID);
int MiniSys_get_IN();
int MiniSys_get_SID(char *systemID);
int MiniSys_open(int comPort);
int MiniSys_read(char *read);
int MiniSys_set_OC(int number, int on);
int MiniSys_write(char *write_char);
int PowerOn(struct struct_test *act_structptr);
int PowerRF_J32(struct struct_test *act_structptr);
int PowerRF_J32New(struct struct_test *act_structptr);
int PowerRF_J33(struct struct_test *act_structptr);
int PowerRF_J33New(struct struct_test *act_structptr);
int RFP_CMD60_connect(struct struct_test *act_structptr);
int SSH_PortConnect(struct struct_test *act_structptr);
int SetCheckCountryTag(struct struct_test *act_structptr);
int SupplyCurrentPSU(struct struct_test *act_structptr);
int SystemCall(char *cCmd, int hide);
int WLanEmbedded_ListAP(int *ant1, int *ant2, int *ant3);
void WLanEmbedded_close();
int WLanEmbedded_open(int comPort1, int comPort2, int comPort3);
int WLanEmbedded_read(int comPort, char *read);
int WLanEmbedded_write(int comPort, char *write_char);
int WLanTxPower_Ant1(struct struct_test *act_structptr);
int WLanTxPower_Ant2(struct struct_test *act_structptr);
int WLanTxPower_Ant3(struct struct_test *act_structptr);
int WriteQPDS(int iPrufstatus);
int adapter_counter_clear(struct rte_globals *globals, int intervalnr);
int adapter_counter_read(struct rte_globals *globals, int intervalnr, int *serviceCount, int *devCount, int *serviceDays, char *lastServiceDate, char *nextServiceDate, char *serviceText);
int adapter_counter_update(struct rte_globals *globals, int count);
void add_days_to_dateStr(char *dateStr, int days);
int authenticate_kbdint(struct ssh_session_struct *session, char *user, char *password);
int backup_logdata_files(struct rte_globals *globals);
int checkFtpDeamon(char *server, int timeOut);
int check_double_connect(unsigned char *pruefstatus, int mode);
int check_other_value(char *name, double lowerlimit, double resultvalue, double upperlimit, int prec, char *unit, char *err_messg);
void check_reference();
int check_value(double resultvalue);
int check_value_TXT_cmp(char *resultvalue, char *strExpected);
int check_value_str(char *resultvalue);
int check_value_str_cmp(char *resultvalue, char *strExpected);
int clean_up(int mode);
int closeSSH();
void cmd60_settings_for_connect();
int days_between_now_and_dateStr(char *dateStr);
void disconnect(int mode);
int double_to_hex_str(double wert, int length, unsigned char *hexstr);
int dut_error(struct test_flow *test_flow);
int dut_set_diversity1();
int dut_set_diversity2();
int emc_counter(char *rfpi_ipei_dut, char *emc_list);
char file_get_group(struct struct_file *file_group, FILE *fa_r, char *group);
char file_get_group_LongLines(struct struct_file_LongLines *file_group, FILE *fa_r, char *group);
char file_getpara(struct struct_file *file_group, char *tag, void *parameter, int type);
char file_getpara_LongLines(struct struct_file_LongLines *file_group, char *tag, void *parameter, int type);
int flow_control(struct test_flow *test_flow);
void generate_xxxi(struct part_ident_parameter *ident_struct);
int getDeliveryUnitNumbersFromDataBase(char *deliveryUnitNumbers, int len);
int getLongDeTeWeSerialNumberFromDataBase(char *serialNumber);
int getMacAddrFromDataBase(char *strMacAddress);
int getParkNumberFromDataBase(char *strParkNumber);
int getParkNumberInfoFromDataBase(char *strParkNumber);
int getPartInfoFromDataBase(char *serialNumber, char *partNumber, char *macAddr);
int getPartNumberFromDataBase(char *partNumber);
int getShortDeTeWeSerialNumberFromDataBase(char *serialNumber);
int get_BandgapData(unsigned char *ucData);
int get_BarcodeData(unsigned char *ucData);
int get_ClockData(unsigned char *ucData);
int get_DeviatData(unsigned char *ucData);
void get_EepromDataPath(char *cPath);
int get_ProdData(unsigned char *ucData);
int get_RssiData(unsigned char *ucData);
int get_okay(int panel, int control, int event, void *callbackData, int eventData1, int eventData2);
int get_part_identity_db(struct part_ident_parameter *ident_struct);
int get_printermode_db();
int get_testresult(int select);
void goto_test(char *test_username);
double hex_str_to_double(unsigned char *buffer, int length);
int interactive_shell_session(struct ssh_channel_struct *channel);
int io_error(struct test_flow *test_flow);
int ipRfp_SystemPing(char *ipAddr, int timeOut);
int ipRfp_cleanupWinsock();
int ipRfp_clearTempFiles();
int ipRfp_getAdapterIpAddr(char *adapterName, char *adapterIpAddr);
int ipRfp_libssh_Close();
int ipRfp_libssh_FactoryCleanup();
int ipRfp_libssh_FactoryReset();
int ipRfp_libssh_FtpGet(char *hostIpAddr, char *srcPath, char *srcFileName);
int ipRfp_libssh_Open(char *ipaddr);
int ipRfp_libssh_Reopen();
int ipRfp_libssh_WriteAndRead(char *wBuffer, char *rBuffer, int maxread, char *wildString, double timeoutSec);
int ipRfp_libssh_changeToRootUser();
int ipRfp_libssh_checkBootDevice(char *bootDevice);
int ipRfp_libssh_deleteFilesFromDut();
int ipRfp_libssh_enableBMCTestmode();
int ipRfp_libssh_enableBMCTestmode_func();
int ipRfp_libssh_enableRSX();
int ipRfp_libssh_getBMCStatus(char *bmcStatus, char *bmcTestmode, char *bmcCalibStatus);
int ipRfp_libssh_getBMCStatus_func(char *bmcStatus, char *bmcTestmode, char *bmcCalibStatus);
int ipRfp_libssh_getIpAddress(char *respBuffer);
int ipRfp_libssh_getLinkStatus(char *ethDev, char *linkStatus);
int ipRfp_libssh_getLinkStatus_func(char *ethDev, char *linkStatus);
int ipRfp_libssh_getMacAddress(char *respBuffer);
int ipRfp_libssh_getManuVersion(char *pManuVersionFW, char *pManuVersionSW);
int ipRfp_libssh_getRTCStatus(char *rtcStatus, char *ntpStatus, char *dateString);
int ipRfp_libssh_getTemperature(double *temperature);
int ipRfp_libssh_getTemperature_func(double *temperature);
int ipRfp_libssh_getUsbDev(char *usbdevStr);
int ipRfp_libssh_getWLanStatus(char *respBuffer);
int ipRfp_libssh_readConfig(char *mac, char *hwType, char *branding, char *sernr, char *exid);
int ipRfp_libssh_readPARK(char *strParkHexNumber);
int ipRfp_libssh_readPARKxml(char *strParkHexNumber, char *strParkOctNumber, char *strDECTrDomain, char *strWLANrDomain);
int ipRfp_libssh_sendShellCmd(char *strCmd);
int ipRfp_libssh_setHeating(char *sCmd);
int ipRfp_libssh_setHeating_func(char *sCmd);
int ipRfp_libssh_setLED(char *ledCmd);
int ipRfp_libssh_setLED_func(char *ledCmd);
int ipRfp_libssh_setWLAN(char *wlanMode);
int ipRfp_libssh_writeConfig(char *mac, char *hwType, char *branding, char *sernr, char *exid);
int ipRfp_libssh_writePARK(char *strParkHexNumber, char *strDECTrDomain, char *strWLANrDomain);
int ipRfp_rs232_CheckOTParea();
int ipRfp_rs232_CheckUBIerror(char *errBuffer);
void ipRfp_rs232_Close();
int ipRfp_rs232_Open(int ComPort, int BaudRate, int TimeOut);
int ipRfp_rs232_WaitForBootUp(int timeOut, char *bootstringA, char *bootstringB, char *bootstringC, char *bootstringD, char *bootstringE);
int ipRfp_rs232_activate();
int ipRfp_rs232_checkBootImage(char *errBuffer);
int ipRfp_rs232_enableRSX();
int ipRfp_rs232_enableSSH();
int ipRfp_rs232_flashErase();
int ipRfp_rs232_getBMCStatus(char *bmcStatus, char *bmcTestmode, char *bmcCalibStatus);
int ipRfp_rs232_getFinalizeState(char *respBuffer);
int ipRfp_rs232_getIpAddress(char *ipStr);
int ipRfp_rs232_getLinkStatus(char *ethDev, char *linkStatus);
int ipRfp_rs232_getUsbDev(char *usbdevStr);
int ipRfp_rs232_mail_out(char *cmdBuffer, char *bufferAnswerA, char *bufferAnswerB);
int ipRfp_rs232_mail_out_timeout(char *cmdBuffer, char *bufferAnswerA, char *bufferAnswerB, int timeOutSec);
int ipRfp_rs232_readConfig(char *mac, char *hwType, char *branding, char *sernr, char *exid);
int ipRfp_rs232_readPARK(char *strParkHexNumber);
int ipRfp_rs232_readPARKxml(char *strParkHexNumber, char *strParkOctNumber, char *strDECTrDomain, char *strWLANrDomain);
int ipRfp_rs232_sendHalt();
int ipRfp_rs232_sendMACandSerial(char *mac, char *serial);
int ipRfp_rs232_sendShellCmd(char *strCmd);
int ipRfp_rs232_setFinalizeState();
int ipRfp_rs232_setIpAddress(char *ipStr);
int ipRfp_rs232_set_timeout(int TimeOut);
int ipRfp_rs232_sync();
int ipRfp_rs232_writeConfig(char *mac, char *hwType, char *branding, char *sernr, char *exid);
int ipRfp_rs232_writePARK(char *strParkHexNumber, char *strDECTrDomain, char *strWLANrDomain);
int ipRfp_rsx_Close();
int ipRfp_rsx_EnterTestmode();
int ipRfp_rsx_ExitTestmode();
int ipRfp_rsx_FinalizeEeprom();
int ipRfp_rsx_GetRSSI(unsigned char *value);
char *ipRfp_rsx_GetSoftwareVersion(char *buffer);
char *ipRfp_rsx_GetSoftwareVersion_Retry(char *buffer, int retry);
int ipRfp_rsx_Open(char *ipAddr, int port);
char *ipRfp_rsx_ReadEeprom(int address, char *buffer, int count);
int ipRfp_rsx_ReadEepromBandgap(unsigned char *value);
int ipRfp_rsx_ReadEepromClockFrequency(unsigned char *value);
int ipRfp_rsx_ReadEepromDeviation(unsigned char *value, unsigned char *radio);
int ipRfp_rsx_ReceiveDataFromIpRfp(int iMaxLength, unsigned char *pBuffer);
int ipRfp_rsx_SendDataToRfp(int iLength, unsigned char *pBuffer);
int ipRfp_rsx_SetB_fieldContent(unsigned char value1, unsigned char value2);
int ipRfp_rsx_SetBandgap(unsigned char value);
int ipRfp_rsx_SetClock(unsigned char value);
int ipRfp_rsx_SetDeviation(unsigned char value);
int ipRfp_rsx_SetDiversity(unsigned char value);
int ipRfp_rsx_SetFrequencyBand(unsigned char value);
int ipRfp_rsx_SetTransmitPower(unsigned char value);
int ipRfp_rsx_SetupDummyBearer(unsigned char dummychannel, unsigned char dummyslot);
int ipRfp_rsx_WriteEeprom(int address, char *buffer_ori, int count, int convert);
int ipRfp_rsx_WriteEepromBandgap(unsigned char value);
int ipRfp_rsx_WriteEepromClockFrequency(unsigned char value);
int ipRfp_rsx_WriteEepromDeviation(unsigned char value, int radio);
int ipRfp_rsx_getEepromValues(unsigned char *eeprom_value);
char *ipRfp_rsx_hexstr2bytestr(char *buffer);
int ipRfp_rsx_mail_out(int write_count, unsigned char *buffer, int read_count);
int ipRfp_rsx_showEepromValues(unsigned char *eeprom_value_adjust, unsigned char *eeprom_value_reboot);
int ipRfp_runOpenOCD(char *openocdScripts, char *respBuffer, int respSize);
int ipRfp_startWinsock();
int isFileExists(char *filename);
int isFtpDeamonRunning(char *server);
int isInDebugMode();
int isInProductionMode();
int isInReferenceMode();
int isInRepairMode();
unsigned long local_strIp_to_ulongIp(char *strIp);
void local_ulongIp_to_strIp(unsigned long ulIp, char *strIp);
void logging_io_error(int error_code);
double myRound(double value, int points);
int openSSH(char *host, char *user, char *password, char *errResp);
int or_wildcmp(const char *wild, const char *string);
int parse_line(char *line_ptr);
int printLabelOnDataBase();
int print_baco();
int print_out(char *print_str);
void print_resultdisplay(char *buffer);
int printer_strobe(int port_address);
int readSSH(char *readBuffer, int maxread, int *nread, char *wildString, double timeoutSec, char *errResp);
int read_eeprom_file(char *pfad, char *buffer);
int read_eeprom_special(char *pfad, struct eeprom_special *buffer);
int serve_logdata_handling(struct rte_globals *globals);
int setPassAndLog(char *logStr);
int set_BandgapData(unsigned char *ucData);
int set_BarcodeData(unsigned char *ucData);
int set_ClockData(unsigned char *ucData);
int set_DeviatData(unsigned char *ucData);
void set_EepromDataPath(char *cPath);
int set_ProdData(unsigned char *ucData);
int set_RssiData(unsigned char *ucData);
void set_char_serial_number(char *number);
void set_hintwindow(char *buffer, long background, int select);
void set_menu_printmode(int on_off);
void set_printermode_db(char *cLabelName);
void set_sequence(int select);
void set_serial_number(int number);
void set_statuswindow(char *buffer, long background, int select);
void set_testheader(int mode);
int startFtpDeamon(char *ipAddr, char *rootPath);
int start_up(struct rte_globals *global_sets);
int stopFtpDeamon();
void stringToLow(char *p);
void stringToUp(char *p);
int verify_knownhost(struct ssh_session_struct *session, char *errResp);
int wildcmp(const char *wild, const char *string);
int writeSSH(char *writeBuffer, int nwrite, char *errResp);
void write_reference_date();
int write_sequ2logfile(struct rte_globals *rte_globals);
         ?? ??     0	                                        
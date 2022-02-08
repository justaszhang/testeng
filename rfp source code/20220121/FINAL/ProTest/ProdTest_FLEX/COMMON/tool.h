#ifndef TOOL_H
#define TOOL_H

#define EEPROM_SPECIAL_RANGE	0x4000
#ifndef I2C_MAXBUFFER
#define I2C_MAXBUFFER 0x400
#endif


#define IO_ERROR_GLOBAL				"io_error: 0x%x"				
#define LOG_SEPERATOR_GLOBAL		"/"


#define	proTest001			1
#define	proTest002			2
#define	proTest003			3
#define	proTest004			4
#define	proTest005			5
#define	proTest006			6
#define	proTest007			7
#define	proTest008			8
#define	proTest009			9
#define	proTest010			10
#define	proTest011			11
#define	proTest012			12
#define	proTest013			13
#define	proTest014			14
#define	proTest015			15
#define	proTest016			16
#define	proTest017			17
#define	proTest018			18
#define	proTest019			19
#define	proTest020			20
#define	proTest021			21
#define	proTest022			22
#define	proTest023			23
#define	proTest024			24
#define	proTest025			25
#define	proTest026			26
#define	proTest027			27
#define	proTest028			28
#define	proTest029			29
#define	proTest030			30
///NEW SYSTEM
#define	proTest103			103
#define	proTest106			106
#define	proTest105			105
#define	proTest200			200  

//Start [@JM030308]
#define MAX_LABEL_DAT_INDEX			16
#define MAXLABELPARAMSTRINGLENGTH	64
//rechnerische Größe zur Verhinderung Speicherüberlauf
#define MAXLABELPARAM				20
//max. zulässige Anzahl Label pro Produkt
#define MAX_LABEL	20
//Ende [@JM030308]

#define BACKUP_EEPROM_DATA_PATH "c:\\test\\backup" //[@JM220408]

char *hexstr2bytestr(char *buffer);
char *bytestr2hexstr(char buffer[], int count);
#ifdef CMD60_SETTINGS_DECT
extern int 	iDummyslot;
extern int 	iDummycarrier;
extern int 	iTrafficslot;
extern int 	iTrafficcarrier;
extern unsigned char 	RFPI_hex[5];
int iHerstellerkennung;  
char 	SerialNrAvaja[20]; 
#endif
char	serial_path_avaya[MAXPATHNAMELEN];   
struct structQPDS
		{
		char	strPathQpds[MAXPATHNAMELEN];
		char 	strExID[80];
		char 	strBgName[80];
		char    strAdapterNr[80];
		char	strCustSn[80];
		char    strMacAddr[40];
		char    strParkNr[40];
		char    strPruefstufe[80];
		char    strWerkfachgruppe[80];
		char	strHostName[80];
		char    strPruestatus[20];
		char	strStartZeit[20];
		char	strEndZeit[20];
		char	strFehlerInfo1[500];
		char	strFehlerInfo2[500];  
		};
		
struct eeprom_special
		{
		int	addresse[EEPROM_SPECIAL_RANGE];
		unsigned char	value[EEPROM_SPECIAL_RANGE];
		};

struct part_ident_parameter
		{
		char serial_local_path[MAXPATHNAMELEN]; 
 		char type;	//unknown = 0, PP = 1, FP = 2;
 		int EMC;
 		unsigned char RFPI[5];
 		unsigned char IPUI[5];
 		int serial_number;
		};

struct mac_adress_parameter
		{
		char mac_adress_local_path[MAXPATHNAMELEN]; 
 		unsigned long mac_adress;
 		unsigned char strMac_adress[13];
		};

//Start [@JM030308]
struct struct_LabelDat
{
	char cLabelDat[MAX_LABEL_DAT_INDEX][160];
};

struct	struct_LabelDat strLabelDat;
//Ende [@JM030308]


//Start [@GC10122012]
int printLabelOnDataBase(void);			// kompletter Labelprint per Datenbank
//Ende [@GC10122012]

//Start [@GC22092012]
int getParkNumberFromDataBase(char *strParkNumber);        // mit generiern
int getParkNumberInfoFromDataBase(char *strParkNumber);    // nur lesen, nicht generieren
//Ende [@GC22092012]

//Start [@GC16052011]
int getMacAddrFromDataBase(char *strMacAdress);
int getShortDeTeWeSerialNumberFromDataBase(char *serialNumber);
int getLongDeTeWeSerialNumberFromDataBase(char *serialNumber);
int getPartNumberFromDataBase(char *partNumber);
int getDeliveryUnitNumbersFromDataBase(char *deliveryUnitNumbers,int len);
int getPartInfoFromDataBase(char *serialNumber, char *partNumber, char *macAddr);
int SystemCall(char *cCmd,int hide);

//Ende [@GC16052011]


//void logging_io_error(int error_code);   // moved from tool.h to rte_tool.h   @GC 30.08.2011
void 	check_reference(void);
void 	cmd60_settings_for_connect(void); 
void 	write_reference_date(void);   
int 	read_eeprom_special(char pfad[], struct eeprom_special *buffer);
int 	read_eeprom_file(char *pfad, char *buffer);
int 	parse_line(char *line_ptr);
int 	write_sequ2logfile(struct rte_globals *rte_globals);
int 	get_part_identity(struct part_ident_parameter *ident_struct);
//Start [@JM120907]
int		get_part_identity_db(struct part_ident_parameter *ident_struct);
//End   [@JM120907]
int 	check_value_str(char *resultvalue) ;
#ifdef PRINT_LABEL  //define this in "test.h" !!
int 	print_label_global(int flag);
#endif //PRINT_LABEL
int 	emc_counter(char *rfpi_ipei_dut, char *emc_list);
int 	check_double_connect(unsigned char *pruefstatus,int mode);
int 	double_to_hex_str(double wert, int length, unsigned char *hexstr);
double 	hex_str_to_double(unsigned char *buffer, int length);
//int get_mac_adress(struct mac_adress_parameter *mac_adress_struct);
int generateDeTeWeSerialNumber(char* strDeTeWeSerialNumber);
int WriteQPDS(int iPrufstatus);
int GetMacFlex(char *strMacAdress);
int get_avaja_ser(char *SerialNrAvaja);    

//Start [@JM030308]
void set_printermode_db(char *cLabelName);
int get_printermode_db(void);

int read_label_dat(void);
int DateSelect(int iSelect, char *cReturn);
int GetCalWeek(int iDay, int iMonth, int iYear, int *iKwYear);
//Ende [@JM030308]


//Start [@JM010408]
int print_label_db(void);

int DisableTestfunktionIfValid(struct struct_test *act_structptr);
int GetRepairInfoFromDb(void);
//Ende [@JM010408]

//Start [@JM220408]
int BackupEepromDataToFile(void);

int get_ProdData(unsigned char *ucData);
int set_ProdData(unsigned char *ucData);
int get_DeviatData(unsigned char *ucData);
int set_DeviatData(unsigned char *ucData);
int get_RssiData(unsigned char *ucData);
int set_RssiData(unsigned char *ucData);
int get_BarcodeData(unsigned char *ucData);
int set_BarcodeData(unsigned char *ucData);
int get_ClockData(unsigned char *ucData);
int set_ClockData(unsigned char *ucData);
int get_BandgapData(unsigned char *ucData);
int set_BandgapData(unsigned char *ucData);
void get_EepromDataPath(char *cPath);
void set_EepromDataPath(char *cPath);
//Ende [@JM220408]

#endif //TOOL_H

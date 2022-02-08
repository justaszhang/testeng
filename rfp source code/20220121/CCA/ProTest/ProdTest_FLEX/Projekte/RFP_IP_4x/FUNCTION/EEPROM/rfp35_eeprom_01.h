#ifndef __EEPROM_B_H
#define __EEPROM_B_H

//
// Dies ist kein Original,
// hier sind nur die Werte vorhanden die Flex benötigt.
//
// die RSSI Werte sind aus der Test Specification abgeschrieben 
// die Anderen Werte habe ich vom alten RFP übernommen
// b.z.w Durch Test/Debug ermittelt
// GC 09.06.2011
//

#define EEPROM_MODULATION_DEV_LATAM0     (0x0030)
#define EEPROM_MODULATION_DEV_LATAM1     (0x0031)
#define EEPROM_MODULATION_DEV_USA0       (0x0032)
#define EEPROM_MODULATION_DEV_USA1       (0x0033)


#define EEPROM_BANDGAP                   (0x0025)       //adjust 1v8

#define EEPROM_MODULATION_DEVIATION0     (0x0045)
#define EEPROM_MODULATION_DEVIATION1     (0x0046)

#define EEPROM_RSSI_COMP_OFFSET0         (0x0047)
#define EEPROM_RSSI_COMP_OFFSET1         (0x0048)
#define EEPROM_RSSI_COMP_K_FACTOR0       (0x0049)
#define EEPROM_RSSI_COMP_K_FACTOR1       (0x004A)

#define EEPROM_DEFAULT_FREQUENCY         (0x004F)       //adjust Freq



#endif

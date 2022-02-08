#include "rte.h"

#define MODULE_PC_ADDR			0x00
#define I2C1_FCT 				0x08
#define PARAMETER_START 		0x100

int module_send (unsigned char length, unsigned char address, unsigned char *buffer)
{
int i,j,tx_count;
char tx_buffer[0x50],rx_buffer[0x50];

	tx_buffer[0] = length+2;
	tx_buffer[1] = address;
	tx_buffer[2] = MODULE_PC_ADDR;
// copy mail:
	memcpy(&tx_buffer[3],buffer,length);
//setup count of outsending bytes	
	tx_count = length+4;
	length += 3;
// chksum:
	tx_buffer[length] = address;
	for(i=2; i<length;i++)
		tx_buffer[length] ^= tx_buffer[i];
//send out:
	FlushInQ (global.module_portB);			  //clear status
	i = ComWrt (global.module_portB,tx_buffer,tx_count);
// check mirrored data:	
	j = ComRd (global.module_portB,rx_buffer,i);
	if(i!=j) 
		return(2);
	for(i=0;i<tx_count;i++)
		{
		if(tx_buffer[i] != rx_buffer[i]) 
			return(3); 
		}
		
	return(0);
}


int module_chk_ackn(void)
{
int j;
char rx_buffer[8];

	j = ComRd (global.module_portB,rx_buffer,6);
	if(j != 6) return(1);
	if(rx_buffer[4] != 0x41) return (2); //"A"
	return(0);
}

int module_read(int start_offset,unsigned char *buffer,int length)
{
int i,j;
unsigned char rx_buffer[0x50];
	
	j = start_offset+length+1; //+1 for chksum
	i = ComRd (global.module_portB,rx_buffer,j);
	if(i!=j) 
		return(1);
	memcpy(buffer,&rx_buffer[start_offset],length);
	return(0);
}



int module_read_para(unsigned char module_addr, unsigned char i2c_addr,int parameter_number,unsigned char *buffer, int length)
{
/*-------------------------------------------------------------
read from i2c-bus1 memory:
length is only for direct read !! (normal to zero)
if(length) --> i2c_sub_adsdress = parameter_number;!!
all parameters as strings-->zero terminated!
step1: 
--> get the parameter address & string-length
step2: 
--> read string from address
*/

// normal maximum data-length per mail: 0x40 

int				current_i2c_address,current_buffer_addr,i,j,k, buffer_sub_addr;
unsigned char 	mail_buffer[0x50],read_count,loc_read_count,act_buffer_addr = 0;


	current_i2c_address = (parameter_number<<2);
	if(length)		//exception for direct read, value for parameter-number represents i2c-sub-address !!
		current_i2c_address = parameter_number;
	else
		current_i2c_address += PARAMETER_START;
	i2c_addr &= 0xfe;  // write memory--> lsb is low
//do a initialize write for setup startaddress for read:
	mail_buffer[0] = I2C1_FCT;
	mail_buffer[1] = 0x02;		//write count
	mail_buffer[2] = i2c_addr;		//
	mail_buffer[3] = current_i2c_address >> 8; //high-byte from i2c address
	mail_buffer[4] = current_i2c_address & 0xff;//low-byte from i2c-address
	if(module_send (5,module_addr, mail_buffer))
		return(1);
	if(module_chk_ackn())
		return(-1);

//---------------------------------------------------------------------
	i2c_addr |= 0x01;			// read memory--> lsb is set
	mail_buffer[0] = I2C1_FCT;
	if(length)
		{
		buffer_sub_addr = 0;
		while(length)
			{
			if(length < 0x41)
				{
				mail_buffer[1] = length;
				length = 0;
				}
			else
				{
				mail_buffer[1] = 0x40;
				length -= 0x40;
				}
			mail_buffer[2] = i2c_addr;
			if(module_send (3,module_addr, mail_buffer))
				return(1);
			if(module_read(4, &buffer[buffer_sub_addr], mail_buffer[1])) return(2);
			if(mail_buffer[1] == 0x40) buffer_sub_addr += 0x40;
			}
		return(0);
		}

	mail_buffer[1] = 0x04;		//read count
	mail_buffer[2] = i2c_addr;
	if(module_send (3,module_addr, mail_buffer))
		return(1);
	if(module_read(4, mail_buffer, 4)) return(2);
// --------------------------------------------------------------------
	current_i2c_address = (mail_buffer[0]<<8) + mail_buffer[1];	//save address
	read_count = mail_buffer[2];									//save read-count
	i2c_addr &= 0xfe;  // write memory--> lsb is low
//do a initialize write for setup startaddress for parameter read:
	mail_buffer[0] = I2C1_FCT;
	mail_buffer[1] = 0x02;		//write count
	mail_buffer[2] = i2c_addr;		//
	mail_buffer[3] = current_i2c_address >> 8;   //high-byte from i2c address
	mail_buffer[4] = current_i2c_address & 0xff; //low-byte from i2c-address
	if(module_send (5,module_addr, mail_buffer))
		return(1);
	if(module_chk_ackn())
			{
			sprintf(mail_buffer,"Can't find module 0x%x !!",module_addr);
			MessagePopup ("Run - Adaptercode Error", mail_buffer);
			return(-1);
			}
//---------------------------------------------------------------------
	i2c_addr |= 0x01;			// read memory--> lsb is set
	mail_buffer[0] = I2C1_FCT;
	do
		{
		if(read_count>0x40)
			loc_read_count = 0x40;
		else
			loc_read_count = read_count;
		mail_buffer[1] = loc_read_count;		//read count
		mail_buffer[2] = i2c_addr;
		if(module_send (3,module_addr, mail_buffer))
			return(1);
		if(module_read(4, &buffer[act_buffer_addr], loc_read_count)) return(2);
		read_count -= loc_read_count;
		act_buffer_addr += loc_read_count;
		}while(read_count);
	buffer[act_buffer_addr] = 0;
	return(0);
}

//---------------------------------------------------------------------------------------------------------------
// GC 10.05.2011 fuer Adapter Tag zu schreiben
// die Funktion ist aus protest\i2c.c kopiert....  
// Achtung es darf nur maximal 0x20 length benutzt werden,
// das mit dem ueberlauf funktioniert nicht.
// ich hab keine Zeit und Lust diese Funktion zu reparieren..
// wird aber eh nicht benutzt!!!
// also: maximal 0x20 Byte schreiben und immer gerade anfangen
int module_write_i2c_mem(unsigned char protest_addr,char i2c_addr, int i2c_sub_address,unsigned char *buffer,int length)
{
// normal maximum data-length per mail: 0x40 

int				current_i2c_address,temp,sub_length,current_buffer_addr=0;
unsigned char 	mail_buffer[0x40+0x10];
	
	current_i2c_address = i2c_sub_address;
	
	//sprintf(str,"Debug len=%d", length);
    //MessagePopup ("Status", str);

	do
		{
		mail_buffer[0] = I2C1_FCT;
			
		if(length > 0x40)
			sub_length = 0x40;
		else
			sub_length = length;
//check for page-overflow(i2c-page for mem-device):
		temp = (current_i2c_address & 0x1f) + sub_length;
		if(temp > 0x40)
			sub_length = 0x40 - (temp - 0x40);
//--------			
		mail_buffer[1] = 0x02 + sub_length;		//write count
		mail_buffer[2] = i2c_addr;		//
		mail_buffer[3] = current_i2c_address >> 8; //high-byte from i2c address
		mail_buffer[4] = current_i2c_address & 0xff;//low-byte from i2c-address
		memcpy(&mail_buffer[5],&buffer[current_buffer_addr],sub_length);
		//if(protest_send_mail (5+sub_length,protest_addr, mail_buffer))
		if(module_send (5+sub_length,protest_addr, mail_buffer))
			return(1);
		Delay(0.04);
		current_i2c_address += sub_length;
		current_buffer_addr += sub_length;
		length -= sub_length;
		}while(length>0);
	return(0);
}


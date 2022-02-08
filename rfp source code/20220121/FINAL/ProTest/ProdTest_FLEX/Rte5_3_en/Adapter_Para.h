int module_write_i2c_mem(unsigned char protest_addr,char i2c_addr, int i2c_sub_address,unsigned char *buffer,int length);
int module_read_para(unsigned char module_addr, unsigned char i2c_addr,int parameter_number,unsigned char *buffer, int length);

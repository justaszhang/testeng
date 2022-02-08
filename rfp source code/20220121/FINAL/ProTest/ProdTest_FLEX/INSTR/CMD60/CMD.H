// setup AF Voltmeter 
#define LT10khz 0
#define LT30Mhz 1
#define LT60Mhz 2
#define RMS  0
#define SQRT 1

// Connectors 
#define RF_INOUT	0 
#define RF_IN1_OUT2 1 
#define RF_IN2_OUT1 2 
#define RF_IN2_OUT2 3 

// Channels
#define CH0 1897.344
#define CH1 1895.616
#define CH2 1893.888
#define CH3 1892.160
#define CH4 1890.432
#define CH5 1888.704
#define CH6 1886.976
#define CH7 1885.248
#define CH8 1883.520
#define CH9 1881.792

// signaling
#define NORMAL 0
#define LOOPBACK 1
#define ECHO 2

// CMD-States:
#define IDLE 1
#define DFP 2
#define DPP 3
#define DCE 4
#define DBAN 5
#define DRFG 6

// Modes from CMD 
#define NOMODE 0
#define MANUAL 1
#define BAN 2
#define RFGEN 3

// Datatyp B field 
#define PRBS 0		  // noise
#define FIG0 1		  // 01010101 
#define FIG1 2		  // 00110011 
#define FIG2 3		  // 00001111 
#define FIG31 4

#define LOCKED 0
#define UNLOCKED 1

#define FP 0
#define PP 1


#define AFGEN 0		   // Source of SPEECH CODER 
#define MULTI 1		   
#define AFIN 0		   // Source of AF Voltmeter 
#define ADPCM 0		   

#define SOURCE_ON 0
#define SOURCE_OFF 1

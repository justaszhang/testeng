s??         ?<   ;  ?  (   ?   ????                               rtx2012                         rtx2012base                                 __stdcall                                                 ?  ? ??ViInt32     ? ??ViStatus  ? ? ??ViReal64     	? 	??ViSession     ?  ViChar[]       output RTX F/W H/W SN         1-success 0-fail    0 W ?     ?    output                            O ? ???    ?    return                             	            	            ?    This control specifies the selected DUT to test.

Value range :
-----------
    0 - Dect Fixed Part as DUT.
    1 - Dect Portable Part as DUT.

Default value :
-------------
    1 - Dect Portable Part    ?    This control contains the status code returned by  the  function call.

Status :
------

Codes     Description
----------------------------------------------------------------
       0  No error.

3FFF0005  The specified termination character was read.
3FFF0006  The specified number of bytes was read.

BBFC0800  Error Register STB.

BFFC0001  Parameter 1 out of range.
BFFC0002  Parameter 2 out of range.
BFFC0003  Parameter 3 out of range.
BFFC0004  Parameter 4 out of range.
BFFC0005  Parameter 5 out of range.
BFFC0006  Parameter 6 out of range.
BFFC0007  Parameter 7 out of range.
BFFC0008  Parameter 8 out of range.

BFFF0000  Miscellaneous or system error occurred.
BFFF000E  Invalid session handle.
BFFF0015  Timeout occurred before operation could complete.
BFFF0034  Violation of raw write protocol occurred.
BFFF0035  Violation of raw read protocol occurred.
BFFF0036  Device report an output protocol error.
BFFF0037  Device reported an input protocol error.
BFFF0038  Bus error occurred during transfer.
BFFF003A  Invalid setup (attributes are not consistent).
BFFF005F  No listeners condition was detected.
BFFF0060  This interface is not the controller in charge.
BFFF0067  Operation is not supported on this session.    ? f}           DUT Type                          ? ? ???    ?    Status                                       'Dect Fixed Part 0 Dect Portable Part 1    	            ?    This control specifies the type of test to activate.

Value range :
-----------
    0 - No Test Mode   
    1 - Manual Test
    2 - Burst Analysis
    3 - RF Generator

Default value :
-------------
    0 - No Test Mode    ?    This control contains the status code returned by  the  function call.

Status :
------

Codes     Description
----------------------------------------------------------------
       0  No error.

3FFF0005  The specified termination character was read.
3FFF0006  The specified number of bytes was read.

BBFC0800  Error Register STB.

BFFC0001  Parameter 1 out of range.
BFFC0002  Parameter 2 out of range.
BFFC0003  Parameter 3 out of range.
BFFC0004  Parameter 4 out of range.
BFFC0005  Parameter 5 out of range.
BFFC0006  Parameter 6 out of range.
BFFC0007  Parameter 7 out of range.
BFFC0008  Parameter 8 out of range.

BFFF0000  Miscellaneous or system error occurred.
BFFF000E  Invalid session handle.
BFFF0015  Timeout occurred before operation could complete.
BFFF0034  Violation of raw write protocol occurred.
BFFF0035  Violation of raw read protocol occurred.
BFFF0036  Device report an output protocol error.
BFFF0037  Device reported an input protocol error.
BFFF0038  Bus error occurred during transfer.
BFFF003A  Invalid setup (attributes are not consistent).
BFFF005F  No listeners condition was detected.
BFFF0060  This interface is not the controller in charge.
BFFF0067  Operation is not supported on this session.    D 4           Mode                              	) ?f??    ?    Status                                        =No Test Mode 0 Manual Test 1 Burst Analysis 2 RF Generator 3    	           ?    This control specifies the mode of connection.

Value range :
-----------
    0 - Normal   : The Device Under Test converts  the  received
                   data in its ADPCM.  Recorded  data  are  also 
                   converted   and   sent   to  the  CMD60.  The
                   respective  audio  signals  are  provided  at
                   the 50-contact socket of the  CMD 60. In this
                   mode, audio measurements (converter)  can  be
                   performed.
 
    1 - Loopback : The Device Under Test is set to the  loopback 
                   mode in order to return the transmitted  data
                   without modification. This is  the  prerequi-
                   site   for  modulation  and  bit  error  rate
                   measurement.

    2 - Echo     : The signals applied  to  the  Fixed Part  are 
                   sent to the CMD60, delayed by one second  and
                   returned to the DUT. This mode  permits  fast  
                   qualitative testing of the DUT.

Note :
----
(1) Not all Fixed Parts permit audio  connections  in  the  test
    mode. Therefore, it may happen that Normal and Echo modes do
    not have the desired result.    ?    This control contains the status code returned by  the  function call.

Status :
------

Codes     Description
----------------------------------------------------------------
       0  No error.

3FFF0005  The specified termination character was read.
3FFF0006  The specified number of bytes was read.

BBFC0800  Error Register STB.

BFFC0001  Parameter 1 out of range.
BFFC0002  Parameter 2 out of range.
BFFC0003  Parameter 3 out of range.
BFFC0004  Parameter 4 out of range.
BFFC0005  Parameter 5 out of range.
BFFC0006  Parameter 6 out of range.
BFFC0007  Parameter 7 out of range.
BFFC0008  Parameter 8 out of range.

BFFF0000  Miscellaneous or system error occurred.
BFFF000E  Invalid session handle.
BFFF0015  Timeout occurred before operation could complete.
BFFF0034  Violation of raw write protocol occurred.
BFFF0035  Violation of raw read protocol occurred.
BFFF0036  Device report an output protocol error.
BFFF0037  Device reported an input protocol error.
BFFF0038  Bus error occurred during transfer.
BFFF003A  Invalid setup (attributes are not consistent).
BFFF005F  No listeners condition was detected.
BFFF0060  This interface is not the controller in charge.
BFFF0067  Operation is not supported on this session.    ? -           Mode                              ? ? ???    ?    Status                                       Normal 0 Loopback 1 Echo 2    	            ?    This control specifies the antenna number  to  be  used  by  the
Fixed Part under test.

Value range :
-----------
    0 to 7

Default value :
-------------
    0
    ?    This control contains the status code returned by  the  function call.

Status :
------

Codes     Description
----------------------------------------------------------------
       0  No error.

3FFF0005  The specified termination character was read.
3FFF0006  The specified number of bytes was read.

BBFC0800  Error Register STB.

BFFC0001  Parameter 1 out of range.
BFFC0002  Parameter 2 out of range.
BFFC0003  Parameter 3 out of range.
BFFC0004  Parameter 4 out of range.
BFFC0005  Parameter 5 out of range.
BFFC0006  Parameter 6 out of range.
BFFC0007  Parameter 7 out of range.
BFFC0008  Parameter 8 out of range.

BFFF0000  Miscellaneous or system error occurred.
BFFF000E  Invalid session handle.
BFFF0015  Timeout occurred before operation could complete.
BFFF0034  Violation of raw write protocol occurred.
BFFF0035  Violation of raw read protocol occurred.
BFFF0036  Device report an output protocol error.
BFFF0037  Device reported an input protocol error.
BFFF0038  Bus error occurred during transfer.
BFFF003A  Invalid setup (attributes are not consistent).
BFFF005F  No listeners condition was detected.
BFFF0060  This interface is not the controller in charge.
BFFF0067  Operation is not supported on this session.    
 ( ?     K    Antenna Number                    ? ???    ?    Status                                                   	               This control specifies the transmit power level in dBm.

Value range :
-----------
    The  range  of  the  RF  level  depends  on  the  external 
    attenuation and the selected  RF  connector  as  follows :
    
    When RF OUT selected :
        -100.0 - ext.att. ... -40.0 - ext.att. dBm 

    When RF OUT2 and RF IN selected :
        -40.0 - ext.att.  ... +7.5 - ext.att. dBm

    When RF OUT2 and RF IN2 selected 
        -20.0 - ext.att.  ... +7.5 - ext.att. dBm

Default value :
-------------
    -73.0 dBm
    ?    This control contains the status code returned by  the  function call.

Status :
------

Codes     Description
----------------------------------------------------------------
       0  No error.

3FFF0005  The specified termination character was read.
3FFF0006  The specified number of bytes was read.

BBFC0800  Error Register STB.

BFFC0001  Parameter 1 out of range.
BFFC0002  Parameter 2 out of range.
BFFC0003  Parameter 3 out of range.
BFFC0004  Parameter 4 out of range.
BFFC0005  Parameter 5 out of range.
BFFC0006  Parameter 6 out of range.
BFFC0007  Parameter 7 out of range.
BFFC0008  Parameter 8 out of range.

BFFF0000  Miscellaneous or system error occurred.
BFFF000E  Invalid session handle.
BFFF0015  Timeout occurred before operation could complete.
BFFF0034  Violation of raw write protocol occurred.
BFFF0035  Violation of raw read protocol occurred.
BFFF0036  Device report an output protocol error.
BFFF0037  Device reported an input protocol error.
BFFF0038  Bus error occurred during transfer.
BFFF003A  Invalid setup (attributes are not consistent).
BFFF005F  No listeners condition was detected.
BFFF0060  This interface is not the controller in charge.
BFFF0067  Operation is not supported on this session.      =     d    RF Level (dBm)                    !. ? ???    ?    Status                             -73.0    	           ?    This control specifies the slot number of the Traffic Bearer  on
call setup.

Value range :
-----------
    0 to 11

Default value :
-------------
    0

Note :
----
(1) Note that the slot values of Dummy Bearer and Traffic Bearer
    are not identical. Besides, adjacent slot  values  are  only
    permissibles if the channel  numbers  of  Dummy  Bearer  and
    Traffic Bearer are identical.    V    This control specifies the channel number of the Traffic  Bearer 
on call setup.

Value range :
-----------
    0 to 9

Default value :
-------------
    0

Note :
----
(1) If the slot values of Dummy Bearer and  Traffic  Bearer  are
    adjacent, the channel numbers of Dummy  Bearer  and  Traffic
    Bearer must correspond to each other.    ?    This  control  specifies  the  carrier  offset  to  change   the 
assignment of channel numbers to frequencies in  half  the  DECT 
channel spacing.

Value range :
-----------
    -3.0 to +3.0 in steps of 0.5

Default value :
-------------
    0.0

Note :
----
(1) To permit to make use of the extended channels of the CMD60,
    the devices under test must be  able  to  perform  the  same 
    channel/frequency assignment (exceeding DECT).    ?    This control contains the status code returned by  the  function call.

Status :
------

Codes     Description
----------------------------------------------------------------
       0  No error.

3FFF0005  The specified termination character was read.
3FFF0006  The specified number of bytes was read.

BBFC0800  Error Register STB.

BFFC0001  Parameter 1 out of range.
BFFC0002  Parameter 2 out of range.
BFFC0003  Parameter 3 out of range.
BFFC0004  Parameter 4 out of range.
BFFC0005  Parameter 5 out of range.
BFFC0006  Parameter 6 out of range.
BFFC0007  Parameter 7 out of range.
BFFC0008  Parameter 8 out of range.

BFFF0000  Miscellaneous or system error occurred.
BFFF000E  Invalid session handle.
BFFF0015  Timeout occurred before operation could complete.
BFFF0034  Violation of raw write protocol occurred.
BFFF0035  Violation of raw read protocol occurred.
BFFF0036  Device report an output protocol error.
BFFF0037  Device reported an input protocol error.
BFFF0038  Bus error occurred during transfer.
BFFF003A  Invalid setup (attributes are not consistent).
BFFF005F  No listeners condition was detected.
BFFF0060  This interface is not the controller in charge.
BFFF0067  Operation is not supported on this session.    &? Z n     K    Traffic Slot Number               ( Z?    K    Traffic Carrier Number            )z ??   K    Carrier Offset                    +> ?h??    ?    Status                                                      	                ??      @      ?                           	            ?    This control specifies the "Radio Fixed Part Identity" (RFPI).

Value range :
-----------
    String (10 digits in hexadecimal format).

Default value :
-------------
    "0000000000"
    ?    This control contains the status code returned by  the  function call.

Status :
------

Codes     Description
----------------------------------------------------------------
       0  No error.

3FFF0005  The specified termination character was read.
3FFF0006  The specified number of bytes was read.

BBFC0800  Error Register STB.
BBFC0900  Error string length.

BFFC0001  Parameter 1 out of range.
BFFC0002  Parameter 2 out of range.
BFFC0003  Parameter 3 out of range.
BFFC0004  Parameter 4 out of range.
BFFC0005  Parameter 5 out of range.
BFFC0006  Parameter 6 out of range.
BFFC0007  Parameter 7 out of range.
BFFC0008  Parameter 8 out of range.

BFFF0000  Miscellaneous or system error occurred.
BFFF000E  Invalid session handle.
BFFF0015  Timeout occurred before operation could complete.
BFFF0034  Violation of raw write protocol occurred.
BFFF0035  Violation of raw read protocol occurred.
BFFF0036  Device report an output protocol error.
BFFF0037  Device reported an input protocol error.
BFFF0038  Bus error occurred during transfer.
BFFF003A  Invalid setup (attributes are not consistent).
BFFF005F  No listeners condition was detected.
BFFF0060  This interface is not the controller in charge.
BFFF0067  Operation is not supported on this session.    1R > ?     d    RFPI                              2 ? ???    ?    Status                             "0000000000"    	           ?    This control contains the status code returned by  the  function call.

Status :
------

Codes     Description
----------------------------------------------------------------
       0  No error.

3FFF0005  The specified termination character was read.
3FFF0006  The specified number of bytes was read.

BBFC0800  Error Register STB.
BBFC0900  Error string length.

BFFC0001  Parameter 1 out of range.
BFFC0002  Parameter 2 out of range.
BFFC0003  Parameter 3 out of range.
BFFC0004  Parameter 4 out of range.
BFFC0005  Parameter 5 out of range.
BFFC0006  Parameter 6 out of range.
BFFC0007  Parameter 7 out of range.
BFFC0008  Parameter 8 out of range.

BFFF0000  Miscellaneous or system error occurred.
BFFF000E  Invalid session handle.
BFFF0015  Timeout occurred before operation could complete.
BFFF0034  Violation of raw write protocol occurred.
BFFF0035  Violation of raw read protocol occurred.
BFFF0036  Device report an output protocol error.
BFFF0037  Device reported an input protocol error.
BFFF0038  Bus error occurred during transfer.
BFFF003A  Invalid setup (attributes are not consistent).
BFFF005F  No listeners condition was detected.
BFFF0060  This interface is not the controller in charge.
BFFF0067  Operation is not supported on this session.     ?    This control specifies the MT message (MAC message).

Value range :
-----------
    String (10 digits in hexadecimal format).

Default value :
-------------
    "7000000000"
    7? ? ???    ?    Status                            <?  ?     d    MT Escape Sequence                 	            "7000000000"   ?    This control contains the status code returned by  the  function call.

Status :
------

Codes     Description
----------------------------------------------------------------
       0  No error.

3FFF0005  The specified termination character was read.
3FFF0006  The specified number of bytes was read.

BBFC0800  Error Register STB.

BFFC0001  Parameter 1 out of range.
BFFC0002  Parameter 2 out of range.
BFFC0003  Parameter 3 out of range.
BFFC0004  Parameter 4 out of range.
BFFC0005  Parameter 5 out of range.
BFFC0006  Parameter 6 out of range.
BFFC0007  Parameter 7 out of range.
BFFC0008  Parameter 8 out of range.

BFFF0000  Miscellaneous or system error occurred.
BFFF000E  Invalid session handle.
BFFF0015  Timeout occurred before operation could complete.
BFFF0034  Violation of raw write protocol occurred.
BFFF0035  Violation of raw read protocol occurred.
BFFF0036  Device report an output protocol error.
BFFF0037  Device reported an input protocol error.
BFFF0038  Bus error occurred during transfer.
BFFF003A  Invalid setup (attributes are not consistent).
BFFF005F  No listeners condition was detected.
BFFF0060  This interface is not the controller in charge.
BFFF0067  Operation is not supported on this session.    =? j ???    ?    Status                             	           ?    This control contains the status code returned by  the  function call.

Status :
------

Codes     Description
----------------------------------------------------------------
       0  No error.

3FFF0005  The specified termination character was read.
3FFF0006  The specified number of bytes was read.

BBFC0800  Error Register STB.

BFFC0001  Parameter 1 out of range.
BFFC0002  Parameter 2 out of range.
BFFC0003  Parameter 3 out of range.
BFFC0004  Parameter 4 out of range.
BFFC0005  Parameter 5 out of range.
BFFC0006  Parameter 6 out of range.
BFFC0007  Parameter 7 out of range.
BFFC0008  Parameter 8 out of range.

BFFF0000  Miscellaneous or system error occurred.
BFFF000E  Invalid session handle.
BFFF0015  Timeout occurred before operation could complete.
BFFF0034  Violation of raw write protocol occurred.
BFFF0035  Violation of raw read protocol occurred.
BFFF0036  Device report an output protocol error.
BFFF0037  Device reported an input protocol error.
BFFF0038  Bus error occurred during transfer.
BFFF003A  Invalid setup (attributes are not consistent).
BFFF005F  No listeners condition was detected.
BFFF0060  This interface is not the controller in charge.
BFFF0067  Operation is not supported on this session.     ?    This control specifies  the  tolerance  value  for  the  minimum  absolute power of the DUT, averaged over a burst.

Value range :
-----------
    0.0 to 30.0 dBm

Default value :
-------------
    20.0 dBm     ?    This control specifies  the  tolerance  value  for  the  maximum  absolute power of the DUT, averaged over a burst.

Value range :
-----------
    0.0 to 30.0 dBm

Default value :
-------------
    25.0 dBm    B? ????    ?    Status                            G? - Z    K    Lower NTP (dBm)                   H? -?   K    Upper NTP (dBm)                    	         ????????@>              @4                ????????@>              @9                   ?    This control specifies the number of bursts over which  the  NTP
value is averaged.

Value range :
-----------
    1 to 200

Default value :
-------------
    4    ?    This control contains the status code returned by  the  function call.

Status :
------

Codes     Description
----------------------------------------------------------------
       0  No error.

3FFF0005  The specified termination character was read.
3FFF0006  The specified number of bytes was read.

BBFC0800  Error Register STB.

BFFC0001  Parameter 1 out of range.
BFFC0002  Parameter 2 out of range.
BFFC0003  Parameter 3 out of range.
BFFC0004  Parameter 4 out of range.
BFFC0005  Parameter 5 out of range.
BFFC0006  Parameter 6 out of range.
BFFC0007  Parameter 7 out of range.
BFFC0008  Parameter 8 out of range.

BFFF0000  Miscellaneous or system error occurred.
BFFF000E  Invalid session handle.
BFFF0015  Timeout occurred before operation could complete.
BFFF0034  Violation of raw write protocol occurred.
BFFF0035  Violation of raw read protocol occurred.
BFFF0036  Device report an output protocol error.
BFFF0037  Device reported an input protocol error.
BFFF0038  Bus error occurred during transfer.
BFFF003A  Invalid setup (attributes are not consistent).
BFFF005F  No listeners condition was detected.
BFFF0060  This interface is not the controller in charge.
BFFF0067  Operation is not supported on this session.    Jt  ?     K    Number Of Bursts                  K ?O??    ?    Status                                ?                 	           ?    This control contains the status code returned by  the  function call.

Status :
------

Codes     Description
----------------------------------------------------------------
       0  No error.

3FFF0005  The specified termination character was read.
3FFF0006  The specified number of bytes was read.

BBFC0800  Error Register STB.

BFFC0001  Parameter 1 out of range.
BFFC0002  Parameter 2 out of range.
BFFC0003  Parameter 3 out of range.
BFFC0004  Parameter 4 out of range.
BFFC0005  Parameter 5 out of range.
BFFC0006  Parameter 6 out of range.
BFFC0007  Parameter 7 out of range.
BFFC0008  Parameter 8 out of range.

BFFF0000  Miscellaneous or system error occurred.
BFFF000E  Invalid session handle.
BFFF0015  Timeout occurred before operation could complete.
BFFF0034  Violation of raw write protocol occurred.
BFFF0035  Violation of raw read protocol occurred.
BFFF0036  Device report an output protocol error.
BFFF0037  Device reported an input protocol error.
BFFF0038  Bus error occurred during transfer.
BFFF003A  Invalid setup (attributes are not consistent).
BFFF005F  No listeners condition was detected.
BFFF0060  This interface is not the controller in charge.
BFFF0067  Operation is not supported on this session.    ?    This control specifies the data type.

Value range :
-----------    
    0 - PRBS     : Pseudo Random Bit Sequence;   Random  numbers
                   similar to those sent during real  operation. 
                   Drift measurement not possible.

    1 - 01010101 : Data sequence with the smallest deviation.

    2 - 00110011 : Drift measurement not possible.

    3 - 00001111 : This  data  pattern  is  interpreted  by  the 
                   converters (CODEC) as silence. Drift measure- 
                   -ment not possible.

    4 - FIG 31   : This data pattern  is  best  suited  for  the
                   modulation measurement, since large areas  of 
                   successive 0  and 1 occur (measurement of the
                   maximum modulation deviation) and there is an
                   alternating sequence of 0 and 1 at the end.
                   This is the prerequisite for  performing  the 
                   drift measurement.

Default value :
-------------
    4 - FIG 31    P? ?E??    ?    Status                            Ua  ?           Data Type in B Field               	                      1PRBS 0 01010101 1 00110011 2 00001111 3 FIG 31 4    ?    This control specifies the number of bursts over which the  time
values are averaged.

Value range :
-----------
    0 to 1000

Default value :
-------------
    32    ?    This control contains the status code returned by  the  function call.

Status :
------

Codes     Description
----------------------------------------------------------------
       0  No error.

3FFF0005  The specified termination character was read.
3FFF0006  The specified number of bytes was read.

BBFC0800  Error Register STB.

BFFC0001  Parameter 1 out of range.
BFFC0002  Parameter 2 out of range.
BFFC0003  Parameter 3 out of range.
BFFC0004  Parameter 4 out of range.
BFFC0005  Parameter 5 out of range.
BFFC0006  Parameter 6 out of range.
BFFC0007  Parameter 7 out of range.
BFFC0008  Parameter 8 out of range.

BFFF0000  Miscellaneous or system error occurred.
BFFF000E  Invalid session handle.
BFFF0015  Timeout occurred before operation could complete.
BFFF0034  Violation of raw write protocol occurred.
BFFF0035  Violation of raw read protocol occurred.
BFFF0036  Device report an output protocol error.
BFFF0037  Device reported an input protocol error.
BFFF0038  Bus error occurred during transfer.
BFFF003A  Invalid setup (attributes are not consistent).
BFFF005F  No listeners condition was detected.
BFFF0060  This interface is not the controller in charge.
BFFF0067  Operation is not supported on this session.    Z  P     K    Number Of Bursts                  Z? ? ???    ?    Status                               ?                  	           ?    This control specifies the measuring window  bursts  (one  burst 
with 320 net bits per slot. Frames are repeated with a period of 10ms).

Value range :
-----------
    1 to 30e6

Default value :
-------------
    100    ?    This control contains the status code returned by  the  function call.

Status :
------

Codes     Description
----------------------------------------------------------------
       0  No error.

3FFF0005  The specified termination character was read.
3FFF0006  The specified number of bytes was read.

BBFC0800  Error Register STB.

BFFC0001  Parameter 1 out of range.
BFFC0002  Parameter 2 out of range.
BFFC0003  Parameter 3 out of range.
BFFC0004  Parameter 4 out of range.
BFFC0005  Parameter 5 out of range.
BFFC0006  Parameter 6 out of range.
BFFC0007  Parameter 7 out of range.
BFFC0008  Parameter 8 out of range.

BFFF0000  Miscellaneous or system error occurred.
BFFF000E  Invalid session handle.
BFFF0015  Timeout occurred before operation could complete.
BFFF0034  Violation of raw write protocol occurred.
BFFF0035  Violation of raw read protocol occurred.
BFFF0036  Device report an output protocol error.
BFFF0037  Device reported an input protocol error.
BFFF0038  Bus error occurred during transfer.
BFFF003A  Invalid setup (attributes are not consistent).
BFFF005F  No listeners condition was detected.
BFFF0060  This interface is not the controller in charge.
BFFF0067  Operation is not supported on this session.    `+ H ?     K    Evaluation Window                 a ? ???    ?    Status                             ?À      d           	          ?    This control contains the status code returned by  the  function call.

Status :
------

Codes     Description
----------------------------------------------------------------
       0  No error.

3FFF0005  The specified termination character was read.
3FFF0006  The specified number of bytes was read.

BBFC0800  Error Register STB.

BFFC0001  Parameter 1 out of range.
BFFC0002  Parameter 2 out of range.
BFFC0003  Parameter 3 out of range.
BFFC0004  Parameter 4 out of range.
BFFC0005  Parameter 5 out of range.
BFFC0006  Parameter 6 out of range.
BFFC0007  Parameter 7 out of range.
BFFC0008  Parameter 8 out of range.

BFFF0000  Miscellaneous or system error occurred.
BFFF000E  Invalid session handle.
BFFF0015  Timeout occurred before operation could complete.
BFFF0034  Violation of raw write protocol occurred.
BFFF0035  Violation of raw read protocol occurred.
BFFF0036  Device report an output protocol error.
BFFF0037  Device reported an input protocol error.
BFFF0038  Bus error occurred during transfer.
BFFF003A  Invalid setup (attributes are not consistent).
BFFF005F  No listeners condition was detected.
BFFF0060  This interface is not the controller in charge.
BFFF0067  Operation is not supported on this session.        This control specifies the transmit power level in dBm.

Value range :
-----------
    The  range  of  the  RF  level  depends  on  the  external 
    attenuation and the selected  RF  connector  as  follows :
    
    When RF OUT selected :
        -100.0 - ext.att. ... -40.0 - ext.att. dBm 

    When RF OUT2 and RF IN selected :
        -40.0 - ext.att.  ... +7.5 - ext.att. dBm

    When RF OUT2 and RF IN2 selected 
        -20.0 - ext.att.  ... +7.5 - ext.att. dBm

Default value :
-------------
    -73.0 dBm
    fv ? ???    ?    Status                            kQ  ?     d    RF Level (dBm)                     	            -73.0   ?    This control contains the status code returned by  the  function call.

Status :
------

Codes     Description
----------------------------------------------------------------
       0  No error.

3FFF0005  The specified termination character was read.
3FFF0006  The specified number of bytes was read.

BBFC0800  Error Register STB.

BFFC0001  Parameter 1 out of range.
BFFC0002  Parameter 2 out of range.
BFFC0003  Parameter 3 out of range.
BFFC0004  Parameter 4 out of range.
BFFC0005  Parameter 5 out of range.
BFFC0006  Parameter 6 out of range.
BFFC0007  Parameter 7 out of range.
BFFC0008  Parameter 8 out of range.

BFFF0000  Miscellaneous or system error occurred.
BFFF000E  Invalid session handle.
BFFF0015  Timeout occurred before operation could complete.
BFFF0034  Violation of raw write protocol occurred.
BFFF0035  Violation of raw read protocol occurred.
BFFF0036  Device report an output protocol error.
BFFF0037  Device reported an input protocol error.
BFFF0038  Bus error occurred during transfer.
BFFF003A  Invalid setup (attributes are not consistent).
BFFF005F  No listeners condition was detected.
BFFF0060  This interface is not the controller in charge.
BFFF0067  Operation is not supported on this session.    m? \q??    ?    Status                             	           ?    This control contains the status code returned by  the  function call.

Status :
------

Codes     Description
----------------------------------------------------------------
       0  No error.

3FFF0005  The specified termination character was read.
3FFF0006  The specified number of bytes was read.

BBFC0800  Error Register STB.

BFFC0001  Parameter 1 out of range.
BFFC0002  Parameter 2 out of range.
BFFC0003  Parameter 3 out of range.
BFFC0004  Parameter 4 out of range.
BFFC0005  Parameter 5 out of range.
BFFC0006  Parameter 6 out of range.
BFFC0007  Parameter 7 out of range.
BFFC0008  Parameter 8 out of range.

BFFF0000  Miscellaneous or system error occurred.
BFFF000E  Invalid session handle.
BFFF0015  Timeout occurred before operation could complete.
BFFF0034  Violation of raw write protocol occurred.
BFFF0035  Violation of raw read protocol occurred.
BFFF0036  Device report an output protocol error.
BFFF0037  Device reported an input protocol error.
BFFF0038  Bus error occurred during transfer.
BFFF003A  Invalid setup (attributes are not consistent).
BFFF005F  No listeners condition was detected.
BFFF0060  This interface is not the controller in charge.
BFFF0067  Operation is not supported on this session.    r? ???    ?    Status                             	               Return the state of the CMD60.

Permissible states :

    1 -> IDLE (initial state) 
    2 -> DFP  (DECT:Fixed Part)
    3 -> DPP  (DECT:Portable Part)
    4 -> DCE  (DECT:Connection Established)
    5 -> DBAN (DECT:Burst Analysis)
    6 -> DRFG (DECT:RF Generator)
    ?    This control contains the status code returned by  the  function call.

Status :
------

Codes     Description
----------------------------------------------------------------
       0  No error.

3FFF0005  The specified termination character was read.
3FFF0006  The specified number of bytes was read.

BBFC0800  Error Register STB.

BFFC0001  Parameter 1 out of range.
BFFC0002  Parameter 2 out of range.
BFFC0003  Parameter 3 out of range.
BFFC0004  Parameter 4 out of range.
BFFC0005  Parameter 5 out of range.
BFFC0006  Parameter 6 out of range.
BFFC0007  Parameter 7 out of range.
BFFC0008  Parameter 8 out of range.

BFFF0000  Miscellaneous or system error occurred.
BFFF000E  Invalid session handle.
BFFF0015  Timeout occurred before operation could complete.
BFFF0034  Violation of raw write protocol occurred.
BFFF0035  Violation of raw read protocol occurred.
BFFF0036  Device report an output protocol error.
BFFF0037  Device reported an input protocol error.
BFFF0038  Bus error occurred during transfer.
BFFF003A  Invalid setup (attributes are not consistent).
BFFF005F  No listeners condition was detected.
BFFF0060  This interface is not the controller in charge.
BFFF0067  Operation is not supported on this session.    x > ?      `    Current Instrument State          y+ ? ???    ?    Status                             	            	            ?    This control specifies the signalling status.

Return :
------
   0 : LOCK | the synchronization is successful (LOCKED).
   1 : UNL  | the synchronization is not successful (UNLOCKED).
 
    ?    This control contains the status code returned by  the  function call.

Status :
------

Codes     Description
----------------------------------------------------------------
       0  No error.

3FFF0005  The specified termination character was read.
3FFF0006  The specified number of bytes was read.

BBFC0800  Error Register STB.

BFFC0001  Parameter 1 out of range.
BFFC0002  Parameter 2 out of range.
BFFC0003  Parameter 3 out of range.
BFFC0004  Parameter 4 out of range.
BFFC0005  Parameter 5 out of range.
BFFC0006  Parameter 6 out of range.
BFFC0007  Parameter 7 out of range.
BFFC0008  Parameter 8 out of range.

BFFF0000  Miscellaneous or system error occurred.
BFFF000E  Invalid session handle.
BFFF0015  Timeout occurred before operation could complete.
BFFF0034  Violation of raw write protocol occurred.
BFFF0035  Violation of raw read protocol occurred.
BFFF0036  Device report an output protocol error.
BFFF0037  Device reported an input protocol error.
BFFF0038  Bus error occurred during transfer.
BFFF003A  Invalid setup (attributes are not consistent).
BFFF005F  No listeners condition was detected.
BFFF0060  This interface is not the controller in charge.
BFFF0067  Operation is not supported on this session.    ~? K ?      `    Lock State                        M ? ???    ?    Status                             	            	                This control specifies the received RFPI of the Fixed-Part under
test.

Return :
------
    String (10 digits in hex format).    ?    This control contains the status code returned by  the  function call.

Status :
------

Codes     Description
----------------------------------------------------------------
       0  No error.

3FFF0005  The specified termination character was read.
3FFF0006  The specified number of bytes was read.

BBFC0800  Error Register STB.
BBFC0801  Error invalid measurement.

BFFC0001  Parameter 1 out of range.
BFFC0002  Parameter 2 out of range.
BFFC0003  Parameter 3 out of range.
BFFC0004  Parameter 4 out of range.
BFFC0005  Parameter 5 out of range.
BFFC0006  Parameter 6 out of range.
BFFC0007  Parameter 7 out of range.
BFFC0008  Parameter 8 out of range.

BFFF0000  Miscellaneous or system error occurred.
BFFF000E  Invalid session handle.
BFFF0015  Timeout occurred before operation could complete.
BFFF0034  Violation of raw write protocol occurred.
BFFF0035  Violation of raw read protocol occurred.
BFFF0036  Device report an output protocol error.
BFFF0037  Device reported an input protocol error.
BFFF0038  Bus error occurred during transfer.
BFFF003A  Invalid setup (attributes are not consistent).
BFFF005F  No listeners condition was detected.
BFFF0060  This interface is not the controller in charge.
BFFF0067  Operation is not supported on this session.    ?? W ?     h    RFPI Value                        ?1 ? ???    ?    Status                             	            	           E    This control specifies the verdict of the measured NTP according 
the tolerances setting in the configuration  menu  "Power Ramp /
NTP Tolerance Range".

Return :
------
   0 : MATC | measurement does not exceed the tolerance.
   1 : NMAT | measurement exceeds the tolerance.
   2 : INV  | no measurement result available.
    ?    This control contains the status code returned by  the  function call.

Status :
------

Codes     Description
----------------------------------------------------------------
       0  No error.

3FFF0005  The specified termination character was read.
3FFF0006  The specified number of bytes was read.

BBFC0800  Error Register STB.

BFFC0001  Parameter 1 out of range.
BFFC0002  Parameter 2 out of range.
BFFC0003  Parameter 3 out of range.
BFFC0004  Parameter 4 out of range.
BFFC0005  Parameter 5 out of range.
BFFC0006  Parameter 6 out of range.
BFFC0007  Parameter 7 out of range.
BFFC0008  Parameter 8 out of range.

BFFF0000  Miscellaneous or system error occurred.
BFFF000E  Invalid session handle.
BFFF0015  Timeout occurred before operation could complete.
BFFF0034  Violation of raw write protocol occurred.
BFFF0035  Violation of raw read protocol occurred.
BFFF0036  Device report an output protocol error.
BFFF0037  Device reported an input protocol error.
BFFF0038  Bus error occurred during transfer.
BFFF003A  Invalid setup (attributes are not consistent).
BFFF005F  No listeners condition was detected.
BFFF0060  This interface is not the controller in charge.
BFFF0067  Operation is not supported on this session.    ?? > ?      `    Verdict for NTP                   ?  ? ???    ?    Status                             	            	           H    This  control  specifies  the  verdict  of  the  measured  power according the  tolerances  setting  in  the  configuration  menu
"Power Ramp / Tolerance Range For Power Time".

Return :
------
   0 : MATC | signal matches template.
   1 : NMAT | signal does not match template.
   2 : INV  | no measurement result available.
    ?    This control contains the status code returned by  the  function call.

Status :
------

Codes     Description
----------------------------------------------------------------
       0  No error.

3FFF0005  The specified termination character was read.
3FFF0006  The specified number of bytes was read.

BBFC0800  Error Register STB.

BFFC0001  Parameter 1 out of range.
BFFC0002  Parameter 2 out of range.
BFFC0003  Parameter 3 out of range.
BFFC0004  Parameter 4 out of range.
BFFC0005  Parameter 5 out of range.
BFFC0006  Parameter 6 out of range.
BFFC0007  Parameter 7 out of range.
BFFC0008  Parameter 8 out of range.

BFFF0000  Miscellaneous or system error occurred.
BFFF000E  Invalid session handle.
BFFF0015  Timeout occurred before operation could complete.
BFFF0034  Violation of raw write protocol occurred.
BFFF0035  Violation of raw read protocol occurred.
BFFF0036  Device report an output protocol error.
BFFF0037  Device reported an input protocol error.
BFFF0038  Bus error occurred during transfer.
BFFF003A  Invalid setup (attributes are not consistent).
BFFF005F  No listeners condition was detected.
BFFF0060  This interface is not the controller in charge.
BFFF0067  Operation is not supported on this session.    ?] !      `    Verdict for Power Ramp            ?? ????    ?    Status                             	            	           ?    This control contains the status code returned by  the  function call.

Status :
------

Codes     Description
----------------------------------------------------------------
       0  No error.

3FFF0005  The specified termination character was read.
3FFF0006  The specified number of bytes was read.

BBFC0800  Error Register STB.
BBFC0801  Error invalid measurement.

BFFC0001  Parameter 1 out of range.
BFFC0002  Parameter 2 out of range.
BFFC0003  Parameter 3 out of range.
BFFC0004  Parameter 4 out of range.
BFFC0005  Parameter 5 out of range.
BFFC0006  Parameter 6 out of range.
BFFC0007  Parameter 7 out of range.
BFFC0008  Parameter 8 out of range.

BFFF0000  Miscellaneous or system error occurred.
BFFF000E  Invalid session handle.
BFFF0015  Timeout occurred before operation could complete.
BFFF0034  Violation of raw write protocol occurred.
BFFF0035  Violation of raw read protocol occurred.
BFFF0036  Device report an output protocol error.
BFFF0037  Device reported an input protocol error.
BFFF0038  Bus error occurred during transfer.
BFFF003A  Invalid setup (attributes are not consistent).
BFFF005F  No listeners condition was detected.
BFFF0060  This interface is not the controller in charge.
BFFF0067  Operation is not supported on this session.     S    This control specifies the measured NTP.

Return :
------
    Value (Unit : dBm).    ?
 ? ???    ?    Status                            ?
 W     `    NTP Value                          	            	           c    This control specifies the measured Longterm BER.

Return :
------
   Longterm BER (Unit : None).     a    This control specifies the measured Longterm FER.

Return :
------
   Longterm FER (Unit : %).
    ?    This control contains the status code returned by  the  function call.

Status :
------

Codes     Description
----------------------------------------------------------------
       0  No error.

3FFF0005  The specified termination character was read.
3FFF0006  The specified number of bytes was read.

BBFC0800  Error Register STB.
BBFC0801  Error invalid measurement.

BFFC0001  Parameter 1 out of range.
BFFC0002  Parameter 2 out of range.
BFFC0003  Parameter 3 out of range.
BFFC0004  Parameter 4 out of range.
BFFC0005  Parameter 5 out of range.
BFFC0006  Parameter 6 out of range.
BFFC0007  Parameter 7 out of range.
BFFC0008  Parameter 8 out of range.

BFFF0000  Miscellaneous or system error occurred.
BFFF000E  Invalid session handle.
BFFF0015  Timeout occurred before operation could complete.
BFFF0034  Violation of raw write protocol occurred.
BFFF0035  Violation of raw read protocol occurred.
BFFF0036  Device report an output protocol error.
BFFF0037  Device reported an input protocol error.
BFFF0038  Bus error occurred during transfer.
BFFF003A  Invalid setup (attributes are not consistent).
BFFF005F  No listeners condition was detected.
BFFF0060  This interface is not the controller in charge.
BFFF0067  Operation is not supported on this session.    ??  7     `    BER Value                         ?R |    `    FER Value                         ?? ?c??    ?    Status                             	           	           	            ?    This control returns the error  message  string  read  from  the instrument's error message queue.

Return :
------
    See SCPI, section 21.7 

Note :
----
    The array must contain at least 256 elements ViChar[256].    ?    This control contains the status code returned by  the  function call.

Status :
------

Codes     Description
----------------------------------------------------------------
       0  No error.

BFFF0000  Miscellaneous or system error occurred.
BFFF000E  Invalid session handle.
BFFF0015  Timeout occurred before operation could complete.
BFFF0034  Violation of raw write protocol occurred.
BFFF0035  Violation of raw read protocol occurred.
BFFF0037  Device reported an input protocol error.
BFFF0038  Bus error occurred during transfer.
BFFF003A  Invalid setup (attributes are not consistent).
BFFF003C  Insufficient system resources to allocate a buffer.
BFFF003E  I/O error occurred during read operation.
BFFF003F  A specifier in the format string is invalid.
BFFF0041  A specifier in the format string is not supported.
BFFF005F  No listeners condition was detected.
BFFF0060  This interface is not the controller in charge.
BFFF0067  Operation is not supported on this session.     ?    This  control  specifies  the  status  code  returned  from  the instrument driver functions.

Default value :
-------------
    0 - VI_SUCCESS    ?~ 2 ?   ,    Error Message                     ?b ?,??    ?    Status                            ?F 1       `    Error Code                         	            	            0        1-success 0-fail  ???? a ?     ?    command                         ????' ???                                         ???? ? ???    ?    return                                 write scpi command    	                scpi command         output         1-success 0-fail    ?z > ?     ?    command                           ?? ? ?    ?    output                            ?? ???    ?    return                                 	            	                1-success 0-fail    ?u ? ???    ?    return                             	                1-success 0-fail    ?? p ???    ?    return                             	                return Frequency Offsetin Hz    ?+ > ?     ?    freqoffset                      ???? ? ???    ?    return                             	            	            s    B-Field modulation information ?Cthe return values are in Hz.
Positive Deviation<value>,Negative Deviation<value>    ?? F ?     ?    BField                          ???? ? ???    ?    return                             	            	            "    Frequency Drift - SLOT  kHz/Slot    ?? 7 ?     ?    FreqDriftSlot                   ???? ? ???    ?    return                             	            	               This command is used for matching the actual power ramp to the
configured power ramp template.
If it returns NMAT the signal does not match the template.
If it returns MATCH the signal matches the template.
If it returns INV there are no available measurement results.    ?| D ?     ?    matching                        ???? ? ???    ?    return                             	            	         ????       ????  i             K.        Initialize                                                                                                                              ????       ????  ?             K.        ConfTest                                                                                                                                ????       ????               K.        ProcSelTest                                                                                                                             ????       ????  j             K.        ConfFpSignMode                                                                                                                          ????       ????  ?             K.        ConfFpAnt                                                                                                                               ????       ????  &	             K.        ConfFpRfLev                                                                                                                             ????       ????  0             K.        ConfFpChannels                                                                                                                          ????       ????  7             K.        ConfFpRfpi                                                                                                                              ????       ????  =F             K.        ConfMessEscMT                                                                                                                           ????       ????  B?             K.        CalcLimPowTranTempCle                                                                                                                   ????       ????  Is             K.        CalcLimPowTranTempTol                                                                                                                   ????       ????  O?             K.        ConfAverBurs                                                                                                                            ????       ????  Y_             K.        ConfModDataType                                                                                                                         ????       ????  _?             K.        ConfTimAverBurs                                                                                                                         ????       ????  e?             K.        ConfBerEvalWind                                                                                                                         ????       ????  m`             K.        ConfBerRfLev                                                                                                                            ????       ????  r?             K.        ProcConnRel                                                                                                                             ????       ????  w?             K.        ProcConnSet                                                                                                                             ????       ????  ~             K.        StatDev                                                                                                                                 ????       ????  ?(             K.        SensSignStat                                                                                                                            ????       ????  ?1             K.        SensDetRfpi                                                                                                                             ????       ????  ??             K.        CalcLimPowTranTolMatc                                                                                                                   ????       ????  ??             K.        CalcLimPowTranMatc                                                                                                                      ????       ????  ?e             K.        ReadNtp                                                                                                                                 ????       ????  ??             K.        ReadBerLter                                                                                                                             ????       ????                    ??          Reset                                                                                                                                   ????       ????  ??             K.        ErrorQuery                                                                                                                              ????         ??  ??             K.        Write                                                                                                                                   ????       ????  ??             K.        Query                                                                                                                                   ????       ????  ??             K.        Loopback_Mode_EU_Setting                                                                                                                ????       ????  ??             K.        Nonsignal_Connect                                                                                                                       ????       ????  ?Q             K.        Read_FrequencyOffset                                                                                                                    ????       ????  ?N             K.        Read_Deviation                                                                                                                          ????       ????  ??             K.        Read_FrequencyDriftSlot                                                                                                                 ????       ????  ??             K.        Read_TemplateMatch                                                                                                                          ????                                                                                     ?Initialize                                                                        ????Configuration Functions                                                           ????Idle                                                                                 ?Test Type                                                                            ?Test Mode                                                                         ????Signalling PP Test                                                                ????                                                                                  ????Signalling FP Test                                                                   ?Signalling Mode                                                                      ?Antenna of the Fix-Part                                                              ?RF Level                                                                             ?Channel Parameters                                                                   ?Radio Fixed Part Identity                                                         ????TX Test                                                                              ?MT Escape Sequence                                                                ????Power Ramp                                                                           ?Reset To Default Values                                                              ?NTP Tolerance Range                                                                  ?Number Of Bursts                                                                  ????RF Modulation                                                                        ?Data Type In B-Field                                                              ????Timing                                                                               ?Number of Bursts                                                                  ????BER                                                                                  ?Evaluation Window                                                                 ????Measurement                                                                          ?RF Level                                                                          ????Connection                                                                           ?Connection Release                                                                   ?Setup Connection                                                                  ????Action/Status Functions                                                           ????Internal Instrument State                                                            ?Status                                                                            ????Data Functions                                                                    ????Signalling FP Test                                                                   ?Lock State                                                                           ?RFPI of the DUT                                                                   ????Power Ramp                                                                        ????Verdict                                                                              ?NTP                                                                                  ?Power Ramp Signal                                                                 ????Measurement                                                                          ?NTP                                                                               ????BER                                                                               ????Measurement                                                                          ?LongTerm BER & FER                                                                ????Utility Functions                                                                    ?Reset                                                                                ?Error-Query                                                                          ?Write                                                                                ?Query                                                                             ????Old                                                                                  ?Loopback_Mode_EU_Setting                                                             ?Nonsignal_Connect                                                                    ?Read_FrequencyOffset                                                                 ?Read_Deviation                                                                       ?Read_FrequencyDriftSlot                                                              ?Read_TemplateMatch                                                              
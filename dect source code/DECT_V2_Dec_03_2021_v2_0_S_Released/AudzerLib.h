#include "extcode.h"
#pragma pack(push)
#pragma pack(1)

#ifdef __cplusplus
extern "C" {
#endif
typedef uint8_t  Enum;
#define Enum__0 0
#define Enum__1 1
typedef struct {
	int32_t dimSize;
	double elt[1];
} DoubleArrayBase;
typedef DoubleArrayBase **DoubleArray;
typedef struct {
	int32_t dimSize;
	double _[1];
} DoubleArray1Base;
typedef DoubleArray1Base **DoubleArray1;
typedef struct {
	int32_t dimSize;
	int16_t elt[1];
} Int16ArrayBase;
typedef Int16ArrayBase **Int16Array;
typedef struct {
	int32_t dimSize;
	double Numeric[1];
} DoubleArray2Base;
typedef DoubleArray2Base **DoubleArray2;

/*!
 * Sweep_signal_play_status
 */
int16_t __cdecl Sweep_signal_play_status(uint32_t *status);
/*!
 * Sweep_signal_analysis
 */
int16_t __cdecl Sweep_signal_analysis(double StartingFrequency, 
	double TerminationFrequency, double Octave, double SoundCardMaxOutputRMS, 
	double Amplitude, double AmplifierGain, double PlaySamplingRate, 
	double BitOfPlaybackSamples, double InputSampleRate, 
	double InputBitsPerSample, double HighestHarmonic, 
	double MicrophoneSensitivity, double SoundCardMaxInputRMS, Enum Channel, 
	DoubleArray *arrayFreq, DoubleArray1 *Thd_data, DoubleArray1 *Frequency_data);
/*!
 * Sweep_signal_capture
 */
int16_t __cdecl Sweep_signal_capture(uint32_t deviceID, 
	int32_t bitsPerSample, double sampleRate, int32_t numberOfChannels, 
	Int16Array *output);
/*!
 * Sweep_signal_capture_stop
 */
int16_t __cdecl Sweep_signal_capture_stop(void);
/*!
 * Sweep_signal_open
 */
int16_t __cdecl Sweep_signal_open(char String[], DoubleArray2 *ArrayL, 
	DoubleArray2 *ArrayR, int32_t *bitsPerSample, double *sampleRateSS, 
	int32_t *numberOfChannels);
/*!
 * Sweep_signal_play
 */
int16_t __cdecl Sweep_signal_play(uint32_t outputID, double sampleRateSS, 
	int32_t numberOfChannels, int32_t bitsPerSample);
/*!
 * Sweep_signal_produce
 */
int16_t __cdecl Sweep_signal_produce(double StartingFrequency, 
	double TerminationFrequency, double Octave, double SoundCardMaxOutputRMS, 
	double Amplitude, double AmplifierGain, double PlaySamplingRate, 
	double BitOfPlaybackSamples, DoubleArray *NumberArray, 
	DoubleArray *FrequencyArray, DoubleArray *Output);
/*!
 * Sweep_signal_save
 */
int16_t __cdecl Sweep_signal_save(char Path[], char FileName[], 
	double sampleRateSS, int32_t numberOfChannels, int32_t bitsPerSample);
/*!
 * Calibration_Amplifier
 */
int16_t __cdecl Calibration_Amplifier(uint32_t InputID, uint32_t OutputID, 
	double SoundCardMaxOutputVoltageRMS, double SoundCardMaxInputVoltageRMS, 
	double *AmplifierGainVValue, double *AmplifierGaindbValue, 
	int32_t *SoundCardDelay);
/*!
 * Calibration_Date_Read
 */
int16_t __cdecl Calibration_Date_Read(char String[], PStr Segment, 
	double *SoundCardMaxOutputVoltageRMS, double *SoundCardMaxInputVoltageRMS, 
	double *SoundCardDelay, double *AmplifierGaindbValue, 
	double *AmplifierGainVValue, double *MicrophoneSensitivity);
/*!
 * Calibration_Date_Save
 */
int16_t __cdecl Calibration_Date_Save(double SoundCardMaxOutputVoltageRMS, 
	double SoundCardMaxInputVoltageRMS, double SoundCardDelay, 
	double AmplifierGaindbValue, double AmplifierGainVValue, 
	double MicrophoneSensitivity, PStr Segment, char String[]);
/*!
 * Calibration_MIC
 */
int16_t __cdecl Calibration_MIC(uint32_t MICID, double SoundCardMaxInpuRMS, 
	double PreGain, double *MicrophoneSensitivity);
/*!
 * Calibration_Soundcard_input
 */
int16_t __cdecl Calibration_Soundcard_input(uint32_t OutputID, 
	uint32_t InputID, double SoundCardMaxOutputVoltageRMS, 
	double *SoundCardMaxInputVoltageRMS);
/*!
 * Calibration_Soundcard_output
 */
int16_t __cdecl Calibration_Soundcard_output(uint32_t OutputID);
/*!
 * Single_tone_analysis
 */
int16_t __cdecl Single_tone_analysis(uint8_t analysis_channel, 
	double mic_sensetivity, double sampleRate, double *detectedFrequency, 
	double *detectedAmplitude, double *THD);
/*!
 * Single_tone_capture
 */
int32_t __cdecl Single_tone_capture(uint8_t record_id, double sample_rate, 
	int32_t channel_count, int32_t sample_bit, DoubleArray *ArrayL, 
	DoubleArray *ArrayR);
/*!
 * Single_tone_capture_status
 */
int16_t __cdecl Single_tone_capture_status(uint8_t *captureStatus);
/*!
 * Single_tone_save
 */
int32_t __cdecl Single_tone_save(char path[], char filename[], 
	int32_t channelCount, double sampleRate, int32_t sampleBit);
/*!
 * Single_tone_open
 */
int32_t __cdecl Single_tone_open(char path[], DoubleArray *arrayL, 
	DoubleArray *arrayR, LVBoolean *read);
/*!
 * Single_tone_play
 */
int32_t __cdecl Single_tone_play(uint8_t play_id, double volume, 
	int32_t sampleBit, int32_t channelCount, double sampleRate);
/*!
 * Single_tone_play_status
 */
int16_t __cdecl Single_tone_play_status(uint8_t *playStatus);
/*!
 * Single_tone_capture_stop
 */
int16_t __cdecl Single_tone_capture_stop(void);
/*!
 * Sweep_record_signal_analysis
 */
int16_t __cdecl Sweep_record_signal_analysis(char file_path[], 
	double StartingFrequency, double TerminationFrequency, double Octave, 
	double SoundCardMaxOutputRMS, double Amplitude, double AmplifierGain, 
	double PlaySamplingRate, double BitOfPlaybackSamples, double InputSampleRate, 
	double InputBitsPerSample, double HighestHarmonic, 
	double MicrophoneSensitivity, double SoundCardMaxInputRMS, 
	DoubleArray *arrayFreq, DoubleArray1 *Frequency_data, DoubleArray1 *THD_data);
/*!
 * Single_tone_creat
 */
int32_t __cdecl Single_tone_creat(double Frequency, double SamplingRate, 
	double Spl, double time, DoubleArray *single_data);

MgErr __cdecl LVDLLStatus(char *errStr, int errStrLen, void *module);

/*
* Memory Allocation/Resize/Deallocation APIs for type 'DoubleArray'
*/
DoubleArray __cdecl AllocateDoubleArray (int32 elmtCount);
MgErr __cdecl ResizeDoubleArray (DoubleArray *hdlPtr, int32 elmtCount);
MgErr __cdecl DeAllocateDoubleArray (DoubleArray *hdlPtr);

/*
* Memory Allocation/Resize/Deallocation APIs for type 'DoubleArray1'
*/
DoubleArray1 __cdecl AllocateDoubleArray1 (int32 elmtCount);
MgErr __cdecl ResizeDoubleArray1 (DoubleArray1 *hdlPtr, int32 elmtCount);
MgErr __cdecl DeAllocateDoubleArray1 (DoubleArray1 *hdlPtr);

/*
* Memory Allocation/Resize/Deallocation APIs for type 'Int16Array'
*/
Int16Array __cdecl AllocateInt16Array (int32 elmtCount);
MgErr __cdecl ResizeInt16Array (Int16Array *hdlPtr, int32 elmtCount);
MgErr __cdecl DeAllocateInt16Array (Int16Array *hdlPtr);

/*
* Memory Allocation/Resize/Deallocation APIs for type 'DoubleArray2'
*/
DoubleArray2 __cdecl AllocateDoubleArray2 (int32 elmtCount);
MgErr __cdecl ResizeDoubleArray2 (DoubleArray2 *hdlPtr, int32 elmtCount);
MgErr __cdecl DeAllocateDoubleArray2 (DoubleArray2 *hdlPtr);

void __cdecl SetExcursionFreeExecutionSetting(Bool32 value);

#ifdef __cplusplus
} // extern "C"
#endif

#pragma pack(pop)


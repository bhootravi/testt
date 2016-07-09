#ifndef AUDIO_H_
#define AUDIO_H_

#include "asf.h"
#include "file_handling.h"

#define AUDIO_OUT_ID	ID_DACC

#define DACC_ANALOG_CONTROL (DACC_ACR_IBCTLCH0(0x02) \
								| DACC_ACR_IBCTLCH1(0x02) \
								| DACC_ACR_IBCTLDACCORE(0x01))
								
#define AUDIO_BUF_SIZE  1024

#define DATA_BITS		16
#define DAC_BITS		12
	
extern FIL file_object;							
//extern volatile uint8_t audio_ready;								

//Initialise the settings for DAC
//DOESN'T ENABLE THE CHANNEL
void audio_init(void);

//open the file and read the headers of wav file 
//read the first block into the buffer 
int audio_open_file(char* file_name);

//uses 2 buffers to provide data to DAC
//one buffer if used to supply data to DAC
//while other is filled with new data from memory(this is
//needed because of slow memory access)
//playing uses only the 12 most significant bits LSBs are discarded
//NOTE reads only in multiples of AUDIO_BUF_SIZE*2
void play(void);

#endif /* AUDIO_H_ */
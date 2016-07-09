#include "audio.h"

//audio file object
FIL file_object;

uint32_t nb_audio_blocks;

volatile uint8_t audio_ready = 0, end_buf = 0;

//counter for number of samples played(used to refresh the buffer)
volatile uint16_t t = 0;

//stucture to store the headers of wav file
COMPILER_PACK_SET(1)
struct wav_header {
	char ChunkID[4];
	uint32_t ChunkSize;
	char Format[4];
	char Subchunk1ID[4];
	uint32_t Subchunk1Size;
	uint16_t AudioFormat;
	uint16_t NumChannels;
	uint32_t SampleRate;
	uint32_t ByteRate;
	uint16_t BlockAlign;
	uint16_t BitsPerSample;
	char Subchunk2ID[4];
	uint32_t Subchunk2Size;
};
struct wav_header audio_header;
COMPILER_PACK_RESET()

//buffer to read the data from file(stores data of both channels Right and Left)
int16_t temp_audio_buf[AUDIO_BUF_SIZE];
//stores the processed data to be supplied to DAC for output
uint16_t audio_data_0[AUDIO_BUF_SIZE/2], audio_data_1[AUDIO_BUF_SIZE/2];
//pointer for data to be fed to DAC
volatile uint16_t *audio_data_ptr = &audio_data_0[0];
volatile uint16_t *read_data_ptr = &audio_data_1[0];

//Feed DAC with data at the sampling rate of file
void SysTick_Handler(void)
{
	if(end_buf)
	{
		dacc_disable_channel(DACC, 0);
		NVIC_DisableIRQ(SysTick_IRQn);
		audio_ready = 0;
		return;
	}
	uint32_t status = dacc_get_interrupt_status(DACC);
	if (t < AUDIO_BUF_SIZE/2 && ((status & DACC_ISR_TXRDY) == DACC_ISR_TXRDY))
	{
		t++;
		dacc_write_conversion_data(DACC, *audio_data_ptr++);
	}
}

//TODO change the timings for better output(add triggers if needed)

void audio_init()
{
	
	sysclk_enable_peripheral_clock(AUDIO_OUT_ID);
	dacc_reset(DACC);
	
	
	/* Half word(16 bit) transfer mode */
	dacc_set_transfer_mode(DACC, 0);
	
	dacc_set_timing(DACC, 0x08, 1, 0x10);
		
	dacc_set_channel_selection(DACC, 0);
	dacc_set_analog_control(DACC, DACC_ANALOG_CONTROL);
	
}

//TODO - check DAC active or not before opening new file
//TODO - check if the file is WAV before opening
int audio_open_file(char* file_name)
{
	//can't open file when an audio play is in progress
	if(audio_ready)
		return -2;
	FRESULT res;
	//mount the device if not already mounted
	#ifndef FS_MOUNTED
		fs_mount();
	#endif
	
	/* Open the WAV file */
	printf("Open a file (f_open)...\r\n");
	res = f_open(&file_object, (char const *)file_name, FA_READ);
	if (res != FR_OK) {
		printf("music file open[FAIL] res %d\r\n", res);
		return -1;
	}
	printf("file open [OK]\r\n");
	unsigned int temp = 0;
	
	//read the wave file header into the structure
	res = f_read(&file_object, (uint8_t *)&audio_header, \
					sizeof(audio_header), &temp);
	if (res != FR_OK || temp != sizeof(audio_header)) {
		printf("header read [FAIL] res %d\r\n", res);
		return -1;
	}
	printf("header read[OK]\r\n");
	
	printf("size %d\n\r", (int)audio_header.Subchunk2Size);
	printf("Sample rate %d\n\r", (int)audio_header.SampleRate);
	printf("bits per sample %d\n\r", (int)audio_header.BitsPerSample);
	
	//ignore last blocks(reads only in multiples of AUDIO_BUF_SIZE*2)
	//and calculate the number of blocks to be read
	nb_audio_blocks = audio_header.Subchunk2Size - \
						(audio_header.Subchunk2Size % (AUDIO_BUF_SIZE*2));
	nb_audio_blocks = nb_audio_blocks/(AUDIO_BUF_SIZE*2);
	
	
	/* Read audio data from the input wav file in temp_audio_buf */
	res = f_read(&file_object, (uint8_t *)temp_audio_buf, \
					AUDIO_BUF_SIZE*2, &temp);
	if (res != FR_OK) {
		printf("[FAIL] Reading first block!\r\n");
		return -1;
	}
	
	/* The input file is in 16 bit signed PCM format. But the DAC
	 * does not support signed values, So shift the input data
	 * by 32767 bits to form signed value. Store the resulting value 
	 * to buffer 1 (i.e. audio_data_0).
	 */
	
	uint16_t j = 0;
	for (int i = 0; i < AUDIO_BUF_SIZE; i = i+2, j++) {
		int32_t temp2 = (int32_t)temp_audio_buf[i] + 32767;
		temp = (uint16_t)(temp2 & 0xFFFF);
		*(audio_data_ptr+j) = DATA_BITS > DAC_BITS ?	
								(temp >> (DATA_BITS - DAC_BITS)) :
								(temp);
	}
	return 0;
}

//TODO - make the interrupt heavy, to prevent blocking of audio playback
void play()
{
	printf("playing\n\r");
	audio_ready = 1;
	t = 0;
	char data_arr = 0;
	FRESULT res;

	//Set the timer to generate interrupt the frequency of sampling rate of the file
	SysTick_Config(sysclk_get_cpu_hz() / (audio_header.SampleRate));
	dacc_enable_channel(DACC, 0);
	//Enable interrupts of the timer 
	NVIC_EnableIRQ(SysTick_IRQn);
		
	//printf("peripheral frequncy %d", sysclk_get_peripheral_bus_hz(DACC));
	//printf("frequncy set %d\n\r", sysclk_get_cpu_hz() / (audio_header.SampleRate));
	
	//printf("blocks %d\r\n", nb_audio_blocks);
	
	//printf("entering loop\n\r");
	
	//update the pointer used to fetch data for DAC
	if(data_arr == 0)
	{
		audio_data_ptr = &audio_data_0[0];
		read_data_ptr = &audio_data_1[0];
		//printf("change from 0 to 1\n\r");
		data_arr = 1;
	}
	else
	{
		audio_data_ptr = &audio_data_1[0];
		read_data_ptr = &audio_data_0[0];
		//printf("change from 1 to 0\n\r");
		data_arr = 0;
	}
	
	//play each block and update the data in buffer
	for (uint16_t block_cnt = 0; block_cnt < nb_audio_blocks-1; block_cnt++)	
	{
		unsigned int j = 0, temp = 0;
		
		/* Read audio data from the input wav file in temp_audio_buf */
		res = f_read(&file_object, (uint8_t *)temp_audio_buf, \
						AUDIO_BUF_SIZE*2, &temp);
		if (res != FR_OK) {
			printf("[FAIL] Reading first block!\r\n");
			return;
		}
		
		//process data for DAC
		for (int i = 0; i < AUDIO_BUF_SIZE; i = i+2, j++) 
		{
			int32_t temp2 = (int32_t)temp_audio_buf[i] + 32767;
			temp = (uint16_t)(temp2 & 0xFFFF);
			*(read_data_ptr+j) = DATA_BITS > DAC_BITS ?
									(temp >> (DATA_BITS - DAC_BITS)) :
									(temp);
		}
		
		//wait for playback to get over
		while(t < AUDIO_BUF_SIZE/2);
		
		//update the pointer used to fetch data for DAC
		if(data_arr == 0)
		{
			audio_data_ptr = &audio_data_0[0];
			read_data_ptr = &audio_data_1[0];
			//printf("change from 0 to 1\n\r");
			data_arr = 1;
		}
		else
		{
			audio_data_ptr = &audio_data_1[0];
			read_data_ptr = &audio_data_0[0];
			//printf("change from 1 to 0\n\r");
			data_arr = 0;
		}
		t = 0;
		//dacc_enable_channel(DACC, 0);
		//NVIC_EnableIRQ(SysTick_IRQn);
	}
	
	f_close(&file_object);
	printf("end of play\n\r");
	end_buf = 1;
}
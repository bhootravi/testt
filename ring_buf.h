#ifndef RING_BUF_H_
#define RING_BUF_H_

#include <inttypes.h>

typedef struct
{
	volatile int8_t* buffer;
	volatile int8_t* buffer_end;
	volatile int8_t* data_start;
	volatile int8_t* data_end;
	int32_t count;
	int32_t size;
} ring_buffer;

void RB_init(volatile ring_buffer* rb, volatile int8_t* buf, int32_t size);
int8_t RB_push(volatile ring_buffer* rb, int8_t data);
int8_t RB_pop(volatile ring_buffer* rb);
int8_t RB_full(volatile ring_buffer* rb);

#endif /* RING_BUF_H_ */
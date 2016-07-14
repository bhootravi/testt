#include "ring_buf.h"
#include <stdio.h>


void RB_init(volatile ring_buffer* rb, volatile int8_t* buf, int32_t size)
{
	rb->buffer = buf;
	rb->buffer_end = rb->buffer + size;
	rb->size = size;
	rb->data_start = rb->buffer;
	rb->data_end = rb->buffer;
	rb->count = 0;
}

int8_t RB_push(volatile ring_buffer* rb, int8_t data)
{
	if (rb == NULL || rb->buffer == NULL)
		return -1;

	*rb->data_end = data;
	rb->data_end++;
	if (rb->data_end == rb->buffer_end)
		rb->data_end = rb->buffer;

	if (RB_full(rb)) 
	{
		if ((rb->data_start + 1) == rb->buffer_end)
			rb->data_start = rb->buffer;
		else
			rb->data_start++;
	} else 
	{
		rb->count++;
	}

	return 0;
}

int8_t RB_pop(volatile ring_buffer* rb)
{
	if (rb == NULL || rb->buffer == NULL)
		return -1;

	int8_t data = *rb->data_start;
	rb->data_start++;
	if (rb->data_start == rb->buffer_end)
		rb->data_start = rb->buffer;
	rb->count--;

	return data;
}

int8_t RB_full(volatile ring_buffer* rb)
{
	return rb->count == rb->size;
}
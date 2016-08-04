#ifndef RING_BUFFER_H__
#define RING_BUFFER_H__

#define BUFF_FAIL_OVERFLOW   2
#define BUFF_FAIL_UNDERFLOW  3
#define BUFF_FAIL_FULL 4

#define PASS 0
#define FAIL 1

#include <stdint.h>

typedef struct {
    uint8_t *head;
    uint8_t *tail;
    uint8_t *start;
    uint8_t *end;
    uint8_t buff_size;
    uint8_t error;
    uint8_t unread_items;
} ring_buffer_t;

#include <stdint.h>
#include "ring_buffer.h"


void ring_buff_init(ring_buffer_t *rb_ptr, uint8_t *buffer_array_ptr, uint8_t buffer_len);
uint8_t ring_buff_pop(ring_buffer_t *rb_ptr, uint8_t *return_item_ptr);
uint8_t ring_buff_push(ring_buffer_t *rb_ptr, uint8_t new_val);
uint8_t ring_buff_has_data(ring_buffer_t *rb_ptr);
void ring_buff_reset(ring_buffer_t *rb_ptr);

#endif /* RING_BUFFER_H__ */

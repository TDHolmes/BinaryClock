#include <stdint.h>
#include "ring_buffer.h"


void ring_buff_init(ring_buffer_t rb, uint8_t *buffer_array_ptr, uint8_t buffer_len)
{
    rb.head = buffer_array_ptr;
    rb.tail = buffer_array_ptr;
    rb.start = buffer_array_ptr;
    rb.end = buffer_array_ptr + buffer_len;
    rb.unread_items = 0;
    rb.error = 0;
    rb.buff_size = buffer_len;
}


uint8_t ring_buff_pop(ring_buffer_t *rb_ptr, uint8_t *return_item_ptr)
{
    if (rb_ptr->unread_items == 0) {
        return BUFF_FAIL_UNDERFLOW;
    } else if(rb_ptr->unread_items == rb_ptr->buff_size){
        return BUFF_FAIL_OVERFLOW;
    } else {
        // no errors, pop item at the tail of the buffer
        *return_item_ptr = *(rb_ptr->tail);
        rb_ptr->unread_items -= 1;
        // increment the tail pointer
        if(rb_ptr->tail == rb_ptr->end) {
            rb_ptr->tail = rb_ptr->start;
        } else {
            rb_ptr->tail += 1;
        }
    }
    return PASS;
}


uint8_t ring_buff_push(ring_buffer_t *rb_ptr, uint8_t new_val)
{
    if(rb_ptr->unread_items == rb_ptr->buff_size) {
        return BUFF_FAIL_FULL;
    } else {
        // no errors, push item to the head of the buffer
        *(rb_ptr->head) = new_val;
        rb_ptr->unread_items += 1;
        // increment the head pointer
        if(rb_ptr->head == rb_ptr->end) {
            rb_ptr->head = rb_ptr->start;
        } else {
            rb_ptr->head += 1;
        }
    }
}


uint8_t ring_buff_has_data(ring_buffer_t *rb_ptr)
{
    return rb_ptr->unread_items;
}


void ring_buff_reset(ring_buffer_t *rb_ptr)
{
    rb_ptr->head = rb_ptr->start;
    rb_ptr->tail = rb_ptr->start;
    rb_ptr->unread_items = 0;
}
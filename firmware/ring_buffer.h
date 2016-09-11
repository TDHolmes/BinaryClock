#ifndef RING_BUFFER_H__
#define RING_BUFFER_H__

#include <stdint.h>
#include "global_defines.h"

/*! \file ring_buffer.h
    \brief Data structures and definitions for the ring buffer.
*/


//! data structure to keep track of important ring buffer variables
typedef struct {
    uint8_t *head;  //!< pointer to the head of the buffer
    uint8_t *tail;  //!< pointer to the tail of the buffer
    uint8_t *start; //!< pointer to the start address of the buffer space
    uint8_t *end;   //!< pointer to the end address of the buffer space
    uint8_t buff_size;    //!< size of the buffer space
    uint8_t error;        //!< error flag (not currently used)
    uint8_t unread_items; //!< number of unread items in the ring buffer
} ring_buffer_t;


void     ring_buff_init(ring_buffer_t *rb_ptr, uint8_t *buffer_array_ptr, uint8_t buffer_len);
retval_t ring_buff_pop(ring_buffer_t *rb_ptr, uint8_t *return_item_ptr);
retval_t ring_buff_push(ring_buffer_t *rb_ptr, uint8_t new_val);
bool_t   ring_buff_has_data(ring_buffer_t *rb_ptr);
void     ring_buff_reset(ring_buffer_t *rb_ptr);
uint8_t  ring_buff_unread_items(ring_buffer_t *rb_ptr);

#endif /* RING_BUFFER_H__ */

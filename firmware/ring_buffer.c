#include "ring_buffer.h"
#include "global_defines.h"
#include <stdint.h>


/*!
 * Initializes the ring buffer structure.
 * 
 * @param[in] rb_ptr (ring_buffer_t *): ring buffer admin structure to initialize.
 * @param[in] buffer_array_ptr (uint8_t *): pointer to the buffer array.
 * @param[in] buffer_len (uint8_t): length of the buffer to be initialized.
 */
void ring_buff_init(ring_buffer_t *rb_ptr, uint8_t *buffer_array_ptr, uint8_t buffer_len)
{
    rb_ptr->start = buffer_array_ptr;
    rb_ptr->end = buffer_array_ptr + buffer_len - 1;
    rb_ptr->head = rb_ptr->start;
    rb_ptr->tail = rb_ptr->start;
    rb_ptr->unread_items = 0;
    rb_ptr->error = 0;
    rb_ptr->buff_size = buffer_len;
}


/*!
 * Pops a value off of the ring buffer.
 * 
 * @param[in] rb_ptr (ring_buffer_t *): admin structure for the ring buffer we're using.
 * @param[in] return_item_ptr (uint8_t *): Location to return the value to
 * @param[out] retval (retval_t): Returns the error or pass status.
 */
retval_t ring_buff_pop(ring_buffer_t *rb_ptr, uint8_t *return_item_ptr)
{
    if (rb_ptr->unread_items == 0) {
        return BUFF_FAIL_UNDERFLOW;
    } else if(rb_ptr->unread_items > rb_ptr->buff_size) {
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
    return GEN_PASS;
}


/*!
 * Pushes a value onto the ring buffer.
 * 
 * @param[in] rb_ptr (ring_buffer_t *): admin structure for the ring buffer we're using.
 * @param[in] new_val (uint8_t): value to push onto the buffer.
 * @param[out] retval (retval_t): Returns the error or pass status.
 */
retval_t ring_buff_push(ring_buffer_t *rb_ptr, uint8_t new_val)
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
    return GEN_PASS;
}


/*!
 * Gets whether or not there is valid data in the ring buffer.
 * 
 * @param[in] rb_ptr (ring_buffer_t *): admin structure for the ring buffer we're using.
 * @param[out] retval (bool_t): Returns whether or not there's data on the buffer.
 */
bool_t ring_buff_has_data(ring_buffer_t *rb_ptr)
{
    if (rb_ptr->unread_items > 0) {
        return TRUE;
    } else {
        return FALSE;
    }
}


/*!
 * Gets how much unread data is on the buffer.
 * 
 * @param[in] rb_ptr (ring_buffer_t *): admin structure for the ring buffer we're using.
 * @param[out] unread_items (uint8_t): Returns the amount of unread data is on the buffer.
 */
uint8_t ring_buff_unread_items(ring_buffer_t *rb_ptr)
{
    return rb_ptr->unread_items;
}


/*!
 * Resets the ring buffer.
 * 
 * @param[in] rb_ptr (ring_buffer_t *): admin structure for the ring buffer we're using.
 */
void ring_buff_reset(ring_buffer_t *rb_ptr)
{
    rb_ptr->head = rb_ptr->start;
    rb_ptr->tail = rb_ptr->start;
    rb_ptr->unread_items = 0;
}

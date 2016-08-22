/* Name: main.c
 * Author: <insert your name here>
 * Copyright: <insert your copyright message here>
 * License: <insert your license reference here>
 */

#include <stdio.h>
#include <stdint.h>
#include "ring_buffer.h"

#define RING_BUFF_LEN 10

uint8_t ring_buffer_arrray[10];

ring_buffer_t rb;
ring_buffer_t *rb_ptr;

int main(void)
{
    uint8_t retval = 0;
    uint8_t offset = 0;
    uint8_t val = 0;
    uint8_t i = 0;
    uint8_t j = 0;
    rb_ptr = &rb;
    ring_buff_init(rb_ptr, ring_buffer_arrray, RING_BUFF_LEN);
    for (j = 0; j != 255; j++) {
        ring_buff_push(rb_ptr, j);
        retval = ring_buff_pop(rb_ptr, &val);
        printf("retval: %d  val: %d\n", retval, val);
        offset += 1;
    }
}

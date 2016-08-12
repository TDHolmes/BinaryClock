#ifdef TIMER_DRVR_4313_H__
#define TIMER_DRVR_4313_H__

typedef struct {
    uint8_t millis_initialized;
    uint8_t counter_initialized;
    uint8_t *counter_flag_ptr;
    uint32_t millis_count;
} timer_drvr_status_t;

#endif /* TIMER_DRVR_4313_H__ */

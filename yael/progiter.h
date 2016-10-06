#ifndef PROGITER_H_INCLUDED
#define PROGITER_H_INCLUDED
#include <stdlib.h>

typedef struct {
    int nTotal;
    char* lbl;

    // Timing info
    double flush_freq_sec;

    double start_time;
    double current_time;
    double prev_flush_time;

    double total_sec;
    double sec_since_last;
    double iter_per_sec;
    double est_sec_remain;
    // Custom measures
    size_t nbytes_have;
    char* extra_buffer;

    void (*begin)();
    void (*end)();
    /*void (*marki)(void* self, int i);*/
    /*void (*markf)(int i, const char* fmt, ...);*/
} ProgIter;



void ProgIter_marki(ProgIter *self, int i);
void ProgIter_markf(ProgIter *self, int i, const char * format, ...);
void ProgIter_init(ProgIter *self, int nTotal_, char* lbl_);
void ProgIter_begin(ProgIter *self);
void ProgIter_end(ProgIter *self);
void ProgIter_delete(ProgIter *self);
#endif

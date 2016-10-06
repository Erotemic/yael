#include "progiter.h"
#include <stdlib.h>
#include <stdio.h>
#include <stdarg.h>
#include <string.h>

#ifdef _WIN32
#else
#include <time.h>
#include <sys/time.h>
#endif


//  Windows
#ifdef _WIN32
#else
//  Posix/Linux
double ProgIter_now_wall()
{
    struct timeval time;
    if (gettimeofday(&time, NULL))
    {
        //  Handle error
        return 0;
    }
    return (double) time.tv_sec + (double) time.tv_usec * .000001;
}
#endif

void ProgIter_update_measures_wall(ProgIter* self, int i)
{
    self->current_time = ProgIter_now_wall(self);
    self->sec_since_last = (self->current_time - self->prev_flush_time);
    self->total_sec = (self->current_time - self->start_time );
    self->iter_per_sec = (double) (i) / self->total_sec;
    self->est_sec_remain = (double) (self->nTotal - i) / self->iter_per_sec;
}

void ProgIter_flush_if_requested(ProgIter* self)
{
    if (self->sec_since_last > self->flush_freq_sec)
    {
        fflush(stdout);
        self->prev_flush_time = ProgIter_now_wall(self);
    }
}

void ProgIter_clear_buffer(ProgIter* self)
{
    if (self->nbytes_have > 0)
    {
        free(self->extra_buffer);
        self->nbytes_have = 0;
    }
}

void ProgIter_ensure_buffer(ProgIter* self, size_t nbytes_need){
    if (nbytes_need > self->nbytes_have)
    {
        if (self->nbytes_have > 0)
        {
            free(self->extra_buffer);
        }
        self->nbytes_have = nbytes_need;
        self->extra_buffer = (char*) malloc(self->nbytes_have);
    }
}


void ProgIter_delete(ProgIter *self)
{
    ProgIter_clear_buffer(self);
}

void ProgIter_begin(ProgIter *self)
{
    self->start_time = ProgIter_now_wall(self);
    self->prev_flush_time = self->start_time;
    fprintf(stdout,"\33[2K\r%s begin...", self->lbl);
    fflush(stdout);
}

void ProgIter_marki(ProgIter *self, int i)
{
    // function to mark standard progress
    ProgIter_update_measures_wall(self, i);
    fprintf(stdout,"%c[2K", 27);
    /*fprintf(stdout,"\33[2K");*/
    fprintf(stdout,"\r%s %d/%d rate=%.2fHz, etr=%ds, total=%.2fs",
            self->lbl, i, self->nTotal, self->iter_per_sec,
            (int) self->est_sec_remain, self->total_sec);
    ProgIter_flush_if_requested(self);
}

void ProgIter_markf(ProgIter *self, int i, const char * format, ...){
    // function to mark customized progress using printf-like syntax
    // @param i: current iteration number
    // @param extra: customized message
    va_list args;
    va_start(args, format);
    // Check to make sure we have a big enough extra_buffer
    size_t nbytes_need = vsnprintf(NULL, 0, format, args) + 1;
    ProgIter_ensure_buffer(self, nbytes_need);
    vsnprintf(self->extra_buffer, self->nbytes_have, format, args);
    va_end(args);
    ProgIter_update_measures_wall(self, i);
    fprintf(stdout,"%c[2K", 27);
    //const char* CLEARLINE_EL2 = "\33[2K";
    fprintf(stdout,"\r%s %d/%d rate=%.2fHz, etr=%ds, total=%.2fs %s",
            self->lbl, i, self->nTotal, self->iter_per_sec,
            (int) self->est_sec_remain, self->total_sec, self->extra_buffer);
    ProgIter_flush_if_requested(self);
}

void ProgIter_end(ProgIter *self)
{
    ProgIter_marki(self, self->nTotal);
    fprintf(stdout,"\n");
    fflush(stdout);
}


void ProgIter_init(ProgIter *self, int nTotal_, char* lbl_)
{
    self->nTotal = nTotal_;
    self->lbl = lbl_;
    self->flush_freq_sec = 1.0;
    self->nbytes_have = 0;
    // try and bind functions to struct,
    // some seem to cause errors
    self->begin = ProgIter_begin;
    self->end = ProgIter_end;
    /*self->marki = ProgIter_marki;*/
    /*self->markf = ProgIter_markf;*/
}

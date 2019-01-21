#ifndef _AE_PERIODIC_H
#define _AE_PERIODIC_H

#include <ae/timer.h>
#include <ae/mux.h>

typedef struct periodic
{
    struct timespec ts;
    ae_timer_t timer;
    ae_mux_event_t mux_event;
    ae_mux_handlers_t handlers;
} ae_periodic_t;

#ifdef __cplusplus
extern "C" {
#endif

    void ae_periodic_uninit(ae_res_t *e, ae_periodic_t *self);
    bool ae_periodic_init(ae_res_t *e, ae_periodic_t *self);

    bool ae_periodic_cfg(ae_res_t *e, ae_periodic_t *self,
                      double rate, ae_mux_cb_t cb, void *ctx);
    bool ae_periodic_recfg(ae_res_t *e, ae_periodic_t *self,
                      double rate, ae_mux_cb_t cb, void *ctx);

    bool ae_periodic_register(ae_res_t *e, ae_periodic_t *self,
                           ae_mux_t *mux, bool trigger_now);

    bool ae_periodic_clear(ae_res_t *e, ae_periodic_t *self,
                        uint64_t *out_overflow);

#ifdef __cplusplus
}
#endif


#endif

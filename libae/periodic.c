#include <ae/periodic.h>

#include <math.h>

void ae_periodic_double_to_timespec(double to, struct timespec *out)
{
    out->tv_sec = floor(to);
    out->tv_nsec = (to - out->tv_sec) * 1E9;
    /* AE_LT("%d %d\n", out->tv_sec, out->tv_nsec); */
}

void ae_periodic_uninit(ae_res_t *e, ae_periodic_t *self)
{
     ae_timer_uninit(e, &self->timer);
}


bool ae_periodic_init(ae_res_t *e, ae_periodic_t *self)
{
     AE_TRY(ae_timer_init(e, &self->timer, CLOCK_MONOTONIC));
     return true;
}


bool ae_periodic_cfg(ae_res_t *e, ae_periodic_t *self,
                  double rate, ae_mux_cb_t cb, void *ctx)
{
     if(rate < 0.0)
     {
          ae_res_err(e, "invalid rate: %g", rate);
          return false;
     }
     self->mux_event.ctx = ctx;
     self->mux_event.handlers = &self->handlers;
     self->handlers.read = cb;
     self->handlers.write = cb;
     self->handlers.hangup = cb;
     self->handlers.priority = cb;
     self->handlers.error = cb;
     ae_periodic_double_to_timespec(rate, &self->ts);
     return true;
}

bool ae_periodic_recfg(ae_res_t *e, ae_periodic_t *self,
                  double rate, ae_mux_cb_t cb, void *ctx)
{
     if(rate < 0.0)
     {
          ae_res_err(e, "invalid rate: %g", rate);
          return false;
     }
     ae_timer_stop(e, &self->timer);
     self->mux_event.ctx = ctx;
     self->handlers.read = cb;
     self->handlers.write = cb;
     self->handlers.hangup = cb;
     self->handlers.priority = cb;
     self->handlers.error = cb;
     ae_periodic_double_to_timespec(rate, &self->ts);
     AE_TRY(ae_timer_every(e, &self->timer, &self->ts, false));
     return true;
}

bool ae_periodic_register(ae_res_t *e, ae_periodic_t *self,
                       ae_mux_t *mux, bool trigger_now)
{
     self->mux_event.fd = self->timer.fd;
     AE_TRY(ae_mux_add(e, mux, &self->mux_event));
     AE_TRY(ae_timer_every(e, &self->timer, &self->ts, trigger_now));
     return true;
}


bool ae_periodic_clear(ae_res_t *e, ae_periodic_t *self,
                    uint64_t *out_overflow)
{
     AE_TRY(ae_timer_read(e, &self->timer, out_overflow));
     return true;
}



#include <iostream>
#include <uv.h>
#include "Throttler.h"
#include "Monitor.h"
#include "workers/Workers.h"

uint64_t Throttler::interval;
uv_timer_t Throttler::m_timer;

void Throttler::init(const uint64_t theInterval)
{
    interval = theInterval;
    uv_timer_init(uv_default_loop(), &m_timer);
    uv_timer_start(&m_timer, Throttler::onCheck, INITIAL_WAIT_MS, interval);
}

void Throttler::onCheck(uv_timer_t *handle)
{
    if (Workers::isEnabled() && Monitor::isTooBusy()) {
        // std::cout << "Cooling down workers..." << std::endl;
        Workers::setEnabled(false);
    } else if (!Workers::isEnabled() && !Monitor::isTooBusy()) {
        // std::cout << "Auto-resuming workers..." << std::endl;
        Workers::setEnabled(true);
    }
}
#ifndef __THROTTLER_H__
#define __THROTTLER_H__

#endif
#include <uv.h>

class Throttler
{
public:
    static const uint64_t INITIAL_WAIT_MS = 50;
    
    static void init(const uint64_t theInterval);
private:
    static uint64_t interval;
    static uv_timer_t m_timer;

    static void onCheck(uv_timer_t *handle);
};


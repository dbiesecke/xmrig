#ifndef __MONITOR_H__
#define __MONITOR_H__

#endif
#include <time.h>
#include <uv.h>

class Monitor
{
public:
    static const int BUF_SIZE = 100;
    static const int INITIAL_WAIT_MS = 100;
    static const int INTERVAL_MS = 50;

    static void init(const uint64_t theThreshold);
    static bool isTooBusy();
    static uint64_t getCurrentPercentage();
    
private:
    static uint64_t threshold;
    static int index;
    static uv_timer_t m_timer;
    static time_t timestamps[BUF_SIZE];
    static uint64_t percentages[BUF_SIZE];
    static struct rusage prevUsage;
    
    static void onAddRecord(uv_timer_t *handle);
    static void addRecord();        
};

#include "Monitor.h"
#include <iostream>
#include <sys/resource.h>
#include <sys/time.h>

uint64_t Monitor::threshold;
int Monitor::index;
uv_timer_t Monitor::m_timer;
time_t Monitor::timestamps[BUF_SIZE];
uint64_t Monitor::percentages[BUF_SIZE];
struct rusage Monitor::prevUsage;

void Monitor::init(const uint64_t theThreshold)
{
    index = 0;
    threshold = theThreshold;

    for (int i = 0; i < BUF_SIZE; i++) {
        timestamps[i] = 0;
        percentages[i] = 0;
    }
    uv_timer_init(uv_default_loop(), &m_timer);
    uv_timer_start(&m_timer, Monitor::onAddRecord, INITIAL_WAIT_S * 1000, INTERVAL_S * 1000);
}

uint64_t Monitor::getCurrentPercentage()
{
    uint64_t percentage = 0;
    struct rusage usage;
    getrusage(RUSAGE_SELF, &usage);
    if (prevUsage.ru_utime.tv_sec != 0) {
        time_t deltaSecond = (usage.ru_utime.tv_sec - prevUsage.ru_utime.tv_sec) + (usage.ru_stime.tv_sec - prevUsage.ru_stime.tv_sec);
        suseconds_t deltaMicroSecond = (usage.ru_utime.tv_usec - prevUsage.ru_utime.tv_usec) + (usage.ru_stime.tv_usec - prevUsage.ru_stime.tv_usec);
        percentage = (deltaSecond * 1000000 + deltaMicroSecond) / (INTERVAL_S * 10000);
    }
    prevUsage = usage;
    return percentage;
}

void Monitor::onAddRecord(uv_timer_t *handle)
{
    addRecord();
}

void Monitor::addRecord()
{
    uint64_t percentage = Monitor::getCurrentPercentage();
    time_t timestamp = time(NULL);
    timestamps[index] = timestamp;
    percentages[index] = percentage;
    index += 1;
    // std::cout << "addRecord timestamp=" << timestamp << "; percentage=" << percentage << std::endl;
}

bool Monitor::isTooBusy()
{
    time_t now = time(NULL);
    time_t cutoff = now - WINDOW_SIZE_S;
    uint64_t total = 0;
    uint64_t count = 0;
    uint64_t avg = 0;
    
    for (int i = 0; i < BUF_SIZE; i++) {
        time_t timestamp = timestamps[i];
        uint64_t percentage = percentages[i];
        if (timestamp >= cutoff) {
            total += percentage;
            count += 1;
        }
    }

    if (count > 0) {
        avg = total / count;
    }

    bool busy = avg > threshold;
    
    // std::cout << "isTooBusy=" << busy << std::endl;
    return busy;
}
#include "Profiler.h"
#include <chrono>

using namespace std::chrono;

high_resolution_clock::time_point start;
high_resolution_clock::time_point end;

void mark_start()
{
    start = high_resolution_clock::now();
}

void mark_end()
{
    end = high_resolution_clock::now();
}

long elapsed_milli()
{
    return (long)(duration_cast<milliseconds>(end - start).count());
}

long elapsed_micro()
{
    return (long)(duration_cast<microseconds>(end - start).count());
}

long elapsed_nano()
{
    return (long)(duration_cast<nanoseconds>(end - start).count());
}
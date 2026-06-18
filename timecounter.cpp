#include "timecounter.h"

timecounter::timecounter()
{}

void
timecounter::tic(void)
{
    start_time = std::chrono::high_resolution_clock::now();
    running = true;
}

double
timecounter::toc(void)
{
    if (!running) {
        throw std::runtime_error("timecounter not running");
    }

    auto end_time = std::chrono::high_resolution_clock::now();
    std::chrono::duration<double> elapsed = end_time - start_time;
    running = false;
    return elapsed.count();
}
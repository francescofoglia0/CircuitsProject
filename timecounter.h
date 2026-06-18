#pragma once

#include <iostream>
#include <chrono>

class timecounter {

    std::chrono::high_resolution_clock::time_point start_time;
    bool running = false;

public:
            timecounter();
    void    tic(void);
    double  toc(void);
};

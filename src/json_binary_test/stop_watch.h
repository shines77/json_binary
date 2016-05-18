#pragma once

#include <chrono>

using namespace std::chrono;

class StopWatch {
public:
    typedef std::chrono::time_point<high_resolution_clock> time_clock;
    typedef std::chrono::duration<double> time_elapsed;

private:
    time_clock start_time_;
    time_clock stop_time_;
    time_elapsed interval_time_;

public:
    StopWatch() {};
    ~StopWatch() {};

    void start() {
        start_time_ = std::chrono::high_resolution_clock::now();
    }

    void stop() {
        stop_time_ = std::chrono::high_resolution_clock::now();
    }
    
    double getElapsedTime() {
        interval_time_ = std::chrono::duration_cast< time_elapsed >(stop_time_ - start_time_);
        double elapsed_time = interval_time_.count();
        return elapsed_time;
    }

    double getMillisec() {
        return getElapsedTime() * 1000.0;
    }

    double getSecond() {
        return getElapsedTime();
    }
};

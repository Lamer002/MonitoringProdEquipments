#include "machine.h"

#include <algorithm>

Machine::Machine(int id) : id_(id) {}

void Machine::setId(int id) { id_ = id; }
void Machine::setStatus(Status status) { status_ = status; }
void Machine::setTemperature(int temperature) { temperature_ = std::max(0, temperature); }
void Machine::setRPM(double rpm)
{
    freqRPM_ = std::clamp(rpm, 0.0, static_cast<double>(DEFAULT_MAX_RPM_FREQ));
}
void Machine::setEngineLoading(int percent)
{
    engine_loading = std::clamp(percent, 0, DEFAULT_MAX_ENGINE_LOADING);
}

int Machine::getId() const { return id_; }
Machine::Status Machine::getStatus() const { return status_; }
int Machine::getTemperature() const { return temperature_; }
double Machine::getRPM() const { return freqRPM_; }
int Machine::getEngineLoading() const { return engine_loading; }
int Machine::getOperHours() const { return count_oper_hours; }
int Machine::getCrashCount() const { return count_crash; }

void Machine::resetEngine()
{
    status_ = STATUS_STOPPED;
    temperature_ = 20;
    freqRPM_ = 0;
    engine_loading = 0;
    count_oper_hours = 0;
}

void Machine::resetCountCrash() { count_crash = 0; }
void Machine::incrementOperHourse() { ++count_oper_hours; }
void Machine::incrementCrashCount() { ++count_crash; }

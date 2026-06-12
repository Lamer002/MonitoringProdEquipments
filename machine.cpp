#include "machine.h"

Machine::Machine() {}

Machine::Machine(int id)
{
    id_ = id;
    status_ = Machine::STATUS_STOPPED;
    temperature_ = 0;
    freqRPM_ = 0;
    engine_loading = 0;
    count_oper_hours = 0;
    count_crash = 0;
}

//Сеттеры
void Machine::setId(int id)
{
    id_ = id;
}

void Machine::setStatus(Status s)
{
    status_ = s;
}

void Machine::setTemperature(int t)
{
    temperature_ = t;
}

void Machine::setRPM(double speed)
{
    freqRPM_ = speed;
}



//Геттеры
int Machine::getId() const
{
    return id_;
}

Machine::Status Machine::getStatus() const
{
    return status_;
}

int Machine::getTemperature() const
{
    return temperature_;
}

double Machine::getRPM() const
{
    return freqRPM_;
}

int Machine::getEngineLoading() const
{
    return engine_loading;
}

int Machine::getOperHours() const
{
    return count_oper_hours;
}

int Machine::getCrashCount() const
{
    return count_crash;
}


//Сброс настроек!!!!!!
void Machine::resetEngine()
{
    temperature_ = 0;
    freqRPM_ = 0;
    engine_loading = 0;
    count_oper_hours = 0;
}

void Machine::resetCountCrash()
{
    count_crash = 0;
}


//Добавление моточасов и поломок
void Machine::incrementOperHourse()
{
    count_oper_hours ++;
}

void Machine::incrementCrashCount()
{
    count_crash ++;
}
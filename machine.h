#ifndef MACHINE_H
#define MACHINE_H

#include <QString>
#include <QColor>

static const int DEFAULT_MAX_TEMPERATURE_WARNING = 120;
static const int DEFAULT_MAX_RPM_FREQ = 1500;
static const int DEFAULT_MAX_ENGINE_LOADING = 100;

class Machine
{
public:

    enum Status
    {
        STATUS_STOPPED = 0,
        STATUS_RUNNING = 1,
        STATUS_WARNING = 2,
        STATUS_EMERGENCY = 3,
        STATUS_MAINTENANCE = 4
    };

    Machine();
    Machine(int id, int status);
    Machine(int id, Status status = STATUS_STOPPED);
    Machine(int id);

    ~Machine() = default;

    void setId(int id);
    void setStatus(Status s);
    void setTemperature(int t);
    void setRPM(double speed);

    int getId() const;
    Status getStatus() const;
    int getTemperature() const;
    double getRPM() const;
    int getEngineLoading() const;
    int getOperHours() const;
    int getCrashCount() const;

    void resetEngine();
    void resetCountCrash();

    void incrementOperHourse();
    void incrementCrashCount();

private:
    int id_;
    Status status_;
    int temperature_;
    double freqRPM_;
    int engine_loading;
    int count_oper_hours;
    int count_crash;
};

#endif // MACHINE_H

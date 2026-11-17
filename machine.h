#ifndef MACHINE_H
#define MACHINE_H

constexpr int DEFAULT_MAX_TEMPERATURE_WARNING = 120;
constexpr int DEFAULT_MAX_RPM_FREQ = 1500;
constexpr int DEFAULT_MAX_ENGINE_LOADING = 100;
constexpr int DEFAULT_TEMPERATURE_WARNING = 100;

class Machine
{
public:
    enum Status {
        STATUS_STOPPED = 0,
        STATUS_RUNNING = 1,
        STATUS_WARNING = 2,
        STATUS_EMERGENCY = 3,
        STATUS_MAINTENANCE = 4
    };

    explicit Machine(int id = 0);

    void setId(int id);
    void setStatus(Status status);
    void setTemperature(int temperature);
    void setRPM(double rpm);
    void setEngineLoading(int percent);

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
    int id_ = 0;
    Status status_ = STATUS_STOPPED;
    int temperature_ = 20;
    double freqRPM_ = 0.0;
    int engine_loading = 0;
    int count_oper_hours = 0;
    int count_crash = 0;
};

#endif // MACHINE_H

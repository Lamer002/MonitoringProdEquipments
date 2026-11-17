#include "machine.h"

#include <iostream>

namespace {
int failures = 0;

void expect(bool condition, const char *message)
{
    if (!condition) {
        std::cerr << "FAIL: " << message << '\n';
        ++failures;
    }
}
} // namespace

int main()
{
    Machine machine(101);

    expect(machine.getId() == 101, "ID initializes");
    expect(machine.getStatus() == Machine::STATUS_STOPPED, "New machine is stopped");
    expect(machine.getTemperature() == 20, "Initial temperature is ambient");
    expect(machine.getRPM() == 0.0, "Initial RPM is zero");
    expect(machine.getEngineLoading() == 0, "Initial load is zero");
    expect(machine.getOperHours() == 0, "Initial operating hours are zero");
    expect(machine.getCrashCount() == 0, "Initial crash count is zero");

    machine.setRPM(-100.0);
    expect(machine.getRPM() == 0.0, "Negative RPM is clamped");
    machine.setRPM(3000.0);
    expect(machine.getRPM() == DEFAULT_MAX_RPM_FREQ, "Maximum RPM is clamped");

    machine.setEngineLoading(-20);
    expect(machine.getEngineLoading() == 0, "Minimum load is clamped");
    machine.setEngineLoading(200);
    expect(machine.getEngineLoading() == 100, "Maximum load is clamped");

    machine.setTemperature(-40);
    expect(machine.getTemperature() == 0, "Temperature cannot be negative");

    machine.setStatus(Machine::STATUS_EMERGENCY);
    machine.incrementCrashCount();
    machine.incrementOperHourse();
    expect(machine.getCrashCount() == 1, "Crash counter increments");
    expect(machine.getOperHours() == 1, "Hours counter increments");

    machine.resetEngine();
    expect(machine.getStatus() == Machine::STATUS_STOPPED, "Reset stops the machine");
    expect(machine.getTemperature() == 20, "Reset returns to ambient temperature");
    expect(machine.getRPM() == 0.0, "Reset clears RPM");
    expect(machine.getEngineLoading() == 0, "Reset clears load");
    expect(machine.getOperHours() == 0, "Reset clears hours");
    expect(machine.getCrashCount() == 1, "Crash history persists through engine reset");

    machine.resetCountCrash();
    expect(machine.getCrashCount() == 0, "Crash counter resets");

    if (failures != 0) {
        std::cerr << failures << " assertions failed\n";
        return 1;
    }
    std::cout << "All core model checks passed\n";
    return 0;
}

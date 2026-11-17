# Monitoring Production Equipment

A desktop application for **monitoring simulated industrial machines**, written in **C++17** with **Qt Widgets** and **SQLite**.

> Portfolio demonstration project: telemetry is simulated. There is no connection to real industrial hardware.

## Features

- Add/remove machines using unique positive IDs.
- Monitor status, temperature, RPM, and engine load.
- Inspect an individual machine in a detail window.
- Simulate changing telemetry once per second.
- Show high-temperature warnings and perform emergency shutdown on critical overheating.
- Store event logs locally in SQLite.
- View equipment status with color-coded labels.

## Technology

C++17 · Qt 5/6 (Widgets, SQL) · SQLite · CMake · CTest · GitHub Actions.

## Build and run

Install a C++17 compiler, CMake 3.16+, and Qt development libraries (Widgets, SQL, SQLite driver).

```bash
cmake -S . -B build -DCMAKE_BUILD_TYPE=Release
cmake --build build --parallel
```

Run the resulting `MonitoringProdEquipment` executable. On Windows with a multi-config generator, build using `cmake --build build --config Release`.

## Tests

```bash
cmake -S . -B build -DBUILD_TESTING=ON
cmake --build build --parallel
ctest --test-dir build --output-on-failure
```

The current tests verify core `Machine` behavior: initial state, value limits, counters, and reset. They do **not** yet cover the UI or database integration. GitHub Actions runs an automated Linux build and these tests on pushes and pull requests.

## Architecture

- **`MainWindow`**: equipment list and user interaction.
- **`EmulatorManager`**: owns simulated machines, runs timer-based updates, handles alarms and SQLite event logging.
- **`Machine`**: one simulated machine's status, telemetry, and counters.
- **Qt signals/slots**: propagate status updates and alarms to the interface.

SQLite event data is saved in the platform-specific application-data directory as `LogsMachine.db`.

## Limitations / roadmap

- Telemetry is simulated, not sourced from physical machines or PLCs.
- Temperature limits are currently compile-time constants.
- Add emulator integration tests, UI tests, and a screenshot/demo in a future iteration.

## Project status

Portfolio work in progress. The automated build is configured, but please check the GitHub Actions run status and test locally before relying on it as a verified successful build.

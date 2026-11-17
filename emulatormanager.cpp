#include "emulatormanager.h"

#include <QDir>
#include <QDebug>
#include <QRandomGenerator>
#include <QSqlError>
#include <QSqlQuery>
#include <QStandardPaths>

#include <algorithm>

EmulatorManager::EmulatorManager(QObject *parent)
    : QObject(parent)
    , timer_(new QTimer(this))
{
    connect(timer_, &QTimer::timeout, this, &EmulatorManager::onTimeout);

    const QString dataDirectory =
        QStandardPaths::writableLocation(QStandardPaths::AppDataLocation);
    if (!QDir().mkpath(dataDirectory)) {
        qWarning() << "Unable to create application data directory:" << dataDirectory;
    }

    db_ = QSqlDatabase::addDatabase("QSQLITE");
    db_.setDatabaseName(QDir(dataDirectory).filePath("LogsMachine.db"));

    if (!db_.open()) {
        qWarning() << "Could not open the event database:" << db_.lastError().text();
        return;
    }

    QSqlQuery query(db_);
    if (!query.exec("CREATE TABLE IF NOT EXISTS Logs ("
                    "id INTEGER PRIMARY KEY AUTOINCREMENT,"
                    "status TEXT NOT NULL,"
                    "machineId INTEGER NOT NULL)")) {
        qWarning() << "Could not create event table:" << query.lastError().text();
    }
}

EmulatorManager::~EmulatorManager()
{
    stopEmulator();
    qDeleteAll(machines_);
    machines_.clear();
    db_.close();
}

bool EmulatorManager::addMachine(int id)
{
    if (id <= 0 || getMachineById(id)) {
        return false;
    }

    machines_.append(new Machine(id));
    emit dataUpdate();
    return true;
}

bool EmulatorManager::deleteMachine(int id)
{
    for (int index = 0; index < machines_.size(); ++index) {
        if (machines_[index]->getId() == id) {
            delete machines_.takeAt(index);
            emit dataUpdate();
            return true;
        }
    }
    return false;
}

void EmulatorManager::startEmulator()
{
    if (isRunning_) return;

    isRunning_ = true;
    timer_->start(1000);
}

void EmulatorManager::stopEmulator()
{
    if (!isRunning_) return;

    timer_->stop();
    isRunning_ = false;
}

void EmulatorManager::startStopMachine(int id, bool on)
{
    Machine *machine = getMachineById(id);
    if (!machine) return;

    const auto status = machine->getStatus();
    if (on && (status == Machine::STATUS_STOPPED ||
               (status == Machine::STATUS_EMERGENCY &&
                machine->getTemperature() < DEFAULT_TEMPERATURE_WARNING))) {
        machine->setStatus(Machine::STATUS_RUNNING);
        logToDatabase(QString("Станок #%1 запущен").arg(id), id);
    } else if (!on && (status == Machine::STATUS_RUNNING ||
                       status == Machine::STATUS_WARNING)) {
        machine->setStatus(Machine::STATUS_STOPPED);
        machine->setEngineLoading(0);
        logToDatabase(QString("Станок #%1 остановлен").arg(id), id);
    } else {
        return;
    }

    emit dataUpdate();
}

Machine *EmulatorManager::getMachineById(int id) const
{
    for (Machine *machine : machines_) {
        if (machine->getId() == id) return machine;
    }
    return nullptr;
}

int EmulatorManager::machineCount() const
{
    return machines_.size();
}

void EmulatorManager::onTimeout()
{
    if (!isRunning_) return;

    ++countSecond;
    auto *random = QRandomGenerator::global();

    for (Machine *machine : machines_) {
        const auto status = machine->getStatus();
        const int temperature = machine->getTemperature();
        const double rpm = machine->getRPM();

        if (status == Machine::STATUS_RUNNING ||
            status == Machine::STATUS_WARNING) {
            const int nextTemperature = temperature + random->bounded(3);
            machine->setTemperature(nextTemperature);

            // A small upward bias makes the demo reach operating speed naturally.
            const double nextRpm = rpm + random->bounded(201) - 70;
            machine->setRPM(nextRpm);
            machine->setEngineLoading(static_cast<int>(
                machine->getRPM() * DEFAULT_MAX_ENGINE_LOADING /
                DEFAULT_MAX_RPM_FREQ));

            if (countSecond % 3600 == 0) {
                machine->incrementOperHourse();
            }

            // Check the actual sensor reading BEFORE deciding what to display.
            if (nextTemperature > DEFAULT_MAX_TEMPERATURE_WARNING) {
                machine->setStatus(Machine::STATUS_EMERGENCY);
                machine->incrementCrashCount();
                machine->setRPM(0);
                machine->setEngineLoading(0);
                logToDatabase(
                    QString("Аварийный перегрев станка #%1: %2 °C")
                        .arg(machine->getId()).arg(nextTemperature),
                    machine->getId());
                emit crashTrigger(
                    machine->getId(),
                    QString("Перегрев: %1 °C. Станок остановлен.")
                        .arg(nextTemperature));
            } else if (nextTemperature >= DEFAULT_TEMPERATURE_WARNING) {
                if (status != Machine::STATUS_WARNING) {
                    machine->setStatus(Machine::STATUS_WARNING);
                    logToDatabase(
                        QString("Предупреждение: станок #%1 нагрелся до %2 °C")
                            .arg(machine->getId()).arg(nextTemperature),
                        machine->getId());
                }
            } else if (status == Machine::STATUS_WARNING) {
                machine->setStatus(Machine::STATUS_RUNNING);
            }
        } else if (status == Machine::STATUS_STOPPED ||
                   status == Machine::STATUS_EMERGENCY) {
            machine->setTemperature(std::max(
                20, temperature - random->bounded(3)));
            machine->setRPM(std::max(
                0.0, rpm - (20 + random->bounded(41))));
            machine->setEngineLoading(0);
        }
    }

    emit dataUpdate();
}

void EmulatorManager::logToDatabase(const QString &status, int machineId)
{
    if (!db_.isOpen()) {
        qWarning() << "Database unavailable; event not saved:" << status;
        return;
    }

    QSqlQuery query(db_);
    query.prepare("INSERT INTO Logs (status, machineId) VALUES (?, ?)");
    query.addBindValue(status);
    query.addBindValue(machineId);

    if (!query.exec()) {
        qWarning() << "Unable to save event:" << query.lastError().text();
    }
}

#ifndef EMULATORMANAGER_H
#define EMULATORMANAGER_H

#include "machine.h"

#include <QObject>
#include <QSqlDatabase>
#include <QString>
#include <QTimer>
#include <QVector>

class EmulatorManager : public QObject
{
    Q_OBJECT

public:
    explicit EmulatorManager(QObject *parent = nullptr);
    ~EmulatorManager() override;

    // EmulatorManager owns each machine. IDs must be unique and positive.
    bool addMachine(int id);
    bool deleteMachine(int id);
    void startEmulator();
    void stopEmulator();
    void startStopMachine(int id, bool on);
    Machine *getMachineById(int id) const;
    int machineCount() const;

signals:
    void dataUpdate();
    void crashTrigger(int id, QString message);

private slots:
    void onTimeout();

private:
    void logToDatabase(const QString &status, int machineId);

    QSqlDatabase db_;
    QVector<Machine *> machines_;
    QTimer *timer_ = nullptr;
    bool isRunning_ = false;
    int countSecond = 0;
};

#endif // EMULATORMANAGER_H

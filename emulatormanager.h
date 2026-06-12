#ifndef EMULATORMANAGER_H
#define EMULATORMANAGER_H

#include "machine.h"

#include <QVector>
#include <QObject>
#include <QTimer>
#include <QMessageBox>
#include <QSqlDatabase>
#include <QSqlQuery>
#include <QDebug>

class EmulatorManager : public QObject
{
    Q_OBJECT

public:
    explicit EmulatorManager(QObject *parent = nullptr);
    ~EmulatorManager() = default;

    void addMachine(Machine* stanok); //Добавить станок
    void deleteMachine(int id);
    void startEmulator(); //Эмулятор запуск
    void stopEmulator(); //Эмулятор стоп
    void startStopMachine(int id, bool on); //Старт стоп станок

    Machine* getMachineById(int id);

signals:
    void dataUpdate(); //Данные для UI что данные изменились
    void crashTrigger(int id, QString message); //Сообщение об аварии

private slots:
    void onTimeout(); //Срабатывает каждую секунду


private:
    QSqlDatabase db_;
    QVector<Machine*> machines_;
    QTimer* timer_;
    bool isRunning_;
    int countSecond;


    void logToDatabase(QString status, int machineId);
};

#endif // EMULATORMANAGER_H

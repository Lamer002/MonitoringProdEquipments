#include "emulatormanager.h"
#include "logs.h"
#include <QSqlQuery>
#include <QSqlError>

EmulatorManager::EmulatorManager(QObject *parent) : QObject(parent)
{
    isRunning_ = false;
    countSecond = 0;

    db_ = QSqlDatabase::addDatabase("QSQLITE");
    db_.setDatabaseName("LogsMachine.db");

    if(db_.open())
    {
        qDebug() << "База данных создана /n" ;
        QSqlQuery query;
        bool success = query.exec("CREATE TABLE IF NOT EXISTS Logs ("
                                  "id INTEGER PRIMARY KEY AUTOINCREMENT,"
                                  "status TEXT NOT NULL,"
                                  "machineId INTEGER"
                                  ")");
        if (!success)
        {
            qDebug() << "Ошибка создания таблицы:" << query.lastError().text();
        }
        else
        {
            qDebug() << "Таблица Logs создана/n";
        }
    }
    else
    {
        qDebug() << "Это фиаско";
    }
}


void EmulatorManager::addMachine(Machine *stanok)
{
    machines_.append(stanok);
}

void EmulatorManager::deleteMachine(int id)
{
    for (int i = 0; i < machines_.count(); ++i)
    {
        if (id == machines_[i]->getId())
        {
            machines_.erase(machines_.begin() + i);
            break;
        }
    }
}

void EmulatorManager::startEmulator()
{

    if (isRunning_) return;

    isRunning_ = true;


    if(!timer_)
    {
        timer_ = new QTimer();
        connect(timer_, &QTimer::timeout, this, &EmulatorManager::onTimeout);
    }
    timer_->start(1000);
}

void EmulatorManager::stopEmulator()
{
    if (!isRunning_) return;

    if (timer_)
    {
        timer_->stop();
    }

    isRunning_ = false;
}

void EmulatorManager::startStopMachine(int id, bool on)
{
    for (auto it = machines_.begin(); it != machines_.end(); ++it)
    {
        if ((*it)->getId() == id)
        {
            if(on)
            {
                if ((*it)->getStatus() == Machine::STATUS_STOPPED)
                {
                    (*it)->setStatus(Machine::STATUS_RUNNING);
                    logToDatabase("Машина запущена с id: " + QString::number(id), id);
                }
            }
            else
            {
                if ((*it)->getStatus() == Machine::STATUS_RUNNING)
                {
                    (*it)->setStatus(Machine::STATUS_STOPPED);
                    logToDatabase("Машина остановлена с id: " + QString::number(id), id);
                }
            }
            break;
        }
    }
}

Machine *EmulatorManager::getMachineById(int id)
{
    for (Machine* m : machines_)
    {
        if(m->getId() == id) return m;
    }
    return nullptr;
}

void EmulatorManager::onTimeout()
{
    if (!isRunning_) return;

    countSecond ++;

    for (Machine* machine : machines_)
    {

        int currentStatus = machine->getStatus();
        int currentTemper = machine->getTemperature();
        int currentRPM = machine->getRPM();

        if (currentStatus == Machine::STATUS_RUNNING)
        {
            //Увеличение тмпературы 0-2 гр в секунду
            int temperIncrement = rand() % 3;
            int newTemper = currentTemper + temperIncrement;
            if (newTemper > DEFAULT_MAX_TEMPERATURE_WARNING) newTemper = DEFAULT_MAX_TEMPERATURE_WARNING;
            machine->setTemperature(newTemper);

            //Обороты +- 50 RPM
            int RPMIncrement = (rand() % 101) - 50;
            double newRPM = currentRPM + RPMIncrement;
            if (newRPM < 0) newRPM = 0;
            if (newRPM > DEFAULT_MAX_RPM_FREQ) newRPM = DEFAULT_MAX_RPM_FREQ;
            machine->setRPM(newRPM);

            //
            if (countSecond % 3600 == 0)
            {
                machine->incrementOperHourse();
            }

            //Провекрка на аварии
            if (newTemper > DEFAULT_MAX_TEMPERATURE_WARNING)
            {
                machine->setStatus(Machine::STATUS_EMERGENCY);
                machine->incrementCrashCount();
                logToDatabase("Температура машины привысила 120°C!, машина остановлена с id: " + QString::number(machine->getId()), machine->getId());

                emit crashTrigger(machine->getId(), "Перегрев станка! Температура: " + QString::number(newTemper) + "°C");
            }
            else if (newTemper > DEFAULT_MAX_TEMPERATURE_WARNING - 20)
            {
                if (machine->getStatus() != Machine::STATUS_EMERGENCY)
                {
                    machine->setStatus(Machine::STATUS_WARNING);
                    logToDatabase("Температура машины привысила 100°C!, машина остановлена с id: ", machine->getId());
                }
            }
        }
        else if (currentStatus == Machine::STATUS_STOPPED
                   || currentStatus == Machine::STATUS_EMERGENCY)
        {

            //Понижаем температуру
            if (currentTemper > 20)
            {
                int newTemper = currentTemper - (rand() % 2);
                if (newTemper < 20) newTemper = 20;
                machine->setTemperature(newTemper);
            }

            //
            if (currentRPM > 0)
            {
                int newRPM = currentRPM - (rand() % 2);
                if (newRPM < 0) newRPM = 0;
                machine->setRPM(newRPM);
            }
        }

    }

    emit dataUpdate();
}

void EmulatorManager::logToDatabase(QString status, int machineId)
{
    if(!db_.isOpen())
    {
        qDebug() << "бд не открыта для записи логов";
    }
    QSqlQuery query;
    query.prepare("INSERT INTO Logs (status, machineId) VALUES (?, ?)");
    query.addBindValue(status);
    query.addBindValue(machineId);

    if(!query.exec())
    {
        qDebug() << "Данные не занесены в бд/n" << query.lastError().text();
    }
    else
    {
        qDebug() << "Данные успешно занесены/n";
    }
}

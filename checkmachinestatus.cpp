#include "checkmachinestatus.h"
#include "ui_checkmachinestatus.h"

CheckMachineStatus::CheckMachineStatus(QWidget *parent)
    : QDialog(parent)
    , ui(new Ui::CheckMachineStatus)
{
    ui->setupUi(this);
}

CheckMachineStatus::~CheckMachineStatus()
{
    delete ui;
}

void CheckMachineStatus::setMachineId(int id, EmulatorManager *emulator)
{
    machineId_ = id;
    emul_ = emulator;
    connect(emul_, &EmulatorManager::dataUpdate,
            this, &CheckMachineStatus::updateUI);
    updateUI();
}

void CheckMachineStatus::updateUI()
{
    if (!emul_) return;
    Machine *machine = emul_->getMachineById(machineId_);
    if (!machine) {
        close(); // The selected machine was removed in the main window.
        return;
    }

    const auto status = machine->getStatus();
    updateStatusDisplay(status);

    ui->label_id->setText(QString::number(machine->getId()));
    ui->label_temperature->setText(
        QString("%1 °C").arg(machine->getTemperature()));
    ui->label_RPM->setText(
        QString("%1 rpm").arg(static_cast<int>(machine->getRPM())));
    ui->label_oper_hours->setText(
        QString::number(machine->getOperHours()));
    ui->label_count_breakdown->setText(
        QString::number(machine->getCrashCount()));
    ui->progressBar->setValue(machine->getEngineLoading());

    ui->pushButtonStart->setEnabled(
        status == Machine::STATUS_STOPPED ||
        (status == Machine::STATUS_EMERGENCY &&
         machine->getTemperature() < DEFAULT_TEMPERATURE_WARNING));
    ui->pushButtonStop->setEnabled(
        status == Machine::STATUS_RUNNING ||
        status == Machine::STATUS_WARNING);
}

void CheckMachineStatus::updateStatusDisplay(Machine::Status status)
{
    ui->label_start->setStyleSheet("color: #475569;");
    ui->label_stop->setStyleSheet("color: #475569;");
    ui->label_emrgency->setStyleSheet("color: #475569;");
    ui->label_warning->setStyleSheet("color: #475569;");

    switch (status) {
    case Machine::STATUS_RUNNING:
        ui->label_start->setStyleSheet("color: #4ade80;");
        break;
    case Machine::STATUS_STOPPED:
        ui->label_stop->setStyleSheet("color: #60a5fa;");
        break;
    case Machine::STATUS_EMERGENCY:
        ui->label_emrgency->setStyleSheet("color: #f87171;");
        break;
    case Machine::STATUS_WARNING:
        ui->label_warning->setStyleSheet("color: #fbbf24;");
        break;
    case Machine::STATUS_MAINTENANCE:
        break;
    }
}

void CheckMachineStatus::on_pushButtonStart_clicked()
{
    if (emul_) emul_->startStopMachine(machineId_, true);
}

void CheckMachineStatus::on_pushButtonStop_clicked()
{
    if (emul_) emul_->startStopMachine(machineId_, false);
}

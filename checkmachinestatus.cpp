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

void CheckMachineStatus::setMachineId(int id, EmulatorManager *emul)
{
    machineId_ = id;
    emul_ = emul;
    updateUI();

    connect(emul_, &EmulatorManager::dataUpdate, this, &CheckMachineStatus::updateUI);
}

void CheckMachineStatus::updateUI()
{
    Machine* machine = emul_->getMachineById(machineId_);

    if(!machine) return;

    updateStatusDisplay(machine->getStatus());

    //get ID
    ui->label_id->setText(QString::number(machine->getId()));

    //get temperature
    ui->label_temperature->setText(QString::number(machine->getTemperature()));

    //get RPM
    ui->label_RPM->setText(QString::number(machine->getRPM()));

    //
    ui->label_oper_hours->setText(QString::number(machine->getOperHours()));

    //
    ui->label_count_breakdown->setText(QString::number(machine->getCrashCount()));

    //
    ui->progressBar->setValue(machine->getEngineLoading());
}

void CheckMachineStatus::updateStatusDisplay(Machine::Status st)
{

    ui->label_start->setStyleSheet("color: gray;");
    ui->label_stop->setStyleSheet("color: gray;");
    ui->label_emrgency->setStyleSheet("color: gray;");
    ui->label_warning->setStyleSheet("color: gray;");

    if(st == Machine::STATUS_RUNNING) ui->label_start->setStyleSheet("color: green;");
    else if(st == Machine::STATUS_STOPPED) ui->label_stop->setStyleSheet("color: blue;");
    else if(st == Machine::STATUS_EMERGENCY) ui->label_emrgency->setStyleSheet("color: yellow;");
    else if(st == Machine::STATUS_WARNING) ui->label_warning->setStyleSheet("color: red;");


}

void CheckMachineStatus::on_pushButtonStart_clicked()
{
    emul_->startStopMachine(machineId_, true);
}


void CheckMachineStatus::on_pushButtonStop_clicked()
{
    emul_->startStopMachine(machineId_, false);
}


#include "mainwindow.h"
#include "./ui_mainwindow.h"
#include "checkmachinestatus.h"
#include "addmachineinlist.h"
#include "logs.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
    , emulator_manager_(new EmulatorManager(this))
{
    ui->setupUi(this);
    emulator_manager_->startEmulator();

    connect(ui->listWidgetMachines, &QListWidget::itemDoubleClicked, this, &MainWindow::onMachineDoubleClick);
}

MainWindow::~MainWindow()
{
    delete ui;
}
////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void MainWindow::onMachineDoubleClick(QListWidgetItem *i)
{
    int machineId = i->data(Qt::UserRole).toInt();

    CheckMachineStatus* machineCheck = new CheckMachineStatus(this);

    //Освобождение памяти
    machineCheck->setAttribute(Qt::WA_DeleteOnClose);

    machineCheck->setMachineId(machineId, emulator_manager_);

    machineCheck->show();
}

void MainWindow::on_pushButtonAddMachine_clicked()
{
    AddMachineInList* add_ = new AddMachineInList(this);

    //Освобождение памяти
    add_->setAttribute(Qt::WA_DeleteOnClose);

    connect(add_, &AddMachineInList::addInMachine, this, &MainWindow::onMachineAdd);

    add_->show();
}

void MainWindow::onMachineAdd(QString value)
{
    int id = value.toInt();

    Machine* machine = new Machine();
    machine->setId(id);

    emulator_manager_->addMachine(machine);

    QListWidgetItem* item = new QListWidgetItem(QString("Станок 1 ID: " + QString::number(id)).arg(id));
    item->setData(Qt::UserRole, id);
    ui->listWidgetMachines->addItem(item);
}


void MainWindow::on_pushButtonDeleteMachine_clicked()
{
    QListWidgetItem* item = ui->listWidgetMachines->currentItem();

    if (item)
    {
        int id = item->data(Qt::UserRole).toInt();
        emulator_manager_->deleteMachine(id);
        delete item;
    }
}


void MainWindow::on_pushButtonLogs_clicked()
{
    Logs* l = new Logs();
    l->show();
}


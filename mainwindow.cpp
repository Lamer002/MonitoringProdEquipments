#include "mainwindow.h"
#include "ui_mainwindow.h"

#include "addmachineinlist.h"
#include "checkmachinestatus.h"
#include "logs.h"

#include <QColor>
#include <QListWidgetItem>
#include <QMessageBox>

namespace {
QString statusName(Machine::Status status)
{
    switch (status) {
    case Machine::STATUS_RUNNING: return QStringLiteral("Работает");
    case Machine::STATUS_WARNING: return QStringLiteral("Предупреждение");
    case Machine::STATUS_EMERGENCY: return QStringLiteral("Авария");
    case Machine::STATUS_MAINTENANCE: return QStringLiteral("Обслуживание");
    case Machine::STATUS_STOPPED: return QStringLiteral("Остановлен");
    }
    return QStringLiteral("Неизвестно");
}

QColor statusColor(Machine::Status status)
{
    switch (status) {
    case Machine::STATUS_RUNNING: return QColor("#4ade80");
    case Machine::STATUS_WARNING: return QColor("#fbbf24");
    case Machine::STATUS_EMERGENCY: return QColor("#f87171");
    default: return QColor("#cbd5e1");
    }
}
} // namespace

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
    , emulator_manager_(new EmulatorManager(this))
{
    ui->setupUi(this);
    connect(ui->listWidgetMachines, &QListWidget::itemDoubleClicked,
            this, &MainWindow::onMachineDoubleClick);
    connect(emulator_manager_, &EmulatorManager::dataUpdate,
            this, &MainWindow::refreshMachineList);
    connect(emulator_manager_, &EmulatorManager::crashTrigger,
            this, [this](int id, const QString &message) {
                QMessageBox::warning(
                    this, tr("Авария станка #%1").arg(id), message);
            });

    refreshMachineList();
    emulator_manager_->startEmulator();
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::onMachineDoubleClick(QListWidgetItem *item)
{
    if (!item) return;

    auto *dialog = new CheckMachineStatus(this);
    dialog->setAttribute(Qt::WA_DeleteOnClose);
    dialog->setMachineId(item->data(Qt::UserRole).toInt(), emulator_manager_);
    dialog->show();
}

void MainWindow::on_pushButtonAddMachine_clicked()
{
    auto *dialog = new AddMachineInList(this);
    dialog->setAttribute(Qt::WA_DeleteOnClose);
    connect(dialog, &AddMachineInList::addInMachine,
            this, &MainWindow::onMachineAdd);
    dialog->show();
}

void MainWindow::onMachineAdd(QString value)
{
    bool parsed = false;
    const int id = value.toInt(&parsed);
    if (!parsed || !emulator_manager_->addMachine(id)) {
        QMessageBox::warning(this, tr("Невозможно добавить станок"),
                             tr("ID должен быть положительным и уникальным."));
        return;
    }

    auto *item = new QListWidgetItem(ui->listWidgetMachines);
    item->setData(Qt::UserRole, id);
    refreshMachineList();
}

void MainWindow::on_pushButtonDeleteMachine_clicked()
{
    QListWidgetItem *item = ui->listWidgetMachines->currentItem();
    if (!item) {
        QMessageBox::information(this, tr("Выберите станок"),
                                 tr("Сначала выберите станок в списке."));
        return;
    }

    const int id = item->data(Qt::UserRole).toInt();
    emulator_manager_->deleteMachine(id);
    delete item;
    refreshMachineList();
}

void MainWindow::on_pushButtonLogs_clicked()
{
    auto *dialog = new Logs(this);
    dialog->setAttribute(Qt::WA_DeleteOnClose);
    dialog->show();
}

void MainWindow::refreshMachineList()
{
    int active = 0;
    for (int index = 0; index < ui->listWidgetMachines->count(); ++index) {
        auto *item = ui->listWidgetMachines->item(index);
        Machine *machine = emulator_manager_->getMachineById(
            item->data(Qt::UserRole).toInt());
        if (!machine) continue;

        if (machine->getStatus() == Machine::STATUS_RUNNING ||
            machine->getStatus() == Machine::STATUS_WARNING) {
            ++active;
        }

        item->setText(
            QString("  #%1    %2     ·     %3 °C     ·     %4 об/мин")
                .arg(machine->getId())
                .arg(statusName(machine->getStatus()))
                .arg(machine->getTemperature())
                .arg(static_cast<int>(machine->getRPM())));
        item->setForeground(statusColor(machine->getStatus()));
    }

    ui->labelSummary->setText(
        tr("Всего: %1    ·    Активны: %2")
            .arg(emulator_manager_->machineCount())
            .arg(active));
}

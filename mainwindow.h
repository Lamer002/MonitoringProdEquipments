#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include "emulatormanager.h"

#include <QMainWindow>
#include <QListWidget>

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = nullptr);
    ~MainWindow() override;

private slots:
    void onMachineDoubleClick(QListWidgetItem *item);
    void on_pushButtonAddMachine_clicked();
    void onMachineAdd(QString value);
    void on_pushButtonDeleteMachine_clicked();
    void on_pushButtonLogs_clicked();
    void refreshMachineList();

private:
    Ui::MainWindow *ui;
    EmulatorManager *emulator_manager_;
};

#endif // MAINWINDOW_H

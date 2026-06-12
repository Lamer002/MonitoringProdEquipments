#ifndef LOGS_H
#define LOGS_H

#include <QDialog>
#include <QDebug>
#include <QSqlTableModel>

namespace Ui {
class Logs;
}

class Logs : public QDialog
{
    Q_OBJECT

public:
    explicit Logs(QWidget *parent = nullptr);
    ~Logs();


private slots:
    void on_pushButtonExit_clicked();
    void updateLogs();

    void on_pushButtonClearLogs_clicked();

private:
    Ui::Logs *ui;
    QSqlTableModel* model_;
    QSqlDatabase db_;

};

#endif // LOGS_H

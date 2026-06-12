#include "logs.h"
#include "ui_logs.h"
#include <QSqlQuery>
#include <QSqlError>

Logs::Logs(QWidget *parent)
    : QDialog(parent)
    , ui(new Ui::Logs)
    , model_(new QSqlTableModel(this))
{
    ui->setupUi(this);

    if(QSqlDatabase::contains())
    {
        db_ = QSqlDatabase::database();
    }
    else
    {
        db_ = QSqlDatabase::addDatabase("QSQLITE");
        db_.setDatabaseName("LogsMachine.db");
    }

    if(db_.open())
    {
        qDebug() << "Logs: База данных открыта";
        updateLogs();
    }
    else
    {
        qDebug() << "Logs: Ошибка открытия БД:" << db_.lastError().text();
    }

}

Logs::~Logs()
{
    delete ui;
}


void Logs::on_pushButtonExit_clicked()
{
    this->close();
}

void Logs::updateLogs()
{
    if (!db_.isOpen())
    {
        qDebug() << "бд не открыта";
        return;
    }

    model_->setTable("Logs");

    //ручное сохранение в бд из TableView
    model_->setEditStrategy(QSqlTableModel::OnManualSubmit);

    model_->select();

    ui->tableViewDB->setModel(model_);

    ui->tableViewDB->setColumnWidth(0,0);
    ui->tableViewDB->setColumnWidth(1,300);
    ui->tableViewDB->setColumnWidth(2,80);
}

void Logs::on_pushButtonClearLogs_clicked()
{
    QSqlQuery query;
    query.prepare("DELETE FROM Logs");

    if(query.exec())
    {
        qDebug() << "Данные удалены из бд";
    }
    else
    {
        qDebug() << "Ошибка удалния " << query.lastError().text();
    }

    updateLogs();
}


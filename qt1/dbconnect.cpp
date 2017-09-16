#include "dbconnect.h"
#include "ui_dbconnect.h"
#include <QSqlDatabase>
#include <QMessageBox>
#include <QSqlError>
#include <QtDebug>
#include <QSqlQuery>

DbConnect :: DbConnect(QWidget *parent):
    QDialog(parent),
    ui(new Ui::DbConnect)
{
    ui->setupUi(this);
    QStringList drivers = QSqlDatabase::drivers();
    ui->dbComboBox->addItems(drivers);
    ui->status_Label->setText(tr("状态: 准备连接数据库"));
}

DbConnect::~DbConnect()
{
    delete ui;
}

void DbConnect::on_submit_clicked()
{
    if(ui->dbComboBox->currentText().isEmpty())
    {
        ui->status_Label->setText(tr("请选择一个数据可驱动"));
        ui->dbComboBox->setFocus();
    }
    else if(ui->dbComboBox->currentText() == "QSQLITE")
    {
        creatDB();
        accept();
    }
    else
    {
        QMessageBox::information(this, tr("数据库"), tr("对不起，目前只有QSQLITE驱动可用"));
    }
}

void DbConnect::creatDB()
{
    QSqlError err;
    QSqlDatabase db = QSqlDatabase::addDatabase(("QSQLITE"));
    db.setDatabaseName("student.db");
    db.open();
    if(!db.open())
    {
        err = db.lastError();
        qDebug()<<err.text();
    }
    else
    {
        ui->status_Label->setText(tr("创建sqlite数据库成功"));
    }

    QSqlQuery query;
    bool success = query.exec("create table student(id int primary key, name varchar(40), age int, major varchar(40), credit int, address varchar(40))");
    if(success)
    {
        qDebug()<<QObject::tr("数据库表创建成功");
    }
    query.exec(QObject::tr("insert into student values(1,'ZYS', 22, 'Tec', 100 ,'China')"));
    query.exec(QObject::tr("insert into student values(2,'HYL', 20, 'Inf',  80, 'Beijing')"));
    query.exec(QObject::tr("insert into student values(3,'KR', 21, 'IOT', 90, 'Shanghai')"));
    query.exec(QObject::tr("insert into student values(4,'YZY', 19, 'BAT', 88, 'Nanjing')"));
    query.exec(QObject::tr("insert into student values(5,'ZYX', 18, 'WTF',  92, 'Shenzheng')"));
}

void DbConnect::on_cancel_clicked()
{
    close();
}

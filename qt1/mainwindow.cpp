#include "mainwindow.h"
#include <QGridLayout>
#include <QPushButton>
#include <QAbstractItemView>
#include <QLabel>
#include <QSqlRecord>
#include <QModelIndexList>
#include <QMessageBox>
#include <QtDebug>
#include <QStringList>
#include <QDialog>
#include <QSqlField>

MainWindow::MainWindow(const QString &studentTable, QWidget *parent)
    : QMainWindow(parent)
{
  stuModel = new QSqlTableModel(this);
  stuModel->setTable(studentTable);
  stuModel->select();

  QGroupBox *student = createStuGroup();
  QGroupBox *detail = createDetailGroup();

  QGridLayout *layout = new QGridLayout;
  layout->addWidget(student, 0, 0, 2, 1 );
  layout->addWidget(detail, 0,1, 2, 1);
  layout->setColumnMinimumWidth(0, 500);
  QWidget *widget = new QWidget;
  widget->setLayout(layout);
  setCentralWidget(widget);
  resize(850,400);
}

QGroupBox* MainWindow::createStuGroup()
{
    stuView = new QTableView;
    stuView->setEditTriggers(QAbstractItemView::NoEditTriggers);
    stuView->setSortingEnabled(true);
    stuView->setSelectionBehavior(QAbstractItemView::SelectRows);
    stuView->setSelectionMode(QAbstractItemView::SingleSelection);
    stuView->setShowGrid(true);
    stuView->setAlternatingRowColors(true);
    stuView->setModel(stuModel);
    connect(stuView,SIGNAL(clicked(QModelIndex)),this,SLOT(changeStuView(QModelIndex)));

    QGroupBox *box = new QGroupBox(tr("学生资料"));
    QGridLayout *layout = new QGridLayout;
    layout->addWidget(stuView,0,0);
    box->setLayout(layout);
    return box;
}

QGroupBox* MainWindow::createDetailGroup()
{
    QGroupBox *box = new QGroupBox(tr("详细信息"));
    showList = new QListWidget;
    QPushButton *deleteBtn = new QPushButton(tr("删除"));
    QPushButton *addBtn = new QPushButton(tr("添加"));
    QPushButton *selectBtn = new QPushButton(tr("查找"));
    QPushButton *returnBtn = new QPushButton(tr("返回列表"));
    QPushButton *quitBtn = new QPushButton(tr("退出"));
    seLineEdit = new QLineEdit;
    QGridLayout *layout = new QGridLayout;
    connect(deleteBtn,SIGNAL(clicked(bool)),this,SLOT(removeStuFromStuModel()));
    connect(selectBtn,SIGNAL(clicked(bool)),this,SLOT(selectSlot()));
    connect(returnBtn,SIGNAL(clicked(bool)),this,SLOT(returnSlot()));
    connect(addBtn,SIGNAL(clicked(bool)),this,SLOT(addNewSlot()));
    connect(quitBtn,SIGNAL(clicked(bool)),this,SLOT(quitBtnSLot()));

    layout->addWidget(showList,0,0,3,2);
    layout->addWidget(deleteBtn,3,0,1,1);
    layout->addWidget(addBtn,3,1,1,1);
    layout->addWidget(seLineEdit,4,0,1,1);
    layout->addWidget(selectBtn,4,1,1,1);
    layout->addWidget(returnBtn,5,0,1,1);
    layout->addWidget(quitBtn,5,1,1,1);

    box->setLayout(layout);
    return box;

}
void MainWindow::changeStuView(QModelIndex index)//定位选择的行，并在右边list中显示出来
{   showList->clear();
    item = new QListWidgetItem(showList);
    QSqlRecord record = stuModel->record(index.row());
    int stuId = record.value("id").toInt();
    QString name =record.value("name").toString();
    int age = record.value("age").toInt();
    QString major =record.value("major").toString();
    int credit = record.value("credit").toInt();
    QString add = record.value("address").toString();
    item->setText(tr("编号：%1\n 姓名：%2\n 年龄：%3\n 专业：%4\n 学分：%5\n 地址：%6\n").arg(stuId).arg(name).arg(age).arg(major).arg(credit).arg(add));
}
void MainWindow::removeStuFromStuModel()
{
    QModelIndexList select =stuView->selectionModel()->selectedRows(0);
    if(!select.empty())
    {
        QModelIndex idIndex = select.at(0);
        QString name =idIndex.sibling(idIndex.row(),1).data().toString();
        QMessageBox::StandardButton button;
        button=QMessageBox::question(this,tr("删除学生信息"),QString (tr("确定从数据库删除'%1'的信息吗？").arg(name)),QMessageBox::Yes|QMessageBox::No);
        if(button==QMessageBox::Yes)
        {
          stuModel->removeRow(idIndex.row());
          stuModel->submitAll();
          stuModel->select();

        }
        else
        {
            QMessageBox::information(this,tr("删除学生信息"),tr("请选择要删除的学生信息！"));
        }
    }

}
void MainWindow::selectSlot()
{

  QString name =seLineEdit->text();
  if(name.isEmpty())
      return;
  //qDebug()<<name;
  else
  {
  stuModel->setFilter(QObject::tr("name = '%1'").arg(name));
  stuModel->select();//显示结果
  seLineEdit->clear();
  }
}
void MainWindow::returnSlot()
{
    stuModel->setTable("student");
    stuModel->setSort(0,Qt::AscendingOrder);
    stuModel->select();
}
void MainWindow::addNewSlot()
{
   QDialog *dialog = new QDialog();
   dialog->setWindowTitle(tr("添加"));
   QGroupBox *box  = new QGroupBox(this);
   QPushButton *submitBtn = new QPushButton(tr("确认"));
   QPushButton *cancelBtn = new QPushButton(tr("取消"));
   connect(submitBtn,SIGNAL(clicked(bool)),this,SLOT(addNewToTable()));
   connect(cancelBtn,SIGNAL(clicked(bool)),this,SLOT(cancelBtnSlot()));

   QLabel *idLabel  = new QLabel(tr("学号："));
   QLabel *nameLabel = new QLabel(tr("姓名："));
   QLabel *ageLabel  = new QLabel(tr("年龄："));
   QLabel *majorLabel = new QLabel(tr("专业:"));
   QLabel *creditLabel = new QLabel(tr("学分:"));
   QLabel *addLabel = new QLabel(tr("地址："));

   idLineEdit = new QLineEdit;
   nameLineEdit = new QLineEdit;
   ageLineEdit = new QLineEdit;
   majorLineEdit = new QLineEdit;
   creditLineEdit = new QLineEdit;
   addLineEdit = new QLineEdit;

   QGridLayout *layout = new QGridLayout;
   layout->addWidget(idLabel,0,0,1,1);
   layout->addWidget(idLineEdit,0,1,1,1);
   layout->addWidget(nameLabel,1,0,1,1);
   layout->addWidget(nameLineEdit,1,1,1,1);
   layout->addWidget(ageLabel,2,0,1,1);
   layout->addWidget(ageLineEdit,2,1,1,1);
   layout->addWidget(majorLabel,3,0,1,1);
   layout->addWidget(majorLineEdit,3,1,1,1);
   layout->addWidget(creditLabel,4,0,1,1);
   layout->addWidget(creditLineEdit,4,1,1,1);
   layout->addWidget(addLabel,5,0,1,1);
   layout->addWidget(addLineEdit,5,1,1,1);
   layout->addWidget(submitBtn,6,0,1,1);
   layout->addWidget(cancelBtn,6,1,1,1);

   box->setLayout(layout);
   QGridLayout *mainLayout = new QGridLayout;
   mainLayout->addWidget(box);
   dialog->setLayout(mainLayout);
   dialog->show();
}
void MainWindow::addNewToTable()
{
    int id = idLineEdit->text().toInt();
    QString name = nameLineEdit->text();
    int age =ageLineEdit->text().toInt();
    QString major = majorLineEdit->text();
    int credit = creditLineEdit->text().toInt();
    QString address = addLineEdit->text();

    //if(id.is||name.isEmpty()||age==0||major.isEmpty()||credit==0||address.isEmpty());
    //return;
    qDebug()<<id<<name<<age<<major<<credit<<address;
    QSqlRecord record;
    QSqlField f1("id",QVariant::Int);
    QSqlField f2("name",QVariant::String);
    QSqlField f3("age",QVariant::Int);
    QSqlField f4("major",QVariant::String);
    QSqlField f5("credit",QVariant::Int);
    QSqlField f6("address",QVariant::String);

    f1.setValue(QVariant(id));
    f2.setValue(QVariant(name));
    f3.setValue(QVariant(age));
    f4.setValue(QVariant(major));
    f5.setValue(QVariant(credit));
    f6.setValue(QVariant(address));

    record.append(f1);
    record.append(f2);
    record.append(f3);
    record.append(f4);
    record.append(f5);
    record.append(f6);

    stuModel->insertRecord(-1,record);
    //stuModel->setTable("student");
    stuModel->setSort(0,Qt::AscendingOrder);
    stuModel->select();
    clearSlot();
}
void MainWindow::cancelBtnSlot()
{
    clearSlot();
}
void MainWindow::clearSlot()
{
    idLineEdit->clear();
    nameLineEdit->clear();
    ageLineEdit->clear();
    majorLineEdit->clear();
    creditLineEdit->clear();
    addLineEdit->clear();
}
void MainWindow::quitBtnSLot()
{
    this->close();
}
MainWindow::~MainWindow()
{

}

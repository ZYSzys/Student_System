#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QSqlTableModel>
#include <QTableView>
#include <QGroupBox>
#include <QListWidget>
#include <QModelIndex>
#include <QLineEdit>

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(const QString &studentTable, QWidget *parent = 0);
    ~MainWindow();

private slots:
    void changeStuView(QModelIndex index);//定位选择的行，并在右边list中显示出来
    void removeStuFromStuModel();//从数据库删除记录
    void selectSlot();//查询函数
    void returnSlot();//返回列表函数
    void addNewSlot();//添加按钮对应的槽，槽中创建添加记录的对话框对象
    void addNewToTable();//添加新纪录到数据库
    void cancelBtnSlot();//添加对话框中的取消按钮对应的槽
    void clearSlot();//清空LineEdit对象
    void quitBtnSLot();//退出

private:
    QGroupBox *createStuGroup();
    QGroupBox *createDetailGroup();
    QSqlTableModel *stuModel;
    QTableView *stuView;
    QListWidget *showList;
    QLineEdit *seLineEdit;
    QListWidgetItem *item;
    QLineEdit *idLineEdit;
    QLineEdit *nameLineEdit;
    QLineEdit *ageLineEdit;
    QLineEdit *majorLineEdit;
    QLineEdit *creditLineEdit;
    QLineEdit *addLineEdit;
};

#endif // MAINWINDOW_H

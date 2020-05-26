#include "login.h"
#include "ui_login.h"
#include "widget.h"
//QString str1;
//构造函数
Form::Form(QWidget *parent) :
    QWidget(parent),
    fui(new Ui::Form)
{
    fui->setupUi(this);
    //Widget *ui=new Widget;
    //connect(this, SIGNAL(sendData(QString)), ui, SLOT(receiveData(QString)));

    QString currentDir = QDir::currentPath();

    QString dir_str=currentDir+"/db";




    QDir dir(dir_str);
        QStringList nameFilters;
        nameFilters << "*.db";
        QStringList files = dir.entryList(nameFilters, QDir::Files|QDir::Readable, QDir::Name);

        //qDebug()<<files.size();
        //qDebug()<<files.at(0);

        QStringList files1;
        for(int i=0;i<files.size();i++)
        {
            QString str=files.at(i);
            str=str.left(str.size()-3);
            files1.append(str);

        }
        fui->comboBox->addItems(files1);



}

//析构函数
Form::~Form()
{
    delete fui;
}

void Form::on_InMainButton_clicked()
{
    //qDebug()<<fui->databaseNamelineEdit->text();

    Widget *ui=new Widget;
    QObject::connect(this, SIGNAL(sendData(QString)), ui, SLOT(receiveData(QString)));

    emit sendData(fui->databaseNamelineEdit->text());  //获取lineEdit的输入并且传递出去

    ui->show();
    this->close();
}

void Form::on_comboBox_currentTextChanged(const QString &arg1)
{
    fui->databaseNamelineEdit->setText(arg1);

    //DatabaseName=arg1;

    //qDebug()<<arg1;
    //qDebug()<<fui->databaseNamelineEdit->text();



}



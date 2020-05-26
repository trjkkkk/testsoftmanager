#ifndef WIDGET_H
#define WIDGET_H

#include <QWidget>
#include<QListWidget>
#include<QLineEdit>
#include<QtGui>
#include<QMessageBox>
#include<string.h>
#include<QDebug>
#include <QCryptographicHash>
#include <QSqlQuery>
#include <QSqlError>
#include <QSqlDatabase>
#include <QMap>
#include <QIcon>
#include <QStandardItemModel>

#include <QPdfWriter>
#include <QFileDialog>
#include <QProgressBar>



//以下是图标的宏设定
#ifndef TREE_ITEM_ICON_Project
#define TREE_ITEM_ICON_Project QStringLiteral("treeItem_Project")
#endif
#ifndef TREE_ITEM_ICON_folder
#define TREE_ITEM_ICON_folder QStringLiteral("treeItem_folder")
#endif
#ifndef TREE_ITEM_ICON_folderOriginal
#define TREE_ITEM_ICON_folderOriginal QStringLiteral("treeItem_folder-ansys")
#endif
#ifndef TREE_ITEM_ICON_folderAnsys
#define TREE_ITEM_ICON_folderAnsys QStringLiteral("treeItem_folder-ansys")
#endif
#ifndef TREE_ITEM_ICON_TdmsGroup
#define TREE_ITEM_ICON_TdmsGroup QStringLiteral("treeItem_group")
#endif
#ifndef TREE_ITEM_ICON_TdmsChannel
#define TREE_ITEM_ICON_TdmsChannel QStringLiteral("treeItem_channel")
#endif
#ifndef TREE_ITEM_ICON_DataItem
#define TREE_ITEM_ICON_DataItem QStringLiteral("treeItem_dataItem")
#endif

namespace Ui {
class Widget;
}

class Widget : public QWidget
{
    Q_OBJECT

public:
    explicit Widget(QWidget *parent = 0);
    ~Widget();

    QString*filePath=new QString[1000];
    QString*resultFilePath=new QString[1000];
    QSqlDatabase db;

    int*sorPos=new int[1000];//源文件的pos
    int*daiPos=new int[1000];//待验文件的pos

    QString Inf;//点击比较树按钮时传递的信息

    QString dName;

    //extern QString DatabaseName;

    //static QString DatabaseName;

    boolean boln;

    int proid;

    int unitid;
    int vid;

    QString ProNamel,UnitNamel,VerNamel,Labell,ProDail;
    QString Adefl,Bdefl,Cdefl,Ddefl;
    QString Statel;
    int FileNuml;

     void initOperTree();//操作界面的树

      void initShowProTree();//展示产品界面的树
      void initShowVerTree(QString str1, QString str2);//展示版本界面的树

      void showSrcTree(QString str1, QString str2);//展示源文件版本界面的树

      void showCurTree();//展示当前文件版本界面的树

      void getCurFileDetail();

      void resultShowTree();//比较
      void compareVer(QString str2);

      void connectDB();
      void createFileTable();

      void createProductTable();

      void createUnitTable();

      void createVersionTable();

      void createLittleFileTable();

      void intoFileData();//插入数据到test_file数据表

      void intoProductData();//插入数据到product数据表

      void intoUnitData();

      void intoLittleFileData();

      void intoVersionData();

      void updateProductData();

      void updateUnitData();

      void updateVersionData();

      void updateFileData();

      QString generateProUnit();

      QString generateProUnitVer();

      void generatePdf();

      void DrawCircuitPaint();



protected:
    void
dragEnterEvent(QDragEnterEvent *event);//当用户把一个对象拖动到窗口部件上时，调用该函数

    void
dropEvent(QDropEvent* event);//窗口部件放下一个对象时，调用该函数

    QString
ReadtestFile(QString str);//导入数据 CompareForm::

    QString
productMD5(QString str);

    bool eventFilter(QObject *watched, QEvent *event);

private slots:
    void on_ProtreeView_clicked(const QModelIndex &index);

    void on_CompareTreeView_clicked(const QModelIndex &index);

    QStandardItem* getTopParent(QStandardItem* item);
    QModelIndex getTopParent(QModelIndex itemIndex);

    void on_pushButton_2Clear_clicked();

    //void on_pushButtonClear_clicked();

    void on_intoUniButton_clicked();

    void on_intoProButton_clicked();

    void on_intoVerButton_clicked();

    void on_updateProButton_clicked();

    void on_updateUnitButton_clicked();

    void on_updateVerButton_clicked();

    //void on_CompareButton_clicked();

    void on_clearButton_clicked();

    void on_verNameComboBox_currentTextChanged(const QString &arg1);

    void on_verStateComboBox_currentTextChanged(const QString &arg1);

    //void on_ProtreeView_doubleClicked(const QModelIndex &index);

    void on_verClearButton_clicked();

    void on_comparButton_clicked();



    void receiveData(QString data);   //接收传递过来的数据的槽

private:
    Ui::Widget *ui;

    QFileInfoList
GetAllFileLst(QString filePath);//循环读取文件路径

    QMap<QString,QIcon> m_publicIconMap;///< 存放公共图标
};


#endif // WIDGET_H

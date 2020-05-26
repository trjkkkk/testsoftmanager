#include "widget.h"
#include "ui_widget.h"
//#include "login.h"

//extern QString str1;

//extern QString DatabaseName;

//构造函数
Widget::Widget(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::Widget)
{
    ui->setupUi(this);
    ui->VerFilelistWidget->setAcceptDrops(false);
    ui->resultListWidget->setAcceptDrops(false);
    setAcceptDrops(true);
    ui->tabWidget->setCurrentIndex(0);
    ui->tabWidget_2->setCurrentIndex(0);
    //ui->ProtreeView->setEnabled(false);
    ui->ProtreeView->setEditTriggers(QAbstractItemView::NoEditTriggers);
    ui->VertreeView->setEditTriggers(QAbstractItemView::NoEditTriggers);
    ui->CompareTreeView->setEditTriggers(QAbstractItemView::NoEditTriggers);


    m_publicIconMap[TREE_ITEM_ICON_Project] = QIcon(QStringLiteral(":/treeIcon/res_treeItemIcon/Project.png"));
    m_publicIconMap[TREE_ITEM_ICON_folder] = QIcon(QStringLiteral(":/treeIcon/res_treeItemIcon/folder.png"));
    m_publicIconMap[TREE_ITEM_ICON_folderAnsys] = QIcon(QStringLiteral(":/treeIcon/res_treeItemIcon/folder-ansys.png"));
    m_publicIconMap[TREE_ITEM_ICON_TdmsGroup] = QIcon(QStringLiteral(":/treeIcon/res_treeItemIcon/group.png"));
    m_publicIconMap[TREE_ITEM_ICON_TdmsChannel] = QIcon(QStringLiteral(":/treeIcon/res_treeItemIcon/channel.png"));
    m_publicIconMap[TREE_ITEM_ICON_folderOriginal] = QIcon(QStringLiteral(":/treeIcon/res_treeItemIcon/folder_original.png"));
    m_publicIconMap[TREE_ITEM_ICON_DataItem] = QIcon(QStringLiteral(":/treeIcon/res_treeItemIcon/dataItem.png"));


    //Ui loginui=new Ui::Form;
    //Form *login=new Form;

    //connect(login, SIGNAL(sendData(QString)), this, SLOT(receiveData(QString)));

    //login->show();
    //QString dbn=login->getDabaName();




    //qDebug()<<dbn;

    //dName=dbn;


    //ui->ProtreeView->

    initShowProTree();
    //initTableWidget();
    Widget::resultShowTree();

    //ui->tab_3->installEventFilter(this);

   //Widget::eventFilter(QObject *watched, QEvent *event);
    //initShowVerTree();

    //delete login;
}
//析构函数
Widget::~Widget()
{
    delete ui;
}

void Widget::receiveData(QString data)
{
    qDebug()<<dName;
    dName=data;     //获取传递过来的数据
    //qDebug()<<dName;

    connectDB();
    //createFileTable();
    createProductTable();
    createUnitTable();
    createVersionTable();
    createLittleFileTable();
}


//拖文件
void
Widget::dragEnterEvent(QDragEnterEvent *event)
{
    if(event->mimeData()->hasFormat("text/uri-list"))
    {
        event->acceptProposedAction();
    }
}
//放文件，然后获取文件路径
void
Widget::dropEvent(QDropEvent *event)
{
    ui->resultLineEdit->clear();
    //窗口部件放下一个对象时，调用该函数
    QList<QUrl>urls=event->mimeData()->urls();
    if(urls.isEmpty())
    {
        return;
    }
    QList<QUrl>::iterator it;
    for(it=urls.begin();it!=urls.end();it++)
    {
        QUrl url=*it;
        QFileInfo fileInfo(it->toLocalFile());
        if(fileInfo.isFile())
        {
            //是文件就直接加进来
            if(ui->tabWidget->currentWidget()==ui->tab)
            {
                ui->VerFilelistWidget->addItem(fileInfo.filePath());
            }
            if(ui->tabWidget->currentWidget()==ui->tab_2)
            {
                ui->resultListWidget->addItem(fileInfo.filePath());
            }
        }
        else if(fileInfo.isDir())
        {
            //是文件路径就循环读取
            QFileInfoList fileInfoList=Widget::GetAllFileLst(fileInfo.filePath());
            for(QFileInfo fileInfo1:fileInfoList)//the range-based for statements
            {
                if(ui->tabWidget->currentWidget()==ui->tab)
                {
                    ui->VerFilelistWidget->addItem(fileInfo1.filePath());
                }
                if(ui->tabWidget->currentWidget()==ui->tab_2)
                {
                    ui->resultListWidget->addItem(fileInfo1.filePath());
                }
            }
        }
    }    
   for( int i=0;i<ui->VerFilelistWidget->count();i++)
   {
    filePath[i]=ui->VerFilelistWidget->item(i)->text();//文件路劲
   }

   for( int i=0;i<ui->resultListWidget->count();i++)
   {
    resultFilePath[i]=ui->resultListWidget->item(i)->text();//文件路劲
    qDebug()<<resultFilePath[i];
   }
   //Widget::showCurTree();
   Widget::getCurFileDetail();
}




void Widget::getCurFileDetail()
{
    //int fileNum=ui->resultListWidget->count();//文件数
    //QString productLabel;//大标签
    //QString*littleProLa=new QString[1000];//小标签
    QString*fName=new QString[100];
    int count=0;
    //QString allfilecontent;//整个版本文件里面的所有内容
    //for( int i=0;i<ui->resultListWidget->count();i++)
    //{
     //allfilecontent.append(Widget::ReadtestFile(resultFilePath[i]));
     //littleProLa[i]=Widget::productMD5(Widget::ReadtestFile(ui->resultListWidget->item(i)->text()));
    //}
    //productLabel=Widget::productMD5(allfilecontent);
    QString*fPath=new QString[100];
    QString*fpn=new QString[100];

    int temp=0;
    for( int i=0;i<ui->resultListWidget->count();i++)
    {
        count=0;
        QFileInfo l_info(resultFilePath[i]);
        fName[i]=l_info.fileName();
        fPath[i]=l_info.path().mid(21,l_info.path().length());
        fpn[i].append(fPath[i]+"/"+fName[i]);

        //qDebug()<<fPath[i];

        for(int j=0;j<fPath[i].length();j++)
        {
            QChar mask=fPath[i].at(j);
            if(mask=='/')
            {
                count++;
            }
        }
        if(count>=temp)
        {
            temp=count;
        }
    }
    count=temp+1;
    //qDebug()<<count;
    int*pos=new int[1000];

    for(int j=0;j<ui->resultListWidget->count();j++)
    {
        for(int i=1;i<=count;i++)//
        {
            switch (i)
            {
            case 1:pos[i-1+count*j]=fpn[j].indexOf("/",0);
                break;
            case 2:pos[i-1+count*j]=fpn[j].indexOf("/",fpn[j].indexOf("/",0)+1);
                break;
            case 3:pos[i-1+count*j]=fpn[j].indexOf("/",fpn[j].indexOf("/",fpn[j].indexOf("/",0)+1)+1);
                break;
            case 4:pos[i-1+count*j]=fpn[j].indexOf("/",fpn[j].indexOf("/",fpn[j].indexOf("/",fpn[j].indexOf("/",0)+1)+1)+1);
                break;
            case 6:qDebug()<<fpn[j].indexOf("/",0);
                break;
            case 7:qDebug()<<fpn[j].indexOf("/",0);
                break;
            case 8:qDebug()<<fpn[j].indexOf("/",0);
                break;
            case 9:qDebug()<<fpn[j].indexOf("/",0);
                break;
            case 10:qDebug()<<fpn[j].indexOf("/",0);
                break;
            default :qDebug()<<fpn[j];
            }
        }

    }

    for(int i=0;i<count*(ui->resultListWidget->count());i++)
    {
        //qDebug()<<pos[i];
        daiPos[i]=pos[i];
    }

}

//递归获取文件路劲
QFileInfoList
Widget::GetAllFileLst(QString path)
{
    QDir dir(path);
    QFileInfoList file_list =dir.entryInfoList(QDir::Files|QDir::Hidden|QDir::NoSymLinks);
     QFileInfoList folder_list=dir.entryInfoList(QDir::Dirs|QDir::NoDotAndDotDot);

     for(int i=0;i!=folder_list.size();i++)
     {
         QString name=folder_list.at(i).absoluteFilePath();
         QFileInfoList child_file_list=GetAllFileLst(name);
         file_list.append(child_file_list);
     }
    return file_list;
}

//根据文件路径读取文件内容
QString
Widget::ReadtestFile(QString str)//单个文件里面的所有内容
{
   QFile file(str);//文件路径 也可以用相对路径
   if(!file.open(QIODevice::ReadOnly|QIODevice::Text))
   {
      QMessageBox::about(NULL,"文件","文件打开失败");

   }
   QTextStream in(&file);//文件流
   QString line=in.readLine();//读取一行，文件里面一行的内容
    QString *fileCon=new QString[1000];//假设一个文件里面代码最多有1000行
    int i=0;
   while(!line.isNull())
   {
       fileCon[i]=line;
       i++;
       line=in.readLine();      
   }
   QString filecontent;//单个文件里面的所有内容
   for(int j=0;j<i;j++)//fileCon
   {
       filecontent.append(fileCon[j]);
   }
   file.close();
   return filecontent;
}

//产生MD5
QString
Widget::productMD5(QString str)
{
    QByteArray ba=str.toLatin1();
    char*str1=ba.data();
    QString strrr=QString::fromLocal8Bit(str1);
    QByteArray bytePwd=strrr.toUtf8();
    QByteArray bytePwdMd5=QCryptographicHash::hash(bytePwd,QCryptographicHash::Md5);
    QString strPwdMd5=bytePwdMd5.toHex();//32字节的MD5码
    return strPwdMd5;
}

//连接数据库
void Widget::connectDB()
{
    //添加数据库
    QString driver="QSQLITE";
    QString userName="root";
    QString password="123456";

    //qDebug()<<dName;

    //保存程序当前路径
    QString currentDir = QDir::currentPath();

    QString dir_str=currentDir+"/db";
    QDir dir;
    if(!dir.exists(dir_str))
    {
        bool res=dir.mkpath(dir_str);
        if(!res)
        {
            QMessageBox::about(NULL,"新建数据库文件夹","失败");
        }
    }

    //生成pdf保存的路径
    QString file_path = dir_str;


    //将程序的执行路径设置到filePath下
    QDir tempDir;
    tempDir.setCurrent(file_path);

    QString DBName=dName+".db";

    QSqlDatabase dbs=QSqlDatabase::addDatabase(driver);
    dbs.setUserName(userName);
    dbs.setPassword(password);
    dbs.setDatabaseName(DBName);

    //打开数据库
    if(!dbs.open())
    {
        QMessageBox::warning(this,"数据库打开失败",dbs.lastError().text());
        qDebug()<<dbs.lastError()<<endl;
        return;
    }
    else
    {
        db=dbs;
    }

    //tempDir.setCurrent(currentDir);

}

//创建数据表

//产品表
void Widget::createProductTable()
{
    QSqlQuery query;
    QString sql="create table if not exists product (pro_id int auto_increment primary key,pro_num varchar(255),pro_name varchar(255))";
    query.prepare(sql);
    if(!query.exec())
    {
        QMessageBox::about(NULL,"建product表失败",query.lastError().driverText());
    }
}

//单元表
void Widget::createUnitTable()
{
    QSqlQuery query;
    QString sql="create table if not exists Unit (unit_id int auto_increment primary key,pro_unit varchar(255),unit_num varchar(255),unit_name varchar(255),pro_id int,foreign key(pro_id)"
                " references product(pro_id) on delete cascade on update cascade)";
    query.prepare(sql);
    if(!query.exec())
    {
        QMessageBox::about(NULL,"建unit表失败",query.lastError().driverText());
    }
}

//版本表
void Widget::createVersionTable()
{
    QSqlQuery query;
    QString sql="create table if not exists Version (v_id int auto_increment primary key,pro_unit_v varchar(255),v_name varchar(255),v_label varchar(255),v_state varchar(255),pro_id int,"
                "unit_id int,Adef varchar(255),Bdef varchar(255),Cdef varchar(255),Ddef varchar(255)"
                ",foreign key(pro_id) references product(pro_id) on delete cascade on update cascade,foreign key(unit_id) references Unit(unit_id) on delete cascade on update cascade)";
    query.prepare(sql);
    if(!query.exec())
    {
        QMessageBox::about(NULL,"建version表失败",query.lastError().driverText());
    }
}

//文件表
void Widget::createLittleFileTable()
{
    QSqlQuery query;
    QString sql="create table if not exists File (file_id int auto_increment primary key ,file_name varchar(255),file_label varchar(255),file_path varchar(255),v_id int,"
                "foreign key(v_id) references Version(v_id) on delete cascade on update cascade)";
    query.prepare(sql);
    if(!query.exec())
    {
        QMessageBox::about(NULL,"建file表失败",query.lastError().driverText());
    }
}

//产生pro_unit
QString Widget::generateProUnit()
{
    QString Pid;
    int proId;
    int unit;
    int unitId;
    QString proUnit;
    if(Widget::db.open())
    {
        QSqlQuery query1;
        query1.prepare("select pro_id from product where pro_num=?");
        query1.addBindValue(ui->ProIdlineEdit->text());
        query1.exec();
        while(query1.next())
        {
            Pid=query1.value(0).toString();
        }
        if(Pid.isNull())
        {
            proId=0;
        }
        else
        {
            proId=Pid.toInt();
        }
        //为新产品
        if(Pid.isNull())//产品没有在数据表里才能插入或产品表为空（判断是否为新产品）
        {
            QString PlastId;
            int proIdlast;
            QString str="select pro_id from product";
            QSqlQuery query1;
            query1.prepare(str);
            query1.exec();
            while(query1.next())
            {
                PlastId=query1.value(0).toString();
            }
            if(PlastId.isNull())
            {
                proIdlast=0;
            }
            else
            {
                proIdlast=Pid.toInt();
            }
            proId=proIdlast+1;
            QString strUid;
            str="select unit_id from Unit";
            query1.prepare(str);
            query1.exec();
            while(query1.next())
            {
                strUid=query1.value(0).toString();
            }
            if(strUid.isNull())
            {
               unitId=0;
            }
            else
            {
                unitId=strUid.toInt();
            }
            if(proId==1)
            {
                unit=unitId+1;
            }
            else if(proId>1)
                {
                        unit=1;
                }
        }
        else//不是新产品
        {
            QString strUid;
            QString str="select unit_id from Unit";
            QSqlQuery query1;
            query1.prepare(str);
            query1.exec();
            while(query1.next())
            {
                strUid=query1.value(0).toString();
            }
            if(strUid.isNull())
            {
               unitId=0;
            }
            else
            {
                unitId=strUid.toInt();
            }
            if(proId==1)
            {
                unit=unitId+1;
                //qDebug()<<unitId;
            }
            else if(proId>1)
                {
                    int unitIdlast;
                    QString un;
                    QString str="select unit_id from Unit where pro_id=?";
                    QSqlQuery query1;
                    query1.prepare(str);
                    query1.addBindValue(proId-1);
                    query1.exec();
                    while(query1.next())
                    {
                        un=query1.value(0).toString();
                    }
                    if(un.isNull())
                    {
                       unitIdlast=0;
                    }
                    else
                    {
                        unitIdlast=un.toInt();
                    }
                        unit=unitId-unitIdlast+1;
                }
        }
    }
    Widget::db.close();
    proUnit.append(QString::number(proId));
    proUnit.append("-");
    proUnit.append(QString::number(unit));
    proId=0;//防止啦新的unit查询出unit_id为1
    return proUnit;
}

//产生pro_unit_v
QString Widget::generateProUnitVer()
{
    QString proUnitVer;
    int v;
    QString proUnit;
    if(Widget::db.open())
    {
        QSqlQuery query1;
        query1.prepare("select pro_unit,unit_id from Unit where unit_num=?");
        query1.addBindValue(ui->UnitIdlineEdit->text());
        query1.exec();
        while(query1.next())
        {
            proUnit=query1.value(0).toString();
        }
    }
    Widget::db.close();
    v=ui->VerNamelineEdit->text().mid(6,ui->VerNamelineEdit->text().length()).toInt();
    v++;
   proUnitVer.append(proUnit);
   proUnitVer.append("-");
   proUnitVer.append(QString::number(v));
   return proUnitVer;
}

//插入数据表

//插入数据到product数据表
void Widget::intoProductData()
{
    boolean bol=false;
    if(Widget::db.open())
    {
        QString Pid;
        QSqlQuery query1;
        query1.prepare("select pro_id from product where pro_num=?");
        query1.addBindValue(ui->ProIdlineEdit->text());
        query1.exec();
        while(query1.next())
        {
            Pid=query1.value(0).toString();
        }
        if(Pid.isNull())//产品没有在数据表里才能插入或产品表为空（判断是否为新产品）
        {
               QString Pid;
               int proId;
               QString str="select pro_id from product";
               QSqlQuery query1;
               query1.prepare(str);
               query1.exec();
               while(query1.next())
               {
                   Pid=query1.value(0).toString();
               }
               if(Pid.isNull())
               {
                   proId=0;
               }
               else
               {
                   proId=Pid.toInt();
               }
                   int i=proId+1;//防止主键的重复
                   QSqlQuery query;
                   query.prepare("insert into product (pro_id,pro_num,pro_name) values (?, ?, ?)");
                   query.addBindValue(i);
                   query.addBindValue(ui->ProIdlineEdit->text());//pro_num
                   query.addBindValue(ui->ProNamelineEdit->text());//pro_name
                   bol=query.exec();
                   if(!bol)
                   {
                        QSqlError lastError=query.lastError();
                        QMessageBox::about(NULL,"插入失败",lastError.driverText());
                       //qDebug()<<"query error:"<<query.lastError();
                   }
                   i=0;
        }
    }
    Widget::db.close();
    boln=bol;
}

//插入数据到Unit数据表
void Widget::intoUnitData()
{
    boolean bol=false;
        QString Uid;
        if(Widget::db.open())
        {
            QSqlQuery query1;
            query1.prepare("select unit_id from Unit where unit_num=?");
            query1.addBindValue(ui->UnitIdlineEdit->text());
            query1.exec();
            while(query1.next())
            {
                Uid=query1.value(0).toString();
            }
        }
        Widget::db.close();
        if(Uid.isNull())
        {
            QString Pid;
            int proId;
            if(Widget::db.open())
            {
                QSqlQuery query2;
                QString sql=QString("select pro_id from product where pro_num=?");
                query2.prepare(sql);
                query2.addBindValue(ui->ProIdlineEdit->text());
                query2.exec();
                while(query2.next())
                {
                    Pid=query2.value(0).toString();
                }                
                if(Pid.isNull())
                {
                    proId=0;
                }
                else
                {
                    proId=Pid.toInt();
                }
            }
            Widget::db.close();
                QString Uid;
                int unitId;
                if(Widget::db.open())
                {
                    QString str="select unit_id from Unit";//防止unit_id的不衔接
                    QSqlQuery query3;
                    query3.prepare(str);
                    query3.exec();
                    while(query3.next())
                    {
                        Uid=query3.value(0).toString();
                    }                    
                    if(Uid.isNull())
                    {
                        unitId=0;
                    }
                    else
                    {
                        unitId=Uid.toInt();
                    }
                }
                Widget::db.close();
                int i=unitId+1;
                QString prounit=Widget::generateProUnit();
                if(Widget::db.open())
                {
                    QSqlQuery query4;
                    QString str4="insert into Unit (unit_id,pro_unit,unit_num,unit_name,pro_id) values (?, ?, ?, ?, ?)";
                    query4.prepare(str4);
                    query4.addBindValue(QString::number(i));
                    query4.addBindValue(prounit);//pro_unit
                    query4.addBindValue(ui->UnitIdlineEdit->text());//unit_num
                    query4.addBindValue(ui->UnitNamelineEdit->text());//unit_name
                    query4.addBindValue(QString::number(proId));//pro_id
                    bol=query4.exec();
                    if(!bol)
                    {
                         QSqlError lastError=query4.lastError();
                         QMessageBox::about(NULL,"插入unit失败",lastError.driverText());
                        //qDebug()<<"query error:"<<query4.lastError();
                    }
                    i=0;
                }
                Widget::db.close();
        }
        boln=bol;
}

//插入数据到Version数据表
void Widget::intoVersionData()
{
    boolean bol=false;
    QString proUnitVer;
    QString vLabel;
    QString allfilecontent;//整个版本文件里面的所有内容
    for( int i=0;i<ui->VerFilelistWidget->count();i++)
    {
     allfilecontent.append(Widget::ReadtestFile(filePath[i]));
    }
    vLabel=Widget::productMD5(allfilecontent);
    QString Verid;
    if(Widget::db.open())//根据v_label获取v_id
    {
        QSqlQuery query;
        QString sql=QString("select v_id from Version where v_label=?");
        query.prepare(sql);
        query.addBindValue(vLabel);
        query.exec();
        while(query.next())
        {
            Verid=query.value(0).toString();
        }
    }
    Widget::db.close();
    if(Verid.isNull())//v_id为空进行插入操作
    {
        int proId;
        if(Widget::db.open())//获取pro_id
        {
            QSqlQuery query;
            QString sql=QString("select pro_id from product where pro_num=?");
            query.prepare(sql);
            query.addBindValue(ui->ProIdlineEdit->text().toInt());
            query.exec();

            while(query.next())
            {
                proId=query.value(0).toInt();
            }
        }
        Widget::db.close();
        int unitId;
        if(Widget::db.open())//获取unit_id
        {
            QSqlQuery query;
            QString sql=QString("select unit_id from Unit where unit_num=?");
            query.prepare(sql);
            query.addBindValue(ui->UnitIdlineEdit->text());
            query.exec();

            while(query.next())
            {
                unitId=query.value(0).toInt();

            }
        }
        Widget::db.close();
        proUnitVer=Widget::generateProUnitVer();
         if(Widget::db.open())
        {
            QString Vid;
            int vId;
            QString str="select v_id from Version";//防止v_id不连续
            QSqlQuery query1;
            query1.prepare(str);
            query1.exec();
            while(query1.next())
            {
                Vid=query1.value(0).toString();
            }
            if(Vid.isNull())
            {
                vId=0;
            }
            else
            {
                vId=Vid.toInt();
            }
                int i=vId+1;
                    QSqlQuery query;
                    query.prepare("insert into Version (v_id,pro_unit_v,v_name,v_label,v_state,pro_id,unit_id,Adef,Bdef,Cdef,Ddef) values (?, ?, ?, ?, ?, ?, ?,?,?,?,?)");
                    query.addBindValue(i);
                    query.addBindValue(proUnitVer);
                    query.addBindValue(ui->VerNamelineEdit->text());//v_name
                    query.addBindValue(vLabel);//v_label
                    query.addBindValue(ui->VerStatelineEdit->text());//v_state
                    query.addBindValue(proId);
                    query.addBindValue(unitId);
                    query.addBindValue(ui->AlineEdit->text());
                    query.addBindValue(ui->BlineEdit->text());
                    query.addBindValue(ui->ClineEdit->text());
                    query.addBindValue(ui->DlineEdit->text());
                    bol=query.exec();
                    if(!bol)
                    {
                         QSqlError lastError=query.lastError();
                         QMessageBox::about(NULL,"插入失败",lastError.driverText());
                         //qDebug()<<"query error:"<<query.lastError();
                }
                i=0;
        }
        Widget::db.close();
    }
    boln=bol;
}

//插入数据到File数据表
void Widget::intoLittleFileData()
{



    boln=false;
    boolean bol=false;
    QString *fileContent=new QString[1000];
    QString*fileLabel=new QString[1000];
    QString*fileName=new QString[1000];
    QString*filepPath=new QString[1000];
    for( int i=0;i<ui->VerFilelistWidget->count();i++)
    {      
        fileContent[i]=Widget::ReadtestFile(filePath[i]);

        //qDebug()<<filePath[i];

        fileLabel[i]=Widget::productMD5(fileContent[i]);
        QFileInfo l_info(filePath[i]);
        fileName[i]=l_info.fileName();
        //QString strpath="C:/Users/Administrator/Desktop/";
        //filepPath[i]=l_info.path().mid(31,l_info.path().length());

        //qDebug()<<filepPath[i];

    }
    QString Verid;
    int vId;
    if(Widget::db.open())//获取v_id
    {
        QSqlQuery query;
        QString sql=QString("select v_id from Version");
        query.prepare(sql);
        query.exec();
        while(query.next())
        {
            Verid=query.value(0).toString();
        }
        //if(Verid.isNull())
        vId=Verid.toInt();
    }
    Widget::db.close();
    for( int i=0;i<ui->VerFilelistWidget->count();i++)
    {
        if(Widget::db.open())
        {
            QString Fileid;
            int fId;
            QString str="select file_id from File";
            QSqlQuery query1;
            query1.prepare(str);
            query1.exec();
            while(query1.next())
            {
                Fileid=query1.value(0).toString();
            }
            if(Fileid.isNull())
            {
                fId=0;
            }
            else
            {
                fId=Fileid.toInt();
            }
                int j=fId+1;//防止主键的重复
                QSqlQuery query;
                query.prepare("insert into File (file_id,file_name,file_label,file_path,v_id) values (?, ?, ?, ?, ?)");
                query.addBindValue(j);//file_id                
                query.addBindValue(fileName[i]);//file_name                
                query.addBindValue(fileLabel[i]);//file_label                
                query.addBindValue(filePath[i]);//file_path
                query.addBindValue(vId);//v_id
                bol=query.exec();
                if(!bol)
                {
                     QSqlError lastError=query.lastError();
                     QMessageBox::about(NULL,"插入失败",lastError.driverText());
                     //qDebug()<<"query error:"<<query.lastError();
                }               
        }
        Widget::db.close();
    }
    boln=bol;
}

//更新数据表

//产品表
void Widget::updateProductData()
{
    boln=false;
    boolean bn;
    int proId=proid;
    if(Widget::db.open())
    {
        QSqlQuery query1;
        query1.prepare("update product set pro_num=? ,pro_name=? where pro_id=?");
        query1.addBindValue((ui->ProIdlineEdit->text()));//string转int
        query1.addBindValue(ui->ProNamelineEdit->text());
        query1.addBindValue(proId);
        bn=query1.exec();
        if(!bn)
        {
             QSqlError lastError=query1.lastError();
             QMessageBox::about(NULL,"插入失败",lastError.driverText());
             //qDebug()<<"query error:"<<query1.lastError();
        }       

    }
    Widget::db.close();
    boln=bn;
}

//单元表
void Widget::updateUnitData()
{
    boln=false;
    boolean bn;
    int unitId=unitid;
    int proId=proid;    
    if(Widget::db.open())
    {        
        QSqlQuery query1;
        query1.prepare("update Unit set unit_num=? ,unit_name=? where unit_id=? and pro_id=?");
        query1.addBindValue(ui->UnitIdlineEdit->text());//string转int
        query1.addBindValue(ui->UnitNamelineEdit->text());
        query1.addBindValue(unitId);
        query1.addBindValue(proId);
        bn=query1.exec();
        if(!bn)
        {
             QSqlError lastError=query1.lastError();
             QMessageBox::about(NULL,"插入失败",lastError.driverText());
             //qDebug()<<"query error:"<<query1.lastError();
        }
    }
    Widget::db.close();
    boln=bn;
}

//版本表
void Widget::updateVersionData()
{
    boln=false;
    boolean bn;
    int vId=vid;
    QString vLabel;
    QString allfilecontent;//整个版本文件里面的所有内容
    for( int i=0;i<ui->VerFilelistWidget->count();i++)
    {
     allfilecontent.append(Widget::ReadtestFile(filePath[i]));
    }
    vLabel=Widget::productMD5(allfilecontent);
    if(Widget::db.open())
    {
        QSqlQuery query1;
        query1.prepare("update Version set v_name=? ,v_label=? ,v_state=?,Adef=?,Bdef=?,Cdef=?,Ddef=? where v_id=?");
        query1.addBindValue(ui->VerNamelineEdit->text());//string转int
        query1.addBindValue(vLabel);//v_label
        query1.addBindValue(ui->VerStatelineEdit->text());

        query1.addBindValue(ui->AlineEdit->text());
        query1.addBindValue(ui->BlineEdit->text());
        query1.addBindValue(ui->ClineEdit->text());
        query1.addBindValue(ui->DlineEdit->text());

        query1.addBindValue(vId);
        bn=query1.exec();
        if(!bn)
        {
             QSqlError lastError=query1.lastError();
             QMessageBox::about(NULL,"插入失败",lastError.driverText());
             //qDebug()<<"query error:"<<query1.lastError();
        }
    }
    Widget::db.close();
    boln=bn;
}

//文件表
void Widget::updateFileData()
{

    int vd=vid;
    //qDebug()<<vd;

    if(Widget::db.open())//获取v_id
    {
        QSqlQuery query;
        QString sql=QString("DELETE FROM File WHERE v_id=?");
        query.prepare(sql);
        query.addBindValue(vd);
        query.exec();
    }
    Widget::db.close();




    //boln=false;
    boolean bol=false;
    QString *fileContent=new QString[1000];
    QString*fileLabel=new QString[1000];
    QString*fileName=new QString[1000];
    QString*filepPath=new QString[1000];
    for( int i=0;i<ui->VerFilelistWidget->count();i++)
    {
        fileContent[i]=Widget::ReadtestFile(filePath[i]);
        fileLabel[i]=Widget::productMD5(fileContent[i]);
        QFileInfo l_info(filePath[i]);
        fileName[i]=l_info.fileName();
        //QString strpath="C:/Users/Administrator/Desktop/";
        filepPath[i]=l_info.path().mid(31,l_info.path().length());
    }

    for( int i=0;i<ui->VerFilelistWidget->count();i++)
    {
        if(Widget::db.open())
        {
            QString Fileid;
            int fId;
            QString str="select file_id from File";
            QSqlQuery query1;
            query1.prepare(str);
            query1.exec();
            while(query1.next())
            {
                Fileid=query1.value(0).toString();
            }
            if(Fileid.isNull())
            {
                fId=0;
            }
            else
            {
                fId=Fileid.toInt();
            }
                int j=fId+1;//防止主键的重复
                QSqlQuery query;
                query.prepare("insert into File (file_id,file_name,file_label,file_path,v_id) values (?, ?, ?, ?, ?)");
                query.addBindValue(j);//file_id
                query.addBindValue(fileName[i]);//file_name
                query.addBindValue(fileLabel[i]);//file_label
                query.addBindValue(filepPath[i]);//file_path
                query.addBindValue(vd);//v_id
                bol=query.exec();
                if(!bol)
                {
                     QSqlError lastError=query.lastError();
                     QMessageBox::about(NULL,"插入失败",lastError.driverText());
                     //qDebug()<<"query error:"<<query.lastError();
                }
        }
        Widget::db.close();
    }
    //boln=bol;






    vid=0;
}

//显示结果
void Widget::initShowProTree()
{   
    QStandardItemModel* model = new QStandardItemModel(ui->ProtreeView);
    model->setHorizontalHeaderLabels(QStringList()<<QStringLiteral("产品单元名")<<QStringLiteral("状态")<<QStringLiteral("标签"));//treeView的头标签
    //查询产品和代号
    QString*proNum=new QString[20];
    QString*pro=new QString[20];

    //查询单元和代号
    QString*unitNum=new QString[20];
    QString*unit=new QString[20];

    int i=0;//i即为总的产品数
    if(Widget::db.open())//查询出所有的产品名和代号
    {
        QString str="select pro_name,pro_num from product";
        QSqlQuery query;
        query.prepare(str);
        query.exec();
        while(query.next())
        {
            pro[i]=query.value(0).toString();
            proNum[i]=query.value(1).toString();
            i++;
        }
    }
    Widget::db.close();  
    for(int j=0;j<i;j++)//产品的操作 i是产品总数
    {        
        QString str=pro[j];
        QString m=proNum[j];//产品代号
        QString pan;
        //pan.append(str);
        //pan.append("和");
        //pan.append(m);
        pan.append("【"+m+"】"+str);
        QStandardItem* itemProject = new QStandardItem(m_publicIconMap[QStringLiteral("treeItem_Project")],QStringLiteral(" %1").arg(pan));//产品和代号
        model->appendRow(itemProject);
        QStandardItem* itemFolder;


        int pId=j+1;
        int sumCountUnit=0;//产品pId的总的单元数
        if(Widget::db.open())//查询单元代号和名称
        {            
            QString str="SELECT unit_num,unit_name FROM Unit WHERE  pro_id =?";
            QSqlQuery query;
            query.prepare(str);
            query.addBindValue(pId);
            query.exec();
            int i=0;
            while(query.next())
            {

                sumCountUnit++;
                unitNum[i]=query.value(0).toString();                
                unit[i]=query.value(1).toString();                
                i++;                
            }
        }
        Widget::db.close();
        for(int k=0;k<sumCountUnit;k++)//每一个产品下的单元操作
        {            
            QString un;
            QString uni=unit[k];            
            QString unin=unitNum[k];            
            //un.append(uni);
            //un.append("和");
            //un.append(unin);
            un.append("【"+unin+"】"+uni);
            itemFolder = new QStandardItem(m_publicIconMap[QStringLiteral("treeItem_folder")],QStringLiteral("%1").arg(un));
            itemProject->appendRow(itemFolder);

            int uId=k+1;//单元uId

            QString proUn;
            proUn.append(QString::number(pId));
            proUn.append("-");
            proUn.append(QString::number(uId));


            int*vId=new int[20];
                        int sumVerCount=0;
                        if(Widget::db.open())//查询出所有的产品名和代号
                        {
                            QString str="select v_id from Version where unit_id=(select unit_id from Unit where pro_unit=?)";
                            QSqlQuery query;
                            query.prepare(str);
                            query.addBindValue(proUn);
                            query.exec();
                            int i=0;
                            while(query.next())
                            {
                                vId[i]=query.value(0).toInt();                                
                                i++;
                                sumVerCount++;
                            }
                        }
                        Widget::db.close();


                        QString*verName=new QString[20];                        
                        QString*verLabel=new QString[20];
                        QString*verState=new QString[20];                        
                        for(int j=0;j<sumVerCount;++j)
                        {

                            if(Widget::db.open())//查询出所有的产品名和代号
                            {
                                int v=0;
                                QString str="select v_name,v_label,v_state from Version where v_id=?";
                                QSqlQuery query;
                                query.prepare(str);
                                v=vId[j];                                
                                query.addBindValue(v);
                                boolean bol=query.exec();
                                if(!bol)
                                {
                                     QSqlError lastError=query.lastError();
                                     QMessageBox::about(NULL,"失败",lastError.driverText());
                                     qDebug()<<"query error:"<<query.lastError();
                                }
                                while(query.next())
                                {   verName[j]=query.value(0).toString();                                    
                                    verLabel[j]=query.value(1).toString();                                    
                                    verState[j]=query.value(2).toString();                                    
                                }
                            }
                            Widget::db.close();
                        }
            //版本状态和标签
            for(int j=0;j<sumVerCount;++j)
            {
                 QString strVN=verName[j];
                 //qDebug()<<strVN;
                 QStandardItem* item = new QStandardItem(m_publicIconMap[QStringLiteral("treeItem_folder")],QStringLiteral("%1").arg(strVN));
                 itemFolder->appendRow(item);
                 itemFolder->setChild(item->index().row(),1,new QStandardItem(QStringLiteral("%1").arg(verState[j])));
                 itemFolder->setChild(item->index().row(),2,new QStandardItem(QStringLiteral("%1").arg(verLabel[j])));
            }
        }
        ui->ProtreeView->setModel(model);        
        ui->VertreeView->setAutoFillBackground(true);
    }


}

void Widget::initShowVerTree(QString str1,QString str2)
{
    QStandardItemModel* model = new QStandardItemModel(ui->VertreeView);
    model->setHorizontalHeaderLabels(QStringList()<<QStringLiteral("版本文件名")
                                     <<QStringLiteral("标签"));//treeView的头标签

    QStandardItem* itemProject = new QStandardItem(m_publicIconMap[QStringLiteral("treeItem_Project")],QStringLiteral("%1").arg(str1));    
    QString verLabel=str2;
    int verId;

    if(Widget::db.open())//查询出所有的产品名和代号
    {
        QString str="select v_id from Version where v_label=?";
        QSqlQuery query;
        query.prepare(str);
        query.addBindValue(verLabel);
        query.exec();
        while(query.next())
        {
            verId=query.value(0).toInt();
        }
    }
    Widget::db.close();

    QString*fPath=new QString[100];
    QString*fName=new QString[100];
    QString*fLabel=new QString[100];
    int i=0;
    if(Widget::db.open())//
    {

        QString str="select file_path,file_name,file_label from File where v_id=?";
        QSqlQuery query;
        query.prepare(str);
        query.addBindValue(verId);
        query.exec();
        while(query.next())
        {
            fPath[i]=query.value(0).toString();
            fName[i]=query.value(1).toString();
            fLabel[i]=query.value(2).toString();
            i++;

        }
    }
    Widget::db.close();



    model->appendRow(itemProject);
    QStandardItem* item;
    for(int j =0;j<i;++j)
    {
        QString fpn="";
        fpn.append(fPath[j]);
        fpn.append("/");
        fpn.append(fName[j]);
        item = new QStandardItem(m_publicIconMap[QStringLiteral("treeItem_dataItem")],QStringLiteral("%1").arg(fpn));
        itemProject->appendRow(item);
        itemProject->setChild(item->index().row(),1,new QStandardItem(QStringLiteral("%1").arg(fLabel[j])));
    }
    ui->VertreeView->setModel(model);
    ui->VertreeView->setAutoFillBackground(true);
}

void Widget::on_ProtreeView_clicked(const QModelIndex &index)
{
    QString proName,unitName,verName,label,proDai;
    QString adef,bdef,cdef,ddef;
    QString state;
    int fileNum,verId;
    //QString str;
    QString name,info,info1;
    if(index.column() == 0)
    {
        name = index.data().toString();
        info = index.sibling(index.row(),1).data().toString();
        info1 = index.sibling(index.row(),2).data().toString();//标签
    }
    else
    {
            if(index.column() == 1)
            {
                name = index.sibling(index.row(),0).data().toString();
                info = index.data().toString();
                info1=index.sibling(index.row(),2).data().toString();
            }
            else
            {
                name = index.sibling(index.row(),0).data().toString();
                info = index.sibling(index.row(),1).data().toString();
                info1=index.data().toString();
            }
    }
    QStandardItemModel* model = static_cast<QStandardItemModel*>(ui->ProtreeView->model());
    QModelIndex currentIndex = ui->ProtreeView->currentIndex();
    QStandardItem* currentItem = model->itemFromIndex(currentIndex);
    //ui->label_2->setText(getTopParent(currentItem)->text());
    //qDebug()<<getTopParent(currentItem)->text();
    //qDebug()<<index.parent().data().toString();
    if(!(info1.isNull()))
    {

        ui->widget->installEventFilter(this);

        ui->VerNamelineEdit->setText(name);
        ui->VerStatelineEdit->setText(info);

        verName=name;

        QString name0=getTopParent(currentItem)->text();
        int i=name0.indexOf("】");
        ui->ProIdlineEdit->setText(name0.mid(2,i-2));
        ui->ProNamelineEdit->setText(name0.mid(i+1,name0.length()));//name0.mid(i+1,name0.length())

        proName=name0.mid(i+1,name0.length());
        proDai=name0.mid(2,i-2);

        QString name1=index.parent().data().toString();
        i=name1.indexOf("】");
        ui->UnitIdlineEdit->setText(name1.mid(1,i-1));//name1.mid(0,i)
        ui->UnitNamelineEdit->setText(name1.mid(i+1,name1.length()));

        unitName=name1.mid(i+1,name1.length());

        ui->tabWidget_2->setCurrentIndex(2);
        Widget::initShowVerTree(name,info1);

        label=info1;


        if(Widget::db.open())//查询出所有的产品名和代号
        {
            QString str="select v_id,Adef,Bdef,Cdef,Ddef,v_state from Version where v_label=?";
            QSqlQuery query;
            query.prepare(str);
            query.addBindValue(label);
            query.exec();
            while(query.next())
            {
                verId=query.value(0).toInt();
                adef=query.value(1).toString();
                bdef=query.value(2).toString();
                cdef=query.value(3).toString();
                ddef=query.value(4).toString();
                state=query.value(5).toString();
            }
        }
        Widget::db.close();


        ui->AlineEdit->setText(adef);
        ui->BlineEdit->setText(bdef);
        ui->ClineEdit->setText(cdef);
        ui->DlineEdit->setText(ddef);


        qDebug()<<verId;

        int j=0;
        if(Widget::db.open())//
        {

            QString str="select file_label from File where v_id=?";
            QSqlQuery query;
            query.prepare(str);
            query.addBindValue(verId);
            query.exec();
            while(query.next())
            {
                j++;

            }
        }
        fileNum=j;
        Widget::db.close();
        //QString proName,unitName,verName,fileNum,label,verId;
        ProNamel=proName;
        UnitNamel=unitName;
        VerNamel=verName;
        FileNuml=fileNum;
        Labell=label;
        ProDail=proDai;
        Adefl=adef;
        Bdefl=bdef;
        Cdefl=cdef;
        Ddefl=ddef;
        Statel=state;
        qDebug()<<ProNamel<<UnitNamel<<VerNamel<<FileNuml<<Labell<<j;




    }
    else
    {
        if(getTopParent(currentItem)->text()==index.parent().data().toString())
        {
            ui->VerStatelineEdit->clear();
            ui->VerNamelineEdit->clear();
            QString name0=getTopParent(currentItem)->text();
            int i=name0.indexOf("】");
            ui->ProIdlineEdit->setText(name0.mid(2,i-2));//name0.mid(1,i-1)
            ui->ProNamelineEdit->setText(name0.mid(i+1,name0.length()));

            i=name.indexOf("】");
            ui->UnitIdlineEdit->setText(name.mid(1,i-1));
            ui->UnitNamelineEdit->setText(name.mid(i+1,name.length()));//name.mid(i+1,name.length())

            ui->tabWidget_2->setCurrentIndex(1);
        }
        else
        {
            ui->VerNamelineEdit->clear();
            ui->VerStatelineEdit->clear();
            ui->UnitIdlineEdit->clear();
            ui->UnitNamelineEdit->clear();
            int i=name.indexOf("】");
            ui->ProIdlineEdit->setText(name.mid(2,i-2));
            ui->ProNamelineEdit->setText(name.mid(i+1,name.length()));//name.mid(i+1,name.length())
            ui->tabWidget_2->setCurrentIndex(0);

        }

    }



    int proId;
    if(Widget::db.open())//获取pro_id
    {
        QSqlQuery query;
        QString sql=QString("select pro_id from product where pro_num='%1'")
                .arg(ui->ProIdlineEdit->text());
        query.exec(sql);
        while(query.next())
        {
            proId=query.value(0).toInt();
        }
    }
    proid=proId;
    Widget::db.close();
    //qDebug()<<proid;

    int unitId;
    if(Widget::db.open())//获取unit_id
    {
        QSqlQuery query;
        QString sql=QString("select unit_id from Unit where unit_num='%1' and pro_id='%2'")
                .arg(ui->UnitIdlineEdit->text()).arg(QString::number(proId,10));
        query.exec(sql);

        while(query.next())
        {
            unitId=query.value(0).toInt();
        }
    }
    unitid=unitId;
    Widget::db.close();
    //qDebug()<<unitid;


    int vId;
    if(Widget::db.open())//获取v_id
    {
        QSqlQuery query;
        QString sql=QString("select v_id from Version where unit_id='%1' and pro_id='%2' and v_name='%3'")
                .arg(unitId).arg(QString::number(proId,10)).arg(ui->VerNamelineEdit->text());
        query.exec(sql);

        while(query.next())
        {
            vId=query.value(0).toInt();
        }
    }
    vid=vId;
    Widget::db.close();
    //qDebug()<<vid;

}

QStandardItem* Widget::getTopParent(QStandardItem* item)
{
    QStandardItem* secondItem = item;
    while(item->parent()!= 0)
    {
        secondItem = item->parent();
        item = secondItem;
    }
    if(secondItem->index().column() != 0)
    {
         QStandardItemModel* model = static_cast<QStandardItemModel*>(ui->ProtreeView->model());
         secondItem = model->itemFromIndex(secondItem->index().sibling(secondItem->index().row(),0));
    }
    return secondItem;
}
QModelIndex Widget::getTopParent(QModelIndex itemIndex)
{
    QModelIndex secondItem = itemIndex;
    while(itemIndex.parent().isValid())
    {
        secondItem = itemIndex.parent();
        itemIndex = secondItem;
    }
    if(secondItem.column() != 0)
    {
         secondItem = secondItem.sibling(secondItem.row(),0);
    }
    return secondItem;
}

//比较
void Widget::resultShowTree()
{
    QStandardItemModel* model = new QStandardItemModel(ui->ProtreeView);
    model->setHorizontalHeaderLabels(QStringList()<<QStringLiteral("产品单元名")<<QStringLiteral("标签")<<QStringLiteral("状态"));//treeView的头标签

    //查询产品和代号
    QString*proNum=new QString[20];
    QString*pro=new QString[20];

    //查询单元和代号
    QString*unitNum=new QString[20];
    QString*unit=new QString[20];

    int i=0;//i即为总的产品数
    if(Widget::db.open())//查询出所有的产品名和代号
    {
        QString str="select pro_name,pro_num from product";
        QSqlQuery query;
        query.prepare(str);
        query.exec();
        while(query.next())
        {
            pro[i]=query.value(0).toString();
            proNum[i]=query.value(1).toString();
            i++;
        }
    }
    Widget::db.close();    
    for(int j=0;j<i;j++)//产品的操作 i是产品总数
    {        
        QString str=pro[j];
        QString m=proNum[j];//产品代号
        QString pan;
//        pan.append(str);
//        pan.append("和");
//        pan.append(QString::number(m));

        pan.append("【"+m+"】"+str);

//        pan.append("【");//+m+"】"+str);
//        pan.append(m);
//        pan.append("】");
//        pan.append(str);
        QStandardItem* itemProject = new QStandardItem(m_publicIconMap[QStringLiteral("treeItem_Project")],QStringLiteral(" %1").arg(pan));//产品和代号
        model->appendRow(itemProject);
        QStandardItem* itemFolder;


        int pId=j+1;
        int sumCountUnit=0;//产品pId的总的单元数
        if(Widget::db.open())//查询单元代号和名称
        {            
            QString str="SELECT unit_num,unit_name FROM Unit WHERE  pro_id =?";
            QSqlQuery query;
            query.prepare(str);
            query.addBindValue(pId);
            query.exec();
            int i=0;
            while(query.next())
            {

                sumCountUnit++;
                unitNum[i]=query.value(0).toString();                
                unit[i]=query.value(1).toString();                
                i++;                
            }
        }
        Widget::db.close();
        for(int k=0;k<sumCountUnit;k++)//每一个产品下的单元操作
        {            
            QString un;
            QString uni=unit[k];            
            QString unin=unitNum[k];            
//            un.append(uni);
//            un.append("和");
//            un.append(unin);
            un.append("【"+unin+"】"+uni);
            itemFolder = new QStandardItem(m_publicIconMap[QStringLiteral("treeItem_folder")],QStringLiteral("%1").arg(un));
            itemProject->appendRow(itemFolder);

            int uId=k+1;//单元uId

            QString proUn;
            proUn.append(QString::number(pId));
            proUn.append("-");
            proUn.append(QString::number(uId));


            int*vId=new int[20];
                        int sumVerCount=0;
                        if(Widget::db.open())//查询出所有的产品名和代号
                        {
                            QString str="select v_id from Version where unit_id=(select unit_id from Unit where pro_unit=?)";
                            QSqlQuery query;
                            query.prepare(str);
                            query.addBindValue(proUn);
                            query.exec();
                            int i=0;
                            while(query.next())
                            {
                                vId[i]=query.value(0).toInt();                                
                                i++;
                                sumVerCount++;
                            }
                        }
                        Widget::db.close();


                        QString*verName=new QString[20];
                        QString*vLabel=new QString[20];
                        QString verLabel;
                        QString verState;
                        QString*vSta=new QString[20];
                        QString*sl=new QString[20];
                        for(int j=0;j<sumVerCount;++j)
                        {

                            if(Widget::db.open())//
                            {
                                int v=0;
                                QString str="select v_name,v_label,v_state from Version where v_id=?";
                                QSqlQuery query;
                                query.prepare(str);
                                v=vId[j];                                
                                query.addBindValue(v);
                                boolean bol=query.exec();
                                if(!bol)
                                {
                                     QSqlError lastError=query.lastError();
                                     QMessageBox::about(NULL,"失败",lastError.driverText());
                                     qDebug()<<"query error:"<<query.lastError();
                                }
                                while(query.next())
                                {   verName[j]=query.value(0).toString();                                    
                                    verLabel=query.value(1).toString();

                                    vLabel[j]=verLabel;

                                    verState=query.value(2).toString();
                                    vSta[j]=verState;                                    
                                    sl[j].append(verState);
                                    sl[j].append("和");
                                    sl[j].append(verLabel);                                    

                                }
                            }
                            Widget::db.close();
                        }
            //版本状态和标签
            for(int j=0;j<sumVerCount;++j)
            {
                QString strVN=verName[j];                

                QString verLabel=vLabel[j];

                QStandardItem* item = new QStandardItem(m_publicIconMap[QStringLiteral("treeItem_folder")],QStringLiteral("%1").arg(strVN));                 
                 itemFolder->appendRow(item);
                 itemFolder->setChild(item->index().row(),1,new QStandardItem(QStringLiteral("%1").arg(verLabel)));
                 itemFolder->setChild(item->index().row(),2,new QStandardItem(QStringLiteral("%1").arg(vSta[j])));
                 //文件和标签

                 int verId;
                 if(Widget::db.open())//
                 {
                     QString str="select v_id from Version where v_label=?";
                     QSqlQuery query;
                     query.prepare(str);
                     query.addBindValue(verLabel);
                     boolean bol=query.exec();
                     if(!bol)
                     {
                          QSqlError lastError=query.lastError();
                          QMessageBox::about(NULL,"失败",lastError.driverText());
                          qDebug()<<"query error:"<<query.lastError();
                     }
                     while(query.next())
                     {
                        verId=query.value(0).toInt();
                     }
                 }
                 Widget::db.close();

                 QString*fPath=new QString[100];
                 QString*fName=new QString[100];
                 QString*fLabel=new QString[100];
                 int i=0;
                 if(Widget::db.open())//
                 {

                     QString str="select file_path,file_name,file_label from File where v_id=?";
                     QSqlQuery query;
                     query.prepare(str);
                     query.addBindValue(verId);
                     query.exec();
                     while(query.next())
                     {
                         fPath[i]=query.value(0).toString();
                         fName[i]=query.value(1).toString();
                         fLabel[i]=query.value(2).toString();
                         i++;

                     }
                 }
                 Widget::db.close();

                 for(int j=0;j<i;++j)
                 {
                      QString fpn="";
                      fpn.append(fPath[j]);
                      fpn.append("/");
                      fpn.append(fName[j]);
                      QStandardItem* iitem = new QStandardItem(m_publicIconMap[QStringLiteral("treeItem_dataItem")],QStringLiteral("%1").arg(fpn));                      
                      item->appendRow(iitem);
                      item->setChild(iitem->index().row(),1,new QStandardItem(QStringLiteral("%1").arg(fLabel[j])));

                 }

            }
        }
        ui->CompareTreeView->setModel(model);
        ui->VertreeView->setAutoFillBackground(true);
    }
}

void Widget::compareVer(QString str2)
{
    QString back32;
    back32=str2.mid(str2.length()-32,str2.length());    
    QString verLabel=back32;

    QString productLabel;
    QString*littleProLa=new QString[1000];

    QString allfilecontent;//整个版本文件里面的所有内容
    for( int i=0;i<ui->resultListWidget->count();i++)
    {
     allfilecontent.append(Widget::ReadtestFile(resultFilePath[i]));
     littleProLa[i]=Widget::productMD5(Widget::ReadtestFile(ui->resultListWidget->item(i)->text()));     
    }
    productLabel=Widget::productMD5(allfilecontent);


    int verId;
    int pId;
    int uId;
    QString vName;
    if(Widget::db.open())//
    {
        QString str="select v_id,pro_id,unit_id,v_name from Version where v_label=?";
        QSqlQuery query;
        query.prepare(str);
        query.addBindValue(verLabel);
        query.exec();
        while(query.next())
        {
            verId=query.value(0).toInt();
            pId=query.value(1).toInt();
            uId=query.value(2).toInt();
            vName=query.value(3).toString();

        }
    }
    Widget::db.close();

    QString pNum;
    QString pName;

    if(Widget::db.open())//
    {
        QString str="select pro_num,pro_name from product where pro_id=?";
        QSqlQuery query;
        query.prepare(str);
        query.addBindValue(pId);
        query.exec();
        while(query.next())
        {
            pNum=query.value(0).toString();
            pName=query.value(1).toString();
        }
    }
    Widget::db.close();


    QString uNum;
    QString uName;
    if(Widget::db.open())//
    {
        QString str="select unit_num,unit_name from Unit where unit_id=?";
        QSqlQuery query;
        query.prepare(str);
        query.addBindValue(uId);
        query.exec();
        while(query.next())
        {
            uNum=query.value(0).toString();
            uName=query.value(1).toString();
        }
    }
    Widget::db.close();


    QString*fPath=new QString[100];
    QString*fName=new QString[100];
    QString*fLabel=new QString[100];
    QString*sofpn=new QString[100];
    QString*result=new QString[100];
    int i=0;//i是数据库里面的文件总数
    int fCount=0;
    if(Widget::db.open())//
    {
        QString str="select file_path,file_name,file_label from File where v_id=?";
        QSqlQuery query;
        query.prepare(str);
        query.addBindValue(verId);
        query.exec();
        while(query.next())
        {
            fPath[i]=query.value(0).toString();

            qDebug()<<fPath[i];

            fName[i]=query.value(1).toString();
            fLabel[i]=query.value(2).toString();
            sofpn[i].append(fPath[i]);
            //sofpn[i].append(fPath[i]+"/"+fName[i]);
            i++;
        }
    }

    fCount=i;

    Widget::db.close();

    QString source;
    source.append(pNum+"/"+pName+"/"+uNum+"/"+uName+"/"+vName);    
    QString current;
    current.append(pNum+"/"+pName+"/"+uNum+"/"+uName+"/"+"Vx.xx.xx");

    int row;
    int j=ui->resultListWidget->count();//拖进来的文件总数
    row=i>j?i:j;












    QString*fName1=new QString[100];
    int count1=0;
    QString*fPath1=new QString[100];
    QString*fpn1=new QString[100];
    int temp=0;
    for( int i=0;i<fCount;i++)
    {
        count1=0;
        QFileInfo l_info(fPath[i]);
        fName1[i]=l_info.fileName();
        fPath1[i]=l_info.path().mid(21,l_info.path().length());
        fpn1[i].append(fPath1[i]+"/"+fName1[i]);

        //qDebug()<<fPath[i];
        //qDebug()<<fPath1[i];

        for(int j=0;j<fPath1[i].length();j++)
        {
            QChar mask=fPath1[i].at(j);
            if(mask=='/')
            {
                count1++;
            }
        }
        if(count1>=temp)
        {
            temp=count1;
        }
    }
    count1=temp+1;
    //qDebug()<<count1;
    int*pos=new int[1000];

    for(int j=0;j<fCount;j++)
    {
        for(int i=1;i<=count1;i++)//
        {
            switch (i)
            {
            case 1:pos[i-1+count1*j]=fpn1[j].indexOf("/",0);
                break;
            case 2:pos[i-1+count1*j]=fpn1[j].indexOf("/",fpn1[j].indexOf("/",0)+1);
                break;
            case 3:pos[i-1+count1*j]=fpn1[j].indexOf("/",fpn1[j].indexOf("/",fpn1[j].indexOf("/",0)+1)+1);
                break;
            case 4:pos[i-1+count1*j]=fpn1[j].indexOf("/",fpn1[j].indexOf("/",fpn1[j].indexOf("/",fpn1[j].indexOf("/",0)+1)+1)+1);
                break;
            case 6:qDebug()<<fpn1[j].indexOf("/",0);
                break;
            case 7:qDebug()<<fpn1[j].indexOf("/",0);
                break;
            case 8:qDebug()<<fpn1[j].indexOf("/",0);
                break;
            case 9:qDebug()<<fpn1[j].indexOf("/",0);
                break;
            case 10:qDebug()<<fpn1[j].indexOf("/",0);
                break;
            default :qDebug()<<fpn1[j];
            }
        }

    }

    for(int i=0;i<count1*fCount;i++)
    {
        //qDebug()<<pos[i];
        sorPos[i]=pos[i];
    }

    for(int i=0;i<row;i++)
    {
        qDebug()<<daiPos[i];
        //qDebug()<<sorPos[i];
    }

    for(int i=0;i<row;i++)
    {
        //qDebug()<<daiPos[i];
        qDebug()<<sorPos[i];
    }

    boolean posbol=false;
    for(int i=0;i<row;i++)
    {
        if(daiPos[i]!=sorPos[i])
        {
            posbol=true;
            break;
        }
    }




    if(productLabel==verLabel)
    {
        //result[0]="相同";
        ui->resultLineEdit->setText("版本文件的标签相同");
        //ui->resultLineEdit->append("版本文件的标签相同");
    }
    else if(posbol)
    {
        ui->resultLineEdit->setText("目录结构或文件名发生了改变");
    }
        else
        {
            if(i>j)
            {
                for(int m=0;m<i;m++)
                {
                    if(fLabel[m]!=littleProLa[m])
                    {
                        result[m]="不同";
                    }
                    else
                        {
                        result[m]="相同";
                    }
                }
            }
            else
            {
                for(int m=0;m<j;m++)
                {
                    if(fLabel[m]!=littleProLa[m])
                    {
                        result[m]="不同";
                    }
                    else
                        {
                        result[m]="相同";
                    }
                }
            }



            //设置表格行和列
            ui->resultTableWidget->setRowCount(row);
            ui->resultTableWidget->setColumnCount(3);
            //设置表格行标题
            QStringList headerLabels;


            headerLabels<<"源文件["+source+"]"<<"当前文件["+current+"]"<<"结果";
            ui->resultTableWidget->setHorizontalHeaderLabels(headerLabels);

            //设置表格行标题的对齐方式
            ui->resultTableWidget->horizontalHeader()->setDefaultAlignment(Qt::AlignLeft);

            //设置行标题
            for(int i=1;i<=row;i++)
            {
                QStringList rowLabels;
                rowLabels<<QString::number(i-(row-1));//有毛斌
                ui->resultTableWidget->setVerticalHeaderLabels(rowLabels);

            }

            //设置每行的高度
            for(int i=0;i<row;i++)
            {
                ui->resultTableWidget->setRowHeight(i,22);
            }

            //自动调整最后一列的宽度使他和表格的右边界对齐

            ui->resultTableWidget->horizontalHeader()->setStretchLastSection(true);

            //设置表格的选择方式

            ui->resultTableWidget->setSelectionBehavior(QAbstractItemView::SelectItems);

            //设置编辑方式
            ui->resultTableWidget->setEditTriggers(QAbstractItemView::DoubleClicked);

            //设置表格的内容
            for(int i=0;i<row;++i)
            {

                QTableWidgetItem *item0, *item1, *item2;
                item0=new QTableWidgetItem;
                item1=new QTableWidgetItem;
                item2=new QTableWidgetItem;

                if(sofpn[i].isNull())
                {
                    sofpn[i]="不存在";
                }

                QString txt=sofpn[i].mid(31,sofpn[i].length());
                item0->setText(txt);
                ui->resultTableWidget->setItem(i,0,item0);

                if(i+1>ui->resultListWidget->count())
                {
                    txt="不存在";
                }
                else
                {
                    txt=ui->resultListWidget->item(i)->text().mid(31,ui->resultListWidget->item(i)->text().length());
                }

                item1->setText(txt);
                ui->resultTableWidget->setItem(i,1,item1);

                //txt=QString("%1").arg(result[i]);
                txt=result[i];
                item2->setText(txt);
                ui->resultTableWidget->setItem(i,2,item2);


            }

        }





}




void Widget::on_CompareTreeView_clicked(const QModelIndex &index)
{


    QString name,info,info1;
    if(index.column() == 0)
    {
        name = index.data().toString();
        info1 = index.sibling(index.row(),1).data().toString();
        info = index.sibling(index.row(),2).data().toString();//标签
    }
    else
    {
            if(index.column() == 1)
            {
                name = index.sibling(index.row(),0).data().toString();
                info1 = index.data().toString();
                info=index.sibling(index.row(),2).data().toString();
            }
            else
            {
                name = index.sibling(index.row(),0).data().toString();
                info1 = index.sibling(index.row(),1).data().toString();
                info=index.data().toString();
            }
    }


    if(!(info1.isNull()))
    {
      //Widget::showSrcTree(name, info1);
    }


    Inf=info1;
    //Widget::compareVer(info1);

}

void Widget::on_pushButton_2Clear_clicked()
{
    ui->resultListWidget->clear();
}

void Widget::on_intoProButton_clicked()
{
    if(!(ui->ProIdlineEdit->text().isEmpty())&&!(ui->ProNamelineEdit->text().isEmpty()))
       {
            intoProductData();
       }
    else
            QMessageBox::about(this,"请输入","        产品信息");
    if(boln==true)
    {
        QMessageBox::warning(this,"产品数据","入库成功");
        ui->ProIdlineEdit->clear();
        ui->ProNamelineEdit->clear();
        ui->UnitIdlineEdit->clear();
        ui->UnitNamelineEdit->clear();
        //QMessageBox::about(NULL,"恭喜你","正在刷新结果");
        Widget::initShowProTree();
        Widget::resultShowTree();
        boln=false;        
    }
}

void Widget::on_intoUniButton_clicked()
{
    if(!(ui->ProIdlineEdit->text().isEmpty())&&!(ui->ProNamelineEdit->text().isEmpty())&&!(ui->UnitIdlineEdit->text().isEmpty())&&!(ui->UnitNamelineEdit->text().isEmpty()))
       {
            intoUnitData();
       }
    else
            QMessageBox::about(this,"请输入","产品，单元信息");
    if(boln==true)
    {
        QMessageBox::warning(this,"数据","入库成功");
        ui->ProIdlineEdit->clear();
        ui->ProNamelineEdit->clear();
        ui->UnitIdlineEdit->clear();
        ui->UnitNamelineEdit->clear();
        //QMessageBox::about(NULL,"恭喜你","正在刷新结果");
        Widget::initShowProTree();
        Widget::resultShowTree();
        boln=false;        
    }
}
void Widget::on_intoVerButton_clicked()
{
    if(!(ui->ProIdlineEdit->text().isEmpty())&&!(ui->ProNamelineEdit->text().isEmpty())&&!(ui->UnitIdlineEdit->text().isEmpty())&&!(ui->UnitNamelineEdit->text().isEmpty())&&!(ui->VerNamelineEdit->text().isEmpty())&&!(ui->VerFilelistWidget->count()==0))
       {
            intoVersionData();
            intoLittleFileData();            
       }
    else
            QMessageBox::about(NULL,"请输入","产品,单元，版本的完整信息");
    if(boln==true)
    {
        QMessageBox::warning(this,"数据","入库成功");
        ui->ProIdlineEdit->clear();
        ui->ProNamelineEdit->clear();
        ui->UnitIdlineEdit->clear();
        ui->UnitNamelineEdit->clear();
        ui->VerNamelineEdit->clear();
        ui->VerStatelineEdit->clear();
        ui->VerFilelistWidget->clear();
        ui->AlineEdit->clear();
        ui->BlineEdit->clear();
        ui->ClineEdit->clear();
        ui->DlineEdit->clear();
        //QMessageBox::about(NULL,"恭喜你","正在刷新结果");
        Widget::initShowProTree();
        Widget::resultShowTree();
        boln=false;        
    }
}

void Widget::on_updateProButton_clicked()
{
    Widget::updateProductData();

    if(boln)
    {
        QMessageBox::about(NULL,"恭喜你","    更新成功    ");
        Widget::initShowProTree();
        Widget::resultShowTree();
        ui->ProIdlineEdit->clear();
        ui->ProNamelineEdit->clear();
        ui->UnitIdlineEdit->clear();
        ui->UnitNamelineEdit->clear();

    }
    else
    {
       QMessageBox::about(NULL,"糟糕","    更新失败    ");
    }

}

void Widget::on_updateUnitButton_clicked()
{
    Widget::updateUnitData();

    if(boln)
    {
        QMessageBox::about(NULL,"恭喜你","    更新成功    ");
        Widget::initShowProTree();
        Widget::resultShowTree();
        ui->ProIdlineEdit->clear();
        ui->ProNamelineEdit->clear();
        ui->UnitIdlineEdit->clear();
        ui->UnitNamelineEdit->clear();

    }
    else
    {
       QMessageBox::about(NULL,"糟糕","    更新失败    ");
    }
}

void Widget::on_updateVerButton_clicked()
{
    if(ui->VerFilelistWidget->count()<=0)
    {
       QMessageBox::about(NULL,"请放入","    版本文件    ");
    }
    else{
        Widget::updateVersionData();
        Widget::updateFileData();


        if(boln)
        {
            QMessageBox::about(NULL,"恭喜你","    更新成功    ");
            Widget::initShowProTree();
            Widget::resultShowTree();
            ui->ProIdlineEdit->clear();
            ui->ProNamelineEdit->clear();
            ui->UnitIdlineEdit->clear();
            ui->UnitNamelineEdit->clear();
            ui->VerNamelineEdit->clear();
            ui->VerStatelineEdit->clear();
            ui->VerFilelistWidget->clear();
            ui->AlineEdit->clear();
            ui->BlineEdit->clear();
            ui->ClineEdit->clear();
            ui->DlineEdit->clear();

        }
        else
        {
           QMessageBox::about(NULL,"糟糕","    更新失败    ");
        }
    }


}

void Widget::on_clearButton_clicked()
{
    ui->resultLineEdit->clear();
    ui->resultTableWidget->clear();
}

void Widget::on_verNameComboBox_currentTextChanged(const QString &arg1)
{
    ui->VerNamelineEdit->setText(arg1);
    //ui->verNameComboBox->setCurrentIndex(1);
}

void Widget::on_verStateComboBox_currentTextChanged(const QString &arg1)
{
    ui->VerStatelineEdit->setText(arg1);
    //ui->verStateComboBox->setCurrentIndex(1);
}



void Widget::on_verClearButton_clicked()//导出
{
    ui->VerFilelistWidget->clear();

    if(Labell.isNull())
    {
        QMessageBox::about(this,"执行顺序出错","请在左侧产品单元结果显示栏中选择需要导出的版本");
    }
    else{
       Widget::generatePdf();
    }



}

void Widget::on_comparButton_clicked()
{
    if(Inf.isNull())
    {
      QMessageBox::about(this,"执行循序出错","请选择源版本然后拖入待验证版本");
    }
    else
    Widget::compareVer(Inf);

    //Widget::generatePdf();
}

void Widget::generatePdf()
{
    QString str="软件版本电子指纹码//"
            "产品名称：（XXX产品）配置项名称：（YYY单元）//"
            "版本：（Vx.yy.zz）包含XX个文件，//"
            "该版本的软件版本电子指纹码：XXXXXXXX-XXXXXXXX-XXXXXXXX-XXXXXXXX.//"
            "请知悉。//"
            "软件工程所//"
            "XXXX年XX月XX日";

    //保存程序当前路径
    QString currentDir = QDir::currentPath();
    //生成pdf保存的路径
    QString file_path = QFileDialog::getExistingDirectory(this,"请选择保存路径","C:\\Users\\Administrator\\Desktop");
    if(file_path.isEmpty())
    {
        return;
        QMessageBox::warning(this,"提示","路径不能为空");
    }

    //将程序的执行路径设置到filePath下
    QDir tempDir;
    tempDir.setCurrent(file_path);


    //进度条
    QProgressBar *m_pProgressBar = new QProgressBar(this);
    m_pProgressBar->setOrientation(Qt::Horizontal);  // 水平方向
    m_pProgressBar->setMinimum(1);  // 最小值
    m_pProgressBar->setMaximum(str.size());  // 最大值
    m_pProgressBar->setMaximumHeight(15);
    m_pProgressBar->setMinimumWidth(60);
    m_pProgressBar->setVisible(true);


        m_pProgressBar->setValue(0);

        QCoreApplication::processEvents();

        //QDate date=QDate::currentDate();
        //QString name_time=date.toString("yyyy年MM月dd日hh时mm分ss秒");
        //QDate date1=QDate::currentT;

        QDateTime current_data_time=QDateTime::currentDateTime();
        QString name_time=current_data_time.toString("yyyy.MM.dd"
                                                     ".hh.mm.ss");

        //Qt5 pdfwriter生成pdf
        QString fileName=ProNamel+UnitNamel+VerNamel+"("+name_time+")"+".pdf";
        //qDebug()<<str.at(i);

        QFile pdfFile(fileName);
        //判断文件是否存在
        if(QFile::exists(fileName))
        {

            QMessageBox::StandardButton reply;
            reply = QMessageBox::question(this, "提示", "文件已经存在,点击是将覆盖原文件", QMessageBox::Yes | QMessageBox::No);
            if(reply == QMessageBox::Yes)
            {
                // 打开要写入的pdf文件
                pdfFile.open(QIODevice::WriteOnly);

                QPdfWriter* pPdfWriter = new QPdfWriter(&pdfFile);  // 创建pdf写入器
                pPdfWriter->setPageSize(QPagedPaintDevice::A4);     // 设置纸张为A4
                pPdfWriter->setResolution(300);                     // 设置纸张的分辨率为300,因此其像素为3508X2479

                int iMargin = 60;                   // 页边距
                pPdfWriter->setPageMargins(QMarginsF(iMargin, iMargin, iMargin, iMargin));

                QPainter* pPdfPainter = new QPainter(pPdfWriter);   // qt绘制工具
                // 标题,居中
                QTextOption option(Qt::AlignHCenter | Qt::AlignVCenter);
                option.setWrapMode(QTextOption::WordWrap);

                //字体
                QFont font;
                font.setFamily("simhei.ttf");
                //标题，字号
                int fontSize = 22;
                font.setPointSize(fontSize);
                pPdfPainter->setFont(font);                    // 为绘制工具设置字体
                pPdfPainter->drawText(QRect(0, 0, 2200, 200),Qt::AlignHCenter|Qt::AlignBottom,
                                      "软件版本电子指纹码");
                //option.setWrapMode(QTextOption::WordWrap);
                //画表格

                pPdfPainter->setFont(QFont("", 20));

                // 标题下边留白
                int iTop =250;
                // 左侧缩进
                int iLeft = 180;
                // 设置画笔颜色、宽度
                //pPdfPainter.setPen(QPen(QColor(0, 160, 230), 2));
                pPdfPainter->setPen(10);
                // 设置画刷颜色
                //pPdfPainter->setBrush(QColor(255, 160, 90));
                pPdfPainter->drawRect(iLeft, iTop, 1980, 1800);//表格矩形方框
                pPdfPainter->drawLine(iLeft,iTop+300,iLeft+1980,iTop+300);
                pPdfPainter->drawLine(iLeft,iTop+600,iLeft+1980,iTop+600);
                pPdfPainter->drawLine(iLeft,iTop+900,iLeft+1980,iTop+900);
                pPdfPainter->drawLine(iLeft,iTop+1200,iLeft+1980,iTop+1200);
                pPdfPainter->drawLine(iLeft,iTop+1500,iLeft+1980,iTop+1500);
                pPdfPainter->drawLine(600,iTop,600,iTop+1800);
                pPdfPainter->drawText(iLeft+50,iTop+200,"产品代号");
                pPdfPainter->drawText(650,iTop+200,QString("%1").arg(ProDail));

                QDate date=QDate::currentDate();
                QString create_time=date.toString("yyyy年MM月dd日");

                pPdfPainter->drawText(iLeft+50,iTop+500,QString("产品名称"));
                pPdfPainter->drawText(650,iTop+500,QString("%1").arg(ProNamel));
                pPdfPainter->drawText(iLeft+50,iTop+800,QString("CSCI名称"));
                pPdfPainter->drawText(650,iTop+800,QString("%1").arg(UnitNamel));
                pPdfPainter->drawText(iLeft+50,iTop+1100,QString("CSCI版本"));
                pPdfPainter->drawText(650,iTop+1100,QString("%1").arg(VerNamel));
                pPdfPainter->drawText(iLeft+50,iTop+1400,QString("文件个数"));
                pPdfPainter->drawText(650,iTop+1400,QString("%1").arg(FileNuml));
                pPdfPainter->drawText(iLeft+50,iTop+1700,QString("指 纹 码"));
                pPdfPainter->drawText(650,iTop+1700,QString("%1").arg(Labell));
                //pPdfPainter->drawText(iLeft+50,iTop+2200,"请悉知。

                pPdfPainter->drawText(iLeft,iTop+1900,"于"+QString("%1").arg(create_time));
                pPdfPainter->drawText(iLeft+708,iTop+1900,"经"+QString("%1").arg(Statel)+"测试,"+
                                                                                        "其缺陷为严重:"+QString("%1").arg(Adefl)+"个"+
                                                                                        ",重要:"+QString("%1").arg(Bdefl)+"个,");
                pPdfPainter->drawText(iLeft-156,iTop+2100,"一般:"+QString("%1").arg(Cdefl)+"个"+
                                                          ",轻微:"+QString("%1").arg(Ddefl)+"个.");

                pPdfPainter->drawText(iLeft+1680,iTop+2500,"软件工程所");
                pPdfPainter->drawText(iLeft+1560,iTop+2700,QString("%1").arg(create_time));
                delete pPdfPainter;
                delete pPdfWriter;
                pdfFile.close();
            }
            else
            {
                return;
            }
        }
        else
        {
            // 打开要写入的pdf文件
            pdfFile.open(QIODevice::WriteOnly);

            QPdfWriter* pPdfWriter = new QPdfWriter(&pdfFile);  // 创建pdf写入器
            pPdfWriter->setPageSize(QPagedPaintDevice::A4);     // 设置纸张为A4
            pPdfWriter->setResolution(300);                     // 设置纸张的分辨率为300,因此其像素为3508X2479

            int iMargin = 60;                   // 页边距
            pPdfWriter->setPageMargins(QMarginsF(iMargin, iMargin, iMargin, iMargin));

            QPainter* pPdfPainter = new QPainter(pPdfWriter);   // qt绘制工具
            // 标题,居中
            QTextOption option(Qt::AlignHCenter | Qt::AlignVCenter);
            option.setWrapMode(QTextOption::WordWrap);

            //字体
            QFont font;
            font.setFamily("simhei.ttf");
            //标题，字号
            int fontSize = 22;
            font.setPointSize(fontSize);
            pPdfPainter->setFont(font);                    // 为绘制工具设置字体
            pPdfPainter->drawText(QRect(0, 0, 2200, 200),Qt::AlignHCenter|Qt::AlignBottom,
                                  "软件版本电子指纹码");
            //option.setWrapMode(QTextOption::WordWrap);
            //画表格

            pPdfPainter->setFont(QFont("", 20));

            // 标题下边留白
            int iTop =250;
            // 左侧缩进
            int iLeft = 180;
            // 设置画笔颜色、宽度
            //pPdfPainter.setPen(QPen(QColor(0, 160, 230), 2));
            pPdfPainter->setPen(10);
            // 设置画刷颜色
            //pPdfPainter->setBrush(QColor(255, 160, 90));
            pPdfPainter->drawRect(iLeft, iTop, 1980, 1800);//表格矩形方框
            pPdfPainter->drawLine(iLeft,iTop+300,iLeft+1980,iTop+300);
            pPdfPainter->drawLine(iLeft,iTop+600,iLeft+1980,iTop+600);
            pPdfPainter->drawLine(iLeft,iTop+900,iLeft+1980,iTop+900);
            pPdfPainter->drawLine(iLeft,iTop+1200,iLeft+1980,iTop+1200);
            pPdfPainter->drawLine(iLeft,iTop+1500,iLeft+1980,iTop+1500);
            pPdfPainter->drawLine(600,iTop,600,iTop+1800);
            pPdfPainter->drawText(iLeft+50,iTop+200,"产品代号");
            pPdfPainter->drawText(650,iTop+200,QString("%1").arg(ProDail));

            QDate date=QDate::currentDate();
            QString create_time=date.toString("yyyy年MM月dd日");

            pPdfPainter->drawText(iLeft+50,iTop+500,QString("产品名称"));
            pPdfPainter->drawText(650,iTop+500,QString("%1").arg(ProNamel));
            pPdfPainter->drawText(iLeft+50,iTop+800,QString("CSCI名称"));
            pPdfPainter->drawText(650,iTop+800,QString("%1").arg(UnitNamel));
            pPdfPainter->drawText(iLeft+50,iTop+1100,QString("CSCI版本"));
            pPdfPainter->drawText(650,iTop+1100,QString("%1").arg(VerNamel));
            pPdfPainter->drawText(iLeft+50,iTop+1400,QString("文件个数"));
            pPdfPainter->drawText(650,iTop+1400,QString("%1").arg(FileNuml));
            pPdfPainter->drawText(iLeft+50,iTop+1700,QString("指 纹 码"));
            pPdfPainter->drawText(650,iTop+1700,QString("%1").arg(Labell));
            //pPdfPainter->drawText(iLeft+50,iTop+2200,"请悉知。

            pPdfPainter->drawText(iLeft,iTop+1900,"于"+QString("%1").arg(create_time));
            pPdfPainter->drawText(iLeft+708,iTop+1900,"经"+QString("%1").arg(Statel)+"测试,"+
                                                                                    "其缺陷为严重:"+QString("%1").arg(Adefl)+"个"+
                                                                                    ",重要:"+QString("%1").arg(Bdefl)+"个,");
            pPdfPainter->drawText(iLeft-156,iTop+2100,"一般:"+QString("%1").arg(Cdefl)+"个"+
                                                      ",轻微:"+QString("%1").arg(Ddefl)+"个.");

            pPdfPainter->drawText(iLeft+1680,iTop+2500,"软件工程所");
            pPdfPainter->drawText(iLeft+1560,iTop+2700,QString("%1").arg(create_time));
            delete pPdfPainter;
            delete pPdfWriter;
            pdfFile.close();
        }

    tempDir.setCurrent(currentDir);
    delete m_pProgressBar;
    QMessageBox::warning(this,"提示","导出完成");
}

bool Widget::eventFilter(QObject *watched, QEvent *event)
{
    if(watched == ui->widget && event->type() == QEvent::Paint)
        Widget::DrawCircuitPaint();
    return QWidget::eventFilter(watched, event);
}
void Widget::DrawCircuitPaint()
{
    QPainter* pPdfPainter = new QPainter(ui->widget);   // qt绘制工具
    // 标题,居中
    QTextOption option(Qt::AlignHCenter | Qt::AlignVCenter);
    option.setWrapMode(QTextOption::WordWrap);

    //字体
    QFont font;
    font.setFamily("simhei.ttf");
    //标题，字号
    int fontSize = 22;
    font.setPointSize(fontSize);
    pPdfPainter->setFont(font);                    // 为绘制工具设置字体
    pPdfPainter->drawText(QRect(0, 0, 1070, 70),Qt::AlignHCenter|Qt::AlignBottom,
                          "软件版本电子指纹码");
    //option.setWrapMode(QTextOption::WordWrap);
    //画表格

    pPdfPainter->setFont(QFont("", 20));

    // 标题下边留白
    int iTop =100;
    // 左侧缩进
    int iLeft = 80;
    // 设置画笔颜色、宽度
    //pPdfPainter.setPen(QPen(QColor(0, 160, 230), 2));
    pPdfPainter->setPen(10);
    // 设置画刷颜色
    //pPdfPainter->setBrush(QColor(255, 160, 90));
    pPdfPainter->drawRect(iLeft, iTop, 930, 480);//表格矩形方框
    pPdfPainter->drawLine(iLeft,iTop+80,iLeft+930,iTop+80);
    pPdfPainter->drawLine(iLeft,iTop+160,iLeft+930,iTop+160);
    pPdfPainter->drawLine(iLeft,iTop+240,iLeft+930,iTop+240);
    pPdfPainter->drawLine(iLeft,iTop+320,iLeft+930,iTop+320);
    pPdfPainter->drawLine(iLeft,iTop+400,iLeft+930,iTop+400);
    pPdfPainter->drawLine(300,iTop,300,iTop+480);
    pPdfPainter->drawText(iLeft+40,iTop+50,"产品代号");
    pPdfPainter->drawText(600,iTop+50,QString("%1").arg(ProDail));

    QDate date=QDate::currentDate();
    QString create_time=date.toString("yyyy年MM月dd日");

    pPdfPainter->drawText(iLeft+40,iTop+130,QString("产品名称"));
    pPdfPainter->drawText(600,iTop+130,QString("%1").arg(ProNamel));
    pPdfPainter->drawText(iLeft+40,iTop+210,QString("CSCI名称"));
    pPdfPainter->drawText(600,iTop+210,QString("%1").arg(UnitNamel));
    pPdfPainter->drawText(iLeft+40,iTop+290,QString("CSCI版本"));
    pPdfPainter->drawText(600,iTop+290,QString("%1").arg(VerNamel));
    pPdfPainter->drawText(iLeft+40,iTop+370,QString("文件个数"));
    pPdfPainter->drawText(600,iTop+370,QString("%1").arg(FileNuml));
    pPdfPainter->drawText(iLeft+40,iTop+450,QString("指 纹 码"));
    pPdfPainter->drawText(350,iTop+450,QString("%1").arg(Labell));
    //pPdfPainter->drawText(iLeft+50,iTop+2200,"请悉知。

    pPdfPainter->drawText(iLeft+50,iTop+520,"于"+QString("%1").arg(create_time)+"经"+QString("%1").arg(Statel)+"测试,"+
                          "其缺陷为严重:"+QString("%1").arg(Adefl)+"个"+
                          ",重要:"+QString("%1").arg(Bdefl)+"个,"+"一般:"+QString("%1").arg(Cdefl)+"个,");
//    pPdfPainter->drawText(iLeft+330,iTop+520,"经"+QString("%1").arg(Statel)+"测试,"+
//                                                                            "其缺陷为严重:"+QString("%1").arg(Adefl)+"个"+
//                                                                            ",重要:"+QString("%1").arg(Bdefl)+"个,");
    pPdfPainter->drawText(iLeft,iTop+560,"轻微:"+QString("%1").arg(Ddefl)+"个.");

    pPdfPainter->drawText(iLeft+760,iTop+600,"软件工程所");
    pPdfPainter->drawText(iLeft+720,iTop+640,QString("%1").arg(create_time));
    delete pPdfPainter;

}

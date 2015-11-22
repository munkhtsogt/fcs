#include "mainwindow.h"
#include "ui_mainwindow.h"

QPushButton * pushButton;
QPushButton * pushButton_2;
QPushButton * pushButton_3;

QLabel * label;
QWidget * tab, * tab_2;
QTabWidget * tabWidget;
QLineEdit * lineEdit;
QTextEdit * textEdit;
QListView * listView;
int i = 0;

QString imagename;

QStandardItemModel * model;
QModelIndex globalModelIndex;

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    QTextCodec::setCodecForCStrings(QTextCodec::codecForName("UTF-8"));
    ui->setupUi(this);
    pushButton = ui->pushButton;
    pushButton_2 = ui->pushButton_2;
    pushButton_3 = ui->pushButton_3;

    pushButton->setIconSize(QSize(41,41));
    pushButton->setIcon(QIcon(":/images/save2.png"));
    connect(pushButton,SIGNAL(clicked()),this, SLOT(pushButton_Click_Handler()));

    pushButton_2->setIconSize(QSize(41,41));
    pushButton_2->setIcon(QIcon(":/images/browse2.png"));
    connect(pushButton_2,SIGNAL(clicked()),this, SLOT(pushButton_2_Click_Handler()));

    pushButton_3->setIconSize(QSize(41,41));
    pushButton_3->setIcon(QIcon(":/images/back.png"));
    connect(pushButton_3, SIGNAL(clicked()), this, SLOT(pushButton_3_Click_Handler()));

    label = ui->label;

    tabWidget = ui->tabWidget;

    tabWidget->setTabEnabled(1,false);

    lineEdit = ui->lineEdit;
    textEdit = ui->textEdit;

    textEdit->setStyleSheet("background:");
    listView = ui->listView;
    listView->setEnabled(true);

    model = new QStandardItemModel();
    listView->setModel(model);
    listView->setIconSize(QSize(40,30));
    food_Walker();

    connect(this, SIGNAL(success()),this, SLOT(clear()));

    connect(listView,SIGNAL(doubleClicked(QModelIndex)),this, SLOT(selected_Food(QModelIndex )) );

    listView->setContextMenuPolicy(Qt::CustomContextMenu);
    connect(listView, SIGNAL(customContextMenuRequested(QPoint)), this, SLOT(listView_Item_Delete(QPoint)));
}
void MainWindow::listView_Item_Delete(QPoint point)
{
    QMenu * menu = new QMenu;
    QIcon icon = QIcon(":/images/trash.png");
    menu->addAction(icon, "delete",this, SLOT(listView_Delete(bool)));
    QAction * action = menu->exec(listView->mapToGlobal(point));
    if(action)
    {
        QModelIndexList indexes = listView->selectionModel()->selectedIndexes();
        foreach(QModelIndex index, indexes)
        {
            QString itemname = listView->model()->itemData(index).values()[2].toString();
            listView->model()->removeRow(index.row(),index.parent());
            QString path = QString("data/%1").arg(itemname);

            QFile * f = new QFile(path);
            if(f->exists())
            {
                f->remove();
            }
            delete f;


            QFileInfo fileInfo("images");
            QDirIterator directoryWalker(fileInfo.absoluteFilePath(), QDir::Files | QDir::NoSymLinks, QDirIterator::Subdirectories);
            QString fileFormat;
            while(directoryWalker.hasNext())
            {
                directoryWalker.next();
                if(take_File_Name(itemname) == take_File_Name(directoryWalker.fileInfo().fileName()))
                {
                    fileFormat = file_Type(directoryWalker.fileInfo().fileName());
                    break;
                }
            }
            if(!fileFormat.isNull())
            {
                QString path2 = QString("images/%1.%2").arg(take_File_Name(itemname)).arg(fileFormat);
                QFile * fi = new QFile(path2);
                if(fi->exists())
                {
                    fi->remove();
                }
                delete fi;
            }
        }
    }
}
void MainWindow::listView_Delete( bool flag) { }
void MainWindow::selected_Food(QModelIndex modelIndex)
{
    tabWidget->setTabEnabled(1, true);
    tabWidget->setTabEnabled(0, false);
    QString itemname = listView->model()->itemData(modelIndex).values()[2].toString();
    ui->textBrowser->setText(take_File_Name(itemname));
    QString path = QString("data/%1").arg(itemname);
    QFile * f = new QFile(path);
    if(!f->open(QIODevice::ReadOnly))
    {
        return;
    }

    ui->textBrowser_2->setText(QString::fromUtf8(f->readAll()));



    QFileInfo fileInfo("images");
    QDirIterator directoryWalker(fileInfo.absoluteFilePath(), QDir::Files | QDir::NoSymLinks, QDirIterator::Subdirectories);
    QString fileFormat;
    while(directoryWalker.hasNext())
    {
        directoryWalker.next();
        if(take_File_Name(itemname) == take_File_Name(directoryWalker.fileInfo().fileName()))
        {
            fileFormat = file_Type(directoryWalker.fileInfo().fileName());
            break;
        }
    }

    if(!fileFormat.isNull())
    {
        QString path2 = QString("images/%1.%2").arg(take_File_Name(itemname)).arg(fileFormat);
        if(QFile(path2).exists())
        {
          QPixmap image(path2);
          int width = ui->label_4->rect().width();
          int height = ui->label_4->rect().height();
          image.scaled(QSize(width, height));
          ui->label_4->setScaledContents(true);
          ui->label_4->setPixmap(image);
          ui->label_4->setMask(image.mask());
        }
    }
}
QString MainWindow::take_File_Name(QString str)
{
    int len =  str.length();
    int index;
    QString s;
    for(int k=0; k<len; k++)
    {
        if(str.at(k) == '.')
        {
            index = k;
            break;
        }
    }
    for(int k = 0; k<index; k++)
     s += str.at(k);
    return s;
}
void MainWindow::clear()
{
    lineEdit->clear();
    textEdit->clear();
    label->clear();
    i = 0;
}
void MainWindow::food_Walker()
{
    QFileInfo fileInfo("data");
    QDirIterator directoryWalker(fileInfo.absoluteFilePath(), QDir::Files | QDir::NoSymLinks, QDirIterator::Subdirectories);
    while(directoryWalker.hasNext())
    {
        directoryWalker.next();
        QIcon qicon = QIcon(":/images/food.png");
        QStandardItem * item = new QStandardItem(qicon, take_File_Name(directoryWalker.fileInfo().fileName()));
        item->setToolTip(directoryWalker.fileInfo().fileName());
        item->setEditable(false);
        model->appendRow(item);
    }
}

void MainWindow::pushButton_Click_Handler()
{
    if(!QDir("data").exists())
    {
        QDir().mkdir("data");
    }


    QString foodname = lineEdit->text();
    QString foodcontent = textEdit->toPlainText();
    QString filepath;

    if(foodname.trimmed().count() == 0 || foodcontent.trimmed().count() == 0)
    {
        QMessageBox qmb;

        qmb.setWindowTitle(trUtf8("?????? ?????"));
        qmb.setText("Please fill subject or content!");
        qmb.exec();
        return;
    }
    else
    {

        QFile *file;
        filepath = QString("data/%1.dat").arg(foodname);

        while(QFile(filepath).exists())
        {
            i++;
            foodname = foodname + "_";
            foodname = foodname + QString("%1").arg(i);
            filepath = QString("data/%1.dat").arg(foodname);
        }

        file = new QFile(filepath);
        if(!file->open(QIODevice::WriteOnly | QIODevice::Text))
        {
            return;
        }

        QTextStream writetofile(file);
        //writetofile << foodname << "\n";
        writetofile << foodcontent.toUtf8() << "\n";

        file->close();

        if(!QDir("images").exists())
        {
            QDir().mkdir("images");
        }
        if(!imagename.isNull())
        {
            QString format = file_Type(imagename);
            QString newpath = QString("images/%1.%2").arg(foodname).arg(format);

            file = new QFile(newpath);
            file->open(QIODevice::WriteOnly);

            QFile * f = new QFile(imagename);
            f->open(QIODevice::ReadOnly);

            while(!f->atEnd())
            {
             file->write(f->readLine());
            }

            file->close();
            f->close();
        }
    }
    QFileInfo fileinfo(filepath);
    QIcon qicon = QIcon(":/images/cake.png");
    QStandardItem * item = new QStandardItem(qicon, take_File_Name(fileinfo.fileName()));
    item->setToolTip(fileinfo.fileName());
    item->setEditable(false);
    model->appendRow(item);

    emit success();
}
QString MainWindow::file_Type(QString filepath)
{
    int lenght = filepath.length();
    int index;
    QString format;
    for(int k=0; k<lenght; k++)
    {
        if(filepath.at(k) == '.')
        {
            index = k;
            break;
        }
    }
    for(int k=index+1;k<lenght;k++)
     format += filepath.at(k);
    return format;
}
void MainWindow::pushButton_2_Click_Handler()
{
    imagename = QFileDialog::getOpenFileName(this,tr("Open File"),QDir::currentPath(),tr("Image Files (*.png *.jpg *.bmp )"));
    if(!imagename.isEmpty())
    {
        QPixmap image(imagename);
        int width = label->rect().width();
        int height = label->rect().height();
        image.scaled(QSize(width, height));
        label->setScaledContents(true);
        label->setPixmap(image);
        label->setMask(image.mask());
    }
}
void MainWindow::pushButton_3_Click_Handler()
{
    tabWidget->setTabEnabled(0, true);
    tabWidget->setTabEnabled(1, false);
}
MainWindow::~MainWindow()
{
    delete ui;
}

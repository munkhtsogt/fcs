#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QFileDialog>
#include <QFile>
#include <QDir>
#include <QPixmap>
#include <QBitmap>
#include <QSize>
#include <QTableWidget>
#include <QTextEdit>
#include <QDebug>
#include <QMessageBox>
#include <QIcon>
#include <QFileIconProvider>
#include <QStandardItem>
#include <QDirIterator>
#include <QTextCodec>
#include <QPoint>
#include <QMenu>
#include <QAction>



namespace Ui {
    class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

public slots:
    void pushButton_Click_Handler();
    void pushButton_2_Click_Handler();
    void pushButton_3_Click_Handler();
    QString file_Type(QString );
    QString take_File_Name(QString );
    void food_Walker();
    void clear();
    void selected_Food(QModelIndex );
    void listView_Item_Delete(QPoint );
    void listView_Delete(bool );


signals:
    void success();

private:
    Ui::MainWindow *ui;
};

#endif // MAINWINDOW_H

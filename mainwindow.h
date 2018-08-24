#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QScrollArea>
#include <QThread>
#include <QString>
#include <QFileDialog>
#include <QMessageBox>
#include <QScrollBar>
#include <QTextCodec>
#include <QProgressDialog>
#include <myeditor.h>
#include <findstring.h>
namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();
    MyEditor *editor;
    QScrollArea *scrollArea;
    FindString *find;

    int hScrollValue;
    int vScrollValue;
    QString nowFileName;
    QString nowFilePath;
    bool isSaved;

    void readInFromFile(QString path);
    void writeOutToFile(QString path);
    void newOneEditor();
    void mousePressEvent(QMouseEvent *event);
    void keyPressEvent(QKeyEvent *event);
    void closeEvent(QCloseEvent *event);
private slots:

    void changeScrollValue(QPair<int, int>);

    void autoSave();

    void on_actionFind_triggered();

    void on_actionOpen_triggered();

    void on_actionNew_triggered();

    void on_actionSave_triggered();

    void on_actionSave_As_triggered();

    void on_actionExit_triggered();

    void on_actionUndo_triggered();

private:

    Ui::MainWindow *ui;
};

#endif // MAINWINDOW_H

#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    setWindowIcon(QIcon(":/new/white.ico"));
    nowFileName = "Untitled";
    nowFilePath = "";

    setWindowTitle(nowFileName);
    newOneEditor();
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::newOneEditor()
{

    QPalette pal(this->palette());
    pal.setColor(QPalette::Background,Qt::white);
    this->setAutoFillBackground(true);
    this->setPalette(pal);

    nowFileName = "Untitled";
    nowFilePath = "";

    setWindowTitle(nowFileName);

    scrollArea = new QScrollArea;

    scrollArea->verticalScrollBar()->setSingleStep(40);
    scrollArea->horizontalScrollBar()->setSingleStep(40);
    hScrollValue = vScrollValue = 0;


    editor = new MyEditor(this);
    editor->setGeometry(QRect(0,0,50,50));

    scrollArea->setWidget(editor);
    setCentralWidget(scrollArea);

    find = new FindString(this);

    QTimer *autoSaveTimer = new QTimer(this);
    connect(autoSaveTimer,SIGNAL(timeout()),this,SLOT(autoSave()));
    connect(editor,SIGNAL(cursorChange(QPair<int,int>)),this,SLOT(changeScrollValue(QPair<int,int>)));
    connect(find, SIGNAL(findedString(QString, int)), editor, SLOT(getFindedString(QString,int)));
    connect(editor,SIGNAL(notFind()),find, SLOT(getNotFind()));
    connect(find,SIGNAL(findedStringReplace(QString)),editor,SLOT(getReplaceString(QString)));
    //autoSaveTimer->start(60000);


    this->editor->setFocus(Qt::MouseFocusReason);

}

void MainWindow::changeScrollValue(QPair<int, int> pair)
{
    //横向滑块
    int width = scrollArea->width() - 2*scrollArea->verticalScrollBar()->width();

    hScrollValue = scrollArea->horizontalScrollBar()->value() > 0?
                scrollArea->horizontalScrollBar()->value():hScrollValue;
    if(editor->x() == 0)    hScrollValue = 0;

    if(pair.first - width >= hScrollValue)
    {
        hScrollValue = pair.first - width;
        scrollArea->horizontalScrollBar()->setValue(hScrollValue);
    }
    else if(pair.first < hScrollValue)
    {
        hScrollValue = pair.first;
        scrollArea->horizontalScrollBar()->setValue(hScrollValue);
    }
    else if(pair.first > hScrollValue && pair.first < hScrollValue + width)
    {
        scrollArea->horizontalScrollBar()->setValue(hScrollValue);
    }

    //纵向滑块
    int height = scrollArea->height() - 2*scrollArea->horizontalScrollBar()->height();

    vScrollValue = scrollArea->verticalScrollBar()->value() > 0?
                scrollArea->verticalScrollBar()->value():vScrollValue;
    if(editor->y() == 0)    vScrollValue = 0;

    if(pair.second - height >= vScrollValue)
    {
        vScrollValue = pair.second - height;
        scrollArea->verticalScrollBar()->setValue(vScrollValue);
    }
    else if(pair.second  < vScrollValue)
    {
        vScrollValue = pair.second;
        scrollArea->verticalScrollBar()->setValue(vScrollValue);
    }
    else
    {
        scrollArea->verticalScrollBar()->setValue(vScrollValue);
    }
}

void MainWindow::autoSave()
{
    writeOutToFile(nowFilePath);
    editor->isHaveSaved = true;
}

void MainWindow::readInFromFile(QString path)
{

    QFile file(path);
    QFileInfo fi(file.fileName());
    nowFileName = fi.fileName() + "  -MiniWord";
    if(!file.open(QIODevice::ReadOnly | QIODevice::Text))
        return;
    QTextStream readIn(&file);
    int totalsize = 0;

    QProgressDialog progress("Open file...", "cancel",0,fi.size(),this);
    progress.setWindowTitle("Open");
    progress.setWindowModality(Qt::WindowModal);
    progress.show();

    readIn.setCodec("UTF-8");
    while (!readIn.atEnd())
    {
        QString line = readIn.readLine();
        editor->addTextString(line.remove(QChar('\n'),Qt::CaseSensitive));

        totalsize += line.size();
        progress.setValue(totalsize);
        if(progress.wasCanceled())
        {
            newOneEditor();
            break;
        }

    }

    progress.setValue(fi.size());
    editor->freshTextLayout(false);
}

void MainWindow::writeOutToFile(QString path)
{
    QFile file(path);
    if(!file.open(QIODevice::WriteOnly | QIODevice::Text))
    {
        return;
    }
    QTextStream writeOut(&file);
    writeOut.setCodec("UTF-8");
    QString text;

    int totalline = 0;
    QProgressDialog progress("Save file...", "",0,editor->totalLine(),this);
    progress.setWindowTitle("Save");
    progress.setCancelButton(0);
    progress.setWindowModality(Qt::WindowModal);
    progress.show();
    while(editor->getOneLine(text))
    {
        writeOut<<text;
        progress.setValue(totalline++);
    }

    progress.setValue(editor->totalLine());

}
void MainWindow::mousePressEvent(QMouseEvent *event)
{
    this->editor->setFocus(Qt::MouseFocusReason);
}

void MainWindow::keyPressEvent(QKeyEvent *event)
{
    isSaved = false;
}

void MainWindow::on_actionFind_triggered()
{
    find->show();
}

void MainWindow::on_actionOpen_triggered()
{
    if(!editor->isHaveSaved)
    {
        QMessageBox::StandardButton reply;
        reply = QMessageBox::question(this, tr("Haven't Saved"),
                                      tr("To Save?"),
                                      QMessageBox::Yes | QMessageBox::No | QMessageBox::Cancel);
        if(reply == QMessageBox::Yes)
        {
            writeOutToFile(nowFilePath);
            newOneEditor();
        }
        else if(reply == QMessageBox::No)
        {
            QString path = QFileDialog::getOpenFileName(this,
                                                        tr("Open File"),
                                                        ".",
                                                        tr("Text Files(*.txt)"));
            if(!path.isEmpty())
            {
                nowFilePath = path;
                newOneEditor();
                readInFromFile(path);

                this->setWindowTitle(nowFileName);
                return;
            }

        }
        else if(reply == QMessageBox::Cancel)
        {
            //todo
        }
    }
    else
    {
        QString path = QFileDialog::getOpenFileName(this,
                                                    tr("Open File"),
                                                    ".",
                                                    tr("Text Files(*.txt)"));
        if(!path.isEmpty())
        {
            nowFilePath = path;
            newOneEditor();
            readInFromFile(path);

            this->setWindowTitle(nowFileName);
            return;
        }
    }

}

void MainWindow::on_actionNew_triggered()
{
    if(!editor->isHaveSaved)
    {
        QMessageBox::StandardButton reply;
        reply = QMessageBox::question(this, tr("Haven't Saved"),
                                      tr("To Save?"),
                                      QMessageBox::Yes | QMessageBox::No | QMessageBox::Cancel);
        if(reply == QMessageBox::Yes)
        {
            writeOutToFile(nowFilePath);
            newOneEditor();
        }
        else if(reply == QMessageBox::No)
        {
            newOneEditor();
        }
        else if(reply == QMessageBox::Cancel)
        {
            //todo
        }
    }
    else
    {
        newOneEditor();
    }
}


void MainWindow::on_actionSave_triggered()
{
    if(!nowFilePath.isEmpty())
        writeOutToFile(nowFilePath);
    else
    {
        QString path = QFileDialog::getSaveFileName(this,
                                                    tr("Save File"),
                                                    ".",
                                                    tr("Text Files(*.txt)"));
        writeOutToFile(path);
    }

}

void MainWindow::on_actionSave_As_triggered()
{
    QString path = QFileDialog::getSaveFileName(this,
                                                tr("Save File"),
                                                ".",
                                                tr("Text Files(*.txt)"));
    writeOutToFile(path);
}

void MainWindow::on_actionExit_triggered()
{
    this->close();
}

void MainWindow::closeEvent(QCloseEvent *event)
{
    if(!editor->isHaveSaved)
    {
        QMessageBox::StandardButton reply;
        reply = QMessageBox::question(this, tr("Save?"),
                                      tr("To Save?"),
                                      QMessageBox::Yes | QMessageBox::No | QMessageBox::Cancel);
        if(reply == QMessageBox::Yes)
        {
            writeOutToFile(nowFilePath);
            newOneEditor();
            event->accept();
        }
        else if(reply == QMessageBox::No)
        {
            event->accept();
        }
        else if(reply == QMessageBox::Cancel)
        {
            event->ignore();
        }
    }
}



void MainWindow::on_actionUndo_triggered()
{
    this->editor->undo();
}

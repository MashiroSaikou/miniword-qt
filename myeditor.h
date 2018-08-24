#ifndef MYEDITOR_H
#define MYEDITOR_H
#include <QWidget>
#include <QTextLayout>
#include <QTextLine>
#include <QPainter>
#include <QString>
#include <QPointF>
#include <QTimer>
#include <QDebug>
#include <QPair>
#include "memoryzero.h"
typedef struct _Cursor
{
    int line;
    int position;
}Cursor;

typedef struct _Select
{
    int startX;
    int startY;
    int endX;
    int endY;

    int startLine;
    int startPosition;
    int endLine;
    int endPosition;

    bool isSelected;
}Select;

class MyEditor : public QWidget
{
    Q_OBJECT
public:
    MyEditor(QWidget *parent = 0);
    
    /*文件到编辑区*/
    void addTextString(QString text);
    
    /*编辑区到文件*/
    bool getOneLine(QString &text);
    int totalLine();
    
    /*编辑区到内存*/
    QPair<int, int> getCursorPosition();
    void setSelected();
    
    /*键盘与鼠标*/
    void cursorUp();
    void cursorDown();
    void cursorRight();
    void cursorLeft();
    void cursorEnter();
    void keyPressEvent(QKeyEvent *event);
    void keyReleaseEvent(QKeyEvent *event);
    void mousePressEvent(QMouseEvent *event);
    void mouseMoveEvent(QMouseEvent *event);
    void mouseReleaseEvent(QMouseEvent *event);
    void inputMethodEvent(QInputMethodEvent *event);
    void setTextCursor_Mouse(int, int);
    void setSelect_1(int, int);
    void setSelect_2(int, int);


    
    /*绘制*/
    void paintEvent(QPaintEvent *event);
    void moveEvent(QMoveEvent *event);
    void drawSelected(QPainter *painter);
    void freshTextLayout(bool);

    
    QPair<int,int> indexToCoordinate(int, int);
    int getStartPositioninLine(int);
    int getEndPositioninLine(int);
    int getMaxLine();


    void undo();
    /*查找*/
    bool findInMemory(QString);
    Cursor *textCursor;
    Select *selected;
    Memory *memoryzero;
    QTimer *cursorTimer;
    QTimer *freshTimer;
    QTextCodec *codec;
    QStringList *textList;
    QVector<QTextLayout*> Lines;
    QFont font;
    bool isCursorVisible;
    bool isHaveSaved = true;

    int zeroWidth, zeroHeight;
    int leading, height;//行间距和行跨度
    int fontHeight, fontWidth;
private:
    int startLineForShow = 0;
    int endLineForShow = 0;
    int maxLineCountForShow = 100;
public slots:
    void setCursorVisible();

    void getFindedString(QString, int);

    void getReplaceString(QString);
signals:
    void cursorChange(QPair<int,int>);

    void notFind();
};
#endif // MYEDITOR_H

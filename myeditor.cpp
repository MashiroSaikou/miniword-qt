#include "myeditor.h"

MyEditor::MyEditor( QWidget *parent)
{
    /*属性设置*/
    this->setFocusPolicy(Qt::StrongFocus);
    this->setAttribute(Qt::WA_InputMethodEnabled, true);
    this->setAttribute(Qt::WA_KeyCompression,true);

    /*光标初始化*/
    textCursor = new Cursor;
    textCursor->line = textCursor->position = 0;
    selected = new Select;
    selected->isSelected = false;
    isCursorVisible = true;

    /*内存区*/
    memoryzero = new Memory();

    leading = 0;
    height = 17;

    /*字体*/
    font.setFamily("Consolas");
    font.setPointSize(11);
    font.setWeight(QFont::Medium);
    font.setStyle(QFont::StyleNormal);
    font.setLetterSpacing(QFont::PercentageSpacing,105);

    QFontMetrics fm(font);
    fontHeight = height = fm.height();
    fontWidth = fm.averageCharWidth();

    /*文本排布*/
    endLineForShow = startLineForShow + maxLineCountForShow;

    textList = new QStringList;
    textList->append("");
    addTextString("");
    freshTextLayout(false);

    /*定时器*/
    cursorTimer = new QTimer(this);
    freshTimer = new QTimer(this);
    connect(cursorTimer,SIGNAL(timeout()),this,SLOT(setCursorVisible()));
    connect(freshTimer,SIGNAL(timeout()),this,SLOT(update()));

    cursorTimer->start(300);
    freshTimer->start(50);

    this->update();
}

void MyEditor::addTextString(QString text)
{
    /*程序启动或者文本读入时添加内容*/
    // 内存区读入
    memoryzero->Allocation(text);
    QTextLayout *line = new QTextLayout(text);
    Lines.append(line);
}

bool MyEditor::getOneLine(QString &textNeed)
{
    /*得到一行文本，写入文件时用*/
    static int lineNumber = 0;
    if(lineNumber == 0)
        textList = memoryzero->GetQStringList();
    if(lineNumber < textList->size()){
        textNeed = textList->at(lineNumber);
        if(lineNumber == textList->size()-1 && !textNeed.isEmpty()); //最后一行不为空
        else textNeed += '\n';
        lineNumber ++;
        return true;
    }
    else{
        lineNumber = 0;
        return false;
    }
}

QPair<int, int> MyEditor::getCursorPosition()
{
    /*返回当前光标位置*/
    return QPair<int, int>(textCursor->line, textCursor->position);
}

/*********************键盘鼠标事件重写以及光标移动和内存区命令********************/
void MyEditor::cursorUp()
{
    /*光标向上*/

    if(textCursor->line > 0)
    {
        textCursor->line--;
        textCursor->position = Lines[textCursor->line]->lineAt(0).xToCursor(Lines[textCursor->line+1]->lineAt(0).cursorToX(textCursor->position,QTextLine::Leading),QTextLine::CursorBetweenCharacters);
    }
    isCursorVisible = true;
    selected->isSelected = false;

}

void MyEditor::cursorDown()
{
    /*光标向下*/
    if(textCursor->line < Lines.size()-1)
    {
        textCursor->line++;
        textCursor->position = Lines[textCursor->line]->lineAt(0).xToCursor(Lines[textCursor->line-1]->lineAt(0).cursorToX(textCursor->position,QTextLine::Leading),QTextLine::CursorBetweenCharacters);
    }
    isCursorVisible = true;
    selected->isSelected = false;

}

void MyEditor::cursorLeft()
{
    /*光标向左*/
    if(textCursor->position == 0 && textCursor->line > 0)
    {
        textCursor->line--;
        textCursor->position = Lines[textCursor->line]->lineAt(0).textLength();
    }
    else if(textCursor->position > 0)
    {
        textCursor->position = Lines[textCursor->line]->leftCursorPosition(textCursor->position);
    }
    isCursorVisible = true;
    selected->isSelected = false;
}

void MyEditor::cursorRight()
{
    /*光标向右*/
    unsigned int i = Lines[textCursor->line]->lineAt(0).textLength();
    if(textCursor->position == i && textCursor->line < Lines.size() - 1)
    {
        textCursor->line++;
        textCursor->position = 0;
    }
    else if(textCursor->position < i)
    {
        textCursor->position = Lines[textCursor->line]->rightCursorPosition(textCursor->position);
    }
    isCursorVisible = true;
    selected->isSelected = false;

}

void MyEditor::cursorEnter()
{
    /*回车*/
    memoryzero->InsertRow();
    textCursor->line++;
    textCursor->position = 0;
    selected->isSelected = false;

}

void MyEditor::inputMethodEvent(QInputMethodEvent *event)
{
    /*输入法输入*/
    textCursor->position += event->commitString().length();
    memoryzero->AddString(event->commitString());
    freshTextLayout(true);
    emit cursorChange(QPair<int,int>(Lines[textCursor->line]->lineAt(0).cursorToX(textCursor->position,QTextLine::Leading),
                      textCursor->line*height));

}


void MyEditor::keyReleaseEvent(QKeyEvent *event)
{
    //todo
}

void MyEditor::keyPressEvent(QKeyEvent *event)
{
    /*键盘输入事件重写*/
    isHaveSaved = false;

    if(event->matches(QKeySequence::Copy))
    {
        memoryzero->Copy();
        return;
    }
    if(event->matches(QKeySequence::Paste))
    {
        memoryzero->Paste();
        memoryzero->selectCursor(textCursor->line,textCursor->position,
                                 selected->startLine,selected->startPosition,
                                 selected->endLine,selected->endPosition);
        freshTextLayout(false);
        return;
    }
    if(event->matches(QKeySequence::Undo))
    {
        undo();
        return;
    }
    switch(event->key())
    {
    case Qt::Key_Return:
        this->cursorEnter();
        freshTextLayout(false);
        break;
    case Qt::Key_Backspace:
        if(!selected->isSelected)
        {
            cursorLeft();
            memoryzero->BackSpace();
            freshTextLayout(false); break;
        }
        else
        {
            memoryzero->BlockDelete();
            textCursor->line = selected->startLine;
            textCursor->position = selected->startPosition;
            selected->isSelected = false;
            freshTextLayout(false);
            break;
        }
    case Qt::Key_Delete:
        //needdo
        memoryzero->Delete();
        freshTextLayout(false);
        break;
    case Qt::Key_Up:this->cursorUp();break;
    case Qt::Key_Down:this->cursorDown();break;
    case Qt::Key_Right:;this->cursorRight();break;
    case Qt::Key_Left:this->cursorLeft();break;
    case Qt::Key_Home:textCursor->position = 0;memoryzero->SetPair(textCursor->position,textCursor->line+1);freshTextLayout(false);break;
    case Qt::Key_End:textCursor->position = Lines[textCursor->line]->text().length();memoryzero->SetPair(textCursor->position,textCursor->line+1);break;
    default: if(event->text() >= '\u0020' && event->text() <= '\u007E')
            {
                textCursor->position++;
                memoryzero->AddString(event->text());
                selected->isSelected = false;
                freshTextLayout(true);
            }
    }
    emit cursorChange(QPair<int,int>(textCursor->position*fontWidth,
                      textCursor->line*height));
    freshTextLayout(false);
    memoryzero->SetPair(textCursor->position, textCursor->line+1);

    update();
}

void MyEditor::mousePressEvent(QMouseEvent *event)
{

    selected->isSelected = false;
    setTextCursor_Mouse(event->x(),event->y());
    setSelect_1(event->x(),event->y());
    memoryzero->SetPair(textCursor->position,textCursor->line+1);


    update();
}

void MyEditor::setTextCursor_Mouse(int x, int y)
{
    int line = 0;
    line = int(y/height) < Lines.size()?int(y/height):Lines.size()-1;
    textCursor->line = line;
    textCursor->position = Lines[line]->lineAt(0).xToCursor(x, QTextLine::CursorBetweenCharacters);
    isCursorVisible = true;

    emit cursorChange(QPair<int,int>(Lines[textCursor->line]->lineAt(0).cursorToX(textCursor->position,QTextLine::Leading),
                      textCursor->line*height));
}

void MyEditor::setSelect_1(int x, int y)
{
    setTextCursor_Mouse(x, y);
    selected->startLine = textCursor->line;
    selected->startPosition = textCursor->position;
}

void MyEditor::setSelect_2(int x, int y)
{
    setTextCursor_Mouse(x, y);
    selected->endLine = textCursor->line;
    selected->endPosition = textCursor->position;
}

void MyEditor::mouseMoveEvent(QMouseEvent *event)
{
    if(event->x() >= 0 && event->y() >= 0)
    {
        setSelect_2(event->x(), event->y());
    }
    if(selected->startPosition == selected->endPosition && selected->startLine == selected->endLine)
        selected->isSelected = false;
    else
        selected->isSelected = true;

    memoryzero->SetPair(textCursor->position,textCursor->line+1);

    update();
}

void MyEditor::mouseReleaseEvent(QMouseEvent *event)
{
    if(event->x() >= 0 && event->y() >= 0)
    {
        setSelect_2(event->x(), event->y());
    }
    if(selected->startPosition == selected->endPosition && selected->startLine == selected->endLine)
        selected->isSelected = false;
    else
        selected->isSelected = true;

    if(selected->startLine > selected->endLine || (selected->startLine == selected->endLine &&selected->endPosition < selected->startPosition))
    {
        selected->startLine = selected->startLine ^ selected->endLine;
        selected->endLine = selected->startLine ^ selected->endLine;
        selected->startLine = selected->startLine ^ selected->endLine;

        selected->startPosition = selected->startPosition ^ selected->endPosition;
        selected->endPosition = selected->startPosition ^ selected->endPosition;
        selected->startPosition = selected->startPosition ^ selected->endPosition;
    }

    memoryzero->SetPair(textCursor->position,textCursor->line+1);
    memoryzero->SetStart(selected->startPosition,selected->startLine+1);
    memoryzero->SetEnd(selected->endPosition, selected->endLine + 1);

    //emit cursorChange(QPair<int,int>(Lines[textCursor->line]->lineAt(0).cursorToX(textCursor->position,QTextLine::Leading), textCursor->line*height));
    update();
}


/********************文本排布以及绘制*******************************/
void MyEditor::drawSelected(QPainter *painter)
{

    QPair<int, int> start, end;
    if(selected->isSelected)
    {
        painter->setBrush(QColor(51, 153, 255));
        painter->setPen(Qt::NoPen);
        if(selected->startLine == selected->endLine)
        {

            /*只选择一行*/
            start = indexToCoordinate(selected->startLine,selected->startPosition);
            end = indexToCoordinate(selected->endLine, selected->endPosition);

            painter->drawRect(start.first, start.second,end.first - start.first, height);
        }
        else if(selected->startLine < selected->endLine)
        {
            /*向下选取*/
            start = indexToCoordinate(selected->startLine, selected->startPosition);
            end = indexToCoordinate(selected->startLine, getEndPositioninLine(selected->startLine));
            painter->drawRect(start.first, start.second,end.first - start.first, height);

            for(int i = selected->startLine + 1; i < selected->endLine; i ++)
            {
              painter->drawRect(0, i * height, Lines[i]->lineAt(0).naturalTextWidth(), height);
            }

            end = indexToCoordinate(selected->endLine, selected->endPosition);
            start = indexToCoordinate(selected->endLine, getStartPositioninLine(selected->endLine));
            painter->drawRect(start.first, start.second,end.first - start.first, height);
        }
        else if(selected->startLine > selected->endLine)
        {
            //选取两行
            start = indexToCoordinate(selected->startLine, selected->startPosition);
            end = indexToCoordinate(selected->startLine, getStartPositioninLine(selected->startLine));
            painter->drawRect(start.first, start.second,end.first - start.first, height);

            for(int i = selected->endLine + 1; i < selected->startLine; i ++)
            {
              painter->drawRect(0, i * height, Lines[i]->lineAt(0).naturalTextWidth(), height);
            }

            end = indexToCoordinate(selected->endLine, selected->endPosition);
            start = indexToCoordinate(selected->endLine, getEndPositioninLine(selected->endLine));
            painter->drawRect(start.first, start.second,end.first - start.first, height);
        }
    }
}

QPair<int, int> MyEditor::indexToCoordinate(int line, int position)
{
    int x,y;
    y = height * line;
    x = Lines[line]->lineAt(0).cursorToX(position, QTextLine::Leading);
    return QPair<int, int> (x,y);
}

int MyEditor::getStartPositioninLine(int line)
{
    QTextLine thisline = Lines[line]->lineAt(0);
    return thisline.xToCursor(thisline.x(),QTextLine::CursorBetweenCharacters);
}

int MyEditor::getEndPositioninLine(int line)
{
    QTextLine thisline = Lines[line]->lineAt(0);
    return thisline.xToCursor(thisline.x()+thisline.width(), QTextLine::CursorBetweenCharacters);
}

void MyEditor::moveEvent(QMoveEvent *event)
{
    if(event->pos().y() != event->oldPos().y())
    {
        startLineForShow = (0 - event->pos().y())/height;
        startLineForShow = startLineForShow - 100 > 0?startLineForShow - 100:0;
        freshTextLayout(false);
    }
    update();
}

void MyEditor::freshTextLayout(bool isAdd)
{

    int lineCount = memoryzero->memory_capacity;
    int oldLineCount = Lines.size();
    if(isAdd)
    {
        textList = memoryzero->stringBetweenLines(textCursor->line,textCursor->line);
        Lines[textCursor->line] = new QTextLayout(textList->at(0));
        Lines[textCursor->line]->setFont(font);
        Lines[textCursor->line]->clearLayout();
        Lines[textCursor->line]->setCacheEnabled(true);
        Lines[textCursor->line]->beginLayout();
        QTextLine line = Lines[textCursor->line]->createLine();

        line.setNumColumns(Lines[textCursor->line]->text().size());
        line.setPosition(QPointF(0,0));
        Lines[textCursor->line]->endLayout();
    }
    else
    {
        endLineForShow = startLineForShow + maxLineCountForShow + 100;
        endLineForShow = endLineForShow < lineCount - 1?endLineForShow:lineCount - 1;

        textList = memoryzero->stringBetweenLines(startLineForShow,endLineForShow);
        qDebug()<<"fresh"<<startLineForShow<<" "<<endLineForShow<<endl;
        Lines.resize(lineCount);
        if(lineCount > oldLineCount)
        {
            for(int i = oldLineCount; i < lineCount; i++){
                Lines[i] = new QTextLayout("");
                Lines[i]->setFont(font);
                Lines[i]->clearLayout();
                Lines[i]->setCacheEnabled(true);
                Lines[i]->beginLayout();
                QTextLine line = Lines[i]->createLine();

                line.setNumColumns(Lines[i]->text().size());
                line.setPosition(QPointF(0,0));
                Lines[i]->endLayout();
            }
        }
        for(int i = 0,j = startLineForShow; j <= endLineForShow; i++, j++)
        {
            Lines[j]->setText(textList->at(i));
            Lines[j]->setFont(font);
            Lines[j]->clearLayout();
            Lines[j]->setCacheEnabled(true);
            Lines[j]->beginLayout();
            QTextLine line = Lines[j]->createLine();

            line.setNumColumns(Lines[j]->text().size());
            line.setPosition(QPointF(0,0));
            Lines[j]->endLayout();
        }
    }

    this->resize(getMaxLine()+20,Lines.size()*height + 20);

}

void MyEditor::paintEvent(QPaintEvent *event)
{
    QPainter painter(this);
    drawSelected(&painter);
    int start = 0;
    painter.setPen(Qt::black);
    for(int i = 0; i < Lines.size();i++,start += height)
    {
       Lines[i]->draw(&painter, QPoint(0,start));
    }
    if(isCursorVisible)
    {
        Lines[textCursor->line]->drawCursor(&painter,QPointF(0,textCursor->line*height),textCursor->position);
    }
}

void MyEditor::setCursorVisible()
{
    isCursorVisible = !isCursorVisible;
}

int MyEditor::getMaxLine()
{
    int max = 0;
    for (int i = 0; i < Lines.size(); i++)
    {
        max = max < Lines[i]->maximumWidth()?Lines[i]->maximumWidth():max;
    }
    return max;
}

void MyEditor::getFindedString(QString finded, int type)
{
;
    if(memoryzero->Search(type,finded))
    {
        qDebug()<<"------------"<<endl;
        memoryzero->selectCursor(textCursor->line,textCursor->position,
                                 selected->startLine,selected->startPosition,
                                 selected->endLine,selected->endPosition);
        selected->isSelected = true;
        emit cursorChange(QPair<int,int>(Lines[textCursor->line]->lineAt(0).cursorToX(textCursor->position,QTextLine::Leading),
                          textCursor->line*height));
        update();
    }
    else
    {
        emit notFind();
    }
}

void MyEditor::getReplaceString(QString replace)
{
    qDebug()<<replace<<endl;
    memoryzero->Exchange(replace);
    memoryzero->selectCursor(textCursor->line,textCursor->position,
                             selected->startLine,selected->startPosition,
                             selected->endLine,selected->endPosition);
    selected->isSelected = true;
    freshTextLayout(false);
    repaint();
}

void MyEditor::undo()
{
    memoryzero->Undo();
    memoryzero->selectCursor(textCursor->line,textCursor->position,
                             selected->startLine,selected->startPosition,
                             selected->endLine,selected->endPosition);
    freshTextLayout(false);
    update();
}

int MyEditor::totalLine()
{
    return memoryzero->memory_capacity;
}

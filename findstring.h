#ifndef FINDSTRING_H
#define FINDSTRING_H

#include <QDialog>
#include <QPushButton>
#include <QLineEdit>
#include <QRadioButton>
#include <QMessageBox>

#define UPSEARCH 6
#define DOWNSEARCH 7
#define WHOLESEARCH 8

namespace Ui {
class FindString;
}

class FindString : public QDialog
{
    Q_OBJECT

public:
    explicit FindString(QWidget *parent = 0);
    int findType = 7;
    void focusOutEvent(QFocusEvent *event);
    ~FindString();
signals:
    void findedString(QString, int);

    void findedStringReplace(QString);
private slots:
    void getNotFind();

    void on_FindButton_clicked();

    void on_FindedString_textChanged(const QString &arg1);

    void on_CancelButton_clicked();

    void on_Up_clicked();

    void on_Down_clicked();

    void on_All_clicked();

    void on_ReplaceButton_clicked();

private:
    Ui::FindString *ui;
};

#endif // FINDSTRING_H

#include "findstring.h"
#include "ui_findstring.h"

FindString::FindString(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::FindString)
{
    ui->setupUi(this);
    this->setFixedSize(390,115);
    ui->FindButton->setEnabled(false);
    ui->ReplaceButton->setEnabled(false);
    ui->Down->setChecked(true);
}

FindString::~FindString()
{
    delete ui;
}

void FindString::on_FindButton_clicked()
{
    ui->ReplaceButton->setEnabled(true);
    emit findedString(ui->FindedString->text(),findType);
}


void FindString::on_FindedString_textChanged(const QString &arg1)
{
    if(arg1.isEmpty())
    {
        ui->FindButton->setEnabled(false);
    }
    else
    {
        ui->FindButton->setEnabled(true);
    }
}

void FindString::on_CancelButton_clicked()
{
    this->close();
}

void FindString::getNotFind()
{
    ui->ReplaceButton->setEnabled(false);
    QMessageBox::warning(this, tr("Not Find"),
                         tr("Not Find \"\n%1\"").arg(ui->FindedString->text()));
}

void FindString::on_Up_clicked()
{
    findType = UPSEARCH;
}

void FindString::on_Down_clicked()
{
    findType = DOWNSEARCH;
}

void FindString::on_All_clicked()
{
    findType = WHOLESEARCH;
}

void FindString::on_ReplaceButton_clicked()
{
    emit findedStringReplace(ui->ReplaceString->text());
}

void FindString::focusOutEvent(QFocusEvent *event)
{
    ui->ReplaceButton->setEnabled(false);
}

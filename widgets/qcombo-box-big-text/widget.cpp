#include "widget.h"
#include "ui_widget.h"
#include <QDebug>
Widget::Widget(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::Widget)
{
    ui->setupUi(this);
    ui->label->setSizePolicy(QSizePolicy(QSizePolicy::Minimum, QSizePolicy::Preferred));
    ui->comboBox->setSizePolicy(QSizePolicy(QSizePolicy::Maximum, QSizePolicy::Preferred));
    ui->comboBox->addItem("1234567890123456789abcdefghijklmnopqrstuvx");


    setFixedSize(300, 300);
}

Widget::~Widget()
{
    delete ui;
}

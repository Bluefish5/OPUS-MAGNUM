#include "secondwindow.h"

SecondWindow::SecondWindow(QWidget *parent) : QWidget(parent)
{
    setWindowTitle("Drugie okno");
    resize(200, 100);

    QLabel *label = new QLabel("Hello World!", this);
    label->setAlignment(Qt::AlignCenter);

    QVBoxLayout *layout = new QVBoxLayout(this);
    layout->addWidget(label);
    setLayout(layout);
}

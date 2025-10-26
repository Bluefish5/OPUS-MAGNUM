#include "mainwindow.h"
#include "secondwindow.h"
#include "./ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
    , secondWindow(nullptr)
{
    ui->setupUi(this);



    button = new QPushButton("Otwórz okno", this);
    setCentralWidget(button);
    connect(button, &QPushButton::clicked, this, &MainWindow::openSecondWindow);
}

MainWindow::~MainWindow()
{
    delete ui;
}


void MainWindow::openSecondWindow()
{
    if (!secondWindow)
        secondWindow = new SecondWindow(); // nie dajemy parenta, żeby było niezależne
    secondWindow->show();
    secondWindow->raise();  // na wierzch
    secondWindow->activateWindow(); // aktywacja
}

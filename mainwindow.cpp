#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    createStatusBar();
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::createStatusBar()
{
    statusBar()->showMessage("Ready",4000);
}

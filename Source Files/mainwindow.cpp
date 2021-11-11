#include "string"
#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "code.h"

using namespace std;

string code, tokens, fileName = "tokens.txt";

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::on_pushButton_clicked()
{
    code = readFile(ui->TinyCode->toPlainText().toUtf8().constData());
    tokens = getTokens(code);
    ui->TokenList->setPlainText(QString::fromStdString(tokens));
    createFile(fileName);
    writeFile(tokens, fileName);
}

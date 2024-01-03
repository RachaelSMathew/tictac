#include "mainwindow.h"
#include "nextmove.h"
#include "./ui_mainwindow.h"
#include <QDebug>
#include <QTime>
#include <string>
#include <QTimer>
#include <iostream>

using namespace std;
using std::cin;

QVector<QPushButton *> buttons;
QString userName;
int userScore = 0;
int compScore = 0;

QTimer *t;

void delay()
{
    QTime dieTime= QTime::currentTime().addSecs(1);
    while (QTime::currentTime() < dieTime)
        QCoreApplication::processEvents(QEventLoop::AllEvents, 100);
}

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{

    ui->setupUi(this);
    ui->compScore->setVisible(false);
    ui->userScore->setVisible(false);
    buttons.append(ui->pushButton_1);
    buttons.append(ui->pushButton_2);
    buttons.append(ui->pushButton_3);
    buttons.append(ui->pushButton_4);
    buttons.append(ui->pushButton_5);
    buttons.append(ui->pushButton_6);
    buttons.append(ui->pushButton_7);
    buttons.append(ui->pushButton_8);
    buttons.append(ui->pushButton_9);
    for(QPushButton * button: buttons) {
        button->setVisible(false);
    }
    ui->pushButton->setVisible(false);
    ui->label->setVisible(false);
    ui->label_2->setText("");

    t = new QTimer(this);
    QTime timeThing = QTime(0,0,0,0);

    t->setInterval(1000);
    connect(t, &QTimer::timeout, [&]() {
        timeThing = timeThing.addSecs(1);
        ui->label_2->setText("Timer: "+timeThing.toString());
        if(ui->label_2->text() == "Timer: 00:03:00") {
            QApplication::quit();
        }
    } );
}

MainWindow::~MainWindow()
{
    delete ui;
}

string ticToNum(QString tic) {
    if(tic == "X") {
        return "1";
    }if(tic == "O") {
        return "-1";
    } else {
        return "0";
    }
}
string makeStr(Ui::MainWindow *ui) {
    string retArr = "";
    for(int i=1;i<4;i++) {
        for(int j=0;j<3;j++) {
            retArr.append(ticToNum(buttons[i+(j*3)-1]->text()));
        }
    }

    return retArr;
}

int gameOver(const string& ticStr) {

    string retArr[9] = {};
    int startIndex = 0;
    for(int i=0;i<9;i++) {
        if(ticStr[startIndex] == '-') {
            retArr[i] = ticStr.substr(startIndex, 2);
            startIndex+=2;
        } else {
            retArr[i] = ticStr.substr(startIndex, 1);
            startIndex+=1;
        }
    }

    bool over = (retArr[0] == "1" and retArr[4] == "1" and retArr[8] == "1");
    over = over or (retArr[2] == "1" and retArr[4] == "1" and retArr[6] == "1");
    if(over) {
        return 1;
    }

    over = (retArr[0] == "-1" and retArr[4] == "-1" and retArr[8] == "-1");
    over = over or (retArr[2] == "-1" and retArr[4] == "-1" and retArr[6] == "-1");
    if(over) {
        return -1;
    }

    for(int i=0;i<3;i++) {
        over = (retArr[3*i] == "1" and retArr[(3*i)+1] == "1" and retArr[(3*i)+2] == "1");
        over = over or (retArr[i] == "1" and retArr[i+3] == "1" and retArr[i+6] == "1");
        if(over) {
            return 1;
        }
        over = (retArr[3*i] == "-1" and retArr[(3*i)+1] == "-1" and retArr[(3*i)+2] == "-1");
        over = over or (retArr[i] == "-1" and retArr[i+3] == "-1" and retArr[i+6] == "-1");
        if(over) {
            return -1;
        }
    }
    //board game completely filled
    if(ticStr.find('0') >= ticStr.length()) {
        return 0;
    }

    return -100; //game's not over yet
}
void computerMoves(Ui::MainWindow *ui, int move_1, int move_2) {

    if(move_2 == 0) {
        buttons.at(1+move_1-1)->setText("O");
    }else if(move_2 == 1) {
        buttons.at(4+move_1-1)->setText("O");
    }else {
        buttons.at(7+move_1-1)->setText("O");
    }


}
void nextMove(QPushButton *b, Ui::MainWindow *ui) {

    if(b->text() == "") {
        b->setText("X");
    }

    for(QPushButton * button: buttons) {
        button->setEnabled(false);
    }


   //find next move
    int move[2] = {-1,-1};

    string answer = minimax(makeStr(ui), 1, -1, move);

    answer = answer.substr(answer.find(' ')+1);
    answer = answer.substr(answer.find(' ')+1);
    int retMove_1 = stoi(answer.substr(0, answer.find(' ')));
    answer = answer.substr(answer.find(' ')+1);
    int retMove_2 = stoi(answer.substr(0, answer.find(' ')));

    //have computer make next move
    computerMoves(ui, retMove_1, retMove_2);

    for(QPushButton * button: buttons) {
        button->setEnabled(true);
    }

    //check if game done
    if(gameOver(makeStr(ui)) != -100) {
        //say who won
        if(gameOver(makeStr(ui)) == -1) {
            compScore += 1;
            ui->compScore->setText("Computer Score: "+ QString::number(compScore));
            ui->label->setText("Computer won!");
        } else if(gameOver(makeStr(ui)) == 1) {
            userScore += 1;
            ui->userScore->setText(userName + " Score: "+QString::number(userScore));
            ui->label->setText(userName + " won!");
        } else {
            ui->label->setText("No one won!");
        }

        ui->label->setVisible(true);
        delay();
        for(QPushButton * button: buttons) {
            button->setText("");
        }
        ui->label->setVisible(false);
    }


}
void MainWindow::on_pushButton_4_clicked()
{
    nextMove(ui->pushButton_4, ui);

}


void MainWindow::on_pushButton_7_clicked()
{
    nextMove(ui->pushButton_7, ui);

}


void MainWindow::on_pushButton_2_clicked()
{
    nextMove(ui->pushButton_2, ui);
}


void MainWindow::on_pushButton_5_clicked()
{
    nextMove(ui->pushButton_5, ui);
}


void MainWindow::on_pushButton_8_clicked()
{
    nextMove(ui->pushButton_8, ui);
}


void MainWindow::on_pushButton_3_clicked()
{
    nextMove(ui->pushButton_3, ui);
}


void MainWindow::on_pushButton_6_clicked()
{
    nextMove(ui->pushButton_6, ui);
}


void MainWindow::on_pushButton_9_clicked()
{
    nextMove(ui->pushButton_9, ui);
}


void MainWindow::on_pushButton_1_clicked()
{
    nextMove(ui->pushButton_1, ui);
}


void MainWindow::on_pushButton_clicked()
{
    for(QPushButton * button: buttons) {
        button->setText("");
    }
    ui->label->setVisible(false);
}


void MainWindow::on_pushButton_10_clicked()
{
    userName = ui->lineEdit->text();
    ui->lineEdit->setVisible(false);
    ui->pushButton_10->setVisible(false);
    ui->label_3->setVisible(false);
    for(QPushButton * button: buttons) {
        button->setVisible(true);
    }
    ui->compScore->setVisible(true);
    ui->userScore->setVisible(true);
    ui->pushButton->setVisible(true);
    ui->userScore->setText(userName + " Score: "+QString::number(compScore));
    ui->compScore->setText("Computer Score: "+ QString::number(userScore));

    t->start();

}


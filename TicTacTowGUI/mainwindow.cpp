#include "mainwindow.h"
#include "nextmove.h"
#include "./ui_mainwindow.h"
#include <QDebug>
#include <QTime>
#include <QLabel>
#include <string>
#include <QTimer>
#include <iostream>
#include <map>
#include <chrono>

using namespace std;
using std::cin;

QVector<QPushButton *> buttons;
QString userName;
int userScore = 0;
int compScore = 0;
int dim = 3;
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
    for(int i=0;i<(dim*dim);i++) {
        retArr.append(ticToNum(buttons[i]->text()));
    }

    return retArr;
}

int countZero(const string& ticStr) {

    //board game (almost) completely filled
    int countZero = 0;
    for(int i=0;i<ticStr.length();i++) {
        if(ticStr[i] == '0') {
            countZero ++;
        }
    }
    return countZero;
}

std::map<std::string, double> overAcc;

double gameOver(const string& ticStr, bool acc) {
    double maxValNotOver = 0.0;

    if (overAcc.find(ticStr) != overAcc.end()) {
        return overAcc[ticStr];
    }
    double maxVal = 0.0;
    double prune = (1.0);

    QVector<string> retArr = {};

    int startIndex = 0;
    for(int i=0;i<dim*dim;i++) {
        if(ticStr[startIndex] == '-') {
            retArr.append(ticStr.substr(startIndex, 2));
            startIndex+=2;
        } else {
            retArr.append(ticStr.substr(startIndex, 1));
            startIndex+=1;
        }
    }

    double diag1 = 0;
    bool broken = false;
    for(int i=0;i<dim;i++) {
        if(retArr[(dim+1)*i] == "-1") {
            broken = true;
        }
        diag1 += retArr[(dim+1)*i] == "1" ? 1/(dim*1.0):0;
        if(broken) {
            diag1 = 0;
        }
    }
    double diag2 = 0;
    broken = false;
    for(int i=0;i<dim;i++) {
        if(retArr[(dim-1)*(i+1)] == "-1") {
            broken = true;
        }
        diag2 += retArr[(dim-1)*(i+1)] == "1" ?  1/(dim*1.0):0;
        if(broken) {
            diag2 = 0;
        }
    }

    if(diag1 >= prune) {
        maxVal = max(diag1, maxVal);
    }
    if(diag2 >= prune) {
        maxVal = max(diag2, maxVal);
    }
    maxValNotOver = max(maxValNotOver, diag1);
    maxValNotOver = max(maxValNotOver, diag2);
    double diag1_n = 0;
    broken = false;
    for(int i=0;i<dim;i++) {
        if(retArr[(dim+1)*i] == "1") {
            broken = true;
        }
        diag1_n -= retArr[(dim+1)*i]  == "-1" ? 1/(dim*1.0):0;
        if(broken) {
            diag1_n = 0;
        }
    }
    double diag2_n = 0;
    broken = false;
    for(int i=0;i<dim;i++) {
        if(retArr[(dim-1)*(i+1)] == "1") {
            broken = true;
        }
        diag2_n -= retArr[(dim-1)*(i+1)]  == "-1" ? 1/(dim*1.0):0;
        if(broken) {
            diag2_n = 0;
        }
    }

    maxValNotOver = max(abs(maxValNotOver), abs(diag1_n));
    maxValNotOver = max(abs(maxValNotOver), abs(diag2_n));

    if(diag1_n <= -1*prune) {
        maxVal = abs(diag1_n) > abs(maxVal) ? diag1_n : maxVal;
    }
    if(diag2_n <= -1*prune) {
        maxVal = abs(diag2_n) > abs(maxVal) ? diag2_n : maxVal;
    }

    int winners[] = {1,-1};
    for(int k=0;k<2;k++) {
        for(int i=0;i<dim;i++) {
            double row = 0;
            double col = 0;
            broken = false;
            for(int j=0;j<dim;j++) {
                if(retArr[(dim*i)+j] == to_string(winners[k==0?1:0])) {
                    broken = true;
                }
                row += retArr[(dim*i)+j]  == to_string(winners[k]) ? winners[k]/(dim*1.0):0;

                if(broken) {
                    row = 0;
                }

            }

            broken = false;
            for(int j=0;j<dim;j++) {
                if(retArr[i+(dim*j)] == to_string(winners[k==0?1:0])) {
                    broken = true;
                }
                col += retArr[i+(dim*j)]  == to_string(winners[k]) ? winners[k]/(dim*1.0):0;
                if(broken) {
                    col = 0;
                }
            }

            if(abs(col) >= abs(winners[k]*prune) or fabs((winners[k]*prune)) - fabs(col) < .00001) {
                maxVal = abs(col) > abs(maxVal) ? col : maxVal;
            }
            if(abs(row) >= abs(winners[k]*prune) or fabs((winners[k]*prune)) - fabs(row) < .00001) {
                maxVal = abs(row) > abs(maxVal) ? row : maxVal;
            }
            maxValNotOver = max(abs(maxValNotOver), abs(col));
            maxValNotOver = max(abs(maxValNotOver), abs(row));
        }
    }

    if(maxVal != 0.0) {
        overAcc[ticStr] = maxVal;
        return maxVal;
    }

    if(countZero(ticStr) == 0) {
        overAcc[ticStr] = 0.0;
       return 0.0;
    }

    overAcc[ticStr] = maxValNotOver;

    return maxValNotOver; //game's not over yet
}

void computerMoves(Ui::MainWindow *ui, int move_1, int move_2) {
    if((move_1*dim)+move_2>=0 and (move_1*dim)+move_2<dim*dim) {
        buttons.at((move_1*dim)+move_2)->setText("O");
    }
}

void checkOver(Ui::MainWindow *ui) {
    double overVal = gameOver(makeStr(ui), true);

    if(overVal == 0 or overVal == -1 or overVal == 1) {
        //say who won
        if(overVal == -1) {
            compScore += 1;
            ui->compScore->setText("Computer Score: "+ QString::number(compScore));
            ui->label->setText("Computer won!");
        } else if(overVal == 1) {
            userScore += 1;
            ui->userScore->setText(userName + " Score: "+QString::number(userScore));
            ui->label->setText(userName + " won!");
        } else if(overVal == 0) {
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
void nextMove(QPushButton *b, Ui::MainWindow *ui) {

    if(b->text() == "") {
        b->setText("X");
    }

    for(QPushButton * button: buttons) {
        button->setEnabled(false);
    }

    int retMove_1 = 0;
    int retMove_2 = 0;
    string answer = "";

    //find next move
    auto start = std::chrono::system_clock::now();
    int move[2] = {-1,-1};
    int maxNum = std::numeric_limits<int>::max();
    int minNum = std::numeric_limits<int>::min();
    answer = minimax(makeStr(ui), 1, -1, move, dim, true, std::numeric_limits<double>::min(), std::numeric_limits<double>::max());
    auto end = std::chrono::system_clock::now();
    std::chrono::duration<double> elapsed_seconds = end-start;
    qDebug() << "Elasped time to find next move: " <<elapsed_seconds;

    answer = answer.substr(answer.find(' ')+1);
    answer = answer.substr(answer.find(' ')+1);
    retMove_1 = stoi(answer.substr(0, answer.find(' ')));
    answer = answer.substr(answer.find(' ')+1);
    retMove_2 = stoi(answer.substr(0, answer.find(' ')));

    //have computer make next move
    computerMoves(ui, retMove_1, retMove_2);

    for(QPushButton * button: buttons) {
        button->setEnabled(true);
    }
    if(countZero(makeStr(ui)) <= (dim*dim)-(dim*2)+1) {
        //check if game done
        checkOver(ui);
    }

}

void MainWindow::on_pushButton_clicked()
{
    for(QPushButton * button: buttons) {
        button->setText("");
    }
    ui->label->setVisible(false);
}

void MainWindow::userMove() {
    QPushButton* button = qobject_cast<QPushButton*>(sender());
    nextMove(button, ui);
}

void MainWindow::MakeBoard() {

    for(int i=0;i<dim;i++) {
        for(int j=0;j<dim;j++) {
            QPushButton *butt = new QPushButton(QString(""));
            ui->gridLayout->addWidget(butt, i,j);
            int heightWidth = 100;
            if(dim > 3) {
                heightWidth = 50;
            }
            const QSize BUTTON_SIZE = QSize(heightWidth, heightWidth);
            butt->setMinimumSize(BUTTON_SIZE);
            QObject::connect(
                butt, &QPushButton::clicked,
                this, &MainWindow::userMove);
            buttons.append(butt);
        }
    }
}

void MainWindow::on_pushButton_10_clicked()
{
    userName = ui->lineEdit->text();
    ui->lineEdit->setVisible(false);
    ui->pushButton_10->setVisible(false);
    ui->spinBox->setVisible(false);
    ui->label_4->setVisible(false);
    ui->label_3->setVisible(false);

    ui->compScore->setVisible(true);
    ui->userScore->setVisible(true);
    ui->pushButton->setVisible(true);
    ui->userScore->setText(userName + " Score: "+QString::number(compScore));
    ui->compScore->setText("Computer Score: "+ QString::number(userScore));
    dim = ui->spinBox->value();
    MakeBoard();
    t->start();

}


void MainWindow::on_pushButton_11_clicked()
{
    QCoreApplication::quit();
}


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

QVector<QPushButton *> buttons; // array of each tile on the tic tac toe board 
QString userName; 
int userScore = 0; // user's score 
int compScore = 0; // computer's score
int dim = 3; // dimension (2-6) user choosed 
QTimer *t; // timer limit of 3 minute 

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

    ui->pushButton->setVisible(false); // reset button to reset the grid 
    ui->label->setVisible(false); // label for saying who won game 
    ui->label_2->setText(""); // label for timer 
    
    //creates a 3 minute timer that closes the app after it's completed 
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

// converts the array of button tiles to a string where X is 1 and O is -1 and empty space is 0
// converting into string right to left, going down 
string makeStr(Ui::MainWindow *ui) { 
    string retArr = "";
    for(int i=0;i<(dim*dim);i++) {
        retArr.append(ticToNum(buttons[i]->text()));
    }

    return retArr;
}

// counts the amount of empty spaces in a board 
int countZero(const string& ticStr) {

    int countZero = 0;
    for(int i=0;i<ticStr.length();i++) {
        if(ticStr[i] == '0') {
            countZero ++;
        }
    }
    return countZero;
}

std::map<std::string, double> overAcc; // stores the previous game boards their values 

double gameOver(const string& ticStr, bool acc) {
    double maxValNotOver = 0.0;

    if (overAcc.find(ticStr) != overAcc.end()) { // checks if ticStr already has been passed in gameOver 
        return overAcc[ticStr];
    }
    
    double maxVal = 0.0;
    double prune = (1.0);
    
    // converts a string of the tic tac toe board into an array where each index is a value on the board 
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
    
    // checks the 2 diagonals of the board game
    // if -1 exists in tile then -1/dim is added to diagonals value
    // if 1 exists in tile then 1/dim is added to diagonals value
    // calculates the 2 diagonals value for player X and O repsectively
    // while calculating the diagonal for one player, if you find the other player, the diagonal becomes "broken" or 0
    // BROKEN: X X O X X
    // UNBROKEN: X X X X X
    
    // sum of the diagonal 1 for player X 
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
    // sum of the diagonal 2 for player X 
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
    
    // sum of the diagonal 1 for player O
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
    // sum of the diagonal 2 for player O 
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

    // calculates maximum UNbroken row/column/diagonal for player X or O 
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

    if(maxVal != 0.0) { // either player or computer has computer filled up a diagonal, row, or coloumn 
        overAcc[ticStr] = maxVal;
        return maxVal;
    }

    if(countZero(ticStr) == 0) { // all spaces are filled 
        overAcc[ticStr] = 0.0;
       return 0.0;
    }

    overAcc[ticStr] = maxValNotOver;

    return maxValNotOver; //game's not over yet, but calculates the row/column/diagonal with the largest(absolute) UNbroken value for either player X or O
}

void computerMoves(Ui::MainWindow *ui, int move_1, int move_2) {
    if((move_1*dim)+move_2>=0 and (move_1*dim)+move_2<dim*dim) {
        buttons.at((move_1*dim)+move_2)->setText("O");
    }
}

//checks if the game is over, and if so, updates the score and clears the grid 
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
        //clears the board 
        for(QPushButton * button: buttons) {
            button->setText("");
        }
        ui->label->setVisible(false);
    }
}

// adds an X to the move a player makes and finds the next best move for the computer to make 
void nextMove(QPushButton *b, Ui::MainWindow *ui) {

    if(b->text() == "") {
        b->setText("X");
    }
    
    // disables all the tile buttons while the comp finds the next move 
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
    retMove_1 = stoi(answer.substr(0, answer.find(' '))); // x coordinate of best move for comp(0 indexed) 
    answer = answer.substr(answer.find(' ')+1);
    retMove_2 = stoi(answer.substr(0, answer.find(' '))); // y coordinate of best move for comp(0 indexed) 

    // computer makes next move
    computerMoves(ui, retMove_1, retMove_2);

    for(QPushButton * button: buttons) {
        button->setEnabled(true);
    }
    if(countZero(makeStr(ui)) <= (dim*dim)-(dim*2)+1) {
        //check if game done
        checkOver(ui);
    }

}

// when reset button is clicked, empties the entire board 
void MainWindow::on_pushButton_clicked()
{
    for(QPushButton * button: buttons) {
        button->setText("");
    }
    ui->label->setVisible(false);
}

// adds an X to the tile the user chooses 
void MainWindow::userMove() {
    QPushButton* button = qobject_cast<QPushButton*>(sender());
    nextMove(button, ui);
}

// creates a tic tac toe board game based on the dimension the user chose 
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

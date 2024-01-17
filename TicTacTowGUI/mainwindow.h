#ifndef MAINWINDOW_H
#define MAINWINDOW_H
#include <string>
#include <iostream>
#include <QMainWindow>


using namespace std;
using std::cin;

double gameOver(const string& ticStr, bool acc=false);
int countZero(const string& ticStr);

QT_BEGIN_NAMESPACE
namespace Ui {
class MainWindow;
}
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private slots:
    void MakeBoard();

    void userMove();

    void on_pushButton_clicked();

    void on_pushButton_10_clicked();

    void on_pushButton_11_clicked();

private:
    Ui::MainWindow *ui;
};
#endif // MAINWINDOW_H

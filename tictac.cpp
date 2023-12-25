#include <string>
#include <iostream>
#include <string.h>
#include <stdio.h>
using namespace std;
using std::cin;

int gameOver(int tic[3][3]) {
    string ticStr = "";
    for(int i=0;i<3;i++) {
        for(int j=0;j<3;j++) {
            ticStr += tic[i][j];
        }
    }
    bool over = ticStr.find("111") != string::npos;
    over = ""+ticStr.at(0) == "1" and ""+ticStr.at(4) == "1" and ""+ticStr.at(8) == "1";
    over = ""+ticStr.at(2) == "1" and ""+ticStr.at(4) == "1" and ""+ticStr.at(6) == "1";
    if(over) {
        return 1;
    }
    over = ticStr.find("-1-1-1") != string::npos;
    over = ""+ticStr.at(0) == "-1" and ""+ticStr.at(4) == "-1" and ""+ticStr.at(8) == "-1";
    over = ""+ticStr.at(2) == "-1" and ""+ticStr.at(4) == "-1" and ""+ticStr.at(6) == "-1";
    if(over) {
        return -1;
    }

    for(int i=0;i<3;i++) {
        over = ""+ticStr.at(i) == "1" and ""+ticStr.at(i+3) == "1" and ""+ticStr.at(i+6) == "1";
        if(over) {
            return 1;
        }
        over = ""+ticStr.at(i) == "-1" and ""+ticStr.at(i+3) == "-1" and ""+ticStr.at(i+6) == "-1";
        if(over) {
            return -1;
        }
    }
    return 0;
}

/*
return int[4] = who won, depth, move[0], move[1] 
*/
int* minimax(int arr[3][3], int depth, int turn, int move[2]) {
    int isOver = gameOver(arr);
    if(isOver != 0) {
        static int retArr[4] = {isOver, depth, move[0], move[1]};
        return retArr;
    }

    int lowestDep = INT_MAX;
    int whoWon = 1;

    for(int i=0;i<3;i++) {
        for(int j=0;j<3;j++) {
            if(arr[i][j] == 0) {
                int moveNext[2] = {i,j};
                arr[i][j] = turn;
                static int* retArr = minimax(arr, depth+1, turn == 1 ? -1 : 1, moveNext);
                if(retArr[0] == -1 and lowestDep > retArr[1]) {
                    whoWon = -1;
                    lowestDep = retArr[1];
                    move[0] = retArr[2];
                    move[1] = retArr[3];
                }

            }
        }
    }
    static int retArr[4] = {whoWon,lowestDep,move[0],move[1]};
    return retArr;
}

int main () {
    //user is 1(max) X
    //computer is -1(min) O

    int  tic[3][3] = {0};
    while(gameOver(tic) == 0) {
        string nextMove;
        //printing board \t is a tab 
        for(int i=0;i<3;i++)
        {
            for(int j=0;j<3;j++)
            {
                string printVar;
                switch(tic[i][j]) {
                case 1:
                    printVar = "X";
                    break;
                case -1:
                    printVar = "O";
                    break;
                default:
                    printVar = "";
                }

                cout<<"   "<<printVar<<"   ";
                if(j==0 or j==1) {
                    cout<<"|";
                }
                if(j==2) {
                    cout<<endl;
                }
            }
            if(i==0 or i==1) {
                cout<<"-------------------"<<endl;
            }
        } 
        cout<<"What's your move? (format: x, y)\n";
        cin>>nextMove;
        int x = stoi(nextMove.substr (0, nextMove.find(",")));
        int y = stoi(nextMove.substr (nextMove.find(",")+1));

        if(tic[x][y] != -1) {
            tic[x][y] = 1;
        }
    }
    // depth = 0 when game board is empty 
    int move[2] = {-1,-1};
    int* answer = minimax(tic, 2, -1, move);
    
    exit(0);
}

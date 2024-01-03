#include "nextmove.h"
#include "mainwindow.h"
#include <QString>
#include <string>
#include <iostream>

using namespace std;
using std::cin;

nextMove::nextMove() {}

string minimax(string arr, int depth, int turn, int move[2]) {
    int isOver = gameOver(arr);
    if(isOver != -100) {
        return to_string(isOver) + " " + to_string(depth) + " " + to_string(move[0]) + " " + to_string(move[1]);
    }

    int lowestDep = INT_MAX;
    int whoWon = turn == -1 ? 100 : -100; //no one won
    int newMoves[2] = {0,0};
    int num = 0;

    for(int i=0;i<arr.length();i++) {
        if(arr[i] == '1') {
            num += 1;
        }
        else if(arr[i] == '0') {
            num+=1;
            int moveNext[2] = {(num-1)/3,(num-1)%3};
            string copyTic = arr.substr(0, i) + to_string(turn) + arr.substr(i+1);

            string retArr = minimax(copyTic, depth+1, turn == 1 ? -1 : 1, moveNext);
            if(depth ==1) {
                cout << retArr<<"okay" << num<<". "<<endl;
            }
            //returns which player won
            int retTurn = stoi(retArr.substr(0, retArr.find(' ')));
            retArr = retArr.substr(retArr.find(' ')+1);

            //returns the path with smallest depth
            int retLowest = stoi(retArr.substr(0, retArr.find(' ')));

            // if turn O, then finding minimum return value
            // if turn X, then finding maximum return value
            if(
                (turn == -1 and retTurn < whoWon) or (turn == 1 and retTurn > whoWon)
                or (retTurn == whoWon and lowestDep > retLowest)
                ) {
                whoWon = retTurn;
                lowestDep = retLowest;
                newMoves[0] = (num-1)/3;
                newMoves[1] = (num-1)%3;
            }
        }
    }

    return to_string(whoWon) + " " + to_string(lowestDep) + " " + to_string(newMoves[0]) + " " + to_string(newMoves[1]);
}

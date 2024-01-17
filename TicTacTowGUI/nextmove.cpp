#include "nextmove.h"
#include "mainwindow.h"
#include <QString>
#include <string>
#include <iostream>

using namespace std;
using std::cin;

nextMove::nextMove() {}
/*
 * returns:
 * whoWon(closer to -1-player O won, closer to 1-player X won)
 * depth: amount of moves to get to winning game board
 * move_1: x coordinate (0 indexed)
 * move_2: y coordinate (0 indexed)
 */
string minimax(string arr, int depth, int turn, int move[2], int dim, bool acc, double alpha, double beta) {
    double bestVal = turn == 1 ? std::numeric_limits<double>::min() : std::numeric_limits<double>::max();

    // value of board game (-1 if O won, 1 if X won and 0 if no one won)
    double isOver = gameOver(arr, acc);
    if(isOver ==0 or isOver == 1 or isOver == -1
        or ((dim == 4 or dim == 5 or countZero(arr) <= dim*3) and depth >= 8) //restrict depth to optimize runtime
        or (dim == 6 and countZero(arr) > dim*3 and depth >=5)) {
        return to_string(isOver) + " " + to_string(depth) + " " + to_string(move[0]) + " " + to_string(move[1]);
    }

    int lowestDep = INT_MAX;
    double whoWon = turn == -1 ? 100.0 : -100.0;
    int newMoves[2] = {0,0};
    int num = 0;

    for(int i=0;i<arr.length();i++) {
        if(arr[i] == '1') {
            num += 1;
        }
        else if(arr[i] == '0') { // finds the empty spaces in an tic tac toe board
            num+=1;
            int moveNext[2] = {(num-1)/dim,(num-1)%dim};
            string copyTic = arr.substr(0, i) + to_string(turn) + arr.substr(i+1); // adds a value (-1-player O or 1-player X) to the board game

            string retArr = minimax(copyTic, depth+1, turn == 1 ? -1 : 1, moveNext, dim, false, alpha, beta); // recursive minimax

            //returns which player won
            double retTurn = stod(retArr.substr(0, retArr.find(' ')));

            retArr = retArr.substr(retArr.find(' ')+1);

            //returns the path with smallest depth
            int retLowest = stoi(retArr.substr(0, retArr.find(' ')));

            // if turn O, then finding minimum return value
            // if turn X, then finding maximum return value
            if(
                (turn == -1 and retTurn < whoWon) or (turn == 1 and retTurn > whoWon)
                or (retTurn == whoWon and lowestDep > retLowest) // finds the returned value with the lowest depth (nearest solution)
                ) {
                whoWon = retTurn;
                lowestDep = retLowest;
                newMoves[0] = (num-1)/dim;
                newMoves[1] = (num-1)%dim;
            }

            //alpha beta pruning
            bestVal = turn == 1?max(bestVal, retTurn):min(bestVal, retTurn);
            if(turn == 1) {
                alpha = max(alpha, bestVal);
            } else {
                beta = min(beta, bestVal);
            }
            if (beta <= alpha) {
                break;
            }




        }
    }
    return to_string(whoWon) + " " + to_string(lowestDep) + " " + to_string(newMoves[0]) + " " + to_string(newMoves[1]);
}

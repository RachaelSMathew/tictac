#include <string>
#include <iostream>
#include <algorithm>

using namespace std;
using std::cin;

string makeStr(int tic[3][3]) {
    string retArr;
    for(int i=0;i<3;i++) {
        for(int j=0;j<3;j++) {
            retArr += to_string(tic[i][j]);
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

/*
return int[4] = who won, depth, move[0], move[1]
*/
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

int main () {

    //user is 1(max) X
    //computer is -1(min) O
    //-1,-1,1,1,0,0,0,-1,0
    int  tic[3][3] = {0};
    while(gameOver(makeStr(tic)) == -100) {
        string nextMove;
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
                cout<<"----------------------"<<endl;
            }
        }
        bool validMove = false;
        string getMoveStr = "What's your move? (format: x,y) 0-indexed\n";
        while(!validMove) {
            cout << getMoveStr;

            cin >> nextMove;
            int x = stoi(nextMove.substr(0, nextMove.find(',')));
            int y = stoi(nextMove.substr(nextMove.find(',') + 1));

            if (x<3 and y<3 and tic[x][y] == 0) {
                tic[x][y] = 1;
                validMove = true;
            }
            if(!validMove) {
                getMoveStr = "Please enter a valid coordinate that isn't filled up";
            }
        }
        if(gameOver(makeStr(tic)) == 1) {
            cout << "You won" <<endl;
            return 0;
        }
        // depth = 0 when game board is empty
        int move[2] = {-1,-1};
        string answer = minimax(makeStr(tic), 1, -1, move);

        int retTurn = stoi(answer.substr(0, answer.find(' ')));
        answer = answer.substr(answer.find(' ')+1);
        answer = answer.substr(answer.find(' ')+1);
        int retMove_1 = stoi(answer.substr(0, answer.find(' ')));
        answer = answer.substr(answer.find(' ')+1);
        int retMove_2 = stoi(answer.substr(0, answer.find(' ')));

        tic[retMove_1][retMove_2] = -1;

    }

    exit(0);
}

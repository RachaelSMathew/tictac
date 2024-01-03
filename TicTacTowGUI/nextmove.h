#include <QString>
#ifndef NEXTMOVE_H
#define NEXTMOVE_H
#include <string>
#include <iostream>

using namespace std;
using std::cin;

class nextMove
{
public:
    nextMove();
};

string minimax(string arr, int depth, int turn, int move[2]);

#endif // NEXTMOVE_H

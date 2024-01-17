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

string minimax(string arr, int depth, int turn, int move[2], int dim, bool acc=false, double alpha=std::numeric_limits<double>::min(), double beta=std::numeric_limits<double>::max());

#endif // NEXTMOVE_H

/****************************************************************************************
*  Benjamin Liu
*  besaliu
*  2024 Winter CSE101 PA5
*  Shuffle.cpp
*  Implementation file for Perfict Shuffle
*****************************************************************************************/

#include <stdexcept>
#include <iomanip>
#include "List.h"
using namespace std;


void shuffle(List& D){
    List leftHalf, rightHalf;
    D.moveFront();
    while (D.position() < (D.length() / 2)){
        leftHalf.insertBefore(D.moveNext());
    }
    while (D.position() < D.length()){
        rightHalf.insertBefore(D.moveNext());
    }
    D.clear();
    leftHalf.moveFront();
    rightHalf.moveFront();
    D.moveFront();
    while(leftHalf.position() < leftHalf.length() && rightHalf.position() < rightHalf.length()){
        D.insertBefore(rightHalf.moveNext());
        D.insertBefore(leftHalf.moveNext());
    }

    while(leftHalf.position() < leftHalf.length()){
        D.insertBefore(leftHalf.moveNext());
    }

    while(rightHalf.position() < rightHalf.length()){
        D.insertBefore(rightHalf.moveNext());
    }
}

int main(int argc, char *argv[]){

   if (argc != 2){
    cout << "Expected Usage: Shuffle <int>" << endl;
    return 1; // Exiting with an error code
}

int size = atoi(argv[1]);
if (size <= 0){
    cout << "Invalid deck size" << endl;
    return 1; // Exiting with an error code
}


    cout << "deck size       shuffle count" << endl;
    cout << "------------------------------" << endl;
    List deck;
    List copy;
    int numShuffles;
    for (int i = 1; i <= size; i++){
        deck.moveBack();
        deck.insertBefore(i);
        numShuffles = 0;
        copy = deck;
        shuffle(deck);
        numShuffles++;
        while(!(copy == deck)){
            shuffle(deck);
            numShuffles++;
        }
        cout << " " << i<< setw(16) << numShuffles << endl;

    }
}

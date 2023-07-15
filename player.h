#ifndef PLAYER
#define PLAYER
#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <queue>
#include <cstdlib>
#include <climits>

// Struktura reprezentująca punkt na mapie gry
struct Point {
    int x;
    int y;
    int type;
};

// Klasa bazowa reprezentująca jednostkę w grze
class Unit
{
private:
    char S; // Strona
    char T; // Typ jednostki
    int ID; // Identyfikator
    int X; // Współrzędna x
    int Y; // Współrzędna y
    int W; // Wytrzymałość
    int B; // Czas budowy
    int P; // Prędkość
    int K; // Koszt jednostki
    int Z; // Zasięg ataku
public:
    // Konstruktor
    Unit(char s, char t, int id, int x, int y, int w, int b, int p, int k, int z) : S(s), T(t), ID(id), X(x), Y(y), W(w), B(b), P(p), K(k), Z(z) {}

    // Metody
    char getS() const { return S; }
    char getT() const { return T; }
    int getID() const { return ID; }
    int getX() const { return X; }
    int getY() const { return Y; }
    int getW() const { return W; }
    int getB() const { return B; }
    int getP() const { return P; }
    int getK() const { return K; }
    int getZ() const { return Z; }
    // Destruktor
    ~Unit() {}
};

#endif

#ifndef PLAYER_H
#define PLAYER_H
#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <queue>
#include <cstdlib>
#include <climits>
#include <map>

// Struktura reprezentująca punkt na mapie gry
struct Point
{
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
    char B; // Czas budowy/Aktualnie budowana jednostka
    int ID; // Identyfikator
    int X;  // Współrzędna x
    int Y;  // Współrzędna y
    int W;  // Wytrzymałość
    int P;  // Prędkość
    int K;  // Koszt jednostki
    int Z;  // Zasięg ataku
public:
    // Konstruktor
    Unit(char s, char t, int id, int x, int y, int w, char b, int p, int k, int z) : S(s), T(t), ID(id), X(x), Y(y), W(w), B(b), P(p), K(k), Z(z) {}

    // Metody
    char getS() const { return S; }
    char getT() const { return T; }
    char getB() const { return B; }
    int getID() const { return ID; }
    int getX() const { return X; }
    int getY() const { return Y; }
    int getW() const { return W; }
    int getP() const { return P; }
    int getK() const { return K; }
    int getZ() const { return Z; }
    void setB(char b) { B = b; }
    void setX(int x) { X = x; }
    void setY(int y) { Y = y; }
    void setW(int w) { W = w; }
    // Metody rozkazow zakladaja ze warunki do wykonania rozkazu sa spelnione
    void Attack(Unit &Att, Unit &Def);
    void Move(Unit &_U, int x, int y);
    void Build(Unit &_U, char b);
    // Destruktor
    ~Unit() {}
};

#endif

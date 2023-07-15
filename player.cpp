#include "player.h" // Dołączenie pliku nagłówkowego "player.h", który zawiera definicje jednostek
void Unit::Attack(Unit &_Att,Unit &_Def)
{
    int A,D;
    std::vector<std::vector<int>> AttackTable = {
        {35, 35, 35, 35, 35, 50, 35, 35},
        {30, 30, 30, 20, 20, 30, 30, 30},
        {15, 15, 15, 15, 10, 10, 15, 15},
        {35, 15, 15, 15, 15, 10, 15, 10},
        {40, 40, 40, 40, 40, 40, 40, 50},
        {10, 10, 10, 10, 10, 10, 10, 50},
        {5, 5, 5, 5, 5, 5, 5, 1}
    };
    enum class Type
    {
        K,S,A,P,C,R,W
    };
    std::map<Type,char> TypeTable = {
        {Type::K,'K'},
        {Type::S,'S'},
        {Type::A,'A'},
        {Type::P,'P'},
        {Type::C,'C'},
        {Type::R,'R'},
        {Type::W,'W'}
    };


    if(_Att.Z>=abs(_Att.X-_Def.X)+abs(_Att.Y-_Def.Y))
    {
        _Def.W-=AttackTable[static_cast<int>(TypeTable[static_cast<Type>(_Att.T)])][static_cast<int>(TypeTable[static_cast<Type>(_Def.T)])];
    }
    else
    {
        std::cout<<"Nie mozna zaatakowac jednostki o ID: "<<_Def.ID<<std::endl;
    }
}
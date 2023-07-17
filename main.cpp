#include "player.h" // Dołączenie pliku nagłówkowego "player.h", który zawiera definicje jednostek

std::vector<std::vector<Point>> MapMaker(const std::string &filename); // Deklaracja funkcji MapMaker do tworzenia mapy

void StatusReader(const std::string &filename, long &gold, std::vector<Unit *> &PUnits, std::vector<Unit *> &EUnits, std::vector<Unit *> &Units); // Deklaracja funkcji StatusReader do odczytu stanu jednostek

void OrdersProcessor(const std::string &filename, std::vector<Unit *> &PUnits, std::vector<Unit *> &EUnits, std::vector<Unit *> &Units); // Deklaracja funkcji OrdersProcessor do odczytu rozkazow

Unit *UnitFinder(std::vector<Unit *> &Units, int id); // Deklaracja funkcji UnitFinder do wyszukiwania jednostki po ID

bool checkIfPointIsValid(std::vector<std::vector<Point>> &Map, int x, int y); // Deklaracja funkcji checkIfPointIsValid do sprawdzania czy punkt jest na mapie

std::vector<std::pair<int, int>> PathFinder(std::vector<std::vector<Point>> &Map, int start_x, int start_y, int end_x, int end_y); // Deklaracja funkcji PathFinder do znajdowania najkrotszej sciezki

std::vector<std::vector<Point>> enemyUnitsMap(const std::vector<std::vector<Point>> &Map, std::vector<Unit *> &EUnits); // Deklaracja funkcji enemyUnitsMap do tworzenia mapy jednostek przeciwnika

void baseFinder(const std::vector<std::vector<Point>> &Map, int &PBaseX, int &PBaseY, int &EBaseX, int &EBaseY);  // Deklaracja funkcji baseFinder do znajdowania baz gracza i przeciwnika

void mineFinder(const std::vector<std::vector<Point>> &Map, int &MineX, int &MineY); // Deklaracja funkcji mineFinder do znajdowania kopalni

void NewUnit(char t, std::vector<Unit *> &Units, Unit *PlayerBaseUnit); // Deklaracja funkcji NewUnit do tworzenia nowej jednostki

bool checkIfEnemyIsNearby(std::vector<Unit *> &Units, Unit *UnitA,Unit* EnemyUnit); // Deklaracja funkcji checkIfEnemyIsNearby do sprawdzania czy w poblizu znajduje sie przeciwnik

Unit *EnemyFinderByXY(std::vector<Unit *> &Units, int x, int y); // Deklaracja funkcji EnemyFinderByXY do znajdowania przeciwnika po wspolrzednych

int main(int argc, char *argv[]) // Deklaracja funkcji main
{
    /////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    // Sprawdzenie poprawnosci argumentow
    /////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    // argv[0]=player argv[1]=mapa argv[2]=status argv[3]=rozkazy argv[4]=limit czasowy
    // sprawdzenie czy podano odpowiednia liczbe argumentow
    if (argc < 4)
    {
        std::cerr << "Niewystarczajaca liczba argumentow"
                  << "\n";
        exit(EXIT_FAILURE);
    }
    // sprawdzanie czy odpowiednie pliki istnieja
    std::ifstream fin;
    for (int file = 1; file < argc - 1; file++)
    {
        fin.open(argv[file], std::ios::in);

        if (!fin.is_open())
        {
            std::cerr << "Blad przy otwarciu pliku: " << argv[file] << '\n';
            fin.clear();
            continue;
        }

        fin.clear();
    }
    int limitCzasowy = 5;
    if (argc > 4)
        limitCzasowy = std::atoi(argv[4]);
    /////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    // Przygotowanie danych
    /////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    std::vector<std::vector<Point>> Map = MapMaker(argv[1]); // Wywołanie funkcji MapMaker do utworzenia mapy
    std::vector<Unit *> PUnits;                              // Wektor wskaźników na jednostki gracza
    std::vector<Unit *> EUnits;                              // Wektor wskaźników na jednostki przeciwnika=
    long gold;
    gold = -1;
    std::vector<Unit *> Units;
    StatusReader(argv[2], gold, PUnits, EUnits, Units); // Wektor rozkazów jednostek gracza
    OrdersProcessor(argv[3], PUnits, EUnits, Units); 
    Unit *EnemyBaseUnit;
    for (int i = 0; i < EUnits.size(); i++)
    {
        if (EUnits[i]->getT() == 'B' - 0)
        {
            EnemyBaseUnit = EUnits[i];
            break;
        }
    }
    Unit *PlayerBaseUnit;
    for (int i = 0; i < PUnits.size(); i++)
    {
        if (PUnits[i]->getT() == 'B' - 0)
        {
            PlayerBaseUnit = PUnits[i];
            break;
        }
    }
    /////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    // Logika
    /////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    int PBaseX, PBaseY, EBaseX, EBaseY;
    PBaseX = PBaseY = EBaseX = EBaseY = -1;
    baseFinder(Map, PBaseX, PBaseY, EBaseX, EBaseY);
    int MineX, MineY;
    MineX = MineY = -1;
    mineFinder(Map, MineX, MineY);
    std::vector<std::vector<Point>> enemyUnits = enemyUnitsMap(Map, EUnits); // mapa jednostek przeciwnika
    std::vector<std::pair<int, int>> enemybasepath = PathFinder(enemyUnits, PBaseX, PBaseY, EBaseX, EBaseY); // sciezka do bazy przeciwnika
    std::vector<std::pair<int, int>> minepath = PathFinder(enemyUnits, PBaseX, PBaseY, MineX, MineY); // sciezka do kopalni
    /////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    // Przygotowanie rozkazow
    /////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    int BuildFlag = 0;
    std::ofstream fout;
    fout.open(argv[3], std::ios::out | std::ios::trunc);
    if (!fout.is_open())
    {
        std::cerr << "Blad przy otwarciu pliku: rozkazy.txt"
                  << "\n";
        fout.clear();
        exit(EXIT_FAILURE);
    }
    for (int i = 0; i < PUnits.size(); i++)
    {
        if (PUnits[i]->getT() == 'B')
        {
            if (PUnits[i]->getB() == '0')
            {
                if (gold >= 1000)
                {
                    fout << PUnits[i]->getID() << " B K" << '\n';
                    BuildFlag = 1;
                    gold -= 400;
                }
                else
                {
                    fout << PUnits[i]->getID() << " B W" << '\n';
                    BuildFlag = 1;
                    gold -= 100;
                }
            }
        }
        else
        {
            if (PUnits[i]->getT() == 'W' - 0)
            {
                if (PUnits[i]->getX() == MineX && PUnits[i]->getY() == MineY)
                {
                    gold += 50;
                    continue;
                }
                std::vector<std::pair<int, int>> path = PathFinder(enemyUnits, PUnits[i]->getX(), PUnits[i]->getY(), MineX, MineY);
                if (path.size() == 0)
                {
                    std::cout << "Nie mozna znalezc sciezki dla robotnika\n";
                    std::cout << PUnits[i]->getID() << " " << PUnits[i]->getX() << " " << PUnits[i]->getY() << " " << MineX << " " << MineY << std::endl;
                }
                else
                {
                    if (PUnits[i]->getP() >= abs(PUnits[i]->getX() - MineX) + abs(PUnits[i]->getY() - MineY))
                    {
                        fout << PUnits[i]->getID() << " M " << MineX << " " << MineY << '\n';
                    }
                    else
                    {
                        if (enemyUnits[path[0].first][path[0].second].type == 3)
                        {
                            fout << PUnits[i]->getID() << " A " << EnemyFinderByXY(EUnits, path[0].first, path[0].second)->getID() << '\n';
                        }
                        else
                        {
                            fout << PUnits[i]->getID() << " M " << path[0].first << " " << path[0].second << '\n';
                        }
                    }
                }
            }
            else
            {
                std::vector<std::pair<int, int>> path = PathFinder(enemyUnits, PUnits[i]->getX(), PUnits[i]->getY(), EBaseX, EBaseY);
                if (path.size() == 0&&PUnits[i]->getX()!=EBaseX&&PUnits[i]->getY()!=EBaseY)
                {
                    std::cout << "Nie mozna znalezc sciezki dla wojownika\n";
                    std::cout << PUnits[i]->getID() << " " << PUnits[i]->getX() << " " << PUnits[i]->getY() << " " << EBaseX << " " << EBaseY << std::endl;
                }
                else
                {
                    if (PUnits[i]->getZ() >= abs(PUnits[i]->getX() - EBaseX) + abs(PUnits[i]->getY() - EBaseY))
                    {
                        fout << PUnits[i]->getID() << " A " << EnemyBaseUnit->getID() << '\n';
                    }
                    else
                    {
                        if (enemyUnits[path[0].first][path[0].second].type == 3)
                        {
                            fout << PUnits[i]->getID() << " A " << EnemyFinderByXY(EUnits, path[0].first, path[0].second)->getID() << '\n';
                        }
                        else
                        {
                            fout << PUnits[i]->getID() << " M " << path[0].first << " " << path[0].second << '\n';
                        }
                    }
                }
            }
        }
    }
    fout.close();
    /////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    // Plik wspomagajacy
    /////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    std::vector<int> BuildTable = {5, 3, 3, 3, 4, 6, 2};
    enum class Type
    {
        K,
        S,
        A,
        P,
        C,
        R,
        W
    };
    std::map<Type, char> TypeTable = {
        {Type::K, 'K'},
        {Type::S, 'S'},
        {Type::A, 'A'},
        {Type::P, 'P'},
        {Type::C, 'C'},
        {Type::R, 'R'},
        {Type::W, 'W'}};

    int turn = 0;
    std::fstream finout;
    finout.open("WarGame.txt", std::ios::in | std::ios::out);
    if (finout)
    {
        if (!finout.is_open())
        {
            std::cerr << "Blad przy otwarciu pliku: WarGame.txt"
                      << "\n";
            finout.clear();
            exit(EXIT_FAILURE);
        }
        else
        {
            finout.close();
            finout.open("WarGame.txt", std::ios::in);
            std::string line;
            int BuildTurn;
            finout >> line >> line >> turn >> line >> line >> BuildTurn;
            finout << "Plik_wspomagajacy_do_gry_WarGame\n";
            finout << "Numer_Tury: " << turn + 1 << "\n";
            finout.clear();
            finout.close();
            finout.open("WarGame.txt", std::ios::in | std::ios::out | std::ios::trunc);
            finout << "Plik_wspomagajacy_do_gry_WarGame\n";
            finout << "Numer_Tury: " << turn + 1 << "\n";
            if (BuildTurn > 0)
            {
                BuildTurn--;
                finout << "Status_Budowy_w_bazie_gracza: " << PlayerBaseUnit->getB() << " " << BuildTurn << "\n";
            }
            else if (BuildTurn == 0)
            {
                NewUnit(PlayerBaseUnit->getB(), Units, PlayerBaseUnit);
                finout << "Status_Budowy_w_bazie_gracza: "
                       << "wybudowano"
                       << " " << BuildTable[static_cast<int>(TypeTable[static_cast<Type>(PlayerBaseUnit->getB() - 0)])] << "\n";
            }
            else
            {
                finout << "Status_Budowy_w_bazie_gracza: "
                       << "brak"
                       << " " << BuildTable[static_cast<int>(TypeTable[static_cast<Type>(PlayerBaseUnit->getB() - 0)])] << "\n";
            }
            finout.clear();
            finout.close();
        }
    }
    else
    {
        std::cout << "Tworzenie Wargame.txt\n";
        finout.open("WarGame.txt", std::ios::in | std::ios::out | std::ios::trunc);
        finout << "Plik_wspomagajacy_do_gry_WarGame\n";
        finout << "Numer_Tury: " << 1 << 0 << "\n";
        finout << "Status_Budowy_w_bazie_gracza: " << PlayerBaseUnit->getB() << " " << BuildTable[static_cast<int>(TypeTable[static_cast<Type>(PlayerBaseUnit->getB() - 0)])] << "\n";
    }
    finout.close();
    /////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    // Sprawdzanie warunkow wygranej/przegranej
    /////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    if (EnemyBaseUnit->getW() <= 0)
    {
        std::cout << "Wygral gracz 1\n";
        return 0;
    }
    if (PlayerBaseUnit->getW() <= 0)
    {
        std::cout << "Wygral gracz 2\n";
        return 0;
    }
    if (turn >= 1000)
    {
        std::cout << "Remis z powodu limitu tur\n";
        return 0;
    }
    /////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    // Wprowadzanie danych do statusu
    /////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    fout.open("status.txt", std::ios::trunc);
    if (!fout.is_open())
    {
        std::cerr << "Blad przy otwarciu pliku: status.txt"
                  << "\n";
        fout.clear();
        exit(EXIT_FAILURE);
    }
    fout << gold << '\n';
    for (int i = 0; i < Units.size(); i++)
    {
        if (Units[i]->getT() == 'B' - 0)
            fout << Units[i]->getS() << " " << Units[i]->getT() << " " << Units[i]->getID() << " " << Units[i]->getY() << " " << Units[i]->getX() << " " << Units[i]->getW() << " " << Units[i]->getB() << '\n';
        else
            fout << Units[i]->getS() << " " << Units[i]->getT() << " " << Units[i]->getID() << " " << Units[i]->getY() << " " << Units[i]->getX() << " " << Units[i]->getW() << '\n';
    }
    fout.close();
    /////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    // Sprzatanie
    /////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    for (int i = 0; i < enemyUnits.size(); i++)
    {
        for (int j = 0; j < enemyUnits[i].size(); j++)
        {
            std::cout << enemyUnits[i][j].type;
        }
        std::cout << std::endl;
    }
    for (int i = 0; i < Units.size(); i++)
    {
        delete Units[i];
    }
    return 0;
    for (int i = 0; i < PUnits.size(); i++)
    {
        delete PUnits[i];
    }
    for (int i = 0; i < EUnits.size(); i++)
    {
        delete EUnits[i];
    }
    return 0;
}
// Definicja funkcji UnitFinder do wyszukiwania jednostki po ID
Unit *EnemyFinderByXY(std::vector<Unit *> &Units, int x, int y)
{
    for (int i = 0; i < Units.size(); i++)
    {
        if (Units[i]->getX() == x && Units[i]->getY() == y)
            return Units[i];
    }
    return nullptr;
}
// Definicja funkcji UnitFinder do wyszukiwania jednostki po ID
bool checkIfEnemyIsNearby(std::vector<Unit *> &Units, Unit *UnitA,Unit* EnemyUnit)
{
    for (int i = 0; i < Units.size(); i++)
    {
        if (Units[i]->getS() != UnitA->getS())
        {
            if (Units[i]->getZ() >= abs(Units[i]->getX() - UnitA->getX()) + abs(Units[i]->getY() - UnitA->getY()))
            {
                EnemyUnit = Units[i];
                return true;
            }
        }
    }
    return false;
}
// Definicja funkcji UnitFinder do wyszukiwania jednostki po ID
void NewUnit(char t, std::vector<Unit *> &Units, Unit *PlayerBaseUnit)
{
    PlayerBaseUnit->setB('0');
    int IDTable[1001];
    for (int i = 0; i < 1001; i++)
    {
        IDTable[i] = i;
    }
    for (int i = 0; i < Units.size(); i++)
    {
        IDTable[Units[i]->getID()] = -1;
    }
    int id;
    for (int i = 0; i < 1001; i++)
    {
        if (IDTable[i] != -1)
        {
            id = IDTable[i];
            break;
        }
    }
    int x = PlayerBaseUnit->getX();
    int y = PlayerBaseUnit->getY();
    switch (t)
    {
    case 'K':
        Units.push_back(new Unit('P', t, id, x, y, 70, '5', 5, 400, 1));
        break;
    case 'S':
        Units.push_back(new Unit('P', t, id, x, y, 60, '3', 2, 250, 1));
        break;
    case 'A':
        Units.push_back(new Unit('P', t, id, x, y, 40, '3', 2, 250, 5));
        break;
    case 'P':
        Units.push_back(new Unit('P', t, id, x, y, 50, '3', 2, 200, 2));
        break;
    case 'R':
        Units.push_back(new Unit('P', t, id, x, y, 90, '4', 2, 500, 1));
        break;
    case 'C':
        Units.push_back(new Unit('P', t, id, x, y, 50, '6', 2, 800, 7));
        break;
    case 'W':
        Units.push_back(new Unit('P', t, id, x, y, 20, '2', 2, 100, 1));
        break;
    default:
        std::cerr << "Blad przy wyborze jednostki\n";
        break;
    }
}
// Definicja funkcji MapMaker do tworzenia mapy na podstawie pliku
std::vector<std::vector<Point>> MapMaker(const std::string &filename)
{
    std::vector<std::vector<Point>> map;
    std::ifstream file(filename, std::ios::in);
    if (file.is_open())
    {
        std::string line;
        int i = 0;

        while (getline(file, line))
        {
            std::vector<Point> row;

            for (char c : line)
            {
                int j = 0;
                row.push_back({i, j, c - '0'});
                j++;
            }

            map.push_back(row);
            i++;
        }
    }
    else
    {
        std::cerr << "Blad przy otwarciu pliku: " << filename << std::endl;
    }

    return map;
}

// Definicja funkcji StatusReader do odczytu stanu jednostek na podstawie pliku
void StatusReader(const std::string &filename, long &gold, std::vector<Unit *> &PUnits, std::vector<Unit *> &EUnits, std::vector<Unit *> &Units)
{
    std::ifstream fin(filename, std::ios::in);

    if (!fin.is_open())
    {
        std::cerr << "Blad przy otwarciu pliku: status.txt" << '\n';
        fin.clear();
        exit(EXIT_FAILURE);
    }
    fin >> gold;
    char side, type, build;
    int x, y, hp, id;
    while (fin >> side >> type >> id >> y >> x >> hp)
    {
        switch (type)
        {
        case 'K':
            Units.push_back(new Unit(side, type, id, x, y, hp, '5' - 0, 5, 400, 1));
            break;
        case 'S':
            Units.push_back(new Unit(side, type, id, x, y, hp, '3' - 0, 2, 250, 1));
            break;
        case 'A':
            Units.push_back(new Unit(side, type, id, x, y, hp, '3' - 0, 2, 250, 5));
            break;
        case 'P':
            Units.push_back(new Unit(side, type, id, x, y, hp, '3' - 0, 2, 200, 2));
            break;
        case 'R':
            Units.push_back(new Unit(side, type, id, x, y, hp, '4' - 0, 2, 500, 1));
            break;
        case 'C':
            Units.push_back(new Unit(side, type, id, x, y, hp, '6' - 0, 2, 800, 7));
            break;
        case 'W':
            Units.push_back(new Unit(side, type, id, x, y, hp, '2' - 0, 2, 100, 1));
            break;
        case 'B':
            fin >> build;
            Units.push_back(new Unit(side, type, id, x, y, hp, build, 0, INT16_MAX, 0));
            break;
        default:
            std::cerr << "Blad przy wyborze jednostki\n";
            break;
        }
    }
    for (int i = 0; i < Units.size(); i++)
    {
        if (Units[i]->getS() == 'P')
            PUnits.push_back(Units[i]);
        else
            EUnits.push_back(Units[i]);
    }
    fin.clear();
}
// Definicja funkcji OrdersProcessor do odczytu rozkazow na podstawie pliku
void OrdersProcessor(const std::string &filename, std::vector<Unit *> &PUnits, std::vector<Unit *> &EUnits, std::vector<Unit *> &Units)
{
    std::ifstream fin;
    fin.open(filename, std::ios::in);
    if (!fin.is_open())
    {
        std::cerr << "Blad przy otwarciu pliku: " << filename << '\n';
        fin.clear();
        exit(EXIT_FAILURE);
    }
    char action, build;
    int x, y, UnitA, UnitB;
    while (fin >> UnitA >> action)
    {
        switch (action)
        {
        case 'M':
            fin >> x >> y;
            if (UnitFinder(Units, UnitA)->getT() == 'B')
                continue;
            UnitFinder(Units, UnitA)->Move(*UnitFinder(Units, UnitA), x, y);
            break;
        case 'A':
            fin >> UnitB;
            UnitFinder(Units, UnitA)->Attack(*UnitFinder(Units, UnitA), *UnitFinder(Units, UnitB));
            break;
        case 'B':
            fin >> build;
            if (UnitFinder(Units, UnitA)->getT() != 'B')
                continue;
            UnitFinder(Units, UnitA)->Build(*UnitFinder(Units, UnitA), build);
            break;
            break;
        }
    }
}
// Definicja funkcji UnitFinder do wyszukiwania jednostki po ID
Unit *UnitFinder(std::vector<Unit *> &Units, int id)
{
    for (int i = 0; i < Units.size(); i++)
    {
        if (Units[i]->getID() == id)
            return Units[i];
    }
    return nullptr;
}
// Definicja funkcji checkIfPointIsValid do sprawdzania czy punkt jest na mapie
bool checkIfPointIsValid(std::vector<std::vector<Point>> &Map, int x, int y)
{
    if (x < 0 || y < 0 || x >= Map.size() || y >= Map[0].size())
        return false;
    if (Map[x][y].type == 9)
        return false;
    return true;
}
// Definicja funkcji PathFinder do znajdowania najkrotszej sciezki
std::vector<std::pair<int, int>> PathFinder(std::vector<std::vector<Point>> &Map, int start_x, int start_y, int end_x, int end_y)
{
    int rows = Map.size();
    int cols = Map[0].size();
    std::vector<std::vector<bool>> visited(rows, std::vector<bool>(cols, false));
    std::vector<std::vector<std::pair<int, int>>> parent(rows, std::vector<std::pair<int, int>>(cols));
    std::queue<std::pair<int, int>> q;
    q.push(std::make_pair(start_x, start_y));
    std::vector<std::pair<int, int>> neighbors = {{0, 1}, {0, -1}, {1, 0}, {-1, 0}};

    while (!q.empty())
    {
        int x = q.front().first;
        int y = q.front().second;
        q.pop();

        if (x == end_x && y == end_y)
            break;

        for (auto &neighbor : neighbors)
        {
            int new_x = x + neighbor.first;
            int new_y = y + neighbor.second;

            if (checkIfPointIsValid(Map, new_x, new_y) && !visited[new_x][new_y])
            {
                visited[new_x][new_y] = true;
                parent[new_x][new_y] = std::make_pair(x, y);
                q.push(std::make_pair(new_x, new_y));
            }
        }
    }

    if (!visited[end_x][end_y])
    {
        return {};
    }

    std::vector<std::pair<int, int>> path;
    int x = end_x;
    int y = end_y;

    while (x != start_x || y != start_y)
    {
        path.push_back(std::make_pair(x, y));
        std::pair<int, int> next = parent[x][y];
        x = next.first;
        y = next.second;
    }

    std::reverse(path.begin(), path.end());
    return path;
}
// Definicja funkcji enemyUnitsMap do tworzenia mapy jednostek przeciwnika
std::vector<std::vector<Point>> enemyUnitsMap(const std::vector<std::vector<Point>> &Map, std::vector<Unit *> &EUnits)
{
    std::vector<std::vector<Point>> enemyUnitsMap = Map;
    for (int i = 0; i < EUnits.size(); i++)
    {
        if (EUnits[i]->getT() != 'B')
            enemyUnitsMap[EUnits[i]->getX()][EUnits[i]->getY()].type = 3;
    }
    return enemyUnitsMap;
}
// Definicja funkcji baseFinder do znajdowania baz gracza i przeciwnika
void baseFinder(const std::vector<std::vector<Point>> &Map, int &PBaseX, int &PBaseY, int &EBaseX, int &EBaseY)
{
    for (int i = 0; i < Map.size(); i++)
    {
        for (int j = 0; j < Map[0].size(); j++)
        {
            if (Map[i][j].type == 1)
            {
                PBaseX = i;
                PBaseY = j;
            }
            if (Map[i][j].type == 2)
            {
                EBaseX = i;
                EBaseY = j;
            }
        }
    }
}
// Definicja funkcji mineFinder do znajdowania kopalni
void mineFinder(const std::vector<std::vector<Point>> &Map, int &MineX, int &MineY)
{
    for (int i = 0; i < Map.size(); i++)
    {
        for (int j = 0; j < Map[0].size(); j++)
        {
            if (Map[i][j].type == 6)
            {
                MineX = i;
                MineY = j;
            }
        }
    }
}
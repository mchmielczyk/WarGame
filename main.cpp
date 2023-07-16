#include "player.h" // Dołączenie pliku nagłówkowego "player.h", który zawiera definicje jednostek

std::vector<std::vector<Point>> MapMaker(const std::string &filename); // Deklaracja funkcji MapMaker do tworzenia mapy

void StatusReader(const std::string &filename, long &gold, std::vector<Unit *> &PUnits, std::vector<Unit *> &EUnits, std::vector<Unit *> &Units); // Deklaracja funkcji StatusReader do odczytu stanu jednostek

void OrdersProcessor(const std::string &filename, std::vector<Unit *> &PUnits, std::vector<Unit *> &EUnits, std::vector<Unit *> &Units);

Unit *UnitFinder(std::vector<Unit *> &Units, int id);

bool checkIfPointIsValid(std::vector<std::vector<Point>> &Map, int x, int y);

std::vector<std::pair<int, int>> PathFinder(std::vector<std::vector<Point>> &Map, int start_x, int start_y, int end_x, int end_y);

std::vector<std::vector<Point>> enemyUnitsMap(const std::vector<std::vector<Point>> &Map, std::vector<Unit *> &EUnits);

void baseFinder(const std::vector<std::vector<Point>> &Map, int &PBaseX, int &PBaseY, int &EBaseX, int &EBaseY);

void mineFinder(const std::vector<std::vector<Point>> &Map, int &MineX, int &MineY);

int main(int argc, char *argv[])
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
    /////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    // Logika
    /////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    int PBaseX, PBaseY, EBaseX, EBaseY;
    PBaseX = PBaseY = EBaseX = EBaseY = -1;
    baseFinder(Map, PBaseX, PBaseY, EBaseX, EBaseY);
    int MineX, MineY;
    MineX = MineY = -1;
    mineFinder(Map, MineX, MineY);
    std::vector<std::vector<Point>> enemyUnits = enemyUnitsMap(Map, EUnits);
    std::vector<std::pair<int, int>> enemybasepath = PathFinder(enemyUnits, PBaseX, PBaseY, EBaseX, EBaseY);
    std::vector<std::pair<int, int>> minepath = PathFinder(enemyUnits, PBaseX, PBaseY, MineX, MineY);
    /////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    // Przygotowanie rozkazow
    /////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    std::ofstream fout;
    fout.open("rozkazy.txt", std::ios::trunc);
    if (!fout.is_open())
    {
        std::cerr << "Blad przy otwarciu pliku: rozkazy.txt"
                  << "\n";
        fout.clear();
        exit(EXIT_FAILURE);
    }
    for (int i = 0; i < PUnits.size(); i++)
    {
        std::cout<<PUnits[i]->getID()<<'\n';
        if (PUnits[i]->getS() == 'B')
        {
            if (PUnits[i]->getB() == '0')
            {
                if (gold >= 1000)
                {
                    fout << PUnits[i]->getID() << " B K"<< '\n';
                    gold -= 400;
                }
                else
                {
                    fout << PUnits[i]->getID() << " B W" << '\n';
                    gold -= 100;
                }
            }
            else
            {
                fout << PUnits[i]->getID() << " B " << PUnits[i]->getB() << '\n';
            }
        }
        else
        {
            if (PUnits[i]->getT() == 'W')
            {
                if (PUnits[i]->getP() >= abs(PUnits[i]->getX() - MineX) + abs(PUnits[i]->getY() - MineY))
                {
                    fout << PUnits[i]->getID() << " M " << MineX << " " << MineY << '\n';
                }
                else
                {
                    fout << PUnits[i]->getID() << " M " << minepath[0].first << " " << minepath[0].second << '\n';
                }
            }
            else 
            {
                if (PUnits[i]->getP() >= abs(PUnits[i]->getX() - EBaseX) + abs(PUnits[i]->getY() - EBaseY))
                {
                    fout << PUnits[i]->getID() << " M " << EBaseX << " " << EBaseY << '\n';
                }
                else
                {
                    fout << PUnits[i]->getID() << " M " << enemybasepath[0].first << " " << enemybasepath[0].second << '\n';
                }
            }
        }
    }
    fout.close();
    return 0;
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
            if (UnitFinder(Units, UnitA)->getS() == 'B')
                continue;
            UnitFinder(Units, UnitA)->Move(*UnitFinder(Units, UnitA), x, y);
            break;
        case 'A':
            fin >> UnitB;
            if (UnitFinder(Units, UnitA)->getS() == 'B' || UnitFinder(Units, UnitB)->getS() == 'B')
                continue;
            UnitFinder(Units, UnitA)->Attack(*UnitFinder(Units, UnitA), *UnitFinder(Units, UnitB));
            break;
        case 'B':
            fin >> build;
            if (UnitFinder(Units, UnitA)->getS() != 'B')
                continue;
            UnitFinder(Units, UnitA)->Build(*UnitFinder(Units, UnitA), build);
            break;
            break;
        }
    }
}
Unit *UnitFinder(std::vector<Unit *> &Units, int id)
{
    for (int i = 0; i < Units.size(); i++)
    {
        if (Units[i]->getID() == id)
            return Units[i];
    }
    return nullptr;
}
bool checkIfPointIsValid(std::vector<std::vector<Point>> &Map, int x, int y)
{
    if (x < 0 || y < 0 || x >= Map.size() || y >= Map[0].size())
        return false;
    if (Map[x][y].type == 9)
        return false;
    return true;
}
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
        std::cout << "Nie mozna znalezc sciezki\n";
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
std::vector<std::vector<Point>> enemyUnitsMap(const std::vector<std::vector<Point>> &Map, std::vector<Unit *> &EUnits)
{
    std::vector<std::vector<Point>> enemyUnitsMap = Map;
    for (int i = 0; i < EUnits.size(); i++)
    {
        enemyUnitsMap[EUnits[i]->getX()][EUnits[i]->getY()].type = 3;
    }
    return enemyUnitsMap;
}
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
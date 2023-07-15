#include "player.h" // Dołączenie pliku nagłówkowego "player.h", który zawiera definicje jednostek

std::vector<std::vector<Point>> MapMaker(const std::string &filename); // Deklaracja funkcji MapMaker do tworzenia mapy

int TypePicker(char c); // Deklaracja funkcji TypePicker do wyboru typu jednostki na podstawie znaku

void StatusReader(const std::string &filename, long &gold, std::vector<Unit *> &PUnits, std::vector<Unit *> &EUnits); // Deklaracja funkcji StatusReader do odczytu stanu jednostek

int main(int argc, char *argv[])
{
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
    /////////////////////////////////////////
    int limitCzasowy = 5;
    if (argc > 4)
        limitCzasowy = std::atoi(argv[4]);
    /////////////////////////////////////////
    std::vector<std::vector<Point>> Map = MapMaker(argv[1]); // Wywołanie funkcji MapMaker do utworzenia mapy
    std::vector<Unit *> PUnits;                              // Wektor wskaźników na jednostki gracza
    std::vector<Unit *> EUnits;                              // Wektor wskaźników na jednostki przeciwnika
    long gold;
    StatusReader(argv[2], gold, PUnits, EUnits); // Wektor rozkazów jednostek gracza
    /////////////////////////////////////////
    // int MapSizeX = Map.size();
    // int MapSizeY = Map[0].size();
    /////////////////////////////////////////
    // Wyświetlanie mapy
    /////////////////////////////////////////
    // for (const auto &row : Map)
    //  {
    //     for (const auto &point : row)
    //     {
    //         std::cout << point.type << " ";
    //     }
    //     std::cout << std::endl;
    // }
    /////////////////////////////////////////
    // Rozkazy.txt
    /////////////////////////////////////////
    for (int i = 0; i < PUnits.size(); i++)
    {
        std::cout << PUnits[i]->getS() << " " << PUnits[i]->getT() << " " << PUnits[i]->getID() << " " << PUnits[i]->getX() << " " << PUnits[i]->getY() << " " << PUnits[i]->getW() << " " << PUnits[i]->getB() << " " << PUnits[i]->getP() << " " << PUnits[i]->getK() << " " << PUnits[i]->getZ() << std::endl;
    }
    /////////////////////////////////////////
    return 0;
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

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
void StatusReader(const std::string &filename, long &gold, std::vector<Unit *> &PUnits, std::vector<Unit *> &EUnits)
{
    std::ifstream fin(filename, std::ios::in);

    if (!fin.is_open())
    {
        std::cerr << "Blad przy otwarciu pliku: status.txt" << '\n';
        fin.clear();
        exit(EXIT_FAILURE);
    }
    char side, type, build;
    int x, y, hp, id;
    fin >> gold;
    while (fin >> side >> type >> id >> x >> y >> hp)
    {
        switch (side)
        {
        case 'P':
            switch (type)
            {
            case 'K':
                PUnits.push_back(new Unit(side, type, id, x, y, hp, '5' - 0, 5, 400, 1));
                break;
            case 'S':
                PUnits.push_back(new Unit(side, type, id, x, y, hp, '3' - 0, 2, 250, 1));
                break;
            case 'A':
                PUnits.push_back(new Unit(side, type, id, x, y, hp, '3' - 0, 2, 250, 5));
                break;
            case 'P':
                PUnits.push_back(new Unit(side, type, id, x, y, hp, '3' - 0, 2, 200, 2));
                break;
            case 'R':
                PUnits.push_back(new Unit(side, type, id, x, y, hp, '4' - 0, 2, 500, 1));
                break;
            case 'C':
                PUnits.push_back(new Unit(side, type, id, x, y, hp, '6' - 0, 2, 800, 7));
                break;
            case 'W':
                PUnits.push_back(new Unit(side, type, id, x, y, hp, '2' - 0, 2, 100, 1));
                break;
            case 'B':
                fin >> build;
                PUnits.push_back(new Unit(side, type, id, x, y, hp, build, 0, INT16_MAX, 0));
                break;
            default:
                std::cerr << "Blad przy wyborze jednostki: " << type << '\n';
                break;
            }
        case 'E':
            switch (type)
            {
            case 'K':
                EUnits.push_back(new Unit(side, type, id, x, y, hp, '5' - 0, 5, 400, 1));
                break;
            case 'S':
                EUnits.push_back(new Unit(side, type, id, x, y, hp, '3' - 0, 2, 250, 1));
                break;
            case 'A':
                EUnits.push_back(new Unit(side, type, id, x, y, hp, '3' - 0, 2, 250, 5));
                break;
            case 'P':
                EUnits.push_back(new Unit(side, type, id, x, y, hp, '3' - 0, 2, 200, 2));
                break;
            case 'R':
                EUnits.push_back(new Unit(side, type, id, x, y, hp, '4' - 0, 2, 500, 1));
                break;
            case 'C':
                EUnits.push_back(new Unit(side, type, id, x, y, hp, '6' - 0, 2, 800, 7));
                break;
            case 'W':
                EUnits.push_back(new Unit(side, type, id, x, y, hp, '2' - 0, 2, 100, 1));
                break;
            case 'B':
                fin >> build;
                EUnits.push_back(new Unit(side, type, id, x, y, hp, build, 0, INT16_MAX, 0));
                break;
            default:
                std::cerr << "Blad przy wyborze jednostki: " << type << '\n';
                break;
            }
        default:
            std::cerr << "Blad przy odczycie pliku: status.txt: " << side << '\n';
            break;
        }
    }

    fin.clear();
}
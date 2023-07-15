#include "player.h" // Dołączenie pliku nagłówkowego "player.h", który zawiera definicje jednostek

std::vector<std::vector<Point>> MapMaker(const std::string &filename); // Deklaracja funkcji MapMaker do tworzenia mapy

void StatusReader(const std::string &filename, long &gold, std::vector<Unit *> &PUnits, std::vector<Unit *> &EUnits, std::vector<Unit *> &Units); // Deklaracja funkcji StatusReader do odczytu stanu jednostek

void OrdersProcessor(const std::string &filename, std::vector<Unit *> &PUnits, std::vector<Unit *> &EUnits, std::vector<Unit *> &Units);

Unit *UnitFinder(std::vector<Unit *> &Units, int id);

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
    std::vector<Unit *> EUnits;                              // Wektor wskaźników na jednostki przeciwnika=
    long gold;
    std::vector<Unit *> Units;
    StatusReader(argv[2], gold, PUnits, EUnits, Units); // Wektor rozkazów jednostek gracza
    OrdersProcessor(argv[3], PUnits, EUnits, Units);
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
    // status.txt
    /////////////////////////////////////////
    // for (int i = 0; i < PUnits.size(); i++)
    //{
    //     std::cout <<"Udane jednostki z"<<PUnits.size()<<": "<< PUnits[i]->getS() << " " << PUnits[i]->getT() << " " << PUnits[i]->getID() << " " << PUnits[i]->getX() << " " << PUnits[i]->getY() << " " << PUnits[i]->getW() << " " << PUnits[i]->getB() << " " << PUnits[i]->getP() << " " << PUnits[i]->getK() << " " << PUnits[i]->getZ() << std::endl;
    // }
    // for (int i = 0; i < EUnits.size(); i++)
    //{
    //     std::cout <<"Udane jednostki z"<<EUnits.size()<<": "<< EUnits[i]->getS() << " " << EUnits[i]->getT() << " " << EUnits[i]->getID() << " " << EUnits[i]->getX() << " " << EUnits[i]->getY() << " " << EUnits[i]->getW() << " " << EUnits[i]->getB() << " " << EUnits[i]->getP() << " " << EUnits[i]->getK() << " " << EUnits[i]->getZ() << std::endl;
    // }
    // for(int i=0;i<Units.size();i++)
    //{
    //     std::cout<<Units[i]->getS()<<" "<<Units[i]->getT()<<" "<<Units[i]->getID()<<" "<<Units[i]->getX()<<" "<<Units[i]->getY()<<" "<<Units[i]->getW()<<" "<<Units[i]->getB()<<" "<<Units[i]->getP()<<" "<<Units[i]->getK()<<" "<<Units[i]->getZ()<<std::endl;
    // }
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
    while (fin >> side >> type >> id >> x >> y >> hp)
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
            if(UnitFinder(Units, UnitA)->getS()=='B')
            continue;
            UnitFinder(Units, UnitA)->Move(*UnitFinder(Units, UnitA), x, y);
            break;
        case 'A':
            fin >> UnitB;
            if(UnitFinder(Units, UnitA)->getS()=='B'||UnitFinder(Units, UnitB)->getS()=='B')
            continue;
            UnitFinder(Units, UnitA)->Attack(*UnitFinder(Units, UnitA), *UnitFinder(Units, UnitB));
            break;
        case 'B':
            fin >> build;
            if(UnitFinder(Units, UnitA)->getS()!='B')
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
#include "Game.h"


// Проверка возможности размещения корабля
bool Game::canPlaceShip(int player, int x, int y, int size, bool horizontal) {
    if (horizontal) {
        if (x + size > 10) return false; // Проверка выхода за границы карты
        for (int i = x; i < x + size; i++) {
            if (map[player][y][i] != 0) return false; // Проверка на пересечение с другими кораблями
        }
    }
    else {
        if (y + size > 10) return false; // Проверка выхода за границы карты
        for (int i = y; i < y + size; i++) {
            if (map[player][i][x] != 0) return false; // Проверка на пересечение с другими кораблями
        }
    }
    return true;
}
void Game::initializeMap()
{
    for (int player = 0; player < 2; player++)
    {
        for (int y = 0; y < 10; y++)
        {
            for (int x = 0; x < 10; x++)
            {
                map[player][y][x] = 0;
            }
        }
    }
}
// Размещение корабля на карте
void Game::placeShip(int player, int x, int y, int size, bool horizontal) {
    if (horizontal) {
        for (int i = x; i < x + size; i++) {
            map[player][y][i] = 1; // Размещение кораблей горизонтально
        }
    }
    else {
        for (int i = y; i < y + size; i++) {
            map[player][i][x] = 1; // Размещение кораблей вертикально
        }
    }
}

void Game::generateMap() {
    for(int j = 0;j < 2;j++)
    {
        for (int i = 0; i < SHIP_COUNT; i++) {
            int size = SHIP_SIZES[i];
            bool placed = false;
            while (!placed) {
                int x = rand() % 10; // Случайная координата X
                int y = rand() % 10; // Случайная координата Y
                bool horizontal = rand() % 2 == 0; // Случайное направление
                if (canPlaceShip(j, x, y, size, horizontal)) {
                    placeShip(j, x, y, size, horizontal);
                    placed = true;
                }
            }
        }
    }
}
Game::Game()
{
    initializeMap();
    generateMap();
    printMap();
}
void Game::printMap() const {
    for(int i = 0;i < 2;i++)
    {
        cout << "Карта игрока " << i + 1 << ":\n";
        for (int y = 0; y < 10; y++) {
            for (int x = 0; x < 10; x++) {
                switch (static_cast<Ships>(map[i][y][x]))
                {
                    case Ships::empty:
                        cout << "-" << " ";
                        break;
                    case Ships::ship:
                        cout << "H" << " ";
                        break;
                    case Ships::dead:
                        cout << "X" << " ";
                        break;
                    case Ships::miss:
                        cout << "O" << " ";
                        break;
                    default:
                        break;
                }
                
            }
            cout << endl;
        }
        cout << endl;
    }
}

json Game::getMap(int player,bool isEnemy)
{
    json j;
    j["map"] = json::array();
    if(!isEnemy)
    {
        for (int y = 0; y < 10; y++) {
            json row = json::array();
            for (int x = 0; x < 10; x++) {
                row.push_back(map[player][y][x]);
            }
            j["map"].push_back(row);
        }
    }
    else
    {
        for (int y = 0; y < 10; y++) {
            json row = json::array();
            for (int x = 0; x < 10; x++) {
                row.push_back(map[player][y][x] == 1 ? 0 : map[player][y][x]);
            }
            j["map"].push_back(row);
        }
    }
    return j;
}

bool Game::hit(int fromPlayer, int x, int y)
{
    fromPlayer = fromPlayer == 1 ? 0 : 1;
    if (map[fromPlayer][y][x] == 1) { map[fromPlayer][y][x] = 2; return true; }
    else if (map[fromPlayer][y][x] != 2) map[fromPlayer][y][x] = 3;
    return false;
}

int Game::winner()
{
    
    for (int player = 0; player < 2; player++)
    {
        int ships = 0;
        for (int y = 0; y < 10; y++)
        {
            for (int x = 0; x < 10; x++)
            {
                if (map[player][y][x] == 1) ships++;
            }
        }
        if (ships == 0) return player == 0 ? 1 : 0;
    }
    return -1;
}
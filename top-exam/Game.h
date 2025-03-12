#pragma once
#include <iostream>
#include <nlohmann/json.hpp>

using json = nlohmann::json;
using namespace std;


class Game
{
private:
    int map[2][10][10]; // Карта для двух игроков
    const int SHIP_SIZES[10] = { 4, 3, 3, 2, 2, 2, 1, 1, 1, 1 }; // Размеры кораблей
    const int SHIP_COUNT = 10; // Количество кораблей

    void initializeMap();
    bool canPlaceShip(int player, int x, int y, int size, bool horizontal);
    void placeShip(int player, int x, int y, int size, bool horizontal);

public:
    enum Ships
    {
        empty = 0,
        ship = 1,
        dead = 2,
        miss = 3
    };
    Game();
    void generateMap();
    void printMap() const;
    json getMap(int player,bool isEnemy);
    bool hit(int fromPlayer, int x, int y);
    int winner();
    int remainingShips(int player);
};
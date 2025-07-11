#ifndef HOURGLASS_GRID_H
#define HOURGLASS_GRID_H

#include <SFML/Graphics.hpp>

struct Grain {
    sf::Color color;
    Grain(sf::Color c) : color(c) {}
    bool moved = false;
    float velocity = 1;
    int immobile_frames = 0;
};

class Hourglass_Grid {
public:
    int width;
    int height;
    std::vector<std::vector<bool>> isWall;
    std::vector<std::vector<Grain*>> cells;
    Hourglass_Grid(int width, int height)
        : width(width), height(height) {
        cells.resize(height, std::vector<Grain*>(width, nullptr));
    }
    void initialize_walls();
    void set(int x, int y, Grain* grain);
    Grain* get(int x, int y);
    void update();
    void updateCell(int x, int y);
    void reset_grains();
    void flip();
    bool is_inside_hourglass(int x, int y);
    ~Hourglass_Grid();
};

#endif //HOURGLASS_GRID_H

#include <SFML/Graphics.hpp>
#include "Hourglass_Grid.h"
#include "Utility.h"

#include <iostream>

const int CELL_SIZE = 2;
const int GRID_WIDTH = 400;
const int GRID_HEIGHT = 400;
const int GRAIN_MULTIPLIER = 20;
const int GRAIN_SPREAD = 10;

sf::Color HSVtoRGB(float hue, float sat, float val) {
    int h = static_cast<int>(hue / 60.0f) % 6;
    float f = (hue / 60.0f) - h;
    float p = val * (1 - sat);
    float q = val * (1 - f * sat);
    float t = val * (1 - (1 - f) * sat);

    float r = 0, g = 0, b = 0;

    switch (h) {
        case 0: r = val, g = t, b = p; break;
        case 1: r = q, g = val, b = p; break;
        case 2: r = p, g = val, b = t; break;
        case 3: r = p, g = q, b = val; break;
        case 4: r = t, g = p, b = val; break;
        case 5: r = val, g = p, b = q; break;
    }

    return sf::Color(
        static_cast<sf::Uint8>(r * 255),
        static_cast<sf::Uint8>(g * 255),
        static_cast<sf::Uint8>(b * 255)
    );
}

int main() {
    Hourglass_Grid grid(GRID_WIDTH, GRID_HEIGHT);
    grid.initialize_walls();
    int grains = 0;

    sf::Font font;
    font.loadFromFile("assets/font.ttf");

    sf::RenderWindow window(sf::VideoMode(GRID_WIDTH * CELL_SIZE, GRID_HEIGHT * CELL_SIZE), "Sandfall");
    sf::RectangleShape cellShape(sf::Vector2f(CELL_SIZE, CELL_SIZE));

    sf::Clock clock;
    const sf::Time timePerUpdate = sf::seconds(1.f / 60.f);  // 60 updates per second
    sf::Time timeSinceLastUpdate = sf::Time::Zero;

    while (window.isOpen()) {
        sf::Event event;
        while (window.pollEvent(event)) {
            if (event.type == sf::Event::Closed)
                window.close();
            if (event.type == sf::Event::KeyReleased && event.key.code == sf::Keyboard::Space) {
                grid.flip();
            }
        }

        timeSinceLastUpdate += clock.restart();

        if (timeSinceLastUpdate >= timePerUpdate) {
            grid.update();
            timeSinceLastUpdate -= timePerUpdate;

            if (sf::Mouse::isButtonPressed(sf::Mouse::Left)) {
                sf::Vector2i mousePos = sf::Mouse::getPosition(window);
                if (!grid.is_inside_hourglass(mousePos.x/CELL_SIZE, mousePos.y/CELL_SIZE)) {
                    continue;
                }

                for (int i = 0; i < GRAIN_MULTIPLIER; ++i) {
                    int gridX = mousePos.x / CELL_SIZE;
                    int gridY = mousePos.y / CELL_SIZE;

                    gridX += Utility::random_int(-GRAIN_SPREAD, GRAIN_SPREAD);
                    gridY += Utility::random_int(-GRAIN_SPREAD, GRAIN_SPREAD);

                    if (gridX >= 0 && gridX < GRID_WIDTH
                        && gridY >= 0 && gridY < GRID_HEIGHT
                        && grid.is_inside_hourglass(gridX, gridY)) {
                        if (grid.get(gridX, gridY) == nullptr && !grid.isWall[gridX][gridY]) {
                            float hue = Utility::random_int(45, 60);
                            sf::Color color = Utility::HSVtoRGB(hue, 1.0f, 1.0f);
                            grid.set(gridX, gridY, new Grain(color));
                            grains++;
                        }
                    }
                }
            }
        }

        window.clear();

        for (int y = 0; y < GRID_HEIGHT; ++y) {
            for (int x = 0; x < GRID_WIDTH; ++x) {
                if (grid.isWall[y][x]) {
                    cellShape.setPosition(x * CELL_SIZE, y * CELL_SIZE);
                    cellShape.setFillColor(sf::Color::White);
                    window.draw(cellShape);
                } else {
                    Grain* g = grid.get(x, y);
                    if (g != nullptr) {
                        cellShape.setPosition(x * CELL_SIZE, y * CELL_SIZE);
                        cellShape.setFillColor(g->color);
                        window.draw(cellShape);
                    }
                }
            }
        }
        Utility::display_instructions(window, font);
        Utility::display_grain_count(window, font, grains);
        window.display();
    }

    return 0;
}
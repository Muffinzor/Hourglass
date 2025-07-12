//
// Created by leo on 7/11/25.
//

#include "Hourglass_Grid.h"
#include "Utility.h"
#include <cmath>

void Hourglass_Grid::initialize_walls() {
    isWall.resize(height, std::vector<bool>(width, false));

    float maxMargin = width * 0.35f;
    float minMargin = width * 0.012f;
    int center = width / 2;

    int topMargin = static_cast<int>(height * 0.05f);
    int bottomMargin = static_cast<int>(height * 0.95f);

    int thickness = 2;

    // Walls
    for (int y = topMargin; y <= bottomMargin; ++y) {
        float t = static_cast<float>(y - topMargin) / (bottomMargin - topMargin);

        float curve = std::cos(t * 2 * M_PI);
        float normalized = (curve + 1.0f) / 2.0f;
        float margin = minMargin + normalized * (maxMargin - minMargin);

        int left = static_cast<int>(center - margin);
        int right = static_cast<int>(center + margin);

        for (int offset = -thickness; offset <= thickness; ++offset) {
            int leftPos = left + offset;
            int rightPos = right + offset;
            if (leftPos >= 0 && leftPos < width) {
                isWall[y][leftPos] = true;
            }
            if (rightPos >= 0 && rightPos < width) {
                isWall[y][rightPos] = true;
            }
        }
    }

    // Floor + Ceiling
    for (int x = static_cast<int>(width * 0.15f) - 1; x < static_cast<int>(width * 0.85f); ++x) {
        for (int offset = -thickness; offset <= thickness; ++offset) {
            int floorX = x + offset;
            if (floorX >= 0 && floorX < width) {
                isWall[topMargin][floorX] = true;
                isWall[topMargin+1][floorX] = true;
                isWall[bottomMargin][floorX] = true;
                isWall[bottomMargin-1][floorX] = true;
            }
        }
    }
}

void Hourglass_Grid::reset_grains() {
    for (int y = height - 2; y >= 0; --y) {
        for (int x = 0; x < width; ++x) {
            Grain* current = cells[y][x];
            if (current != nullptr) {
                current->moved = false;
                if (current->immobile_frames >= 3) current->velocity = 1;
            }
        }
    }
}

void Hourglass_Grid::flip() {
    for (int y = 0; y < height / 2; ++y) {
        for (int x = 0; x < width; ++x) {
            std::swap(cells[y][x], cells[height - 1 - y][x]);
        }
    }

    for (int y = 0; y < height; ++y) {
        for (int x = 0; x < width; ++x) {
            Grain* g = cells[y][x];
            if (g != nullptr) {
                g->moved = false;
                g->immobile_frames = 0;
                g->velocity = 1.0f;
            }
        }
    }
}

void Hourglass_Grid::update() {
    reset_grains();

    for (int y = height - 2; y >= 0; --y) {
        bool leftToRight = Utility::random_bool();
        if (leftToRight) {
            for (int x = 0; x < width; ++x) {
                updateCell(x, y);
            }
        } else {
            for (int x = width - 1; x >= 0; --x) {
                updateCell(x, y);
            }
        }
    }
}

void accelerate_grain(Grain* current) {
    current->velocity += 0.1f;
}

void Hourglass_Grid::updateCell(int x, int y) {
    Grain* current = cells[y][x];
    if (current != nullptr && !current->moved) {
        accelerate_grain(current);

        int fallDistance = static_cast<int>(std::floor(current->velocity));
        int currentY = y;
        int currentX = x;

        for (int i = 0; i < fallDistance; ++i) {
            int nextY = currentY + 1;
            if (nextY >= height) {
                current->velocity = 1;
                break;
            }

            bool canMoveDown = (cells[nextY][currentX] == nullptr) && !isWall[nextY][currentX];
            if (canMoveDown) {
                cells[nextY][currentX] = current;
                cells[currentY][currentX] = nullptr;
                currentY = nextY;
                current->moved = true;
            } else {
                Grain* blocking_grain = cells[nextY][currentX];
                if (blocking_grain != nullptr && blocking_grain->velocity < current->velocity) {
                    float avg_velocity = (blocking_grain->velocity + current->velocity) / 2;
                    blocking_grain->velocity = avg_velocity;
                    current->velocity = avg_velocity;
                }

                const int maxSlide = 2;
                bool movedSideways = false;

                for (int slideDist = 1; slideDist <= maxSlide && !movedSideways; ++slideDist) {
                    int tryX = currentX - slideDist;
                    if (tryX >= 0 && !isWall[nextY][tryX] && cells[nextY][tryX] == nullptr) {
                        cells[nextY][tryX] = current;
                        cells[currentY][currentX] = nullptr;
                        currentX = tryX;
                        currentY = nextY;
                        current->moved = true;
                        movedSideways = true;
                        break;
                    }
                    tryX = currentX + slideDist;
                    if (tryX < width && !isWall[nextY][tryX] && cells[nextY][tryX] == nullptr) {
                        cells[nextY][tryX] = current;
                        cells[currentY][currentX] = nullptr;
                        currentX = tryX;
                        currentY = nextY;
                        current->moved = true;
                        movedSideways = true;
                        break;
                    }
                }

                if (!movedSideways) {
                    current->velocity = 1;
                    break;
                }
            }
        }

        if (!current->moved) {
            current->immobile_frames++;
        } else {
            current->immobile_frames = 0;
        }
    }
}

void Hourglass_Grid::set(int x, int y, Grain* grain) {
    if (x >= 0 && x < width && y >= 0 && y < height) {
        cells[y][x] = grain;
    }
}

Grain* Hourglass_Grid::get(int x, int y) {
    if (x >= 0 && x < width && y >= 0 && y < height) {
        return cells[y][x];
    }
    return nullptr;
}

bool Hourglass_Grid::is_inside_hourglass(int x, int y) {
    if (y < height * 0.05f || y >= height * 0.95f)
        return false;
    int inner_left_wall;
    int inner_right_wall;
    for (int i = 0; i < width; ++i) {
        if (isWall[y][i]) {
            inner_left_wall = i + 5;
            break;
        }
    }
    for (int i = width; i > 0; --i) {
        if (isWall[y][i]) {
            inner_right_wall = i - 5;
            break;
        }
    }
    return x > inner_left_wall && x < inner_right_wall;
}

Hourglass_Grid::~Hourglass_Grid() {
    for (int y = 0; y < height; ++y) {
        for (int x = 0; x < width; ++x) {
            delete cells[y][x];
        }
    }
}

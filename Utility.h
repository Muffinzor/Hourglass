//
// Created by leo on 7/11/25.
//

#ifndef UTILITY_H
#define UTILITY_H
#include <cstdlib>
#include <SFML/Graphics/Color.hpp>


class Utility {
public:
    static int random_int(int min, int max) {
        return rand() % (max - min) + min;
    }
    static bool random_bool() {
        return rand() % 2;
    }
    static sf::Color HSVtoRGB(float hue, float sat, float val) {
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
};



#endif //UTILITY_H

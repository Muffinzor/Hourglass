//
// Created by leo on 7/11/25.
//

#ifndef UTILITY_H
#define UTILITY_H
#include <cstdlib>
#include <SFML/Graphics/Color.hpp>
#include <SFML/Graphics/Font.hpp>
#include <SFML/Graphics/RenderWindow.hpp>
#include <SFML/Graphics/Text.hpp>


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

    static void display_instructions(sf::RenderWindow& window, const sf::Font& font) {
        sf::Text instructionText;
        instructionText.setFont(font);
        instructionText.setString("Press Space to reverse the hourglass");
        instructionText.setCharacterSize(16);
        instructionText.setFillColor(sf::Color::White);

        instructionText.setPosition(
            window.getSize().x / 2.f - instructionText.getLocalBounds().width / 2.f,
            window.getSize().y - instructionText.getLocalBounds().height - 15
        );

        window.draw(instructionText);

        sf::Text instructionText2;
        instructionText2.setFont(font);
        instructionText2.setString("Click inside the hourglass to add sand");
        instructionText2.setCharacterSize(16);
        instructionText2.setFillColor(sf::Color::White);

        instructionText2.setPosition(
            window.getSize().x / 2.f - instructionText2.getLocalBounds().width / 2.f,
            0 + instructionText2.getLocalBounds().height - 5
        );

        window.draw(instructionText2);
    }
    static void display_grain_count(sf::RenderWindow& window, const sf::Font& font, int grains) {
        sf::Text grain_count;
        grain_count.setFont(font);
        grain_count.setString("Grains of sand: " + std::to_string(grains));
        grain_count.setCharacterSize(16);
        grain_count.setFillColor(sf::Color::White);

        sf::FloatRect bounds = grain_count.getLocalBounds();
        grain_count.setPosition(
            20.f,
            window.getSize().y / 2.f - bounds.height / 2.f
        );

        window.draw(grain_count);
    }
};

#endif //UTILITY_H

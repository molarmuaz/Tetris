#include <SFML/Graphics.hpp>
#include <time.h>
#include <fstream>
#include <iostream>
#include <string>

#include "utils.h"
#include "piece.h"
#include "functionality.h"

using namespace sf;

int main(void)
{
    srand(time(0));

    RenderWindow window(VideoMode(320, 480), "TETRIS by Muaz Ahmed");

    Font font;
    font.loadFromFile("DIGITAL-7 (MONO).TTF");

    Texture obj1,obj2,obj3, obj4, obj5, obj6;
    obj1.loadFromFile("img/tiles.png");
    obj2.loadFromFile("img/background.png");
    obj3.loadFromFile("img/frame.png");
    obj4.loadFromFile("img/overlay.png");
    obj5.loadFromFile("img/lose.png");
    obj6.loadFromFile("img/highscore.png");
    Sprite sprite(obj1), background(obj2), frame(obj3), overlay(obj4), lose(obj5), hiscore(obj6);

    // Game state
    Clock clock;
    bool paused = false;
    bool lost = false;
    bool hs = false;
    int colorNum = 1;
    float timer = 0;
    int score = 0;

    // Controls
    bool restart = false;
    bool rotate = false;
    bool drop = false;
    float delay = 0.3;
    int delta_x = 0;

    // Highscore
    int highscore[10] = { 0 };
    readHighscore(highscore);

    while (window.isOpen()) {
        float time = clock.getElapsedTime().asSeconds();
        clock.restart();
        timer += time;

        delta_x = 0;
        rotate = false;
        delay = 0.3;

        Event e;
        while (window.pollEvent(e)) {
            if (e.type == Event::Closed)
                window.close();

            if (e.type == Event::KeyPressed) {
                switch (e.key.code) {
                    case Keyboard::Up:
                        rotate = true;
                        break;
                    case Keyboard::Left:
                        delta_x = -1;
                        break;
                    case Keyboard::Right:
                        delta_x = 1;
                        break;
                    case Keyboard::Space:
                        drop = true;
                        break;
                    case Keyboard::P:
                        paused = !paused;
                        break;
                    case Keyboard::H:
                        hs = (paused || lost) ? !hs : hs;
                        break;
                    case Keyboard::R:
                        restart = paused || lost;
                        break;
                    default:
                        break;
                };
            }
        }

        // Game restart
        if (restart) {
            for (int i = 0; i < M; i++) {
                for (int j = 0; j < N; j++)
                    gameGrid[i][j] = 0;
            }

            for (int i = 0; i < 4; i++) {
                point_1[i][0] = 0;
                point_1[i][1] = 0;
            }

            timer = 0;
            colorNum = 1;
            score = 0;
            restart = false;
            paused = false;
            lost = false;
            drop = false;
            continue;
        }

        // Show highscore screen
        if (hs) {
            window.clear(Color::Black);
            window.draw(hiscore);

            Text text;
            text.setFont(font);
            text.setFillColor(Color::White);
            text.setStyle(Text::Bold);
            text.setCharacterSize(24);
            for (int i = 0; i < 10; i++) {
                std::string s;
                if (i < 9)
                    s += " ";
                s += std::to_string(i + 1);

                s += "...............";

                int score = highscore[i];
                if (score < 10)
                    s += "  ";
                else if (score < 100)
                    s += " ";
                s += std::to_string(highscore[i]);
                text.setString(s);

                text.setPosition(45, 80 + i * text.getCharacterSize());
                window.draw(text);
           }

            window.display();
            continue;
        }

        // Show pause screen
        if (paused) {
            window.clear(Color::Black);
            window.draw(overlay);
            window.display();
            continue;
        }

        // Show lost screen
        if (lost) {
            window.clear(Color::Black);
            window.draw(lose);
            window.display();
            continue;
        }

        // Update pieces
        hs = false;

        if (Keyboard::isKeyPressed(Keyboard::Down))
            delay = 0.05;

        if (rotate) {
            rotates(point_1[1][0], point_1[1][1]);

            if (!anamoly()) {
                for (int i = 0; i < 4; i++) {
                    point_1[i][0] = point_2[i][0];
                    point_1[i][1] = point_2[i][1];
                }
            }
        }

        move(delta_x);
        fallingPiece(timer, delay, colorNum, drop);

        // Update score
        checkLines(score);

        // Check for lost
        for (int i = 0; i < N; i++) {
            if (gameGrid[0][i]) {
                updateHighscore(highscore, score);
                lost = true;
                break;
            }
        }

        // Drawing
        window.clear(Color::Black);
        window.draw(background);

        for (int i = 0; i < M; i++) {
            for (int j = 0; j < N; j++) {
                if (!gameGrid[i][j])
                    continue;

                sprite.setTextureRect(IntRect(gameGrid[i][j] * 18, 0, 18, 18));
                sprite.setPosition(j * 18, i * 18);
                sprite.move(28, 31); //offset
                window.draw(sprite);
            }
        }

        for (int i = 0; i < 4; i++) {
            sprite.setTextureRect(IntRect(colorNum * 18, 0, 18, 18));
            sprite.setPosition(point_1[i][0] * 18, point_1[i][1] * 18);
            sprite.move(28, 31); //offset
            window.draw(sprite);
        }

        window.draw(frame);
        window.display();
    }
}

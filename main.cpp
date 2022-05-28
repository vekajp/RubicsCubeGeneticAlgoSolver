

#include <iostream>
#include <vector>
#include <string>
#include "cubic.hpp"
#include <SFML/Graphics.hpp>
#include "genetic.hpp"
#define EDGE 50
#define GAP 52
#define OUTLINE 2
#define CHARACTERS_ON_THE_LINE 50
sf::Color getColor(int n) {
    sf::Color color(0, 0, 0);
    if (n == 0) return sf::Color(255, 171, 43);
    if (n == 1) return sf::Color(255, 228, 43);
    if (n == 2) return sf::Color(43, 255, 64);
    if (n == 3) return sf::Color(255, 255, 255);
    if (n == 4) return sf::Color(255, 77, 0);
    if (n == 5) return sf::Color(0, 119, 255);
    return sf::Color::Transparent;
}
void draw(sf::RenderWindow& window, const Cube& cube, int offset) {
    for (int j = 0; j < cube.number(); j++) {
        for (int k = 0; k < cube.number(); k++) {
            sf::RectangleShape rectangle(sf::Vector2f(EDGE, EDGE));
            rectangle.setFillColor(getColor(cube.getCube()[0][j][k]));
            rectangle.setPosition((cube.number() + 1 + k + offset) * GAP, (1 + j) * GAP);
            rectangle.setOutlineThickness(OUTLINE);
            rectangle.setOutlineColor(sf::Color::Black);
            window.draw(rectangle);
        }
    }
    for (int i = 1; i < 4; i++) {
        for (int j = 0; j < cube.number(); j++) {
            for (int k = 0; k < cube.number(); k++) {
                sf::RectangleShape rectangle(sf::Vector2f(EDGE, EDGE));
                rectangle.setFillColor(getColor(cube.getCube()[i][j][k]));
                rectangle.setPosition((1 + k + cube.number() * (i - 1) + offset) * GAP, (cube.number() + 1 + j) * GAP);
                rectangle.setOutlineThickness(OUTLINE);
                rectangle.setOutlineColor(sf::Color::Black);
                window.draw(rectangle);
            }
        }
    }
    for (int i = 4; i < 6; i++) {
        for (int j = 0; j < cube.number(); j++) {
            for (int k = 0; k < cube.number(); k++) {
                sf::RectangleShape rectangle(sf::Vector2f(EDGE, EDGE));
                rectangle.setFillColor(getColor(cube.getCube()[i][j][k]));
                rectangle.setPosition((cube.number() + 1 + k + offset) * GAP, (1 + j + cube.number() * (i - 2)) * GAP);
                rectangle.setOutlineThickness(OUTLINE);
                rectangle.setOutlineColor(sf::Color::Black);
                window.draw(rectangle);
            }
        }
    }
}
void displayAnswer(std::string ans, sf::Text& solutionMoves, sf::RenderWindow& window) {
    long pos = 0;
    std::string result = "";
    while (pos + CHARACTERS_ON_THE_LINE < ans.length()) {
        result += (ans.substr(pos, CHARACTERS_ON_THE_LINE) + "\n");
        pos += CHARACTERS_ON_THE_LINE;
    }
    result += (ans.substr(pos) + "\n");
    solutionMoves.setString(result);
    window.draw(solutionMoves);
}
void displayInfo(sf::RenderWindow& window, sf::Text& header,
                 sf::Text& generationCount, sf::Text& bestScore,
                 sf::Text& startMoves, int generation, long best_score,
                 std::string start_moves) {
    header.setString("Start position: ");
    startMoves.setString(start_moves);
    generationCount.setString("Generation: " + std::to_string(generation));
    bestScore.setString("Best fitness: " + std::to_string(best_score));
    window.draw(header);
    window.draw(startMoves);
    window.draw(generationCount);
    window.draw(bestScore);
}

int main(int argc, const char * argv[]) {
    Cube startCube = Cube(3);
    startCube.randomGenerate();
    Cube resultCube = startCube;
    Population popul = Population(300, startCube);
    Individual bestChoice;
    bestChoice.empty();
    bestChoice.fitness(startCube);
    
    std::string solution;
    bool pause = true;
    bool solved_current = true;
    
    sf::RenderWindow window(sf::VideoMode((3 * startCube.number() + 2) * GAP * 3, (4 * startCube.number() + 3) * GAP), "memorial of my fine mental state");
    sf::Font font;
    if (!font.loadFromFile("../res/arial.ttf")) {
        return EXIT_FAILURE;
    }
    sf::Text header, generationCount, bestScore, startMoves, solutionMoves;
    header.setFont(font); generationCount.setFont(font); bestScore.setFont(font); startMoves.setFont(font); solutionMoves.setFont(font);
    
    generationCount.setPosition(startCube.number() * 4 * EDGE, GAP * 2 + 5); bestScore.setPosition(startCube.number() * 4 * EDGE, GAP * 3 + 5);
    header.setPosition(startCube.number() * 4 * EDGE, 5); startMoves.setPosition(startCube.number() * 4 * EDGE, GAP + 5);
    solutionMoves.setPosition(startCube.number() * 4 * EDGE, GAP * 4 + 5);
    while (window.isOpen()) {
        sf::Event event;
        while (window.pollEvent(event)) {
            switch (event.type) {
                case sf::Event::Closed:
                    window.close();
                    break;
                case sf::Event::KeyPressed:
                    if (event.key.code == sf::Keyboard::Space) {
                        solved_current = false;
                        pause = !pause;
                    } else {
                        if (event.key.code == sf::Keyboard::F) {
                            pause = true;
                            startCube = Cube(3);
                            startCube.randomGenerate();
                            draw(window, startCube, 0);
                            solution = "";
                            popul.init();
                        }
                        if (solved_current) {
                            if (popul.found()) {
                                startCube = Cube(3);
                            }
                            if (event.key.code == sf::Keyboard::W) {
                                startCube.addMove({0, 1});
                            }
                            if (event.key.code == sf::Keyboard::A) {
                                startCube.addMove({1, 1});
                            }
                            if (event.key.code == sf::Keyboard::S) {
                                startCube.addMove({2, 1});
                            }
                            if (event.key.code == sf::Keyboard::D) {
                                startCube.addMove({3, 1});
                            }
                            if (event.key.code == sf::Keyboard::X) {
                                startCube.addMove({4, 1});
                            }
                            if (event.key.code == sf::Keyboard::C) {
                                startCube.addMove({5, 1});
                            }
                            if (popul.found()) {
                                popul.init();
                            }
                        }
                        resultCube = startCube;
                    }
                    break;
                    
                default:
                    break;
            }
        }
        
        window.clear(sf::Color::Black);
        if (pause) {
            draw(window, resultCube,  0);
            displayInfo(window, header, generationCount, bestScore, startMoves, popul.getGenerations(), bestChoice.getFitness(), startCube.getMoves());
            displayAnswer(solution, solutionMoves, window);
            window.display();
            continue;
        } else {
            if (!popul.found()) {
                popul.evoluteNewPopulation();
                bestChoice = popul.getFittest();
                solution = movesToString(bestChoice.getGenes());
                resultCube = startCube;
                resultCube = bestChoice.implement(resultCube);
            } else {
                pause = true;
                solved_current = true;
                std::cout << solution;
            }
        }
        
        displayAnswer(solution, solutionMoves, window);
        window.draw(solutionMoves);
        draw(window, resultCube,  0);
        displayInfo(window, header, generationCount, bestScore, startMoves, popul.getGenerations(), bestChoice.getFitness(), startCube.getMoves());

        window.display();
        
    }
    return 0;
}

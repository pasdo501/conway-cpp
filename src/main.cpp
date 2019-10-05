#include <iostream>
#include <chrono>
#include <SFML/Graphics.hpp>
#include "conway.hpp"
#include "cli_args.hpp"

using namespace std;

int main(int argc, char *argv[])
{
    cli_args::Parser parser = cli_args::Parser();
    if(EXIT_FAILURE == parser.parse_args(argc, argv)) {
        return EXIT_FAILURE;
    }

    int height = parser.get_height();
    int width = parser.get_width();
    int prob_alive = parser.get_prob_alive();
    int max_generations = parser.get_generations();
    int generation = 0;

    conway::Grid *g = new conway::Grid(height, width, prob_alive);
    sf::RenderWindow window(sf::VideoMode(width, height), "Conway's Game of Life", sf::Style::Titlebar | sf::Style::Close);
    window.setVerticalSyncEnabled(true);

    auto start = chrono::high_resolution_clock::now();
    sf::Event event;
    while (window.isOpen() && generation < max_generations) {
        while (window.pollEvent(event)) {
            switch(event.type) {
            case sf::Event::Closed:
                window.close();
                break;
            case sf::Event::Resized:
            {
                sf::FloatRect visibleArea(0, 0, event.size.width, event.size.height);
                window.setView(sf::View(visibleArea));
                break;
            }
            case sf::Event::KeyPressed:
                if (event.key.code == sf::Keyboard::Escape) {
                    window.close();
                }
                break;
            default:
                break;
            }
        }

        if (generation != 0) g->next_tick();
        sf::VertexArray p(sf::Points);
        for(int i = 0; i < height; i++) {
            for (int j = 0; j < width; j++) {
                if (g->alive_at(i, j)) {
                    float x = static_cast<float>(j);
                    float y = static_cast<float>(i);
                    p.append(sf::Vertex(sf::Vector2f(x, y), sf::Color::Green));
                }
            }
        }
        window.clear();
        window.draw(p);
        window.display();
        generation++;
    }

    window.close();
    delete g;
    auto stop = chrono::high_resolution_clock::now();
    auto duration = chrono::duration_cast<chrono::milliseconds>(stop - start).count();
    int generations_per_second = (int) (((double) generation / (double) duration) * 1000);
    std::cout << "Time: " << duration << " ms" << std::endl;
    std::cout << generations_per_second << " generations / second" << std::endl;

    return EXIT_SUCCESS;
}
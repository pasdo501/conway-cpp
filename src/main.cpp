#include <iostream>
#include <chrono>
#include <SFML/Graphics.hpp>
#include "conway.hpp"

using namespace std;

int main()
{
    int size;
    float prob_alive;
    int int_prob;
    int max_generations;
    int generation = 0;

    cout << "Enter size of grid (same width & height): ";
    cin >> size;

    cout << "Enter chance of being alive (float between 0 & 1): ";
    cin >> prob_alive;
    int_prob = (int) (prob_alive * 10);

    cout << "Enter the number of generations: ";
    cin >> max_generations;

    conway::Grid *g = new conway::Grid(size, size, int_prob);
    sf::RenderWindow window(sf::VideoMode(size, size), "Conway's Game of Life", sf::Style::Titlebar | sf::Style::Close);
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
        for(int i = 0; i < size; i++) {
            for (int j = 0; j < size; j++) {
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
    cout << "Time: " << duration << " ms" << endl;
    cout << generations_per_second << " generations / second" << endl;

    return 0;
}
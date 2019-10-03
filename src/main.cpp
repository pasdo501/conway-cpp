#include <iostream>
#include <chrono>
#include <SFML/Graphics.hpp>
#include "conway_class.hpp"

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

    srand(time(NULL));
    conway::Grid *g = new conway::Grid(size, size, int_prob);
    auto start = chrono::high_resolution_clock::now();
    while (generation < max_generations) {
        g->next_tick();
        generation++;
    }
    delete g;
    auto stop = chrono::high_resolution_clock::now();
    auto duration = chrono::duration_cast<chrono::milliseconds>(stop - start).count();
    int generations_per_second = (int) (((double) generation / (double) duration) * 1000);
    cout << "Time: " << duration << " ms" << endl;
    cout << generations_per_second << " generations / second" << endl;

    return 0;
}
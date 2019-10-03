#include <iostream>
#include <cstdlib>
#include <vector>
#include <chrono>

using namespace std;

static const int BIRTH_THRESH = 3;
static const int ISOLATAION_TRESH = 1;
static const int OVERCROWDING_THRESH = 4;

void debug_display(unsigned char **grid, int n)
{
    for (int i = 0; i < n; i++) {
        for (int j = 0; j < n; j++) {
            cout << i << ", " << j << ": " << ((int) grid[i][j]) << endl;
        }
    }
}

void set_neighbours(unsigned char **grid, int n)
{
    for (int i = 0; i < n; i++) {
        for (int j = 0; j < n; j++) {
            unsigned char neighbours = 0x00;

            // TOP
            if (i == 0) {
                // LEFT
                if (j == 0) {
                    if (grid[n-1][n-1] & 0x01) neighbours += 1;
                } else {
                    if (grid[n-1][j-1] & 0x01) neighbours += 1;
                }

                // CENTER
                if (grid[n-1][j] & 0x01) neighbours += 1;

                // RIGHT
                if (j == n-1) {
                    if (grid[n-1][0] & 0x01) neighbours += 1;
                } else {
                    if (grid[n-1][j+1] & 0x01) neighbours += 1;
                }
            } else {
                // LEFT
                if (j == 0) {
                    if (grid[i-1][n-1] & 0x01) neighbours += 1;
                } else {
                    if (grid[i-1][j-1] & 0x01) neighbours += 1;
                }

                // CENTER
                if (grid[i-1][j] & 0x01) neighbours += 1;

                // RIGHT
                if (j == n-1) {
                    if (grid[i-1][0] & 0x01) neighbours += 1;
                } else {
                    if (grid[i-1][j+1] & 0x01) neighbours += 1;
                }
            }

            // LEFT
            if (j == 0) {
                if (grid[i][n-1] & 0x01) neighbours += 1;
            } else {
                if (grid[i][j-1] & 0x01) neighbours += 1;
            }

            // RIGHT
            if (j == n-1) {
                if (grid[i][0] & 0x01) neighbours += 1;
            } else {
                if (grid[i][j+1] & 0x01) neighbours += 1;
            }

            // BOTTOM
            if (i == n-1) {
                // LEFT
                if (j == 0) {
                    if (grid[0][n-1] & 0x01) neighbours += 1;
                } else {
                    if (grid[0][j-1] & 0x01) neighbours += 1;
                }

                // CENTER
                if (grid[0][j] & 0x01) neighbours += 1;

                // RIGHT
                if (j == n-1) {
                    if (grid[0][0] & 0x01) neighbours += 1;
                } else {
                    if (grid[0][j+1] & 0x01) neighbours += 1;
                }
            } else {
                // LEFT
                if (j == 0) {
                    if (grid[i+1][n-1] & 0x01) neighbours += 1;
                } else {
                    if (grid[i+1][j-1] & 0x01) neighbours += 1;
                }

                // CENTER
                if (grid[i+1][j] & 0x01) neighbours += 1;

                // RIGHT
                if (j == n-1) {
                    if (grid[i+1][0] & 0x01) neighbours += 1;
                } else {
                    if (grid[i+1][j+1] & 0x01) neighbours += 1;
                }
            }

            neighbours <<= 1;
            grid[i][j] |= neighbours;
        }
    }
}

void toggle_cell(unsigned char **grid, int i, int j, int n)
{
    int delta = (grid[i][j] & 0x01) ? -2 : 2;
    grid[i][j] ^= 0x01;

    // Update neighbours
    // TOP
    if (i == 0) {
        // LEFT
        if (j == 0) {
            grid[n-1][n-1] += delta;
        } else {
            grid[n-1][j-1] += delta;
        }

        // CENTER
        grid[n-1][j] += delta;

        // RIGHT
        if (j == n-1) {
            grid[n-1][0] += delta;
        } else {
            grid[n-1][j+1] += delta;
        }
    } else {
        // LEFT
        if (j == 0) {
            grid[i-1][n-1] += delta;
        } else {
            grid[i-1][j-1] += delta;
        }

        // CENTER
        grid[i-1][j] += delta;

        // RIGHT
        if (j == n-1) {
            grid[i-1][0] += delta;
        } else {
            grid[i-1][j+1] += delta;
        }
    }

    // LEFT
    if (j == 0) {
        grid[i][n-1] += delta;
    } else {
        grid[i][j-1] += delta;
    }

    // RIGHT
    if (j == n-1) {
        grid[i][0] += delta;
    } else {
        grid[i][j+1] += delta;
    }

    // BOTTOM
    if (i == n-1) {
        // LEFT
        if (j == 0) {
            grid[0][n-1] += delta;
        } else {
            grid[0][j-1] += delta;
        }

        // CENTER
        grid[0][j] += delta;

        // RIGHT
        if (j == n-1) {
            grid[0][0] += delta;
        } else {
            grid[0][j+1] += delta;
        }
    } else {
        // LEFT
        if (j == 0) {
            grid[i+1][n-1] += delta;
        } else {
            grid[i+1][j-1] += delta;
        }

        // CENTER
        grid[i+1][j] += delta;

        // RIGHT
        if (j == n-1) {
            grid[i+1][0] += delta;
        } else {
            grid[i+1][j+1] += delta;
        }
    }
}

void generation_tick(unsigned char **grid, int n)
{
    vector<vector<int>> c;
    for (int i = 0; i < n; i++) {
        for (int j = 0; j < n; j++) {
            if (grid[i][j] == 0) continue;

            int neighbours = grid[i][j] >> 1;
            if (grid[i][j] & 0x01) {
                // LIVE CELL
                if (neighbours >= OVERCROWDING_THRESH ||
                    neighbours <= ISOLATAION_TRESH) {
                    c.push_back({i, j});
                }
            } else {
                // DEAD CELL
                if (neighbours == BIRTH_THRESH) c.push_back({i, j});
            }
        }
    }

    while (c.size()) {
        vector<int> curr = c.back();
        c.pop_back();
        toggle_cell(grid, curr[0], curr[1], n);
    }
}

void cleanup(unsigned char **grid, int n)
{
    for (int i = 0; i < n; i++) {
        delete [] grid[i];
    }
    delete [] grid;
}

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
    unsigned char **grid = new unsigned char *[size];

    for (int i = 0; i < size; i++) {
        grid[i] = new unsigned char[size];
        for(int j = 0; j < size; j++) {
            if (rand() % 10 < int_prob) {
                grid[i][j] = 0x01;
            } else {
                grid[i][j] = 0x00;
            }
        }
    }

    set_neighbours(grid, size);
    auto start = chrono::high_resolution_clock::now();
    while (generation < max_generations) {
        generation_tick(grid, size);
        generation++;
    }
    auto stop = chrono::high_resolution_clock::now();
    auto duration = chrono::duration_cast<chrono::milliseconds>(stop - start).count();
    int generations_per_second = (int) (((double) generation / (double) duration) * 1000);
    cout << "Time: " << duration << " ms" << endl;
    cout << generations_per_second << " generations / second" << endl;

    cleanup(grid, size);
    return 0;
    // debug_display(grid, size);
}
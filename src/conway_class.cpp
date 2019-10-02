#include <iostream>
#include <cstdlib>
#include <vector>
#include <chrono>
#include <stack>
#include <omp.h>

using namespace std;

static const int BIRTH_THRESH = 3;
static const int ISOLATAION_TRESH = 1;
static const int OVERCROWDING_THRESH = 4;

class Grid
{
    private:
        class Cell
        {
            private:
                unsigned char state;
                vector<Cell*> neighbours;
            public:
                Cell(unsigned char state)
                {
                    this->state = state;
                }

                ~Cell() {}

                void set_neighbours(Grid &g, int row, int col, int height, int width)
                {
                    Cell *temp;

                    if (row == 0) {
                        // TOP ROW - WRAP TO BOTTOM
                        // TOP LEFT
                        temp = col == 0
                            ? g.get(height-1, width-1)
                            : g.get(height-1, col-1);
                        neighbours.push_back(temp);
                        if (temp->is_alive()) increase_neighbour_count();

                        // TOP CENTER
                        temp = g.get(height-1, col);
                        neighbours.push_back(temp);
                        if (temp->is_alive()) increase_neighbour_count();

                        // TOP RIGHT
                        temp = col == width - 1
                            ? g.get(height-1, 0)
                            : g.get(height-1, col+1);
                        if (temp->is_alive()) increase_neighbour_count();
                        neighbours.push_back(temp);
                    } else {
                        // TOP LEFT
                        temp = col == 0
                            ? g.get(row-1, width-1)
                            : g.get(row-1, col-1);
                        neighbours.push_back(temp);
                        if (temp->is_alive()) increase_neighbour_count();

                        // TOP CENTER
                        temp = g.get(row-1, col);
                        neighbours.push_back(temp);
                        if (temp->is_alive()) increase_neighbour_count();

                        // TOP RIGHT
                        temp = col == width-1
                            ? g.get(row-1, 0)
                            : g.get(row-1, col+1);
                        neighbours.push_back(temp);
                        if (temp->is_alive()) increase_neighbour_count();
                    }

                    // LEFT
                    temp = col == 0
                        ? g.get(row, width-1)
                        : g.get(row, col-1);
                    neighbours.push_back(temp);
                    if (temp->is_alive()) increase_neighbour_count();

                    // RIGHT
                    temp = col == width - 1
                        ? g.get(row, 0)
                        : g.get(row, col+1);
                    neighbours.push_back(temp);
                    if (temp->is_alive()) increase_neighbour_count();

                    if (row == height-1) {
                        // BOTTOM ROW - WRAP TO TOP
                        // BOTTOM LEFT
                        temp = col == 0
                            ? g.get(0, width - 1)
                            : g.get(0, col - 1);
                        neighbours.push_back(temp);
                        if (temp->is_alive()) increase_neighbour_count();

                        // BOTTOM CENTER
                        temp = g.get(0, col);
                        neighbours.push_back(temp);
                        if (temp->is_alive()) increase_neighbour_count();

                        // BOTTOM RIGHT
                        temp = col == width - 1
                            ? g.get(0, 0)
                            : g.get(0, col+1);
                        neighbours.push_back(temp);
                        if (temp->is_alive()) increase_neighbour_count();
                    } else {
                        // BOTTOM LEFT
                        temp = col == 0
                            ? g.get(row+1, width-1)
                            : g.get(row+1, col-1);
                        neighbours.push_back(temp);
                        if (temp->is_alive()) increase_neighbour_count();

                        // BOTTOM CENTER
                        temp = g.get(row+1, col);
                        neighbours.push_back(temp);
                        if (temp->is_alive()) increase_neighbour_count();

                        // BOTTOM RIGHT
                        temp = col == height-1
                            ? g.get(row+1, 0)
                            : g.get(row+1, col+1);
                        neighbours.push_back(temp);
                        if (temp->is_alive()) increase_neighbour_count();
                    }
                }

                unsigned char get_state()
                {
                    return state;
                }

                bool is_alive()
                {
                    return state & 0x01;
                }

                void decrease_neighbour_count()
                {
                    state -= 2;
                }

                void increase_neighbour_count()
                {
                    state += 2;
                }

                void toggle_cell()
                {
                    if (is_alive()) {
                        for (auto neighbour : neighbours)
                            neighbour->decrease_neighbour_count();
                    } else {
                        for (auto neighbour : neighbours)
                            neighbour->increase_neighbour_count();
                    }
                    state ^= 0x01;
                }
        }; // Cell Inner Class

        // Remaining private fields
        int width;
        int height;
        vector<vector<Cell*>> grid_;
        Cell* get(int row, int col)
        {
            return grid_[row][col];
        }

    public:
        Grid(int height, int width, int prob_alive)
        {

            srand(time(NULL));
            this->width = width;
            this->height = height;
            for (int i = 0; i < height; i++) {
                vector<Cell*> row;
                grid_.push_back(row);
                for (int j = 0; j < width; j++) {
                    unsigned char initial_state = rand() % 10 < prob_alive 
                        ? 0x01
                        : 0x00;
                    Cell *c = new Cell(initial_state);
                    grid_[i].push_back(c);
                }
            }

            for (int i = 0; i < height; i++) {
                for (int j = 0; j < width; j++) {
                    grid_[i][j]->set_neighbours(*this, i, j, height, width);
                }
            }
        }

        ~Grid() {
            for (int i = 0; i < height; i++) {
                for (auto c : grid_[i]) {
                    delete c;
                }
            }
        }

        void display()
        {
            for (int i = 0; i < height; i++) {
                for (int j = 0; j < width; j++) {
                    if (grid_[i][j]->get_state() & 0x01) {
                        cout << " * ";
                    } else {
                        cout << " o ";
                    }
                }
                cout << endl;
            }
        }

        void next_tick()
        {
            stack<Cell*> s;
            
            #pragma omp parallel
            {
                int t_id = omp_get_thread_num();
                int t_total = omp_get_num_threads();

                int seg_length = height / t_total;
                int start = seg_length * t_id;
                int end = t_id == t_total - 1 ? height : seg_length * (t_id + 1);

                for (int i = start; i < end; i++) {
                    for (int j = start; j < end; j++) {
                        Cell *c = grid_[i][j];
                        unsigned char state = c->get_state();
                        if (state == 0) continue;

                        unsigned char neighbour_count = state >> 1;
                        if (state & 0x01) {
                            // Live cell
                            if (neighbour_count >= OVERCROWDING_THRESH ||
                                neighbour_count <= ISOLATAION_TRESH) {
                                #pragma omp critical
                                { s.push(c); }
                            }
                        } else {
                            // Dead cell
                            if (neighbour_count == BIRTH_THRESH) {
                                #pragma omp critical
                                { s.push(c); }
                            }
                        }
                    }
                }
            }
            while (!s.empty()) {
                s.top()->toggle_cell();
                s.pop();
            }
        }
};

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
    Grid *g = new Grid(size, size, int_prob);
    auto start = chrono::high_resolution_clock::now();
    while (generation < max_generations) {
        // g->display();
        g->next_tick();
        // cout << "---------------------------------" << endl;
        generation++;
    }
    delete g;
    auto stop = chrono::high_resolution_clock::now();
    auto duration = chrono::duration_cast<chrono::milliseconds>(stop - start).count();
    int generations_per_second = (int) (((double) generation / (double) duration) * 1000);
    cout << "Time: " << duration << " ms" << endl;
    cout << generations_per_second << " generations / second" << endl;

    return 0;
    // debug_display(grid, size);
}
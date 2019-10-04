#pragma once
#include <vector>

namespace conway
{
    static const int BIRTH_THRESH = 3;
    static const int ISOLATION_THRESH = 1;
    static const int OVERCROWDING_THRESH = 4;

    class Grid
    {
        private:
            class Cell
            {
                private:
                    unsigned char state;
                    std::vector<Cell*> neighbours;
                public:
                    Cell(unsigned char state) { this->state = state; };
                    ~Cell() { };
                    void set_neighbours(Grid &g, int row, int col, int height, int width);
                    unsigned char get_state() { return state; };
                    bool is_alive() { return state & 0x01; };
                    void decrease_neighbour_count() { state -= 2; };
                    void increase_neighbour_count() { state += 2; };
                    void toggle_cell();
            }; // Cell

            int width;
            int height;
            std::vector<std::vector<Cell*>> grid_;
            Cell* get(int row, int col) { return grid_[row][col]; };
        
        public:
            Grid(int height, int width, int prob_alive);
            ~Grid();
            bool alive_at(int row, int col);
            void next_tick();
    };
}
#include "conway.hpp"
#include <cstdlib>
#include <vector>
#include <stack>
#include <chrono>
#include <omp.h>

using namespace std;
using namespace conway;

void Grid::Cell::set_neighbours(Grid& g, int row, int col, int height, int width)
{
	Cell* temp;

	if (row == 0) {
		// TOP ROW - WRAP TO BOTTOM
		// TOP LEFT
		temp = col == 0
			? g.get(height - 1, width - 1)
			: g.get(height - 1, col - 1);
		neighbours.push_back(temp);
		if (temp->is_alive()) increase_neighbour_count();

		// TOP CENTER
		temp = g.get(height - 1, col);
		neighbours.push_back(temp);
		if (temp->is_alive()) increase_neighbour_count();

		// TOP RIGHT
		temp = col == width - 1
			? g.get(height - 1, 0)
			: g.get(height - 1, col + 1);
		if (temp->is_alive()) increase_neighbour_count();
		neighbours.push_back(temp);
	}
	else {
		// TOP LEFT
		temp = col == 0
			? g.get(row - 1, width - 1)
			: g.get(row - 1, col - 1);
		neighbours.push_back(temp);
		if (temp->is_alive()) increase_neighbour_count();

		// TOP CENTER
		temp = g.get(row - 1, col);
		neighbours.push_back(temp);
		if (temp->is_alive()) increase_neighbour_count();

		// TOP RIGHT
		temp = col == width - 1
			? g.get(row - 1, 0)
			: g.get(row - 1, col + 1);
		neighbours.push_back(temp);
		if (temp->is_alive()) increase_neighbour_count();
	}

	// LEFT
	temp = col == 0
		? g.get(row, width - 1)
		: g.get(row, col - 1);
	neighbours.push_back(temp);
	if (temp->is_alive()) increase_neighbour_count();

	// RIGHT
	temp = col == width - 1
		? g.get(row, 0)
		: g.get(row, col + 1);
	neighbours.push_back(temp);
	if (temp->is_alive()) increase_neighbour_count();

	if (row == height - 1) {
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
			: g.get(0, col + 1);
		neighbours.push_back(temp);
		if (temp->is_alive()) increase_neighbour_count();
	}
	else {
		// BOTTOM LEFT
		temp = col == 0
			? g.get(row + 1, width - 1)
			: g.get(row + 1, col - 1);
		neighbours.push_back(temp);
		if (temp->is_alive()) increase_neighbour_count();

		// BOTTOM CENTER
		temp = g.get(row + 1, col);
		neighbours.push_back(temp);
		if (temp->is_alive()) increase_neighbour_count();

		// BOTTOM RIGHT
		temp = col == width - 1
			? g.get(row + 1, 0)
			: g.get(row + 1, col + 1);
		neighbours.push_back(temp);
		if (temp->is_alive()) increase_neighbour_count();
	}
}

void Grid::Cell::toggle_cell()
{
	if (is_alive()) {
		for (auto neighbour : neighbours)
			neighbour->decrease_neighbour_count();
	}
	else {
		for (auto neighbour : neighbours)
			neighbour->increase_neighbour_count();
	}
	state ^= 0x01;
}

Grid::Grid(int height, int width, int prob_alive)
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
			Cell* c = new Cell(initial_state);
			grid_[i].push_back(c);
		}
	}

	for (int i = 0; i < height; i++) {
		for (int j = 0; j < width; j++) {
			grid_[i][j]->set_neighbours(*this, i, j, height, width);
		}
	}
}

Grid::~Grid()
{
	for (int i = 0; i < height; i++) {
		for (auto c : grid_[i]) {
			delete c;
		}
	}
}

bool Grid::alive_at(int row, int col)
{
	return grid_[row][col]->is_alive();
}

void Grid::next_tick()
{
	stack<Cell*> s;

#pragma omp parallel
	{
		int t_id = omp_get_thread_num();
		int t_total = omp_get_num_threads();

		int seg_length = height / t_total;
		int start = seg_length * t_id;
		int end = t_id == t_total - 1 ? height : seg_length * (t_id + 1);
		stack<Cell*> t_stack;

		for (int i = start; i < end; i++) {
			for (int j = 0; j < width; j++) {
				Cell* c = grid_[i][j];
				unsigned char state = c->get_state();
				if (state == 0) continue;

				unsigned char neighbour_count = state >> 1;
				if (state & 0x01) {
					// Live cell
					if (neighbour_count >= OVERCROWDING_THRESH ||
						neighbour_count <= ISOLATION_THRESH) {
						t_stack.push(c);
					}
				}
				else {
					// Dead cell
					if (neighbour_count == BIRTH_THRESH) t_stack.push(c);
				}
			}
		}
#pragma omp critical
		{
			while (!t_stack.empty()) {
				s.push(t_stack.top());
				t_stack.pop();
			}
		}
	}
	while (!s.empty()) {
		s.top()->toggle_cell();
		s.pop();
	}
}
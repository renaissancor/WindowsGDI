#include "stdafx.h"
#include "Grid.h"

// Grid.cpp 

bool Grid::wall[GRID_WIDTH][GRID_HEIGHT] = { false, };

void Grid::ResetWall() noexcept {
	for (int x = 0; x < GRID_WIDTH; ++x) {
		for (int y = 0; y < GRID_HEIGHT; ++y) {
			wall[x][y] = false;
		}
	}
	// memset(wall, 0, sizeof(wall)); 
}

void Grid::Manager::Clear() noexcept {
	for (Node* node_ptr : _opened) {
		delete node_ptr;
	}
	_opened.clear();
	
}

void Grid::Manager::StepAStar() noexcept {
	auto cmpHeuristic = [](const Node* a, const Node* b) {
		return a->f < b->f; 
		};
	std::vector<Node*>::iterator itHeuristic = std::min_element(
		_opened.begin(), _opened.end(), cmpHeuristic
	); // O(n), later improve with indexed priority queue 

	Node* currNode = *itHeuristic; 
	Pos currPos = currNode->pos; 

	if (currPos == _endPos) return; // Reached the goal 
	
	_opened.erase(itHeuristic);


}
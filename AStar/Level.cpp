#include "stdafx.h"
#include "Level.h"

// Level.cpp 

bool Level::wall[GRID_WIDTH][GRID_HEIGHT] = { false, };

void Level::ResetWall() noexcept {
	for (int x = 0; x < GRID_WIDTH; ++x) {
		for (int y = 0; y < GRID_HEIGHT; ++y) {
			wall[x][y] = false;
		}
	}
	// memset(wall, 0, sizeof(wall)); 
}

void Level::Manager::Clear() noexcept {
    _opened.clear();
    for (int x = 0; x < GRID_WIDTH; ++x) {
        for (int y = 0; y < GRID_HEIGHT; ++y) {
            if (_grid[x][y] != nullptr) {
                delete _grid[x][y];
                _grid[x][y] = nullptr;
            }
            _closed[x][y] = false;
        }
    }
}

void Level::Manager::Start() noexcept {
    Clear();
    Node* startNode = new Node(_startPos.x, _startPos.y);
    startNode->g = 0;
    startNode->h = GetEuclidean(_startPos, _endPos);
    startNode->f = startNode->g + startNode->h;
    startNode->parent = { -1, -1 };
    _grid[_startPos.x][_startPos.y] = startNode;
    _opened.push_back(startNode);
}

void Level::Manager::Step() noexcept {
    if (_opened.empty()) {
        _state = State::SHOW_RESULT;
        return;
    }

    auto it = std::min_element(_opened.begin(), _opened.end(),
        [](const Node* a, const Node* b) { 
            if (a->f == b->f) {
                if (a->h == b->h) {
                    return a->g < b->g;
				}
				return a->h < b->h; 
            }
            return a->f < b->f; 
        });

    Node* curr = *it;

    if (curr->pos == _endPos) {
        _state = State::SHOW_RESULT;
        return;
    }

    _opened.erase(it);
    _closed[curr->pos.x][curr->pos.y] = true;

    const int dx[] = { -1, 0, 1, -1, 1, -1, 0, 1 };
    const int dy[] = { -1, -1, -1, 0, 0, 1, 1, 1 };
    const double cost[] = { 1.414, 1.0, 1.414, 1.0, 1.0, 1.414, 1.0, 1.414 };

    for (int i = 0; i < 8; ++i) {
        int nx = curr->pos.x + dx[i];
        int ny = curr->pos.y + dy[i];

        if (nx < 0 || nx >= GRID_WIDTH || ny < 0 || ny >= GRID_HEIGHT) continue;
        if (wall[nx][ny]) continue;
        if (_closed[nx][ny]) continue;

        double newG = curr->g + cost[i];

        if (_grid[nx][ny] != nullptr) {
            if (newG < _grid[nx][ny]->g) {
                _grid[nx][ny]->g = newG;
                _grid[nx][ny]->f = newG + _grid[nx][ny]->h;
                _grid[nx][ny]->parent = curr->pos;
            }
        }
        else {
            Node* newNode = new Node(nx, ny);
            newNode->g = newG;
            newNode->h = GetEuclidean({ nx, ny }, _endPos);
            newNode->f = newG + newNode->h;
            newNode->parent = curr->pos;
            _grid[nx][ny] = newNode;
            _opened.push_back(newNode);
        }
    }
}


void Level::Manager::RunAStar() noexcept {
    
}

void Level::Manager::NextState() noexcept {
    switch (_state) {
    case State::DRAW_WALL:
        _state = State::SET_POINTS;
        _setStart = false;
        _setEnd = false;
        _startPos = { -1, -1 };
        _endPos = { -1, -1 };
        break;
    case State::SET_POINTS:
        if (_setStart && _setEnd) {
            _state = State::RUN_ASTAR;
            Start();  
        }
        break;
    case State::RUN_ASTAR:
    case State::SHOW_RESULT:
        break;
    }
}

void Level::Manager::PrevState() noexcept {
    switch (_state) {
    case State::DRAW_WALL:
        break;
    case State::SET_POINTS:
        _state = State::DRAW_WALL;
        _setStart = false;
        _setEnd = false;
        break;
    case State::RUN_ASTAR:
    case State::SHOW_RESULT:
        Clear(); 
        _state = State::SET_POINTS;
        break;
    }
}


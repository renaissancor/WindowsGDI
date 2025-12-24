#pragma once 

// Input.hpp 

enum KEY {
	_0, _1, _2, _3, _4, _5, _6, _7, _8, _9,
	Q, W, E, R, T, Y, A, S, D, F, G, H, Z, X, C, V, B, N,
	ENTER, ESCAPE, SPACE, LEFT, UP, RIGHT, DOWN,
	LBTN, RBTN, KEY_COUNT
};

enum KEY_STATE {
	KEY_IDLE,		// 0 : No Press 
	KEY_TAPP,		// 1 : Pressed Moment 
	KEY_HOLD,		// 2 : During Press
	KEY_FREE		// 3 : Released Moment 
};

struct KeyInfo {
	KEY_STATE state;
	bool pressed; // Pressed in Previous Tick 
};

extern KeyInfo KeyState[KEY_COUNT];

extern int EnemiesCount;
extern int BulletsCount;

inline const KEY_STATE GetKeyState(KEY key) {
	return KeyState[key].state;
}

void InputCheck();

namespace Input {
	enum KEY {
		_0, _1, _2, _3, _4, _5, _6, _7, _8, _9,
		Q, W, E, R, T, Y, A, S, D, F, G, H, Z, X, C, V, B, N,
		ENTER, ESCAPE, SPACE, LEFT, UP, RIGHT, DOWN,
		LBTN, RBTN, KEY_COUNT
	};
	enum KEY_STATE {
		KEY_IDLE,		// 0 : No Press 
		KEY_TAPP,		// 1 : Pressed Moment 
		KEY_HOLD,		// 2 : During Press
		KEY_FREE		// 3 : Released Moment 
	};
	struct KeyInfo {
		KEY_STATE state;
		bool pressed; // Pressed in Previous Tick 
	};

	POINT MousePosCurr; // Current Mouse Position
	POINT MousePosPrev; // Previous Mouse Position 

	extern KeyInfo KeyState[KEY_COUNT]; 
	void InputCheck();

}
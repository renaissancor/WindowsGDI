#include "stdafx.h"
#include "binary_search_tree.h"
#include "Window.h"

using namespace std; 

int main() {

	Window& window = Window::GetInstance();
	window.Initialize(); 

	binary_search_tree<int> bst;
	set<int> stl_set; 
	vector<lot_node<int>> snapshot;

	auto update = [&]() -> void {
		snapshot = bst.lot_snapshot(); 
		cout << "STL binary search set : "; 
		for (auto it = stl_set.begin(); it != stl_set.end(); ++it)
			cout << *it << ' '; 
		cout << '\n'; 
		cout << "My binary search tree : ";
		for (auto it = bst.begin(); it != bst.end(); ++it)
			cout << *it << ' ';
		cout << '\n';
	};

	srand(time(NULL)); 
	constexpr static int MOD = 64; 

	while (true) {
		MSG msg;
		if (PeekMessage(&msg, nullptr, 0, 0, PM_REMOVE)) {
			if (msg.message == WM_QUIT) break; 
			if (msg.message == WM_KEYDOWN) {
				int val = rand() % MOD;
				switch (msg.wParam) {
				case 'Q':
				case 'q': 
					bst.insert(val); 
					stl_set.insert(val); 
					update(); 
					break; 
				case 'W':
				case 'w':
					bst.erase(val); 
					stl_set.erase(val); 
					update();
					break; 
				}
			}
			TranslateMessage(&msg);
			DispatchMessage(&msg);
		}
		else {
			window.RenderTree(snapshot);
			window.Present();
			Sleep(15); // Approximate ~60 FPS 
		}
	}

	window.Shutdown();

	return 0;
}
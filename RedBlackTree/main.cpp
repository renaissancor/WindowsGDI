#include "stdafx.h"
#include "rbtree.h"
#include "Window.h"

using namespace std; 

int main() {

	Window& window = Window::GetInstance();
	window.Initialize();

	rbtree<int> rbt_set;
	set<int> stl_set;
	vector<lot_node<int>> snapshot;

	auto check_black_count = [&]() -> void {
		int black_height = rbt_set.is_black_count_same(); 
		if (black_height >= 0) {
			std::cout << "[CHECK] Black Height: " << black_height - 1 << " (All paths consistent)\n";
		}
		else {
			std::cout << "[ALERT] Black Height Mismatch detected!\n";
		}
		}; // end of lambda function check

	auto update = [&]() -> void {
		rbt_set.lot_snapshot(snapshot);

		if (stl_set.size() != rbt_set.size()) {
			cout << "[ERROR] Size mismatch! STL: " << stl_set.size()
				<< ", RBT: " << rbt_set.size() << endl;
			__debugbreak(); 
			return;
		}

		auto stl_it = stl_set.begin();
		auto rbt_it = rbt_set.begin();

		while (stl_it != stl_set.end() && rbt_it != rbt_set.end()) {
			if (*stl_it != *rbt_it) {
				cout << "[ERROR] Value mismatch! STL: " << *stl_it
					<< ", RBT: " << *rbt_it << endl;
				__debugbreak();
				return;
			}
			++stl_it;
			++rbt_it;
		}
		cout << "Verification by Comparison with std::set Success. (Size: " << rbt_set.size() << ")" << endl;
		}; // end of lambda function update 

	srand(static_cast<unsigned int>(time(NULL)));
	constexpr static int MOD = 256;
	constexpr int SCROLL_SPEED = 20;
	while (true) {
		MSG msg;
		if (PeekMessage(&msg, nullptr, 0, 0, PM_REMOVE)) {
			if (msg.message == WM_QUIT) break;
			if (msg.message == WM_KEYDOWN) {
				int val = rand() % MOD;
				switch (msg.wParam) {
				case 'Q':
				case 'q':
					rbt_set.insert(val);
					stl_set.insert(val);
					check_black_count();
					update();
					break;
				case 'W':
				case 'w':
					rbt_set.erase(val);
					stl_set.erase(val);
					check_black_count();
					update();
					break;
				case 'E':
				case 'e':
					for (int i = 0; i < 20; ++i) {
						val = rand() % MOD;
						rbt_set.insert(val);
						stl_set.insert(val);
					}
					check_black_count();
					update();
					break;
				case 'R':
				case 'r':
					for (int i = 0; i < 20; ++i) {
						val = rand() % MOD;
						rbt_set.erase(val);
						stl_set.erase(val);
					}
					check_black_count();
					update();
					break;
				case VK_LEFT:  window.Scroll(-SCROLL_SPEED, 0); break;
				case VK_RIGHT: window.Scroll(SCROLL_SPEED, 0); break;
				case VK_UP:    window.Scroll(0, -SCROLL_SPEED); break;
				case VK_DOWN:  window.Scroll(0, SCROLL_SPEED); break;
				}
			}
			TranslateMessage(&msg);
			DispatchMessage(&msg);
		}
		else {
			window.RenderTree(snapshot);
			window.RenderUI(); 
			window.Present();
			Sleep(15); // Approximate ~60 FPS 
		}
	}

	window.Shutdown();

	/*
	rbt_set.clear(); 
	stl_set.clear(); 

	constexpr static int MOD2 = 100000;
	
	for (int cnt = 0; cnt < 10000; ++cnt) {
		for (int i = 0; i < 10000; ++i) {
			{
				int val = rand() * i % MOD2;
				rbt_set.insert(val);
				stl_set.insert(val);
			}
			{
				int val = rand() * i % MOD2;
				rbt_set.erase(val);
				stl_set.erase(val);
			}
		}
		check_black_count();
		update();
	}
	*/
	

	return 0;
}
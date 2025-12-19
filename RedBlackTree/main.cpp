#include "stdafx.h"
#include "rbtree.h"
#include "Window.h"

using namespace std;

int main() {

	Window& window = Window::GetInstance();
	window.Initialize();

	rbtree<int> rbt;
	set<int> stl_set;
	vector<lot_node<int>> snapshot;

	auto update = [&]() -> void {
		rbt.lot_snapshot(snapshot);

		if (!snapshot.empty()) {
			int total_nodes = static_cast<int>(snapshot.size());
			int tree_width = min(4096, (total_nodes + 1) * 16 * 3);  // RenderTree와 동일한 계산
			int centerX = (tree_width - 1440) / 2;  // WINDOW_WIDTH
			window.SetScroll(centerX, 0);
		}

		cout << "STL binary search set : ";
		for (auto it = stl_set.begin(); it != stl_set.end(); ++it)
			cout << *it << ' ';
		cout << '\n';
		cout << "My binary search tree : ";
		for (auto it = rbt.begin(); it != rbt.end(); ++it)
			cout << *it << ' ';
		cout << '\n';
		};

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
					rbt.insert(val);
					stl_set.insert(val);
					update();
					break;
				case 'W':
				case 'w':
					rbt.erase(val);
					stl_set.erase(val);
					update();
					break;
				case 'E':
				case 'e':
					for (int i = 0; i < 100; ++i) {
						val = rand() % MOD;
						rbt.insert(val);
						stl_set.insert(val);
					}
					update();
					break;
				case 'R':
				case 'r':
					for (int i = 0; i < 100; ++i) {
						val = rand() % MOD;
						rbt.erase(val);
						stl_set.erase(val);
					}
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
			window.Present();
			Sleep(15); // Approximate ~60 FPS 
		}
	}

	window.Shutdown();

	return 0;
}
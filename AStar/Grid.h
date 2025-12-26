#pragma once

// Grid.h 

// namespace Grid later 

namespace Grid {

	constexpr static const int NODE_SIZE = 16;
	constexpr static const int GRID_WIDTH = 80;  // 1280 / 16 = 80
	constexpr static const int GRID_HEIGHT = 48; // 768 / 16 = 48 

	extern bool wall[GRID_WIDTH][GRID_HEIGHT]; 

	void ResetWall() noexcept;

	struct Pos {
		int x = 0;
		int y = 0;
		inline bool operator==(const Pos& other) const noexcept {
			return x == other.x && y == other.y;
		}
	}; 

	struct Node { // 32 Byte in Release Mode 
		double g = 0; // Start to Current Cost 
		double h = 0; // Heuristic Cost 
		double f = 0; // Total Cost 
		Pos parent = { -1, -1 }; 
		Pos pos = { -1, -1 }; 
		// bool isClosed = false;
		// bool isOpened = false;
		Node(const int x, const int y) : pos({x, y}) {}
	};

	class Manager {
	private:
		Manager() = default;
		~Manager() = default;
		Manager(const Manager&) = delete;
		Manager& operator=(const Manager&) = delete;
	private:
		Pos _startPos = { 0, 0 }; 
		Pos _endPos = { 10, 10 }; 

		std::vector<Node*> _opened; 
		
		Node* _nodeMap[GRID_WIDTH][GRID_HEIGHT] = { nullptr }; 

	public:
		inline static Manager& GetInstance() noexcept {
			static Manager instance;
			return instance;
		}
		inline void SetStartPos(const Pos& pos) noexcept { _startPos = pos; }
		inline void SetEndPos(const Pos& pos) noexcept { _endPos = pos; }
		inline const Pos& GetStartPos() const noexcept { return _startPos; }
		inline const Pos& GetEndPos() const noexcept { return _endPos; } 

		inline const double GetManhattan(const Pos& a, const Pos& b) const noexcept {
			return static_cast<double>(abs(a.x - b.x) + abs(a.y - b.y));
		}
		inline const double GetEuclidean(const Pos& a, const Pos& b) const noexcept {
			return sqrt(static_cast<double>((a.x - b.x) * (a.x - b.x) + (a.y - b.y) * (a.y - b.y)));
		}

		void Clear() noexcept; 
		void StepAStar() noexcept; 

	};

} // namespace Grid 

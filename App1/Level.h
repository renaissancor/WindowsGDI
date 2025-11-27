#pragma once

namespace Level {
	struct Info {
		const int wMax; 
		const int hMax; 
		Info(int wMax_, int hMax_) noexcept : wMax(wMax_), hMax(hMax_) {}
	};

	class Manager {
	private:
		Manager() = default;
		~Manager() = default;
		Manager(const Manager&) = delete;
		Manager& operator=(const Manager&) = delete;
	public:
		inline static Manager& GetInstance() noexcept {
			static Manager instance;
			return instance;
		}

	}; 
}
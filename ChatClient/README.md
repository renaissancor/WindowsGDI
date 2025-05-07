## 🧠 Project Summary: Building a Win32 Chat Client with Engine Architecture

### ✅ Goal:
Design a modular **chat client GUI** using **game engine-style architecture** in C++ with Win32 API.

---

## ✅ What We Did (Chronological Steps)

### 1. **Set Up the Main Window (`ChatClient.cpp`)**
- Used `wWinMain` to create a standard Win32 window.
- Registered a custom window class (`"WindowKey"`).
- Called `CreateWindowW(...)` to create the main window.
- Forwarded all window messages via a clean `WndProc`.

---

### 2. **Created an `Engine` Singleton**
- Implemented `Engine` as a `Singleton<Engine>`.
- Centralized ownership of:
  - Window instance (`HWND`)
  - Instance handle (`HINSTANCE`)
  - Resolution
  - Device context (`HDC`)
- Added `Engine::Init(...)` and `Engine::HandleMessage(...)` methods.
- Made `Engine` forward input to subsystems like `InputMgr`.

---

### 3. **Promoted Input Handling into `InputMgr`**
- Created `Input/InputMgr.h/.cpp` as a modular input manager.
- Made `InputMgr` a `Singleton` too.
- Responsibilities:
  - Create GUI controls (`Edit` for chat log & input, `Button` to send)
  - Handle `WM_COMMAND` (button clicks)
  - Append messages to the chat log.

---

### 4. **Avoided Common Win32 Pitfalls**
- Fixed crashes and silent failures by:
  - Ensuring `InputMgr::Init()` is called *after* window is created.
  - Passing `hWnd` from `WndProc` through `Engine` to `InputMgr`.
  - Avoiding use of uninitialized `m_hWnd` inside message handlers.
  - Avoiding broken resource references like `IDI_CHATCLIENT`.
  - Using `ShowWindow(hWnd, nCmdShow)` instead of `true`.

---

### 5. **Refactored Message Routing**
- `WndProc` → calls `Engine::HandleMessage(...)`
- `Engine::HandleMessage(...)` → safely checks init flag, then forwards to `InputMgr`
- `InputMgr::HandleMessage(...)` → handles `WM_COMMAND` and other relevant events

---

## ✅ Current Functional State

- ✅ Win32 window launches correctly
- ✅ InputMgr creates visible UI controls
- ✅ Clicking "Send" updates the chat log
- ✅ Architecture is modular, extensible, and clean

---

## 📂 Current Folder Structure (Simplified)

```
├─App
│      WinMain.cpp
│      WinMain.h
│
├─Common
│      Singleton.h
│
├─Core
│      Engine.cpp
│      Engine.h
│
├─Input
│      InputMgr.cpp
│      InputMgr.h
│
├─pch
│      pch.cpp
│      pch.h
│
└─Resources
        ChatClient.aps
        ChatClient.ico
        ChatClient.rc
        Resource.h
        small.ico
        targetver.h
```

---

## 🧱 Architectural Pattern

- Inspired by game engines:
  - **Core** = central control (`Engine`)
  - **Framework/Managers** = modular subsystems (`InputMgr`)
  - **Singletons** = global ownership & access

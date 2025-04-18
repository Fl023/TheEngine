# TheEngine

This repository contains **TheEngine**, a custom game engine using OpenGL, and an accompanying **Sandbox** project showcasing its features.

---

## Prerequisites

- **Git** (version control)
- **Visual Studio 2022** with the **Desktop development with C++** workload installed

---

## 1. Clone the repository (with submodules)

Open a PowerShell or Command Prompt and run:

```powershell
# Clone main repo and all submodules in one go
git clone --recurse-submodules --remote-submodules https://github.com/FL023/TheEngine
cd TheEngine
```

---

## 2. Generate Visual Studio solution via Premake

A helper script is provided to invoke Premake and produce the `.sln` and `.vcxproj` files for VS2022.

In the repository root, run:

```bat
scripts\Win-GenProjects.bat
```

This will execute `premake5 vs2022` (as specified in `premake5.lua`) and write:

- `TheEngine.sln`
- Project files under `TheEngine/` and `Sandbox/`

---

## 3. Build and run

1. Open **TheEngine.sln** in Visual Studio 2022.
2. Press **F5** to run.

After a successful build, the `Sandbox.exe` will launch and demonstrate the engine’s capabilities.

---

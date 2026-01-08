# 3D-Helicopter-Animation-Simulator
A computer graphics project featuring a fully modelled and animated helicopter built with OpenGL. The project demonstrates hierarchical modelling, transformations, animation control, lighting, and camera systems in a real-time 3D environment.

# 3D Helicopter Animation Simulator

**Author:** Ranwei Zhang  
**Course:** COMP710 – Game Programming  
**Institution:** Auckland University of Technology  
**Project Type:** Individual Project  

---

## Overview
The **3D Helicopter Animation Simulator** is a real-time 3D graphics project developed in C++ using OpenGL as part of the COMP710 Game Programming course.  
The project focuses on hierarchical modelling, real-time animation, and input-driven simulation to represent a controllable helicopter within a dynamic 3D environment.

Key learning objectives include understanding transformation hierarchies, animation control, and real-time rendering pipelines.

---

## Key Features
- Hierarchical helicopter model (body, main rotor, tail rotor)
- Engine on/off state control
- Rotor animation linked to engine state
- Helicopter movement and rotation
- Multiple camera view modes
- Weather system (rain effect)
- Real-time OpenGL rendering

---

## User Manual / Controls

### Movement
- **W** – Move forward  
- **A** – Move left  
- **S** – Move backward  
- **D** – Move right  

### Vertical Movement
- **↑ (Up Arrow)** – Move up  
- **↓ (Down Arrow)** – Move down  

> Note: Vertical movement is only enabled when the engine is turned on.

---

### Rotation
- **← (Left Arrow)** – Turn left  
- **→ (Right Arrow)** – Turn right  

---

### System Controls
- **E** – Turn engine (motor) on / off  
- **L** – Toggle render mode (`RENDER_FILL`)  
- **V** – Change camera view direction  
- **R** – Toggle weather control (rain system)  

---

## Technologies Used
- C++
- OpenGL
- FreeGLUT
- Hierarchical Transformations
- Real-Time Rendering
- Microsoft Visual Studio (Windows)

---

## Project Structure
```text
.
├─ assets/            # Textures and visual resources
├─ *.cpp / *.h        # Source code (rendering, animation, controls)
├─ README.md
├─ .gitignore
└─ *.sln / *.vcxproj  # Visual Studio project files

## Build Instructions
-**Requirements**
Windows
Microsoft Visual Studio (recommended: Visual Studio 2019 or later)
OpenGL-compatible graphics hardware
FreeGLUT installed and configured

---

## FreeGLUT Dependency
This project uses FreeGLUT for window management and input handling.

FreeGLUT is not included in this repository.
Please download and install it from the official website:

http://freeglut.sourceforge.net/

Ensure that the FreeGLUT include and library paths are correctly configured in Visual Studio before building the project.

---

## Steps to Build

-**Clone the repository:**
git clone <repository-url>
-**Open the solution file in Visual Studio.**
-**Configure FreeGLUT include and library directories.**
-**Build the project using:**
Configuration: Debug or Release
Platform: x64
-**Run the simulator from Visual Studio.**

---

## About Excluded Files

This repository contains source code only.
The following files are intentionally excluded:

-**Compiled binaries (.exe)**
-**Build output folders (Debug/, Release/, x64/)**
-**Third-party libraries and SDKs**
-**User-specific Visual Studio files**

---

## Learning Outcomes

Through this project, I gained practical experience in:

-**Hierarchical modelling and transformations**
-**Real-time animation control**
-**Input-driven state management**
-**OpenGL rendering techniques**
-**Managing external dependencies in C++ projects**

---

Author

Ranwei Zhang



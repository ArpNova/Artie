<div align="center">

# 🤖 Desktop Mate (Artie)

*A delightful, interactive desktop companion that lives on your screen.*

<p>
  <a href="https://github.com/ArpNova/Artie/stargazers"><img src="https://img.shields.io/github/stars/ArpNova/Artie?style=for-the-badge&logo=github&color=white&labelColor=white&logoColor=black" alt="Stars"></a>
  <a href="https://github.com/ArpNova/Artie/network/members"><img src="https://img.shields.io/github/forks/ArpNova/Artie?style=for-the-badge&logo=github&color=white&labelColor=white&logoColor=black" alt="Forks"></a>
  <a href="https://github.com/ArpNova/Artie/watchers"><img src="https://img.shields.io/github/watchers/ArpNova/Artie?style=for-the-badge&logo=github&color=white&labelColor=white&logoColor=black" alt="Watchers"></a>
</p>

<p>
  <a href="https://opensource.org/licenses/MIT"><img src="https://img.shields.io/badge/License-MIT-a4d037?style=for-the-badge" alt="License"></a>
  <a href="https://isocpp.org/"><img src="https://img.shields.io/badge/C++-00599C?style=for-the-badge&logo=c%2B%2B&logoColor=white" alt="C++"></a>
  <a href="https://www.qt.io/"><img src="https://img.shields.io/badge/Qt_6-41CD52?style=for-the-badge&logo=qt&logoColor=white" alt="Qt 6"></a>
  <a href="https://cmake.org/"><img src="https://img.shields.io/badge/CMake-064F8C?style=for-the-badge&logo=cmake&logoColor=white" alt="CMake"></a>
</p>

<p>
  <a href="https://github.com/ArpNova/Artie/issues"><img src="https://img.shields.io/github/issues/ArpNova/Artie?style=for-the-badge&color=dfb317" alt="Issues"></a>
  <a href="https://github.com/ArpNova/Artie/pulls"><img src="https://img.shields.io/github/issues-pr/ArpNova/Artie?style=for-the-badge&color=28a745" alt="Pull Requests"></a>
  <a href="https://github.com/ArpNova/Artie/commits"><img src="https://img.shields.io/github/last-commit/ArpNova/Artie?style=for-the-badge&color=a4d037" alt="Last Commit"></a>
</p>

</div>

---

## Project Demo
![Project Walkthrough](assets/demo.gif)

## Features

Artie is more than just a static image; he's a lively character with his own behaviors:

*   **Physics-Based Movement**: Artie obeys gravity, falls when dropped, and walks along the bottom of your screen.
*   **Autonomous Behavior**: He decides when to walk, change direction, or stand still and observe.
*   **Interactive**:
    *   **Drag & Drop**: Pick him up with your mouse (Left Click) and toss him around.
    *   **Customization**: Right-click to access a context menu where you can change his size (Small, Normal, Large) or arm style (Thin, Thick, Red).
    *   **Gestures**: Press `W` to make him wave at you!
*   **Always on Top**: He stays visible over your other windows, keeping you company while you work.

## System Requirements

**Note: This application is designed specifically for X11 environments.**

Artie relies on advanced window management features (absolute positioning, "always on top" behavior, and transparent overlays) that are standard in X11 but may be restricted or behave differently on Wayland compositors due to security protocols.

*   **Recommended**: X11 Display Server


## Prerequisites

To build Artie, you will need the following installed:

*   **C++ Compiler** (supporting C++17)
*   **CMake** (Version 3.16 or higher)
*   **Qt 6** (specifically the `Widgets` component)

## Installation & Build

1.  **Clone the repository:**
    ```bash
    git clone https://github.com/ArpNova/Artie.git
    cd Artie
    ```

2.  **Create a build directory:**
    ```bash
    mkdir build
    cd build
    ```

3.  **Configure with CMake:**
    ```bash
    cmake ..
    ```

4.  **Build the project:**
    ```bash
    make
    ```

## Usage

After building, run the executable from the `build` directory:

```bash
./artie
```

### Controls

| Action | Input |
| :--- | :--- |
| **Move Artie** | Left-click and drag with mouse |
| **Open Menu** | Right-click on Artie |
| **Wave** | Press `W` key |
| **Exit** | Close the terminal or kill the process (Context menu exit coming soon) |

## Project Structure

*   `src/`: Source files (`main.cpp`, `artie.cpp`)
*   `include/`: Header files (`artie.h`)
*   `assets/`: Image resources for the character body parts.

## Contributing

Feel free to fork the project and submit pull requests for new animations, outfits, or smarter AI behaviors!

## License

This project is licensed under the MIT License - see the [LICENSE](LICENSE) file for details.

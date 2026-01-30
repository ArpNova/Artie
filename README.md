# Desktop Mate (Artie)

A delightful, interactive desktop companion that lives on your screen. Built with C++ and Qt 6.

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

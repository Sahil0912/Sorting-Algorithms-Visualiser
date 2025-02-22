# Sorting Algorithm Visualiser

Sorting Algorithm Visualiser is a C++ project that demonstrates real-time visualization of various sorting algorithms using SDL2 and SDL2_ttf. It provides an interactive SDL window where users can select a sorting algorithm and watch it sort an array step-by-step.

## Features

- **Multiple Sorting Algorithms:**  
  Visualizes Bubble Sort, Selection Sort, Insertion Sort and Quick Sort (Though you can add any other sorting algo accordingly).

- **Integrated Menu:**  
  A menu is rendered directly in the SDL window using SDL_ttf for selecting the desired sorting algorithm. Users can choose an algorithm by clicking an option or pressing keys 1-4.

- **Real-Time Visualization:**  
  The sorting process is animated with color-coded highlights:
  - **Red:** Primary element being considered.
  - **Blue:** Secondary element or comparison highlight.
  - **White:** Unaffected elements.

- **Automatic Exit:**  
  After the sorting is completed, the program waits for 2 seconds or until the user closes the window before automatically exiting.

- **Robust Error Handling:**  
  SDL initialization, window and renderer creation, and font loading are all error-checked to ensure a stable and responsive application.

## Requirements

- **C++11** or later.
- **SDL2 Library**
- **SDL2_ttf Library**
- **make**

### Installing Dependencies (Linux Example)

For Ubuntu or Debian-based systems, install the necessary packages with:

```bash
sudo apt-get update
sudo apt-get install libsdl2-dev libsdl2-ttf-dev make
```

For arch based systems:

```bash
sudo pacman -S sdl2-compat
sudo pacman -S sdl2_ttf
sudo pacman -S make
```
or if using aur/yay :
```bash
yay -S sdl2 sdl2_ttf make
```
## Compilation and run

```bash
make
```
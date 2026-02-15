# 🧩 Maze Solver & Path Visualizer

An interactive maze generator and shortest-path visualizer built using
**C and Python**.\
This project demonstrates core concepts of recursion, graph traversal,
and shortest-path algorithms through real-time animation.

------------------------------------------------------------------------

## 🚀 Features

-   Random maze generation using Recursive Backtracking
-   Solvability check using Breadth-First Search (BFS)
-   Shortest path computation using Dijkstra's Algorithm
-   Difficulty levels (Easy / Medium / Hard)
-   Animated visualization using Pygame
-   Custom maze size input
-   Cross-platform support (Windows / Linux)

------------------------------------------------------------------------

## 🛠 Technologies Used

-   **C** -- Maze generation & pathfinding logic\
-   **Python** -- Visualization\
-   **Pygame** -- Graphics rendering\
-   **GCC Compiler**

------------------------------------------------------------------------

## 📂 Project Structure

maze_solver.c → Maze generation & Dijkstra logic\
visualizer.py → Python animation\
maze.txt → Generated maze grid\
path.txt → Shortest path coordinates

------------------------------------------------------------------------

## ▶️ How to Run

### 1️⃣ Compile C Program

gcc maze_solver.c -o maze_solver

### 2️⃣ Run Maze Generator

./maze_solver (Windows: maze_solver.exe)

### 3️⃣ Visualization

The Python visualizer launches automatically\
OR run manually: python visualizer.py

------------------------------------------------------------------------

## 📖 How It Works

1.  Maze is generated using Recursive Backtracking.
2.  BFS checks if the maze is solvable.
3.  Dijkstra's Algorithm finds the shortest path.
4.  Maze and path are saved to text files.
5.  Pygame animates the solution visually.

------------------------------------------------------------------------

## 🔮 Future Improvements

-   Add A\* algorithm
-   Interactive maze editing
-   Performance comparison between algorithms
-   3D maze visualization

------------------------------------------------------------------------

## 👩‍💻 Author

**Bodgam Rishika Reddy**\
CSE-A

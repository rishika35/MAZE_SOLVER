import pygame
import sys
import time

def read_maze(filename):
    maze = []
    with open(filename, "r") as f:
        for line in f:
            row = [int(x) for x in line.split()]
            if row:
                maze.append(row)
    return maze

def read_path(filename):
    path = []
    with open(filename, "r") as f:
        for line in f:
            try:
                r, c = map(int, line.split())
                path.append((r, c))
            except:
                pass
    return path

def draw_maze(screen, maze, cell_size):
    rows, cols = len(maze), len(maze[0])
    for r in range(rows):
        for c in range(cols):
            color = (0, 0, 0) if maze[r][c] == 1 else (255, 255, 255)
            pygame.draw.rect(screen, color, (c * cell_size, r * cell_size, cell_size, cell_size))

def visualize(maze, path):
    rows, cols = len(maze), len(maze[0])
    screen_width, screen_height = 1000, 800
    cell_size = max(3, min(screen_width // cols, screen_height // rows))

    if rows < 25 and cols < 25:
        flags = pygame.FULLSCREEN
        screen_width, screen_height = cols * cell_size, rows * cell_size
    else:
        flags = 0

    pygame.init()
    screen = pygame.display.set_mode((screen_width, screen_height), flags)
    pygame.display.set_caption("Maze Solver — Dijkstra Path Visualization")
    clock = pygame.time.Clock()

    draw_maze(screen, maze, cell_size)
    pygame.display.flip()
    time.sleep(0.5)

    pygame.draw.rect(screen, (0, 255, 0), (path[0][1] * cell_size, path[0][0] * cell_size, cell_size, cell_size))
    pygame.draw.rect(screen, (255, 0, 0), (path[-1][1] * cell_size, path[-1][0] * cell_size, cell_size, cell_size))
    pygame.display.flip()
    time.sleep(0.5)

    for r, c in path:
        pygame.draw.rect(screen, (255, 255, 0), (c * cell_size, r * cell_size, cell_size, cell_size))
        pygame.display.flip()
        clock.tick(120)

    pygame.draw.rect(screen, (0, 255, 0), (path[0][1] * cell_size, path[0][0] * cell_size, cell_size, cell_size))
    pygame.draw.rect(screen, (255, 0, 0), (path[-1][1] * cell_size, path[-1][0] * cell_size, cell_size, cell_size))
    pygame.display.flip()

    font = pygame.font.SysFont("Consolas", 26)
    msg = font.render("Press ESC to exit", True, (180, 180, 255))
    screen.blit(msg, (10, 10))
    pygame.display.flip()

    running = True
    while running:
        for event in pygame.event.get():
            if event.type == pygame.QUIT:
                running = False
            elif event.type == pygame.KEYDOWN and event.key == pygame.K_ESCAPE:
                running = False
        time.sleep(0.05)

    pygame.quit()

if __name__ == "__main__":
    try:
        maze = read_maze("maze.txt")
        path = read_path("path.txt")

        if not maze:
            print("maze.txt is empty or missing.")
            sys.exit(1)
        if not path:
            print("path.txt is empty or invalid.")
            sys.exit(1)

        visualize(maze, path)
    except Exception as e:
        print("Error while visualizing:", e)

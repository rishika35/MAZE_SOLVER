#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <stdint.h>
#include <limits.h>

#ifdef _WIN32
#include <windows.h>
uint64_t get_seed() {
    LARGE_INTEGER counter;
    QueryPerformanceCounter(&counter);
    return (uint64_t)(counter.QuadPart ^ time(NULL));
}
#else
#include <sys/time.h>
uint64_t get_seed() {
    struct timeval tv;
    gettimeofday(&tv, NULL);
    return ((uint64_t)tv.tv_sec << 32) ^ tv.tv_usec;
}
#endif

#define WALL 1
#define PATH 0
#define INF INT_MAX
#define MAX 105

int maze[MAX][MAX];
int path[MAX*MAX][2];
int path_len = 0;
int ROWS, COLS;

typedef struct { int r, c, dist; } Node;
typedef struct { Node *nodes; int size, cap; } MinHeap;

void swap(Node *a, Node *b) { Node t = *a; *a = *b; *b = t; }

void heapify_up(MinHeap *h, int i) {
    while (i && h->nodes[i].dist < h->nodes[(i-1)/2].dist) {
        swap(&h->nodes[i], &h->nodes[(i-1)/2]);
        i = (i - 1) / 2;
    }
}

void heapify_down(MinHeap *h, int i) {
    int smallest = i;
    int l = 2*i + 1, r = 2*i + 2;
    if (l < h->size && h->nodes[l].dist < h->nodes[smallest].dist)
        smallest = l;
    if (r < h->size && h->nodes[r].dist < h->nodes[smallest].dist)
        smallest = r;
    if (smallest != i) {
        swap(&h->nodes[i], &h->nodes[smallest]);
        heapify_down(h, smallest);
    }
}

void push(MinHeap *h, Node n) {
    h->nodes[h->size++] = n;
    heapify_up(h, h->size - 1);
}

Node pop(MinHeap *h) {
    Node top = h->nodes[0];
    h->nodes[0] = h->nodes[--h->size];
    heapify_down(h, 0);
    return top;
}

MinHeap* create_heap(int cap) {
    MinHeap *h = malloc(sizeof(MinHeap));
    h->nodes = malloc(sizeof(Node) * cap);
    h->size = 0;
    h->cap = cap;
    return h;
}

void init_maze() {
    for (int r = 0; r < ROWS; r++)
        for (int c = 0; c < COLS; c++)
            maze[r][c] = WALL;
}

void shuffle(int *arr, int n) {
    for (int i = n-1; i > 0; i--) {
        int j = rand() % (i+1);
        int t = arr[i]; arr[i] = arr[j]; arr[j] = t;
    }
}

void carve(int r, int c) {
    int dr[4] = {0, 0, 2, -2};
    int dc[4] = {2, -2, 0, 0};
    int dirs[4] = {0,1,2,3};
    shuffle(dirs, 4);
    maze[r][c] = PATH;

    for (int i = 0; i < 4; i++) {
        int nr = r + dr[dirs[i]];
        int nc = c + dc[dirs[i]];
        if (nr > 0 && nr < ROWS-1 && nc > 0 && nc < COLS-1 && maze[nr][nc] == WALL) {
            maze[r + dr[dirs[i]]/2][c + dc[dirs[i]]/2] = PATH;
            carve(nr, nc);
        }
    }
}

int is_solvable(int sr, int sc, int er, int ec) {
    int visited[MAX][MAX] = {0};
    int q[MAX*MAX][2];
    int front = 0, rear = 0;

    q[rear][0] = sr;
    q[rear++][1] = sc;
    visited[sr][sc] = 1;

    int dr[4] = {1,-1,0,0};
    int dc[4] = {0,0,1,-1};

    while (front < rear) {
        int r = q[front][0];
        int c = q[front++][1];

        if (r == er && c == ec) return 1;

        for (int i = 0; i < 4; i++) {
            int nr = r + dr[i];
            int nc = c + dc[i];

            if (nr>=0 && nr<ROWS && nc>=0 && nc<COLS &&
                !visited[nr][nc] && maze[nr][nc] == PATH) {
                visited[nr][nc] = 1;
                q[rear][0] = nr;
                q[rear++][1] = nc;
            }
        }
    }
    return 0;
}

void add_extra_walls(int difficulty, int sr, int sc, int er, int ec) {
    int extraWalls;
    if (difficulty == 1) extraWalls = (ROWS * COLS) / 25;
    else if (difficulty == 2) extraWalls = (ROWS * COLS) / 18;
    else extraWalls = (ROWS * COLS) / 12;

    for (int i = 0; i < extraWalls; i++) {
        int r = rand() % (ROWS-2) + 1;
        int c = rand() % (COLS-2) + 1;

        if ((r == sr && c == sc) || (r == er && c == ec))
            continue;

        int backup = maze[r][c];
        maze[r][c] = WALL;

        if (!is_solvable(sr, sc, er, ec))
            maze[r][c] = backup;
    }
}

void dijkstra(int sr, int sc, int er, int ec) {
    int dist[MAX][MAX], pr[MAX][MAX], pc[MAX][MAX], vis[MAX][MAX]={0};

    for (int i=0;i<ROWS;i++)
        for (int j=0;j<COLS;j++)
            dist[i][j]=INF, pr[i][j]=pc[i][j]=-1;

    MinHeap *h = create_heap(ROWS*COLS);
    dist[sr][sc] = 0;
    push(h, (Node){sr, sc, 0});

    int dr[4]={1,-1,0,0}, dc[4]={0,0,1,-1};

    while (h->size) {
        Node cur = pop(h);
        int r = cur.r, c = cur.c;

        if (vis[r][c]) continue;
        vis[r][c] = 1;

        if (r==er && c==ec) break;

        for (int i=0;i<4;i++) {
            int nr=r+dr[i], nc=c+dc[i];
            if (nr<0||nc<0||nr>=ROWS||nc>=COLS) continue;
            if (maze[nr][nc] == WALL) continue;

            if (dist[r][c] + 1 < dist[nr][nc]) {
                dist[nr][nc] = dist[r][c] + 1;
                pr[nr][nc] = r; pc[nr][nc] = c;
                push(h,(Node){nr,nc,dist[nr][nc]});
            }
        }
    }

    if (dist[er][ec] == INF) { path_len=0; return; }

    path_len = 0;
    int r=er, c=ec;

    while (r!=-1 && c!=-1) {
        path[path_len][0]=r;
        path[path_len][1]=c;
        path_len++;
        int tr=pr[r][c], tc=pc[r][c];
        r=tr; c=tc;
    }

    for (int i=0;i<path_len/2;i++) {
        int t0=path[i][0], t1=path[i][1];
        path[i][0]=path[path_len-1-i][0];
        path[i][1]=path[path_len-1-i][1];
        path[path_len-1-i][0]=t0;
        path[path_len-1-i][1]=t1;
    }
}

void save_maze(const char *fname) {
    FILE *f=fopen(fname,"w");
    for (int r=0;r<ROWS;r++){
        for (int c=0;c<COLS;c++)
            fprintf(f,"%d ",maze[r][c]);
        fprintf(f,"\n");
    }
    fclose(f);
}

void save_path(const char *fname) {
    FILE *f=fopen(fname,"w");
    for (int i=0;i<path_len;i++)
        fprintf(f,"%d %d\n",path[i][0],path[i][1]);
    fclose(f);
}

int main() {
    srand(get_seed());

    printf("Enter maze size (rows cols): ");
    scanf("%d %d", &ROWS, &COLS);

    if (ROWS % 2 == 0) ROWS++;
    if (COLS % 2 == 0) COLS++;

    int difficulty;
    printf("Select difficulty:\n1. Easy\n2. Medium\n3. Hard\nChoice: ");
    scanf("%d",&difficulty);

    init_maze();
    carve(1,1);

    int sr=1, sc=1;
    int er=ROWS-2, ec=COLS-2;

    add_extra_walls(difficulty, sr, sc, er, ec);

    if (!is_solvable(sr, sc, er, ec)) {
        printf("Maze has no valid path.\n");
        return 0;
    }

    dijkstra(sr, sc, er, ec);

    save_maze("maze.txt");
    save_path("path.txt");

    printf("Maze and path saved. Launching visualizer...\n");

    system("python visualizer.py");

    return 0;
}

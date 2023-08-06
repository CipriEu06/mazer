#include<iostream>
#include<fstream>
#include<SDL.h>
#include<queue>
#define MAX_WIDTH 64
#define MAX_HEIGHT 64
using namespace std;
int maze[MAX_HEIGHT+2][MAX_WIDTH+2], sol[MAX_HEIGHT+2][MAX_WIDTH+2], N, M, scale, sx, sy, fx, fy;
int SCREEN_WIDTH = 640;
int SCREEN_HEIGHT = 640;

queue<pair<int, int> > Q, P;

void read(){
    ifstream fin("maze.txt");
    char s;
    fin>>scale;
    fin>>N>>M;
    SCREEN_HEIGHT = (N+2)*scale;
    SCREEN_WIDTH = (M+2)*scale;
    for(int i=0; i<=N+1; i++)
        for(int j=0; j<=M+1; j++){
            if(i==0 || j==0 || i==N+1 || j==M+1) maze[i][j] = -1;
            else{ fin>>s; if(s == '-') maze[i][j] = -1;}
            sol[i][j] = maze[i][j];
        }
    fin>>sx>>sy>>fx>>fy;
    maze[sx][sy] = 1;
    Q.push({sx, sy});
    fin.close();
}

void bfs(){
    int dx[] = {0, 1, 0, -1};
    int dy[] = {1, 0, -1, 0};
    int x = Q.front().first, y = Q.front().second;
    for(int k=0; k<4; k++){
        int xx = x+dx[k];
        int yy = y+dy[k];
        if(maze[xx][yy] == 0){
            maze[xx][yy] = maze[x][y]+1;
            Q.push({xx, yy});
        }
    }
    Q.pop();
}

void bkt(int &x, int &y){
    int dx[] = {0, 1, 0, -1};
    int dy[] = {1, 0, -1, 0};
    for(int k=0; k<4; k++){
        int xx = x+dx[k];
        int yy = y+dy[k];
        if(maze[xx][yy] == maze[x][y] - 1 && maze[xx][yy] >= 0){
            x = xx; y = yy; break;
        }
    }
}

int main(int argc, char* args[]){
    read();
    SDL_Window *window = NULL;
    SDL_Surface *screenSurface = NULL;
    SDL_Rect sq[MAX_HEIGHT+2][MAX_WIDTH+2];
    for(int i=0; i<=N+1; i++){
        for(int j=0; j<=M+1; j++){
            sq[i][j].h = scale;
            sq[i][j].w = scale;
            sq[i][j].x = j*scale;
            sq[i][j].y = i*scale;
        }
    }
    if(SDL_Init(SDL_INIT_VIDEO)<0){
        cout<<"eroare init!\n"<<SDL_GetError()<<"\n";
    }
    else{
        window = SDL_CreateWindow("gemcu's mazer", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, SCREEN_WIDTH, SCREEN_HEIGHT, SDL_WINDOW_SHOWN);
        if(window==NULL){
            cout<<"eroare bindow!\n"<<SDL_GetError()<<"\n";
        }
        else{
            screenSurface = SDL_GetWindowSurface(window);
            SDL_FillRect(screenSurface, NULL, SDL_MapRGB(screenSurface->format, 0xff, 0xff, 0xff));
            SDL_UpdateWindowSurface(window);
            SDL_Event e; bool quit = false; int x = -1, y = -1, dx=-1, dy=-1;

            for(int i=0; i<=N+1; i++){
                for(int j=0; j<=M+1; j++){
                    int r=0xff, g=0xff, b=0xff;
                    if(maze[i][j] == -1) r = g = b = 0x44;
                    if(SDL_FillRect(screenSurface, &sq[i][j], SDL_MapRGB(screenSurface->format, r, g, b)) < 0){
                        cout<<"ERROR: "<<SDL_GetError()<<"\n";
                    }
                }
            }
            SDL_UpdateWindowSurface(window);

            while(!Q.empty()){
                x = Q.front().first;
                y = Q.front().second;
                bfs();
                for(int i=0; i<=N+1; i++){
                    for(int j=0; j<=M+1; j++){
                        int r=0xff, g, b=0xff;
                        if(maze[i][j] == -1) r = g = b = 0x44;
                        else{
                            r = max(0, 0xff - maze[i][j]*4);
                            g = max(0, 0xff - maze[i][j]*2);
                            b = max(0, 0xff - maze[i][j]*2);
                        }
                        SDL_FillRect(screenSurface, &sq[i][j], SDL_MapRGB(screenSurface->format, r, g, b));
                    }
                }
                SDL_FillRect(screenSurface, &sq[x][y], SDL_MapRGB(screenSurface->format, 0xee, 0x00, 0xff));
                SDL_UpdateWindowSurface(window);
            }

            if(maze[fx][fy]) {dx = fx; dy = fy;}
            while(dx != -1 && dy != -1){
                sol[dx][dy] = 1;
                int r, g, b;
                r = max(0, 0xff - maze[dx][dy]*2);
                g = max(0, 0xff - maze[dx][dy]);
                b = max(0, 0xff - maze[dx][dy]*2);
                SDL_FillRect(screenSurface, &sq[dx][dy], SDL_MapRGB(screenSurface->format, r, g, b));
                if(dx == sx && dy == sy){dx = -1; dy = -1;}
                else bkt(dx, dy);
                SDL_UpdateWindowSurface(window);
            }

            while(!quit){
                while(SDL_PollEvent(&e)){
                    if(e.type == SDL_QUIT){
                        quit = true;
                    }
                }
            }
        }

        ofstream fout("sol.txt");
        for(int i=1; i<=N; i++){
            for(int j=1; j<=M; j++){
                if(sol[i][j] == -1) fout<<"- ";
                if(sol[i][j] == 0) fout<<"o ";
                if(sol[i][j] == 1) fout<<"* ";
            }
            fout<<"\n";
        }
        fout.close();

        SDL_DestroyWindow(window);
        SDL_Quit();
    }
}

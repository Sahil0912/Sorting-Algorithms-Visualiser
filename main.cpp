#include <iostream>
#include <random>
#include <algorithm>
#include <SDL2/SDL.h>
#include <SDL2/SDL_ttf.h> 
#include <time.h>
using namespace std;
typedef vector<int> vi;

void process_Events(bool &quit) {
    SDL_Event event;
    while(SDL_PollEvent(&event))
        if(event.type == SDL_QUIT)
            quit = true;
}


void Visualize(vi &v, int i, int j, SDL_Renderer* renderer){
    for(int idx = 0; idx < (int)v.size(); idx++){
        if(idx == i) SDL_SetRenderDrawColor(renderer, 255, 0, 0, 255);
        else if(idx == j) SDL_SetRenderDrawColor(renderer, 0, 0, 255, 255);
        else SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);
        SDL_RenderDrawLine(renderer, idx, (int)v.size() - 1, idx, v[idx]);
    }
}

void Render(SDL_Renderer* renderer, vi &v, int i, int j, bool &quit) {
    SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
    SDL_RenderClear(renderer);
    Visualize(v, i, j, renderer);
    SDL_RenderPresent(renderer);
    SDL_Delay(10);
    process_Events(quit);
}

void bubble_Sort(vi &v, SDL_Renderer* renderer, bool &quit){
    for(int i = 0; i < (int)v.size() && !quit; i++){
        for(int j = i + 1; j < (int)v.size() && !quit; j++){
            if(v[i] < v[j]) swap(v[i], v[j]);    
            Render(renderer, v, i, j, quit);
        }
    }
}
void selection_Sort(vi &v, SDL_Renderer* renderer, bool &quit) {
    for(int i = 0; i < (int)v.size() && !quit; i++){
        int idx = i;
        for(int j = i + 1; j < (int)v.size() && !quit; j++){
            if(v[idx] < v[j]) idx = j;
            Render(renderer, v, i, j, quit);
        }
        if(idx != i) swap(v[i], v[idx]);
        Render(renderer, v, i, idx, quit);
    }
}

void insertion_Sort(vi &v, SDL_Renderer* renderer, bool &quit) {
    for(int i = 1; i < (int)v.size() && !quit; i++){
        int curr = v[i];
        int j = i - 1;
        while(j >= 0 && v[j] < curr && !quit){
            v[j+1] = v[j];
            j--;
            Render(renderer, v, j, i, quit);
        }
        v[j+1] = curr;
        Render(renderer, v, j+1, i, quit);
    }
}
int partition(vi &v, int lo, int hi, SDL_Renderer* renderer, bool &quit) {
    int pivot = v[hi], i = lo - 1;
    for(int j = lo; j < hi && !quit; j++){
        if(v[j] > pivot){
            i++;
            swap(v[i], v[j]);
            Render(renderer, v, i, j, quit);
        }
    }
    swap(v[i + 1], v[hi]);
    Render(renderer, v, i + 1, hi, quit);
    return i + 1;
}

void rec(vi &v, int lo, int hi, SDL_Renderer* renderer, bool &quit) {
    if(lo < hi && !quit){
        int p = partition(v, lo, hi, renderer, quit);
        rec(v, lo, p - 1, renderer, quit);
        rec(v, p + 1, hi, renderer, quit);
    }
}

void quick_Sort(vi &v, SDL_Renderer* renderer, bool &quit) {
    rec(v, 0, v.size() - 1, renderer, quit);
}


SDL_Texture* render_Text(SDL_Renderer* renderer, TTF_Font* font, const string &message, SDL_Color color, SDL_Rect &textRect) {
    SDL_Surface* surface = TTF_RenderText_Solid(font, message.c_str(), color);
    if(!surface){
        cerr << TTF_GetError() << endl;
        return nullptr;
    }
    SDL_Texture* texture = SDL_CreateTextureFromSurface(renderer, surface);
    if(!texture){
        cerr << SDL_GetError() << endl;
    }
    textRect.w = surface->w;
    textRect.h = surface->h;
    SDL_FreeSurface(surface);
    return texture;
}

int display_Menu(SDL_Renderer* renderer, TTF_Font* font, int windowWidth, int windowHeight) {
    vector<string> options = {
        "1. Bubble Sort",
        "2. Selection Sort",
        "3. Insertion Sort",
        "4. Quick Sort"
    };
    vector<SDL_Texture*> textures;
    vector<SDL_Rect> rects;
    SDL_Color white = {255, 255, 255, 255};
    int verticalSpacing = 10;
    int totalHeight = 0;
    
    for(const auto &option : options){
        SDL_Rect textRect = {0, 0, 0, 0};
        SDL_Texture* texture = render_Text(renderer, font, option, white, textRect);
        if (!texture) continue;
        textures.push_back(texture);
        rects.push_back(textRect);
        totalHeight += textRect.h + verticalSpacing;
    }
    int startY = (windowHeight - totalHeight)/2;
    for (auto &rect : rects) {
        rect.x = (windowWidth - rect.w) / 2;
        rect.y = startY;
        startY += rect.h + verticalSpacing;
    }
    
    int selectedOption{0};
    bool quit{false};
    SDL_Event event;
    
    while(!quit && !selectedOption){
        while(SDL_PollEvent(&event)) {
            if(event.type == SDL_QUIT){
                for(auto tex : textures) SDL_DestroyTexture(tex);
                exit(0);
            } 
            else if(event.type == SDL_KEYDOWN){
                if(event.key.keysym.sym >= SDLK_1 && event.key.keysym.sym <= SDLK_9) selectedOption = event.key.keysym.sym - SDLK_0;
            }
            else if(event.type == SDL_MOUSEBUTTONDOWN){
                int x = event.button.x, y = event.button.y;
                for(int i = 0; i < rects.size(); i++){
                    if(x >= rects[i].x && x <= rects[i].x + rects[i].w && y >= rects[i].y && y <= rects[i].y + rects[i].h){
                        selectedOption = i + 1;
                        break;
                    }
                }
            }
        }
        
        SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
        SDL_RenderClear(renderer);
        for(int i = 0; i < textures.size(); i++){
            SDL_RenderCopy(renderer, textures[i], NULL, &rects[i]);
        }
        SDL_RenderPresent(renderer);
        SDL_Delay(16);
    }
    
    for(auto tex : textures) SDL_DestroyTexture(tex);
    
    return selectedOption;
}



int main(int argc, char const *argv[])
{
    srand(time(NULL));
    random_device rd;
    uniform_int_distribution dis(1, 100);
    vi v;
    int n = 100;
    v.resize(n);
    for(auto &x : v) x = dis(rd);
    if(SDL_Init(SDL_INIT_VIDEO) != 0){
        cerr << SDL_GetError() << endl;
        return 1;
    }
    if(TTF_Init() != 0){
        cerr << TTF_GetError() << endl;
        SDL_Quit();
        return 1;
    }
    SDL_Window* window = SDL_CreateWindow("Visualization", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, 800, 600, SDL_WINDOW_SHOWN);
    if(!window){
        cerr << SDL_GetError() << endl;
        TTF_Quit();
        SDL_Quit();
        return 1;
    }
    SDL_Renderer* renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);
    if(!renderer){
        cerr << SDL_GetError() << endl;
        SDL_DestroyWindow(window);
        TTF_Quit();
        SDL_Quit();
        return 1;
    }
    TTF_Font* font = TTF_OpenFont("Arial.ttf", 24);
    if(!font) {
        cerr << TTF_GetError() << endl;
        SDL_DestroyRenderer(renderer);
        SDL_DestroyWindow(window);
        TTF_Quit();
        SDL_Quit();
        return 1;
    }

    for(auto &x : v) cout << x << " ";
    cout << endl;

    int choice = display_Menu(renderer, font, 800, 600);
    float X = static_cast<float>(800) / n;
    float Y = static_cast<float>(600) / n;
    SDL_RenderSetScale(renderer, X, Y);
    
    bool quit(false);
    switch(choice){
        case 1:
            bubble_Sort(v, renderer, quit);
            break;
        case 2:
            selection_Sort(v, renderer, quit);
            break;
        case 3:
            insertion_Sort(v, renderer, quit);
            break;
        case 4:
            quick_Sort(v, renderer, quit);
            break;
        default:
            cout << "Invalid selection" << endl;
            break;
    }
    unsigned start = SDL_GetTicks();
    bool quit2 = false;
    while(!quit2){
        SDL_Event event;
        while(SDL_PollEvent(&event)){
            if (event.type == SDL_QUIT) {
                quit2 = true;
            }
        }
        if (SDL_GetTicks() - start >= 2000) {
            quit2 = true;
        }
        SDL_Delay(10);
    }

    for(auto &x : v) cout << x << " ";
    cout << endl;

    TTF_CloseFont(font);
    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    TTF_Quit();
    SDL_Quit();
    return 0;
}

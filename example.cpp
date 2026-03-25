import std;//clang++-20 -std=c++23 -stdlib=libc++ --precompile -o std.pcm /usr/lib/llvm-20/share/libc++/v1/std.cppm
#include <SDL2/SDL.h>
//clang++-20 -std=c++23 -stdlib=libc++ -fmodule-file=std=std.pcm main.cpp -o app -lSDL2
static constexpr std::string_view SUIT_NAMES[] = {"Hearts", "Diamonds", "Clubs", "Spades"};//0 1 2 3
static constexpr std::string_view COLOR_NAMES[] = {"Red", "Black"};//0 1
static constexpr std::string_view FACE_NAMES[] = {"Ace","Two","Three","Four","Five","Six","Seven","Eight","Nine","Ten","Jack","Queen","King"};//1 2 3 .. 14
static constexpr std::string_view STATE_NAMES[] = {"Back","Face"};//0 1
struct Card{
    std::int8_t suit,color,face,state;
};

struct Deck{
    std::vector<Card> deck;
};
struct Hole:public Deck{

};
struct Home:public Deck{

};
void shuffle(std::vector<Card> &v){
    std::random_device rd;
    std::mt19937 g(rd());

    std::shuffle(v.begin(), v.end(), g);
}
void build_deck(std::vector<Card> &deck){
    auto push = [&](std::int8_t a,std::int8_t b,std::int8_t c) {
        for(std::int8_t i=1;i<14;i++){
            Card temp =Card{.suit=a, .color=b, .face=i, .state=c};
            deck.push_back(temp);
        }
    };
    for(int j=0;j<4;j++){
        switch (j) {
            case 0:push(0,0,0);break;
            case 1:push(1,0,0);break;
            case 2:push(2,1,0);break;
            case 3:push(3,1,0);break;
        }
    }
}

void print(const std::vector<Card> &deck){
    for (const auto &a : deck) {
        //std::println("Suit: {}, Color: {}, Face: {}, State: {}",a.suit,a.color,a.face,a.state);
        std::println("{} of {} (Color: {}, State: {})",
            FACE_NAMES[a.face - 1], // -1, так как цикл от 1 до 13
            SUIT_NAMES[a.suit],
            COLOR_NAMES[a.color],
            STATE_NAMES[a.state]
        );
    }
}

struct StateApp{
    SDL_Window* window=0;
    SDL_Renderer* render=0;
    bool run=false;
    ~StateApp(){
        SDL_DestroyWindow(window);
        SDL_DestroyRenderer(render);
    }
};

void initApp(StateApp &app){
    SDL_Init(SDL_INIT_VIDEO);
    app.window=SDL_CreateWindow("Test", SDL_WINDOWPOS_CENTERED,SDL_WINDOWPOS_CENTERED, 1290, 720, SDL_WINDOW_ALLOW_HIGHDPI|SDL_WINDOW_RESIZABLE);
    app.render=SDL_CreateRenderer(app.window, -1, SDL_RENDERER_ACCELERATED);
    app.run=true;
}

void closeApp(){
    SDL_Quit();
}
int main() {
    StateApp app;
    initApp(app);
    // Hole hole;
    Deck deck;
    build_deck(deck.deck);
    print(deck.deck);
    shuffle(deck.deck);
    print(deck.deck);
    SDL_Event e;
    while(app.run){
        while(SDL_PollEvent(&e)){
            switch(e.type){
                case SDL_QUIT:{
                    app.run=false;
                }
            }
        }
        SDL_SetRenderDrawColor(app.render, 0, 120, 0, 255);
        SDL_RenderClear(app.render);

        SDL_RenderPresent(app.render);
    }

    closeApp();
    return 0;
}
// Deck deck;
// build_deck(deck.deck);
// print(deck.deck);
// // Пример фичи из новых стандартов (auto в параметрах лямбды и т.д.)
// auto welcome = [](auto name) {
//     return "Roadmap C++26 started for " + std::string(name);
// };

// std::cout << welcome("Gamedev Master") << std::endl;

// // Проверка версии стандарта
// std::println( "C++ Standard: {}",__cplusplus);

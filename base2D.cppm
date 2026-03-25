// base2D.cppm
module; // Начало глобального фрагмента
//clang++-20 -std=c++23 -c base2D.cppm --precompile -o base2D.pcm
//clang++-20 -std=c++23 -c base2D.pcm -o base2D.o
//clang++-20 -std=c++23 -stdlib=libc++ -fmodule-file=std=std.pcm -fprebuilt-module-path=. main.cpp base2D.o -lSDL2 -o app
#include <SDL2/SDL.h> // Все инклуды ДОЛЖНЫ быть здесь
#include <string_view>
export module base2D;

export struct Options{
    std::string_view name="Test";
    int posx=SDL_WINDOWPOS_CENTERED,posy=SDL_WINDOWPOS_CENTERED,width=1290,height=720,index=-1;
    uint32_t window_flags=SDL_WINDOW_ALLOW_HIGHDPI|SDL_WINDOW_RESIZABLE,render_flags=SDL_RENDERER_ACCELERATED,sdl_init=SDL_INIT_VIDEO;
};
// Теперь экспортируем нужные функции или структуры
export using SDL_Event = ::SDL_Event; // Экспортируем тип из глобального пространства
export using SDL_Window = ::SDL_Window;
// Экспортируем функции (теперь они часть модуля)
export using ::SDL_SetRenderDrawColor;
export using ::SDL_RenderClear;
export using ::SDL_RenderPresent;
export using ::SDL_PollEvent;
export using ::SDL_Quit;
export using ::SDL_CreateRenderer;

// Экспортируем типы, которые эти функции используют
export using ::SDL_Renderer;
export using ::SDL_Window;
export struct StateApp{
    Options default_options;
    SDL_Window* window=nullptr;
    SDL_Renderer* render=nullptr;
    bool run=false;
    ~StateApp(){
        SDL_DestroyRenderer(render);
        SDL_DestroyWindow(window);
    }
};
export void initApp(StateApp &app){
    SDL_Init(app.default_options.sdl_init);
    app.window=SDL_CreateWindow(
        app.default_options.name.data(),
        app.default_options.posx,
        app.default_options.posy,
        app.default_options.width,
        app.default_options.height,
        app.default_options.window_flags);
    app.render=SDL_CreateRenderer(
        app.window,
        app.default_options.index,
        app.default_options.render_flags);
    app.run=true;
}

export void closeApp(){
    SDL_Quit();
}

// base2D.cppm
module; // Начало глобального фрагмента
//stdclang++-20 -std=c++23 -stdlib=libc++ --precompile -o std.pcm /usr/lib/llvm-20/share/libc++/v1/std.cppm
//resultstdclang++-20 -std=c++23 -stdlib=libc++ -fmodule-file=std=std.pcm main.cpp -o app -lSDL2
//clang++-20 -std=c++23 -c base2D.cppm --precompile -o base2D.pcm
//clang++-20 -std=c++23 -c base2D.pcm -o base2D.o
//clang++-20 -std=c++23 -stdlib=libc++ -fmodule-file=std=std.pcm -fprebuilt-module-path=. main.cpp base2D.o -lSDL2 -o app
#include <SDL2/SDL.h> // Все инклуды ДОЛЖНЫ быть здесь
#include <SDL2/SDL_image.h>
#include <string_view>
#include <memory>
export module base2D;

export struct Options{
    std::string_view name="Test";

    int posx=SDL_WINDOWPOS_CENTERED,
    posy=SDL_WINDOWPOS_CENTERED,
    width=1290,
    height=720,
    index=-1;

    uint32_t window_flags=SDL_WINDOW_ALLOW_HIGHDPI|SDL_WINDOW_RESIZABLE,
    render_flags=SDL_RENDERER_ACCELERATED|SDL_RENDERER_PRESENTVSYNC,
    sdl_init=SDL_INIT_VIDEO,
    sdl_image_init=IMG_INIT_PNG;
};
// Теперь экспортируем нужные функции или структуры
export using SDL_Event = ::SDL_Event; // Экспортируем тип из глобального пространства
export using SDL_Window = ::SDL_Window;
// Экспортируем функции (теперь они часть модуля)
export using ::SDL_SetRenderDrawColor;
export using ::SDL_CreateTexture;
export using ::SDL_CreateRGBSurface;
export using ::SDL_CreateTextureFromSurface;
export using ::SDL_RenderClear;
export using ::SDL_RenderPresent;
export using ::SDL_PollEvent;
export using ::SDL_FillRect;
export using ::SDL_RenderDrawRect;
export using ::SDL_RenderFillRect;
export using ::SDL_MapRGB;
export using ::SDL_RenderCopy;
export using ::SDL_HasIntersection;
export using ::SDL_Quit;
export using ::SDL_CreateRenderer;
//TexturePtr texture{ SDL_CreateTextureFromSurface(app.render.get(), temp_surface) };
// Экспортируем типы, которые эти функции используют
export using ::SDL_Renderer;
export using ::SDL_Window;
export using ::SDL_Texture;
export using ::SDL_Surface;
export using ::SDL_Rect;
export using ::SDL_Color;
export using ::SDL_Point;
export using ::IMG_Load;

// Универсальный хелпер для SDL ресурсов
export template<typename T, auto Deleter>
using SDL_UniquePtr = std::unique_ptr<T, std::integral_constant<decltype(Deleter), Deleter>>;

// Теперь создание новых типов превращается в элегантный список:
export {
    using WindowPtr  = SDL_UniquePtr<SDL_Window,   SDL_DestroyWindow>;
    using RenderPtr  = SDL_UniquePtr<SDL_Renderer, SDL_DestroyRenderer>;
    using TexturePtr = SDL_UniquePtr<SDL_Texture,  SDL_DestroyTexture>;
    using SurfacePtr = SDL_UniquePtr<SDL_Surface,  SDL_FreeSurface>;
}

export struct StateApp{
    Options default_options;
    WindowPtr window;
    RenderPtr render;
    bool run=false;
};
export void initApp(StateApp &app){
    if (SDL_Init(app.default_options.sdl_init) < 0) return;
    if (IMG_Init(app.default_options.sdl_image_init)< 0) return;
    // reset() сам удалит старое окно, если оно было
    app.window.reset(SDL_CreateWindow(
        app.default_options.name.data(),
        app.default_options.posx, app.default_options.posy,
        app.default_options.width, app.default_options.height,
        app.default_options.window_flags
    ));

    app.render.reset(SDL_CreateRenderer(
        app.window.get(), // .get() достает сырой указатель для SDL
        app.default_options.index,
        app.default_options.render_flags
    ));

    if (app.window && app.render) app.run = true;

}

export void closeApp(){
    IMG_Quit();
    SDL_Quit();
}

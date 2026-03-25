#!/bin/bash

# Настройки компилятора
CXX="clang++-20"
STD="-std=c++23"
STDLIB="-stdlib=libc++"
LIBS="-lSDL2"
MOD_EXT="cppm"

# Путь к системному модулю std (может меняться в зависимости от дистрибутива)
STD_MODULE_PATH="/usr/lib/llvm-20/share/libc++/v1/std.cppm"

build_std() {
    if [ ! -f "std.pcm" ]; then
        echo "--- Сборка стандартного модуля (std.cppm) ---"
        $CXX $STD $STDLIB --precompile -o std.pcm "$STD_MODULE_PATH"
    else
        echo "std.pcm уже существует, пропускаю."
    fi
}

build_module() {
    local file=$1
    local name=$(basename "$file" .$MOD_EXT)
    echo "--- Сборка модуля: $name ---"
    
    # При сборке своих модулей тоже указываем путь к std.pcm
    $CXX $STD $STDLIB -fmodule-file=std=std.pcm --precompile "$file" -o "$name.pcm"
    $CXX $STD $STDLIB -c "$name.pcm" -o "$name.o"
}

build_project() {
    echo "--- Финальная сборка проекта ---"
    local modules_objs=$(ls *.o 2>/dev/null | grep -v "std.o") # std.o обычно не нужен, если есть pcm
    
    # -fmodule-file=std=std.pcm говорит компилятору, где искать 'import std'
    $CXX $STD $STDLIB -fmodule-file=std=std.pcm -fprebuilt-module-path=. \
         main.cpp $modules_objs $LIBS -o app
    
    if [ $? -eq 0 ]; then echo "Готово! Запуск: ./app"; fi
}

clear_build() {
    rm -f *.pcm *.o app
    echo "Очищено."
}

echo "1. Собрать всё (включая std)"
echo "2. Быстрая сборка (только проект)"
echo "3. Очистить"
read -p "Выбор: " choice

case $choice in
    1)
        build_std
        for f in *.$MOD_EXT; do [ -e "$f" ] && build_module "$f"; done
        build_project
        ;;
    2)
        build_project
        ;;
    3)
        clear_build
        ;;
esac

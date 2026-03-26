cmake_minimum_required(VERSION 3.10)
project(CarRace)

set(CMAKE_CXX_STANDARD 17)

# Hier müssen ALLE deine .cpp Dateien aufgelistet sein!
set(SOURCES 
    main.cpp 
    config.cpp 
    player.cpp 
    scoreboard.cpp 
    shop.cpp 
    ui.cpp
)

# Raylib Pfad (angepasst an deine Struktur)
add_subdirectory(raylib)

add_executable(CarRace ${SOURCES})

# Verknüpfung mit Raylib und System-Bibliotheken
target_link_libraries(CarRace raylib opengl32 gdi32 winmm user32 shell32)

target_include_directories(CarRace PUBLIC ${CMAKE_CURRENT_SOURCE_DIR} raylib/src)
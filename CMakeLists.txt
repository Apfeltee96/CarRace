cmake_minimum_required(VERSION 3.10)
project(CarRace VERSION 1.0 LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Raylib als Subprojekt einbinden (ohne die Beispiele zu kompilieren)
set(BUILD_EXAMPLES OFF CACHE BOOL "" FORCE)
add_subdirectory(raylib)

# Deine ausführbare Datei definieren
# NEU: Das WIN32 hier sorgt dafür, dass sich das schwarze Terminal nicht mehr öffnet!
add_executable(${PROJECT_NAME} WIN32 main.cpp level.cpp scoreboard.cpp)

# Raylib mit deinem Spiel verknüpfen
target_link_libraries(${PROJECT_NAME} raylib)

# Betriebssystem-spezifische Bibliotheken verknüpfen (wichtig für Windows)
if(WIN32)
    target_link_libraries(${PROJECT_NAME} opengl32 gdi32 winmm user32 shell32)
    
    # NEU: Sagt dem Microsoft-Compiler, dass er bei "main" starten soll, auch wenn das Terminal aus ist!
    if(MSVC)
        target_link_options(${PROJECT_NAME} PRIVATE "/ENTRY:mainCRTStartup")
    endif()
endif()
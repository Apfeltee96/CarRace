cmake_minimum_required(VERSION 3.10)
project(CarRace)

set(CMAKE_CXX_STANDARD 17)

# Liste deiner Quelldateien
set(SOURCES 
    main.cpp 
    config.cpp 
    player.cpp 
    scoreboard.cpp 
    ui.cpp
    game.cpp
)
if(WIN32)
    set(SOURCES ${SOURCES} resource.rc)
endif()

# Raylib Pfad
add_subdirectory(raylib)

# WIN32 sorgt dafür, dass kein Terminal erscheint
add_executable(CarRace WIN32 ${SOURCES})

# --- REFACTORING FIX: Terminal ausblenden ohne WinMain-Fehler ---
if(MSVC)
    # Dieser Befehl sagt dem Linker: "Nimm das Windows-Subsystem, 
    # aber starte trotzdem bei der normalen main() Funktion."
    set_target_properties(CarRace PROPERTIES LINK_FLAGS "/ENTRY:mainCRTStartup")
endif()

# Verknüpfung mit Raylib und System-Bibliotheken
target_link_libraries(CarRace raylib)
if(WIN32)
    target_link_libraries(CarRace opengl32 gdi32 winmm user32 shell32)
endif()


target_include_directories(CarRace PUBLIC ${CMAKE_CURRENT_SOURCE_DIR} raylib/src)
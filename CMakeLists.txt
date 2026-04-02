cmake_minimum_required(VERSION 3.10)
project(CarRace)

set(CMAKE_CXX_STANDARD 17)

# ---------------------------------------------------------------------------
# Quelldateien
# ---------------------------------------------------------------------------
set(GAME_SOURCES
    main.cpp
    config.cpp
    player.cpp
    scoreboard.cpp
    ui.cpp
    game.cpp
)

set(TEST_SOURCES
    tests.cpp
    config.cpp
)

if(WIN32)
    list(APPEND GAME_SOURCES resource.rc)
endif()

# ---------------------------------------------------------------------------
# Raylib
# ---------------------------------------------------------------------------
add_subdirectory(raylib)

# ---------------------------------------------------------------------------
# Haupt-Executable
# ---------------------------------------------------------------------------
add_executable(CarRace ${GAME_SOURCES})

# Kein Konsolenfenster unter Windows
if(WIN32)
    set_target_properties(CarRace PROPERTIES WIN32_EXECUTABLE TRUE)
endif()

# MSVC: normales main() statt WinMain verwenden
if(MSVC)
    set_target_properties(CarRace PROPERTIES LINK_FLAGS "/ENTRY:mainCRTStartup")
endif()

target_include_directories(CarRace PRIVATE ${CMAKE_CURRENT_SOURCE_DIR} raylib/src)
target_link_libraries(CarRace PRIVATE raylib)

if(WIN32)
    target_link_libraries(CarRace PRIVATE opengl32 gdi32 winmm user32 shell32)
endif()

# ---------------------------------------------------------------------------
# Tests (CTest)
# ---------------------------------------------------------------------------
enable_testing()

add_executable(CarRaceTests ${TEST_SOURCES})
target_include_directories(CarRaceTests PRIVATE ${CMAKE_CURRENT_SOURCE_DIR} raylib/src)
target_link_libraries(CarRaceTests PRIVATE raylib)

if(WIN32)
    target_link_libraries(CarRaceTests PRIVATE opengl32 gdi32 winmm user32 shell32)
endif()

add_test(NAME SpeedTests COMMAND CarRaceTests)

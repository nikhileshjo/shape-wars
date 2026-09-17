add_executable(start_game
    main.cpp
    Game.h
    Game.cpp
    EntityManager.h
    EntityManager.cpp
    Entity.h
    Entity.cpp
    Components.hpp
    Vec2.h
    Vec2.cpp
    )

target_include_directories(start_game PRIVATE
    ${imgui_SOURCE_DIR}
)

target_link_libraries(start_game PRIVATE
    ImGui-SFML
)
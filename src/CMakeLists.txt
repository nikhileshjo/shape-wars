add_executable(start_game
    main.cpp 
    Vec2.hpp
    )

target_include_directories(start_game PRIVATE
    ${imgui_SOURCE_DIR}
)

target_link_libraries(start_game PRIVATE
    ImGui-SFML
)
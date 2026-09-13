include(FetchContent)

FetchContent_Declare(
  sfml
  GIT_REPOSITORY "https://github.com/SFML/SFML.git"
  GIT_TAG 7f1162dfea4969bc17417563ac55d93b72e84c1e # 3.0.0
)

add_subdirectory(sfml)
FetchContent_Declare(
  imgui
  GIT_REPOSITORY "https://github.com/ocornut/imgui.git"
  GIT_TAG f63c95a076a401721ceaf21f30d4f12e8c40cb2c # 1.91.1-docked
)

FetchContent_MakeAvailable(imgui)

FetchContent_Declare(
  imgui-sfml
  GIT_REPOSITORY "https://github.com/SFML/imgui-sfml.git"
  GIT_TAG 5e8f8fb857c8f3cff1096bf3e4402c64e3bcf6af # v3.0
)
add_subdirectory(imgui-sfml)
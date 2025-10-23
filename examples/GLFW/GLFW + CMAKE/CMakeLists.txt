cmake_minimum_required(VERSION 3.15)
project(GLFW_App LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# ---------------------------------------------------------------------
# 🔹 Wbudowany sposób na pobranie GLFW przez FetchContent
# ---------------------------------------------------------------------
include(FetchContent)

FetchContent_Declare(
  glfw
  GIT_REPOSITORY https://github.com/glfw/glfw.git
  GIT_TAG latest
)

FetchContent_MakeAvailable(glfw)

# ---------------------------------------------------------------------
# 🔹 (Opcjonalnie) Dodaj GLAD – loader funkcji OpenGL
# ---------------------------------------------------------------------
FetchContent_Declare(
  glad
  GIT_REPOSITORY https://github.com/Dav1dde/glad.git
  GIT_TAG master
)
FetchContent_MakeAvailable(glad)

# ---------------------------------------------------------------------
# 🔹 Główne źródła aplikacji
# ---------------------------------------------------------------------
add_executable(${PROJECT_NAME}
    "main.cpp"
    "main.h"
)

# ---------------------------------------------------------------------
# 🔹 Linkowanie bibliotek: GLFW + GLAD + OpenGL
# ---------------------------------------------------------------------
find_package(OpenGL REQUIRED)

target_link_libraries(${PROJECT_NAME}
    PRIVATE
        glfw
        glad
        OpenGL::GL
)

# ---------------------------------------------------------------------
# 🔹 Wymagane include directory (żeby nagłówki działały)
# ---------------------------------------------------------------------
target_include_directories(${PROJECT_NAME}
    PRIVATE
        ${glfw_SOURCE_DIR}/include
        ${glad_SOURCE_DIR}/include
)

# ---------------------------------------------------------------------
# 🔹 Ustawienia dla Visual Studio (ładne grupowanie w Solution Explorer)
# ---------------------------------------------------------------------
source_group(TREE ${CMAKE_CURRENT_SOURCE_DIR} FILES "main.cpp" "main.h")

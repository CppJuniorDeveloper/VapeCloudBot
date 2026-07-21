cmake_minimum_required(VERSION 3.11)

set(PROJECT_NAME VapeCloudBot)
set(SOURCE_DIR source)

set(HEADER_DIRS
    ${CMAKE_SOURCE_DIR}/config
    ${SOURCE_DIR}/logger
)

set(SOURCES
    ${SOURCE_DIR}/main.cpp
    ${SOURCE_DIR}/logger/Logger.cpp
)

project(${PROJECT_NAME} VERSION 1.0.0 LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_SOURCE_DIR}/bin/${CMAKE_BUILD_TYPE})

add_subdirectory(libs/tgbot-cpp)

add_executable(${PROJECT_NAME} ${SOURCES})

target_link_directories(${PROJECT_NAME} PRIVATE ${HEADER_DIRS})

target_link_libraries(${PROJECT_NAME} PRIVATE TgBot)

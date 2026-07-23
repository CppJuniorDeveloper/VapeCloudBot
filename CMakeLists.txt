cmake_minimum_required(VERSION 3.11)

set(PROJECT_NAME VapeCloudBot)
set(SOURCE_DIR source)

set(HEADER_DIRS
    ${CMAKE_SOURCE_DIR}/config
    ${SOURCE_DIR}/utils/logger
    ${SOURCE_DIR}/utils/messages
    ${SOURCE_DIR}/utils/translation
    ${SOURCE_DIR}/utils/keyboard
    ${SOURCE_DIR}/admin
    ${SOURCE_DIR}/core
    ${SOURCE_DIR}/handlers
    ${SOURCE_DIR}/session
)

set(SOURCES
    ${SOURCE_DIR}/main.cpp
    ${SOURCE_DIR}/utils/logger/Logger.cpp
    ${SOURCE_DIR}/utils/translation/TranslationClient.cpp
    ${SOURCE_DIR}/utils/keyboard/KeyboardBuilder.cpp
    ${SOURCE_DIR}/admin/AdminManager.cpp
    ${SOURCE_DIR}/core/TgBot.cpp
    ${SOURCE_DIR}/core/RegisterBot.cpp
    ${SOURCE_DIR}/handlers/CommandHandler.cpp
    ${SOURCE_DIR}/session/UserSessionManager.cpp
)

project(${PROJECT_NAME} VERSION 1.0.0 LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_SOURCE_DIR}/bin/${CMAKE_BUILD_TYPE})

add_subdirectory(libs/tgbot-cpp)

find_package(nlohmann_json REQUIRED)

add_executable(${PROJECT_NAME} ${SOURCES})

target_link_directories(${PROJECT_NAME} PRIVATE ${HEADER_DIRS})

target_link_libraries(${PROJECT_NAME} PRIVATE TgBot)
target_link_libraries(${PROJECT_NAME} PRIVATE nlohmann_json::nlohmann_json)

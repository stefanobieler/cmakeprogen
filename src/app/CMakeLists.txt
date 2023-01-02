set(APP_NAME cmakeprogen)
set(APP_SRC cmakeprogen.cpp)

add_executable(${APP_NAME} ${APP_SRC})

target_include_directories(
    ${APP_NAME}
    PRIVATE
    ./)

target_link_libraries(
    ${APP_NAME}
    PRIVATE
    progen)

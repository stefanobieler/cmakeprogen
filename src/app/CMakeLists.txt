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

install(
    TARGETS ${APP_NAME}
    RUNTIME DESTINATION bin
    ARCHIVE DESTINATION lib64/cmakeprogen
    LIBRARY DESTINATION lib64/cmakeprogen
    PUBLIC_HEADER DESTINATION include/cmakeprogen)

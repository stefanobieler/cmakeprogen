set(LIB_NAME progen)
set(
    LIB_SRC 
    filegen.cpp
    progen.cpp)


add_subdirectory(settings)
add_subdirectory(utils)

add_library(${LIB_NAME} STATIC ${LIB_SRC})
target_include_directories(
    ${LIB_NAME} 
    PRIVATE 
    ${CMAKE_BINARY_DIR}/linux/include/
    ./)

install(
    TARGETS ${LIB_NAME}
    ARCHIVE DESTINATION lib64/cmakeprogen
    LIBRARY DESTINATION lib64/cmakeprogen
    PUBLIC_HEADER DESTINATION include/cmakeprogen)

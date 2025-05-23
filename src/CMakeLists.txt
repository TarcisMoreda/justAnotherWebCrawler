cmake_minimum_required(VERSION 4.0.0)

project(jawc_app)

add_executable(
    ${PROJECT_NAME}
    main.c
)
target_link_libraries(${PROJECT_NAME} PRIVATE jawc)
target_compile_options(${PROJECT_NAME} PRIVATE
    -Wall -Wextra -Wfatal-errors
    $<$<CONFIG:Debug>:-O0 -g -DDEBUG>          # Debug flags
    $<$<CONFIG:Release>:-O3 -DNDEBUG>          # Release flags
    $<$<CONFIG:RelWithDebInfo>:-O2 -g>         # RelWithDebInfo flags
    $<$<CONFIG:MinSizeRel>:-Os>
)
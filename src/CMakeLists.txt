# Part of build system for project_class

# Specify the name of the current binary
set(APP_NAME app)

set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)
set(CMAKE_AUTOUIC ON)

find_package(Qt6 COMPONENTS Core Widgets REQUIRED)

find_package(jsoncons COMPONENTS json json_pointer)


# Add the source files
set( PRJ_SRC
    main.cpp
    gui/frm_main.cpp
    models/tree_model.cpp
    models/tree_model_delegate.cpp
    res/resource.qrc
)

include_directories( ${CMAKE_SOURCE_DIR}/src )

add_executable(${APP_NAME} ${PRJ_SRC})

target_link_libraries(${APP_NAME} PRIVATE 
    Qt6::Core
    Qt6::Widgets
    jsoncons
)

add_custom_command(TARGET ${APP_NAME}
    POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E make_directory ${CMAKE_SOURCE_DIR}/run/bin
    COMMENT "Create ${CMAKE_SOURCE_DIR}/run/bin folder.")

add_custom_command(TARGET ${APP_NAME}
    POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E copy $<TARGET_FILE:${APP_NAME}> ${CMAKE_SOURCE_DIR}/run/bin
    COMMENT "Copying ${APP_NAME} to run/bin/ folder.")
add_subdirectory(gui)

target_sources(swag_scanner_lib PRIVATE
        ${CMAKE_CURRENT_SOURCE_DIR}/Visualizer.cpp
        ${CMAKE_CURRENT_SOURCE_DIR}/Visualizer.h
        )

target_include_directories(swag_scanner_lib PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})
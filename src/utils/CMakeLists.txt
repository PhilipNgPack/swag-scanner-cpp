target_sources(swag_scanner_lib PRIVATE
        ${CMAKE_CURRENT_SOURCE_DIR}/Algorithms.cpp
        ${CMAKE_CURRENT_SOURCE_DIR}/Algorithms.h
        ${CMAKE_CURRENT_SOURCE_DIR}/Constants.h
        ${CMAKE_CURRENT_SOURCE_DIR}/Logger.h
        )

target_include_directories(swag_scanner_lib PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})
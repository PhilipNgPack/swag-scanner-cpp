target_sources(swag_scanner_lib PRIVATE
        ${CMAKE_CURRENT_SOURCE_DIR}/ICamera.h
        ${CMAKE_CURRENT_SOURCE_DIR}/SR305.cpp
        ${CMAKE_CURRENT_SOURCE_DIR}/SR305.h
        )

target_include_directories(swag_scanner_lib PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})
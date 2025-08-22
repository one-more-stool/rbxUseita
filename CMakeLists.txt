cmake_minimum_required(VERSION 3.10)

# Version information
set(PROJECT_VERSION_MAJOR 1)
set(PROJECT_VERSION_MINOR 0)
set(PROJECT_VERSION_PATCH 0)
set(PROJECT_VERSION "${PROJECT_VERSION_MAJOR}.${PROJECT_VERSION_MINOR}.${PROJECT_VERSION_PATCH}")

project(rbxUseita VERSION ${PROJECT_VERSION})

set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Find packages
find_package(OpenGL REQUIRED)

# Add ImGui source files
set(IMGUI_DIR ${CMAKE_SOURCE_DIR}/imgui)
set(IMGUI_SOURCES
    ${IMGUI_DIR}/imgui.cpp
    ${IMGUI_DIR}/imgui_demo.cpp
    ${IMGUI_DIR}/imgui_draw.cpp
    ${IMGUI_DIR}/imgui_tables.cpp
    ${IMGUI_DIR}/imgui_widgets.cpp
    ${IMGUI_DIR}/backends/imgui_impl_win32.cpp
    ${IMGUI_DIR}/backends/imgui_impl_dx11.cpp
)

add_executable(rbxUseita WIN32
    main.cpp
    process_manager.cpp
    ui_manager.cpp
    localization.cpp
    theme.cpp
    logger.cpp
    config.cpp
    resource.rc
    font_data.h
    fa_solid_data.h
    ${IMGUI_SOURCES}
)

if(WIN32)
    target_link_libraries(rbxUseita psapi d3d11 d3dcompiler dwmapi)
    target_include_directories(rbxUseita PRIVATE ${IMGUI_DIR} ${IMGUI_DIR}/backends)
    set_target_properties(rbxUseita PROPERTIES WIN32_EXECUTABLE TRUE)
    
    # Add version information
    target_compile_definitions(rbxUseita PRIVATE 
        PROJECT_VERSION_MAJOR=${PROJECT_VERSION_MAJOR}
        PROJECT_VERSION_MINOR=${PROJECT_VERSION_MINOR}
        PROJECT_VERSION_PATCH=${PROJECT_VERSION_PATCH}
        PROJECT_VERSION_STRING="${PROJECT_VERSION}"
    )
    
    # Optimization flags for Release builds
    set(CMAKE_CXX_FLAGS_RELEASE "${CMAKE_CXX_FLAGS_RELEASE} /O2 /GL")
    set(CMAKE_EXE_LINKER_FLAGS_RELEASE "${CMAKE_EXE_LINKER_FLAGS_RELEASE} /LTCG")
    
    # UPX compression for Release builds
    find_program(UPX_EXECUTABLE upx)
    if(NOT UPX_EXECUTABLE)
        # Try to find UPX in project root directory
        set(UPX_EXECUTABLE "${CMAKE_SOURCE_DIR}/upx.exe")
        if(EXISTS ${UPX_EXECUTABLE})
            message(STATUS "UPX found in project root: ${UPX_EXECUTABLE}")
        endif()
    endif()
    
    # Auto-download UPX if not found
    if(NOT UPX_EXECUTABLE OR NOT EXISTS ${UPX_EXECUTABLE})
        message(STATUS "UPX not found, downloading automatically...")
        
        # Detect system architecture
        if(CMAKE_SIZEOF_VOID_P EQUAL 8)
            set(UPX_ARCH "64")
        else()
            set(UPX_ARCH "32")
        endif()
        
        set(UPX_VERSION "5.0.2")
        set(UPX_URL "https://github.com/upx/upx/releases/download/v${UPX_VERSION}/upx-${UPX_VERSION}-win${UPX_ARCH}.zip")
        set(UPX_ZIP "${CMAKE_SOURCE_DIR}/upx.zip")
        set(UPX_DIR "${CMAKE_SOURCE_DIR}/upx-${UPX_VERSION}-win${UPX_ARCH}")
        
        # Download UPX
        file(DOWNLOAD ${UPX_URL} ${UPX_ZIP} SHOW_PROGRESS)
        
        # Extract UPX
        file(ARCHIVE_EXTRACT INPUT ${UPX_ZIP} DESTINATION ${CMAKE_SOURCE_DIR})
        
        # Copy upx.exe to project root
        file(COPY ${UPX_DIR}/upx.exe DESTINATION ${CMAKE_SOURCE_DIR})
        
        # Cleanup
        file(REMOVE ${UPX_ZIP})
        file(REMOVE_RECURSE ${UPX_DIR})
        
        set(UPX_EXECUTABLE "${CMAKE_SOURCE_DIR}/upx.exe")
        message(STATUS "UPX downloaded and installed: ${UPX_EXECUTABLE}")
    endif()
    
    if(UPX_EXECUTABLE AND EXISTS ${UPX_EXECUTABLE})
        message(STATUS "UPX found: ${UPX_EXECUTABLE}")
        add_custom_command(TARGET rbxUseita POST_BUILD
            COMMAND ${UPX_EXECUTABLE} --best --lzma $<TARGET_FILE:rbxUseita> || echo "File already compressed or compression failed"
            COMMENT "Compressing executable with UPX..."
            VERBATIM
        )
    else()
        message(WARNING "UPX not found. Install UPX for executable compression.")
    endif()
endif()

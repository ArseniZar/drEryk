cmake_minimum_required(VERSION 3.5)

project(UNotePad VERSION 1.0.1 LANGUAGES CXX)

# Set the C++ standard for the entire project
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED True)
set(CMAKE_CXX_EXTENSIONS OFF)

# Find wxWidgets
find_package(wxWidgets REQUIRED COMPONENTS core base stc)
include(${wxWidgets_USE_FILE})

# Add include directory to search path for header files
include_directories(include)

# List all source files (.cpp) for the build, specifying their path
set(SRC_FILES
    src/main.cpp
    src/app.cpp
    src/mainFrame.cpp
    src/about.cpp
    src/textUtils.cpp
)

# Add the resource file for Windows
if(WIN32)
    set(SRC_FILES ${SRC_FILES} res.rc)
endif()

# Create the executable
add_executable(UNotePad WIN32 MACOSX_BUNDLE ${SRC_FILES})

# Link with wxWidgets libraries
target_link_libraries(UNotePad PRIVATE ${wxWidgets_LIBRARIES})

# --- Optional: Doxygen documentation generation ---
find_package(Doxygen)
if(DOXYGEN_FOUND)
    set(DOXYGEN_PROJECT_NAME "${PROJECT_NAME}")
    set(DOXYGEN_PROJECT_NUMBER "${PROJECT_VERSION}")
    set(DOXYGEN_PROJECT_BRIEF "A simple text editor application built with C++ and wxWidgets.")

    doxygen_add_docs(doxygen
        ${PROJECT_SOURCE_DIR}
        COMMENT "Generated Doxygen HTML documentation."
    )

    message(STATUS "Doxygen found. Added 'doxygen' target to generate documentation.")
else()
    message(WARNING "Doxygen not found. Documentation will not be generated.")
endif()
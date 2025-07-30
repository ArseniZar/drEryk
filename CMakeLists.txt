cmake_minimum_required(VERSION 3.16)

project(UNotePad VERSION 1.0.1)

find_package(wxWidgets REQUIRED COMPONENTS core base stc)
include(${wxWidgets_USE_FILE})

file(GLOB_RECURSE SRC_FILES
    "${PROJECT_SOURCE_DIR}/src/*.cpp"
    "${PROJECT_SOURCE_DIR}/include/*.h"
)

include_directories(${PROJECT_SOURCE_DIR}/include)

if(WIN32)
    set(SRC_FILES ${SRC_FILES} res.rc)
endif()

add_executable(UNotePad WIN32 MACOSX_BUNDLE ${SRC_FILES})

target_link_libraries(UNotePad ${wxWidgets_LIBRARIES})

# Предкомпилированные заголовки
target_precompile_headers(UNotePad PRIVATE <wx/wx.h>)

# Включение ccache
set(CMAKE_C_COMPILER_LAUNCHER ccache)
set(CMAKE_CXX_COMPILER_LAUNCHER ccache)

# Опция для документации
option(BUILD_DOC "Build documentation with Doxygen" OFF)

find_package(Doxygen)
if(BUILD_DOC AND DOXYGEN_FOUND)
    set(DOXYGEN_PROJECT_NAME "UNotePad")
    set(DOXYGEN_PROJECT_NUMBER ${PROJECT_VERSION})
    set(DOXYGEN_PROJECT_BRIEF "A simple text editor application.")

    doxygen_add_docs(doxygen
        ${PROJECT_SOURCE_DIR}
        COMMENT "Generated html pages"
    )

    message("Found doxygen and generated the doxyfile")
else()
    message("Didn't find doxygen or documentation disabled")
endif()

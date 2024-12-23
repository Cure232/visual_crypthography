
cmake_minimum_required(VERSION 3.15)

project(makemake)

set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

# ����������� ��������� ��� ����������� Visual Studio
if(MSVC)
    # ���������� ����������� �������� ��� ������������ ���������
    set(CMAKE_WINDOWS_EXPORT_ALL_SYMBOLS true)

    # �������� ������� ������� ��������������
    if(CMAKE_CXX_FLAGS MATCHES "/W[0-4]")
        string(REGEX REPLACE "/W[0-4]" "/W4" CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS}")
    else()
        set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} /W4")
    endif()
endif()

# ����� OpenCV
find_package(opencv REQUIRED CONFIG HINTS "opencv/build/x64/vc16/lib")

if(NOT OpenCV_FOUND)
    message(FATAL_ERROR "OpenCV not found. Please set the correct path.")
endif()

# �������� ���� � ������������ ������ OpenCV
include_directories(${OpenCV_INCLUDE_DIRS})

# ����������
add_library(functions SHARED "include/main.h" "src/main.cc")
target_include_directories(functions PUBLIC ${OpenCV_INCLUDE_DIRS} "include")

# exe-����
add_executable(console "src/main.cc")

# �������� ���������
target_link_libraries(functions PUBLIC ${OpenCV_LIBS})
target_link_libraries(console PUBLIC functions ${OpenCV_LIBS})

add_custom_command(TARGET console POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E copy $<TARGET_RUNTIME_DLLS:console> $<TARGET_FILE_DIR:console>
    COMMAND_EXPAND_LISTS
)

# ���������� googletest
include(FetchContent)
FetchContent_Declare(googletest URL https://github.com/google/googletest/archive/6b63c98ac43efc992122f1da12aaf0a0e0658206.zip)
set(gtest_force_shared_crt ON CACHE BOOL "" FORCE)
FetchContent_MakeAvailable(googletest)

# exe � �������
add_executable(tests "tests/tests.cc")
target_link_libraries(tests functions GTest::gtest_main)
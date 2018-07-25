cmake_minimum_required(VERSION 3.10)
project(stereo_matching)

find_package(OpenCV REQUIRED)
find_package(OpenMP)
IF(OPENMP_FOUND)
    message("Found OpenMP!")
    # add flags for OpenMP
    set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} ${OpenMP_C_FLAGS}")
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} ${OpenMP_CXX_FLAGS}")
    set(CMAKE_SHARED_LINKER_FLAGS "${CMAKE_SHARED_LINKER_FLAGS} ${OpenMP_SHARED_LINKER_FLAGS}")
    set(CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} ${OpenMP_EXE_LINKER_FLAGS}")
ELSE()
    message("Missed OpenMP!")
ENDIF()
set(CMAKE_CXX_STANDARD 11)

add_executable(stereo_matching main.cpp inc/config.h src/CVC.cpp inc/CVC.h src/fastGIF.cpp inc/fastGIF.h src/matcher.cpp inc/matcher.h inc/jointWMF.h)
target_link_libraries(stereo_matching ${OpenCV_LIBS})
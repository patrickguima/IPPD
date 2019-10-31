cmake_minimum_required (VERSION 3.4)

project (ippd)

find_package (MPI REQUIRED)

find_package (OpenMP REQUIRED)

find_package (OpenCL REQUIRED)

set (CMAKE_RUNTIME_OUTPUT_DIRECTORY "${PROJECT_SOURCE_DIR}/bin")

set (CMAKE_CXX_STANDARD 17)

set (CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall -Wextra")

set (CMAKE_CXX_COMPILER mpic++)

set (SRC
    src/main.cc
)

add_executable (${CMAKE_PROJECT_NAME} ${SRC})

target_link_libraries (${CMAKE_PROJECT_NAME}
    PRIVATE MPI::MPI_C OpenMP::OpenMP_CXX  OpenCL::OpenCL
)

target_compile_definitions (${CMAKE_PROJECT_NAME} PRIVATE OMPI_SKIP_MPICXX)
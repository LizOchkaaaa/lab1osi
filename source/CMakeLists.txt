get_filename_component(MONOLITH_INCLUDE_PATH "." ABSOLUTE)
get_filename_component(MONOLITH_SOURCE_PATH "./${PROJECT_NAME}" ABSOLUTE)
include_directories("C:/Libraries/phnt")
file(GLOB_RECURSE MONOLITH_SOURCES CONFIGURE_DEPENDS *.hpp *.cpp)

set(MONOLITH_LIB_SOURCES ${MONOLITH_SOURCES}
        monolith/console/Console.cpp
        monolith/console/Console.hpp
        monolith/console/Command.cpp
        monolith/console/Command.hpp
        monolith/commands/Exit.cpp
        monolith/commands/Exit.hpp
        monolith/commands/Help.cpp
        monolith/commands/Help.hpp
        monolith/console/CommandFactory.cpp
        monolith/console/CommandFactory.hpp
        monolith/commands/History.cpp
        monolith/commands/History.hpp
        monolith/commands/Ls.cpp
        monolith/commands/Ls.hpp
        monolith/commands/Cat.cpp
        monolith/commands/Cat.hpp
        monolith/commands/Cd.cpp
        monolith/commands/Cd.hpp
        monolith/commands/Mkdir.cpp
        monolith/commands/Mkdir.hpp
        monolith/commands/Rm.cpp
        monolith/commands/Rm.hpp
        monolith/bench/IOThptWright.cpp
        monolith/bench/IOThptWright.hpp
        monolith/bench/BinSearch.cpp
        monolith/bench/BinSearch.hpp
        monolith/bench/JoinBench.cpp
        monolith/bench/IO.cpp
        monolith/bench/Bin.cpp
        monolith/bench/CreateProcessHelper.cpp
        monolith/bench/CreateProcessHelper.hpp
)
set(CMAKE_EXE_LINKER_FLAGS "-static")
add_executable(IO
        monolith/bench/CreateProcessHelper.cpp
        monolith/bench/CreateProcessHelper.hpp
    monolith/bench/IO.cpp)
add_custom_target(
        run_IO
        COMMAND IO 10 testfile.txt 4 1024
        DEPENDS IO
        COMMENT "Running IO with custom arguments"
)
add_executable(IOThptWright
        monolith/bench/IOThptWright.cpp
        monolith/bench/IOThptWright.hpp
)
add_executable(IO_OPT
        monolith/bench/IOThptWright.cpp
        monolith/bench/IOThptWright.hpp
        monolith/bench/IO.cpp)
add_executable(Bin
        monolith/bench/CreateProcessHelper.cpp
        monolith/bench/CreateProcessHelper.hpp
        monolith/bench/Bin.cpp)

add_executable(Bin_OPT
        monolith/bench/CreateProcessHelper.cpp
        monolith/bench/Bin.cpp)

add_executable(BinSearch
        monolith/bench/BinSearch.cpp
        monolith/bench/BinSearch.hpp
)

target_compile_options(Bin_OPT PRIVATE -O3 -march=native -ffast-math -funroll-loops)

target_compile_options(IO_OPT PRIVATE -O3 -march=native -ffast-math -pipe)

set(MONOLITH_APP_SOURCES monolith/app/Main.cpp)
add_library(${PROJECT_NAME} STATIC ${MONOLITH_LIB_SOURCES})
target_include_directories(${PROJECT_NAME} PUBLIC ${MONOLITH_INCLUDE_PATH})

add_executable(${PROJECT_NAME}-app ${MONOLITH_APP_SOURCES})
set_property(TARGET ${PROJECT_NAME}-app PROPERTY OUTPUT_NAME ${PROJECT_NAME})
target_link_libraries(
    ${PROJECT_NAME}-app PRIVATE
    ${PROJECT_NAME}
)

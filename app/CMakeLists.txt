
cmake_minimum_required(VERSION 3.4.1)

add_library( fix
             SHARED
             src/main/cpp/fix.cpp )

find_library( log-lib
              log )

target_link_libraries( fix
                       ${log-lib} )
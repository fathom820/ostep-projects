cmake_minimum_required(VERSION 3.10)
project(ostep_projects C)

set(CMAKE_C_STANDARD 11)

include_directories(concurrency-mapreduce)
include_directories(concurrency-webserver/src)
include_directories(filesystems-distributed)
include_directories(processes-shell)

add_executable(ostep_projects
        concurrency-mapreduce/mapreduce.h
        concurrency-webserver/src/io_helper.c
        concurrency-webserver/src/io_helper.h
        concurrency-webserver/src/request.c
        concurrency-webserver/src/request.h
        concurrency-webserver/src/spin.c
        concurrency-webserver/src/wclient.c
        concurrency-webserver/src/wserver.c
        filesystems-distributed/mfs.h
        initial-xv6/tests/test_1.c
        initial-xv6/tests/test_2.c
        processes-shell/builtins.c
        processes-shell/builtins.h
        processes-shell/cmdops.c
        processes-shell/cmdops.h
        processes-shell/error.c
        processes-shell/error.h
        processes-shell/main.c
        processes-shell/paths.c
        processes-shell/paths.h
        processes-shell/redirect.c
        processes-shell/redirect.h)

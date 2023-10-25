cmake_minimum_required(VERSION 3.26)
project(server)

set(CMAKE_CXX_STANDARD 17)

add_executable(server main.cpp
        logger.h
        logger_builder.h
        logger_builder_concrete.h
        logger_concrete.h
        memory.h
        allocator_operator_new.h
        allocator_border_descriptor.h
        allocator_on_sorte_list.h
        allocator_buddies.h
        associotive_conteiner.h
        binary_search_tree.h
        AVL.h
        splay_tree.h
        red_black_tree.h
        flyweight_for_string.h
        database.h
        logger_IPC.h
)

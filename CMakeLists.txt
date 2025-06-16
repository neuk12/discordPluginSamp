cmake_minimum_required(VERSION 3.10)

project(DiscordLogs)

list(APPEND CMAKE_MODULE_PATH ${CMAKE_SOURCE_DIR}/cmake/modules)

include(AddSAMPPlugin)

add_samp_plugin(${PROJECT_NAME} 
    src/main.cpp
    src/DiscordNatives.cpp
    src/discord_logs.cpp
    src/discord.cpp
)



if (WIN32)

    target_sources(${PROJECT_NAME} PRIVATE ${CMAKE_SOURCE_DIR}/discord.def)
endif()

if (CMAKE_SIZEOF_VOID_P EQUAL 8)
    message(FATAL_ERROR "SA:MP plugins deben compilarse en 32 bits")
endif()

# Markdown-CLI/CMakeLists.txt
# Created by Robin Rowe 2024-12-13
# License Open Source MIT

cmake_minimum_required(VERSION 3.10)
#set(CMAKE_C_STANDARD 23)
set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

project(Markdown-CLI)
message("Configuring ${CMAKE_PROJECT_NAME}...")

if(NOT WIN32 AND NOT APPLE)
	link_libraries(rt pthread)
endif(NOT WIN32 AND NOT APPLE)

option(SUPPRESS "Enable suppression of code warnings" false)
if(SUPPRESS)
	if(WIN32)
		add_definitions(-D_CRT_SECURE_NO_WARNINGS /wd4244 /wd4305 /wd4018 /wd26451 /wd6031 /wd4267)
	endif(WIN32)
endif(SUPPRESS)

option(UNISTD "Enable libunistd" true)
if(UNISTD)
	if(WIN32)
#		message("CMAKE_GENERATOR_PLATFORM = ${CMAKE_GENERATOR_PLATFORM}")
		set(LIBUNISTD_PATH /code/github/libunistd)
		include_directories(${LIBUNISTD_PATH}/unistd)
		set (UNISTD_LINK_DIRECTORIES ${LIBUNISTD_PATH}/build/${CMAKE_GENERATOR_PLATFORM}/Release)
		message(libunistd = ${UNISTD_LINK_DIRECTORIES}/libunistd.lib)
		link_directories(${UNISTD_LINK_DIRECTORIES})
		link_libraries(libunistd Ws2_32 Imm32)
	endif(WIN32)
endif(UNISTD)

file(STRINGS sources.cmake SOURCES)
set(MODULE_NAME ${CMAKE_PROJECT_NAME}_lib)
add_library(${MODULE_NAME} ${SOURCES})
link_libraries(${MODULE_NAME})
add_executable(md_html md_html.cpp)



# MIT License

# Copyright (c) 2018 Alexander Gorin pu1s@outlook.com

# Permission is hereby granted, free of charge, to any person obtaining a copy
# of this software and associated documentation files (the "Software"), to deal
# in the Software without restriction, including without limitation the rights
# to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
# copies of the Software, and to permit persons to whom the Software is
# furnished to do so, subject to the following conditions:
#
# The above copyright notice and this permission notice shall be included in all
# copies or substantial portions of the Software.
#
# THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
# IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
# FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
# AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
# LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
# OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
# SOFTWARE.

#
cmake_minimum_required (VERSION 3.8 FATAL_ERROR)
#
set(PROJECT_NAME "libasterix")
project (${PROJECT_NAME})
set("${PROJECT_NAME}_SOURCE_DIR" "src")
set("$CMAKE_BINARY_DIR" "bin")
set (CMAKE_INCLUDE_PATH ${${PROJECT_NAME}_SOURCE_DIR})
set(TARGET_STATIC_LIB "")
set(TARGET_SHARED_LIB "")
set(TARGET_PLATFORM "")
set(OUT_PATH "out")
set(OUT_LIB_PATH "lib")

if(MSVC)
	set(TARGET_STATIC_LIB "libasterix")
	set(TARGET_SHARED_LIB "asterixlib")
	set(TARGET_PLATFORM "WIN32")
else()
	set(TARGET_STATIC_LIB "asterix-st")
	set(TARGET_SHARED_LIB "asterix-sh")
	set(TARGET_PLATFORM "UNIX")
endif()
set(INSTALL_LIB_BIN "${CMAKE_SOURCE_DIR}/${OUT_PATH}/${TARGET_PLATFORM}/${OUT_LIB_PATH}")
set(CMAKE_INSTALL_PREFIX ${INSTALL_LIB_BIN})
message(STATUS ${CMAKE_INCLUDE_PATH})
message(STATUS ${PROJECT_NAME} " Build is begin...")
file(GLOB_RECURSE SOURCES *.cpp *.h *.cxx *.hpp)
add_library("${TARGET_STATIC_LIB}" STATIC "${SOURCES}")
set_property(TARGET ${TARGET_STATIC_LIB} PROPERTY CXX_STANDARD 17)
target_include_directories(${TARGET_STATIC_LIB} PUBLIC ${CMAKE_INCLUDE_PATH})

add_library("${TARGET_SHARED_LIB}" SHARED "${SOURCES}")
set_property(TARGET "${TARGET_SHARED_LIB}" PROPERTY CXX_STANDARD 14)
target_include_directories(${TARGET_SHARED_LIB} PUBLIC ${CMAKE_INCLUDE_PATH})

message(${CMAKE_INSTALL_PREFIX})
#add_custom_command(TARGET libasterix POST_BUILD
#                  COMMAND ${CMAKE_COMMAND} -E copy
#                      "${CMAKE_CURRENT_BINARY_DIR}" DESTINATION "${CMAKE_SOURCE_DIR}/out/libs"
#)
install(TARGETS ${TARGET_STATIC_LIB} DESTINATION ${CMAKE_INSTALL_PREFIX})
#install(TARGETS lib_asterix DESTINATION ${CMAKE_INSTALL_PREFIX})
install(FILES "${CMAKE_INCLUDE_PATH}/libasterix.h" DESTINATION ${CMAKE_INSTALL_PREFIX})
#add_custom_command(TARGET libasterix POST_BUILD COMMAND ${CMAKE_COMMAND} -E copy ${CMAKE_SOURCE_DIR}"/${PROJECT_NAME}/${PROJECT_NAME}.lib" DESTINATION "${CMAKE_SOURCE_DIR}/build" )

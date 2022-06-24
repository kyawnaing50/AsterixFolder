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

# CMakeList.txt: файл проекта CMake верхнего уровня; проведите глобальную конфигурацию
# и включите сюда подпроекты.

#
cmake_minimum_required (VERSION 3.8 FATAL_ERROR)
#
project("asterix")

add_executable(asterix main.cpp)
	set(CMAKE_INSTALL_PREFIX "${CMAKE_SOURCE_DIR}/out")
	file(MAKE_DIRECTORY  "${CMAKE_INSTALL_PREFIX}")
	target_include_directories(asterix PUBLIC ${CURRENT_SOURCE_DIR})
	find_library(LIBASTERIX_LIBRARY libasterix)
if(LIBASTERIX_LIBRARY-NOTFOUND)
	message(STATUS "FATAL ERROR! libasreix not found.")
else()
	message(STATUS "Ok! libasreix found...")
	target_include_directories(asterix PUBLIC "${CMAKE_SOURCE_DIR}/bin")
endif()
#message("${LIBASTERIX_LIBRARY}")
target_link_libraries(asterix PUBLIC libasterix)

install(TARGETS asterix DESTINATION ${CMAKE_INSTALL_PREFIX})
#add_custom_command(TARGET asterix POST_BUILD COMMAND ${CMAKE_COMMAND} -E copy ${CMAKE_SOURCE_DIR}"/${PROJECT_NAME}/${PROJECT_NAME}.exe" DESTINATION "${CMAKE_SOURCE_DIR}/build" )

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

# Get System Info
function(get_operation_system)
message(STATUS "Prepare Operating System...")
if(WIN32)
message(STATUS "Welcome to build from Windows...")
set(${SYSTEM} "Windows")
else()
message(STATUS "Welcome to UNIX...")
set(${SYSTEM} " UNIX")
endif()
endfunction()
#
#
# Get config file exsist
function(get_config_file)
find_path (config_path ${CONFIG_FILE} ${CONFIG_DIR})
if(config_path-NOTFOUND)
message(STATUS "Config directory is not found.")
else()
message(STATUS "Config directory is found.")
message(STATUS ${config_path})
endif()
find_file(config_file ${CONFIG_FILE} ${CONFIG_DIR})
if(config_file-NOTFOUND)
message(STATUS "Config file not found...")
else()
message(STATUS "Config file is found.")
message(STATUS ${config_file})
endif()

endfunction()

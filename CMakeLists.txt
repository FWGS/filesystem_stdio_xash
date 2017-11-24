#
# Copyright (c) 2015 Pavlo Lavrenenko
#
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

cmake_minimum_required(VERSION 2.6.0)
project (FS_XASH)

set(XASH_SDK "../xash3d/" CACHE STRING "path to Xash3D FWGS SDK")

# set (CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall -Wextra -pedantic")
set (FS_XASH_LIBRARY filesystem_stdio)

file (GLOB FS_XASH_SOURCES src/*.cpp)
file (GLOB FS_XASH_HEADERS include/*.h)

include_directories (include/ ${XASH_SDK}/engine/)
add_compile_options(-std=c++11)
add_compile_options(-Wl,--no-undefined)

add_library (${FS_XASH_LIBRARY} SHARED ${FS_XASH_SOURCES} ${FS_XASH_HEADERS})

target_link_libraries(${FS_XASH_LIBRARY} ${CMAKE_DL_LIBS})

set_target_properties (${FS_XASH_LIBRARY} PROPERTIES
	POSITION_INDEPENDENT_CODE 1
	OUTPUT_NAME "filesystem_stdio"
	PREFIX "")

install( TARGETS ${FS_XASH_LIBRARY} DESTINATION ${LIB_INSTALL_DIR}/xash3d
        PERMISSIONS OWNER_READ OWNER_WRITE GROUP_READ WORLD_READ)


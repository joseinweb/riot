##########################################################################
# If not stated otherwise in this file or this component's LICENSE
# file the following copyright and licenses apply:
#
# Copyright 2023 RDK Management
#
# Licensed under the Apache License, Version 2.0 (the "License");
# you may not use this file except in compliance with the License.
# You may obtain a copy of the License at
#
# http://www.apache.org/licenses/LICENSE-2.0
#
# Unless required by applicable law or agreed to in writing, software
# distributed under the License is distributed on an "AS IS" BASIS,
# WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
# See the License for the specific language governing permissions and
# limitations under the License.
##########################################################################

cmake_minimum_required (VERSION 2.8.11)
set(CMAKE_VERBOSE_MAKEFILE ON)

project (Riot)

find_path(RTMESSAGE_API_HEADER NAMES rtMessage.h REQUIRED HINTS ${RBUS_INSTALL_DIR}/usr/include PATH_SUFFIXES rtmessage)

set(RTMESSAGE_API_HEADERS "")
if (NOT ${RTMESSAGE_API_HEADER} STREQUAL "RTMESSAGE_API_HEADER-NOTFOUND")
        message("Found rtMessage.h ${RTMESSAGE_API_HEADER}")
        set(RTMESSAGE_API_HEADERS ${RTMESSAGE_API_HEADER})

endif()

set(RTMESSAGE_LIBRARIES "")

find_library(RTMESSAGE_LIBRARY REQUIRED HINTS ${RBUS_INSTALL_DIR}/usr/lib NAMES rtMessage)
if (NOT ${RTMESSAGE_LIBRARY} STREQUAL "RTMESSAGE_LIBRARY-NOTFOUND")
        set(RTMESSAGE_LIBRARIES ${RTMESSAGE_LIBRARY})
endif()

add_subdirectory(riotclient)
add_subdirectory(riotdaemon)


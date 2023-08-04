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

set(MODULE_NAME RiotClient)
set(CMAKE_VERBOSE_MAKEFILE ON)


find_package(PkgConfig)

find_library(AVAHI_CLIENT_LIBRARIES NAMES avahi-client)
find_library(AVAHI_COMMON_LIBRARIES NAMES avahi-common)
mark_as_advanced(AVAHI_CLIENT_LIBRARIES)
mark_as_advanced(AVAHI_COMMON_LIBRARIES)

add_executable(${MODULE_NAME} 
        RIoTControl.cpp
        RIoTConnector.cpp
        AvahiClient.cpp)



set_target_properties(${MODULE_NAME} PROPERTIES 
        CXX_STANDARD 11
        CXX_STANDARD_REQUIRED YES)

target_include_directories(${MODULE_NAME} PRIVATE ../helpers ${RTMESSAGE_API_HEADERS})
target_link_libraries(${MODULE_NAME} PRIVATE ${RTMESSAGE_LIBRARIES} ${AVAHI_CLIENT_LIBRARIES} ${AVAHI_COMMON_LIBRARIES} pthread)

install(TARGETS ${MODULE_NAME} DESTINATION bin)

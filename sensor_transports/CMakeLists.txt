cmake_minimum_required(VERSION 3.12)
project(sensor_transports VERSION 0.0.1 LANGUAGES CXX)

set(CMAKE_C_STANDARD 11)
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_INSTALL_PREFIX "${CMAKE_CURRENT_BINARY_DIR}/install")

# Build for Debug, Release, NoOptimize
if(CMAKE_BUILD_TYPE MATCHES "Debug")
    # Debug
    set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -O0 -rdynamic -g -Wall -Wextra")
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -O0 -rdynamic -g -Wall -Wextra")
elseif(CMAKE_BUILD_TYPE MATCHES "Release")
    # Release
    set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -O2 -Wall -Wextra")
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -O2 -Wall -Wextra")
elseif(CMAKE_BUILD_TYPE MATCHES "NoOptimize")
    # Release
    set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -O0 -Wall -Wextra -fPIC")
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -O0 -Wall -Wextra -fPIC")
endif()

add_subdirectory(${CMAKE_SOURCE_DIR}/x86_bsp)

add_executable(${PROJECT_NAME}
    transport_main_server.cpp
    TransportManager.cpp
    Transporter.cpp
    CameraTransporter.cpp
    ImuTransporter.cpp
    GnssTransporter.cpp
    LidarTransporter.cpp
    RadarTransporter.cpp
)

target_include_directories(${PROJECT_NAME} PRIVATE ${CMAKE_SOURCE_DIR}/x86_bsp ${CMAKE_SOURCE_DIR}/x86_bsp/3rdparty)
target_link_libraries(${PROJECT_NAME} PRIVATE x86_bsp)
set_target_properties(${PROJECT_NAME} PROPERTIES INSTALL_RPATH "$ORIGIN/../lib")

install(TARGETS ${PROJECT_NAME}
  RUNTIME DESTINATION bin
  LIBRARY DESTINATION lib
)
cmake_minimum_required(VERSION 3.12)

project(x86_bsp VERSION 0.0.1 LANGUAGES CXX)

set(CMAKE_C_STANDARD 11)
set(CMAKE_CXX_STANDARD 17)


find_package(PkgConfig  REQUIRED)
pkg_check_modules(PC_ZMQ REQUIRED libzmq)
pkg_check_modules(PC_FFMPEG REQUIRED libavcodec libavformat libavutil libswscale)

set(SOURCES
  ArgParser.hpp
  RtpStreamer.cpp
  VideoEncoder.cpp
  zmqPublisher.cpp
  zmqSubscriber.cpp
)

add_library(${PROJECT_NAME} SHARED ${SOURCES})

set_target_properties(${PROJECT_NAME} PROPERTIES INSTALL_RPATH "$ORIGIN/../lib")

target_link_libraries(${PROJECT_NAME} PRIVATE ${PC_ZMQ_LIBRARIES} ${PC_FFMPEG_LIBRARIES})

target_include_directories(${PROJECT_NAME} PRIVATE
    $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/..>
    $<INSTALL_INTERFACE:include>
)

# 指定pkgconfig文件的内容
set(${PROJECT_NAME}_PC "${CMAKE_CURRENT_BINARY_DIR}/${PROJECT_NAME}.pc")

# 配置pkgconfig文件
configure_file(cmake/subProject.pc.in ${${PROJECT_NAME}_PC} @ONLY)

# 安装pkgconfig文件
install(FILES ${${PROJECT_NAME}_PC} DESTINATION lib/pkgconfig)


install(TARGETS ${PROJECT_NAME}
  RUNTIME DESTINATION bin
  LIBRARY DESTINATION lib
)

install(FILES
    ${CMAKE_CURRENT_SOURCE_DIR}/ArgParser.hpp
    ${CMAKE_CURRENT_SOURCE_DIR}/RtpStreamer.hpp
    ${CMAKE_CURRENT_SOURCE_DIR}/VideoEncoder.hpp
    ${CMAKE_CURRENT_SOURCE_DIR}/zmqPublisher.hpp
    ${CMAKE_CURRENT_SOURCE_DIR}/zmqSubscriber.hpp
    DESTINATION include/${CMAKE_PROJECT_NAME}
)




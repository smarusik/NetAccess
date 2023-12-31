cmake_minimum_required(VERSION 3.14)

project(NetAccess LANGUAGES CXX)

set(CMAKE_INCLUDE_CURRENT_DIR ON)

set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

find_package(QT NAMES Qt6 Qt5 COMPONENTS Core Network REQUIRED)
find_package(Qt${QT_VERSION_MAJOR} COMPONENTS Core Network REQUIRED)

add_executable(NetAccess
  main.cpp
  worker.h
  worker.cpp
  acceptor.h
  acceptor.cpp
  subject.cpp
  subject.h
  initiator.cpp
  initiator.h
  globals.h
  tokens/accesstoken.cpp
  tokens/accesstoken.h
  tokens/accesstokenbase.cpp
  tokens/accesstokenbase.h
  tokens/proxyaccesstoken.cpp
  tokens/proxyaccesstoken.h
  tokens/tokenattributes.h
  locations/locationattributes.h
  locations/autoserverlocation.cpp
  locations/autoserverlocation.h
  locations/locationattributes.cpp
)
target_link_libraries(NetAccess Qt${QT_VERSION_MAJOR}::Core Qt${QT_VERSION_MAJOR}::Network)

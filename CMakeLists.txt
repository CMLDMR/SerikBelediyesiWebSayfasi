cmake_minimum_required(VERSION 3.16)

project(SerikBelediyesiWebSayfasi LANGUAGES CXX)



set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)
set(CMAKE_AUTOUIC ON)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_STANDARD 20)

ADD_DEFINITIONS(-DQT_NO_KEYWORDS)

find_package(Qt6 6.2 COMPONENTS Core Gui Xml Widgets REQUIRED)

set(bsoncxx_DIR C:/Mongo/mongo-msvc2022x64-noBoost/lib/cmake/bsoncxx-3.7.0)
set(mongocxx_DIR C:/Mongo/mongo-msvc2022x64-noBoost/lib/cmake/mongocxx-3.7.0)

include_directories("${CMAKE_SOURCE_DIR}")
include_directories("C:/SerikBLDCoreRelease/MSVC2022x64noBoost/include/bilgiEdinme")

file(GLOB_RECURSE HEADERS ${CMAKE_CURRENT_SOURCE_DIR}/*.h)
file(GLOB_RECURSE SOURCES ${CMAKE_CURRENT_SOURCE_DIR}/*.cpp)


add_executable(SerikBelediyesiWebSayfasi
    ${HEADERS}
    ${SOURCES}
)

target_include_directories(SerikBelediyesiWebSayfasi PUBLIC "C:/Mongo/mongo-msvc2022x64-noBoost/include")
target_link_directories(SerikBelediyesiWebSayfasi PUBLIC "C:/Mongo/mongo-msvc2022x64-noBoost/lib")


target_include_directories(SerikBelediyesiWebSayfasi PUBLIC "C:/SerikBLDCoreRelease/MSVC2022x64noBoost/include")
target_link_directories(SerikBelediyesiWebSayfasi PUBLIC "C:/SerikBLDCoreRelease/MSVC2022x64noBoost/lib")

target_link_libraries(SerikBelediyesiWebSayfasi
    PRIVATE Qt6::Core Qt6::Gui Qt::Xml Qt::Widgets SerikBLDCore)


find_package(mongocxx REQUIRED)
find_package(bsoncxx REQUIRED)
include_directories(${LIBMONGOCXX_INCLUDE_DIR})
include_directories(${LIBBSONCXX_INCLUDE_DIR})

target_link_libraries(SerikBelediyesiWebSayfasi PRIVATE mongo::bsoncxx_shared)
target_link_libraries(SerikBelediyesiWebSayfasi PRIVATE mongo::mongocxx_shared)


set(Wt_DIR C:/Wt_4.7.0_msvs2022_x64/lib/cmake/wt)
set(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} "C:/Wt_4.7.0_msvs2022_x64/lib/cmake/wt")
FIND_PACKAGE( Wt REQUIRED )
#include_directories(${Wt_INCLUDE_DIR})
target_include_directories(SerikBelediyesiWebSayfasi PUBLIC "C:/Wt_4.7.0_msvs2022_x64/include")
target_link_directories(SerikBelediyesiWebSayfasi PUBLIC "C:/Wt_4.7.0_msvs2022_x64/lib")
target_link_libraries(SerikBelediyesiWebSayfasi PRIVATE Wt WtHttp)
set_target_properties(${PROJECT_NAME} PROPERTIES LINK_FLAGS "/ignore:4251")


include_directories(${CMAKE_CURRENT_SOURCE_DIR}/../../Comman)






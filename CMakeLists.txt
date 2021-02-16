cmake_minimum_required(VERSION 3.9 FATAL_ERROR)
project(ProcessManager VERSION 0.1 DESCRIPTION "ProcessManager" LANGUAGES C CXX)

enable_testing()

set(DEFAULT_BUILD_TYPE "Debug")
set(CMAKE_BUILD_TYPE "Debug")
set(TARGET_NAME "ProcessManager")

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} ${FLAGS_COMMON} -O1")

set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)
set(CMAKE_AUTOUIC ON)

file(MAKE_DIRECTORY "${CMAKE_BINARY_DIR}/bin")
file(MAKE_DIRECTORY "${CMAKE_BINARY_DIR}/bin/${CMAKE_BUILD_TYPE}")
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/bin")
configure_file(config.json ${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/${CMAKE_BUILD_TYPE}/config.json)
configure_file(config.json ${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/${CMAKE_BUILD_TYPE}/config.json)
configure_file(config.json ${CMAKE_BINARY_DIR}/bin/${CMAKE_BUILD_TYPE}/config.json)

if (WIN32)
	list(APPEND CMAKE_PREFIX_PATH "E:\\repo\\opencvBuild")
	list(APPEND CMAKE_PREFIX_PATH "E:\\Qt\\5.12.8\\msvc2017_64")
	list(APPEND CMAKE_PREFIX_PATH "E:\\repo\\GeneticOptimization/ConfigReader")
    list(APPEND CMAKE_PREFIX_PATH "E:\\repo\\GeneticOptimization\\PostProcessingModules")
endif (WIN32)



set(THREADS_PREFER_PTHREAD_FLAG ON)
find_package(Threads REQUIRED)
find_package(Qt5Core REQUIRED)
find_package(OpenCV REQUIRED)
find_package(Qt5Test REQUIRED)
find_package(Qt5Widgets REQUIRED)
find_package(Qt5Network REQUIRED)
include_directories(${OpenCV_INCLUDE_DIRS})

add_subdirectory(IncludeSpdlog)
add_subdirectory(ConfigReader)


set(SRC
  src/main.cc
  src/processmanager.cc
  src/process.cc)

set(HEADERS
  src/processmanager.h
  src/process.h)
  
add_executable(${TARGET_NAME} ${SRC} ${HEADERS})

target_link_libraries(${PROJECT_NAME} IncludeSpdlog ConfigReader)
target_link_libraries(${PROJECT_NAME} ${QT_LIBRARIES} ${OpenCV_LIBS} Threads::Threads)
target_link_libraries(${PROJECT_NAME} Qt5::Network Qt5::Widgets)



message("CMAKE_BUILD_TYPE: ${CMAKE_BUILD_TYPE} ")
if (CMAKE_BUILD_TYPE STREQUAL "Debug" AND WIN32)
	set (CMAKE_LIBRARY_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR})
	set (CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR})
endif()

if (CMAKE_BUILD_TYPE STREQUAL "Release" AND WIN32)
	set (CMAKE_LIBRARY_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR})
	set (CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR})
endif()

if (WIN32)
    get_target_property(QT5_QMAKE_EXECUTABLE Qt5::qmake IMPORTED_LOCATION)
    get_filename_component(QT5_WINDEPLOYQT_EXECUTABLE ${QT5_QMAKE_EXECUTABLE} PATH)
    set(QT5_WINDEPLOYQT_EXECUTABLE "${QT5_WINDEPLOYQT_EXECUTABLE}/windeployqt.exe")

    add_custom_command(TARGET ${PROJECT_NAME} POST_BUILD
       COMMAND ${QT5_WINDEPLOYQT_EXECUTABLE} --qmldir ${CMAKE_SOURCE_DIR} $<TARGET_FILE_DIR:${PROJECT_NAME}>)
endif(WIN32)
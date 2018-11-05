cmake_minimum_required(VERSION 2.8.3)
project(roco318_project)
add_compile_options(-std=c++11)
 
set(EXAMPLE_FILES 
    contours2.cpp)
  

find_package(catkin REQUIRED COMPONENTS          
)

find_package(OpenCV REQUIRED)

MESSAGE("Version: ${OpenCV_VERSION}")


catkin_package(CATKIN_DEPENDS)



include_directories(
${catkin_INCLUDE_DIRS}
)

foreach(EXAMPLE_FILE ${EXAMPLE_FILES})

  get_filename_component(SOURCE_NAME ${EXAMPLE_FILE} NAME_WE)
  
  if (UNIX AND NOT APPLE)
    set(EXE_NAME "${SOURCE_NAME}.bin")
  elseif (WIN32)
    set(EXE_NAME "${SOURCE_NAME}")
  endif ()

  message(STATUS "Adding Example ${EXE_NAME}")
  message(STATUS "Adding Example FILE ${EXAMPLE_FILE}")
  add_executable(${SOURCE_NAME} ${EXAMPLE_FILE})
  
  target_link_libraries(${SOURCE_NAME} ${catkin_LIBRARIES} ${OpenCV_LIBS})
  message(STATUS "Successfully targeted link libraries") 

add_dependencies(${SOURCE_NAME} ${catkin_EXPORTED_TARGETS})


endforeach()

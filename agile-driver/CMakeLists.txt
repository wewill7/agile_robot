cmake_minimum_required(VERSION 2.8.3)
project(agile_driver)

add_definitions(-std=c++11)
# Load catkin and all dependencies required for this package
find_package(catkin REQUIRED COMPONENTS
  cmake_modules
  class_loader
  agile_foundation
)

# Include a custom cmake file for TinyXML
find_package(TinyXML REQUIRED)
find_package(Eigen3  REQUIRED)

# Declare a catkin package
catkin_package(
  LIBRARIES
    controlcan
  INCLUDE_DIRS
    include
  DEPENDS
    TinyXML
    class_loader
)

###########
## Build ##
###########

set(THIRDPARTY_ROOT     thirdparty)

set(CAN_LIB_ROOT        ${THIRDPARTY_ROOT}/controlcan)
# Copy the libcontrolcan.so into devel/lib directory.
execute_process(COMMAND cp
  ${PROJECT_SOURCE_DIR}/${CAN_LIB_ROOT}/lib/libcontrolcan.so
  ${CATKIN_DEVEL_PREFIX}/${CATKIN_GLOBAL_LIB_DESTINATION}
)
# execute_process(COMMAND echo xxxxxxx${CMAKE_LIBRARY_PATH})

# Build
include_directories(
  include
  ${CAN_LIB_ROOT}/include

  ${catkin_INCLUDE_DIRS}
  ${TinyXML_INCLUDE_DIRS}
  ${EIGEN3_INCLUDE_DIRS}
)

# Add the thirdparty lib path.
link_directories(
  ${CAN_LIB_ROOT}/lib
)

set(SRC_ROOT            src)
set(SRC_TEST_ROOT       test)

set(SRC_SYS             ${SRC_ROOT})
set(SRC_SYS_PLATFORM    ${SRC_SYS}/platform)
set(SRC_SYS_REP         ${SRC_ROOT}/repository)

# Files define.
set(SYS_FLATFORM_FILES  ${SRC_SYS_PLATFORM}/log_robot.cpp

                        ${SRC_SYS_PLATFORM}/sw_node/sw_node.cpp
                        ${SRC_SYS_PLATFORM}/sw_node/leg_node.cpp
                        ${SRC_SYS_PLATFORM}/sw_node/motor_node.cpp
                        ${SRC_SYS_PLATFORM}/sw_node/power_node.cpp
                        ${SRC_SYS_PLATFORM}/sw_node/test_node.cpp
                        ${SRC_SYS_PLATFORM}/sw_node/pd_node.cpp
                        
                        ${SRC_SYS_PLATFORM}/propagate/propagate.cpp
                        ${SRC_SYS_PLATFORM}/propagate/can_usb.cpp
                        ${SRC_SYS_PLATFORM}/propagate/motor_can.cpp
                        ${SRC_SYS_PLATFORM}/propagate/leg_can.cpp
                        ${SRC_SYS_PLATFORM}/propagate/gazebo_propagate.cpp
                        ${SRC_SYS_PLATFORM}/propagate/fake_can.cpp
)

set(SYS_REP_FILES       ${SRC_SYS_REP}/imu_sensor.cpp
                        ${SRC_SYS_REP}/joint.cpp
                        ${SRC_SYS_REP}/motor.cpp
                        ${SRC_SYS_REP}/power.cpp
                        ${SRC_SYS_REP}/force_sensor.cpp
                        ${SRC_SYS_REP}/joint_manager.cpp
)

set(SYS_ROBOT_FILES     ${SRC_SYS_PLATFORM}/sw_node/sw_node_manager.cpp
                        ${SRC_SYS_PLATFORM}/propagate/propagate_manager.cpp
                        ${SRC_SYS_PLATFORM}/master.cpp
                        ${SRC_ROOT}/mii_robot.cpp
)

set(SYS_TEST_FILES      ${SRC_TEST_ROOT}/main.cpp)

## Declare a C++ library
add_library(${PROJECT_NAME}_sys_platform
  ${SYS_FLATFORM_FILES}
)

add_library(${PROJECT_NAME}_repository
  ${SYS_REP_FILES}
)

add_library(${PROJECT_NAME}_sys_robot
  ${SYS_ROBOT_FILES}
)

## Specify libraries to link a library or executable target against
target_link_libraries(${PROJECT_NAME}_sys_platform
  ${catkin_LIBRARIES}
  ${EIGEN3_LIBRARIES}
  agile_foundation
  agile_foundation_toolbox
  controlcan
  # pcanbasic
)

target_link_libraries(${PROJECT_NAME}_repository
  ${catkin_LIBRARIES}
  agile_foundation
)

target_link_libraries(${PROJECT_NAME}_sys_robot
  ${catkin_LIBRARIES}
  ${PROJECT_NAME}_sys_platform
  ${PROJECT_NAME}_repository
  agile_foundation
  agile_foundation_toolbox
)

#############
## Install ##
#############

install(DIRECTORY include/
  DESTINATION ${CATKIN_PACKAGE_INCLUDE_DESTINATION})

install(TARGETS ${PROJECT_NAME}_sys_platform
                ${PROJECT_NAME}_sys_robot
  ARCHIVE DESTINATION ${CATKIN_PACKAGE_LIB_DESTINATION}
  LIBRARY DESTINATION ${CATKIN_PACKAGE_LIB_DESTINATION}
  RUNTIME DESTINATION ${CATKIN_PACKAGE_BIN_DESTINATION}
)

foreach(dir images config)
  install(DIRECTORY ${dir}/
    DESTINATION ${CATKIN_PACKAGE_SHARE_DESTINATION}/${dir})
endforeach(dir)

#############
## Testing ##
#############

## Add gtest based cpp test target and link libraries
catkin_add_gtest(${PROJECT_NAME}_test
  ${SYS_TEST_FILES}
)

if(TARGET ${PROJECT_NAME}-test)
  target_link_libraries(${PROJECT_NAME}_test
    ${PROJECT_NAME}_sys_robot
  )
endif()

## Add folders to be run by python nosetests
# catkin_add_nosetests(test)
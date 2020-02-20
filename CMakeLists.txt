cmake_minimum_required(VERSION 3.1)

project(cognitao-examples VERSION 1.0.4)

#
# Set to 'On' for non-nightly version build (stable version X.Y.Z)
# When set to 'Off' - build packaged as X.Y.Z-nightly version
#
set(RELEASE_VERSION On)

add_compile_options(-std=c++11)

#
# Find CogniTao cmake package
#
find_package(cognitao REQUIRED)

#
# Make doc target - generates html API reference using doxygen
#
if(${CMAKE_VERSION} VERSION_GREATER "3.8.0") 
    add_custom_target(docs 
    ${CMAKE_COMMAND} -E env 
        PROJECT_VERSION=${PROJECT_VERSION}$<IF:$<BOOL:${RELEASE_VERSION}>,"","-nightly"> 
        "doxygen" "${CMAKE_SOURCE_DIR}/Doxyfile"
    WORKING_DIRECTORY ${CMAKE_SOURCE_DIR}
    )
endif()

#
# Build
#

# Set default build type to 'Release'
if(NOT CMAKE_BUILD_TYPE AND NOT CMAKE_CONFIGURATION_TYPES)
  set(DEFAULT_BUILD_TYPE "Release")
  message(STATUS "Setting build type to '${DEFAULT_BUILD_TYPE}' as none was specified.")
  set(CMAKE_BUILD_TYPE "${DEFAULT_BUILD_TYPE}" CACHE
      STRING "Choose the type of build." FORCE)
  # Set the possible values of build type for cmake-gui
  set_property(CACHE CMAKE_BUILD_TYPE PROPERTY STRINGS
    "Debug" "Release" "MinSizeRel" "RelWithDebInfo")
endif()

include_directories("c:/Program Files/CogniTao/include")

#
# Add your code
#
add_executable(cognitao_example_xml_machine_loader 
  src/example_xml_machine_loader.cpp
)

add_executable(cognitao_example_state_machine
  src/example_state_machine.cpp
)

add_executable(cognitao_example_state_machine_turnstile
  src/example_state_machine_turnstile.cpp
)

add_executable(cognitao_example_behavior_tree_simple
  src/example_behavior_tree_simple.cpp
) 

add_executable(cognitao_example_behavior_tree_trigger
  src/example_behavior_tree_trigger.cpp
)

add_executable(cognitao_example_behavior_tree_user_action
  src/example_behavior_tree_user_action.cpp
)

add_executable(cognitao_example_behavior_tree_runner
  src/example_behavior_tree_runner.cpp
)

add_library(cognitao_example_runner_plugin SHARED
    src/example_runner_plugin
)

add_library(cognitao_example_datasource_plugin SHARED
    src/example_datasource_plugin
)
#
# Link cognitao c++ core library
#
target_link_libraries(cognitao_example_xml_machine_loader
  CogniTao::cognitao_core
)

target_link_libraries(cognitao_example_state_machine
  CogniTao::cognitao_core
)

target_link_libraries(cognitao_example_state_machine_turnstile
  CogniTao::cognitao_core
)

target_link_libraries(cognitao_example_behavior_tree_simple
  CogniTao::cognitao_core
)

target_link_libraries(cognitao_example_behavior_tree_user_action
  CogniTao::cognitao_core
)

target_link_libraries(cognitao_example_behavior_tree_runner
  CogniTao::cognitao_core
)

target_link_libraries(cognitao_example_runner_plugin
  CogniTao::cognitao_core
)

target_link_libraries(cognitao_example_datasource_plugin
  CogniTao::cognitao_core
)

target_link_libraries(cognitao_example_behavior_tree_trigger
  CogniTao::cognitao_core
)

#
# Install
# 
if(WIN32)
  #
  # Windows install paths
  # 
  set(CMAKE_INSTALL_PREFIX ".")
  set(INSTALL_LIB_DIR ${CMAKE_INSTALL_PREFIX})
  set(INSTALL_BIN_DIR ${CMAKE_INSTALL_PREFIX})
  set(INSTALL_SHARE_DIR ${CMAKE_INSTALL_PREFIX})
else()
  #
  # Linux install paths
  # 
  set(CMAKE_INSTALL_PREFIX "/usr")
  set(INSTALL_LIB_DIR ${CMAKE_INSTALL_PREFIX}/lib)
  set(INSTALL_BIN_DIR ${CMAKE_INSTALL_PREFIX}/bin/)
  set(INSTALL_SHARE_DIR ${CMAKE_INSTALL_PREFIX}/share/${PROJECT_NAME})
endif()

set(INSTALL_INCLUDE_DIR ${CMAKE_INSTALL_PREFIX}/include/${PROJECT_NAME})
set(INSTALL_CMAKE_DIR ${CMAKE_INSTALL_PREFIX}/lib/cmake/${PROJECT_NAME})

# CogniTao Examples
install(TARGETS cognitao_example_xml_machine_loader 
    cognitao_example_state_machine 
    cognitao_example_state_machine_turnstile
    cognitao_example_behavior_tree_simple
    cognitao_example_behavior_tree_user_action 
    cognitao_example_behavior_tree_runner 
    cognitao_example_runner_plugin
    cognitao_example_datasource_plugin 
    cognitao_example_behavior_tree_trigger EXPORT CogniTaoExamplesExports
  ARCHIVE DESTINATION ${INSTALL_LIB_DIR}
  LIBRARY DESTINATION ${INSTALL_LIB_DIR}
  RUNTIME DESTINATION ${INSTALL_BIN_DIR}
)

# Source files
install(
  DIRECTORY ${CMAKE_SOURCE_DIR}/src/
  DESTINATION ${INSTALL_SHARE_DIR}/sources
  FILES_MATCHING PATTERN "*"
)

if(WIN32)
  # MinGw64 windows dlls
  install(
    DIRECTORY ${CMAKE_SOURCE_DIR}/libs/mingw64/
    DESTINATION ${INSTALL_LIB_DIR}
    FILES_MATCHING PATTERN "*.dll"
  )
endif()

#
# CMake package configuration
#

include(CMakePackageConfigHelpers)

write_basic_package_version_file(
  "${CMAKE_CURRENT_BINARY_DIR}/${PROJECT_NAME}/${PROJECT_NAME}-version.cmake"
  VERSION ${PACKAGE_VERSION}
  COMPATIBILITY AnyNewerVersion
)

export(EXPORT CogniTaoExamplesExports
  FILE "${CMAKE_CURRENT_BINARY_DIR}/${PROJECT_NAME}/${PROJECT_NAME}-targets.cmake"
  NAMESPACE CogniTaoExamples::
)

configure_file(cmake/${PROJECT_NAME}-config.cmake
  "${CMAKE_CURRENT_BINARY_DIR}/${PROJECT_NAME}/${PROJECT_NAME}-config.cmake"
  COPYONLY
)

install(EXPORT CogniTaoExamplesExports
  FILE
    ${PROJECT_NAME}-targets.cmake
  NAMESPACE
    CogniTaoExamples::
  DESTINATION
    ${INSTALL_CMAKE_DIR}
)
install(
  FILES
    cmake/${PROJECT_NAME}-config.cmake
    "${CMAKE_CURRENT_BINARY_DIR}/${PROJECT_NAME}/${PROJECT_NAME}-version.cmake"
  DESTINATION
    ${INSTALL_CMAKE_DIR}
)

#
# Make Uninstall
#
if(NOT TARGET uninstall)
  configure_file(
    "${CMAKE_CURRENT_SOURCE_DIR}/cmake/cmake_uninstall.cmake"
    "${CMAKE_CURRENT_BINARY_DIR}/cmake/cmake_uninstall.cmake"
    IMMEDIATE @ONLY)

  add_custom_target(uninstall
    COMMAND ${CMAKE_COMMAND} -P ${CMAKE_CURRENT_BINARY_DIR}/cmake/cmake_uninstall.cmake)
endif()

#
# CPack (deb packaging)
#
if (EXISTS "${CMAKE_ROOT}/Modules/CPack.cmake")
  include(InstallRequiredSystemLibraries)

  if (${CMAKE_CROSSCOMPILING})
    message("-- [!] Cross compiling for ${CMAKE_SYSTEM_PROCESSOR}")
    set(CPACK_DEBIAN_PACKAGE_ARCHITECTURE ${CMAKE_SYSTEM_PROCESSOR})
  else()
    execute_process(COMMAND "dpkg" "--print-architecture"
      OUTPUT_VARIABLE CPACK_DEBIAN_PACKAGE_ARCHITECTURE
      OUTPUT_STRIP_TRAILING_WHITESPACE)
  endif()

  execute_process(COMMAND "lsb_release" "-sc"
    OUTPUT_VARIABLE CPACK_DEBIAN_PACKAGE_DISTRONAME
    OUTPUT_STRIP_TRAILING_WHITESPACE
  )
  
  # SET(CPACK_SET_DESTDIR "on")
  
  if (WIN32)
    # Windows
    set(CPACK_GENERATOR "NSIS64")
    set(CPACK_SYSTEM_NAME ${CPACK_DEBIAN_PACKAGE_ARCHITECTURE}-installer)
  else()
    # Linux
    set(CPACK_GENERATOR "DEB")
    set(CPACK_SYSTEM_NAME ${CPACK_DEBIAN_PACKAGE_DISTRONAME}_${CPACK_DEBIAN_PACKAGE_ARCHITECTURE})
  endif()

  message(STATUS "Build target system name: '${CMAKE_SYSTEM_NAME}'")
  message(STATUS "Packaging tool: ${CPACK_GENERATOR}")

  set(CPACK_NSIS_ENABLE_UNINSTALL_BEFORE_INSTALL ON)
  set(CPACK_NSIS_MODIFY_PATH ON)
  set(CPACK_PACKAGE_DESCRIPTION "CogniTAO Decision Making Engine by Cogniteam")
  set(CPACK_PACKAGE_DESCRIPTION_SUMMARY "Decision makeing engine SDK & Runtime")
  set(CPACK_PACKAGE_VENDOR "Cogniteam")
  set(CPACK_PACKAGE_CONTACT "info@cogniteam.com")
  set(CPACK_NSIS_CONTACT "info@cogniteam.com")
  set(CPACK_PACKAGE_VERSION_MAJOR ${PROJECT_VERSION_MAJOR})
  set(CPACK_PACKAGE_VERSION_MINOR ${PROJECT_VERSION_MINOR})
  set(CPACK_DEBIAN_PACKAGE_DEPENDS " cognitao ")

  if (RELEASE_VERSION) 
    set(CPACK_PACKAGE_VERSION_PATCH "${PROJECT_VERSION_PATCH}")
    set(CPACK_NSIS_PACKAGE_NAME "CogniTao")
    set(CPACK_NSIS_DISPLAY_NAME "CogniTao")
    set(CPACK_PACKAGE_INSTALL_DIRECTORY "CogniTao")
    message(STATUS "Release build")
  else()
    set(CPACK_DEBIAN_PACKAGE_NAME "${PROJECT_NAME}-nightly")
    set(CPACK_NSIS_PACKAGE_NAME "${PROJECT_NAME} Nightly")
    set(CPACK_NSIS_DISPLAY_NAME "${PROJECT_NAME} Nightly")
    set(CPACK_PACKAGE_INSTALL_DIRECTORY "${PROJECT_NAME} Nightly")
    set(CPACK_PACKAGE_VERSION_PATCH "${PROJECT_VERSION_PATCH}-nightly")
    message(STATUS "Development build")
  endif()
  
  set(CPACK_DEBIAN_PACKAGE_PRIORITY "optional")
  set(CPACK_DEBIAN_PACKAGE_SECTION "dev")

  include(CPack)
  
ENDIF(EXISTS "${CMAKE_ROOT}/Modules/CPack.cmake")

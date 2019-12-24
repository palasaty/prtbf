cmake_minimum_required(VERSION 3.7)

set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

set (SUPERBUILD ON CACHE BOOL "Initial pull and build of all dependent libraries/executables")

if(NOT CMAKE_BUILD_TYPE)
  message(STATUS "Setting build type to 'Release' as none was specified.")
  set(CMAKE_BUILD_TYPE "Release" CACHE STRING "Choose the type of build." FORCE)
endif()

if(SUPERBUILD)

  set(CMAKE_INSTALL_PREFIX ${CMAKE_BINARY_DIR}/install CACHE PATH "Install location")
  project(ComputationalLifeProtocol-SuperBuild)
  include(ExternalProject)
  include(CMakeDetermineSystem)
  
  if(MSVC OR XCode)
  # For multi configuration IDE environments start with release
  set(CMAKE_CONFIGURATION_TYPES Release CACHE STRING INTERNAL FORCE )
endif()

  set(CMAKE_GENERATION
       CMAKE_GENERATOR ${CMAKE_GENERATOR}
       CMAKE_GENERATOR_PLATFORM ${CMAKE_GENERATOR_PLATFORM}
       CMAKE_GENERATOR_TOOLSET ${CMAKE_GENERATOR_TOOLSET}
       CMAKE_GENERATOR_INSTANCE ${CMAKE_GENERATOR_INSTANCE}
  )

  message( STATUS "External project - protobuf" )
  set(protobuf_URL "https://github.com/protocolbuffers/protobuf/releases/download/v3.11.1/protobuf-all-3.11.1.zip")
  set(protobuf_MD5 "01f0e58bb432727d494b390e62f877d5" )
  set(protobuf_SRC "${CMAKE_BINARY_DIR}/protobuf/src/protobuf")
  set(protobuf_DIR "${CMAKE_BINARY_DIR}/protobuf/install")

  ExternalProject_Add( protobuf
    PREFIX protobuf
    URL ${protobuf_URL}
    URL_MD5 ${protobuf_MD5}
    DOWNLOAD_DIR ${protobuf_SRC}
    SOURCE_SUBDIR ./cmake
    ${CMAKE_GENERATION}
    CMAKE_ARGS
      -DCMAKE_TOOLCHAIN_FILE:FILE=${CMAKE_TOOLCHAIN_FILE}
      -DBUILD_SHARED_LIBS:BOOL=OFF
      -DCMAKE_BUILD_TYPE:STRING=${CMAKE_BUILD_TYPE}
      -DCMAKE_INSTALL_PREFIX:STRING=${protobuf_DIR}
      -Dprotobuf_BUILD_TESTS:BOOL=OFF
      -Dprotobuf_BUILD_EXAMPLES:BOOL=OFF
      -Dprotobuf_BUILD_SHARED_LIBS:BOOL=OFF
      -Dprotobuf_MSVC_STATIC_RUNTIME:BOOL=OFF#Don't change MSVC runtime settings (/MD or /MT)
      -Dprotobuf_WITH_ZLIB:BOOL=OFF
  )

  # ExternalProject_Add doesn't like to work with lists: it keeps only the first element
  string(REPLACE ";" "::" CMAKE_PREFIX_PATH "${CMAKE_PREFIX_PATH}")
  # Generate the ComputationalLife project after dependencies have been built
  ExternalProject_Add( GeneratorConfig
    PREFIX GeneratorConfig
    DEPENDS protobuf
    DOWNLOAD_COMMAND ""
    DOWNLOAD_DIR ${CMAKE_SOURCE_DIR}
    SOURCE_DIR ${CMAKE_SOURCE_DIR}
    BINARY_DIR ${CMAKE_BINARY_DIR}/ProtocolExample
    ${CMAKE_GENERATION}
    CMAKE_ARGS
      -DCMAKE_TOOLCHAIN_FILE:FILE=${CMAKE_TOOLCHAIN_FILE}
      -DCMAKE_PREFIX_PATH:STRING=${CMAKE_PREFIX_PATH}
      -DCMAKE_INSTALL_PREFIX:STRING=${CMAKE_INSTALL_PREFIX}
      -DCMAKE_BUILD_TYPE:STRING=${CMAKE_BUILD_TYPE}
      -DBUILD_SHARED_LIBS:BOOL=${BUILD_SHARED_LIBS}
      -DSUPERBUILD:BOOL=OFF
      -Dprotobuf_SRC:PATH=${protobuf_SRC}
      -Dprotobuf_DIR:PATH=${protobuf_DIR}
  )
else()
  set_property(GLOBAL PROPERTY USE_FOLDERS ON)
  
  project(ProtocolExample)
  
  # Add Protobuf to our Project
  if(NOT protobuf_SRC)
    set(protobuf_SRC ${CMAKE_BINARY_DIR}/../protobuf/src/protobuf)
  endif()
  set(protobuf_Header ${protobuf_SRC}/src/google/protobuf/package_info.h)
  # Proto headers should have been installed here by the outer build
  if(NOT EXISTS ${protobuf_Header})
    message(FATAL_ERROR "I cannot find protobuf source, please set protobuf_SRC to its root directory")
  endif()
  if(NOT protobuf_DIR)
    set(protobuf_DIR ${CMAKE_BINARY_DIR}/../protobuf/install)
  endif()
  list(APPEND CMAKE_PREFIX_PATH ${protobuf_DIR})
  # Settings for protobuf configuration
  set(protobuf_BUILD_PROTOC_BINARIES OFF CACHE BOOL INTERNAL FORCE)
  set(protobuf_BUILD_SHARED_LIBS OFF CACHE BOOL INTERNAL FORCE)
  set(protobuf_MSVC_STATIC_RUNTIME OFF CACHE BOOL INTERNAL FORCE)#Use our MSVC runtime settings (/MD or /MT)
  set(protobuf_BUILD_TESTS OFF CACHE BOOL INTERNAL FORCE)
  set(protobuf_BUILD_EXAMPLES OFF CACHE BOOL INTERNAL FORCE)
  add_subdirectory("${protobuf_SRC}/cmake" "${protobuf_SRC}-build")
  set_target_properties (libprotobuf libprotobuf-lite PROPERTIES FOLDER protobufs)
  
  set(BINDINGS ${CMAKE_SOURCE_DIR})
  include(GenerateBindings.cmake)
  add_custom_target(GenerateBindings ALL
                    COMMAND ${CMAKE_COMMAND} -Dprotobuf_DIR:STRING=${protobuf_DIR} -DBINDINGS:STRING=${BINDINGS}
                                           -P ${CMAKE_SOURCE_DIR}/GenerateBindings.cmake)

  set(SOURCE "${CMAKE_SOURCE_DIR}/main.cpp"
             "${CMAKE_SOURCE_DIR}/ComputationalLifeProtocol.pb.h"
             "${CMAKE_SOURCE_DIR}/ComputationalLifeProtocol.pb.cc"
             "${CMAKE_SOURCE_DIR}/ComputationalLifeProtocol.proto")

  add_executable(ProtocolGenerator ${SOURCE})
  add_dependencies(ProtocolGenerator GenerateBindings)
  target_link_libraries(ProtocolGenerator libprotobuf)
  target_include_directories(ProtocolGenerator PUBLIC ${protobuf_DIR}/include)

  if(MSVC) # Configure running executable out of MSVC
    set_property(TARGET ProtocolGenerator PROPERTY VS_DEBUGGER_WORKING_DIRECTORY "${CMAKE_SOURCE_DIR}/examples")
  endif()

endif()
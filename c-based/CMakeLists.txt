cmake_minimum_required(VERSION 2.8)
project(protobuf-c-demo )
set(CMAKE_BUILD_TYPE Debug)
include(ExternalProject)

set(GLOBAL_OUTPUT_PATH ${PROJECT_BINARY_DIR}/bin)
set(CMAKE_INSTALL_PREFIX ${PROJECT_BINARY_DIR}/install)

set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${GLOBAL_OUTPUT_PATH})
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${GLOBAL_OUTPUT_PATH})
set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${GLOBAL_OUTPUT_PATH})

foreach(OUTPUTCONFIG ${CMAKE_CONFIGURATION_TYPES})
    string(TOUPPER ${OUTPUTCONFIG} OUTPUTCONFIG)
    set(CMAKE_RUNTIME_OUTPUT_DIRECTORY_${OUTPUTCONFIG} ${GLOBAL_OUTPUT_PATH})
    set(CMAKE_LIBRARY_OUTPUT_DIRECTORY_${OUTPUTCONFIG} ${GLOBAL_OUTPUT_PATH})
    set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY_${OUTPUTCONFIG} ${GLOBAL_OUTPUT_PATH})
endforeach(OUTPUTCONFIG CMAKE_CONFIGURATION_TYPES)

set (3RDPARTY ${CMAKE_SOURCE_DIR}/3rdparty)
set (PROTOBUF_C ${3RDPARTY}/protobuf-c)
set (JANSON ${3RDPARTY}/janson)
set (PROTO2JSON ${3RDPARTY}/proto2json)

function(generate_bindings)
	set (PROTO_PATH "${CMAKE_SOURCE_DIR}/proto")
	file(GLOB PROTO_FILES ${PROTO_PATH}/*.proto)
	foreach(file ${PROTO_FILES})
		set(PROTO_ARGS  protoc-c --proto_path=${PROTO_PATH}  --c_out=${PROTO_PATH} ${file})
        	execute_process(COMMAND  ${PROTO_ARGS}
                	WORKING_DIRECTORY ${PROJECT_SOURCE_DIR}
                	RESULT_VARIABLE ret
                	OUTPUT_VARIABLE res )
        	if(ret EQUAL "1")
                	message ("" ${res})
                	message (FATAL_ERROR "Bad exit status")
		else()
			message ("Successfully generate c-bindings for ${file}")
        	endif()
	endforeach()
endfunction()

function (download_protobuf_lib)
	message ("Start download protobuf-c")
	set_property(DIRECTORY PROPERTY EP_STEP_TARGETS download build)
  	ExternalProject_Add(protobuf-c
    		URL    https://github.com/protobuf-c/protobuf-c/archive/master.zip
		##PREFIX ${PROTOBUF_C} 
		#UPDATE_COMMAND cp ${CMAKE_SOURCE_DIR}/patch/protobuf-build ${PROTOBUF_C}/build-cmake/CMakeLists.txt
		SOURCE_DIR ${PROTOBUF_C}
		INSTALL_DIR ${GLOBAL_OUTPUT_PATH}
		BINARY_DIR ${GLOBAL_OUTPUT_PATH}
		SOURCE_SUBDIR build-cmake
		CMAKE_ARGS  -DCMAKE_INSTALL_PREFIX=${CMAKE_INSTALL_PREFIX}
  	)	
	message ("Successfully downloaded")
endfunction()

function (download_json_lib)
        message ("Start download janson")
	set_property(DIRECTORY PROPERTY EP_STEP_TARGETS download build)
        ExternalProject_Add(janson
                URL  https://github.com/akheron/jansson/archive/master.zip  
		##PREFIX ${JANSON}
		CMAKE_ARGS -DJANSSON_BUILD_DOCS=OFF -DCMAKE_INSTALL_PREFIX=${CMAKE_INSTALL_PREFIX}
		SOURCE_DIR ${JANSON}
		INSTALL_DIR ${GLOBAL_OUTPUT_PATH}
		BINARY_DIR ${GLOBAL_OUTPUT_PATH}
        )
        message ("Successfully downloaded")

endfunction()

function (download_convert_lib)
	message ("Start download convert lib")
	##set (PROTO2JSON_SRC ${PROTO2JSON}/src/) 
	set_property(DIRECTORY PROPERTY EP_STEP_TARGETS download configure build)
	ExternalProject_Get_Property(protobuf-c install_dir)
	include_directories(${install_dir}/include)

	ExternalProject_Add(proto2json
		DEPENDS janson protobuf-c
                URL  https://github.com/Sannis/protobuf2json-c/archive/master.zip
		##PREFIX ${PROTO2JSON}
		SOURCE_DIR ${PROTO2JSON}
		INSTALL_DIR ${GLOBAL_OUTPUT_PATH}
		##BINARY_DIR ${GLOBAL_OUTPUT_PATH}
		#PATCH_COMMAND patch -p1 <${CMAKE_SOURCE_DIR}/proto2json.patch
		UPDATE_COMMAND cp ${CMAKE_SOURCE_DIR}/patch/protobuf2json.c ${PROTO2JSON}/src 
		CONFIGURE_COMMAND ${PROTO2JSON}/configure --with-protobuf-c=${PROJECT_BINARY_DIR}/install/
		BUILD_COMMAND make
		INSTALL_COMMAND make install
		BUILD_IN_SOURCE 1
        )
	ExternalProject_Add_Step(proto2json
        	autogen
                COMMAND ./autogen.sh
                DEPENDEES download
                WORKING_DIRECTORY ${PROTO2JSON}
	)
        message ("Successfully downloaded")

endfunction()

function (link_with_gen_c)
	set (PROTO_PATH "${CMAKE_SOURCE_DIR}/proto")
	file(GLOB PROTO_FILES ${PROTO_PATH}/*.c)
	target_sources(protobuf-c-demo PRIVATE ${PROTO_FILES})
endfunction()

function(link_with_lib name)
	unset (LIB_VAR CACHE)
	message (${name})
	find_library(LIB_VAR
                 NAMES ${name}
		 PATHS "${PROJECT_BINARY_DIR/install}"
		 	"/usr/local"
                       "/usr"
                 #ENV PROTOBUF_ROOTDIR
                 PATH_SUFFIXES "lib")
	if(LIB_VAR)
		message (${LIB_VAR})
		target_link_libraries (protobuf-c-demo ${LIB_VAR})
		message ("Successfully link with ${name}")
	else()
		message ("Can't found ${name}")
	endif()

endfunction()

add_executable(protobuf-c-demo src/main.c)
link_directories(${GLOBAL_OUTPUT_PATH})


download_protobuf_lib()
generate_bindings()

download_json_lib()

add_dependencies(protobuf-c-demo proto2json)
download_convert_lib()


link_with_gen_c()
link_with_lib("libprotobuf-c.so")
link_with_lib("libprotobuf2json-c.so")


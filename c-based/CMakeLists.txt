cmake_minimum_required(VERSION 3.14)
project(protobuf-c-demo)
include(ExternalProject)
add_executable(protobuf-c-demo src/main.c)

set (PROTOBUF_C "${CMAKE_SOURCE_DIR}/protobuf-c")

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

function (download_lib)
	message ("Start download protobuf-c")
	set (PROTOBUF_SRC ${CMAKE_SOURCE_DIR/protobuf-c})
  	ExternalProject_Add(protubuf-c
    		URL    https://github.com/protobuf-c/protobuf-c/archive/master.zip
		PREFIX ${CMAKE_SOURCE_DIR}/protobuf-c 
		SOURCE_SUBDIR build-cmake
		#DOWNLOAD_DIR ${PROTOBUF_SRC}
  	)	
	message ("Successfully downloaded")
endfunction()

function (link_with_gen_c)
	set (PROTO_PATH "${CMAKE_SOURCE_DIR}/proto")
	file(GLOB PROTO_FILES ${PROTO_PATH}/*.c)
	target_sources(protobuf-c-demo PRIVATE ${PROTO_FILES})
endfunction()

function(link_with_protobuf_c)
	find_library(PROTOBUF_LIB
                 NAMES "libprotobuf-c.so"
                 PATHS "/usr/local"
                       "/usr"
                 ENV PROTOBUF_ROOTDIR
                 PATH_SUFFIXES "lib")
	if(PROTOBUF_LIB)
		target_link_libraries (protobuf-c-demo ${PROTOBUF_LIB})
		message ("Successfully link with libprotobuf-c")
	else()
		message (FATAL_ERROR "Can't found libprotobuf-c")
	endif()

endfunction()

if (NOT EXISTS ${PROTOBUF_C})
	download_lib()
	generate_bindings()
else()
	message ("Protobuf-c already downloaded")
endif()

link_with_gen_c()
link_with_protobuf_c()


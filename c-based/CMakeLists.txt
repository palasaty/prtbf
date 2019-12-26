cmake_minimum_required(VERSION 3.14)
project(protobuf-c-demo)
set(CMAKE_BUILD_TYPE Debug)
include(ExternalProject)
add_executable(protobuf-c-demo src/main.c)

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
  	ExternalProject_Add(protubuf-c
    		URL    https://github.com/protobuf-c/protobuf-c/archive/master.zip
		PREFIX ${PROTOBUF_C} 
		SOURCE_SUBDIR build-cmake
  	)	
	message ("Successfully downloaded")
endfunction()

function (download_json_lib)
        message ("Start download janson")
        ExternalProject_Add(janson
                URL  https://github.com/akheron/jansson/archive/master.zip  
		PREFIX ${JANSON}
		CMAKE_ARGS -DJANSSON_BUILD_DOCS=OFF 
        )
        message ("Successfully downloaded")

endfunction()

function (download_convert_lib)
	message ("Start download convert lib")
      	set (PROTO2JSON_SRC ${PROTO2JSON}/src/proto2json) 
	ExternalProject_Add(proto2json
                URL  https://github.com/Sannis/protobuf2json-c/archive/master.zip
		PREFIX ${PROTO2JSON}
		#PATCH_COMMAND patch -p1 <${CMAKE_SOURCE_DIR}/proto2json.patch
		CONFIGURE_COMMAND ${PROTO2JSON}/src/proto2json/configure
		BUILD_COMMAND make
		INSTALL_COMMAND make install
		BUILD_IN_SOURCE 1
        )
	ExternalProject_Add_Step(proto2json
        	autogen
                COMMAND ./autogen.sh
                DEPENDEES download
                WORKING_DIRECTORY ${PROTO2JSON_SRC}
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
                 PATHS "/usr/local"
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

if (NOT EXISTS ${PROTOBUF_C})
	download_protobuf_lib()
	generate_bindings()
else()
	message ("Protobuf-c already downloaded")
endif()

if (NOT EXISTS ${JANSON})
	download_json_lib()
else()
	message ("Json lib already downloaded")
endif()

if (NOT EXISTS ${PROTO2JSON})
        download_convert_lib()
else()
	message ("Convert lib already downloaded")
endif()


link_with_gen_c()
link_with_lib("libprotobuf-c.so")
link_with_lib("libprotobuf2json-c.so")


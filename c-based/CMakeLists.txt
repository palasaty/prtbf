cmake_minimum_required(VERSION 2.8)
project(protobuf-c-demo )
set(CMAKE_BUILD_TYPE Debug)
include(ExternalProject)

set(GLOBAL_OUTPUT_PATH ${PROJECT_BINARY_DIR}/bin)
set(CMAKE_INSTALL_PREFIX ${PROJECT_BINARY_DIR}/install)

#set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${GLOBAL_OUTPUT_PATH})
#set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${GLOBAL_OUTPUT_PATH})
#set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${GLOBAL_OUTPUT_PATH})

#foreach(OUTPUTCONFIG ${CMAKE_CONFIGURATION_TYPES})
#    string(TOUPPER ${OUTPUTCONFIG} OUTPUTCONFIG)
#    set(CMAKE_RUNTIME_OUTPUT_DIRECTORY_${OUTPUTCONFIG} ${GLOBAL_OUTPUT_PATH})
#    set(CMAKE_LIBRARY_OUTPUT_DIRECTORY_${OUTPUTCONFIG} ${GLOBAL_OUTPUT_PATH})
#    set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY_${OUTPUTCONFIG} ${GLOBAL_OUTPUT_PATH})
#endforeach(OUTPUTCONFIG CMAKE_CONFIGURATION_TYPES)

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
  	ExternalProject_Add(protobuf-c
    		URL    https://github.com/protobuf-c/protobuf-c/archive/master.zip
		UPDATE_COMMAND cp ${CMAKE_SOURCE_DIR}/patch/protobuf-build ${PROTOBUF_C}/build-cmake/CMakeLists.txt
		SOURCE_DIR ${PROTOBUF_C}
		INSTALL_DIR ${PROJECT_BINARY_DIR}/install
		#BINARY_DIR ${GLOBAL_OUTPUT_PATH}
		SOURCE_SUBDIR build-cmake
		CONFIGURE_COMMAND  ${PROTOBUF_C}/configure -C --prefix=${CMAKE_INSTALL_PREFIX}  "CFLAGS=-g -O2 -fPIC" 
		CMAKE_ARGS  -DCMAKE_INSTALL_PREFIX=${CMAKE_INSTALL_PREFIX}
  	)	
        ExternalProject_Add_Step(protobuf-c
                autogen
                COMMAND ./autogen.sh
                DEPENDEES download
		WORKING_DIRECTORY ${PROTOBUF_C}
        )

	message ("Successfully downloaded")
endfunction()

function (download_json_lib)
        message ("Start download janson")
        ExternalProject_Add(janson
                URL  https://github.com/akheron/jansson/archive/master.zip  
		CMAKE_ARGS -DJANSSON_BUILD_DOCS=OFF -DCMAKE_INSTALL_PREFIX=${CMAKE_INSTALL_PREFIX} 
		SOURCE_DIR ${JANSON}
		#INSTALL_DIR ${GLOBAL_OUTPUT_PATH}
		#BINARY_DIR ${GLOBAL_OUTPUT_PATH}
        )

        message ("Successfully downloaded")

endfunction()

function (download_convert_lib)
	message ("Start download convert lib")

	ExternalProject_Add(proto2json
		DEPENDS protobuf-c janson
                URL  https://github.com/Sannis/protobuf2json-c/archive/master.zip
		SOURCE_DIR ${PROTO2JSON}
		INSTALL_DIR ${PROJECT_BINARY_DIR}/install
		UPDATE_COMMAND cp ${CMAKE_SOURCE_DIR}/patch/protobuf2json.c ${PROTO2JSON}/src 
		CONFIGURE_COMMAND ${PROTO2JSON}/configure -C --prefix=${CMAKE_INSTALL_PREFIX}  --with-protobuf-c=${PROJECT_BINARY_DIR}/install --with-libjansson=${PROJECT_BINARY_DIR}/install
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

function(link_with_lib name libname)
	message(${name} " " ${libname})
	ExternalProject_Get_Property(${name} install_dir)
	message(${install_dir})

	if(install_dir)
		target_link_libraries (protobuf-c-demo ${install_dir}/lib/${libname})
		message ("Successfully link with ${libname}")
	else()
		message ("Can't found ${libname}")
	endif()

endfunction()

include_directories(${CMAKE_INSTALL_PREFIX}/include)
add_executable(protobuf-c-demo src/main.c)
#link_directories(${GLOBAL_OUTPUT_PATH})

add_dependencies(protobuf-c-demo proto2json)
download_protobuf_lib()
download_json_lib()
download_convert_lib()
generate_bindings()

link_with_gen_c()
link_with_lib(proto2json "libprotobuf2json-c.a")
link_with_lib(protobuf-c "libprotobuf-c.a")
link_with_lib(janson "libjansson.a")


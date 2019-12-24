message ("Start")
cmake_minimum_required(VERSION 3.0.0)
project(protobuf-c-demo)
message ("create exe")
add_executable(protobuf-c-demo src/main.c)

set (PROTOBUF_C "${CMAKE_SOURCE_DIR}/protobuf-c")

if (NOT EXISTS ${PROTOBUF_C})
	message ("Start download protobuf-c")

else()
	message ("Protobuf-c already downloaded")
endif()

find_library(libprotobuf-c REQUIRED)

target_link_libraries(protobuf-c-demo ${PROTOBUF_C_LIB})
set (PROTO_PATH "${CMAKE_SOURCE_DIR}/proto")
file(GLOB PROTO_FILES "${PROTO_PATH}/*.proto")
foreach(file ${PROTO_FILES})
	set(PROTO_ARGS " --proto_path=${PROTO_PATH}  --c_out=${PROTO_PATH} ${file}")
	execute_process(COMMAND  protoc-c ${PROTO_ARGS} 
		WORKING_DIRECTORY ${PROJECT_SOURCE_DIR}
		RESULT_VARIABLE ret
		RESULT_OUTPUT )
	if(ret EQUAL "1")
    		message( FATAL_ERROR "Bad exit status")
	endif()
endforeach()

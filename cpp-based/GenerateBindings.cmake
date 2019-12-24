
if( protobuf_DIR )
  set(BINDER "${protobuf_DIR}/bin/protoc")
else()#
  message(FATAL_ERROR "Where is the protoc binder?")
endif()

message(STATUS "Generating Schema Bindings Using : ${BINDER}")
message(STATUS "Generating Schema Bindings in : ${BINDINGS}")

# C++ Bindings
execute_process(COMMAND ${BINDER} --proto_path=${BINDINGS}
                                  --cpp_out=${BINDINGS}
                                  ${BINDINGS}/ComputationalLifeProtocol.proto)
# JavaScript Bindings
#execute_process(COMMAND ${BINDER} --proto_path=${BINDINGS}
#                                  --js_out=library=computational_life,binary:javascript
#                                  ${BINDINGS}/ComputationalLifeProtocol.proto)

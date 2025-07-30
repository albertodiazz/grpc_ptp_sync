cmake_minimum_required(VERSION 3.15)

project(clientGRPC)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

set(grpc_PACKAGE_FOLDER_RELEASE "C:/Users/alffy/.conan2/p/b/grpc12cdb121bde5b/p")
set(PROTO_FOLDER ${CMAKE_SOURCE_DIR}/proto)

set(grpc_p_cc ${PROTO_FOLDER}/test.grpc.pb.cc)
set(grpc_p_h ${PROTO_FOLDER}/test.grpc.pb.h)
set(pb_cpp ${PROTO_FOLDER}/test.pb.cc)
set(pb_h ${PROTO_FOLDER}/test.pb.h)

add_custom_command(
	OUTPUT ${grpc_p_h} ${grpc_p_cc}
	COMMAND protoc 
	ARGS -I ${PROTO_FOLDER} 
	--grpc_out=${PROTO_FOLDER} 
	--cpp_out=${PROTO_FOLDER} 
	--plugin=protoc-gen-grpc=${grpc_PACKAGE_FOLDER_RELEASE}/bin/grpc_cpp_plugin.exe 
	${PROTO_FOLDER}/test.proto
	COMMENT "Generando archivo proto grpc y cpp!"
)

# Cuando ocupamos un custom target tenemos que ocupar el atributo --target <nameTarget>
add_custom_target(generador_protoc
	DEPENDS ${grpc_p_h} ${grpc_p_cc} ${pb_cpp} ${pb_h}
)

find_package(protobuf REQUIRED)
find_package(gRPC CONFIG REQUIRED)

set(SOURCES
	src/main.cpp
	${grpc_p_cc}
	${pb_cpp}
)

add_executable(${PROJECT_NAME} ${SOURCES})

target_include_directories(${PROJECT_NAME} PUBLIC ${PROTO_FOLDER})

target_link_libraries(${PROJECT_NAME} 
	protobuf::protobuf
	gRPC::grpc++
)


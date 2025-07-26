cmake_minimum_required(VERSION 3.15)
project(rpcserver) 

find_package(Protobuf REQUIRED)
find_package(gRPC CONFIG REQUIRED)

set(proto_folder ${CMAKE_SOURCE_DIR}/proto)
set(grpc_pb_cc ${proto_folder}/test.grpc.pb.cc)
set(grpc_pb_h ${proto_folder}/test.grpc.pb.h)
set(pb_cc ${proto_folder}/test.pb.cc)
set(pb_h ${proto_folder}/test.pb.h)
set(PROTOC_CONAN /home/alffy/.conan2/p/b/proto660ba8a148420/p/bin/protoc)

add_custom_command(
	OUTPUT ${grpc_pb_h} ${grpc_pb_cc} ${pb_cc} ${pb_h}
	COMMAND ${PROTOC_CONAN} 
	ARGS -I ${proto_folder}
	--cpp_out=${proto_folder}
	--grpc_out=${proto_folder}
	--plugin=protoc-gen-grpc=/usr/bin/grpc_cpp_plugin
	${proto_folder}/test.proto
	COMMENT "protoc cpp y grpc generados!"
)

add_custom_target(generador_protoc
	DEPENDS ${grpc_pb_h} ${grpc_pb_cc} ${pb_cc} ${pb_h}
)

# Lista de fuentes: tu main y el archivo generado por protoc
set(SOURCES
    src/main.cpp
    ${grpc_pb_cc}
    ${pb_cc}
)
# add_executable(${PROJECT_NAME} src/main.cpp)
add_executable(${PROJECT_NAME} ${SOURCES})

# Incluimos headers
target_include_directories(${PROJECT_NAME} PRIVATE proto)

# Enlazamos librerias
target_link_libraries(${PROJECT_NAME} 
	protobuf::libprotobuf
	gRPC::grpc++
)


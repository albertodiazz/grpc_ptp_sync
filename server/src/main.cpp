#include <algorithm>
#include <cstdio>
#include <cstdlib>
#include <ctime>
#include <grpcpp/completion_queue.h>
#include <grpcpp/server_context.h>
#include <grpcpp/support/status.h>
#include <grpcpp/server_builder.h>
#include <iostream>
#include <memory>
#include <ostream>
#include <string>
#include "test.pb.h"
#include "test.grpc.pb.h"

class Peticiones final : public OffsetService::Service {
	public:
		explicit Peticiones(const std::string& db_path){
		}
		double GetTimestamp(){
			struct timespec ts;
			clock_gettime(CLOCK_REALTIME, &ts);
			return ts.tv_sec + static_cast<double>(ts.tv_nsec) / 1e9;
		}
	// Esta es la version synchronous
	grpc::Status GetOffset(grpc::ServerContext* context,
			const Client* request,
			Server* response) override {

		double ts = GetTimestamp();
		response->set_t2(ts);

		return grpc::Status::OK;
	}

}; 


void RunServer(const std::string& db_path){
	// & Se pasa una referencia al mismo objeto en memoria
	std::string server_address("0.0.0.0:50051");
	Peticiones service(db_path);

	grpc::ServerBuilder builder;
	builder.AddListeningPort(server_address, grpc::InsecureServerCredentials());
	// Aqui es donde puedo generar mis diferentes servicios
	builder.RegisterService(&service);
	std::unique_ptr<grpc::Server> server(builder.BuildAndStart());

	std::cout << "Server listening on " << server_address << std::endl;
	server->Wait();
}

int main(void){

	RunServer("");

	return EXIT_SUCCESS;
}

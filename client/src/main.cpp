#include <grpcpp/security/credentials.h>
#include <iostream>
#include <memory>
#include <grpc/credentials.h>
#include <grpcpp/grpcpp.h>
#include "test.grpc.pb.h"
#include "test.pb.h"

class SendRequest : public OffsetService::Service {
	public:
		SendRequest(std::shared_ptr<grpc::ChannelInterface> channel)
			: stub_(OffsetService::NewStub(channel)){}

		double GetTimestamp(){
			struct timespec ts;
			clock_gettime(CLOCK_REALTIME, &ts);
			return ts.tv_sec + static_cast<double>(ts.tv_nsec) / 1e9;
		}

		std::string SendMsg(){
			Client request;
			request.set_t1(GetTimestamp());

			grpc::ClientContext context;
			Server server;
			grpc::Status status = stub_->GetOffset(&context, request, &server);

			if (status.ok()){
				std::cout << "Msg recibido timestamp: " << GetTimestamp() << std::endl;
				return "Todo bien";

			}else {
				return status.error_message();
			}
		}

	private:
		std::unique_ptr<OffsetService::Stub> stub_;
}; 

int main(void){
	// TODO: DEV
	// Hay que hacer la conexion con servidor
	SendRequest cliente(grpc::CreateChannel("localhost:50051", grpc::InsecureChannelCredentials()));
	cliente.SendMsg();
	return EXIT_SUCCESS;
}

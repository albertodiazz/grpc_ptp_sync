#include <grpcpp/security/credentials.h>
#include <iostream>
#include <memory>
#include <grpc/credentials.h>
#include <grpcpp/grpcpp.h>
#include "../proto/test.grpc.pb.h"
#include <chrono>

class SendRequest : public OffsetService::Service {
	public:
		SendRequest(std::shared_ptr<grpc::ChannelInterface> channel)
			: stub_(OffsetService::NewStub(channel)){}

		double GetTimestamp() {
			using namespace std::chrono;
			auto now = system_clock::now();
			auto duration = now.time_since_epoch();

			auto sec = duration_cast<std::chrono::seconds>(duration).count();
			auto nsec = duration_cast<std::chrono::nanoseconds>(duration).count() % 1'000'000'000;

			return static_cast<double>(sec) + static_cast<double>(nsec) / 1e9;
		}

		std::string SendMsg(){
			double t3 = GetTimestamp();
			Client request;
			request.set_t3(t3);

			grpc::ClientContext context;
			Server response;
			grpc::Status status = stub_->GetOffset(&context, request, &response);
			double t2 = GetTimestamp();

			if (status.ok()) {
				double t1 = response.t1();
				double t4 = response.t4();
				double offset = ((t2 - t1) - (t4 - t3)) / 2.0;
				std::cout << "Offset (PTP): " << offset << " segundos" << std::endl;
				return "Todo Ok";
			}
			else {
				return status.error_message();
			}
		}

	private:
		std::unique_ptr<OffsetService::Stub> stub_;
}; 

int main(void){
	// TODO: DEV
	// Hay que hacer la conexion con servidor
	SendRequest cliente(grpc::CreateChannel("10.90.125.35:50051", grpc::InsecureChannelCredentials()));
	cliente.SendMsg();
	return EXIT_SUCCESS;
}

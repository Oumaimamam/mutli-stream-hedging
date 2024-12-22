#include <iostream>
#include <fstream>
#include <grpcpp/ext/proto_server_reflection_plugin.h>
#include <grpcpp/grpcpp.h>
#include <grpcpp/health_check_service_interface.h>
#include "pricing.grpc.pb.h"
#include "pricer.hpp"

using grpc::Server;
using grpc::ServerBuilder;
using grpc::ServerContext;
using grpc::Status;

PnlMat* convertPastToPnlMat(const PricingInput *input) {
    int m = input->past_size();
    if (m == 0) return NULL;
    
    int n = input->past(0).value_size();
    for (int i = 0; i < m; i++) {
        if (input->past(i).value_size() != n) {
            std::cerr << "Size mismatch in past data" << std::endl;
            return NULL;
        }
    }
    
    PnlMat *past = pnl_mat_create(m, n);
    for (int i = 0; i < m; i++) {
        for (int j = 0; j < n; j++) {
            MLET(past, i, j) = input->past(i).value(j);
        }
    }
    return past;
}

class GrpcPricerImpl final : public GrpcPricer::Service {
private:
    BlackScholesPricer* pricer_;

public:
    GrpcPricerImpl(const nlohmann::json &params) {
        pricer_ = new BlackScholesPricer(params);
        pricer_->print();
    }
    
    ~GrpcPricerImpl() {
        delete pricer_;
    }

    Status PriceAndDeltas(ServerContext* context, const PricingInput* input,
                         PricingOutput* output) override {
        PnlMat *past = convertPastToPnlMat(input);
        if (!past) {
            return Status(grpc::StatusCode::INVALID_ARGUMENT, "Invalid past data");
        }
        //pnl_mat_print(past);
        double price, priceStdDev;
        PnlVect *deltas, *deltasStdDev;

        pricer_->priceAndDeltas(past, input->time(), input->monitoringdatereached(),
                               price, priceStdDev, deltas, deltasStdDev);

        output->set_price(price);
        output->set_pricestddev(priceStdDev);
        
        for (int i = 0; i < deltas->size; i++) {
            output->add_deltas(GET(deltas, i));
            output->add_deltasstddev(GET(deltasStdDev, i));
        }

        pnl_mat_free(&past);
        pnl_vect_free(&deltas);
        pnl_vect_free(&deltasStdDev);
        
        return Status::OK;
    }

    Status Heartbeat(ServerContext* context, const Empty* input,
                    ReqInfo* output) override {
        output->set_domesticinterestrate(pricer_->getInterestRate());
        output->set_relativefinitedifferencestep(pricer_->getFdStep());
        output->set_samplenb(pricer_->getSampleNb());
        return Status::OK;
    }
};

void RunServer(const nlohmann::json &jsonParams) {
    std::string server_address("0.0.0.0:50051");
    GrpcPricerImpl service(jsonParams);

    grpc::EnableDefaultHealthCheckService(true);
    grpc::reflection::InitProtoReflectionServerBuilderPlugin();
    
    ServerBuilder builder;
    builder.AddListeningPort(server_address, grpc::InsecureServerCredentials());
    builder.RegisterService(&service);
    
    std::unique_ptr<Server> server(builder.BuildAndStart());
    std::cout << "Server listening on " << server_address << std::endl;
    server->Wait();
}

int main(int argc, char** argv) {
    if (argc != 2) {
        std::cerr << "Usage: " << argv[0] << " <math_params.json>" << std::endl;
        return 1;
    }

    try {
        std::ifstream file(argv[1]);
        if (!file.is_open()) {
            throw std::runtime_error("Cannot open file: " + std::string(argv[1]));
        }
        
        nlohmann::json jsonParams = nlohmann::json::parse(file);
        RunServer(jsonParams);
        
        return 0;
    }
    catch (const std::exception& e) {
        std::cerr << "Error: " << e.what() << std::endl;
        return 1;
    }
}

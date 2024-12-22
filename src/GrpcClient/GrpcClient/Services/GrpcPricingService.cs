using Grpc.Net.Client;
using GrpcPricing.Protos;

public class GrpcPricingService
{
    public GrpcPricer.GrpcPricerClient grpcClient;

    public GrpcPricingService()
    {
        GrpcChannel channel = GrpcChannel.ForAddress("http://localhost:50051");
        grpcClient = new GrpcPricer.GrpcPricerClient(channel);
    }

    public async Task<PricingOutput> ComputePricingAsync(PricingInput input)
    {
        return await grpcClient.PriceAndDeltasAsync(input);
    }
}

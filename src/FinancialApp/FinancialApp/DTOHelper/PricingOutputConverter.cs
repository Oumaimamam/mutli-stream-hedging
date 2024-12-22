using GrpcPricing.Protos;

namespace FinancialApp.DTOHelper
{
    internal class PricingOutputConverter
    {
        // Convertit un PricingOutput (Protobuf) en DTOPricingOutput (DTO)
        public static DTOPricingOutput ToDTO(PricingOutput protoOutput)
        {
            return new DTOPricingOutput
            {
                Price = protoOutput.Price,
                Deltas = protoOutput.Deltas.ToArray(),
                PriceStdDev = protoOutput.PriceStdDev,
                DeltasStdDev = protoOutput.DeltasStdDev.ToArray()
            };
        }
    }
}

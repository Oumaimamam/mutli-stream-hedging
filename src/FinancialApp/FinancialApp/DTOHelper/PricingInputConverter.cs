using GrpcPricing.Protos;
namespace FinancialApp.DTOHelper
{
    public class PricingInputConverter
    {
        // Convertit un PricingInput (DTO) en PricingInput (Protobuf)
        public static PricingInput ToProtobuf(DTOHelper.DTOPricingInput dtoInput)
        {
            PricingInput request = new PricingInput();

            request.MonitoringDateReached = dtoInput.MonitoringDateReached;
            request.Time = dtoInput.Time;

            foreach (double[] line in dtoInput.Past)
            {
                PastLines pastLines = new PastLines();
                pastLines.Value.Add(line);
                request.Past.Add(pastLines);
            }
            return request;
        }
    }
}

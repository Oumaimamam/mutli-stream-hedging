using FinancialApp.DTOHelper;
using ParameterInfo;

namespace FinancialApp.OutputHandlers
{
    public class RebalancingResults
    {
        public List<OutputData> result { get; set; } = new List<OutputData>();
        public void Save(DTOPricingOutput pricingResult, DateTime date, double portfolioValue)
        {
            OutputData outputData = new OutputData
            {
                Date = date,
                Deltas = pricingResult.Deltas,
                DeltasStdDev = pricingResult.DeltasStdDev,
                Price = pricingResult.Price,
                PriceStdDev = pricingResult.PriceStdDev,
                Value = portfolioValue
            };
            this.result.Add(outputData);
        }
    }
}

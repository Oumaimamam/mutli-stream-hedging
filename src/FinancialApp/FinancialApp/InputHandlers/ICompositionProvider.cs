using FinancialApp.HedgingEngine;
using MarketData;

namespace FinancialApp.InputHandlers
{
    internal interface ICompositionProvider
    {
        public Dictionary<string, double> GetComposition(DataFeed currentDataFeed, RebalancingUtils rebalancingUtils, GrpcPricingService pricingService);

    }
}

using MarketData;
using FinancialApp.PortfolioManager;
using FinancialApp.InputHandlers;
using ParameterInfo;
using GrpcPricing.Protos;
using FinancialApp.OutputHandlers;

namespace FinancialApp.HedgingEngine
{
    public class RebalancingStrategy
    {
        public Portfolio portfolio;
        public GrpcPricingService pricingService;
        public TestParameters financialParameters;
        public List<DataFeed> marketData;
        public RebalancingResults results;

        public RebalancingStrategy(TestParameters financialParameters, List<DataFeed> marketData, Portfolio portfolio)
        {
            this.portfolio = portfolio;
            this.financialParameters = financialParameters;
            this.marketData = marketData;
            pricingService = new GrpcPricingService();
            results = new RebalancingResults();
        }

        // Stratégie Principale
        public List<OutputData> RunRebalancingStrategy()
        {
            //prépare les objets utils
            RebalancingUtils rebalancingUtils = RebalancingUtils.CreateFromParameters(this.financialParameters);
            DeltaNeutralCompositionProvider deltaNeutralCompositionProvider = new DeltaNeutralCompositionProvider();

            // Initialise portefeuille t = 0
            InitializePortfolio(rebalancingUtils, deltaNeutralCompositionProvider);

            // ** Boucle de rééquilibrage ** t : 0 -> len(MarketData)
            ExecuteRebalancingLoop(rebalancingUtils, deltaNeutralCompositionProvider);

            return this.results.result;
        }

        // Méthode pour initialiser le portefeuille
        private void InitializePortfolio(RebalancingUtils rebalancingUtils, DeltaNeutralCompositionProvider deltaNeutralCompositionProvider)
        {
            Dictionary<string, double> composition = deltaNeutralCompositionProvider.GetComposition(marketData[0], rebalancingUtils, pricingService);
            this.portfolio.Initialize(composition, marketData[0], rebalancingUtils.dtoPricingOutput.Price);
            this.results.Save(rebalancingUtils.dtoPricingOutput, this.marketData[0].Date, portfolio.ComputeValue(this.marketData[0]));
            rebalancingUtils.dtoPricingInput.UpdatePast();
        }

        // Méthode pour exécuter la boucle de rééquilibrage
        private void ExecuteRebalancingLoop(RebalancingUtils rebalancingUtils, DeltaNeutralCompositionProvider deltaNeutralCompositionProvider)
        {
            int dateSinceRebalance = 1;
            for (int i = 1; i < this.marketData.Count; i++)
            {
                bool isRebalancingDate = (dateSinceRebalance == rebalancingUtils.RebalancingPeriod);

                if (isRebalancingDate)
                {
                    HandleRebalancing(rebalancingUtils, deltaNeutralCompositionProvider, i);
                    dateSinceRebalance = 1;
                }
                else
                {
                    HandleNonRebalancing(rebalancingUtils, deltaNeutralCompositionProvider, i);
                    dateSinceRebalance += 1;
                }
            }
        }

        // Méthode pour gérer les dates de rééquilibrage
        private void HandleRebalancing(RebalancingUtils rebalancingUtils, DeltaNeutralCompositionProvider deltaNeutralCompositionProvider, int index)
        {
            Dictionary<string, double> composition = deltaNeutralCompositionProvider.GetComposition(marketData[index], rebalancingUtils, pricingService);
            portfolio.UpdatePortfolio(composition, this.marketData[index]);
            this.results.Save(rebalancingUtils.dtoPricingOutput, this.marketData[index].Date, portfolio.ComputeValue(this.marketData[index]));
            rebalancingUtils.dtoPricingInput.UpdatePast();
        }

        // Méthode pour gérer les dates sans rééquilibrage
        private void HandleNonRebalancing(RebalancingUtils rebalancingUtils, DeltaNeutralCompositionProvider deltaNeutralCompositionProvider, int index)
        {
            if (rebalancingUtils.PaymentDates.Contains(this.marketData[index].Date))
            {
                rebalancingUtils.dtoPricingInput.Past.Add(deltaNeutralCompositionProvider.MakeSpots(marketData[index]));
            }
        }
    }
}

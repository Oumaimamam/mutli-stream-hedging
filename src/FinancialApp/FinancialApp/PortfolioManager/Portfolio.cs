using MarketData;
using FinancialApp.MathTools;
using TimeHandler;

namespace FinancialApp.PortfolioManager
{
    public class Portfolio
    {
        public Dictionary<string, double> Composition { get; set; }
        public double Cash { get; set; }
        public DateTime PreviousRebalancingDate { get; set; }
        public double riskFreeRate { get; set; }
        MathDateConverter mathDateConverter { get; set; }

        public Portfolio(Dictionary<string, double> composition, DataFeed dataFeed, double initialValue, double riskFreeRate, int daysInYear)
        {
            Composition = composition;
            Cash = initialValue - Matrix.ScalarProduct(composition, dataFeed.SpotList);
            PreviousRebalancingDate = dataFeed.Date;
            this.riskFreeRate = riskFreeRate;
            mathDateConverter = new MathDateConverter(daysInYear);
        }

        public void Initialize(Dictionary<string, double> composition, DataFeed dataFeed, double initialValue)
        {
            Cash = initialValue - Matrix.ScalarProduct(composition, dataFeed.SpotList);
            Composition = composition;
            PreviousRebalancingDate = dataFeed.Date;
        }
        public double ComputeValue(DataFeed dataFeed)
        {
            Dictionary<string, double> spotValues = dataFeed.SpotList;
            
            double dt = mathDateConverter.ConvertToMathDistance(this.PreviousRebalancingDate, dataFeed.Date);
            double capitalisation = Math.Exp(this.riskFreeRate * dt);
            double value = Cash * capitalisation;
            value += Matrix.ScalarProduct(Composition, spotValues);
            return value;
        }

        public void UpdatePortfolio(Dictionary<string, double> newComposition, DataFeed dataFeed)
        {
            Cash = ComputeValue(dataFeed) - Matrix.ScalarProduct(newComposition, dataFeed.SpotList);
            Composition = newComposition;
            PreviousRebalancingDate = dataFeed.Date;
        }
    }
}



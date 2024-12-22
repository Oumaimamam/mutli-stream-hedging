using FinancialApp.DTOHelper;
using MarketData;
using ParameterInfo;
using TimeHandler;

namespace FinancialApp.HedgingEngine
{
    public class RebalancingUtils
    {
        public int DaysInYear { get; set; }
        public int RebalancingPeriod { get; set; }
        public DateTime StartDate { get; set; }
        public DateTime[] PaymentDates { get; set; }
        public double RiskFreeRate { get; set; }
        public List<ShareValue> ShareValues { get; set; }
        public List<OutputData> outputResults { get; set; }
        public MathDateConverter mathDateConverter { get; set; }
        public DTOPricingInput dtoPricingInput { get; set; }
        public DTOPricingOutput dtoPricingOutput { get; set; }
        public RebalancingUtils(TestParameters financialParameters)
        {
            DaysInYear = financialParameters.NumberOfDaysInOneYear;
            RebalancingPeriod = financialParameters.RebalancingOracleDescription.Period;
            StartDate = financialParameters.PayoffDescription.CreationDate;
            PaymentDates = financialParameters.PayoffDescription.PaymentDates;
            RiskFreeRate = financialParameters.AssetDescription.CurrencyRates["eur"];
            ShareValues = new List<ShareValue>();
            outputResults = new List<OutputData>();
            mathDateConverter = new MathDateConverter(this.DaysInYear);
            dtoPricingInput = new DTOPricingInput();
            dtoPricingOutput = new DTOPricingOutput();
        }

        public static RebalancingUtils CreateFromParameters(TestParameters financialParameters)
        {
            return new RebalancingUtils(financialParameters);
        }
    }
}
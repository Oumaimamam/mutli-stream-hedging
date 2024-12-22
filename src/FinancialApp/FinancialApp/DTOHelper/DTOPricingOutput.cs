namespace FinancialApp.DTOHelper
{
    public class DTOPricingOutput
    {
        public double Price { get; set; }
        public double[] Deltas { get; set; }
        public double PriceStdDev { get; set; }
        public double[] DeltasStdDev { get; set; }

        public DTOPricingOutput()
        {
            Deltas = [];
            DeltasStdDev = [];
        }
    }
}

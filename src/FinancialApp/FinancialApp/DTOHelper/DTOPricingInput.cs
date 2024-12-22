namespace FinancialApp.DTOHelper
{
    public class DTOPricingInput
    {
        public List<double[]> Past { get; set; } = new List<double[]>();
        public bool MonitoringDateReached { get; set; } = false;
        public double Time { get; set; } = 0.0;
        public void UpdatePast()
        {
            if (!this.MonitoringDateReached && this.Past.Count > 0)
            {
                this.Past.RemoveAt(this.Past.Count - 1);
            }
        }
    }
}

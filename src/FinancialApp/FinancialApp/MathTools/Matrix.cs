namespace FinancialApp.MathTools
{
    internal class Matrix
    {
        public static double ScalarProduct(Dictionary<string, double> composition, Dictionary<string, double> spotValues)
        {
            double result = 0.0;

            foreach (var item in composition)
            {
                if (spotValues.ContainsKey(item.Key))
                {
                    result += item.Value * spotValues[item.Key];
                }
            }
            return result;
        }
    }
}

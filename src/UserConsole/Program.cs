using FinancialApp.InputHandlers;
using FinancialApp.HedgingEngine;
using ParameterInfo.JsonUtils;
using ParameterInfo;
using MarketData;
using FinancialApp.PortfolioManager;
class Program
{
    static void Main(string[] args)
    {
        if (args.Length < 3)
        {
            Console.WriteLine("Usage : dotnet run <marketDataPath> <financialParamsPath> <outputResultsPath>");
            return;
        }

        string marketDataPath = args[0];
        string financialParamsPath = args[1];
        string outputFilePath = args[2];

        try
        {
            CsvInputReader csvInputReader = new CsvInputReader();
            JsonInputReader jsonInputReader = new JsonInputReader();

            List<DataFeed> marketData = csvInputReader.ReadMarketData(marketDataPath);
            TestParameters financialParams = jsonInputReader.LoadFinancialParams(financialParamsPath);

            List<OutputData> results = RunRebalancingStrategy(marketData, financialParams);

            string jsonResults = JsonIO.ToJson(results);
            File.WriteAllText(outputFilePath, jsonResults);
        }
        catch (Exception ex)
        {
            Console.WriteLine($"Erreur : {ex.Message}");
        }
    }

    static List<OutputData> RunRebalancingStrategy(List<DataFeed> marketData, TestParameters financialParams)
    {
        // logique métier
        Dictionary<string, double> compositionDict = new Dictionary<string, double>();
        Portfolio portfolio = new Portfolio(compositionDict, marketData[0], 0.0, financialParams.AssetDescription.CurrencyRates["eur"], financialParams.NumberOfDaysInOneYear);
        RebalancingStrategy strategy = new RebalancingStrategy(financialParams, marketData, portfolio);
        List<OutputData> results = strategy.RunRebalancingStrategy();
        return results;
    }
}

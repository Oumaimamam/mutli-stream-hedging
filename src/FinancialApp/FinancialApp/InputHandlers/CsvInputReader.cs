using MarketData;

namespace FinancialApp.InputHandlers
{
    public class CsvInputReader
    {
        // Lit les données du marché depuis un fichier CSV.
        public List<DataFeed> ReadMarketData(string filePath)
        {
            return MarketDataReader.ReadDataFeeds(filePath);
        }

        // Lit les données du marché à partir d'une liste ShareValue.
        public List<DataFeed> ReadMarketData(List<ShareValue> shareValues)
        {
            return MarketDataReader.ReadDataFeeds(shareValues);
        }
    }
}

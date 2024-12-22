using ParameterInfo.JsonUtils;
using ParameterInfo;

namespace FinancialApp.InputHandlers
{
    public class JsonInputReader
    {
        /* Méthode pour charger les paramètres financiers depuis un fichier JSON */
        public TestParameters LoadFinancialParams(string filePath)
        {
            string jsonContent = File.ReadAllText(filePath);
            return JsonIO.FromJson(jsonContent);
        }
    }
}


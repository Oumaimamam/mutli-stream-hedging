using FinancialApp.DTOHelper;
using FinancialApp.HedgingEngine;
using GrpcPricing.Protos;
using MarketData;

namespace FinancialApp.InputHandlers
{
    internal class DeltaNeutralCompositionProvider : ICompositionProvider
    {
        /// <summary>
        /// Récupère la composition en suivant tout le processus : spots, pricing et construction de la composition.
        /// </summary>
        public Dictionary<string, double> GetComposition(DataFeed currentDataFeed, RebalancingUtils rebalancingUtils, GrpcPricingService pricingService)
        {
            double[] spots = MakeSpots(currentDataFeed);
            PricingInput pricingInput = PreparePricingInput(rebalancingUtils, spots, currentDataFeed.Date);

            PricingOutput pricingOutput = pricingService.ComputePricingAsync(pricingInput).GetAwaiter().GetResult(); //Appel Pricer

            DTOPricingOutput dtoPricingOutput = PricingOutputConverter.ToDTO(pricingOutput); //conversion résultat pricer en DTO
            rebalancingUtils.dtoPricingOutput = dtoPricingOutput;

            //Contruction de la composition avec les résultats de pricing
            Dictionary<string, double> composition = new Dictionary<string, double>();
            List<ShareValue> shareValues = ExtractShareValues(currentDataFeed);

            for (int i = 0; i < shareValues.Count; i++)
            {
                composition[shareValues[i].Id] = dtoPricingOutput.Deltas[i]; // Association de l'ID avec la valeur delta recuperée du pricer
            }
            return composition;
        }

        public PricingInput PreparePricingInput(RebalancingUtils rebalancingUtils, double[] spots, DateTime currentDate)
        {
            rebalancingUtils.dtoPricingInput.Time = rebalancingUtils.mathDateConverter.ConvertToMathDistance(rebalancingUtils.StartDate, currentDate);
            rebalancingUtils.dtoPricingInput.Past.Add(spots);
            rebalancingUtils.dtoPricingInput.MonitoringDateReached = rebalancingUtils.PaymentDates.Contains(currentDate);

            return PricingInputConverter.ToProtobuf(rebalancingUtils.dtoPricingInput);
        }

        /// <summary>
        /// Extrait les ShareValues depuis Marketdata.
        /// </summary>
        public List<ShareValue> ExtractShareValues(DataFeed dataFeed)
        {
            List<ShareValue> shareValues = new List<ShareValue>();

            foreach (var spot in dataFeed.SpotList)
            {
                ShareValue shareValue = new ShareValue(spot.Key, dataFeed.Date, spot.Value);
                shareValues.Add(shareValue);
            }
            return shareValues;
        }

        /// <summary>
        /// Récupère les prix (spots) pour les actions spécifiées dans le datafeed.
        /// </summary>
        public double[] MakeSpots(DataFeed dataFeed)
        {
            List<ShareValue> shareValues = ExtractShareValues(dataFeed);
            double[] spots = new double[shareValues.Count];

            for (int i = 0; i < shareValues.Count; i++)
            {
                spots[i] = dataFeed.SpotList[shareValues[i].Id]; // Récupération du prix dans SpotList
            }
            return spots;
        }

    }
}

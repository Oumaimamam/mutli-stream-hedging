WebApplicationBuilder builder = WebApplication.CreateBuilder(args);

builder.Services.AddGrpc(); // Configuration des services gRPC

WebApplication app = builder.Build();
app.MapGrpcService<GrpcPricingService>(); // Exposition du service gRPC
app.MapGet("/", () => "Rebalancing gRPC Client is running."); // Route GET pour tester le démarrage du serveur

app.Run();

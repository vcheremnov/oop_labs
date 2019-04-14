package factory;

import factory.details.Accessory;
import factory.details.Body;
import factory.details.Engine;
import factory.exceptions.FactoryLoadingException;
import factory.production.Car;
import factory.production.CarFactory;
import factory.production.ProductionController;

import java.io.IOException;
import java.io.InputStream;
import java.util.ArrayList;
import java.util.List;
import java.util.Properties;

public class CarManufacturing {
    private static final String configFilename = "factory.properties";

    private Provider<Body> bodyProvider;
    private Provider<Engine> engineProvider;
    private List<Provider<Accessory>> accessoryProviders;

    private Warehouse<Body> bodyWarehouse;
    private Warehouse<Engine> engineWarehouse;
    private Warehouse<Accessory> accessoryWarehouse;

    private List<Dealer> dealers;
    private List<Worker> workers;
    private Warehouse<Car> carWarehouse;
    private CarFactory carFactory;
    private ProductionController controller;

    public CarManufacturing() throws FactoryLoadingException {
        Properties props = new Properties();
        try (InputStream configFile = getClass().getClassLoader().getResourceAsStream(configFilename)) {
            if (configFile == null) {
                String msg = String.format("Failed to open configuration file \"%s\"", configFilename);
                throw new FactoryLoadingException(msg);
            }
            props.load(configFile);
            init(props);
        } catch (IOException | NumberFormatException e) {
            throw new FactoryLoadingException("Failed to load the carFactory", e);
        }
    }

    private void init(Properties props) {
        String bodyWarehouseCapacity = props.getProperty("bodyWarehouseCapacity");
        bodyWarehouse = new Warehouse<>(Integer.parseInt(bodyWarehouseCapacity));

        String engineWarehouseCapacity = props.getProperty("engineWarehouseCapacity");
        engineWarehouse = new Warehouse<>(Integer.parseInt(engineWarehouseCapacity));

        String accessoryWarehouseCapacity = props.getProperty("accessoryWarehouseCapacity");
        accessoryWarehouse  = new Warehouse<>(Integer.parseInt(accessoryWarehouseCapacity));

        String carWarehouseCapacity = props.getProperty("carWarehouseCapacity");
        carWarehouse = new Warehouse<>(Integer.parseInt(carWarehouseCapacity));


        int accessoryProvidersNumber = Integer.parseInt(props.getProperty("accessoryProvidersNumber"));
        accessoryProviders = new ArrayList<>(accessoryProvidersNumber);
        for (int i = 0; i < accessoryProvidersNumber; ++i) {
            accessoryProviders.add(new Provider<>(accessoryWarehouse, Accessory::new));
        }

        bodyProvider = new Provider<>(bodyWarehouse, Body::new);

        engineProvider = new Provider<>(engineWarehouse, Engine::new);


        int workersNumber = Integer.parseInt(props.getProperty("workersNumber"));
        workers = new ArrayList<>(workersNumber);
        for (int i = 0; i < workersNumber; i++) {
            workers.add(new Worker());
        }

        int dealersNumber = Integer.parseInt(props.getProperty("dealersNumber"));
        dealers = new ArrayList<>(dealersNumber);
        for (int i = 0; i < dealersNumber; i++) {
            dealers.add(new Dealer(i, carWarehouse));
        }
        boolean logSale = Boolean.parseBoolean(props.getProperty("logSale"));
        for (Dealer dealer: dealers) {
            dealer.setLogging(logSale);
        }

        carFactory = new CarFactory(workersNumber, bodyWarehouse, engineWarehouse, accessoryWarehouse, carWarehouse);
        controller = new ProductionController(carFactory, carWarehouse);
    }

    public void startProduction() {
        for (Dealer dealer: dealers) {
            dealer.startSales();
        }

        bodyProvider.startSupplies();
        engineProvider.startSupplies();
        for (Provider<Accessory> provider : accessoryProviders) {
            provider.startSupplies();
        }

        controller.startSupervision();
    }

    public void stopProduction() {
        controller.stopSupervision();
        carFactory.stopProduction();

        bodyProvider.stopSupplies();
        engineProvider.stopSupplies();
        for (Provider<Accessory> provider : accessoryProviders) {
            provider.stopSupplies();
        }

        for (Dealer dealer: dealers) {
            dealer.stopSales();
        }
    }
}

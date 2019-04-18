package factory;

import factory.details.Accessory;
import factory.details.Body;
import factory.details.Engine;
import factory.exceptions.FactoryLoadingException;
import factory.production.Car;
import factory.production.AssemblyShop;
import factory.production.ProductionController;
import factory.sales.SalesDepartment;
import factory.supply.Supplies;

import java.io.IOException;
import java.io.InputStream;
import java.util.ArrayList;
import java.util.List;
import java.util.Properties;

public class Factory {
    private static final String configFilename = "factory.properties";

    private Supplies<Body> bodySupplies;
    private Supplies<Engine> engineSupplies;
    private Supplies<Accessory> accessorySupplies;

    private Warehouse<Car> carWarehouse;
    private Warehouse<Body> bodyWarehouse;
    private Warehouse<Engine> engineWarehouse;
    private Warehouse<Accessory> accessoryWarehouse;

    private ProductionController controller;
    private AssemblyShop assemblyShop;
    private SalesDepartment salesDepartment;


    public Factory() throws FactoryLoadingException {
        Properties props = new Properties();
        try (InputStream configFile = getClass().getClassLoader().getResourceAsStream(configFilename)) {
            if (configFile == null) {
                String msg = String.format("Failed to open configuration file \"%s\"", configFilename);
                throw new FactoryLoadingException(msg);
            }
            props.load(configFile);
            init(props);
        } catch (IOException | NumberFormatException e) {
            throw new FactoryLoadingException("Failed to initialize the factory", e);
        }
    }

    public void startProduction() {
        salesDepartment.startSales();
        bodySupplies.startSupplies();
        engineSupplies.startSupplies();
        accessorySupplies.startSupplies();
        controller.startSupervision();
    }

    public void stopProduction() {
        salesDepartment.stopSales();
        bodySupplies.startSupplies();
        engineSupplies.startSupplies();
        accessorySupplies.startSupplies();
        assemblyShop.stopProduction();
        controller.stopSupervision();
    }


    public Warehouse<Car> getCarWarehouse() {
        return carWarehouse;
    }

    public Warehouse<Body> getBodyWarehouse() {
        return bodyWarehouse;
    }

    public Warehouse<Engine> getEngineWarehouse() {
        return engineWarehouse;
    }

    public Warehouse<Accessory> getAccessoryWarehouse() {
        return accessoryWarehouse;
    }


    public Supplies<Body> getBodySupplies() {
        return bodySupplies;
    }

    public Supplies<Engine> getEngineSupplies() {
        return engineSupplies;
    }

    public Supplies<Accessory> getAccessorySupplies() {
        return accessorySupplies;
    }


    public AssemblyShop getAssemblyShop() {
        return assemblyShop;
    }

    public SalesDepartment getSalesDepartment() {
        return salesDepartment;
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


        int bodyProvidersNumber = Integer.parseInt(props.getProperty("bodyProvidersNumber"));
        bodySupplies = new Supplies<>(bodyProvidersNumber, bodyWarehouse, Body::new);

        int engineProvidersNumber = Integer.parseInt(props.getProperty("engineProvidersNumber"));
        engineSupplies = new Supplies<>(engineProvidersNumber, engineWarehouse, Engine::new);

        int accessoryProvidersNumber = Integer.parseInt(props.getProperty("accessoryProvidersNumber"));
        accessorySupplies = new Supplies<>(accessoryProvidersNumber, accessoryWarehouse, Accessory::new);


        int workersNumber = Integer.parseInt(props.getProperty("workersNumber"));
        assemblyShop = new AssemblyShop(workersNumber, this);
        controller = new ProductionController(assemblyShop, carWarehouse);


        int dealersNumber = Integer.parseInt(props.getProperty("dealersNumber"));
        boolean isLogging = Boolean.parseBoolean(props.getProperty("logSale"));
        salesDepartment = new SalesDepartment(dealersNumber, this);
        salesDepartment.setLogging(isLogging);
    }
}

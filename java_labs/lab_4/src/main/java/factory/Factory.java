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
import misc.Observable;

import java.io.IOException;
import java.io.InputStream;
import java.util.Properties;

public class Factory extends Observable {
    private static final String configFilename = "factory.properties";

    private boolean isStarted = false;

    private int bodySuppliersNumber;
    private Supplies<Body> bodySupplies;

    private int engineSuppliersNumber;
    private Supplies<Engine> engineSupplies;

    private int accessorySuppliersNumber;
    private Supplies<Accessory> accessorySupplies;


    private int bodyWarehouseCapacity;
    private Warehouse<Car> carWarehouse;

    private int carWarehouseCapacity;
    private Warehouse<Body> bodyWarehouse;

    private int engineWarehouseCapacity;
    private Warehouse<Engine> engineWarehouse;

    private int accessoryWarehouseCapacity;
    private Warehouse<Accessory> accessoryWarehouse;


    private int workersNumber;
    private ProductionController productionController;
    private AssemblyShop assemblyShop;

    private int dealersNumber;
    private SalesDepartment salesDepartment;
    private boolean isLogging;

    static public class Property extends Observable.Property {
        public static final Property IS_STARTED = new Property("IS_STARTED");

        protected Property(String name) {
            super(name);
        }
    }

    public Factory() throws FactoryLoadingException {
        Properties props = new Properties();
        try (InputStream configFile = getClass().getClassLoader().getResourceAsStream(configFilename)) {
            if (configFile == null) {
                String msg = String.format("Failed to open configuration file \"%s\"", configFilename);
                throw new FactoryLoadingException(msg);
            }
            props.load(configFile);
            parseProperties(props);
        } catch (IOException | NumberFormatException e) {
            throw new FactoryLoadingException("Failed to initialize the factory", e);
        }
    }

    public void startProduction() {
        if (!isStarted) {
            initialize();
            salesDepartment.startSales();
            bodySupplies.startSupplies();
            engineSupplies.startSupplies();
            accessorySupplies.startSupplies();
            productionController.startSupervision();

            isStarted = true;
            firePropertyChanged(Property.IS_STARTED, false, true);
        }
    }

    public void stopProduction() {
        if (isStarted) {
            salesDepartment.stopSales();
            bodySupplies.stopSupplies();
            engineSupplies.stopSupplies();
            accessorySupplies.stopSupplies();
            assemblyShop.stopProduction();
            productionController.stopSupervision();

            isStarted = false;
            firePropertyChanged(Property.IS_STARTED, true, false);
        }
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


    private void initialize() {
        bodyWarehouse = new Warehouse<>(bodyWarehouseCapacity);
        engineWarehouse = new Warehouse<>(engineWarehouseCapacity);
        accessoryWarehouse  = new Warehouse<>(accessoryWarehouseCapacity);
        carWarehouse = new Warehouse<>(carWarehouseCapacity);

        bodySupplies = new Supplies<>(bodySuppliersNumber, bodyWarehouse, Body::new);
        engineSupplies = new Supplies<>(engineSuppliersNumber, engineWarehouse, Engine::new);
        accessorySupplies = new Supplies<>(accessorySuppliersNumber, accessoryWarehouse, Accessory::new);

        assemblyShop = new AssemblyShop(workersNumber, this);
        productionController = new ProductionController(assemblyShop, carWarehouse);

        salesDepartment = new SalesDepartment(dealersNumber, this);
        salesDepartment.setLogging(isLogging);
    }

    private void parseProperties(Properties props) {
        bodyWarehouseCapacity = Integer.parseInt(props.getProperty("bodyWarehouseCapacity"));
        engineWarehouseCapacity = Integer.parseInt(props.getProperty("engineWarehouseCapacity"));
        accessoryWarehouseCapacity = Integer.parseInt(props.getProperty("accessoryWarehouseCapacity"));
        carWarehouseCapacity = Integer.parseInt(props.getProperty("carWarehouseCapacity"));

        bodySuppliersNumber = Integer.parseInt(props.getProperty("bodySuppliersNumber"));
        engineSuppliersNumber = Integer.parseInt(props.getProperty("engineSuppliersNumber"));
        accessorySuppliersNumber = Integer.parseInt(props.getProperty("accessorySuppliersNumber"));

        workersNumber = Integer.parseInt(props.getProperty("workersNumber"));
        dealersNumber = Integer.parseInt(props.getProperty("dealersNumber"));
        isLogging = Boolean.parseBoolean(props.getProperty("logSale"));
    }
}

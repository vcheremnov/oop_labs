package factory;

import factory.exceptions.UnitFactoryLoadingException;
import factory.exceptions.UnitCreationException;
import factory.exceptions.UnitNotFoundException;
import units.Unit;

import java.io.IOException;
import java.io.InputStream;
import java.util.HashMap;
import java.util.Map;
import java.util.Properties;

public class UnitFactory {
    private static final String _configFileName = "config.properties";
    private static volatile UnitFactory _instance;

    private Map<String, Class<?>> _unitClasses = new HashMap<>();

    public static UnitFactory getInstance() throws UnitFactoryLoadingException {
        if (_instance == null) {
            synchronized (UnitFactory.class) {
                if (_instance == null) {
                    _instance = new UnitFactory();
                }
            }
        }
        return _instance;
    }

    public Unit getUnit(String unitName) throws UnitNotFoundException, UnitCreationException {
        Class<?> unitClass = _unitClasses.get(unitName);
        if (unitClass == null) {
            throw new UnitNotFoundException(String.format("Unit \"%s\" was not registered", unitName));
        }

        Unit unit;
        try {
            Object unitObj = unitClass.getConstructor().newInstance();
            if (!(unitObj instanceof Unit)) {
                throw new UnitCreationException(String.format("Failed to create \"%s\" unit: " +
                        "not an instance of the interface Unit", unitName));
            }
            unit = (Unit)unitObj;
        } catch (ReflectiveOperationException | RuntimeException ex) {
            throw new UnitCreationException(String.format("Failed to create \"%s\" unit", unitName), ex);
        }

        return unit;
    }

    private UnitFactory() throws UnitFactoryLoadingException {
        Properties props = new Properties();
        String unitClassName = null;

        try (InputStream configFile = UnitFactory.class.getClassLoader().getResourceAsStream(_configFileName)) {
            if (configFile == null) {
                throw new UnitFactoryLoadingException("Failed to initialize the unit factory: " +
                        "failed to open the config file \"" + _configFileName + "\"");
            }

            props.load(configFile);
            for (String unitName: props.stringPropertyNames()) {
                // is it really needed?? ...
                if (_unitClasses.containsKey(unitName)) {
                    throw new UnitFactoryLoadingException("Failed to initialize the unit factory: " +
                            "duplicate unit name \"" + unitName + "\"");
                }

                unitClassName = props.getProperty(unitName);
                Class unitClass = Class.forName(unitClassName);
                _unitClasses.put(unitName, unitClass);
            }

        } catch (ClassNotFoundException ex) {
            throw new UnitFactoryLoadingException("Failed to initialize the unit factory: " +
                    "class \"" + unitClassName + "\" was not found", ex);
        } catch (IOException ex) {
            throw new UnitFactoryLoadingException("Failed to initialize the unit factory: " +
                    "failed to read the config file \"" + _configFileName + "\"", ex);
        }
    }

}

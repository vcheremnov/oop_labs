package factory;

import factory.exceptions.LoadingFailedException;
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
    private static UnitFactory _instance;

    private Map<String, Class<?>> _unitClasses = new HashMap<>();

    public static UnitFactory getInstance() {
        if (_instance == null) {
            _instance = new UnitFactory();
        }
        return _instance;
    }

    public Unit getUnit(String unitName) {
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
        } catch (Exception ex) {
            throw new UnitCreationException(String.format("Failed to create \"%s\" unit", unitName), ex);
        }

        return unit;
    }

    private UnitFactory() {
        Properties props = new Properties();
        String unitClassName = null;

        try (InputStream configFile = getClass().getResourceAsStream(_configFileName)){
            if (configFile == null) {
                throw new LoadingFailedException("Failed to initialize the unit factory: " +
                        "failed to open the config file \"" + _configFileName + "\"");
            }

            props.load(configFile);
            for (String unitName: props.stringPropertyNames()) {
                if (_unitClasses.containsKey(unitName)) {
                    throw new LoadingFailedException("Failed to initialize the unit factory: " +
                            "duplicate unit name \"" + unitName + "\"");
                }

                unitClassName = props.getProperty(unitName);
                Class unitClass = Class.forName(unitClassName);
                _unitClasses.put(unitName, unitClass);
            }

        } catch (ClassNotFoundException ex) {
            throw new LoadingFailedException("Failed to initialize the unit factory: " +
                    "class \"" + unitClassName + "\" was not found", ex);
        } catch (IOException ex) {
            throw new LoadingFailedException("Failed to initialize the unit factory: " +
                    "failed to read the config file \"" + _configFileName + "\"", ex);
        }
    }

}

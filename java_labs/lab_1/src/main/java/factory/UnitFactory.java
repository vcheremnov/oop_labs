package factory;

import factory.exceptions.UnitFactoryLoadingException;
import factory.exceptions.UnitCreationException;
import factory.exceptions.UnitNotFoundException;
import org.apache.logging.log4j.LogManager;
import org.apache.logging.log4j.Logger;
import units.Unit;

import java.io.IOException;
import java.io.InputStream;
import java.util.HashMap;
import java.util.Map;
import java.util.Properties;

public class UnitFactory {
    private static final Logger logger = LogManager.getLogger(UnitFactory.class);
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
        logger.info(String.format("Getting \"%s\" unit", unitName));
        Class<?> unitClass = _unitClasses.get(unitName);
        if (unitClass == null) {
            UnitNotFoundException ex = new UnitNotFoundException(String.format("Unit \"%s\" was not registered", unitName));
            logger.error("Failed to get the unit", ex);
            throw ex;
        }

        Unit unit;
        try {
            Object unitObj = unitClass.getConstructor().newInstance();
            if (!(unitObj instanceof Unit)) {
                UnitCreationException ex = new UnitCreationException(unitName + " is not an instance of the interface Unit");
                logger.error("Failed to get the unit", ex);
                throw ex;
            }
            unit = (Unit)unitObj;
        } catch (ReflectiveOperationException | RuntimeException ex) {
            logger.error("Failed to get the unit", ex);
            throw new UnitCreationException(ex);
        }

        return unit;
    }

    private UnitFactory() throws UnitFactoryLoadingException {
        Properties props = new Properties();
        String unitClassName = null;

        logger.info("UnitFactory initialization");
        try (InputStream configFile = UnitFactory.class.getClassLoader().getResourceAsStream(_configFileName)) {
            if (configFile == null) {
                UnitFactoryLoadingException ex = new UnitFactoryLoadingException("Failed to open the config file \""
                        + _configFileName + "\"");
                logger.error("UnitFactory initialization failure", ex);
                throw ex;
            }

            props.load(configFile);
            for (String unitName: props.stringPropertyNames()) {
                // is it really needed?? ...
                if (_unitClasses.containsKey(unitName)) {
                    UnitFactoryLoadingException ex = new UnitFactoryLoadingException("Duplicate unit name \""
                            + unitName + "\"");
                    logger.error("UnitFactory initialization failure", ex);
                    throw ex;
                }

                unitClassName = props.getProperty(unitName);
                Class unitClass = Class.forName(unitClassName);
                _unitClasses.put(unitName, unitClass);
            }

        } catch (ClassNotFoundException | IOException ex) {
            logger.error("UnitFactory initialization failure", ex);
            throw new UnitFactoryLoadingException(ex);
        }
    }

}

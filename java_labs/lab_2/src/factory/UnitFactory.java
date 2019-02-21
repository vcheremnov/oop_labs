package factory;

import factory.exceptions.LoadingFailedException;
import factory.exceptions.UnitCreationException;
import factory.exceptions.UnitNotFoundException;
import units.Unit;

import java.io.IOException;
import java.lang.reflect.Constructor;
import java.util.HashMap;
import java.util.Map;

public class UnitFactory {
    private static final String _configFileName = "config.properties";
    private static UnitFactory _instance;

    private Map<String, Class<Unit>> _unitClasses = new HashMap<>();

    public static UnitFactory getInstance() {
        if (_instance == null) {
            _instance = new UnitFactory();
        }
        return _instance;
    }

    private UnitFactory() {
        try {
            _loadUnits();
        } catch (Exception ex) {
            throw new LoadingFailedException("Failed to initialize the unit factory", ex);
        }
    }

    private void _loadUnits() throws IOException {

    }

    public Unit getUnit(String unitName) {
        Class<Unit> unitClass = _unitClasses.get(unitName);
        if (unitClass == null) {
            throw new UnitNotFoundException(String.format("Unit \"%s\" was not found", unitName));
        }

        Unit unit;
        try {
            unit = unitClass.getConstructor().newInstance();
        } catch (Exception ex) {
            throw new UnitCreationException(String.format("Failed to create \"%s\" unit", unitName));
        }

        return unit;
    }

}

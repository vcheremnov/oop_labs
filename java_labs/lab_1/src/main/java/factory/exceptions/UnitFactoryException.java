package factory.exceptions;

public class UnitFactoryException extends Exception {
    public UnitFactoryException() {
        super();
    }

    public UnitFactoryException(String message) {
        super(message);
    }

    public UnitFactoryException(Throwable cause) {
        super(cause);
    }

    public UnitFactoryException(String message, Throwable cause) {
        super(message, cause);
    }
}

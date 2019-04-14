package factory.exceptions;

public class FactoryException extends Exception {
    public FactoryException() {
        super();
    }

    public FactoryException(String message) {
        super(message);
    }

    public FactoryException(Throwable cause) {
        super(cause);
    }

    public FactoryException(String message, Throwable cause) {
        super(message, cause);
    }
}

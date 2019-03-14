package factory.exceptions;

public class UnitFactoryLoadingException extends UnitFactoryException {
    public UnitFactoryLoadingException() {
        super();
    }

    public UnitFactoryLoadingException(String message) {
        super(message);
    }

    public UnitFactoryLoadingException(Throwable cause) {
        super(cause);
    }

    public UnitFactoryLoadingException(String message, Throwable cause) {
        super(message, cause);
    }
}

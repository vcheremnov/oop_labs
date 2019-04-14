package factory.exceptions;

public class FactoryLoadingException extends FactoryException {
    public FactoryLoadingException() {
        super();
    }

    public FactoryLoadingException(String message) {
        super(message);
    }

    public FactoryLoadingException(Throwable cause) {
        super(cause);
    }

    public FactoryLoadingException(String message, Throwable cause) {
        super(message, cause);
    }
}

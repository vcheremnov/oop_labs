package misc.factory.exceptions;

public class ItemFactoryException extends Exception {
    public ItemFactoryException() {
        super();
    }

    public ItemFactoryException(String message) {
        super(message);
    }

    public ItemFactoryException(String message, Throwable cause) {
        super(message, cause);
    }

    public ItemFactoryException(Throwable cause) {
        super(cause);
    }
}

package misc.itemfactory.exceptions;

public class ItemCreationException extends ItemFactoryException {
    public ItemCreationException() {
        super();
    }

    public ItemCreationException(String message) {
        super(message);
    }

    public ItemCreationException(String message, Throwable cause) {
        super(message, cause);
    }

    public ItemCreationException(Throwable cause) {
        super(cause);
    }
}

package misc.itemfactory.exceptions;

public class ItemFactoryLoadingException extends ItemFactoryException {
    public ItemFactoryLoadingException() {
        super();
    }

    public ItemFactoryLoadingException(String message) {
        super(message);
    }

    public ItemFactoryLoadingException(String message, Throwable cause) {
        super(message, cause);
    }

    public ItemFactoryLoadingException(Throwable cause) {
        super(cause);
    }
}

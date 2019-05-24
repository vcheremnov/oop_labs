package server.handlers;

import misc.itemfactory.ItemFactory;
import misc.itemfactory.exceptions.ItemCreationException;
import misc.itemfactory.exceptions.ItemFactoryLoadingException;
import misc.itemfactory.exceptions.ItemNotFoundException;
import server.data.ChatDatabase;

public class RequestHandlerFactory extends ItemFactory {
    private static volatile RequestHandlerFactory instance;

    public static RequestHandlerFactory getInstance() throws ItemFactoryLoadingException {
        if (instance == null) {
            synchronized (RequestHandlerFactory.class) {
                if (instance == null) {
                    instance = new RequestHandlerFactory();
                }
            }
        }
        return instance;
    }

    private RequestHandlerFactory() throws ItemFactoryLoadingException {}

    public RequestHandler getRequestHandler(String type, ChatDatabase database)
            throws ItemCreationException, ItemNotFoundException {
        return (RequestHandler) getItem(type, database);
    }

    @Override
    protected String getConfigFilepath() {
        return "requestHandlers.properties";
    }

    @Override
    protected Class<?> getItemBaseClass() {
        return RequestHandler.class;
    }

    @Override
    protected Class<?>[] getParameterTypes() {
        return new Class[]{ChatDatabase.class};
    }

    @Override
    protected String getItemName() {
        return "request handler";
    }
}

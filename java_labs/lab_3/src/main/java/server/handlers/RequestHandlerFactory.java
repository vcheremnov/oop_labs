package server.handlers;

import misc.factory.ItemFactory;
import misc.factory.exceptions.ItemCreationException;
import misc.factory.exceptions.ItemFactoryLoadingException;
import misc.factory.exceptions.ItemNotFoundException;
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
        return "server/requestHandlers.properties";
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

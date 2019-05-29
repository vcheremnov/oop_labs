package client.model.handlers.responses;

import misc.factory.ItemFactory;
import misc.factory.exceptions.ItemCreationException;
import misc.factory.exceptions.ItemFactoryLoadingException;
import misc.factory.exceptions.ItemNotFoundException;

public class ResponseHandlerFactory extends ItemFactory {
    private static volatile ResponseHandlerFactory instance;

    public static ResponseHandlerFactory getInstance() throws ItemFactoryLoadingException {
        if (instance == null) {
            synchronized (ResponseHandlerFactory.class) {
                if (instance == null) {
                    instance = new ResponseHandlerFactory();
                }
            }
        }
        return instance;
    }

    private ResponseHandlerFactory() throws ItemFactoryLoadingException {}

    public ResponseHandler getResponseHandler(String type) throws ItemCreationException, ItemNotFoundException {
        return (ResponseHandler) getItem(type);
    }

    @Override
    protected String getConfigFilepath() {
        return "client/responseHandlers.properties";
    }

    @Override
    protected Class<?> getItemBaseClass() {
        return ResponseHandler.class;
    }

    @Override
    protected Class<?>[] getParameterTypes() {
        return new Class[0];
    }

    @Override
    protected String getItemName() {
        return "response handler";
    }
}

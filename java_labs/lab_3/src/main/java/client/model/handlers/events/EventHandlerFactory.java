package client.model.handlers.events;

import misc.factory.ItemFactory;
import misc.factory.exceptions.ItemCreationException;
import misc.factory.exceptions.ItemFactoryLoadingException;
import misc.factory.exceptions.ItemNotFoundException;

public class EventHandlerFactory extends ItemFactory {
    private static volatile EventHandlerFactory instance;

    public static EventHandlerFactory getInstance() throws ItemFactoryLoadingException {
        if (instance == null) {
            synchronized (EventHandlerFactory.class) {
                if (instance == null) {
                    instance = new EventHandlerFactory();
                }
            }
        }
        return instance;
    }

    private EventHandlerFactory() throws ItemFactoryLoadingException {}

    public EventHandler getEventHandler(String type) throws ItemCreationException, ItemNotFoundException {
        return (EventHandler) getItem(type);
    }

    @Override
    protected String getConfigFilepath() {
        return "client/eventHandlers.properties";
    }

    @Override
    protected Class<?> getItemBaseClass() {
        return EventHandler.class;
    }

    @Override
    protected Class<?>[] getParameterTypes() {
        return new Class[0];
    }

    @Override
    protected String getItemName() {
        return "event handler";
    }
}


package client.model.handlers.events;

import misc.itemfactory.ItemFactory;
import misc.itemfactory.exceptions.ItemCreationException;
import misc.itemfactory.exceptions.ItemFactoryLoadingException;
import misc.itemfactory.exceptions.ItemNotFoundException;

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
        return "eventHandlers.properties";
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


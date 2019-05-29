package misc.factory;

import misc.factory.exceptions.ItemCreationException;
import misc.factory.exceptions.ItemFactoryLoadingException;
import misc.factory.exceptions.ItemNotFoundException;

import java.io.IOException;
import java.io.InputStream;
import java.lang.reflect.Constructor;
import java.util.HashMap;
import java.util.Properties;

public abstract class ItemFactory {
    private HashMap<String, Constructor<?>> constructors;

    protected Object getItem(String type, Object ...initArgs) throws ItemNotFoundException, ItemCreationException {
        Constructor<?> itemConstructor = constructors.get(type);
        if (itemConstructor == null) {
            String message = String.format("%s of type \"%s\" was not found", getItemName(), type);
            throw new ItemNotFoundException(message);
        }

        try {
            return itemConstructor.newInstance(initArgs);
        } catch (ReflectiveOperationException e) {
            String message = String.format("Failed to create an %s of type \"%s\"", getItemName(), type);
            throw new ItemCreationException(message, e);
        }
    }

    protected ItemFactory() throws ItemFactoryLoadingException {
        constructors = new HashMap<>();
        Properties props = new Properties();

        String configFilepath = getConfigFilepath();
        try (InputStream configFile = getClass().getClassLoader().getResourceAsStream(configFilepath)) {
            if (configFile == null) {
                String message = String.format("Failed to load configuration file \"%s\"", configFilepath);
                throw new ItemFactoryLoadingException(message);
            }

            props.load(configFile);
            for (String itemType : props.stringPropertyNames()) {
                String itemClassName = props.getProperty(itemType);
                Class<?> itemClass = Class.forName(itemClassName);

                Class<?> itemBaseClass = getItemBaseClass();
                boolean isItem = itemBaseClass.isAssignableFrom(itemClass);
                if (!isItem) {
                    String message = itemClassName + " is not an instance of " + itemBaseClass.getName();
                    throw new ItemFactoryLoadingException(message);
                }

                Class<?>[] parameterTypes = getParameterTypes();
                Constructor<?> itemConstructor = itemClass.getConstructor(parameterTypes);
                constructors.put(itemType, itemConstructor);
            }
        } catch (ReflectiveOperationException | IOException e) {
            String message = String.format("Failed to load the %s factory", getItemName());
            throw new ItemFactoryLoadingException(message, e);
        }

    }

    protected abstract String getConfigFilepath();

    protected abstract Class<?> getItemBaseClass();

    protected abstract Class<?>[] getParameterTypes();

    protected abstract String getItemName();
}
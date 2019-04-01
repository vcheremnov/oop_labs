package server;

import com.google.gson.Gson;
import com.google.gson.JsonSyntaxException;

public class MessageHandler {
    private Gson jsonParser = new Gson();

    public void handleMessage(String message) {
        System.out.println("Received message:");
        System.out.println(message);
        try {
//            String commandType = jsonParser.
//            Server.Student student = gson.fromJson(msg, Server.Student.class);
        } catch (JsonSyntaxException ex) {
            System.err.println("Json syntax error:");
            ex.printStackTrace();
        }
    }
}

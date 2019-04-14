package server;

import com.google.gson.Gson;
import com.google.gson.JsonParser;
import com.google.gson.JsonSyntaxException;

public class MessageHandler {
    private Gson gson = new Gson();
    private JsonParser jsonParser = new JsonParser();
    private String requestField = "request";

    public String handleMessage(String message) {
        System.out.println("Received message:");
        System.out.println(message);
        try {
            String requestName = jsonParser.parse(requestField).getAsString();
        } catch (JsonSyntaxException ex) {
            System.err.println("Json syntax error:");
            ex.printStackTrace();
        } catch (ClassCastException | IllegalStateException ex) {
            System.err.println("Failed to extract the type of client request: " + ex.getMessage());
            ex.printStackTrace();
        }
        return null;
    }
}

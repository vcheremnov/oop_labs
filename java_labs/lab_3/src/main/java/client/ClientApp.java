package client;

import client.gui.JavafxApp;
import com.google.gson.Gson;
import com.google.gson.JsonElement;
import com.google.gson.JsonObject;
import com.google.gson.JsonParser;
import protocol.commands.request.Request;
import protocol.commands.types.chatmessage.ChatMessageRequest;
import protocol.commands.types.login.LoginRequest;
import protocol.commands.types.logout.LogoutRequest;
import protocol.commands.types.serverinfo.ServerInfoRequest;

import java.io.BufferedReader;
import java.io.IOException;
import java.io.InputStreamReader;
import java.io.PrintWriter;
import java.net.Socket;
import java.util.UUID;

public class ClientApp {
    public static void main(String[] args) {
        JavafxApp.main(args);
    }

//    private static final int REQUIRED_ARGS_NUM = 2;
//    private static final int ERROR_CODE = 1;
//    private static Gson gson = new Gson();
//    private static String sessionID;
//
//    private static void printUsage() {
//        System.err.println("Required args: <host name> <port>");
//    }
//
//    public static void main(String[] args) {
//
//        try (Socket socket = new Socket("localhost", 12345);
//             final BufferedReader reader = new BufferedReader(new InputStreamReader(socket.getInputStream()));
//             final PrintWriter writer = new PrintWriter(socket.getOutputStream(), true);
//             BufferedReader stdreader = new BufferedReader(new InputStreamReader(System.in))) {
//
//            LoginRequest loginRequest = new LoginRequest();
//            loginRequest.setUsername(UUID.randomUUID().toString());
//            writer.println(gson.toJson(loginRequest));
//
//            String loginResponseString = reader.readLine();
//            System.out.println(loginResponseString);
//
//            JsonParser jsonParser = new JsonParser();
//            JsonObject jsonObject = jsonParser.parse(loginResponseString).getAsJsonObject();
//            JsonElement requestTypeField = jsonObject.get("sessionID");
//            sessionID = requestTypeField.getAsString();
//
//            Thread readerThread = new Thread(() -> {
//                try {
//                    String line = null;
//                    while ((line = reader.readLine()) != null) {
//                        System.err.println(line);
//                    }
//                    System.err.println("socket was closed");
//                } catch (IOException e) {
//                    e.printStackTrace();
//                }
//            });
//            readerThread.setDaemon(true);
//            readerThread.start();
//
//
//            String cmd = null;
//            while ((cmd = stdreader.readLine()) != null && !socket.isClosed()) {
//                cmd = cmd.trim();
//                if (cmd.equals("message")) {
//                    String message = stdreader.readLine();
//                    ChatMessageRequest request = new ChatMessageRequest();
//                    request.setMessage(message);
//                    request.setSessionID(sessionID);
//                    String str = gson.toJson(request);
//                    System.out.println(str);
//                    writer.println(str);
//                    continue;
//                }
//                handleCommand(cmd, writer);
//            }
//
//        } catch (IOException e) {
//            e.printStackTrace();
//        }
//    }
//
//    static void handleCommand(String command, PrintWriter writer) {
//        Request request;
//        if (command.equals("login")) {
//            LoginRequest loginRequest = new LoginRequest();
//            loginRequest.setUsername(UUID.randomUUID().toString());
//            request = loginRequest;
//        } else if (command.equals("logout")) {
//            request = new LogoutRequest();
//        } else if (command.equals("serverinfo")) {
//            request = new ServerInfoRequest();
//        } else return;
//
//        request.setSessionID(sessionID);
//        String requestString = gson.toJson(request);
//        writer.println(requestString);
//    }
}

package server;

import java.io.BufferedReader;
import java.io.IOException;
import java.io.InputStreamReader;
import java.net.InetAddress;
import java.net.UnknownHostException;
import java.nio.Buffer;
import java.util.StringTokenizer;
import java.util.regex.Matcher;
import java.util.regex.Pattern;

public class ServerApp {
    private static boolean exitFlag = false;
    private static Thread serverThread = null;
    private static String hostName = "localhost";
    private static int port = 12345;


    public static void main(String[] args) {
        try (BufferedReader stdin = new BufferedReader(new InputStreamReader(System.in))) {
            printGreetings();
            while (!exitFlag) {
                System.out.print(">> ");
                String cmd = stdin.readLine();
                executeCommand(cmd.trim(), stdin);
            }
        } catch (IOException ex) {
            ex.printStackTrace();
            if (serverThread != null) {
                serverThread.interrupt();
            }
        }
    }

    private static Thread createServerThread() {
        Thread thread = new Thread(() -> {
            try (Server server = new Server()) {
                server.init(hostName, port);
                server.run();
            } catch (Exception ex) {
                System.err.println("Server fatal error:");
                ex.printStackTrace();
            }
        });
        thread.setDaemon(true);
        return thread;
    }

    private static final String START_COMMAND = "start";
    private static final String SHUTDOWN_COMMAND = "shutdown";
    private static final String SET_ADDRESS_COMMAND = "setaddr";
    private static final String GET_ADDRESS_COMMAND = "getaddr";
    private static final String EXIT_COMMAND = "exit";
    private static final String HELP_COMMAND = "help";

    private static final Pattern addressPattern = Pattern.compile("\\s*\\S+\\s+[0-9]{1,5}\\s*");

    private static void printGreetings() {
        System.out.println(
                "Welcome to the WebChat server v1.0\n" +
                "To print the list of available commands, type \"" + HELP_COMMAND + "\""
        );
    }

    private static void executeCommand(String cmd, BufferedReader stdin) throws IOException {
        if (cmd == null) {
            throw new NullPointerException("Command is null");
        }
        if (cmd.isEmpty()) {
            return;
        }
        switch (cmd) {
            case START_COMMAND:
                startServer();
                break;
            case SET_ADDRESS_COMMAND:
                setServerAddress(stdin);
                break;
            case GET_ADDRESS_COMMAND:
                getServerAddress();
                break;
            case SHUTDOWN_COMMAND:
                closeServer();
                break;
            case EXIT_COMMAND:
                exitApp();
                break;
            case HELP_COMMAND:
                printCommandList();
                break;
            default:
                System.out.println("Error: Unknown command \"" + cmd + "\"");

        }
    }

    private static void printCommandList() {
        System.out.println(
                "* start - start the server\n" +
                "* setaddr - set an address of the server\n" +
                "* getaddr - get the address of the server\n" +
                "* shutdown - close the server\n" +
                "* exit - exit the program\n" +
                "* help - print the list of commands"
        );
    }

    private static void startServer() {
        if (serverThread == null || !serverThread.isAlive()) {
            System.out.println("Starting the server...");
            serverThread = createServerThread();
            serverThread.start();
        } else {
            System.out.println("Error: Server is running now");
        }
    }

    private static void closeServer() {
        if (serverThread == null) {
            System.out.println("Error: Server is not running now");
        } else {
            System.out.println("Shutting down the server...");
            serverThread.interrupt();
            serverThread = null;
        }
    }

    private static void setServerAddress(BufferedReader stdin) throws IOException {
        System.out.print("Enter \"<hostname> <port>\": ");

        String line = stdin.readLine();
        Matcher addrMatcher = addressPattern.matcher(line);
        if (!addrMatcher.matches()) {
            System.out.println("Entered address doesn't match the pattern given above");
            return;
        }

        StringTokenizer tokenizer = new StringTokenizer(line);
        String newHostName = tokenizer.nextToken();
        int newPort = Integer.parseInt(tokenizer.nextToken());

        if (newPort < 0 || newPort > 65535) {
            System.out.println("Port has to be from 0 to 65535");
            return;
        }

        hostName = newHostName;
        port = newPort;
    }

    private static void getServerAddress() {
        System.out.println(String.format("Server address is %s:%d", hostName, port));
    }

    private static void exitApp() {
        if (serverThread == null || !serverThread.isAlive()) {
            exitFlag = true;
        } else {
            System.out.println("Error: server is running now; close the server before exiting");
        }
    }
}

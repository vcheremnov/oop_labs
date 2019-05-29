package server;

import server.data.ChatDatabase;
import server.network.ChatServer;

import java.io.BufferedReader;
import java.io.IOException;
import java.io.InputStream;
import java.io.InputStreamReader;
import java.util.List;
import java.util.Properties;


public class ServerApp {
    public static void main(String[] args) {
        Server server = new Server();
        Server.CommandProcessor commandProcessor = server.new CommandProcessor();
        try (BufferedReader stdin = new BufferedReader(new InputStreamReader(System.in))) {
            server.init();
            commandProcessor.printGreetings();
            while (!server.isExitFlagSet()) {
                commandProcessor.printPrompt();
                String command = stdin.readLine();
                if (command == null) {
                    break;
                }
                commandProcessor.executeCommand(command.trim());
            }
        } catch (IOException | NullPointerException ex) {
            ex.printStackTrace();
        } finally {
            server.shutdown();
        }
    }
}

class Server {
    private static final String configFilepath = "server/config.properties";

    private int port;
    private String hostname;
    private String serverName;
    private int maxClientsNumber;
    private boolean isLogging = false;
    private ChatServer chatServer;

    private boolean exitFlag = false;
    private Thread serverThread = null;

    void init() throws IOException {
        Properties props = new Properties();
        InputStream configFile = ServerApp.class.getClassLoader().getResourceAsStream(configFilepath);
        if (configFile == null) {
            throw new IOException(configFilepath + " couldn't be open");
        }

        props.load(configFile);
        parseProperties(props);
    }

    void shutdown() {
        if (isRunning()) {
            chatServer.close();
            serverThread.interrupt();
            serverThread = null;
        }
    }

    boolean isExitFlagSet() {
        return exitFlag;
    }

    private boolean isRunning() {
        return serverThread != null && serverThread.isAlive();
    }

    private void parseProperties(Properties props) {
        hostname = props.getProperty("hostname");
        port = Integer.parseInt(props.getProperty("port"));

        serverName = props.getProperty("servername");
        maxClientsNumber = Integer.parseInt(props.getProperty("maxclients"));

        isLogging = Boolean.parseBoolean(props.getProperty("logging"));
    }

    class CommandProcessor {
        private static final String START_COMMAND = "start";
        private static final String SHUTDOWN_COMMAND = "shutdown";
        private static final String INFO_COMMAND = "info";
        private static final String CLIENTS_COMMAND = "clients";
        private static final String EXIT_COMMAND = "exit";
        private static final String HELP_COMMAND = "help";

        void printGreetings() {
            System.out.println(
                "<<< Chat server v1.0 >>>\n" +
                "To print the list of available commands, type \"" + HELP_COMMAND + "\""
            );
        }

        void printPrompt() {
            System.out.print(">> ");
            System.out.flush();
        }

        void executeCommand(String cmd) {
            if (cmd.isEmpty()) {
                return;
            }
            switch (cmd) {
                case START_COMMAND:
                    startServer();
                    break;
                case INFO_COMMAND:
                    printServerInfo();
                    break;
                case CLIENTS_COMMAND:
                    printClientsList();
                    break;
                case SHUTDOWN_COMMAND:
                    shutdownServer();
                    break;
                case EXIT_COMMAND:
                    exitApp();
                    break;
                case HELP_COMMAND:
                    printCommandList();
                    break;
                default:
                    System.err.println("Error: Unknown command \"" + cmd + "\"");

            }
        }

        private void printCommandList() {
            System.out.println(
                "* start - start the server\n" +
                "* shutdown - close the server\n" +
                "* info - print server info\n" +
                "* clients - print clients list\n" +
                "* exit - exit the program\n" +
                "* help - print the list of commands"
            );
        }

        private Thread createServerThread() {
            Thread thread = new Thread(() -> {
                try {
                    chatServer = new ChatServer();
                    chatServer.setAddress(hostname, port);
                    chatServer.start(serverName, maxClientsNumber);
                } catch (RuntimeException ex) {
                    ex.printStackTrace();
                }
            });
            thread.setDaemon(true);
            return thread;
        }

        private void startServer() {
            if (isRunning()) {
                System.err.println("Error: Server is running now");
                return;
            }

            System.out.println("Starting the server...");
            serverThread = createServerThread();
            serverThread.start();
        }

        private void shutdownServer() {
            if (!isRunning()) {
                System.out.println("Error: Server is not running now");
                return;
            }

            System.out.println("Shutting down the server...");
            shutdown();
        }

        private void printClientsNumber() {
            ChatDatabase database = chatServer.getDatabase();
            int clientsNumbers = database.getClientsNumber();
            System.out.println(String.format("Clients: %d/%d", clientsNumbers, maxClientsNumber));
        }

        private void printServerInfo() {
            System.out.println(String.format("Server name: %s", serverName));
            System.out.println(String.format("Server address: %s:%d", hostname, port));
            System.out.println(String.format("Server status: %s", isRunning() ? "online" : "offline"));
            if (isRunning()) {
                printClientsNumber();
            }
        }

        private void printClientsList() {
            if (!isRunning()) {
                System.err.println("Error: Server is not running");
                return;
            }

            ChatDatabase database = chatServer.getDatabase();
            List<String> clients = database.getClientsNames();

            System.out.println(String.format("Clients: %d/%d", clients.size(), maxClientsNumber));
            for (String clientName: clients) {
                System.out.println(String.format("* %s", clientName));
            }
        }

        private void exitApp() {
            if (isRunning()) {
                System.out.println("Error: server is running now; close the server before exiting");
                return;
            }
            exitFlag = true;
        }
    }
}

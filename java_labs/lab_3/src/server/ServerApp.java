package server;

public class ServerApp {
    private static final int REQUIRED_ARGS_NUM = 2;
    private static final int ERROR_CODE = 1;

    private static void printUsage() {
        System.err.println("Required args: <host name> <port>");
    }

    public static void main(String[] args) {
        if (args.length != REQUIRED_ARGS_NUM) {
            printUsage();
            System.exit(ERROR_CODE);
        }

        try (Server server = new Server()) {
            String hostName = args[0];
            int port = Integer.parseInt(args[1]);

            server.init(hostName, port);
            server.run();
        } catch (NumberFormatException ex) {
            System.err.println("Failed to parse port: " + ex.getMessage());
            System.exit(ERROR_CODE);
        } catch (Exception ex) {
            System.err.println("Server application error:");
            ex.printStackTrace();
            System.exit(ERROR_CODE);
        }
    }
}

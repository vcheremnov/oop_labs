package client;

public class ClientApp {
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

        try (Client client = new Client()) {
            String hostName = args[0];
            int port = Integer.parseInt(args[1]);

            client.init(hostName, port);
            client.run();
        } catch (NumberFormatException ex) {
            System.err.println("Failed to parse port: " + ex.getMessage());
            System.exit(ERROR_CODE);
        } catch (Exception ex) {
            System.err.println("Client application error:");
            ex.printStackTrace();
            System.exit(ERROR_CODE);
        }
    }
}

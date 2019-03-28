package client;

import java.io.BufferedReader;
import java.io.IOException;
import java.io.InputStreamReader;
import java.net.InetAddress;
import java.net.InetSocketAddress;
import java.net.SocketAddress;
import java.nio.ByteBuffer;
import java.nio.channels.SocketChannel;

public class Client implements AutoCloseable {
    private SocketChannel socketChannel;
    private ByteBuffer buffer;

    public void init(String hostName, int port) throws IOException {
        InetAddress address = InetAddress.getByName(hostName);
        SocketAddress socketAddress = new InetSocketAddress(address, port);

        socketChannel = SocketChannel.open(socketAddress);
    }

    public void run() throws IOException {
        try (BufferedReader reader = new BufferedReader(new InputStreamReader(System.in))) {
            while (true) {
                String msg = reader.readLine();
                sendMessage(msg);
                if (msg.equals("end")) {
                    System.out.println("Closed the connection");
                    break;
                }
                String response = getResponse();
                System.out.println("Response: " + response);
            }
        }
    }

    private void sendMessage(String msg) throws IOException {
        buffer = ByteBuffer.wrap(msg.getBytes());
        socketChannel.write(buffer);
        buffer.clear();
    }

    private String getResponse() throws IOException {
        socketChannel.read(buffer);
        return new String(buffer.array());
    }

    public void close() throws IOException {
        buffer = null;
        if (socketChannel != null) {
            socketChannel.close();
        }
    }
}

package client;

import java.io.BufferedReader;
import java.io.IOException;
import java.io.InputStreamReader;
import java.net.InetAddress;
import java.net.InetSocketAddress;
import java.net.SocketAddress;
import java.nio.ByteBuffer;
import java.nio.CharBuffer;
import java.nio.channels.SocketChannel;
import java.nio.charset.Charset;
import java.nio.charset.CharsetDecoder;
import java.nio.charset.CharsetEncoder;

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
                if (msg.equals("end")) {
                    System.out.println("Closed the connection");
                    break;
                }
                if (msg.equals("send")) {
                    sendMessage("");
                }
            }
        }
    }

    private void sendMessage(String msg) throws IOException {
        // get message body
        String messageString = "";
        CharsetEncoder encoder = Charset.forName("UTF-8").newEncoder();
        CharBuffer charBuffer = CharBuffer.wrap(messageString);
        ByteBuffer messageBody = encoder.encode(charBuffer);
        // get header body
        int messageLength = messageBody.limit();
        ByteBuffer headerBody = ByteBuffer.allocate(Integer.BYTES);
        headerBody.putInt(messageLength);
        headerBody.flip();

        ByteBuffer buf = ByteBuffer.allocate(messageBody.limit() + headerBody.limit());
        buf.put(headerBody);
        buf.put(messageBody);
        buf.flip();

        int bytesWritten = socketChannel.write(buf);
        System.out.println("Bytes written: " + bytesWritten);
//        buffer.clear();
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

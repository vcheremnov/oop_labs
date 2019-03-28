package server;

import java.io.IOException;
import java.io.ObjectInputStream;
import java.net.InetAddress;
import java.net.InetSocketAddress;
import java.net.SocketAddress;
import java.net.UnknownHostException;
import java.nio.ByteBuffer;
import java.nio.channels.*;
import java.util.Iterator;
import java.util.Set;

public class Server implements AutoCloseable {
    private ServerSocketChannel serverSocketChannel;
    private Selector selector;
    private ByteBuffer buffer;
    private static final int BUFFER_SIZE = 1024;

    public void init(String hostName, int port) throws IOException {
        InetAddress address = InetAddress.getByName(hostName);
        SocketAddress socketAddress = new InetSocketAddress(address, port);

        selector = Selector.open();

        serverSocketChannel = ServerSocketChannel.open();
        serverSocketChannel.configureBlocking(false);
        serverSocketChannel.bind(socketAddress);
        serverSocketChannel.register(selector, SelectionKey.OP_ACCEPT);

        buffer = ByteBuffer.allocate(BUFFER_SIZE);
    }

    public void run() throws IOException {
        while (true) {
            selector.select();
            Set<SelectionKey> selectedKeys = selector.selectedKeys();
            Iterator<SelectionKey> keyIterator = selectedKeys.iterator();
            while (keyIterator.hasNext()) {
                SelectionKey key = keyIterator.next();
                keyIterator.remove();
                if (!key.isValid()) {
                    continue;
                }
                if (key.isAcceptable()) {
                    acceptConnection();
                }
                if (key.isReadable()) {
                    processMessage(key);
                }
            }
        }
    }

    private void acceptConnection() throws IOException {
        SocketChannel socketChannel = serverSocketChannel.accept();
        socketChannel.configureBlocking(false);
        socketChannel.register(selector, SelectionKey.OP_READ);

        System.out.println("Connection accepted: " + socketChannel.getRemoteAddress());
    }

    private void processMessage(SelectionKey key) throws IOException {
        SocketChannel socketChannel = (SocketChannel) key.channel();
        socketChannel.read(buffer);
        if (new String(buffer.array()).trim().equals("end")) {
            System.out.println("Connection closed: " + socketChannel.getRemoteAddress());
            key.cancel();
            socketChannel.close();
        } else {
            buffer.flip();
            socketChannel.write(buffer);
        }
        buffer.clear();
    }

    private void respond(SelectionKey key) {

    }

    @Override
    public void close() throws IOException {
        if (selector != null) {
            for (SelectionKey key: selector.keys()) {
                SelectableChannel channel =  key.channel();
                channel.close();
            }
            selector.close();
            selector = null;
        }
        if (serverSocketChannel != null) {
            serverSocketChannel.socket().close();
            serverSocketChannel.close();
            serverSocketChannel = null;
        }
        if (buffer != null) {
            buffer = null;
        }
    }
}

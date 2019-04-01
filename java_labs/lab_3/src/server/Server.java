package server;

import com.google.gson.Gson;
import com.google.gson.GsonBuilder;
import com.google.gson.JsonSyntaxException;

import java.io.IOException;
import java.net.InetAddress;
import java.net.InetSocketAddress;
import java.net.SocketAddress;
import java.nio.ByteBuffer;
import java.nio.CharBuffer;
import java.nio.channels.*;
import java.nio.charset.CharacterCodingException;
import java.nio.charset.Charset;
import java.nio.charset.CharsetDecoder;
import java.nio.charset.CharsetEncoder;
import java.util.*;

public class Server implements AutoCloseable {
    private MessageHandler messageHandler = new MessageHandler();
    private ServerSocketChannel serverSocketChannel;
    private Selector selector;
    private static final int BUFFER_SIZE = 1024;

    public void init(String hostName, int port) throws IOException {
        InetAddress address = InetAddress.getByName(hostName);
        SocketAddress socketAddress = new InetSocketAddress(address, port);

        selector = Selector.open();

        serverSocketChannel = ServerSocketChannel.open();
        serverSocketChannel.configureBlocking(false);
        serverSocketChannel.bind(socketAddress);
        serverSocketChannel.register(selector, SelectionKey.OP_ACCEPT);

    }

    public void run() throws IOException {
        System.out.println("Server is running");
        while (!Thread.currentThread().isInterrupted()) {
            selector.select();
            Set<SelectionKey> selectedKeys = selector.selectedKeys();
            Iterator<SelectionKey> keyIterator = selectedKeys.iterator();
            while (keyIterator.hasNext()) {
                SelectionKey key = keyIterator.next();
                keyIterator.remove();
                if (!key.isValid()) {
                    continue;
                }
                try {
                    if (key.isAcceptable()) {
                        acceptConnection();
                    }
                    if (key.isConnectable()) {
                        finishConnection(key);
                    }
                    if (key.isReadable()) {
                        readMessage(key);
                    }
                } catch (CharacterCodingException ex) {
                    System.err.println("Character coding error: " + ex.getMessage());
                    ex.printStackTrace();
                } catch (Exception ex) {
                    System.err.println("An error has occurred: " + ex.getMessage());
                    ex.printStackTrace();
                    closeConnection(key);
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

    private void finishConnection(SelectionKey key) throws IOException {
        SocketChannel socketChannel = (SocketChannel) key.channel();
        socketChannel.finishConnect();
    }

    private void closeConnection(SelectionKey key) {
        try {
            key.cancel();
            key.channel().close();
        } catch (IOException e) {
            System.err.println("Error while closing the connection ");
            e.printStackTrace();
        }
    }

    static class Attachment {
        ByteBuffer inputBuffer;
        ByteBuffer outputBuffer;
        Message message;

        Attachment(int bufferSize) {
            inputBuffer = ByteBuffer.allocate(bufferSize);
            outputBuffer = ByteBuffer.allocate(bufferSize);
            message = new Message();
        }
    }

    static class Message {
        static final String charsetName;
        static final int HEADER_SIZE = Integer.BYTES;
        static final CharsetEncoder encoder;
        static final CharsetDecoder decoder;

        static {
            charsetName = "UTF-8";
            encoder = Charset.forName(charsetName).newEncoder();
            decoder = Charset.forName(charsetName).newDecoder();
        }

        private byte[] header = new byte[HEADER_SIZE];
        private byte[] message = null;
        private String stringMessage = null;

        private int headerPos = 0;
        private int messagePos = 0;

        boolean messageIsReady() {
            return message != null && messagePos == message.length;
        }

        private boolean headerIsReady() {
            return headerPos == HEADER_SIZE;
        }

        String getMessageAsString() {
            return stringMessage;
        }

        void clear() {
            message = null;
            stringMessage = null;
            headerPos = messagePos = 0;
        }

        void readMessageFrom(ByteBuffer buffer) throws CharacterCodingException {
            if (!headerIsReady()) {
                readHeaderFrom(buffer);
            }
            if (headerIsReady() && !messageIsReady() && buffer.remaining() > 0) {
                int bytesAvailable = buffer.remaining();
                int bytesToRead = Math.min(message.length - messagePos, bytesAvailable);
                buffer.get(message, messagePos, bytesToRead);
                messagePos += bytesToRead;
            }
            if (messageIsReady() && stringMessage == null) {
                decodeMessage();
            }
        }

        private void decodeMessage() throws CharacterCodingException {
            ByteBuffer wrappedMessage = ByteBuffer.wrap(message);
            stringMessage = decoder.decode(wrappedMessage).toString();
        }

        private void readHeaderFrom(ByteBuffer buffer) {
            int bytesAvailable = buffer.remaining();
            int bytesToRead = Math.min(HEADER_SIZE - headerPos, bytesAvailable);
            buffer.get(header, headerPos, bytesToRead);
            headerPos += bytesToRead;
            if (headerIsReady()) {
                analyzeHeader();
            }
        }

        private void analyzeHeader() {
            int messageLength = ByteBuffer.wrap(header).getInt();
            message = new byte[messageLength];
        }
    }

    private void readMessage(SelectionKey key) throws IOException {
        SocketChannel socketChannel = (SocketChannel) key.channel();
        Attachment attachment = (Attachment) key.attachment();
        if (attachment == null) {
            attachment = new Attachment(BUFFER_SIZE);
            key.attach(attachment);
        }

        attachment.inputBuffer.clear();
        int bytesRead = socketChannel.read(attachment.inputBuffer);
        if (bytesRead == -1) {
            closeConnection(key);
            return;
        }
        attachment.inputBuffer.flip();

        while (attachment.inputBuffer.remaining() > 0) {
            // TODO: add try catch for char coding exception and clear the message there
            attachment.message.readMessageFrom(attachment.inputBuffer);
            if (attachment.message.messageIsReady()) {
                processMessage(attachment.message.getMessageAsString());
                attachment.message.clear();
            }
        }
    }

    class Student {
        String name;
        int age;
    }

    private void processMessage(String msg) {
        GsonBuilder gsonBuilder = new GsonBuilder();
        gsonBuilder.setPrettyPrinting();

        System.out.println(msg);
        Gson gson = gsonBuilder.create();
        try {
            Student student = gson.fromJson(msg, Student.class);
            System.out.println("Received a message:");
            System.out.println(gson.toJson(student));
        } catch (JsonSyntaxException ex) {
            System.err.println("Json syntax error:");
            ex.printStackTrace();
        }
    }

    private void respond(SelectionKey key) {

    }

    @Override
    public void close() throws IOException {
        if (selector != null) {
            for (SelectionKey key: selector.keys()) {
                closeConnection(key);
            }
            selector.close();
            selector = null;
        }
        if (serverSocketChannel != null) {
            serverSocketChannel.socket().close();
            serverSocketChannel.close();
            serverSocketChannel = null;
        }
        System.out.println("Server has been successfully closed");
    }
}

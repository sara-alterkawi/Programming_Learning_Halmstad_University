import java.io.*;
import java.net.Socket;
import java.util.Map;
import java.util.Set;
import java.util.TreeSet;

/**
 * This class represents a user connected to the server.
 * It handles communication with the user by creating a separate thread for each user.
 */
public class CustomUserThread extends Thread {
    private ObjectInputStream objectInputStream;
    private ObjectOutputStream objectOutputStream;
    private final Socket userSocket;
    private final Server customServer;

    /**
     * Constructor for the CustomUserThread class.
     *
     * @param clientSocket The socket reserved by the client.
     * @param server       The running server object to send global updates.
     */
    public CustomUserThread(Socket clientSocket, Server server) {
        this.customServer = server;
        this.userSocket = clientSocket;
        try {
            objectInputStream = new ObjectInputStream(userSocket.getInputStream());
            objectOutputStream = new ObjectOutputStream(userSocket.getOutputStream());
        } catch (IOException ioe) {
            ioe.printStackTrace();
        }
    }

    /**
     * This method of the Thread class is used to receive updates from the client
     * to update the saved state in memory and send the updates to all clients.
     */
    @Override
    public void run() {
        try {
            while (!interrupted()) {
                @SuppressWarnings("unchecked")
                TreeSet<FileInfo> fileInfoSet = (TreeSet<FileInfo>) objectInputStream.readObject();
                customServer.updateState(fileInfoSet);
            }
        } catch (EOFException e) {
            System.out.println("EOF:" + e.getMessage());
        } catch (IOException e) {
            System.out.println("IO:" + e.getMessage());
        } catch (ClassNotFoundException e) {
            e.printStackTrace();
        } finally {
            try {
                userSocket.close();
            } catch (IOException ioe) {
                ioe.printStackTrace();
            }
        }
    }

    /**
     * This method is used by the server to send updates to the connected users.
     *
     * @param update The updated files as a map of string (update name) and set of FileInfo (the updated files).
     */
    public void send(Map<String, Set<FileInfo>> update) {
        try {
            objectOutputStream.writeObject(update);
        } catch (IOException e) {
            e.printStackTrace();
        }
    }
}

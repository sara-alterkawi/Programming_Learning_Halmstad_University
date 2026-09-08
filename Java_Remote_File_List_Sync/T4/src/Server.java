import java.io.IOException;
import java.net.ServerSocket;
import java.util.*;

/**
 * This class represents a server that creates a server socket and manages the
 * sharing of directory state and its updates among connected clients.
 */
public class Server extends Thread {

    public static final int PORT_NUMBER = 9090;
    /**
     * The path of the desired directory.
     */
    public static final String DIRECTORY_PATH = "C:\\Users\\" + System.getProperty("user.name") + "\\Downloads";
    private final ArrayList<CustomUserThread> connectedUsers = new ArrayList<>();
    private final Thread sendThread;
    private final Queue<Map<String, Set<FileInfo>>> sendQueue;
    private ServerSocket serverSocket;
    private Set<FileInfo> filesInfo;

    /**
     * This constructor of the Server class.
     * It initializes the server by getting the initial state of the directory,
     * starting the socket, and creating a sender thread to send updates
     * whenever the sending queue is not empty.
     */
    public Server() {
        filesInfo = FileTools.getAllFiles(DIRECTORY_PATH);
        sendQueue = new LinkedList<>();
        try {
            serverSocket = new ServerSocket(PORT_NUMBER);
        } catch (IOException e) {
            e.printStackTrace();
        }
        System.out.println("The server is now operational and awaiting connections..");
        sendThread = new Thread(() -> {
            while (!isInterrupted())
                if (!sendQueue.isEmpty()) {
                    Map<String, Set<FileInfo>> map = sendQueue.remove();
                    for (CustomUserThread user : connectedUsers)
                        user.send(map);
                }
        });
        sendThread.start();
    }

    /**
     * This method runs on a server thread to accept and construct new user
     * connections.
     */
    @Override
    public void run() {
        try {
            while (!interrupted()) {
                CustomUserThread user = new CustomUserThread(serverSocket.accept(), this);
                user.start();
                connectedUsers.add(user);
            }
        } catch (IOException e) {
            System.out.println("Error accepting connections: " + e.getMessage());
        }
    }

    /**
     * Stops all active threads and closes the socket.
     */

    public void stopServer() {
        for (CustomUserThread user : connectedUsers)
            user.interrupt();
        sendThread.interrupt();
        try {
            serverSocket.close();
        } catch (IOException e) {
            e.printStackTrace();
        }
        interrupt();
    }

    /**
     * This method is used when there is any new state of the directory files,
     * so we can then search for differences and then send them back to the client.
     * 
     * @param newSet the new state of the directory files
     */

    public void updateState(TreeSet<FileInfo> newSet) {
        Map<String, Set<FileInfo>> map = new TreeMap<>();

        Set<FileInfo> addedFiles = FileTools.addedFiles(filesInfo, newSet);
        if (!addedFiles.isEmpty()) {
            map.put("Added Files:", addedFiles);
        }
        Set<FileInfo> deletedFiles = FileTools.deletedFiles(filesInfo, newSet);
        if (!deletedFiles.isEmpty()) {
            map.put("Deleted Files:", deletedFiles);
        }
        Set<FileInfo> updatedFiles = FileTools.updatedFiles(filesInfo, newSet);
        if (!updatedFiles.isEmpty()) {
            map.put("Updated Files:", updatedFiles);
        }
        sendQueue.add(map);
        filesInfo = newSet;
    }

    public static void main(String[] args) throws InterruptedException {
        Server server = new Server();
        server.start();
        new Scanner(System.in).next();
        server.stopServer();
        server.join();
    }
}
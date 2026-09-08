import java.io.*;
import java.net.Socket;
import java.net.UnknownHostException;
import java.util.Scanner;
import java.util.TreeMap;
import java.util.TreeSet;

/**
 * This class handles the client-side portion of the program, enabling users to send the directory state to the server upon request.
 */
public class Client {
    static class Receiver extends Thread {
        private final ObjectInputStream inputStream;

        public Receiver(ObjectInputStream inputStream) {
            this.inputStream = inputStream;
        }

        /**
         * This Thread class method is designed to receive updates from the server and subsequently display them to the user.
         */
        @Override
        public void run() {
            try {
                while (!isInterrupted()) {
                    @SuppressWarnings("unchecked")
                    TreeMap<String, TreeSet<FileInfo>> updates = (TreeMap<String, TreeSet<FileInfo>>) inputStream.readObject();
                    for (String key : updates.keySet()) {
                        System.out.println(key + ":\n" + updates.get(key));
                    }
                }
            } catch (IOException | ClassNotFoundException e) {
                e.printStackTrace();
            }
        }
    }

    /**
     * Establishes a connection with a server to send directory updates and receive messages.
     */
    public static void main(String[] args) {
        Socket clientSocket = null;
        try {
            clientSocket = new Socket("localhost", Server.PORT_NUMBER);
            ObjectOutputStream out = new ObjectOutputStream(clientSocket.getOutputStream());
            ObjectInputStream in = new ObjectInputStream(clientSocket.getInputStream());
            new Receiver(in).start();
            Scanner scanner = new Scanner(System.in);
            String input = "";
            while (!input.equals("q")){
                System.out.println("u -> update.\nq -> exit.");
                input = scanner.nextLine();
                if (input.equals("u"))
                    out.writeObject(FileTools.getAllFiles(Server.DIRECTORY_PATH));
            }
        } catch (UnknownHostException e) {
            System.out.println("Sock: " + e.getMessage());
        } catch (EOFException e) {
            System.out.println("EOF: " + e.getMessage());
        } catch (IOException e) {
            System.out.println("IO: " + e.getMessage());
        } finally {
            if (clientSocket != null) {
                try {
                    clientSocket.close();
                } catch (IOException ioe) {
                    System.out.println("close failed: " + ioe.getMessage());
                }
            }
        }
    }
}

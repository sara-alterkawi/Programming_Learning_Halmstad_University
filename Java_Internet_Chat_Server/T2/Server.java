package T2;

import java.io.IOException;
import java.net.ServerSocket;
import java.net.Socket;
import java.util.ArrayList;
import java.util.List;

public class Server {
  // Specify port number for the server to listen on.
  private static final int PORT = 8700;
  // List of connected clients using UserHandler instances.
  private List<UserHandler> clients = new ArrayList<>();

  public static void main(String[] args) {
    // Start the server
    new Server().startServer();
  }

  // Start the server
  public void startServer() {
    try (ServerSocket serverSocket = new ServerSocket(PORT)) {
      System.out.println("Server started at port: " + PORT + "\nWaiting for clients...");
      // Continuously listen for incoming client connections.
      while (true) {
        // Accept a new client connection.
        Socket clientSocket = serverSocket.accept();
        System.out.println("New client connected: " + clientSocket);
        UserHandler UserHandler = new UserHandler(clientSocket, this);
        // Add the UserHandler to the list of clients
        clients.add(UserHandler);
        // Handle the client communication
        Thread thread = new Thread(UserHandler);
        thread.start();
      }
    } catch (IOException e) {
      e.printStackTrace();
    }
  }

  // To send message to all clients except the sender.
  public void sendMessage(String message, UserHandler sender) {
    for (UserHandler client : clients) {
      // Send the message to all clients except the sender.
      if (client != sender) {
        client.sendMessage(message);
      }
    }
  }

  // Remove client from the list of connected clients.
  public void removeClient(UserHandler client) {
    clients.remove(client);
    System.out.println("Client disconnected: " + client.getClientSocket());
  }
}

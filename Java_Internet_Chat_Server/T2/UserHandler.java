package T2;

import java.io.IOException;
import java.io.PrintWriter;
import java.net.Socket;
import java.util.Scanner;

public class UserHandler implements Runnable {
  // Socket representing the connection to a client.
  private Socket clientSocket;
  // Manage the communication between clients.
  private Server server;
  // Send messages to the client.
  private PrintWriter out;
  // Receive messages from the client.
  private Scanner in;
  // The name or nickname chosen by the client.
  private String username;

  // UserHandler constructor
  public UserHandler(Socket socket, Server server) {
    this.clientSocket = socket;
    this.server = server;
  }

  // Manage the communication with the client.
  @Override
  public void run() {
    try {
      out = new PrintWriter(clientSocket.getOutputStream(), true);
      in = new Scanner(clientSocket.getInputStream());
      // Prompt the client to enter a username.
      out.println("Welcome to the chat! Enter your name or nickname:");
      username = in.nextLine();
      // Announce the client's entry to the chat.
      server.sendMessage(username + " has joined the chat.", this);
      // Continuously listen for and send messages from the client until "/exit"
      String clientMessage;
      while (true) {
        clientMessage = in.nextLine();
        if (clientMessage.equalsIgnoreCase("/exit")) {
          break;
        }
        // Send client's message to all connected clients.
        server.sendMessage(username + ": " + clientMessage, this);
      }
    } catch (IOException e) {
      e.printStackTrace();
    } finally {
      try {
        // Notify clients about the departure of the current client.
        server.sendMessage(username + " has left the chat", this);
        // Close the clientSocket.
        clientSocket.close();
      } catch (IOException e) {
        e.printStackTrace();
      }
      // Remove the client from the server's list of active clients.
      server.removeClient(this);
    }
  }

  // Send message to the connected client.
  public void sendMessage(String message) {
    out.println(message);
  }

  // Retrieve client's socket.
  public Socket getClientSocket() {
    return clientSocket;
  }

  // Retrieve client's username.
  public String getUsername() {
    return username;
  }
}

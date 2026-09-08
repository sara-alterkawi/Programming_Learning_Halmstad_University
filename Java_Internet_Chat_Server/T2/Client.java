package T2;

import java.io.IOException;
import java.io.PrintWriter;
import java.net.Socket;
import java.util.Scanner;

public class Client {
  // Server details
  private static final String ServerIP = "localhost";
  private static final int PORT = 8700;

  // Start the chat client
  public static void main(String[] args) {
    new Client().startClient();
  }

  // Initiate the chat client
  public void startClient() {
    try {
      // Connect to the server
      Socket socket = new Socket(ServerIP, PORT);
      // Set up input and output streams
      Scanner serverIn = new Scanner(socket.getInputStream());
      PrintWriter out = new PrintWriter(socket.getOutputStream(), true);
      Scanner consoleIn = new Scanner(System.in);
      // Read and print server welcome message
      String welcomeMessage = serverIn.nextLine();
      System.out.println(welcomeMessage);
      // Enter username and send it to the server
      String username = consoleIn.nextLine();
      out.println(username);
      // Start receiving messages from the server
      Thread receiveThread = new Thread(() -> {
        while (serverIn.hasNextLine()) {
          String serverMessage = serverIn.nextLine();
          System.out.println(serverMessage);
        }
      });
      receiveThread.start();
      // Start sending messages to the server
      while (true) {
        String message = consoleIn.nextLine();
        out.println(message);
        // If the user types "/quit", exit the loop
        if (message.equalsIgnoreCase("/quit")) {
          break;
        }
      }
      // Close resources
      receiveThread.join();
      // Wait for the receive thread to finish
      socket.close();
      serverIn.close();
      out.close();
      consoleIn.close();
    } catch (IOException | InterruptedException e) {
      e.printStackTrace();
    }
  }
}

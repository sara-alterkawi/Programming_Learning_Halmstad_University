package org.example.prime;


import java.util.Scanner;

public class Main {
    private static final PrimeClient client = new PrimeClient();

    /**
     * This code is working as a client side.
     * so we can:
     * 1 - send number from user to the server
     * 2 - check state of number from the server
     * @param args notUsed
     */
    public static void main(String[] args) {
        System.out.print("Port:");
        Scanner scanner = new Scanner(System.in);
        int port = scanner.nextInt();
        PrimeClient.REST_URI = String.format("http://localhost:%d/primes-api/webapi", port);
        while (true) {
            System.out.println(" \"q\" to exit.\n \"n\" to add new.\n \"?\" to ask.");
            String s = scanner.nextLine();
            while (s.equals(""))
                s = scanner.nextLine();
            if (s.equals("q"))
                break;
            if (s.equals("n")) {
                int number = scanner.nextInt();
                System.out.println(client.addNumber(number).readEntity(String.class));
            }
            else if(s.equals("?")){
                int number = scanner.nextInt();
                System.out.println(client.isPrime(number));
            }
        }
    }
}


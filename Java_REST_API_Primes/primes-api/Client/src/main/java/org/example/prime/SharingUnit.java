package org.example.prime;

import javax.ws.rs.client.Client;
import javax.ws.rs.client.ClientBuilder;
import javax.ws.rs.core.MediaType;
import java.util.ArrayList;

/**
 * This class is to facilitate the sharing of numbers among servers.
 */
public class SharingUnit {
    /**
     * The array containing available ports.
     */
    private static final int[] ports = {8081, 8082};
    /**
     * The present port number utilized by the server to prevent self-sharing.
     */
    private final int curPort;
    /**
     * The server acts as a client to connect with other servers.
     */
    private final Client client;

    /**
     * Constructor that triggers an automatic refresh every 5 seconds in a separate thread.
     *
     * @param curPort current port number of the server in use
     */
    public SharingUnit(int curPort) {
        this.curPort = curPort;
        this.client = ClientBuilder.newClient();
        startAutoRefresh();
    }

    /**
     * Starts the automatic refresh thread.
     */
    private void startAutoRefresh() {
        new Thread(() -> {
            while (true) {
                refresh();
                try {
                    Thread.sleep(5000);
                } catch (InterruptedException e) {
                    e.printStackTrace();
                }
            }
        }).start();
    }

    /**
     * Refreshes the data sets by iterating through each server port and retrieving their respective lists of numbers.
     */
    private void refresh() {
        for (int port : ports) {
            if (port != curPort) {
                PrimeService.primes.addAll(getListFromPort(port, "primes"));
                PrimeService.nonPrimes.addAll(getListFromPort(port, "nonPrimes"));
            }
        }
    }

    /**
     * Retrieves a list from a server using a REST GET method, providing the list name and server number.
     *
     * @param port the server we want to get data from
     * @param type the name of the list we want to get
     * @return an ArrayList of numbers from the wanted server
     */
    private ArrayList<Integer> getListFromPort(int port, String type) {
        try {
            String endpoint = String.format("http://localhost:%d/primes-api/webapi/primes/listPrimes/%s", port, type);
            String response = client.target(endpoint).request(MediaType.TEXT_PLAIN).get(String.class);
            return getAsIntegers(response);
        } catch (Exception e) {
            System.out.println("Cannot get " + type + " list from " + port);
            return new ArrayList<>();
        }
    }

    /**
     * Static method for converting a String (Array_Form) into an ArrayList.
     *
     * @param s the string we want to transform
     * @return the desired ArrayList
     */
    private static ArrayList<Integer> getAsIntegers(String s) {
        String[] strings = s.replaceAll("\\[", "")
                .replaceAll("]", "")
                .replaceAll(" ", "")
                .split(",");
        ArrayList<Integer> integers = new ArrayList<>();
        for (String ss : strings)
            if (!ss.equals(""))
                integers.add(Integer.parseInt(ss));
        return integers;
    }
}

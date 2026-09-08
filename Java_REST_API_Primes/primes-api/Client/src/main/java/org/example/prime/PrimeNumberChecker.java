package org.example.prime;

import java.net.URI;

public class PrimeNumberChecker {


    private static final String PRIME_NUMBERS_API_URL = "https://primes.io/api/v1/primes/check/";

    /*public static boolean checkPrime(Integer number) throws Exception {
        String url = PRIME_NUMBERS_API_URL + "?number=" + number;
        Boolean isPrime= false;
        try {
            HttpClient httpClient = HttpClient.newHttpClient();
            // Construct the URL with the number to check as a query parameter
            URI uri = URI.create(url);
            // Create an HTTP request
            HttpRequest request = HttpRequest.newBuilder()
                    .uri(uri)
                    .GET()
                    .build();
            // Send the request and receive the response
            HttpResponse<String> response = httpClient.send(request, HttpResponse.BodyHandlers.ofString());
            // Check the response status
            if (response.statusCode() == 200) {
                // Parse the response body (assuming it's a simple "true" or "false" indicating prime status)
              isPrime= Boolean.parseBoolean(response.body());
            } else {
                System.out.println("Error: " + response.statusCode() + ", " + response.body());
            }
        } catch (Exception e) {
            e.printStackTrace();
        }
        return isPrime;
    }*/
    public static boolean checkPrime(Integer number){
        if (number <= 1) {
            return false; // 0 and 1 are not prime
        }

        // Check for divisibility by integers from 2 to the square root of the number
        for (int i = 2; i * i <= number; i++) {
            if (number % i == 0) {
                return false; // Number is divisible, not a prime
            }
        }

        return true; // Number is prime
    }
}

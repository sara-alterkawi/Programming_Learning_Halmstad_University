package org.example.prime;

import java.io.File;
import java.io.FileWriter;
import java.io.IOException;
import java.io.RandomAccessFile;
import java.util.ArrayList;
import java.util.List;

public class PrimeService {

    public static List<Integer> primes = new ArrayList<>();
    public static List<Integer> nonPrimes = new ArrayList<>();

    static {
        primes.add(2);
        primes.add(3);
        primes.add(5);
    }
    static {
        nonPrimes.add(1);
        nonPrimes.add(4);
        nonPrimes.add(9);
    }

    public static String getState(Integer number){
        Boolean exist= false;
        if (primes.contains(number))
            return number + " is a prime number!!";
        else if (nonPrimes.contains(number))
            return number + " is not a prime number!!";
        else
            PrimeClient.addNumber(number);
            return number + " The number doesn't exist! I will add it for you :) ";
    }

    public static String addNumber(Integer number){
        Boolean isPrime = null;
        try {
            isPrime = PrimeNumberChecker.checkPrime(number);
        } catch (Exception e) {
            throw new RuntimeException(e);
        }
        if (isPrime) {
            primes.add(number);
            savePrimeData(primes);
            return number + "has been added to Prime List";
        } else {
            nonPrimes.add(number);
            saveNonPrimeData(nonPrimes);
            return number + "has been added to non Prime List";
        }
    }

    public static String getListPrimes(){
        System.out.println(primes.toString());
        return primes.toString();
    }

    public static String getListNonPrimes(){
        return nonPrimes.toString();
    }

    public static void savePrimeData(List<Integer> numbers){
        String newData = numbers.toString();
        File fileName = new File("/src/main/resources/primes.txt");
        /*try (RandomAccessFile file = new RandomAccessFile(fileName, "rw")) {
            file.setLength(0);
            System.out.println("File content truncated successfully.");
        } catch (IOException e) {
            System.err.println("Error truncating file: " + e.getMessage());
        }*/
        try (FileWriter writer = new FileWriter(fileName, true)) {
            writer.write(newData);
            System.out.println("writing....");

        } catch (IOException e) {
            System.err.println("Error writing data to file: " + e.getMessage());
        }

        System.out.println("Data appended successfully to existing text file.");
    }

    public static void saveNonPrimeData(List<Integer> numbers){
        String newData = numbers.toString();
        //System.out.println("Current Working Directory: " + System.getProperty("user.dir"));
        File fileName = new File("src/main/resources/nonPrimes.txt");
        /*try (RandomAccessFile file = new RandomAccessFile(fileName, "rw")) {
            file.setLength(0);
            System.out.println("File content truncated successfully.");
        } catch (IOException e) {
            System.err.println("Error truncating file: " + e.getMessage());
        }*/
        try (FileWriter writer = new FileWriter(fileName, true)) {
            writer.write(newData);

            System.out.println("writing....");
        } catch (IOException e) {
            System.err.println("Error writing data to file: " + e.getMessage());
        }

        System.out.println("Data appended successfully to existing text file.");
    }

}

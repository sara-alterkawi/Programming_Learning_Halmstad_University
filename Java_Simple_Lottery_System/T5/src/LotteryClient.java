import java.io.*;
import java.net.Socket;
import java.time.Year;
import java.util.ArrayList;
import java.time.LocalDateTime;
import java.util.Scanner;
import java.util.regex.Matcher;
import java.util.regex.Pattern;

public class LotteryClient {
    private static boolean isValidEmail(String email) {
        String emailRegex = "^[a-zA-Z0-9_+&*-]+(?:\\.[a-zA-Z0-9_+&*-]+)*@(?:[a-zA-Z0-9-]+\\.)+[a-zA-Z]{2,7}$";
        Pattern pattern = Pattern.compile(emailRegex);
        Matcher matcher = pattern.matcher(email);
        return matcher.matches();
    }

    private static void handleRegData(ObjectInputStream in, ObjectOutputStream out, Scanner scanner) throws IOException {
        System.out.print("Welcome to register in our lottery system\nEnter email: ");
        String email = scanner.nextLine();

        // Limit the number of registrations to a maximum of 4
        final int maxRegistrations = 4;
        int numOfRegistrations = 0;

        while (!isValidEmail(email)) {
            System.out.println("Invalid email. Please enter a valid email address:");
            email = scanner.nextLine();
        }

        while (numOfRegistrations <= 0 || numOfRegistrations > maxRegistrations) {
            System.out.print("Enter number of registrations (maximum 4): ");
            try {
                numOfRegistrations = Integer.parseInt(scanner.nextLine());
                if (numOfRegistrations <= 0 || numOfRegistrations > maxRegistrations) {
                    System.out.println("Invalid number. Please enter a value between 1 and 4.");
                }
            } catch (NumberFormatException e) {
                System.out.println("Invalid input. Please enter a numeric value.");
            }
        }

        ArrayList<UserRegistration> registrations = new ArrayList<>();
        for (int i = 0; i < numOfRegistrations; i++) {
            System.out.println("Registration " + (i + 1) + ":");
            LocalDateTime date;
            while ((date = getRegData(scanner)) == null) {
                System.out.println("Invalid date. Please try again.");
            }

            int number = -1;
            while (number < 0 || number > 255) {
                System.out.print("Enter number (0-255): ");
                try {
                    number = Integer.parseInt(scanner.nextLine());
                    if (number < 0 || number > 255) {
                        System.out.println("Invalid number. Please enter a value between 0 and 255.");
                    }
                } catch (NumberFormatException e) {
                    System.out.println("Invalid input. Please enter a numeric value.");
                }
            }

            registrations.add(new UserRegistration(email, date, number));
        }

        out.writeObject("r");
        out.writeObject(registrations);

        String response;
        try {
            response = (String) in.readObject();
        } catch (ClassNotFoundException e) {
            response = "Error: Invalid response from server.";
        }
        System.out.println(response);
    }

    private static void handleHisData(ObjectInputStream in, ObjectOutputStream out, Scanner scanner) throws IOException, ClassNotFoundException {
        System.out.println("Enter the period for historical data:");
        LocalDateTime start = getRegData(scanner);
        LocalDateTime end = getRegData(scanner);

        out.writeObject("h");
        out.writeObject(start);
        out.writeObject(end);

        Object response = in.readObject();
        if (response instanceof String) {
            System.out.println((String) response);
        } else if (response instanceof ArrayList) {
            ArrayList<LotteryResult> historicalRecords = (ArrayList<LotteryResult>) response;
            for (LotteryResult record : historicalRecords) {
                System.out.println(record);
            }
        }
    }

    private static LocalDateTime getRegData(Scanner scanner) {
        int year, month, day, hour;
        LocalDateTime now = LocalDateTime.now();
        try {
            System.out.print("Enter year: ");
            year = scanner.nextInt();
            /*
            // Validate if the year is the current year
            int currentYear = Year.now().getValue();
            if (year != currentYear) {
                System.out.println("Invalid year. Please enter the current year: " + currentYear);
                scanner.nextLine();
                return null;
            }*/

            System.out.print("Enter month: ");
            month = scanner.nextInt();
            /*
            // Validate if the month is not in the past
            if (month < now.getMonthValue()) {
                System.out.println("Invalid month. Please enter a current or future month.");
                scanner.nextLine(); // Consume invalid input
                return null;
            }*/

            System.out.print("Enter day: ");
            day = scanner.nextInt();
            /*
            // Validate if the day is not in the past
            if (day < now.getDayOfMonth() && month == now.getMonthValue()) {
                System.out.println("Invalid day. Please enter a current or future day.");
                scanner.nextLine(); // Consume invalid input
                return null;
            }*/

            System.out.print("Enter hour: ");
            hour = scanner.nextInt();
            scanner.nextLine();
            /*LocalDateTime enteredDateTime = LocalDateTime.of(year, month, day, hour, 0);

            // Check if the entered date and time is in the past
            if (enteredDateTime.isBefore(now)) {
                System.out.println("Invalid date and time. Please enter a current or future date and time.");
                return null;
            }*/

            return LocalDateTime.of(year, month, day, hour, 0);
        } catch (Exception e) {
            scanner.nextLine(); // Consume invalid input
            System.out.println("Invalid input. Please enter valid date and time values.");
            return null;
        }
    }


    public static void main(String[] args) {
        try (Socket socket = new Socket("localhost", LotteryServer.PORT);
             ObjectOutputStream out = new ObjectOutputStream(socket.getOutputStream());
             ObjectInputStream in = new ObjectInputStream(socket.getInputStream());
             Scanner scanner = new Scanner(System.in)) {

            while (true) {
                System.out.println("Enter:\n'r' to Register.\n'h' to retrieve historical data.\n'q' to quit.");
                String input = scanner.nextLine();

                if ("r".equalsIgnoreCase(input)) {
                    handleRegData(in, out, scanner);
                } else if ("h".equalsIgnoreCase(input)) {
                    handleHisData(in, out, scanner);
                } else if ("q".equalsIgnoreCase(input)) {
                    break;
                } else {
                    System.out.println("Invalid input. Please try again.");
                }
            }

        } catch (IOException e) {
            System.err.println("Network error: " + e.getMessage());
        } catch (ClassNotFoundException e) {
            System.err.println("Error processing response from server: " + e.getMessage());
        }
    }

}

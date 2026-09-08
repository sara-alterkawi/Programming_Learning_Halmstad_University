import java.io.*;
import java.net.ServerSocket;
import java.net.Socket;
import java.time.LocalDateTime;
import java.time.temporal.ChronoUnit;
import java.util.*;

public class LotteryServer {
    public static final int PORT = 8080;
    private ServerSocket serverSocket;

    private final Map<LocalDateTime, List<UserRegistration>> comingReg;
    private final Random random = new Random();
    private double pool = 0.0;
    private NotifyMail notifyMail = new NotifyMail();

    public LotteryServer() {
        comingReg = new TreeMap<>();
        try {
            serverSocket = new ServerSocket(PORT);
        } catch (IOException e) {
            e.printStackTrace();
        }
        System.out.println("Lottery Server is ready..");

        Thread drawsThread = new Thread(() -> {
            while (!Thread.interrupted()) {
                try {
                    Thread.sleep(getMillisUntilNextHour());
                    makeDraw();
                } catch (InterruptedException e) {
                    e.printStackTrace();
                }
            }
        });
        drawsThread.start();
    }

    private long getMillisUntilNextHour() {
        LocalDateTime now = LocalDateTime.now();
        LocalDateTime nextHour = now.plusHours(1).truncatedTo(ChronoUnit.HOURS);
        return ChronoUnit.MILLIS.between(now, nextHour);
    }

    private void makeDraw() {
        LocalDateTime now = LocalDateTime.now();
        List<LocalDateTime> toBeRemoved = new ArrayList<>();
        for (LocalDateTime dateTime : comingReg.keySet()) {
            if (!dateTime.isAfter(now) && ChronoUnit.HOURS.between(dateTime, now) < 1) {
                int winningNumber = random.nextInt(256);
                List<UserRegistration> winners = new ArrayList<>();
                for (UserRegistration registration : comingReg.get(dateTime)) {
                    if (registration.getRegNum() == winningNumber) {
                        winners.add(registration);
                    }
                }
                double winnings = 0;
                if (!winners.isEmpty()) {
                    winnings = (pool + comingReg.get(dateTime).size() * 100) / winners.size();
                    for (UserRegistration winner : winners) {
                        notifyMail.notifyWinner(winner, winnings);
                    }
                    pool = 0;
                } else {
                    pool += 100 * comingReg.get(dateTime).size();
                }
                toBeRemoved.add(dateTime);
            }
        }
        toBeRemoved.forEach(comingReg::remove);
    }
    private void notifyWinners(List<UserRegistration> winners, double winnings) {
        for (UserRegistration winner : winners) {
            System.out.println("Sending email to: " + winner.getUserEmail() +
                    "\nCongratulations! You won " + winnings + " SEK.");
        }
    }

    public void start() {
        while (!Thread.interrupted()) {
            try {
                Socket client = serverSocket.accept();
                new ClientHandler(client).start();
            } catch (IOException e) {
                e.printStackTrace();
            }
        }
    }

    private class ClientHandler extends Thread {
        private Socket clientSocket;

        public ClientHandler(Socket socket) {
            this.clientSocket = socket;
        }

        @Override
        public void run() {
            try (
                    ObjectInputStream in = new ObjectInputStream(clientSocket.getInputStream());
                    ObjectOutputStream out = new ObjectOutputStream(clientSocket.getOutputStream())
            ) {
                String type = (String) in.readObject();
                if (type.equals("reg")) {
                    handleReg(in, out);
                } else if (type.equals("his")) {
                    handleHisDataReq(in, out);
                }
            } catch (IOException e) {
                System.err.println("IO Exception in Client Handler: " + e.getMessage());
            } catch (ClassNotFoundException e) {
                System.err.println("Class Not Found in Client Handler: " + e.getMessage());
            } catch (Exception e) {
                System.err.println("Error in Client Handler: " + e.getMessage());
            } finally {
                try {
                    clientSocket.close();
                } catch (IOException e) {
                    System.err.println("Error closing socket: " + e.getMessage());
                }
            }
        }

        private void handleReg(ObjectInputStream in, ObjectOutputStream out) throws IOException {
            List<UserRegistration> registrations;
            String response;

            try {
                registrations = (List<UserRegistration>) in.readObject();
            } catch (ClassNotFoundException e) {
                System.err.println("Invalid object received: " + e.getMessage());
                out.writeObject("Error: Invalid registration data received.");
                return;
            }

            if (registrations == null || registrations.isEmpty()) {
                response = "Error: No registration data provided.";
            } else {
                response = checkReg(registrations);
            }

            out.writeObject(response);

            if (response.equals("OK")) {
                processReg(registrations);
            }
        }

        private void processReg(List<UserRegistration> registrations) {
            for (UserRegistration registration : registrations) {
                LocalDateTime dateTime = registration.getRegDate();
                comingReg.computeIfAbsent(dateTime, k -> new ArrayList<>()).add(registration);
                System.out.println("New registration: " + registration.getRegDate() +
                        ", " + registration.getRegNum() +
                        ", " + registration.getUserEmail());
                pool += 100;
            }
        }

        private String checkReg(List<UserRegistration> registrations) {
            LocalDateTime now = LocalDateTime.now();
            for (UserRegistration registration : registrations) {
                if (registration.getRegDate().isBefore(now)) {
                    return "Registration date is in the past.";
                }
                if (registration.getRegNum() < 0 || registration.getRegNum() > 255) {
                    return "Numbers must be between 0 and 255.";
                }
                if (comingReg.containsKey(registration.getRegDate())
                        && comingReg.get(registration.getRegDate()).contains(registration)) {
                    return "You have been registered before.";
                }
            }
            for (int i = 0; i < registrations.size(); i++) {
                for (int j = 0; j < registrations.size(); j++) {
                    if (i != j && registrations.get(i).equals(registrations.get(j))) {
                        return "You can't repeat the same slot with the same number.";
                    }
                }
            }
            return "OK";
        }

        private void handleHisDataReq(ObjectInputStream in, ObjectOutputStream out) throws IOException, ClassNotFoundException {
            LocalDateTime start = (LocalDateTime) in.readObject();
            LocalDateTime end = (LocalDateTime) in.readObject();
            if (start.isBefore(end)) {
                List<LotteryResult> lotteryResults = getHisRec(start, end);
                out.writeObject(lotteryResults);
            } else {
                out.writeObject("Error: The start point is after the end time point.");
            }
        }

        private List<LotteryResult> getHisRec(LocalDateTime from, LocalDateTime to) {
            List<LotteryResult> results = new ArrayList<>();
            for (Map.Entry<LocalDateTime, List<UserRegistration>> entry : comingReg.entrySet()) {
                LocalDateTime date = entry.getKey();
                if (!date.isBefore(from) && !date.isAfter(to)) {
                    int winningNumber = random.nextInt(256);
                    List<UserRegistration> winners = new ArrayList<>();
                    for (UserRegistration registration : entry.getValue()) {
                        if (registration.getRegNum() == winningNumber) {
                            winners.add(registration);
                        }
                    }
                    double winnings = (pool + entry.getValue().size() * 100) / winners.size();
                    LotteryResult historicalRecord = new LotteryResult(date, new ArrayList<>(winners), winnings, winningNumber);
                    results.add(historicalRecord);
                }
            }
            return results;
        }
    }

    public static void main(String[] args) {
        LotteryServer server = new LotteryServer();
        server.start();
    }
}

/**
 * This class simulates the functionality of sending notification emails.
 */
public class NotifyMail {

    /**
     * Sends a notification email to the provided registration with details of winnings.
     *
     * @param userRegistration The registration information.
     * @param prizeAmount      The amount of winnings.
     */
    public void notifyWinner(UserRegistration userRegistration, double prizeAmount) {
        System.out.println(new NotificationMessage(userRegistration, prizeAmount));
    }

    /**
     * Inner class representing the structure of the notification message.
     */
    static class NotificationMessage {
        private final UserRegistration userRegistration;
        private final double prizeAmount;

        /**
         * Constructs a notification message with the provided registration and winnings.
         *
         * @param userRegistration The registration information.
         * @param prizeAmount      The amount of winnings.
         */
        NotificationMessage(UserRegistration userRegistration, double prizeAmount) {
            this.userRegistration = userRegistration;
            this.prizeAmount = prizeAmount;
        }

        /**
         * Converts the notification message to a formatted string.
         *
         * @return The formatted notification message.
         */
        public String toString() {
            return "Email to: " + userRegistration.getUserEmail() + '\n'
                    + "Content: \n"
                    + "Congratulations! You have won " + prizeAmount + " SEK \n"
                    + "due to the draw on " + userRegistration.getRegDate() + '\n'
                    + "with the winning number: " + userRegistration.getRegNum() + ".\n";
        }
    }
}

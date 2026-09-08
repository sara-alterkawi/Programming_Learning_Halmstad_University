import java.io.Serializable;
import java.time.LocalDateTime;

public record UserRegistration(String userEmail, java.time.LocalDateTime regDate, int regNum) implements Serializable {
    /**
     * Constructs a UserRegistrationInfo object.
     *
     * @param userEmail The email address of the user.
     * @param regDate   The date of registration.
     * @param regNum    A unique registration number.
     */
    public UserRegistration {
    }

    /**
     * Gets the email address of the user.
     *
     * @return The email address.
     */
    public String getUserEmail() {
        return userEmail;
    }
    /**
     * Gets the unique registration number.
     *
     * @return The registration date.
     */
    public LocalDateTime getRegDate() {
        return regDate;
    }

    /**
     * Gets the unique registration number.
     *
     * @return The registration number.
     */
    public int getRegNum() {
        return regNum;
    }

    /**
     * Returns a string representation of the UserRegistrationInfo object.
     *
     * @return A string representation containing user registration details.
     */
    @Override
    public String toString() {
        return "User Registration Info: {" +
                "User Email= '" + userEmail + '\'' +
                ", Registration Date= " + regDate +
                ", Registration Number= " + regNum +
                '}';
    }
}

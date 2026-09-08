import java.io.Serializable;
import java.time.LocalDateTime;
import java.util.ArrayList;

public class LotteryResult implements Serializable {
    private final LocalDateTime resultDate;
    private final ArrayList<UserRegistration> winner;
    private final double totalPrize;
    private final int winTicketNum;

    public LotteryResult(LocalDateTime drawingDate, ArrayList<UserRegistration> winner, double totalPrize, int winTicketNum) {
        this.resultDate = drawingDate;
        this.winner = winner;
        this.totalPrize = totalPrize;
        this.winTicketNum = winTicketNum;
    }

    public LocalDateTime getResultDate() {
        return resultDate;
    }

    @Override
    public String toString() {
        return "Lottery History{" +
                "Result Date=" + resultDate +
                ", Prize Winners=" + winner +
                ", Total Prize Amount =" + totalPrize +
                ", Winning Ticket Number=" + winTicketNum +
                '}';
    }
}

import javax.swing.*;
import javax.swing.event.ChangeEvent;
import javax.swing.event.ChangeListener;
import java.awt.*;

public class Levels implements ChangeListener {

    private int currentLevel;
    private final Model model;       
  // Define 3D array of strings. It contains data for several levels of a game,
    // where each level is represented as a 2D array of strings.
    private final static String[][][] gameLevels = new String[][][]{
            {
                    {"f", "w", "w", "w", "w", "w"},
                    {"f", "w", ".", ".", "f", "w"},
                    {"w", "w", "w", "f", "f", "w"},
                    {"w", "f", "x", "f", "f", "w"},
                    {"w", "f", "x", "f", "w", "w"},
                    {"w", "s", "f", "f", "w", "f"},
                    {"w", "w", "w", "w", "w", "f"},
            },
            {
                    {"w", "w", "w", "w", "w", "w", "w", "w", "w", "w", "w", "w", "w", "w", "w"},
                    {"w", "w", "w", "f", "f", "w", ".", "f", "f", "w", "f", "f", "f", "w", "w"},
                    {"w", "w", "w", "x", "f", "f", "f", "f", "s", "f", "f", "x", "f", "w", "w"},
                    {"w", "w", "w", ".", "f", "f", "f", "f", "f", "f", "f", "f", "w", "w", "w"},
                    {"w", "w", "w", "w", "f", "f", "f", "f", "f", "f", "f", "w", "w", "w", "w"},
                    {"w", "w", "w", "w", "w", "f", "f", ".", "f", "x", "f", "w", "w", "w", "w"},
                    {"w", "w", "w", "w", "w", "w", "w", "w", "w", "w", "w", "w", "w", "w", "w"},
                    {"w", "w", "w", "w", "w", "w", "w", "w", "w", "w", "w", "w", "w", "w", "w"},
                    {"w", "w", "w", "w", "w", "w", "w", "w", "w", "w", "w", "w", "w", "w", "w"},
                    {"w", "w", "w", "w", "w", "w", "w", "w", "w", "w", "w", "w", "w", "w", "w"}
            },
            {
                    {"f", "f", "w", "w", "w", "w", "w", "f"},
                    {"w", "w", "w", "f", "f", "f", "w", "f"},
                    {"w", ".", "s", "x", "f", "f", "w", "f"},
                    {"w", "w", "w", "f", "x", ".", "w", "f"},
                    {"w", ".", "w", "w", "x", "f", "w", "f"},
                    {"w", "f", "w", "f", ".", "f", "w", "w"},
                    {"w", "x", "f", "*", "x", "x", ".", "w"},
                    {"w", "f", "f", "f", ".", "f", "f", "w"},
                    {"w", "w", "w", "w", "w", "w", "w", "w"}
            }
    };
    
    public Levels(Model model) {
        this.model = model;
        currentLevel = 1;
        model.addNotifyListener(this);
    }

    /**
     * Defines what should happen when the state of the "Model" object changes.
     * */
    @Override
    public void stateChanged(ChangeEvent e) {
        boolean hasWon = model.winCase();
        boolean hasLost = model.loseCase();
        boolean isRunning = currentLevel <= gameLevels.length;

        if (hasWon && isRunning) {
            // JOptionPane.showMessageDialog(this, "Level" + currentLevel + "is completed", "You won!", JOptionPane.INFORMATION_MESSAGE);
            updateLevelData(currentLevel + 1);
            model.reset();
        } else if (hasLost) {
            // JOptionPane.showMessageDialog(this, "Level" + currentLevel + "is not completed", "You lost!", JOptionPane.INFORMATION_MESSAGE);
            updateLevelData(currentLevel);
            model.reset();
        }
    }

     private void updateLevelData(int level) {
        currentLevel = level;
        model.levelDataSetting(getCurrentLevel(level), level);
    }

    public static String[][] getCurrentLevel(int index){
        // Retrieve the data for each level in the game.
        return gameLevels[index-1];
    }

}

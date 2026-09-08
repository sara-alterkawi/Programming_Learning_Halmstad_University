import javax.swing.*;
import javax.swing.event.ChangeEvent;
import javax.swing.event.ChangeListener;
import java.awt.event.KeyEvent;
import java.awt.event.KeyListener;

public class View extends JFrame implements ChangeListener, Controller, KeyListener {

    private final Model model;
    private String dataSaver = null;

    public View(Model model, String name) {
        this.model = model;
        model.startTimer();
        model.addNotifyListener(this);
        add(model);

        JOptionPane.showMessageDialog(this, "How to play SOKOBAN!\n"
                + "Move the box to the goal place using the arrow keys.\n"
                + "You win when all the boxes are set on goal places.\n"
                + "You can move only one box at a time.");

        addKeyListener(this);

        setTitle(name); // Title of the View.
        setDefaultCloseOperation(JFrame.EXIT_ON_CLOSE); // Set the default close operation to EXIT_ON_CLOSE.
        setSize(model.getWidth(),model.getHeight()); // Set the size of the View to the width and height of gameModel.
        setVisible(true); // Sets the View to be visible.
    }
  
  @Override
    public String getDataSaver() {
        String currentDataSaver = dataSaver;
        dataSaver = null;
        return currentDataSaver; // Current value of gameDataSaver
    }

    @Override
    public void stateChanged(ChangeEvent e) {
        setSize(model.getWidth(),model.getHeight()); // Set the size of the View to the width and height of gameModel
        model.repaint();
    }
    
     @Override
    public void keyPressed(KeyEvent e) {
        int keyCode = e.getKeyCode();
        if (keyCode == KeyEvent.VK_RIGHT) {
            dataSaver = "RIGHT";
            Model.moved();
        } else if (keyCode == KeyEvent.VK_LEFT) {
            dataSaver = "LEFT";
            Model.moved();
        } else if (keyCode == KeyEvent.VK_UP) {
            dataSaver = "UP";
            Model.moved();
        } else if (keyCode == KeyEvent.VK_DOWN) {
            dataSaver = "DOWN";
            Model.moved();
        }
        model.gameInstruction(this);
    }

    @Override
    public void keyTyped(KeyEvent e) {}

    @Override
    public void keyReleased(KeyEvent e) {}

}

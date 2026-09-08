import javax.swing.event.ChangeEvent;
import javax.swing.event.ChangeListener;

public class ConsolePrinter implements ChangeListener {
    private final Model model;

public ConsolePrinter(Model model) {
        this.model = model;
        model.addNotifyListener(this);
    }
    /**
     * This line is an annotation that indicates that the following method is meant
     * to override a method from the parent class or interface.
     * In this case, the method "stateChanged" from the "ChangeListener" interface is being overridden.
     * The method takes a "ChangeEvent" object as a parameter.
     * */
    @Override
    public void stateChanged(ChangeEvent e) {
        String output = generateOutput();
        System.out.print(output);
    }
    
    private String generateOutput() {
        StringBuilder sb = new StringBuilder();

        for (int i = 0; i < model.getNumRows(); i++) {
            for (int j = 0; j < model.getNumCols(); j++) {
                sb.append(model.getContent(i, j));
            }
            sb.append("\n");
        }

        return sb.toString();
    }


}

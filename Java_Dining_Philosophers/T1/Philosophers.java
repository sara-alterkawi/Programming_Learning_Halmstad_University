
public class Philosophers {
    private static int NUM_PHIL = 5;

    public static void main(String[] args) throws InterruptedException {
        Object[] forks = new Object[NUM_PHIL];
        Philosopher[] phils = new Philosopher[NUM_PHIL];

        // Create forks
        for (int i = 0; i < phils.length; i++) {
            forks[i] = new Object();
        }

        // Initialize philosophers with two forks each
        for (int i = 0; i < phils.length; i++) {
            // Break symmetry for at least one philosopher
            boolean startWithRightFork = i % 2 == 0;
            phils[i] = new Philosopher(i, forks[i], forks[(i + 1) % phils.length], startWithRightFork);
        }

        for (Thread p : phils) {
            // Start the philosophers
            p.start();
        }

        // Keep running until the user presses ENTER
        new java.util.Scanner(System.in).nextLine();

        for (Thread p : phils) {
            // Request termination from all philosophers
            p.interrupt();
        }

        for (Thread p : phils) {
            // Wait for all philosophers to finish
            p.join();
        }

        System.out.println("All philosophers finished dining.");
    }

    static class Philosopher extends Thread {
        private final Object leftFork, rightFork;
        private final int id;

        public Philosopher(int id, Object leftFork, Object rightFork, boolean startWithRightFork) {
            this.id = id;
            this.leftFork = startWithRightFork ? rightFork : leftFork;
            this.rightFork = startWithRightFork ? leftFork : rightFork;
        }

        private void eat() throws InterruptedException {
            System.out.println("Philosopher " + id + " is eating.");
            Thread.sleep((long) (Math.random() * 3));
        }

        private void think() throws InterruptedException {
            System.out.println("Philosopher " + id + " is thinking.");
            Thread.sleep((long) (Math.random() * 3));
        }

        private void waiting() throws InterruptedException {
            System.out.println("Philosopher " + id + " is waiting.");
            Thread.sleep((long) (Math.random() * 3));
        }

        public void run() {
            try {
                while (!isInterrupted()) {
                    think();
                    waiting();

                    // Pick up forks by synchronizing on the two forks
                    synchronized (leftFork) {
                        System.out.println("Left fork is with Philosopher " + id);
                        synchronized (rightFork) {
                            System.out.println("Right fork is with Philosopher " + id);
                            // To eat, forks have to be "held"
                            eat();
                            // Put down forks by exiting the synchronized context
                        }
                    }
                    System.out.println("Philosopher " + id + " put down the two forks.");
                }
            } catch (InterruptedException ie) {
                // Do nothing, interruption may cause this, but safe to ignore
            }
        }
    }
}

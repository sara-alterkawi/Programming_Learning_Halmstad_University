package org.example.prime;

import javax.servlet.ServletContextEvent;
import javax.servlet.ServletContextListener;
import java.util.Map;

/**
 * A ServletContextListener implementation for initializing the SharingUnit on application startup.
 */
public class StartupListener implements ServletContextListener {

    /**
     * This method is called when the servlet context is initialized (application startup).
     * @param event The ServletContextEvent triggered on application startup.
     */
    @Override
    public void contextInitialized(ServletContextEvent event) {
        try {
            // Retrieve the PORT from the environment variables
            Map<String, String> env = System.getenv();
            int port = Integer.parseInt(env.get("PORT"));

            // Print the port for debugging purposes
            System.out.println("Port: " + port);

            // Initialize the SharingUnit with the specified port
            SharingUnit unit = new SharingUnit(port);

        } catch (NumberFormatException | NullPointerException e) {
            // Handle potential exceptions related to parsing or missing environment variable
            System.err.println("Error initializing SharingUnit: " + e.getMessage());
            e.printStackTrace(); // Print the stack trace for debugging
        }
    }

    /**
     * This method is called when the servlet context is destroyed (application shutdown).
     * @param event The ServletContextEvent triggered on application shutdown.
     */
    @Override
    public void contextDestroyed(ServletContextEvent event) {
        // Perform any necessary cleanup or actions during application shutdown
    }
}

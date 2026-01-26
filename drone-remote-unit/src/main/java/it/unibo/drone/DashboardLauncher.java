package it.unibo.drone;

import it.unibo.drone.controller.DashboardController;
import it.unibo.drone.view.DashboardView;
import it.unibo.drone.view.LogView;

import javax.swing.*;

/**
 * Main application launcher for Drone Remote Unit
 */
public class DashboardLauncher {

    public static void main(String[] args) {
        // Set Look and Feel
        try {
            UIManager.setLookAndFeel(UIManager.getSystemLookAndFeelClassName());
        } catch (Exception e) {
            e.printStackTrace();
        }

        // Serial port selection
        String portName = selectSerialPort(args);

        if (portName == null) {
            System.err.println("No serial port specified. Exiting.");
            System.exit(1);
        }

        // Create views
        DashboardView dashboardView = new DashboardView();
        LogView logView = new LogView();

        try {
            // Create controller (connects to Arduino)
            DashboardController controller = new DashboardController(
                portName,
                dashboardView,
                logView
            );

            // Register controller with view
            dashboardView.registerController(controller);

            // Show windows
            logView.display();
            dashboardView.display();

        } catch (Exception ex) {
            JOptionPane.showMessageDialog(
                null,
                "Failed to connect to Arduino:\n" + ex.getMessage(),
                "Connection Error",
                JOptionPane.ERROR_MESSAGE
            );
            ex.printStackTrace();
            System.exit(1);
        }
    }

    private static String selectSerialPort(String[] args) {
        // Command line argument
        if (args.length > 0) {
            return args[0];
        }
        return "COM6"; // Default port
    }
}
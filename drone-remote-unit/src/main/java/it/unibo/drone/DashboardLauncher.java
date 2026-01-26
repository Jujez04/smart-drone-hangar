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

            // Shutdown hook
            Runtime.getRuntime().addShutdownHook(new Thread(() -> {
                System.out.println("Shutting down...");
                controller.close();
            }));

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
        // Option 1: Command line argument
        if (args.length > 0) {
            return args[0];
        }

        // Option 2: Show dialog
        String[] commonPorts = {
            "COM3", "COM4", "COM5", "COM6",  // Windows
            "/dev/ttyUSB0", "/dev/ttyUSB1",  // Linux
            "/dev/cu.usbmodem14101", "/dev/cu.usbserial-14110"  // macOS
        };

        return (String) JOptionPane.showInputDialog(
            null,
            "Select Arduino serial port:",
            "Serial Port Selection",
            JOptionPane.QUESTION_MESSAGE,
            null,
            commonPorts,
            commonPorts[0]
        );
    }
}
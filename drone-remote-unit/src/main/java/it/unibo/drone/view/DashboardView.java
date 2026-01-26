package it.unibo.drone.view;

import it.unibo.drone.controller.DashboardController;

import javax.swing.*;
import javax.swing.border.*;
import java.awt.*;
import java.awt.event.*;

/**
 * Main dashboard GUI for Drone Remote Unit
 */
public class DashboardView extends JFrame implements ActionListener {

    private DashboardController controller;
    private JButton takeoffButton;
    private JButton landingButton;
    private JButton statusButton;
    private JButton emergencyButton;
    private JTextField hangarStatusField;
    private JTextField droneStatusField;
    private JTextField temperatureField;
    private JTextField distanceField;

    // Status panel with color
    private JPanel statusIndicator;

    public DashboardView() {
        super("Drone Remote Unit (DRU)");
        setSize(600, 450);
        setResizable(false);
        setDefaultCloseOperation(JFrame.EXIT_ON_CLOSE);

        // Main panel with padding
        JPanel mainPanel = new JPanel();
        mainPanel.setLayout(new BoxLayout(mainPanel, BoxLayout.Y_AXIS));
        mainPanel.setBorder(new EmptyBorder(20, 20, 20, 20));

        // Title
        JLabel titleLabel = new JLabel("Drone Remote Unit", SwingConstants.CENTER);
        titleLabel.setFont(new Font("Arial", Font.BOLD, 24));
        titleLabel.setAlignmentX(Component.CENTER_ALIGNMENT);
        mainPanel.add(titleLabel);
        mainPanel.add(Box.createRigidArea(new Dimension(0, 20)));

        // Status indicator panel
        JPanel indicatorPanel = createStatusIndicatorPanel();
        mainPanel.add(indicatorPanel);
        mainPanel.add(Box.createRigidArea(new Dimension(0, 15)));

        // Information panel
        JPanel infoPanel = createInfoPanel();
        mainPanel.add(infoPanel);
        mainPanel.add(Box.createRigidArea(new Dimension(0, 20)));

        // Control buttons panel
        JPanel controlPanel = createControlPanel();
        mainPanel.add(controlPanel);

        setContentPane(mainPanel);
        setLocationRelativeTo(null); // Center on screen
    }

    private JPanel createStatusIndicatorPanel() {
        JPanel panel = new JPanel(new BorderLayout());
        panel.setBorder(BorderFactory.createTitledBorder("System Status"));
        panel.setMaximumSize(new Dimension(560, 80));

        statusIndicator = new JPanel();
        statusIndicator.setPreferredSize(new Dimension(540, 50));
        statusIndicator.setBackground(Color.GRAY);
        statusIndicator.setBorder(new LineBorder(Color.BLACK, 2));

        JLabel statusLabel = new JLabel("INITIALIZING...", SwingConstants.CENTER);
        statusLabel.setFont(new Font("Arial", Font.BOLD, 20));
        statusLabel.setForeground(Color.WHITE);
        statusIndicator.add(statusLabel);

        panel.add(statusIndicator, BorderLayout.CENTER);
        return panel;
    }

    private JPanel createInfoPanel() {
        JPanel panel = new JPanel();
        panel.setLayout(new GridLayout(4, 2, 10, 10));
        panel.setBorder(BorderFactory.createTitledBorder("Telemetry Data"));
        panel.setMaximumSize(new Dimension(560, 150));

        // Hangar Status
        panel.add(new JLabel("Hangar Status:"));
        hangarStatusField = new JTextField("--");
        hangarStatusField.setEditable(false);
        hangarStatusField.setFont(new Font("Monospaced", Font.BOLD, 12));
        panel.add(hangarStatusField);

        // Drone Status
        panel.add(new JLabel("Drone Status:"));
        droneStatusField = new JTextField("--");
        droneStatusField.setEditable(false);
        droneStatusField.setFont(new Font("Monospaced", Font.BOLD, 12));
        panel.add(droneStatusField);

        // Temperature
        panel.add(new JLabel("Temperature:"));
        temperatureField = new JTextField("--");
        temperatureField.setEditable(false);
        temperatureField.setFont(new Font("Monospaced", Font.PLAIN, 12));
        panel.add(temperatureField);

        // Distance
        panel.add(new JLabel("Distance to Ground:"));
        distanceField = new JTextField("--");
        distanceField.setEditable(false);
        distanceField.setFont(new Font("Monospaced", Font.PLAIN, 12));
        panel.add(distanceField);

        return panel;
    }

    private JPanel createControlPanel() {
        JPanel panel = new JPanel();
        panel.setLayout(new GridLayout(2, 2, 10, 10));
        panel.setBorder(BorderFactory.createTitledBorder("Drone Commands"));
        panel.setMaximumSize(new Dimension(560, 120));

        // Takeoff button
        takeoffButton = new JButton("TAKEOFF");
        takeoffButton.setFont(new Font("Arial", Font.BOLD, 14));
        takeoffButton.setBackground(new Color(76, 175, 80));
        takeoffButton.setForeground(Color.BLACK);
        takeoffButton.setFocusPainted(false);
        takeoffButton.addActionListener(this);
        panel.add(takeoffButton);

        // Landing button
        landingButton = new JButton("LANDING");
        landingButton.setFont(new Font("Arial", Font.BOLD, 14));
        landingButton.setBackground(new Color(33, 150, 243));
        landingButton.setForeground(Color.BLACK);
        landingButton.setFocusPainted(false);
        landingButton.addActionListener(this);
        panel.add(landingButton);
        return panel;
    }

    public void display() {
        SwingUtilities.invokeLater(() -> setVisible(true));
    }

    public void registerController(DashboardController controller) {
        this.controller = controller;
    }

    public void setHangarStatus(String status) {
        SwingUtilities.invokeLater(() -> {
            hangarStatusField.setText(status);
            updateStatusIndicator(status);
        });
    }

    public void setDroneStatus(String status) {
        SwingUtilities.invokeLater(() -> {
            droneStatusField.setText(status);
        });
    }

    public void setTemperature(double temp) {
        SwingUtilities.invokeLater(() -> {
            temperatureField.setText(String.format("%.1f °C", temp));
        });
    }

    public void setDistance(double distance) {
        SwingUtilities.invokeLater(() -> {
            distanceField.setText(String.format("%.1f cm", distance));
        });
    }

    private void updateStatusIndicator(String status) {
        JLabel label = (JLabel) statusIndicator.getComponent(0);
        label.setText(status);

        if (status.contains("ALARM")) {
            statusIndicator.setBackground(Color.RED);
            label.setForeground(Color.WHITE);
        } else if (status.contains("PRE_ALARM")) {
            statusIndicator.setBackground(Color.ORANGE);
            label.setForeground(Color.BLACK);
        } else if (status.contains("OUT")) {
            statusIndicator.setBackground(new Color(33, 150, 243)); // Blue
            label.setForeground(Color.WHITE);
        } else if (status.contains("INSIDE")) {
            statusIndicator.setBackground(new Color(76, 175, 80)); // Green
            label.setForeground(Color.WHITE);
        } else {
            statusIndicator.setBackground(Color.GRAY);
            label.setForeground(Color.WHITE);
        }
    }

    @Override
    public void actionPerformed(ActionEvent e) {
        if (controller == null) return;

        try {
            if (e.getSource() == takeoffButton) {
                controller.sendTakeoffCommand();
            } else if (e.getSource() == landingButton) {
                controller.sendLandingCommand();
            } else if (e.getSource() == statusButton) {
                controller.requestStatus();
            } else if (e.getSource() == emergencyButton) {
                int confirm = JOptionPane.showConfirmDialog(
                    this,
                    "Send emergency reset to hangar?",
                    "Confirm Emergency Reset",
                    JOptionPane.YES_NO_OPTION,
                    JOptionPane.WARNING_MESSAGE
                );
                if (confirm == JOptionPane.YES_OPTION) {
                    controller.sendEmergencyReset();
                }
            }
        } catch (Exception ex) {
            JOptionPane.showMessageDialog(
                this,
                "Error: " + ex.getMessage(),
                "Communication Error",
                JOptionPane.ERROR_MESSAGE
            );
        }
    }
}
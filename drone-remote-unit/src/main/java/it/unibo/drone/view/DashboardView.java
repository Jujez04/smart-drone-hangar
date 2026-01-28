package it.unibo.drone.view;

import it.unibo.drone.controller.DashboardController;

import javax.swing.*;
import javax.swing.border.EmptyBorder;
import java.awt.*;
import java.awt.event.ActionEvent;
import java.awt.event.ActionListener;

/**
 * Main dashboard GUI for Drone Remote Unit
 */
public class DashboardView extends JFrame implements ActionListener {

    private DashboardController controller;
    
    // UI Elements
    private JTextField droneStateField;
    private JTextField hangarStateField;
    private JTextField distanceField;
    
    private JButton takeoffButton;
    private JButton landingButton;

    public DashboardView() {
        super("Drone Remote Unit (DRU)");
        setSize(400, 350);
        setResizable(false);
        setDefaultCloseOperation(JFrame.EXIT_ON_CLOSE);

        // Main container
        JPanel mainPanel = new JPanel();
        mainPanel.setLayout(new BoxLayout(mainPanel, BoxLayout.Y_AXIS));
        mainPanel.setBorder(new EmptyBorder(20, 20, 20, 20));
        mainPanel.setBackground(new Color(240, 240, 240));

        // 1. HEADER TITLE
        JLabel titleLabel = new JLabel("Drone Remote Unit", SwingConstants.CENTER);
        titleLabel.setFont(new Font("Arial", Font.BOLD, 24));
        titleLabel.setAlignmentX(Component.CENTER_ALIGNMENT);
        titleLabel.setBorder(new EmptyBorder(0, 0, 20, 0));
        mainPanel.add(titleLabel);

        // 2. INFO FIELDS PANEL (3 Fields)
        JPanel fieldsPanel = new JPanel(new GridLayout(3, 2, 10, 15));
        fieldsPanel.setOpaque(false);

        // Field 1: Drone State
        fieldsPanel.add(createLabel("Drone State:"));
        droneStateField = createReadOnlyField("REST");
        fieldsPanel.add(droneStateField);

        // Field 2: Hangar State
        fieldsPanel.add(createLabel("Hangar State:"));
        hangarStateField = createReadOnlyField("NORMAL");
        hangarStateField.setBackground(new Color(144, 238, 144)); // Light Green
        fieldsPanel.add(hangarStateField);

        // Field 3: Distance
        fieldsPanel.add(createLabel("Distance (cm):"));
        distanceField = createReadOnlyField("--");
        fieldsPanel.add(distanceField);

        mainPanel.add(fieldsPanel);
        mainPanel.add(Box.createRigidArea(new Dimension(0, 30)));

        // 3. BUTTONS PANEL (2 Buttons)
        JPanel buttonPanel = new JPanel(new GridLayout(1, 2, 20, 0));
        buttonPanel.setOpaque(false);

        takeoffButton = createStyledButton("TAKEOFF", new Color(70, 130, 180)); // Steel Blue
        takeoffButton.setForeground(Color.black);
        landingButton = createStyledButton("LANDING", new Color(255, 140, 0));  // Dark Orange
        landingButton.setForeground(Color.black);
        buttonPanel.add(takeoffButton);
        buttonPanel.add(landingButton);

        mainPanel.add(buttonPanel);

        setContentPane(mainPanel);
        setLocationRelativeTo(null);
    }

    // --- Helper Methods for UI Styling ---

    private JLabel createLabel(String text) {
        JLabel l = new JLabel(text);
        l.setFont(new Font("Arial", Font.BOLD, 14));
        return l;
    }

    private JTextField createReadOnlyField(String initialText) {
        JTextField tf = new JTextField(initialText);
        tf.setEditable(false);
        tf.setFont(new Font("Monospaced", Font.BOLD, 14));
        tf.setHorizontalAlignment(JTextField.CENTER);
        tf.setBorder(BorderFactory.createLineBorder(Color.GRAY));
        return tf;
    }

    private JButton createStyledButton(String text, Color bg) {
        JButton btn = new JButton(text);
        btn.setFont(new Font("Arial", Font.BOLD, 14));
        btn.setBackground(bg);
        btn.setForeground(Color.WHITE);
        btn.setFocusPainted(false);
        btn.addActionListener(this);
        return btn;
    }


    public void display() {
        SwingUtilities.invokeLater(() -> setVisible(true));
    }

    public void registerController(DashboardController controller) {
        this.controller = controller;
    }

    /**
     * Updates the UI based on the raw status received from Arduino.
     * Maps the specific firmware states to the required GUI states.
     */
    public void updateViewFromStatus(String rawStatus) {
        SwingUtilities.invokeLater(() -> {
            // 1. Map Hangar State (Normal vs Alarm)
            if (rawStatus.equals("ALARM")) {
                hangarStateField.setText("ALARM");
                hangarStateField.setBackground(Color.RED);
                hangarStateField.setForeground(Color.WHITE);
            } else {
                // Includes INSIDE, TAKE_OFF, PRE_ALARM, etc.
                hangarStateField.setText("NORMAL");
                hangarStateField.setBackground(new Color(144, 238, 144)); // Green
                hangarStateField.setForeground(Color.BLACK);
            }

            // 2. Map Drone State
            if (rawStatus.contains("INSIDE")) {
                droneStateField.setText("REST");
            } else if (rawStatus.contains("TAKING_OFF")) {
                droneStateField.setText("TAKING OFF");
            } else if (rawStatus.contains("OUT")) {
                droneStateField.setText("OPERATING");
            } else if (rawStatus.contains("LANDING")) {
                droneStateField.setText("LANDING");
            }
            if (rawStatus.contains("INSIDE") || rawStatus.contains("OUT")) {
                distanceField.setText("--");
            }
        });
    }

    public void setDistance(double distance) {
        SwingUtilities.invokeLater(() -> {
            if (distance <= 0.001) {
                distanceField.setText("--");
            } else {
                distanceField.setText(String.format("%.1f", distance * 100));
            }
        });
    }

    @Override
    public void actionPerformed(ActionEvent e) {
        if (controller == null) return;
        try {
            if (e.getSource() == takeoffButton) {
                controller.sendTakeoffCommand();
            } else if (e.getSource() == landingButton) {
                controller.sendLandingCommand();
            }
        } catch (Exception ex) {
            JOptionPane.showMessageDialog(this, "Error: " + ex.getMessage());
        }
    }
}
package it.unibo.drone.view;

import javax.swing.*;
import java.awt.*;
import java.text.SimpleDateFormat;
import java.util.Date;

/**
 * Log window showing system messages
 */
public class LogView extends JFrame {

    private JTextArea logArea;
    private SimpleDateFormat dateFormat;

    public LogView() {
        super("Drone Hangar - System Log");
        dateFormat = new SimpleDateFormat("HH:mm:ss.SSS");

        setSize(700, 500);
        setResizable(true);

        // Main panel
        JPanel mainPanel = new JPanel(new BorderLayout());

        // Log text area with scroll
        logArea = new JTextArea(25, 60);
        logArea.setEditable(false);
        logArea.setFont(new Font("Monospaced", Font.PLAIN, 12));
        logArea.setBackground(Color.BLACK);
        logArea.setForeground(Color.GREEN);

        JScrollPane scrollPane = new JScrollPane(logArea);
        scrollPane.setVerticalScrollBarPolicy(JScrollPane.VERTICAL_SCROLLBAR_ALWAYS);

        mainPanel.add(scrollPane, BorderLayout.CENTER);

        // Clear button
        JButton clearButton = new JButton("Clear Log");
        clearButton.addActionListener(e -> logArea.setText(""));

        JPanel bottomPanel = new JPanel();
        bottomPanel.add(clearButton);
        mainPanel.add(bottomPanel, BorderLayout.SOUTH);

        setContentPane(mainPanel);

        // Position window
        setLocation(100, 100);
    }

    /**
     * Add a log message with timestamp
     */
    public void log(String message) {
        SwingUtilities.invokeLater(() -> {
            String timestamp = dateFormat.format(new Date());
            String logEntry = "[" + timestamp + "] " + message + "\n";
            logArea.append(logEntry);

            // Auto-scroll to bottom
            logArea.setCaretPosition(logArea.getDocument().getLength());
        });
    }

    /**
     * Show the window
     */
    public void display() {
        SwingUtilities.invokeLater(() -> {
            setVisible(true);
            log("Log system initialized");
        });
    }
}
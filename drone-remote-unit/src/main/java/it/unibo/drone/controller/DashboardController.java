package it.unibo.drone.controller;

import it.unibo.drone.model.SerialCommChannel;
import it.unibo.drone.view.DashboardView;
import it.unibo.drone.view.LogView;

enum DroneState {
    REST,
    TAKING_OFF,
    LANDING,
    OPERATING;

    String getState() {
        return this.name();
    }
}

enum HangarState {
    NORMAL,
    ALARM;

    String getState() {
        return this.name();
    }
}

/**
 * Controller managing communication between View and Arduino
 */
public class DashboardController {

    private SerialCommChannel channel;
    private LogView logger;

    public DashboardController(String port, DashboardView view, LogView logger) throws Exception {
        this.logger = logger;

        logger.log("Connecting to Arduino on port: " + port);

        try {
            channel = new SerialCommChannel(port, 9600);
            logger.log("Serial connection established");

            // Start monitoring thread
            new MonitoringAgent(channel, view, logger).start();
            logger.log("Monitoring agent started");

            // Wait for Arduino to reboot
            logger.log("Waiting for Arduino to initialize...");
            Thread.sleep(4000);
            logger.log("System ready!");

        } catch (Exception ex) {
            logger.log("ERROR: Failed to connect - " + ex.getMessage());
            throw ex;
        }
    }

    public void sendTakeoffCommand() {
        channel.sendMsg("TAKEOFF");
        logger.log("Command sent: TAKEOFF");
    }

    public void sendLandingCommand() {
        channel.sendMsg("LANDING");
        logger.log("Command sent: LANDING");
    }

    public void requestStatus() {
        channel.sendMsg("STATUS");
        logger.log("Status request sent");
    }

    public void sendEmergencyReset() {
        channel.sendMsg("RESET");
        logger.log("Emergency RESET command sent");
    }

    public void close() {
        if (channel != null) {
            channel.close();
            logger.log("Connection closed");
        }
    }
}
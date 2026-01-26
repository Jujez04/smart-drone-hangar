package it.unibo.drone.controller;

import it.unibo.drone.model.SerialCommChannel;
import it.unibo.drone.view.DashboardView;
import it.unibo.drone.view.LogView;

public class DashboardController {

    private SerialCommChannel channel;
    private LogView logger;
    
    // DEFINIZIONE PROTOCOLLO LATO JAVA
    private static final String CMD_PREFIX = "ru:";

    public DashboardController(String port, DashboardView view, LogView logger) throws Exception {
        this.logger = logger;
        logger.log("Connecting to Arduino on port: " + port);

        try {
            channel = new SerialCommChannel(port, 9600);
            logger.log("Serial connection established");

            new MonitoringAgent(channel, view, logger).start();
            logger.log("Monitoring agent started");

            logger.log("Waiting for Arduino to initialize...");
            Thread.sleep(4000);
            logger.log("System ready!");

        } catch (Exception ex) {
            logger.log("ERROR: Failed to connect - " + ex.getMessage());
            throw ex;
        }
    }

    public void sendTakeoffCommand() {
        // Aggiungo il prefisso ru:
        String cmd = CMD_PREFIX + "TAKEOFF";
        channel.sendMsg(cmd);
        logger.log("Sent: " + cmd);
    }

    public void sendLandingCommand() {
        // Aggiungo il prefisso ru:
        String cmd = CMD_PREFIX + "LANDING";
        channel.sendMsg(cmd);
        logger.log("Sent: " + cmd);
    }

    public void requestStatus() {
        // Opzionale: se volessi chiedere lo stato manualmente
        // channel.sendMsg(CMD_PREFIX + "STATUS");
    }

    public void sendEmergencyReset() {
        // Aggiungo il prefisso ru:
        String cmd = CMD_PREFIX + "RESET";
        channel.sendMsg(cmd);
        logger.log("Sent: " + cmd);
    }

    public void close() {
        if (channel != null) {
            channel.close();
            logger.log("Connection closed");
        }
    }
}
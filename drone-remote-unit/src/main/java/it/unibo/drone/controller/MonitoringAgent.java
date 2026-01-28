package it.unibo.drone.controller;

import it.unibo.drone.model.SerialCommChannel;
import it.unibo.drone.view.DashboardView;
import it.unibo.drone.view.LogView;

/**
 * Background thread monitoring incoming messages from Arduino
 */
public class MonitoringAgent extends Thread {

    private SerialCommChannel channel;
    private DashboardView view;
    private LogView logger;

    private static final String APP_PREFIX = "dh:";  // Drone Hangar messages
    private static final String LOG_PREFIX = "lo:";  // Log messages

    public MonitoringAgent(SerialCommChannel channel, DashboardView view, LogView logger) {
        this.channel = channel;
        this.view = view;
        this.logger = logger;
        setDaemon(true); // Thread terminates when main program exits
    }

    @Override
    public void run() {
        logger.log("MonitoringAgent thread started");

        while (true) {
            try {
                // Blocking call - waits for message
                String msg = channel.receiveMsg();

                if (msg.startsWith(APP_PREFIX)) {
                    handleAppMessage(msg.substring(APP_PREFIX.length()));

                } else if (msg.startsWith(LOG_PREFIX)) {
                    String logMsg = msg.substring(LOG_PREFIX.length());
                    logger.log("[HANGAR] " + logMsg);

                } else {
                    // Unknown format
                    logger.log("[RAW] " + msg);
                }

            } catch (InterruptedException ex) {
                logger.log("MonitoringAgent interrupted");
                break;
            } catch (Exception ex) {
                logger.log("ERROR in MonitoringAgent: " + ex.getMessage());
                ex.printStackTrace();
            }
        }
    }

    private void handleAppMessage(String msg) {
        if (msg.startsWith("STATUS:")) {
            String status = msg.substring("STATUS:".length());
            view.updateViewFromStatus(status);

        } else if (msg.startsWith("ALARM")) {
            view.updateViewFromStatus("ALARM");
            logger.log("CRITICAL: ALARM SIGNAL RECEIVED");

        } else if (msg.startsWith("DIST:")) {
            try {
                double distance = Double.parseDouble(msg.substring("DIST:".length()));
                view.setDistance(distance);
            } catch (NumberFormatException ex) {
                logger.log("Invalid distance data: " + msg);
            }
        }
        else {
            logger.log("[APP] " + msg);
        }
    }

}
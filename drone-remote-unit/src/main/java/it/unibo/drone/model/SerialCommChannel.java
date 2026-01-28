package it.unibo.drone.model;

import java.util.concurrent.ArrayBlockingQueue;
import java.util.concurrent.BlockingQueue;

import jssc.SerialPort;
import jssc.SerialPortEvent;
import jssc.SerialPortEventListener;
import jssc.SerialPortException;

/**
 * Serial communication channel using jSSC library
 */
public class SerialCommChannel implements CommChannel, SerialPortEventListener {

	private SerialPort serialPort;
	private BlockingQueue<String> queue;
	private StringBuilder currentMsg = new StringBuilder();

	public SerialCommChannel(String port, int rate) throws Exception {
		queue = new ArrayBlockingQueue<String>(100);
		serialPort = new SerialPort(port);

		try {
			serialPort.openPort();
			serialPort.setParams(rate,
					SerialPort.DATABITS_8,
					SerialPort.STOPBITS_1,
					SerialPort.PARITY_NONE);

			serialPort.setFlowControlMode(SerialPort.FLOWCONTROL_RTSCTS_IN |
					SerialPort.FLOWCONTROL_RTSCTS_OUT);

			serialPort.addEventListener(this);

		} catch (SerialPortException ex) {
			System.err.println("Errore apertura porta: " + ex.getMessage());
			throw ex;
		}
	}

	@Override
	public void sendMsg(String msg) {
		try {
			synchronized (serialPort) {
				String msgWithNewline = msg + "\n";
				serialPort.writeBytes(msgWithNewline.getBytes());
			}
		} catch (SerialPortException ex) {
			System.err.println("Error sending message: " + ex.getMessage());
		}
	}

	@Override
	public String receiveMsg() throws InterruptedException {
		return queue.take();
	}

	@Override
	public boolean isMsgAvailable() {
		return !queue.isEmpty();
	}

	@Override
	public void close() {
		try {
			if (serialPort != null) {
				serialPort.removeEventListener();
				if (serialPort.isOpened()) {
					serialPort.closePort();
				}
			}
		} catch (Exception ex) {
			ex.printStackTrace();
		}
	}

	@Override
	public void serialEvent(SerialPortEvent event) {
		if (event.isRXCHAR()) {
			try {
				String msg = serialPort.readString(event.getEventValue());
				if (msg == null)
					return;

				msg = msg.replaceAll("\r", "");

				currentMsg.append(msg);

				int index = currentMsg.indexOf("\n");

				while (index >= 0) {
					String completeMsg = currentMsg.substring(0, index);

					if (!completeMsg.isEmpty()) {
						queue.put(completeMsg);
					}

					currentMsg.delete(0, index + 1);
					index = currentMsg.indexOf("\n");
				}

			} catch (Exception ex) {
				ex.printStackTrace();
				System.out.println("Error in serialEvent: " + ex.getMessage());
			}
		}
	}
}
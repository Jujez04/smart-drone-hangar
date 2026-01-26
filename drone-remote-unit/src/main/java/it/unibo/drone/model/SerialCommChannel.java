package it.unibo.drone.model;

import java.util.concurrent.ArrayBlockingQueue;
import java.util.concurrent.BlockingQueue;

import jssc.SerialPort;
import jssc.SerialPortEvent;
import jssc.SerialPortEventListener;
import jssc.SerialPortException;

/**
 * Comm channel implementation based on serial port.
 *
 * @author aricci
 *
 */
public class SerialCommChannel implements CommChannel, SerialPortEventListener {

	private SerialPort serialPort;
	private BlockingQueue<String> queue;
	private StringBuilder currentMsg = new StringBuilder();

	/**
	 * Constructor.
	 *
	 * @param port the serial port name
	 * @param rate the baud rate
	 * @throws Exception
	 */
	public SerialCommChannel(String port, int rate) throws Exception {
		queue = new ArrayBlockingQueue<String>(100);

		try {
			serialPort = new SerialPort(port);
			serialPort.openPort();

			serialPort.setParams(rate,
					SerialPort.DATABITS_8,
					SerialPort.STOPBITS_1,
					SerialPort.PARITY_NONE);

			serialPort.setFlowControlMode(SerialPort.FLOWCONTROL_RTSCTS_IN |
					SerialPort.FLOWCONTROL_RTSCTS_OUT);

			// serialPort.addEventListener(this, SerialPort.MASK_RXCHAR);
			serialPort.addEventListener(this);

			// Clear serial queue
			queue.clear();

		} catch (Exception ex) {
			if (serialPort != null) {
				serialPort.closePort();
			}
			throw ex;
		}
	}

	@Override
	public void sendMsg(String msg) {
		try {
            synchronized (serialPort) {
                String msgWithNewline = msg + "\n";
                serialPort.writeBytes(msgWithNewline.getBytes());
                System.out.println("[TX] " + msg);
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

	/**
	 * This should be called when you stop using the port.
	 * This will prevent port locking on platforms like Linux.
	 */
	@Override
	public void close() {
		try {
			if (serialPort != null) {
				serialPort.removeEventListener();
				serialPort.closePort();
			}
		} catch (Exception ex) {
			ex.printStackTrace();
		}
	}

	@Override
	public void serialEvent(SerialPortEvent event) {
		/* if there are bytes received in the input buffer */
		if (event.isRXCHAR()) {
			try {
				String msg = serialPort.readString(event.getEventValue());

				msg = msg.replaceAll("\r", "");

				currentMsg.append(msg);

				String buffer = currentMsg.toString();
				int index = buffer.indexOf("\n");
				// TODO: da vedere
				while (index >= 0) {
						String completeMsg = buffer.substring(0, index);
						if (!completeMsg.isEmpty()) {
							queue.put(completeMsg);
						}
						buffer = buffer.substring(index + 1);
				}
				currentMsg = new StringBuilder(buffer);
			} catch (Exception ex) {
				ex.printStackTrace();
				System.out.println("Error in receiving string from COM-port: " + ex);
			}
		}
	}
}
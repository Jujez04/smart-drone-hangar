package it.unibo.drone.model;

import java.util.concurrent.ArrayBlockingQueue;
import java.util.concurrent.BlockingQueue;

import jssc.SerialPort;
import jssc.SerialPortEvent;
import jssc.SerialPortEventListener;
import jssc.SerialPortException;

/**
 * Canale di comunicazione seriale robusto.
 * Corretto per evitare StringIndexOutOfBoundsException.
 */
public class SerialCommChannel implements CommChannel, SerialPortEventListener {

	private SerialPort serialPort;
	private BlockingQueue<String> queue;
	private StringBuilder currentMsg = new StringBuilder();

	public SerialCommChannel(String port, int rate) throws Exception {
		queue = new ArrayBlockingQueue<String>(100);

		// Inizializza la porta ma non chiamare close() qui!
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
				// System.out.println("[TX] " + msg); // Decommenta per debug TX
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

	/**
	 * Questo è il metodo che causava l'errore.
	 * Questa versione è riscritta per essere sicura.
	 */
	@Override
	public void serialEvent(SerialPortEvent event) {
		if (event.isRXCHAR()) {
			try {
				// Leggi la stringa dal buffer
				String msg = serialPort.readString(event.getEventValue());

				// Se msg è null (può capitare), esci
				if (msg == null)
					return;

				// Rimuovi ritorni a capo Windows strani (\r)
				msg = msg.replaceAll("\r", "");

				// Aggiungi al buffer globale
				currentMsg.append(msg);

				// CERCA E ESTRAI I MESSAGGI COMPLETI
				// Continua finché trovi un "a capo" (\n)
				int index = currentMsg.indexOf("\n");

				while (index >= 0) {
					// 1. Estrai il messaggio dall'inizio fino al \n
					String completeMsg = currentMsg.substring(0, index);

					// 2. Aggiungi alla coda solo se non è vuoto
					if (!completeMsg.isEmpty()) {
						queue.put(completeMsg);
					}

					// 3. CANCELLA la parte processata dal buffer (incluso il \n)
					// Questo previene l'errore di memoria/indice
					currentMsg.delete(0, index + 1);

					// 4. Cerca il prossimo \n nel buffer rimanente
					index = currentMsg.indexOf("\n");
				}

			} catch (Exception ex) {
				ex.printStackTrace();
				System.out.println("Error in serialEvent: " + ex.getMessage());
			}
		}
	}
}
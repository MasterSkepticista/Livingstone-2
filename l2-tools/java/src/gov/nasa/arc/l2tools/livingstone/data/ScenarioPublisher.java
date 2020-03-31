//
// * See the file "l2-tools/disclaimers-and-notices.txt" for 
// * information on usage and redistribution of this file, 
// * and for a DISCLAIMER OF ALL WARRANTIES.
//
package gov.nasa.arc.l2tools.livingstone.data;

/**
 * Creation date: (1/24/2001 9:53:03 AM)
 * @author: Andrew Bachmann
 */
public class ScenarioPublisher implements gov.nasa.arc.l2tools.data.EventPublisher {
	// listeners
	private javax.swing.DefaultListModel listeners = new javax.swing.DefaultListModel();
	/**
	 * ScenarioPublisher constructor comment.
	 */
	public ScenarioPublisher() {
		super();
	}
	public synchronized void addListener(gov.nasa.arc.l2tools.data.EventPublisher.Listener l) {
		getListeners().addElement(l);
	}
	protected javax.swing.DefaultListModel getListeners() {
		return listeners;
	}
	public synchronized void removeListener(gov.nasa.arc.l2tools.data.EventPublisher.Listener l) {
		getListeners().removeElement(l);
	}
	private void publishEvent(gov.nasa.arc.l2tools.data.Event event) {
		java.util.Enumeration enum=getListeners().elements();
		while (enum.hasMoreElements()) {
			((gov.nasa.arc.l2tools.data.EventPublisher.Listener)enum.nextElement()).eventPublished(event);
		}
	}

/**
 * Creation date: (1/24/2001 12:14:50 PM)
 * @param args java.lang.String[]
 */
public static void main(String[] args) {
	ScenarioPublisher publisher=new ScenarioPublisher();
	publisher.runMain(args);	
}

/**
 * Creation date: (1/24/2001 11:26:26 AM)
 * @param file java.io.File
 */
public void read(java.io.Reader r) throws java.io.IOException {
	java.io.LineNumberReader reader = new java.io.LineNumberReader(r);
	while (true) {
		String line = reader.readLine();
		if (line == null) break;
		java.util.StringTokenizer tokenizer = new java.util.StringTokenizer(line);
		if (!tokenizer.hasMoreTokens()) continue;
		String command = tokenizer.nextToken();
		java.util.List arguments = new java.util.ArrayList();
		while (tokenizer.hasMoreTokens()) {
			arguments.add(tokenizer.nextToken());
		}
		publishEvent(new gov.nasa.arc.l2tools.livingstone.data.L2Event(new L2Command(command,arguments)));
	}
}

/**
 * Creation date: (1/24/2001 12:14:50 PM)
 * @param args java.lang.String[]
 */
private void runMain(String[] args) {
	gov.nasa.arc.l2tools.data.PrintListener listener = new gov.nasa.arc.l2tools.data.PrintListener(null);
	addListener(listener);
	try {
		for (int i=0;(i<args.length);i++) {
			read(new java.io.FileReader(args[i]));
		}
	} catch (java.lang.Exception e) {
		System.err.println("exceptional.");
	}
}
}

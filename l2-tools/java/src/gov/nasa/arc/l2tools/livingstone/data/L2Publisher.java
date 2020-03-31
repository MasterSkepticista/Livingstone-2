//
// * See the file "l2-tools/disclaimers-and-notices.txt" for 
// * information on usage and redistribution of this file, 
// * and for a DISCLAIMER OF ALL WARRANTIES.
//
package gov.nasa.arc.l2tools.livingstone.data;

/**
 * Creation date: (1/26/2001 4:22:49 PM)
 * @author: Andrew Bachmann
 */
public class L2Publisher extends gov.nasa.arc.l2tools.data.EventBroadcaster {
	private class L2Thread extends Thread {
		java.util.List commandList = new java.util.ArrayList();
		public L2Thread() {
			super("L2 thread");
			start();
		}
		public void run() {
			while (true) {
				int range=commandList.size();
				for (int i=0; (i<range); i++) {
					executeCommand((gov.nasa.arc.l2tools.data.Event)commandList.get(i));
				}
				synchronized (commandList) {
					commandList.subList(0,range).clear();
					try {
						if (commandList.size()==0) {
							commandList.wait();
						}
					} catch (InterruptedException ie) {
					}
				}
			}
		}
		public void queueCommand(gov.nasa.arc.l2tools.data.Event commandEvent) {
			synchronized (commandList) {
				commandList.add(commandEvent);
				commandList.notify();
			}
		}
	}
	private L2Thread thread = new L2Thread();
	// the engine
	private LivingstoneEngineInterface livingstone;
	public LivingstoneEngineInterface getLivingstone() {
		return livingstone;
	}
	public void setLivingstone(LivingstoneEngineInterface newLivingstone) {
		livingstone = newLivingstone;
	}
	// constructors
	public L2Publisher() {
		this(null);
	}
	// event published
	public synchronized void eventPublished(gov.nasa.arc.l2tools.data.Event e) {
		if (e.getData() instanceof L2Command) {
			thread.queueCommand(e);
		}
	}

	/**
	 * Creation date: (1/24/2001 12:14:50 PM)
	 * @param args java.lang.String[]
	 */
	public static void main(String[] args) {
		L2Publisher publisher=new L2Publisher();
		publisher.runMain(args);	
	}
	/**
	 * Creation date: (1/24/2001 12:14:50 PM)
	 * @param args java.lang.String[]
	 */
	private void runMain(String[] args) {
		// create a scenario publisher
		ScenarioPublisher publisher = new ScenarioPublisher();
		// create a data proxy
		gov.nasa.arc.l2tools.data.proxy.Proxy proxy = new gov.nasa.arc.l2tools.data.proxy.Proxy();
		// tie in the proxy to the publisher
		publisher.addListener(proxy);
		// tie in this to the proxy
		proxy.addListener("L2Command",this);
		// print the output
		gov.nasa.arc.l2tools.data.EventPublisher.Listener listener 
		  = new gov.nasa.arc.l2tools.data.PrintListener(null);
		addListener(listener);
		// run it
		try {
			for (int i=0;(i<args.length);i++) {
				publisher.read(new java.io.FileReader(args[i]));
			}
		} catch (java.lang.Exception e) {
			System.err.println("exceptional.");
		}
	}

	public L2Publisher(LivingstoneEngineInterface livingstone) {
		super();
		setLivingstone(livingstone);
	}

	private int timestep = 0; // for faking it
		
	public void executeAssign(gov.nasa.arc.l2tools.data.TelemetryTime time, java.util.List arguments) {
		LivingstoneEngineInterface liv = getLivingstone();
		java.util.Iterator itr = arguments.iterator();
		while (itr.hasNext()) {
			String argument = (String)itr.next();
			int equalPos = argument.indexOf('=');
			L2Attribute attribute;
			if (equalPos > 0) {
				attribute = new L2Attribute(time,argument.substring(0,equalPos),argument.substring(equalPos+1));
			} else {
				attribute = new L2Attribute(time,argument);
			}
			if (liv!=null) {
				liv.assignJNI(attribute.getName(),attribute.getValue());
			} else {
				System.out.println("livingstone stub: assign "+attribute.getName()+"="+attribute.getValue());
			}
			super.eventPublished(new L2Event(attribute));
		}
	}

	public void executeCommand(gov.nasa.arc.l2tools.data.Event commandEvent) {
		gov.nasa.arc.l2tools.data.TelemetryTime time = null;
		if (commandEvent instanceof gov.nasa.arc.l2tools.data.TelemetryEvent) {
			time = ((gov.nasa.arc.l2tools.data.TelemetryEvent)commandEvent).getCreationTime();
		}
		L2Command command = (L2Command)commandEvent.getData();
		if ("assign".equals(command.getCommand())) {
			executeAssign(time,command.getArguments());
		} else
		if ("progress".equals(command.getCommand())) {
			executeProgress(time,command.getArguments());
		} else
		if ("fc".equals(command.getCommand())) {
			executeFindCandidates(time);
		}
	}

	public void executeFindCandidates(gov.nasa.arc.l2tools.data.TelemetryTime time) {
		LivingstoneEngineInterface liv = getLivingstone();
		if (liv!=null) {
			liv.findCandidatesJNI();
			// generate report
			super.eventPublished(new L2Event(new L2Engine(time)));
		} else {
			System.out.println("livingstone stub: find candidates");
			super.eventPublished(new L2Event(new L2Engine(time,"file"+timestep+".dat")));
		}
	}

	public void executeProgress(gov.nasa.arc.l2tools.data.TelemetryTime time, java.util.List arguments) {
		executeAssign(time,arguments);
		LivingstoneEngineInterface liv = getLivingstone();
		if (liv!=null) {
			liv.progressJNI("");
		} else {
			System.out.println("livingstone stub: progress");
		}
		if (liv!=null) {
			super.eventPublished(new L2Event(new L2Time(time,liv.getTimeStepJNI())));
		} else {
			super.eventPublished(new L2Event(new L2Time(time,timestep++)));
		}
	}
}

//
// * See the file "l2-tools/disclaimers-and-notices.txt" for 
// * information on usage and redistribution of this file, 
// * and for a DISCLAIMER OF ALL WARRANTIES.
//
package gov.nasa.arc.l2tools.livingstone.data;

/**
 * Creation date: (12/7/2000 2:23:38 PM)
 * @author: Andrew Bachmann
 */
public class L2Command extends L2Data {
	// constructor
	public L2Command() {
		this(null,null);
	}
	public L2Command(String newCommand) {
		this(newCommand,null);
	}
	public L2Command(String newCommand, java.util.List newArguments) {
		super();
		command=newCommand; arguments=newArguments;
	}
	// property: command		
	private String command;
	public String getCommand() {
		return command;
	}
	public void setCommand(String newCommand) {
		String oldCommand=command;
		command=newCommand;
		support.firePropertyChange("command",oldCommand,newCommand);
	}
	// property: arguments
	private java.util.List arguments;
	public java.util.List getArguments() {
		return arguments;
	}
	public void setArguments(java.util.List newArguments) {
		java.util.List oldArguments=arguments;
		arguments=newArguments;
		support.firePropertyChange("arguments",oldArguments,newArguments);
	}
	//
	public java.lang.String getPackageName() {
		return "L2Command";
	}
	public java.lang.String getClassName() {
		return getCommand();
	} 
	public java.lang.String getInstanceName() {
		if ((getArguments()==null) || (getArguments().size()<2))
			return null;
		else {
			String argument = getArguments().get(0).toString();
			int equalPos = argument.indexOf('=');
			if (equalPos > 0) {
				return argument.substring(0,equalPos);
			} else {
				return argument;
			}
		}
	} 
	public java.lang.String getValueName() {
		if ((getArguments()==null) || (getArguments().size()<2))
			return null;
		else {
			String argument = getArguments().get(0).toString();
			int equalPos = argument.indexOf('=');
			if (equalPos > 0) {
				return argument.substring(equalPos+1);
			} else {
				return argument;
			}
		}
	}

	public class ArgumentsIterator implements java.util.Iterator {
		private java.util.Iterator argumentIterator=getArguments().iterator();
		private java.util.Iterator nameIterator=new NameIterator();
		public boolean hasNext() {
			return argumentIterator.hasNext() || nameIterator.hasNext();
		}
		public Object next() {
			if (argumentIterator.hasNext()) {
				String argument = (String)argumentIterator.next();
				int equalPos = argument.indexOf('=');
				if (equalPos > 0) {
					return argument.substring(0,equalPos);
				} else {
					return argument;
				}
			} else {
				return nameIterator.next();
			}
		}
		public void remove() {
			throw new UnsupportedOperationException();
		}
	}

	public java.util.Iterator getRowNames() {
		return new ArgumentsIterator();
	}

	public String toString() {
		String string="L2Command \"";
		if (getCommand()!=null) {
			string+=getCommand();
			if (getArguments()!=null) {
				string+=getArguments().toString();
			}
		}
		string+="\"";
		return string;
	}
}

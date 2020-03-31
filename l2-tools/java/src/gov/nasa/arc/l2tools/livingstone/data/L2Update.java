//
// * See the file "l2-tools/disclaimers-and-notices.txt" for 
// * information on usage and redistribution of this file, 
// * and for a DISCLAIMER OF ALL WARRANTIES.
//
package gov.nasa.arc.l2tools.livingstone.data;

/**
 * Creation date: (1/29/2001 12:42:26 PM)
 * @author: Andrew Bachmann
 */
public abstract class L2Update extends L2Data {
	//
	public L2Update() {
		this(null);
	}
	public L2Update(gov.nasa.arc.l2tools.data.TelemetryTime commandTime) {
		super();
		this.commandTime = commandTime;
	}
	// property: commandTime
	private gov.nasa.arc.l2tools.data.TelemetryTime commandTime;
	public gov.nasa.arc.l2tools.data.TelemetryTime getCommandTime() {
		return commandTime;
	}
	public void setTime(gov.nasa.arc.l2tools.data.TelemetryTime newCommandTime) {
		gov.nasa.arc.l2tools.data.TelemetryTime oldCommandTime=commandTime;
		commandTime=newCommandTime;
		support.firePropertyChange("commandTime",oldCommandTime,newCommandTime);
	}
	//
	public java.lang.String getPackageName() {
		return "L2Update";
	}
}

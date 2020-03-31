//
// * See the file "l2-tools/disclaimers-and-notices.txt" for 
// * information on usage and redistribution of this file, 
// * and for a DISCLAIMER OF ALL WARRANTIES.
//
package gov.nasa.arc.l2tools.livingstone.data;

/**
 * Creation date: (1/29/2001 1:22:16 PM)
 * @author: Andrew Bachmann
 */
public class L2Time extends L2Update {
	//
	public L2Time() {
		this(null);
	}
	public L2Time(gov.nasa.arc.l2tools.data.TelemetryTime commandTime) {
		this(commandTime,-1);
	}
	public L2Time(gov.nasa.arc.l2tools.data.TelemetryTime commandTime, int time) {
		super(commandTime);
		this.time=time;
	}
	// property: time
	private int time;
	public int getTime() {
		return time;
	}
	public void setTime(int newTime) {
		int oldTime=time;
		time=newTime;
		support.firePropertyChange("time",oldTime,newTime);
	}
	//
	public String getClassName() {
		return "L2Time";
	}
	public String getInstanceName() {
		return null;
	}
	public String getValueName() {
		return null;
	}

	// 
	public String toString() {
		return "L2Time "+getTime();
	}
}

//
// * See the file "l2-tools/disclaimers-and-notices.txt" for 
// * information on usage and redistribution of this file, 
// * and for a DISCLAIMER OF ALL WARRANTIES.
//
package gov.nasa.arc.l2tools.livingstone.data;

/**
 * Creation date: (1/24/2001 12:04:56 PM)
 * @author: Andrew Bachmann
 */
public class L2Event extends gov.nasa.arc.l2tools.data.AbstractTelemetryEvent {
/**
 * L2Event constructor comment.
 */
public L2Event() {
	super();
}
/**
 * L2Event constructor comment.
 * @param data gov.nasa.arc.l2tools.livingstone.data.L2Data
 */
public L2Event(L2Data data) {
	super(data);
}
	public String toString() {
		boolean comma=false;
		String string="L2Event [";
		if (getCreationTime()!=null) {
			string+="c="+getCreationTime();
			comma=true;
		}
		if (getTransmissionTime()!=null) {
			if (comma) string+=",";
			string+="t="+getTransmissionTime();
			comma=true;
		}
		string+="]\n";
		string+="  ";
		string+=getData().toString();
		string+="\n";
		return string;
	}
}

//
// * See the file "l2-tools/disclaimers-and-notices.txt" for 
// * information on usage and redistribution of this file, 
// * and for a DISCLAIMER OF ALL WARRANTIES.
//
package gov.nasa.arc.l2tools.livingstone.data;

/**
 * Creation date: (12/7/2000 3:35:26 PM)
 * @author: Andrew Bachmann
 */
public class L2Engine extends L2Update {
	public L2Engine() {
		this(null);
	}
	public L2Engine(gov.nasa.arc.l2tools.data.TelemetryTime commandTime) {
		this(commandTime,null);
	}
	public L2Engine(gov.nasa.arc.l2tools.data.TelemetryTime commandTime, String filename) {
		super(commandTime);
		this.filename = filename;
	}
	// property: filename
	private String filename;
	public String getFilename() {
		return filename;
	}
	public void setFilename(String newFilename) {
		String oldFilename=filename;
		filename=newFilename;
		support.firePropertyChange("filename",oldFilename,newFilename);
	}
	// row stuff
	public java.lang.String getPackageName() {
		return "L2Engine";
	}
	public java.lang.String getClassName() {
		return null;
	}
	public java.lang.String getInstanceName() {
		return null;
	}
	public java.lang.String getValueName() {
		return null;
	}
	// 
	public String toString() {
		String string="L2Engine";
		if (getFilename()!=null) {
			string+=" \"";
			string+=getFilename();
			string+="\"";
		}
		return string;
	}
}

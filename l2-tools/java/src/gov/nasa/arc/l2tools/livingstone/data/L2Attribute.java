//
// * See the file "l2-tools/disclaimers-and-notices.txt" for 
// * information on usage and redistribution of this file, 
// * and for a DISCLAIMER OF ALL WARRANTIES.
//
package gov.nasa.arc.l2tools.livingstone.data;

/**
 * Creation date: (1/29/2001 12:36:42 PM)
 * @author: Andrew Bachmann
 */
public class L2Attribute extends L2Update {
	public L2Attribute() {
		this(null);
	}
	public L2Attribute(gov.nasa.arc.l2tools.data.TelemetryTime time) {
		this(time,null);
	}
	public L2Attribute(gov.nasa.arc.l2tools.data.TelemetryTime time,String name) {
		this(time,name,null);
	}
	public L2Attribute(gov.nasa.arc.l2tools.data.TelemetryTime time,String name, String value) {
		super(time);
		this.name=name;
		this.value=value;
	}
	// property: name		
	private String name;
	public String getName() {
		return name;
	}
	public void setName(String newName) {
		String oldName=name;
		name=newName;
		support.firePropertyChange("name",oldName,newName);
	}
	// property: value		
	private String value;
	public String getValue() {
		return value;
	}
	public void setValue(String newValue) {
		String oldValue=value;
		value=newValue;
		support.firePropertyChange("value",oldValue,newValue);
	}
	//
	public java.lang.String getClassName() {
		return "L2Attribute";
	} 
	public java.lang.String getInstanceName() {
		return getName();
	} 
	public java.lang.String getValueName() {
		return getValue();
	}
	// 
	public String toString() {
		String string="L2Attribute \"";
		if (getName()!=null) {
			string+=getName();
			if (getValue()!=null) {
				string+=" ";
				string+=getValue().toString();
			}
		}
		string+="\"";
		return string;
	}
}

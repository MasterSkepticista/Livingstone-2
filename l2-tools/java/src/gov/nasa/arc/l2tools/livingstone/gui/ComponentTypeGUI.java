//
// * See the file "l2-tools/disclaimers-and-notices.txt" for 
// * information on usage and redistribution of this file, 
// * and for a DISCLAIMER OF ALL WARRANTIES.
//
package gov.nasa.arc.l2tools.livingstone.gui;

/**
 * Creation date: (10/20/2000 9:22:08 AM)
 * @author: Andrew Bachmann
 */
public class ComponentTypeGUI extends gov.nasa.arc.l2tools.gui.GUI {
	private gov.nasa.arc.l2tools.livingstone.ComponentType fieldComponentType = new gov.nasa.arc.l2tools.livingstone.ComponentType();
/**
 * ComponentGUI constructor comment.
 */
public ComponentTypeGUI() {
	this(new gov.nasa.arc.l2tools.swing.Frame("ComponentTypeGUI"),new gov.nasa.arc.l2tools.livingstone.ComponentType());
}
/**
 * ComponentGUI constructor comment.
 * @param componentType gov.nasa.arc.l2tools.livingstone.ComponentType
 */
public ComponentTypeGUI(gov.nasa.arc.l2tools.livingstone.ComponentType componentType) {
	this(new gov.nasa.arc.l2tools.swing.Frame("ComponentTypeGUI"),componentType);
}
/**
 * ComponentGUI constructor comment.
 * @param view gov.nasa.arc.l2tools.swing.RootPaneContainer
 */
public ComponentTypeGUI(gov.nasa.arc.l2tools.swing.RootPaneContainer view) {
	this(view,new gov.nasa.arc.l2tools.livingstone.ComponentType());
}
/**
 * ComponentGUI constructor comment.
 * @param view gov.nasa.arc.l2tools.swing.RootPaneContainer
 * @param componentType gov.nasa.arc.l2tools.livingstone.ComponentType
 */
public ComponentTypeGUI(gov.nasa.arc.l2tools.swing.RootPaneContainer view, gov.nasa.arc.l2tools.livingstone.ComponentType componentType) {
	super(view);
	setComponentType(componentType);
}
/**
 * Gets the componentType property (gov.nasa.arc.l2tools.livingstone.ComponentType) value.
 * @return The componentType property value.
 * @see #setComponentType
 */
public gov.nasa.arc.l2tools.livingstone.ComponentType getComponentType() {
	return fieldComponentType;
}
/**
 * Sets the componentType property (gov.nasa.arc.l2tools.livingstone.ComponentType) value.
 * @param componentType The new value for the property.
 * @see #getComponentType
 */
public void setComponentType(gov.nasa.arc.l2tools.livingstone.ComponentType componentType) {
	gov.nasa.arc.l2tools.livingstone.ComponentType oldValue = fieldComponentType;
	fieldComponentType = componentType;
	firePropertyChange("componentType", oldValue, componentType);
}
}

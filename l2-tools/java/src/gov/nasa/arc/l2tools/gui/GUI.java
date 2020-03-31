//
// * See the file "l2-tools/disclaimers-and-notices.txt" for 
// * information on usage and redistribution of this file, 
// * and for a DISCLAIMER OF ALL WARRANTIES.
//
package gov.nasa.arc.l2tools.gui;

/**
 * Creation date: (10/19/2000 3:56:03 PM)
 * @author: Andrew Bachmann
 */
public class GUI {
	protected transient java.beans.PropertyChangeSupport propertyChange;
	private gov.nasa.arc.l2tools.swing.RootPaneContainer fieldView = null;
	private class ViewListener implements java.beans.PropertyChangeListener {
		public ViewListener() {
		}
		/**
		 * This method gets called when a bound property is changed.
		 * @param evt A PropertyChangeEvent object describing the event source 
		 *   	and the property that has changed.
		 */
		public void propertyChange(java.beans.PropertyChangeEvent evt) {
			java.lang.Object oldView = evt.getOldValue();
			java.lang.Object newView = evt.getNewValue();
			if (oldView instanceof gov.nasa.arc.l2tools.swing.Frame) {
				if (newView instanceof gov.nasa.arc.l2tools.swing.InternalFrame) {
					gov.nasa.arc.l2tools.swing.util.RootPaneContainers.transfer((gov.nasa.arc.l2tools.swing.Frame)oldView,(gov.nasa.arc.l2tools.swing.InternalFrame)newView);
					((gov.nasa.arc.l2tools.swing.Frame)oldView).setVisible(false);
				} else if (newView instanceof gov.nasa.arc.l2tools.swing.Frame) {
					gov.nasa.arc.l2tools.swing.util.RootPaneContainers.transfer((gov.nasa.arc.l2tools.swing.Frame)oldView,(gov.nasa.arc.l2tools.swing.Frame)newView);
					((gov.nasa.arc.l2tools.swing.Frame)oldView).setVisible(false);
				}
			} else if (oldView instanceof gov.nasa.arc.l2tools.swing.InternalFrame) {
				if (newView instanceof gov.nasa.arc.l2tools.swing.InternalFrame) {
					gov.nasa.arc.l2tools.swing.util.RootPaneContainers.transfer((gov.nasa.arc.l2tools.swing.InternalFrame)oldView,(gov.nasa.arc.l2tools.swing.InternalFrame)newView);
					((gov.nasa.arc.l2tools.swing.InternalFrame)oldView).setVisible(false);
				} else if (newView instanceof gov.nasa.arc.l2tools.swing.Frame) {
					gov.nasa.arc.l2tools.swing.util.RootPaneContainers.transfer((gov.nasa.arc.l2tools.swing.InternalFrame)oldView,(gov.nasa.arc.l2tools.swing.Frame)newView);
					((gov.nasa.arc.l2tools.swing.InternalFrame)oldView).setVisible(false);
				}
			}
		}
	}
	private ViewListener viewListener = new ViewListener();
	private java.lang.String fieldParent = new String();
/**
 * GUI constructor comment.
 */
public GUI() {
	this(new gov.nasa.arc.l2tools.swing.Frame("GUI"));
}
/**
 * GUI constructor comment.
 */
public GUI(gov.nasa.arc.l2tools.swing.RootPaneContainer view) {
	super();
	addPropertyChangeListener(viewListener);
	setView(view);
}
/**
 * The addPropertyChangeListener method was generated to support the propertyChange field.
 */
public synchronized void addPropertyChangeListener(java.beans.PropertyChangeListener listener) {
	getPropertyChange().addPropertyChangeListener(listener);
}
/**
 * The addPropertyChangeListener method was generated to support the propertyChange field.
 */
public synchronized void addPropertyChangeListener(java.lang.String propertyName, java.beans.PropertyChangeListener listener) {
	getPropertyChange().addPropertyChangeListener(propertyName, listener);
}
/**
 * The firePropertyChange method was generated to support the propertyChange field.
 */
public void firePropertyChange(java.beans.PropertyChangeEvent evt) {
	getPropertyChange().firePropertyChange(evt);
}
/**
 * The firePropertyChange method was generated to support the propertyChange field.
 */
public void firePropertyChange(java.lang.String propertyName, int oldValue, int newValue) {
	getPropertyChange().firePropertyChange(propertyName, oldValue, newValue);
}
/**
 * The firePropertyChange method was generated to support the propertyChange field.
 */
public void firePropertyChange(java.lang.String propertyName, java.lang.Object oldValue, java.lang.Object newValue) {
	getPropertyChange().firePropertyChange(propertyName, oldValue, newValue);
}
/**
 * The firePropertyChange method was generated to support the propertyChange field.
 */
public void firePropertyChange(java.lang.String propertyName, boolean oldValue, boolean newValue) {
	getPropertyChange().firePropertyChange(propertyName, oldValue, newValue);
}
/**
 * Gets the parent property (java.lang.String) value.
 * @return The parent property value.
 * @see #setParent
 */
public java.lang.String getParent() {
	return fieldParent;
}
/**
 * Accessor for the propertyChange field.
 */
protected java.beans.PropertyChangeSupport getPropertyChange() {
	if (propertyChange == null) {
		propertyChange = new java.beans.PropertyChangeSupport(this);
	};
	return propertyChange;
}
/**
 * Gets the view property (gov.nasa.arc.l2tools.swing.RootPaneContainer) value.
 * @return The view property value.
 * @see #setView
 */
public gov.nasa.arc.l2tools.swing.RootPaneContainer getView() {
	return fieldView;
}
/**
 * The hasListeners method was generated to support the propertyChange field.
 */
public synchronized boolean hasListeners(java.lang.String propertyName) {
	return getPropertyChange().hasListeners(propertyName);
}
/**
 * The removePropertyChangeListener method was generated to support the propertyChange field.
 */
public synchronized void removePropertyChangeListener(java.beans.PropertyChangeListener listener) {
	getPropertyChange().removePropertyChangeListener(listener);
}
/**
 * The removePropertyChangeListener method was generated to support the propertyChange field.
 */
public synchronized void removePropertyChangeListener(java.lang.String propertyName, java.beans.PropertyChangeListener listener) {
	getPropertyChange().removePropertyChangeListener(propertyName, listener);
}
/**
 * Sets the parent property (java.lang.String) value.
 * @param parent The new value for the property.
 * @see #getParent
 */
public void setParent(java.lang.String parent) {
	String oldValue = fieldParent;
	fieldParent = parent;
	firePropertyChange("parent", oldValue, parent);
}
/**
 * Sets the view property (gov.nasa.arc.l2tools.swing.RootPaneContainer) value.
 * @param view The new value for the property.
 * @see #getView
 */
public void setView(gov.nasa.arc.l2tools.swing.RootPaneContainer view) {
	gov.nasa.arc.l2tools.swing.RootPaneContainer oldValue = fieldView;
	fieldView = view;
	firePropertyChange("view", oldValue, view);
}
}

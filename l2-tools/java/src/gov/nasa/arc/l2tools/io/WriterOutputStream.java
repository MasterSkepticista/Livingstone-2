//
// * See the file "l2-tools/disclaimers-and-notices.txt" for 
// * information on usage and redistribution of this file, 
// * and for a DISCLAIMER OF ALL WARRANTIES.
//
package gov.nasa.arc.l2tools.io;

/**
 * Insert the type's description here.
 * Creation date: (3/20/2000 11:24:43 AM)
 * @author: Andrew Bachmann
 */
public class WriterOutputStream extends java.io.OutputStream {
	protected transient java.beans.PropertyChangeSupport propertyChange;
	private java.io.Writer fieldWriter = null;
/**
 * WriterOutputStream constructor comment.
 */
public WriterOutputStream() {
	super();
}
/**
 *
 * Creation date: (3/20/2000 11:26:20 AM)
 * @author: Andrew Bachmann
 * 
 * @param writer java.io.Writer
 */
public WriterOutputStream(java.io.Writer writer) {
	super();
	setWriter(writer);
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
public void close() throws java.io.IOException {
	getWriter().close();
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
public void flush() throws java.io.IOException {
	getWriter().flush();
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
 * Gets the writer property (java.io.Writer) value.
 * @return The writer property value.
 * @see #setWriter
 */
public java.io.Writer getWriter() {
	return fieldWriter;
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
 * Sets the writer property (java.io.Writer) value.
 * @param writer The new value for the property.
 * @see #getWriter
 */
public void setWriter(java.io.Writer writer) {
	java.io.Writer oldValue = fieldWriter;
	fieldWriter = writer;
	firePropertyChange("writer", oldValue, writer);
}
/**
 * write method comment.
 */
public void write(int b) throws java.io.IOException {
	getWriter().write(b);
}
}

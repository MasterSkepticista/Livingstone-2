//
// * See the file "l2-tools/disclaimers-and-notices.txt" for 
// * information on usage and redistribution of this file, 
// * and for a DISCLAIMER OF ALL WARRANTIES.
//
package gov.nasa.arc.l2tools.io;

/**
 * Insert the type's description here.
 * Creation date: (3/20/2000 10:54:34 AM)
 * @author: Andrew Bachmann
 */
public class ReaderInputStream extends java.io.InputStream {
	protected transient java.beans.PropertyChangeSupport propertyChange;
	private java.io.Reader fieldReader = null;
/**
 * ReaderInputStream constructor comment.
 */
public ReaderInputStream() {
	super();
}
/**
 *
 * Creation date: (3/20/2000 10:55:39 AM)
 * @author: Andrew Bachmann
 * 
 * @param reader java.io.Reader
 */
public ReaderInputStream(java.io.Reader reader) {
	super();
	setReader(reader);
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
	getReader().close();
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
 * Accessor for the propertyChange field.
 */
protected java.beans.PropertyChangeSupport getPropertyChange() {
	if (propertyChange == null) {
		propertyChange = new java.beans.PropertyChangeSupport(this);
	};
	return propertyChange;
}
/**
 * Gets the reader property (java.io.Reader) value.
 * @return The reader property value.
 * @see #setReader
 */
public java.io.Reader getReader() {
	return fieldReader;
}
/**
 * The hasListeners method was generated to support the propertyChange field.
 */
public synchronized boolean hasListeners(java.lang.String propertyName) {
	return getPropertyChange().hasListeners(propertyName);
}
/**
 * read method comment.
 */
public int read() throws java.io.IOException {
	return getReader().read();
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
public synchronized void reset() throws java.io.IOException {
	getReader().reset();
}
/**
 * Sets the reader property (java.io.Reader) value.
 * @param reader The new value for the property.
 * @see #getReader
 */
public void setReader(java.io.Reader reader) {
	java.io.Reader oldValue = fieldReader;
	fieldReader = reader;
	firePropertyChange("reader", oldValue, reader);
}
}

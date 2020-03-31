//
// * See the file "l2-tools/disclaimers-and-notices.txt" for 
// * information on usage and redistribution of this file, 
// * and for a DISCLAIMER OF ALL WARRANTIES.
//
package gov.nasa.arc.l2tools.io;

import java.lang.reflect.Constructor;
import java.lang.reflect.InvocationTargetException;
/**
 * A reader to unify URL and file connections
 * Creation date: (12/2/99 2:09:24 PM)
 * @author: Andrew Bachmann
 */
public class Reader extends java.io.InputStreamReader {
	protected transient java.beans.PropertyChangeSupport propertyChange;
	private Object fieldLocation = new Object();
/**
 * Reader constructor comment.
 * @param file java.io.File
 * @exception java.io.FileNotFoundException The exception description.
 */
public Reader(java.io.File file) throws java.io.FileNotFoundException {
	super(new java.io.FileInputStream(file));
	setLocation(file);
}
/**
 * Reader constructor comment.
 * @param fd java.io.FileDescriptor
 */
public Reader(java.io.FileDescriptor fd) {
	super(new java.io.FileInputStream(fd));
	setLocation(fd);
}
/**
 * Reader constructor comment.
 * @param in java.io.InputStream
 */
public Reader(java.io.InputStream in) {
	super(in);
	setLocation(in);
}
/**
 * Reader constructor comment.
 * @param in java.io.InputStream
 * @param enc java.lang.String
 * @exception java.io.UnsupportedEncodingException The exception description.
 */
public Reader(java.io.InputStream in, String enc) throws java.io.UnsupportedEncodingException {
	super(in, enc);
	setLocation(in);
}
/**
 * Creation date: (4/9/2000 3:16:52 AM)
 * @author: Andrew Bachmann
 * 
 * @param reader java.io.Reader
 */
public Reader(java.io.Reader reader) {
	super(new ReaderInputStream(reader));
	setLocation(reader);
}
/**
 * Open the reader with an URL.  This opens a connection to the URL immediately.
 * Creation date: (12/2/99 1:58:52 PM)
 * @author: Andrew Bachmann
 * @param url java.net.URL
 * @exception java.net.MalformedURLException, java.io.IOException
 */
public Reader(java.net.URL url) throws java.net.MalformedURLException, java.io.IOException {
	super(url.openConnection().getInputStream());
	setLocation(url);
}
/**
 * The addPropertyChangeListener method was generated to support the propertyChange field.
 */
public synchronized void addPropertyChangeListener(java.beans.PropertyChangeListener listener) {
	getPropertyChange().addPropertyChangeListener(listener);
}
/**
 * A constructor for readers from strings.
 * Unfortunately stupid java does not allow you
 * to put something like this into a constructor.
 * Creation date: (12/10/99 2:32:30 PM)
 * @author: Andrew Bachmann
 * @return gov.nasa.arc.l2tools.io.Reader
 * @param object java.lang.Object
 */
public static Reader create(Class clazz, String string) 
	throws java.io.IOException {
	Class[] parameterTypes = new Class[1]; 
	Object[] parameters = new Object[1];
	Constructor constructor;
	try {
		java.io.File file = new java.io.File(string);
		parameterTypes[0] = file.getClass();
		parameters[0] = file;
		constructor = clazz.getDeclaredConstructor(parameterTypes);
		return (Reader)constructor.newInstance(parameters);
	} catch (Exception e) {
		try {
			java.net.URL url = new java.net.URL(string);
			parameterTypes[0] = url.getClass();
			parameters[0] = url;
			constructor = clazz.getDeclaredConstructor(parameterTypes);
			return (Reader)constructor.newInstance(parameters);
		} catch (Exception e2) {
			throw new java.io.IOException("unable to find file or URL");
		}			
	}
}
/**
 * The firePropertyChange method was generated to support the propertyChange field.
 */
public void firePropertyChange(String propertyName, Object oldValue, Object newValue) {
	getPropertyChange().firePropertyChange(propertyName, oldValue, newValue);
}
/**
 * Gets the location property (java.lang.Object) value.
 * @return The location property value.
 * @see #setLocation
 */
public Object getLocation() {
	return fieldLocation;
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
 * The removePropertyChangeListener method was generated to support the propertyChange field.
 */
public synchronized void removePropertyChangeListener(java.beans.PropertyChangeListener listener) {
	getPropertyChange().removePropertyChangeListener(listener);
}
/**
 * Sets the location property (java.lang.Object) value.
 * @param location The new value for the property.
 * @see #getLocation
 */
public void setLocation(Object location) {
	Object oldValue = fieldLocation;
	fieldLocation = location;
	firePropertyChange("location", oldValue, location);
}
}

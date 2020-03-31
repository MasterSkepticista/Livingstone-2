//
// * See the file "l2-tools/disclaimers-and-notices.txt" for 
// * information on usage and redistribution of this file, 
// * and for a DISCLAIMER OF ALL WARRANTIES.
//
package gov.nasa.arc.l2tools.io;

/**
 * A writer to unify URL and file connections
 * Creation date: (12/2/99 2:09:24 PM)
 * @author: Andrew Bachmann
 */
public class Writer extends java.io.OutputStreamWriter {
	protected transient java.beans.PropertyChangeSupport propertyChange;
/**
 * Creation date: (4/9/2000 3:20:14 AM)
 * @author: Andrew Bachmann
 * 
 * @param writer java.io.Writer
 */
public Writer(Writer writer) {
	super(new WriterOutputStream(writer));
}
/**
 * Writer constructor comment.
 * @param file java.io.File
 * @exception java.io.FileNotFoundException The exception description.
 */
public Writer(java.io.File file) throws java.io.IOException {
	super(new java.io.FileOutputStream(file));
}
/**
 * Writer constructor comment.
 * @param fd java.io.FileDescriptor
 */
public Writer(java.io.FileDescriptor fd) {
	super(new java.io.FileOutputStream(fd));
}
/**
 * Writer constructor comment.
 * @param in java.io.OutputStream
 */
public Writer(java.io.OutputStream out) {
	super(out);
}
/**
 * Writer constructor comment.
 * @param in java.io.OutputStream
 * @param enc java.lang.String
 * @exception java.io.UnsupportedEncodingException The exception description.
 */
public Writer(java.io.OutputStream out, String enc) throws java.io.UnsupportedEncodingException {
	super(out, enc);
}
/**
 * Creation date: (4/9/2000 3:20:14 AM)
 * @author: Andrew Bachmann
 * 
 * @param writer java.io.Writer
 */
public Writer(java.io.Writer writer) {
	super(new WriterOutputStream(writer));
}
/**
 * Try to interpret name as a filename.  Failing that, try it as an URL
 * @param fileName java.lang.String
 * @exception java.io.FileNotFoundException The exception description.
 */
public Writer(String name) throws java.io.IOException {
	super(findOutputStream(name));
}
/**
 * Open the writer with an URL.  This opens a connection to the URL immediately.
 * Creation date: (12/2/99 1:58:52 PM)
 * @author: Andrew Bachmann
 * @param url java.net.URL
 * @exception java.net.MalformedURLException, java.io.IOException
 */
public Writer(java.net.URL url) throws java.net.MalformedURLException, java.io.IOException {
	super(url.openConnection().getOutputStream());
}
/**
 * The addPropertyChangeListener method was generated to support the propertyChange field.
 */
public synchronized void addPropertyChangeListener(java.beans.PropertyChangeListener listener) {
	getPropertyChange().addPropertyChangeListener(listener);
}
/**
 * get a FileOutputStream if name defines a file.
 * get an OutputStream if name defines an URL.  
 * This opens a connection to the url immediately.
 * Creation date: (12/2/99 2:20:11 PM)
 * @author: Andrew Bachmann
 * @return java.io.OutputStream
 * @param name java.lang.String
 * @exception IOException
 */
public static java.io.OutputStream findOutputStream(String name) throws java.io.IOException {
	try {
		java.io.FileOutputStream fileOutputStream = new java.io.FileOutputStream(name);
		return fileOutputStream;
	} catch (java.io.IOException ioe) {
		try {
			java.net.URL url = new java.net.URL(name);
			return url.openConnection().getOutputStream();
		} catch (java.net.MalformedURLException mue) {
			throw new java.io.IOException("file not found, URL not found: " + name);
		} catch (java.io.IOException ioe2) {
			throw new java.io.IOException("file not found, URL not found: " + name);
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
}

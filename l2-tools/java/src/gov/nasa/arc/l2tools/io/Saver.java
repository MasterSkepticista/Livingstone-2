//
// * See the file "l2-tools/disclaimers-and-notices.txt" for 
// * information on usage and redistribution of this file, 
// * and for a DISCLAIMER OF ALL WARRANTIES.
//
package gov.nasa.arc.l2tools.io;

/**
 * Insert the type's description here.
 * Creation date: (3/19/2000 11:53:41 AM)
 * @author: Andrew Bachmann
 */
public abstract class Saver {
	private Writer writer = null;
/**
 *
 * Creation date: (3/19/2000 12:13:17 PM)
 * @author: Andrew Bachmann
 * 
 */
public Saver() {
	super();
}
/**
 *
 * Creation date: (3/19/2000 12:12:22 PM)
 * @author: Andrew Bachmann
 * 
 * @param writer gov.nasa.arc.l2tools.io.Writer
 */
public Saver(Writer writer) {
	setWriter(writer);
}
/**
 * Writer constructor comment.
 * @param file java.io.File
 * @exception java.io.FileNotFoundException The exception description.
 */
public Saver(java.io.File file) throws java.io.IOException {
	setWriter(new Writer(new java.io.FileOutputStream(file)));
}
/**
 * Writer constructor comment.
 * @param fd java.io.FileDescriptor
 */
public Saver(java.io.FileDescriptor fd) {
	setWriter(new Writer(fd));
}
/**
 * Writer constructor comment.
 * @param in java.io.OutputStream
 */
public Saver(java.io.OutputStream out) {
	setWriter(new Writer(out));
}
/**
 * Writer constructor comment.
 * @param in java.io.OutputStream
 * @param enc java.lang.String
 * @exception java.io.UnsupportedEncodingException The exception description.
 */
public Saver(java.io.OutputStream out, String enc) throws java.io.UnsupportedEncodingException {
	setWriter(new Writer(out, enc));
}
/**
 * Try to interpret name as a filename.  Failing that, try it as an URL
 * @param fileName java.lang.String
 * @exception java.io.FileNotFoundException The exception description.
 */
public Saver(String name) throws java.io.IOException {
	setWriter(new Writer(name));
}
/**
 * Open the writer with an URL.  This opens a connection to the URL immediately.
 * Creation date: (12/2/99 1:58:52 PM)
 * @author: Andrew Bachmann
 * @param url java.net.URL
 * @exception java.net.MalformedURLException, java.io.IOException
 */
public Saver(java.net.URL url) throws java.net.MalformedURLException, java.io.IOException {
	setWriter(new Writer(url));
}
/**
 *
 * Creation date: (3/19/2000 11:55:32 AM)
 * @author: Andrew Bachmann
 * 
 * @return gov.nasa.arc.l2tools.io.Writer
 */
public Writer getWriter() {
	return writer;
}
/**
 *
 * Creation date: (3/19/2000 12:00:48 PM)
 * @author: Andrew Bachmann
 * 
 * @param project gov.nasa.arc.l2tools.DataObject
 */
public abstract void save(gov.nasa.arc.l2tools.DataObject object) throws Exception;
/**
 *
 * Creation date: (3/19/2000 12:00:48 PM)
 * @author: Andrew Bachmann
 * 
 * @param project gov.nasa.arc.l2tools.Project
 */
public abstract void save(gov.nasa.arc.l2tools.Project project) throws Exception;
/**
 *
 * Creation date: (3/19/2000 11:55:32 AM)
 * @author: Andrew Bachmann
 * 
 * @param newWriter gov.nasa.arc.l2tools.io.Writer
 */
public void setWriter(Writer newWriter) {
	writer = newWriter;
}
}

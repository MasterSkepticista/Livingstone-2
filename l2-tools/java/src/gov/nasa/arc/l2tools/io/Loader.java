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
public abstract class Loader {
	private Reader reader = null;
	private gov.nasa.arc.l2tools.Workspace workspace = null;
/**
 * Loader constructor comment.
 */
public Loader() {
	super();
}
/**
 * Loader constructor comment.
 */
public Loader(gov.nasa.arc.l2tools.Workspace workspace) {
	super();
	setWorkspace(workspace);
}
/**
 *
 * Creation date: (3/19/2000 11:57:53 AM)
 * @author: Andrew Bachmann
 * 
 * @param workspace gov.nasa.arc.l2tools.Workspace 
 * @param reader gov.nasa.arc.l2tools.io.Reader
 */
public Loader(gov.nasa.arc.l2tools.Workspace workspace, Reader reader) {
	setReader(reader);
	setWorkspace(workspace);
}
/**
 * Reader constructor comment.
 * @param workspace gov.nasa.arc.l2tools.Workspace 
 * @param file java.io.File
 * @exception java.io.FileNotFoundException The exception description.
 */
public Loader(gov.nasa.arc.l2tools.Workspace workspace, java.io.File file) throws java.io.FileNotFoundException {
	setWorkspace(workspace);
	setReader(new Reader(file));
}
/**
 * Reader constructor comment.
 * @param workspace gov.nasa.arc.l2tools.Workspace 
 * @param fd java.io.FileDescriptor
 */
public Loader(gov.nasa.arc.l2tools.Workspace workspace, java.io.FileDescriptor fd) {
	setReader(new Reader(fd));
	setWorkspace(workspace);
}
/**
 * Reader constructor comment.
 * @param workspace gov.nasa.arc.l2tools.Workspace 
 * @param in java.io.InputStream
 */
public Loader(gov.nasa.arc.l2tools.Workspace workspace, java.io.InputStream in) {
	setReader(new Reader(in));
	setWorkspace(workspace);
}
/**
 * Reader constructor comment.
 * @param workspace gov.nasa.arc.l2tools.Workspace 
 * @param in java.io.InputStream
 * @param enc java.lang.String
 * @exception java.io.UnsupportedEncodingException The exception description.
 */
public Loader(gov.nasa.arc.l2tools.Workspace workspace, java.io.InputStream in, String enc) throws java.io.UnsupportedEncodingException {
	setReader(new Reader(in, enc));
	setWorkspace(workspace);
}
/**
 *
 * Creation date: (3/19/2000 11:57:53 AM)
 * @author: Andrew Bachmann
 * 
 * @param workspace gov.nasa.arc.l2tools.Workspace 
 * @param reader java.io.Reader
 */
public Loader(gov.nasa.arc.l2tools.Workspace workspace, java.io.Reader reader) {
	setReader(new Reader(reader));
	setWorkspace(workspace);
}
/**
 * Open the reader with an URL.  This opens a connection to the URL immediately.
 * Creation date: (12/2/99 1:58:52 PM)
 * @author: Andrew Bachmann
 * @param workspace gov.nasa.arc.l2tools.Workspace 
 * @param url java.net.URL
 * @exception java.net.MalformedURLException, java.io.IOException
 */
public Loader(gov.nasa.arc.l2tools.Workspace workspace, java.net.URL url) throws java.net.MalformedURLException, java.io.IOException {
	setReader(new Reader(url));
	setWorkspace(workspace);
}
/**
 *
 * Creation date: (3/19/2000 11:55:32 AM)
 * @author: Andrew Bachmann
 * 
 * @return gov.nasa.arc.l2tools.io.Reader
 */
public Reader getReader() {
	return reader;
}
/**
 *
 * Creation date: (3/20/2000 12:36:29 PM)
 * @author: Andrew Bachmann
 * 
 * @return gov.nasa.arc.l2tools.Workspace
 */
public gov.nasa.arc.l2tools.Workspace getWorkspace() {
	return workspace;
}
/**
 *
 * Creation date: (3/19/2000 12:01:48 PM)
 * @author: Andrew Bachmann
 *
 * @return gov.nasa.arc.l2tools.DataObject
 */
public abstract gov.nasa.arc.l2tools.DataObject load() throws Exception;
/**
 *
 * Creation date: (3/19/2000 12:00:48 PM)
 * @author: Andrew Bachmann
 * 
 * @param defaultProject gov.nasa.arc.l2tools.Project
 * @return gov.nasa.arc.l2tools.DataObject
 */
public abstract gov.nasa.arc.l2tools.DataObject load(gov.nasa.arc.l2tools.Project defaultProject) throws Exception;
/**
 *
 * Creation date: (3/19/2000 11:55:32 AM)
 * @author: Andrew Bachmann
 * 
 * @param newReader gov.nasa.arc.l2tools.io.Reader
 */
private void setReader(Reader newReader) {
	reader = newReader;
}
/**
 *
 * Creation date: (3/20/2000 12:36:29 PM)
 * @author: Andrew Bachmann
 * 
 * @param newWorkspace gov.nasa.arc.l2tools.Workspace
 */
private void setWorkspace(gov.nasa.arc.l2tools.Workspace newWorkspace) {
	workspace = newWorkspace;
}
}

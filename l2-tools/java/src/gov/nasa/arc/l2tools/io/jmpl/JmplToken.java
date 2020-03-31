//
// * See the file "l2-tools/disclaimers-and-notices.txt" for 
// * information on usage and redistribution of this file, 
// * and for a DISCLAIMER OF ALL WARRANTIES.
//
package gov.nasa.arc.l2tools.io.jmpl;

/**
 * Creation date: (7/2/2000 5:55:06 PM)
 * @author: Andrew Bachmann
 */
public class JmplToken extends antlr.CommonToken {
	// most tokens will want column and filename information
	private int column;
	private java.lang.String filename = null;
/**
 * JmplToken constructor comment.
 */
public JmplToken() {
	super();
}
/**
 * JmplToken constructor comment.
 * @param t int
 * @param txt java.lang.String
 */
public JmplToken(int t, String txt) {
	super(t, txt);
}
/**
 * JmplToken constructor comment.
 * @param s java.lang.String
 */
public JmplToken(String s) {
	super(s);
}
	public int getColumn() { return column; }
/**
 * Creation date: (7/2/2000 6:46:48 PM)
 * @author: Andrew Bachmann
 * 
 * @return java.lang.String
 */
public java.lang.String getFilename() {
	return filename;
}
	public void setColumn(int c) { column=c; }
/**
 * Creation date: (7/2/2000 6:46:48 PM)
 * @author: Andrew Bachmann
 * 
 * @param newFilename java.lang.String
 */
public void setFilename(java.lang.String newFilename) {
	filename = newFilename;
}
}

//
// * See the file "l2-tools/disclaimers-and-notices.txt" for 
// * information on usage and redistribution of this file, 
// * and for a DISCLAIMER OF ALL WARRANTIES.
//
package gov.nasa.arc.l2tools;

/**
 * Insert the type's description here.
 * Creation date: (3/10/2000 1:36:41 PM)
 * @author: Andrew Bachmann
 */
public class Exception extends java.lang.Exception {
	private java.lang.String message = "\n";
/**
 * Exception constructor comment.
 */
public Exception() {
	super();
	setMessage("\n");
}
/**
 * Exception constructor comment.
 * @param s java.lang.String
 */
public Exception(String s) {
	this();
	concatMessage(s);
}
/**
 *
 * Creation date: (3/24/2000 10:47:53 AM)
 * @author: Andrew Bachmann
 * 
 * @param str java.lang.String
 */
public String concatMessage(java.lang.String str) {
	message += str;
	return message;
}
/**
 *
 * Creation date: (3/24/2000 10:47:53 AM)
 * @author: Andrew Bachmann
 * 
 * @return java.lang.String
 */
public java.lang.String getMessage() {
	return message;
}
/**
 *
 * Creation date: (3/24/2000 10:47:53 AM)
 * @author: Andrew Bachmann
 * 
 * @param newMessage java.lang.String
 */
protected void setMessage(java.lang.String newMessage) {
	message = newMessage;
}
}

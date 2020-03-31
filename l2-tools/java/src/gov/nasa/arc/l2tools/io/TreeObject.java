//
// * See the file "l2-tools/disclaimers-and-notices.txt" for 
// * information on usage and redistribution of this file, 
// * and for a DISCLAIMER OF ALL WARRANTIES.
//
package gov.nasa.arc.l2tools.io;

/**
 * Insert the type's description here.
 * Creation date: (3/3/2000 4:17:50 PM)
 * @author: Andrew Bachmann
 */
public interface TreeObject {
/**
 *
 * Creation date: (3/3/2000 4:20:38 PM)
 * @author: Andrew Bachmann
 * 
 * @return java.lang.String
 * @param field java.lang.String
 */
String get(String field);
/**
 *
 * Creation date: (3/6/2000 1:24:02 PM)
 * @author: Andrew Bachmann
 * 
 * @return javax.swing.ListModel
 */
javax.swing.DefaultListModel getChildren();
/**
 *
 * Creation date: (3/6/2000 1:24:38 PM)
 * @author: Andrew Bachmann
 * 
 * @return gov.nasa.arc.l2tools.io.TreeObject
 */
TreeObject getParent();
/**
 *
 * Creation date: (3/3/2000 4:20:11 PM)
 * @author: Andrew Bachmann
 * 
 * @param field java.lang.String
 * @param value java.lang.String
 */
void set(String field, String value);
/**
 *
 * Creation date: (3/6/2000 1:27:00 PM)
 * @author: Andrew Bachmann
 * 
 * @param newParent gov.nasa.arc.l2tools.io.TreeObject
 */
void setParent(TreeObject newParent);
}

//
// * See the file "l2-tools/disclaimers-and-notices.txt" for 
// * information on usage and redistribution of this file, 
// * and for a DISCLAIMER OF ALL WARRANTIES.
//
package gov.nasa.arc.l2tools.io.jmpl;

/**
 * Creation date: (4/9/2000 3:13:25 AM)
 * @author: Andrew Bachmann
 */
public class JmplASTFactory extends antlr.ASTFactory {
	private java.lang.String location = null;
/**
 * Creation date: (4/9/2000 3:25:07 AM)
 * @author: Andrew Bachmann
 * 
 * @param location java.lang.Object
 */
public JmplASTFactory(Object location) {
	if (location instanceof java.io.File) {
		setLocation(((java.io.File)location).getPath());
	} else if (location instanceof java.net.URL) {
		setLocation(((java.net.URL)location).toExternalForm());
	}	else if (location instanceof java.io.FileDescriptor) {
		setLocation(((java.io.FileDescriptor)location).toString());
	} else if (location instanceof java.io.InputStream) {
		setLocation(((java.io.InputStream)location).toString());
	} else if (location instanceof java.io.Reader) {
		setLocation(((java.io.Reader)location).toString());
	} else {
		setLocation(location.toString());
	}
	setASTNodeType("gov.nasa.arc.l2tools.io.jmpl.JmplASTNode");
}
/** Create a new empty AST node; if the user did not specify
 *  an AST node type, then create a default one: CommonAST.
 */
public antlr.collections.AST create() {
	antlr.collections.AST t = null;
	if (theASTNodeTypeClass == null) {
	    t = new JmplASTNode();
	    ((JmplASTNode)t).setFilename(getLocation());
	} else {
	    try {
		t = (antlr.collections.AST) theASTNodeTypeClass.newInstance(); // make a new one
	    } catch (Exception e) {
		antlr.Tool.warning("Can't create AST Node " + theASTNodeType);
		return null;
	    }
	}
	return t;
}
/**
 * Creation date: (4/9/2000 3:25:28 AM)
 * @author: Andrew Bachmann
 * 
 * @return java.lang.String
 */
public java.lang.String getLocation() {
	return location;
}
/**
 * Creation date: (4/9/2000 3:25:28 AM)
 * @author: Andrew Bachmann
 * 
 * @param newLocation java.lang.String
 */
protected void setLocation(java.lang.String newLocation) {
	location = newLocation;
}
}

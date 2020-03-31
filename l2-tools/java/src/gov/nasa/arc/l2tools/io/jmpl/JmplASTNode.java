//
// * See the file "l2-tools/disclaimers-and-notices.txt" for 
// * information on usage and redistribution of this file, 
// * and for a DISCLAIMER OF ALL WARRANTIES.
//
package gov.nasa.arc.l2tools.io.jmpl;

/**
 * Creation date: (4/9/2000 2:53:45 AM)
 * @author: Andrew Bachmann
 */
public class JmplASTNode extends antlr.CommonAST {
	private String filename = "";
	private int line = -1;
	private int column = -1;
public JmplASTNode() {
	super();
}
public JmplASTNode(antlr.Token tok) {
	initialize(tok);
}
/**
 * Creation date: (4/9/2000 3:01:47 AM)
 * @author: Andrew Bachmann
 * 
 * @return int
 */
public int getColumn() {
	return column;
}
/**
 * Creation date: (4/9/2000 2:58:31 AM)
 * @author: Andrew Bachmann
 * 
 * @return java.lang.String
 */
public java.lang.String getFilename() {
	return filename;
}
/**
 * Creation date: (4/9/2000 2:58:31 AM)
 * @author: Andrew Bachmann
 * 
 * @return int
 */
public int getLine() {
	return line;
}
public void initialize(int t, String txt) {
	super.initialize(t,txt);
}
	public void initialize(antlr.collections.AST t) {
		super.initialize(t);
		setLine(((JmplASTNode)t).getLine());
		setColumn(((JmplASTNode)t).getColumn());
		setFilename(((JmplASTNode)t).getFilename());
	}
public void initialize(antlr.Token tok) {
	super.initialize(tok);
	setLine(((JmplToken)tok).getLine());
	setColumn(((JmplToken)tok).getColumn());
	setFilename(((JmplToken)tok).getFilename());
}
/**
 * Creation date: (4/9/2000 3:01:47 AM)
 * @author: Andrew Bachmann
 * 
 * @param newColumn int
 */
protected void setColumn(int newColumn) {
	column = newColumn;
}
/**
 * Creation date: (4/9/2000 2:58:31 AM)
 * @author: Andrew Bachmann
 * 
 * @param newFilename java.lang.String
 */
protected void setFilename(java.lang.String newFilename) {
	filename = newFilename;
}
/**
 * Creation date: (4/9/2000 2:58:31 AM)
 * @author: Andrew Bachmann
 * 
 * @param newLine int
 */
protected void setLine(int newLine) {
	line = newLine;
}
}

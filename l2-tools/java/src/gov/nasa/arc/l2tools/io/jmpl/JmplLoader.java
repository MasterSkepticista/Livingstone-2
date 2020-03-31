//
// * See the file "l2-tools/disclaimers-and-notices.txt" for 
// * information on usage and redistribution of this file, 
// * and for a DISCLAIMER OF ALL WARRANTIES.
//
package gov.nasa.arc.l2tools.io.jmpl;

/**
 * Insert the type's description here.
 * Creation date: (3/20/2000 12:44:28 PM)
 * @author: Andrew Bachmann
 */
public class JmplLoader extends gov.nasa.arc.l2tools.io.Loader {
/**
 * XmplLoader constructor comment.
 */
public JmplLoader() {
	super();
}
/**
 * XmplLoader constructor comment.
 * @param workspace gov.nasa.arc.l2tools.Workspace
 */
public JmplLoader(gov.nasa.arc.l2tools.Workspace workspace) {
	super(workspace);
}
/**
 * XmplLoader constructor comment.
 * @param workspace gov.nasa.arc.l2tools.Workspace
 * @param reader gov.nasa.arc.l2tools.io.Reader
 */
public JmplLoader(gov.nasa.arc.l2tools.Workspace workspace, gov.nasa.arc.l2tools.io.Reader reader) {
	super(workspace, reader);
}
/**
 * XmplLoader constructor comment.
 * @param workspace gov.nasa.arc.l2tools.Workspace
 * @param file java.io.File
 * @exception java.io.FileNotFoundException The exception description.
 */
public JmplLoader(gov.nasa.arc.l2tools.Workspace workspace, java.io.File file) throws java.io.FileNotFoundException {
	super(workspace, file);
}
/**
 * XmplLoader constructor comment.
 * @param workspace gov.nasa.arc.l2tools.Workspace
 * @param fd java.io.FileDescriptor
 */
public JmplLoader(gov.nasa.arc.l2tools.Workspace workspace, java.io.FileDescriptor fd) {
	super(workspace, fd);
}
/**
 * XmplLoader constructor comment.
 * @param workspace gov.nasa.arc.l2tools.Workspace
 * @param in java.io.InputStream
 */
public JmplLoader(gov.nasa.arc.l2tools.Workspace workspace, java.io.InputStream in) {
	super(workspace, in);
}
/**
 * XmplLoader constructor comment.
 * @param workspace gov.nasa.arc.l2tools.Workspace
 * @param in java.io.InputStream
 * @param enc java.lang.String
 * @exception java.io.UnsupportedEncodingException The exception description.
 */
public JmplLoader(gov.nasa.arc.l2tools.Workspace workspace, java.io.InputStream in, String enc) throws java.io.UnsupportedEncodingException {
	super(workspace, in, enc);
}
/**
 * XmplLoader constructor comment.
 * @param workspace gov.nasa.arc.l2tools.Workspace
 * @param reader java.io.Reader
 */
public JmplLoader(gov.nasa.arc.l2tools.Workspace workspace, java.io.Reader reader) {
	super(workspace, reader);
}
/**
 * XmplLoader constructor comment.
 * @param workspace gov.nasa.arc.l2tools.Workspace
 * @param url java.net.URL
 * @exception java.io.IOException The exception description.
 * @exception java.net.MalformedURLException The exception description.
 */
public JmplLoader(gov.nasa.arc.l2tools.Workspace workspace, java.net.URL url) throws java.io.IOException, java.net.MalformedURLException {
	super(workspace, url);
}
/**
 * load method comment.
 */
public gov.nasa.arc.l2tools.DataObject load() throws Exception {
	return load(null);
}
/**
 * load method comment.
 */
public gov.nasa.arc.l2tools.DataObject load(gov.nasa.arc.l2tools.Project defaultProject) throws Exception {
	JmplLexer lexer=new JmplLexer(getReader());
	lexer.initialize(getReader().getLocation());
	JmplParser parser=new JmplParser(lexer);
	JmplASTFactory factory=new JmplASTFactory(getReader().getLocation());
	parser.setASTFactory(factory);
	parser.jmpl();
	antlr.collections.AST resultTree = parser.getAST();
	JmplTreeParser treeParser = new JmplTreeParser();
	if (defaultProject==null) {
		defaultProject=new gov.nasa.arc.l2tools.Project();
		getWorkspace().getChildren().addElement(defaultProject);
	}
	treeParser.jmpl(resultTree,defaultProject);
	return null;
}
/**
 *
 * Creation date: (3/20/2000 12:57:17 PM)
 * @author: Andrew Bachmann
 * 
 * @param args java.lang.String[]
 */
public static void main(String[] args) throws Exception {
	if (args.length==0) {
		System.out.println("Supply a filename:");
		System.out.println("java gov.nasa.arc.l2tools.io.jmpl.JmplLoader <filename>");
		System.out.println("\nExample:");
		System.out.println("java gov.nasa.arc.l2tools.io.jmpl.JmplLoader \"c:\\directory\\filename.jmpl\"");
	}	else {
		System.out.println("creating JmplLoader for: "+args[0]);
		JmplLoader jmplLoader = new JmplLoader(new gov.nasa.arc.l2tools.Workspace(),new java.io.File(args[0]));
		System.out.print("JmplLoader loading...");
		jmplLoader.load();
		System.out.println("finished.");
	}
}
}

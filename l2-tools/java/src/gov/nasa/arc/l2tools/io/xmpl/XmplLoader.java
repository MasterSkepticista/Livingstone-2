//
// * See the file "l2-tools/disclaimers-and-notices.txt" for 
// * information on usage and redistribution of this file, 
// * and for a DISCLAIMER OF ALL WARRANTIES.
//
package gov.nasa.arc.l2tools.io.xmpl;

/**
 * Insert the type's description here.
 * Creation date: (3/20/2000 12:44:28 PM)
 * @author: Andrew Bachmann
 */
public class XmplLoader extends gov.nasa.arc.l2tools.io.Loader {
/**
 * XmplLoader constructor comment.
 */
public XmplLoader() {
	super();
}
/**
 * XmplLoader constructor comment.
 * @param workspace gov.nasa.arc.l2tools.Workspace
 */
public XmplLoader(gov.nasa.arc.l2tools.Workspace workspace) {
	super(workspace);
}
/**
 * XmplLoader constructor comment.
 * @param workspace gov.nasa.arc.l2tools.Workspace
 * @param reader gov.nasa.arc.l2tools.io.Reader
 */
public XmplLoader(gov.nasa.arc.l2tools.Workspace workspace, gov.nasa.arc.l2tools.io.Reader reader) {
	super(workspace, reader);
}
/**
 * XmplLoader constructor comment.
 * @param workspace gov.nasa.arc.l2tools.Workspace
 * @param file java.io.File
 * @exception java.io.FileNotFoundException The exception description.
 */
public XmplLoader(gov.nasa.arc.l2tools.Workspace workspace, java.io.File file) throws java.io.FileNotFoundException {
	super(workspace, file);
}
/**
 * XmplLoader constructor comment.
 * @param workspace gov.nasa.arc.l2tools.Workspace
 * @param fd java.io.FileDescriptor
 */
public XmplLoader(gov.nasa.arc.l2tools.Workspace workspace, java.io.FileDescriptor fd) {
	super(workspace, fd);
}
/**
 * XmplLoader constructor comment.
 * @param workspace gov.nasa.arc.l2tools.Workspace
 * @param in java.io.InputStream
 */
public XmplLoader(gov.nasa.arc.l2tools.Workspace workspace, java.io.InputStream in) {
	super(workspace, in);
}
/**
 * XmplLoader constructor comment.
 * @param workspace gov.nasa.arc.l2tools.Workspace
 * @param in java.io.InputStream
 * @param enc java.lang.String
 * @exception java.io.UnsupportedEncodingException The exception description.
 */
public XmplLoader(gov.nasa.arc.l2tools.Workspace workspace, java.io.InputStream in, String enc) throws java.io.UnsupportedEncodingException {
	super(workspace, in, enc);
}
/**
 * XmplLoader constructor comment.
 * @param workspace gov.nasa.arc.l2tools.Workspace
 * @param reader java.io.Reader
 */
public XmplLoader(gov.nasa.arc.l2tools.Workspace workspace, java.io.Reader reader) {
	super(workspace, reader);
}
/**
 * XmplLoader constructor comment.
 * @param workspace gov.nasa.arc.l2tools.Workspace
 * @param url java.net.URL
 * @exception java.io.IOException The exception description.
 * @exception java.net.MalformedURLException The exception description.
 */
public XmplLoader(gov.nasa.arc.l2tools.Workspace workspace, java.net.URL url) throws java.io.IOException, java.net.MalformedURLException {
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
	gov.nasa.arc.l2tools.io.xml.XMLElementDeserializer deserializer
  	= new gov.nasa.arc.l2tools.io.xml.XMLElementDeserializer(getReader(),new XmplElementFactoryImp());
	fr.dyade.koala.xml.domlight.XMLElement xmlElement = deserializer.readXMLElement();
	if (xmlElement instanceof gov.nasa.arc.l2tools.DataObject.XmlElement) {
		gov.nasa.arc.l2tools.DataObject container = ((gov.nasa.arc.l2tools.DataObject.XmlElement)xmlElement).getObject();
		if (container == null) {
			throw new gov.nasa.arc.l2tools.Exception("XMLElement without skunkworks object"+xmlElement.toString());
		}
		int size = container.getChildren().size();
		if (!(container instanceof XmplContainer)) {
			throw new java.io.IOException("xmpl container not found");
		}
		java.util.Enumeration objects = container.getChildren().elements();
		java.lang.Object object = null;
		while (objects.hasMoreElements()) {
			object = objects.nextElement();
			if (object instanceof gov.nasa.arc.l2tools.Project) {
				getWorkspace().getChildren().addElement(object);
			} else if (object instanceof gov.nasa.arc.l2tools.DataObject) {
				if (defaultProject == null) {
					defaultProject = new gov.nasa.arc.l2tools.Project();
					getWorkspace().getChildren().addElement(defaultProject);
				}
				defaultProject.getChildren().addElement(object);
			} else if (!(object instanceof gov.nasa.arc.l2tools.Content)) {
				throw new java.io.IOException("non-skunkworks object found: "+object.toString());
			}
		}
		if (size == 0) {
			return null;
		} else if (size == 1) {
			return (gov.nasa.arc.l2tools.DataObject)object;
		} else {
			return getWorkspace();
		}
	}
	throw new java.io.IOException("skunkworks xmlElement not found");
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
		System.out.println("java gov.nasa.arc.l2tools.io.xmpl.XmplLoader <filename>");
		System.out.println("\nExample:");
		System.out.println("java gov.nasa.arc.l2tools.io.xmpl.XmplLoader \"c:\\directory\\filename.xmpl\"");
	}	else {
		System.out.println("creating XmplLoader for: "+args[0]);
		XmplLoader xmplLoader = new XmplLoader(new gov.nasa.arc.l2tools.Workspace(),new java.io.File(args[0]));
		System.out.print("XmplLoader loading...");
		xmplLoader.load();
		System.out.println("finished.");
	}
}
}

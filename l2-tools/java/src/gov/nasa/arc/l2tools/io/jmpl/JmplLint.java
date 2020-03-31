//
// * See the file "l2-tools/disclaimers-and-notices.txt" for 
// * information on usage and redistribution of this file, 
// * and for a DISCLAIMER OF ALL WARRANTIES.
//
package gov.nasa.arc.l2tools.io.jmpl;

import fr.dyade.koala.xml.domlight.XMLElementSerializer;
/**
 * Insert the type's description here.
 * Creation date: (1/13/2000 2:07:04 PM)
 * @author: Andrew Bachmann
 */
class JmplLint {
/**
 * JavaTest constructor comment.
 */
public JmplLint() {
	super();
}
/**
 * Starts the application.
 * @param args an array of command-line arguments
 */
public static void main(java.lang.String[] args) throws Exception {
  try {
		if ( args.length==0 ) {
			System.out.println("Supply a filename:");
			System.out.println("java gov.nasa.arc.l2tools.io.jmpl.JmplLint <filename>");
			System.out.println("\nExample:");
			System.out.println("java gov.nasa.arc.l2tools.io.jmpl.JmplLint \"c:\\directory\\filename.jmpl\"");
		} else {
			System.out.println("JmplLint loading: "+args[0]);
			gov.nasa.arc.l2tools.Workspace workspace=new gov.nasa.arc.l2tools.Workspace();
			JmplLoader loader=new JmplLoader(workspace,new java.io.File(args[0]));
			gov.nasa.arc.l2tools.Project project=new gov.nasa.arc.l2tools.Project();
			loader.load(project);
			// start of code snagged from Dom Light Demo
			System.out.println("--- serialize using System.out ---");
			XMLElementSerializer ser = new XMLElementSerializer(System.out);
			ser.writeXMLElement(project.getXmlElement());
			System.out.println("\n");
//			System.out.println("--- print node hierarchy ---");
//			fr.dyade.koala.xml.domlight.Util.print(project.getXmlElement());
//			System.out.println("\n");
			ser.flush();
			// end of code snagged from Dom Light Demo
//			com.ibm.uvm.tools.DebugSupport.inspect(result);
//			System.in.read();
		}
	}
	catch (Exception e) {
		System.err.println(System.getProperty("line.separator")+
			System.getProperty("line.separator"));
		System.err.println("internal error: "+e.toString());
		e.printStackTrace();
	}
	System.out.println("JmplLint exiting.");
	System.exit(0);	
}
}

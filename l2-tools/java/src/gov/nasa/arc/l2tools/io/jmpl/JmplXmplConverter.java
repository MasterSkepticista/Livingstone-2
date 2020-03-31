//
// * See the file "l2-tools/disclaimers-and-notices.txt" for 
// * information on usage and redistribution of this file, 
// * and for a DISCLAIMER OF ALL WARRANTIES.
//
package gov.nasa.arc.l2tools.io.jmpl;

/**
 * Insert the type's description here.
 * Creation date: (3/21/2000 12:41:17 PM)
 * @author: Andrew Bachmann
 */
public class JmplXmplConverter {
/**
 * JmplXmplConverter constructor comment.
 */
public JmplXmplConverter() {
	super();
}
/**
 * Starts the application.
 * @param args an array of command-line arguments
 */
public static void main(java.lang.String[] args) throws Exception {
  try {
		if ( args.length==0 ) {
			System.out.println("Supply at least one filename:");
			System.out.println("java gov.nasa.arc.l2tools.io.jmpl.JmplXmplConverter <filename> ...");
			System.out.println("\nExample:");
			System.out.println("java gov.nasa.arc.l2tools.io.jmpl.JmplXmplConverter \"c:\\directory\\filename.jmpl\"");
		} else {
			gov.nasa.arc.l2tools.Workspace workspace=new gov.nasa.arc.l2tools.Workspace();

			// LOAD
			gov.nasa.arc.l2tools.Project project=new gov.nasa.arc.l2tools.Project();
			project.set("name",args[0]);
			System.out.println("Loading:");
			for (int i=0;(i<args.length);i++) {
				java.io.File file=new java.io.File(args[i]);
				if (!file.canRead()) {
					throw new java.io.FileNotFoundException("unable to read file: "+args[i]);
				}
				System.out.println("  "+args[i]);
				JmplLoader loader=new JmplLoader(workspace,file);
				loader.load(project);
			}
			System.out.println();

			// SAVE
			System.out.println("JmplXmplConverter saving: "+args[0]+".xmpl");
			gov.nasa.arc.l2tools.io.xmpl.XmplSaver saver
			  = new gov.nasa.arc.l2tools.io.xmpl.XmplSaver(new java.io.File(args[0]+".xmpl"));
			saver.save(project);
		}
	}
	catch (Exception e) {
		System.err.println(System.getProperty("line.separator")+
			System.getProperty("line.separator"));
		System.err.println("internal error: "+e.toString());
		e.printStackTrace();
	}
	System.out.println("JmplXmplConverter exiting.");
	System.exit(0);	
}
}

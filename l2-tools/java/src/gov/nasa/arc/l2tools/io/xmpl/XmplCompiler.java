//
// * See the file "l2-tools/disclaimers-and-notices.txt" for 
// * information on usage and redistribution of this file, 
// * and for a DISCLAIMER OF ALL WARRANTIES.
//
package gov.nasa.arc.l2tools.io.xmpl;

/**
 * Insert the type's description here.
 * Creation date: (1/13/2000 2:07:04 PM)
 * @author: Andrew Bachmann
 */
class XmplCompiler {
/**
 * JavaTest constructor comment.
 */
  public XmplCompiler() {
	super();
}
/**
 * Starts the application.
 * @param args an array of command-line arguments
 */
public static void main(java.lang.String[] args) throws Exception {
  try {
		System.out.println("XmplCompiler version 2.0");
		if ( args.length==0 ) {
			System.out.println("Supply a component type, component name, and at least one file name:");
			System.out.println("java gov.nasa.arc.l2tools.io.xmpl.XmplCompiler <component type> <component name> <filename1> <filename2> ... ");
			System.out.println("\nExample:");
			System.out.println("java gov.nasa.arc.l2tools.io.xmpl.XmplCompiler sensor sensor1 \"c:\\directory\\filename.xmpl\"");
		} else if ( args.length==1 ) {
			System.out.println("Supply a component name, and at least one file name:");
			System.out.println("java gov.nasa.arc.l2tools.io.xmpl.XmplCompiler <component type> <component name> <filename1> <filename2> ... ");
			System.out.println("\nExample:");
			System.out.println("java gov.nasa.arc.l2tools.io.xmpl.XmplCompiler sensor sensor1 \"c:\\directory\\filename.xmpl\"");
		} else if ( args.length==2 ) {
			System.out.println("Supply at least one file name:");
			System.out.println("java gov.nasa.arc.l2tools.io.xmpl.XmplCompiler <component type> <component name> <filename1> <filename2> ... ");
			System.out.println("\nExample:");
			System.out.println("java gov.nasa.arc.l2tools.io.xmpl.XmplCompiler sensor sensor1 \"c:\\directory\\filename.xmpl\"");
		} else {
			// LOAD
			gov.nasa.arc.l2tools.Workspace workspace=new gov.nasa.arc.l2tools.Workspace();
			gov.nasa.arc.l2tools.Project project=new gov.nasa.arc.l2tools.Project();
			project.set("name",args[0]);
			System.out.println("Loading:");
			for (int i=2;(i<args.length);i++) {
				java.io.File file=new java.io.File(args[i]);
				if (!file.canRead()) {
					throw new java.io.FileNotFoundException("unable to read file: "+args[i]);
				}
				System.out.println("  "+args[i]);
				XmplLoader loader=new XmplLoader(workspace,file);
				loader.load(project);
			}
			System.out.println();

			//// FIND THE COMPONENT TYPE
			String componentTypeName=args[0];
			System.out.println("Finding the component type: "+componentTypeName);
			java.util.Enumeration children=project.getChildren().elements();
			gov.nasa.arc.l2tools.livingstone.ComponentType componentType = null;
			while (children.hasMoreElements()) {
				java.lang.Object child=children.nextElement();
				if (child instanceof gov.nasa.arc.l2tools.livingstone.ComponentType) {
					if (((gov.nasa.arc.l2tools.livingstone.ComponentType)child).get("name").equals(componentTypeName)) {
						componentType = (gov.nasa.arc.l2tools.livingstone.ComponentType)child;
						break;
					}
				} else if (child instanceof gov.nasa.arc.l2tools.livingstone.AttributeType) {
					if (((gov.nasa.arc.l2tools.livingstone.AttributeType)child).get("name").equals(componentTypeName)) {
						String errorString="The name '"+componentTypeName+"' corresponds to an attribute type, not component type.";
						throw new gov.nasa.arc.l2tools.Exception(errorString);
					}
				}
			}
			if (componentType==null) {
				String errorString2="The name '"+componentTypeName+"' is not defined as a type in the files supplied";
				throw new gov.nasa.arc.l2tools.Exception(errorString2);
			}

			//// COMPILE
      String resultName = args[1];
      int separatorIndex = args[1].lastIndexOf(System.getProperty("file.separator"));
      if (separatorIndex != -1) resultName = resultName.substring(separatorIndex+1);
      System.out.println("XmplCompiler beginning compile of component to be named: "+resultName);
      gov.nasa.arc.l2tools.livingstone.ComponentInstance result=componentType.buildInstance(resultName);
			System.out.println("");
			System.out.println("XmplCompiler finished compile.");

			//// SAVE
			System.out.println("XmplCompiler writing output file: "+args[1]+".xmpl");
			gov.nasa.arc.l2tools.io.xmpl.XmplSaver saver 
  			= new gov.nasa.arc.l2tools.io.xmpl.XmplSaver(new java.io.File(args[1]+".xmpl"));
  		saver.save(result);
		}
	}	catch (gov.nasa.arc.l2tools.livingstone.InputException e) {
		System.err.println(System.getProperty("line.separator")+System.getProperty("line.separator"));
		System.err.println(e);
	} catch (Exception e) {
		System.err.println(System.getProperty("line.separator")+System.getProperty("line.separator"));
		e.printStackTrace();
	} 
	System.out.println("XmplCompiler exiting.");
	System.exit(0);	
}
}

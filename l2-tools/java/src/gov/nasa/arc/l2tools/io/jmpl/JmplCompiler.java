//
// * See the file "l2-tools/disclaimers-and-notices.txt" for 
// * information on usage and redistribution of this file, 
// * and for a DISCLAIMER OF ALL WARRANTIES.
//
package gov.nasa.arc.l2tools.io.jmpl;

/**
 * Insert the type's description here.
 * Creation date: (1/13/2000 2:07:04 PM)
 * @author: Andrew Bachmann
 */
class JmplCompiler {
    /**
     * JavaTest constructor comment.
     */
    public JmplCompiler() {
        super();
    }
    
    private static void exit() {
        System.out.println("JmplCompiler exiting.");
        System.exit(0);
    }
    
    private static void printUsage() {
        System.out.println("Follow one of these two patterns:");
        System.out.println("java gov.nasa.arc.l2tools.io.jmpl.JmplCompiler <filename with arguments in it>");
        System.out.println("\njava gov.nasa.arc.l2tools.io.jmpl.JmplCompiler [options] <component type> <component name> <filename1> <filename2> ... ");
        System.out.println("    -h, --help      Print this usage information.");
        System.out.println("    -o, --optimize  Perform tree-shaking optimization.");
        System.out.println("\nExamples:");
        System.out.println("java gov.nasa.arc.l2tools.io.jmpl.JmplCompiler \"c:\\directory\\argsfile.txt\"");
        System.out.println("java gov.nasa.arc.l2tools.io.jmpl.JmplCompiler -optimize sensor sensor1 \"c:\\directory\\filename.jmpl\"");
    }
    
    private static java.util.Vector getArgs(java.lang.String[] args) {
        java.util.Vector argVector = new java.util.Vector();
        if (args.length != 1) {
            for (int i = 0 ; (i < args.length) ; i++) {
                argVector.add(args[i]);
            }
            return argVector;
        }
        try {
            java.io.File file = new java.io.File(args[0]);
            java.io.FileReader fis = new java.io.FileReader(file);
            java.io.BufferedReader buf = new java.io.BufferedReader(fis);
            while (true) {
                java.lang.String line = buf.readLine();
                if (line == null) {
                    break;
                } 
                argVector.add(line.trim());
            }
        } catch (Exception e) {
            System.out.println(e);
            printUsage();
            exit();
        }
        return argVector;
    }

    public static void load(gov.nasa.arc.l2tools.Workspace workspace, gov.nasa.arc.l2tools.Project project, java.util.Enumeration filenames) throws Exception {
        System.out.println("Loading:");
        while (filenames.hasMoreElements()) {
            String filename = (String) filenames.nextElement();
            java.io.File file=new java.io.File(filename);
            if (!file.canRead()) {
                throw new java.io.FileNotFoundException("unable to read file: "+filename);
            }
            System.out.println("  "+filename);
            JmplLoader loader=new JmplLoader(workspace,file);
            loader.load(project);
        }
        System.out.println();
    }
    
    /**
     * Starts the application.
     * @param args an array of command-line arguments
     */
    public static void main(java.lang.String[] args) throws Exception {
        try {
            System.out.println("JmplCompiler version 2.2");
            // check for single arg, which is the file name of a file
            // containing the args, one per line
            java.util.Vector arguments = getArgs(args);
            // now handle the args
            if (arguments.isEmpty()) {
                printUsage();
                exit();
            }
            java.util.Enumeration enum = arguments.elements();
            boolean optimize = false;
            // check for flags
            String argument;
            do {
                argument = (String) enum.nextElement();
                if (argument.length() == 0) continue;
                if (argument.charAt(0)!='-') break;
                if (argument.compareTo("-h") == 0) {
                    printUsage();
                    exit();
                }
                if (argument.compareTo("-o") == 0) {
                    optimize = true;
                    continue;
                }
                if (argument.compareTo("--help") == 0) {
                    printUsage();
                    exit();
                }
                if (argument.compareTo("--optimize") == 0) {
                    optimize = true;
                    continue;
                }
                System.out.println("Unknown flag: "+argument);
                printUsage();
                exit();
            } while (enum.hasMoreElements());
            if (!enum.hasMoreElements()) {
                System.out.println("Missing component name, and filenames.");
                printUsage();
                exit();
            }
            String componentTypeName = argument;
            String componentName = (String)enum.nextElement();
            if (!enum.hasMoreElements()) {
                System.out.println("Missing filenames.");
                printUsage();
                exit();
            }
            
            gov.nasa.arc.l2tools.Workspace workspace=new gov.nasa.arc.l2tools.Workspace();
            gov.nasa.arc.l2tools.Project project=new gov.nasa.arc.l2tools.Project();
            project.set("name",componentTypeName);
            
            // LOAD
            load(workspace,project,enum);

            // FIND THE COMPONENT TYPE
            gov.nasa.arc.l2tools.livingstone.ComponentType componentType;
            System.out.println("Finding the component type: "+componentTypeName);
            componentType = (gov.nasa.arc.l2tools.livingstone.ComponentType)
                project.find(gov.nasa.arc.l2tools.livingstone.ComponentType.class,"name",componentTypeName);
            if (componentType==null) {
                String errorString="The name '"+componentTypeName+"' is not defined as a component type in the files supplied";
                throw new gov.nasa.arc.l2tools.Exception(errorString);
            }
                
            //// COMPILE
            String resultName = componentName;
            int separatorIndex = componentName.lastIndexOf(System.getProperty("file.separator"));
            if (separatorIndex != -1) resultName = resultName.substring(separatorIndex+1);
            System.out.println("JmplCompiler beginning compile of component to be named: "+resultName);
            gov.nasa.arc.l2tools.livingstone.ComponentInstance result;
            result = componentType.buildInstance(resultName,optimize);
            System.out.println("");
            System.out.println("JmplCompiler finished compile.");
                
            //// SAVE
            System.out.println("JmplCompiler writing output file: "+componentName+".xmpl");
            gov.nasa.arc.l2tools.io.xmpl.XmplSaver saver
            = new gov.nasa.arc.l2tools.io.xmpl.XmplSaver(new java.io.File(componentName+".xmpl"));
            saver.save(result);
        } catch (gov.nasa.arc.l2tools.livingstone.InputException e) {
            System.err.println(System.getProperty("line.separator")+System.getProperty("line.separator"));
            System.err.println(e);
        } catch (Exception e) {
            System.err.println(System.getProperty("line.separator")+System.getProperty("line.separator"));
            e.printStackTrace();
        }
        System.out.println("JmplCompiler exiting.");
        System.exit(0);    }
}

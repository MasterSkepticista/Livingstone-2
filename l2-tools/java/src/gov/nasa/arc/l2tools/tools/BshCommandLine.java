//
// * See the file "l2-tools/disclaimers-and-notices.txt" for 
// * information on usage and redistribution of this file, 
// * and for a DISCLAIMER OF ALL WARRANTIES.
//
/*
 * Author : Mark Shirley
 * Description : BshCommandLine.java
 *
 * CHANGELOG:
 * 05/05/00      Mhs     Created
 */

package gov.nasa.arc.l2tools.tools;

import bsh.Interpreter;
import bsh.NameSpace;
import java.io.*;

/** A combined BeanShell interpreter and Livingstone commandline. */
public class BshCommandLine {

    public Interpreter interpreter;

    public boolean batch = false;
    public String sourcedFilename = null;

    BshCommandLineInputStream inputStream = null;
    
    public void run() {
        InputStream in1 = System.in;
        inputStream = new BshCommandLineInputStream(in1);
        interpreter = new Interpreter(inputStream,System.out,System.err,true,getNameSpace());
        interpreter.run();
    }

    public void source(String filename) throws Exception {
        interpreter = new Interpreter();
        File file = interpreter.pathToFile( filename );
        interpreter.debug("Sourcing file: "+file);
        inputStream = new BshCommandLineInputStream( new FileInputStream(file) );
        sourcedFilename = filename;
        batch = true;
        interpreter.eval( inputStream, getNameSpace(), filename );
    }

    public static void main(String[] args) {
        try {
            if (args.length>0) {
                new BshCommandLine().source(args[0]);
            } else {
                new BshCommandLine().run();
            }
        } catch (Exception e) { e.printStackTrace(); }
        System.exit(0);
    }

    public NameSpace getNameSpace() {
        NameSpace ns = new NameSpace("global");
        try {
            System.out.println("Setting commandline="+this);
            ns.setVariable( "cmd", this);
        } catch (Exception e) { e.printStackTrace(); }
        System.out.println("in BshCommandLine>>getNameSpace ns="+ns);
        return ns;
    }

    public void command(String command) {
        System.out.println("Got a command: "+command);
        //processCommandLine(command);
    }

    public void setEcho(boolean b) {
        System.out.println("inputStream="+inputStream);
        if (inputStream!=null) inputStream.echo = b;
    }

    class EchoStream extends FilterInputStream {
        public EchoStream(InputStream in) {
            super(in);
        }
        public int read() throws IOException {
            int c = in.read();
            System.out.print((char)c);
            return c;
        }
    }

}

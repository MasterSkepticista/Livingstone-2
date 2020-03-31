//
// * See the file "l2-tools/disclaimers-and-notices.txt" for 
// * information on usage and redistribution of this file, 
// * and for a DISCLAIMER OF ALL WARRANTIES.
//
/*
 * Author : Mark Shirley
 * Description : CommandLineInputStream.java
 *
 * CHANGELOG:
 * 05/05/00      Mhs     Created
 *               Mhs     I'm having difficulty getting it to work under a dosshell and emacs.
                         Under the dosshell, I get \r\n at the end of lines.  Under emacs,
                         I only get \n (like unix).
 */

package gov.nasa.arc.l2tools.tools;

import java.io.*;

/** Reads the input and expands lines starting with : into
    commandline.command(<rest-of-line>); */
class BshCommandLineInputStream extends FilterInputStream {

    boolean echo = false;

    public BshCommandLineInputStream(InputStream in) {
        super(in);
    }

    int[] buf = new int[256];
    int bufsize=256, ptr = 0, fillptr = 0;
        
    static final int normal = 0, lastCharNL = 1, bufEmptying = 2;
    int state = lastCharNL;

    public int read() throws IOException {
        int c;

        //System.out.println("state="+state);

        if ( state == bufEmptying ) {
            c = buf[ptr++];
            if (ptr>=fillptr) state = lastCharNL;
            return echo(c);
        }

        c = in.read();

        // Throw out the returns and only attend to the linefeeds.  (I'm
        // not sure about the parser).
        if (c==13) c = in.read();

        if (c==10) {            // the linefeed
            state=lastCharNL;
            return echo(c);
        }

        if (state==lastCharNL) {
            if (c!=58) {        // ':'
                state=normal;
                return echo(c);
            }
                
            // We've seen <cr>:
            ptr = fillptr = 0;

            while((c=in.read()) == 32);
            if (c==10) {        // handle the case of ":\r"
                pushbuf("null();\r");
                state = bufEmptying;
                return echo(buf[ptr++]);
            }
            pushbuf(c);
            while((c=in.read())!=32 && c!=10) pushbuf(c);
            if (c==10) {
                pushbuf("(\"\");\r");
                state = bufEmptying;
                return echo(buf[ptr++]);
            }
            pushbuf("(\"");
            while ((c = in.read()) != 10) { // '\n'=10
                if (c!=13) {    // ignore '\r'
                    if (c==34) pushbuf(92); // '"'=34, '\\'=92
                    if (c==92) pushbuf(92);
                    pushbuf(c);
                }
            }
            pushbuf("\");\r");
            state = bufEmptying;

            return echo(buf[ptr++]);
        }

        state=normal;       // redundant
        return echo(c);
    }

    // degenerate implementation (from bsh)
    public int read(byte buff[], int off, int len) throws IOException {
        int b = read();
        if ( b == -1 )
            return 0;
        else {
            buff[off]=(byte)b;
            return 1;
        }
    }

    void pushbuf(String s) {
        int max = s.length();
        for (int i=0;i<max;i++)
            pushbuf((int)s.charAt(i));
    }

    void pushbuf(int c) {
        if (fillptr>=bufsize) {
            int[] buf1 = new int[bufsize+64];
            for (int i=0;i<fillptr;i++)
                buf1[i] = buf[i];
            buf = buf1;
            bufsize = bufsize + 64;
        }
        buf[fillptr++] = c;
    }

	// Test it
	public static void main( String [] args ) throws Exception {
		InputStream in = new BshCommandLineInputStream( System.in );
        //System.out.println("\\r="+(int)'\r');
        //System.out.println("\\n="+(int)'\n');
		while ( true ) {
            int c = in.read();
			//System.out.println( (char)c+ "="+c);
            //System.out.print( (char)c );
        }
	}

    protected int echo(int c) {
        if (echo) System.out.write(c);
        //System.out.print((char)c);
        return c;
    }

}

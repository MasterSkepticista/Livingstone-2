//
// * See the file "l2-tools/disclaimers-and-notices.txt" for 
// * information on usage and redistribution of this file, 
// * and for a DISCLAIMER OF ALL WARRANTIES.
//
/*
 * Author : Mark Shirley
 * Description : NullWriter.java
 *
 * CHANGELOG:
 * 11/29/99      Mhs     Created
 */

package gov.nasa.arc.l2tools.util;

import java.io.Writer;

public class NullWriter extends Writer {
    public void close() { }
    public void flush() { }
    public void write(String str) { }
    public void write(String str, int off, int len) { }
    public void write(char[] cbuf) { }
    public void write(char[] cbuf, int off, int len) { }
    public void write(int c) { }
}

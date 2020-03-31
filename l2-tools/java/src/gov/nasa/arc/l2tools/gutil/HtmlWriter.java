//
// * See the file "l2-tools/disclaimers-and-notices.txt" for 
// * information on usage and redistribution of this file, 
// * and for a DISCLAIMER OF ALL WARRANTIES.
//
/*
 * Author : Mark Shirley
 * Description : HtmlWriter.java
 *
 * CHANGELOG:
 * 11/27/99      Mhs     Created
 */

package gov.nasa.arc.l2tools.gutil;

import java.io.*;
import java.util.Hashtable;

public class HtmlWriter extends PrintWriter {

    /** The actual stream (a Writer) that this puts characters on */
    //protected Writer stream;

    /** This hashtable holds onto any presentations that have been displayed
        as links in the html. */
    // This isn't protected because HtmlObjectBrowser needs access for efficiency
    Hashtable presentations = new Hashtable();

    /** Create an HtmlWriter which puts characters onto stream */
    public HtmlWriter(Writer stream) {
        super(stream);
    //    this.stream = stream;
    }

    //
    // Implementing the Writer methods
    //

    /** Close the stream.  This lets go of the stream now because this
        HtmlStream might be kept around for a while. */
    //public void close() { super().close(); stream = null; }

    // Changed to subclass PrintWriter
    //public void flush() throws IOException { stream.flush(); }
    //public void write(String str) throws IOException { stream.write(str); }
    //public void write(String str, int off, int len) throws IOException { stream.write(str,off,len); }
    //public void write(char[] cbuf) throws IOException { stream.write(cbuf); }
    //public void write(char[] cbuf, int off, int len) throws IOException { stream.write(cbuf,off,len); }
    //public void write(int c) throws IOException { stream.write(c); }

    //
    // Extensions
    //

    /** Remove all stored presentations */
    public void clear() {
        presentations.clear();
    }

    /** Write the linkText as an html link and associate the presentation with it. */
    public void presentAsLink(Presentation presentation, String linkText) throws IOException {
        String key = "presentation:"+presentations.size();
        presentations.put(key,presentation);
        write("<a href=\"");
        write(key);
        write("\">");
        write(linkText);
        write("</a>");
    }

    public Presentation lookupPresentation(String label) {
        return (Presentation)presentations.get(label);
    }

}

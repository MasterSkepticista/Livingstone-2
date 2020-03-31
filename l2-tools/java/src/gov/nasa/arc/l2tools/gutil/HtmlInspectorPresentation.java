//
// * See the file "l2-tools/disclaimers-and-notices.txt" for 
// * information on usage and redistribution of this file, 
// * and for a DISCLAIMER OF ALL WARRANTIES.
//
/*
* Author : Mark Shirley
* Description : HtmlInspectorPresentation.java
*
* CHANGELOG:
* 11/29/99      Mhs     Created
*/

package gov.nasa.arc.l2tools.gutil;

import gov.nasa.arc.l2tools.util.EventLog;
import java.lang.reflect.*;

/** This presents objects as a table of slots and values.  Currently, it can
    only access public slots.  That'll change when we move to 1.2. */
public class HtmlInspectorPresentation extends DefaultHtmlPresentation {

    public HtmlInspectorPresentation(Object object) {
        super(object);
    }

    /** This is intended to be subclassed by different kinds of html viewers. */
    protected Class getPresentationContext() { 
        return HtmlInspector.class; 
    }

    /** Write a representation of this object to the stream. */
    public void write(HtmlWriter writer) throws java.io.IOException {
        write(writer,this);
    }
        
    /** This is available statically so that presentation can use this internally. */
    public static void write(HtmlWriter writer, Object object) throws java.io.IOException {
        if (object==null) {
            writer.write("null");
            return;
        }
        // Why doesn't the align attribute inherit from the <table> or <tr> tags?
        writer.write("<table border=\"1\"><caption>Inspector for "+object.toString()+"</caption>");
        writer.write("<tr><em><th align=\"right\">Field Name<th align=\"left\">Value</em></tr>");

        writeInstanceVariables(writer,object,object.getClass());

        writer.write("</table>");
    }

    protected static void writeInstanceVariables(HtmlWriter writer, Object object, Class c) 
         throws java.io.IOException 
    {
        if (c==null) return;
        Object v = null;
        Field[] fields = c.getDeclaredFields();
        for (int i=0; i<fields.length; i++) {
            writer.write("<tr><th align=\"right\">"+fields[i].getName());
            try {
                boolean temp = fields[i].isAccessible();
                fields[i].setAccessible(fields,true);
                v = fields[i].get(object);
                if (!temp) fields[i].setAccessible(fields,false);
            } catch (IllegalAccessException e) {
                v = "<em>Illegal access error</em>";
            }
            writer.write("<th align=\"left\">"+v+"</tr>");
        }
        writeInstanceVariables(writer,object,c.getSuperclass());
    }
}

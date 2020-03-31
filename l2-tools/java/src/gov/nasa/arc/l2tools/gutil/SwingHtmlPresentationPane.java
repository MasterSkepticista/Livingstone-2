//
// * See the file "l2-tools/disclaimers-and-notices.txt" for 
// * information on usage and redistribution of this file, 
// * and for a DISCLAIMER OF ALL WARRANTIES.
//
/*
* Author : Mark Shirley
* Description : SwingHtmlPresentationPane.java
*
* CHANGELOG:
* 12/03/99      Mhs     Created
*/

package gov.nasa.arc.l2tools.gutil;

import gov.nasa.arc.l2tools.util.NullWriter;
import gov.nasa.arc.l2tools.util.EventLog;
import gov.nasa.arc.l2tools.gutil.InputModeIdle;
import gov.nasa.arc.l2tools.util.Vector;

import java.awt.*;
import java.awt.event.MouseEvent;
import java.io.IOException;
import java.io.PrintWriter;
import java.io.StringWriter;
import java.util.Hashtable;
import javax.swing.*;
import javax.swing.text.*;

/** This is a generic, HTML-based view for any object.
    This version is implemented using the Swing HTML support.
    The interface needs to be changed for that, but I'm leaving
    it the same for early testing. - Shirley
    */
public class SwingHtmlPresentationPane extends JPanel {

    /** A stream that knows about presentations */
    HtmlWriter htmlWriter;
    
    /** The currently displayed object. */
    Object currentObject = null;

    JEditorPane pane;

    JScrollPane scrollPane;

    public SwingHtmlPresentationPane() {
        super();
        pane = new JEditorPane();
        pane.setEditable(false);
        pane.setContentType("text/html");
        setLayout(new BorderLayout(0,0));
        scrollPane = new JScrollPane(pane);
        add(scrollPane ,"Center");
    }

    public JScrollPane getScrollPane() { return scrollPane; }
    public JEditorPane getEditorPane() { return pane; }

    ///
    /// Interface for editing from outside
    ///

    HtmlWriter getHtmlWriter() {
        if (htmlWriter!=null) return htmlWriter;
        return htmlWriter = new HtmlWriter(new NullWriter());
    }

    protected class GeneratedHtml{
        StringBuffer buf = new StringBuffer();
        Hashtable presentations = new Hashtable();
    }

    protected boolean isDisplayingObject() {
        if (currentObject instanceof GeneratedHtml) return false;
        return true;
    }

    protected GeneratedHtml getGeneratedHtml() {
        if (isDisplayingObject()) return null;
        return (GeneratedHtml)currentObject;
    }

    public void setText(String str) {
        pane.setText(str);
    }

    /** Clear the window. */
    public void htmlClear() {
        pane.setText("");
        getHtmlWriter().clear();
        currentObject = new GeneratedHtml();
    }

    public void htmlAppend(java.lang.String str) {
        if (isDisplayingObject()) htmlClear();
        pane.setText(str);
        getGeneratedHtml().buf.append(str);
    }

    /** Append text using a Reader.  I'm currently using the string version to
     * avoid multithreading code that just needs a static display.  */
    //public void htmlAppend(java.io.Reader reader, java.lang.String mimeType) {
    //    super.htmlAppend(reader, mimeType);
    //}

    /** I don't think this is needed for the Swing version. */
    public void htmlWait(boolean all) { }

    public void presentAsLink(Object object, String linkText) throws IOException {
        presentAsLink(PresentationUtils.getPresentation(object,SwingHtmlPresentationPane.class),
                      linkText);
    }

    /*
    public void presentAsLink(Presentation presentation, String linkText) throws IOException {
        //super.htmlWait(false);
        if (isDisplayingObject()) htmlClear();
        String key = "presentation:"+getHtmlWriter().presentations.size();
        getHtmlWriter().presentations.put(key,presentation);
        String str = "<a href=\""+key+"\">"+linkText+"</a>";
        super.htmlAppend(str);
        getGeneratedHtml().buf.append(str);
        getGeneratedHtml().presentations.put(key,presentation);
    }
    */

    /** Call this to ask this component to present an object as html. */
    public void present(Object o) {
        presentInternal(o);
    }

    /** This is only supposed to be used by this class or by windows that are
        using this as a subcomponent.  Enforcing this while allowing other
        packages to use this is a problem right now. */
    boolean presentInternal(Object o) { 
        try { 
            if (o instanceof GeneratedHtml) {
                GeneratedHtml gh = (GeneratedHtml)o; 
                htmlClear();
                htmlAppend(gh.buf.toString()); 
                htmlWriter.presentations = gh.presentations;
            } else {
                htmlClear(); 
                HtmlPresentation p = (HtmlPresentation)PresentationUtils.getPresentation(o,getPresentationContext());
                StringWriter stringWriter = new StringWriter(); 
                htmlWriter = new HtmlWriter(stringWriter); 
                p.write(htmlWriter); 
                htmlAppend(stringWriter.toString());
                return true;
            } 
        } catch(Exception e) { // Put the error message into the html window EventLog.report(e);
            try {
                StringWriter stringWriter = new StringWriter();
                PrintWriter writer = new PrintWriter(stringWriter);
                e.printStackTrace(writer);
                htmlAppend("<plaintext>"+stringWriter.toString());
            } catch (Exception e1) {
                EventLog.report(e1);
            } 
        } 
        return false;
    }

    /** This is intended to be subclassed by different kinds of html viewers. */
    protected Class getPresentationContext() { 
        return HtmlPresentationPane.class; 
    }

    public Object getCurrentObject() {
        return currentObject;
    }

    ///
    /// Menu handling
    ///

    ///
    /// Browser default menu
    ///

    protected void exposeDefaultPopupMenu(int x, int y) {
        PopupMenu.popup("HTML Object Viewer",
                        new Vector(menuItemCopy()),
                        this,this,x,y);
    }

    protected PopupMenuItem menuItemCopy() {
        return new PopupMenuItem("Copy",
                                 new PopupMenuCallback() {
            public void callback(Component context) { 
                Object o = getCurrentObject();
                if (o!=null)
                    HtmlObjectBrowser.presentObject(o,"HTML Object Browser");
            }
        });
    }



}

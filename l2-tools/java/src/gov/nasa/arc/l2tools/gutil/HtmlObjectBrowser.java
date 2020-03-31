//
// * See the file "l2-tools/disclaimers-and-notices.txt" for 
// * information on usage and redistribution of this file, 
// * and for a DISCLAIMER OF ALL WARRANTIES.
//
/*
* Author : Mark Shirley
* Description : HtmlObjectBrowser.java
*
* CHANGELOG:
* 11/01/99      Mhs     Created
*/

package gov.nasa.arc.l2tools.gutil;

import gov.nasa.arc.l2tools.util.NullWriter;
import gov.nasa.arc.l2tools.util.EventLog;
import gov.nasa.arc.l2tools.gutil.InputModeIdle;
import gov.nasa.arc.l2tools.util.Vector;

import java.awt.Component;
import java.awt.*;
import java.awt.event.MouseEvent;
import javax.swing.*;
import java.io.StringWriter;
import java.io.PrintWriter;
import java.io.IOException;
import java.util.Hashtable;

/** This is a generic JFrame for holding an HTML presentation of any
    object. */
public class HtmlObjectBrowser extends javax.swing.JFrame {

    // Used by addNotify
    boolean frameSizeAdjusted = false;

    /** Holds the actual browser pane. */
    HtmlPresentationPaneWithHistory browser;

    public HtmlObjectBrowser() {
        super("HTML Object Browser");
        browser = getBrowserPane();
        getContentPane().setLayout(new BorderLayout(0, 0));
        setSize(405, 305);
        setVisible(false);
        getContentPane().add(BorderLayout.CENTER, browser);
        browser.setBounds(0, 0, 405, 305);

        SymWindow aSymWindow = new SymWindow();
        this.addWindowListener(aSymWindow);
    }

    public HtmlObjectBrowser(String sTitle) {
        this();
        setTitle(sTitle);
    }

    public void setVisible(boolean b) {
        if (b)
            setLocation(50, 50);
        super.setVisible(b);
    }

    static public void main(String[] args) {
        (new HtmlObjectBrowser()).setVisible(true);
    }

    public void addNotify() {
        // Record the size of the window prior to calling parents addNotify.
        Dimension size = getSize();
        super.addNotify();
        if (frameSizeAdjusted)
            return;
        frameSizeAdjusted = true;
        // Adjust size of frame according to the insets and menu bar
        Insets insets = getInsets();
        javax.swing.JMenuBar menuBar = getRootPane().getJMenuBar();
        int menuBarHeight = 0;
        if (menuBar != null)
            menuBarHeight = menuBar.getPreferredSize().height;
        setSize(insets.left + insets.right + size.width, insets.top + insets.bottom + size.height + menuBarHeight);
    }

    protected HtmlPresentationPaneWithHistory getBrowserPane() { return new HtmlPresentationPaneWithHistory(); }

    /** This was synthesized by Visual Cafe to handle window closing. */
    class SymWindow extends java.awt.event.WindowAdapter {
        public void windowClosed(java.awt.event.WindowEvent event) {
            Object object = event.getSource();
            if (object == HtmlObjectBrowser.this)
                HtmlObjectBrowser_windowClosed(event);
        }
    }

    void HtmlObjectBrowser_windowClosed(java.awt.event.WindowEvent event) {
        // Needed this for the ice browser
        //if (browser != null) browser.dispose();
    }

    ///
    /// Interface for editing from outside
    ///
    
    HtmlWriter getHtmlWriter() {
        return browser.getHtmlWriter();
    }

    /** Clear the window. */
    public void htmlClear(boolean clearHistory) { 
        browser.htmlClear();
    }

    /** Append a string to the window.  If you do this more than once, you
     * must call htmlWait() in between to make sure the rendering thread has
     * caught up.  Otherwise, text can get lost.  */
    public void htmlAppend(java.lang.String str) {
        browser.htmlAppend(str);
    }

    /** Append text using a Reader.  I'm currently using the string version to
     * avoid multithreading code that just needs a static display.  */
    //public void htmlAppend(java.io.Reader reader, java.lang.String mimeType) {
    //    browser.htmlAppend(reader, mimeType);
    //}

    /** Wait for the browser's parsing and rendering thread to finish. */
    public void htmlWait(boolean all) { 
        browser.htmlWait(all);
    }

    public void presentAsLink(Presentation presentation, String linkText) throws IOException {
        browser.presentAsLink(presentation, linkText);
    }

    public void present(Object o) {
        browser.present(o);
    }

    public void present(Object o, String title) {
        browser.present(o);
        setTitle(title);
    }

    public Object getCurrentObject() {
        return browser.getCurrentObject();
    }

    ///
    /// Static functionality
    ///

    /** Create and show a new HtmlObjectBrowser displaying any object
        implementing the Presentable inferface. */
    public static void presentObject(Object o) {
        presentObject(o,"Object Browser");
    }

    public static void presentObject(Object o, String title) {
        HtmlObjectBrowser browser = new HtmlObjectBrowser();
        browser.setVisible(true);
        browser.present(o,title); 
   }
    
}

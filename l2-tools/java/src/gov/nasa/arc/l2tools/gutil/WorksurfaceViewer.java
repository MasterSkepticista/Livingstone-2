//
// * See the file "l2-tools/disclaimers-and-notices.txt" for 
// * information on usage and redistribution of this file, 
// * and for a DISCLAIMER OF ALL WARRANTIES.
//
/*
* Author : Mark Shirley
* Description : WorksurfaceViewer.java
*
* CHANGELOG:
* 11/01/99      Mhs     Created
*/

package gov.nasa.arc.l2tools.gutil;

import java.awt.*;
import java.util.Vector;
import javax.swing.*;

public class WorksurfaceViewer extends javax.swing.JFrame {

    Worksurface surface;

    public WorksurfaceViewer() {
        setTitle("Skunkworks Work Surface");
        getContentPane().setLayout(new BorderLayout(0, 0));
        setSize(500, 500);
        setVisible(false);
        surface = new Worksurface();
        surface.setBounds(0,0,800,800);
        JScrollPane scrollPane = new JScrollPane(surface);
        getContentPane().add(BorderLayout.CENTER, scrollPane);
        SymWindow aSymWindow = new SymWindow();
        this.addWindowListener(aSymWindow);
    }

    public WorksurfaceViewer(String sTitle) {
        this();
        setTitle(sTitle);
    }

    public void setVisible(boolean b) {
        if (b)
            setLocation(50, 50);
        super.setVisible(b);
    }

    static public void main(String[] args) {
        (new WorksurfaceViewer()).setVisible(true);
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

    // Used by addNotify
    boolean frameSizeAdjusted = false;

    /** This was synthesized by Visual Cafe to handle window closing. */
    class SymWindow extends java.awt.event.WindowAdapter {
        public void windowClosed(java.awt.event.WindowEvent event) {
            Object object = event.getSource();
            if (object == WorksurfaceViewer.this)
                WorksurfaceViewer_windowClosed(event);
        }
    }

    void WorksurfaceViewer_windowClosed(java.awt.event.WindowEvent event) {
    }

    public Worksurface getSurface() { return surface; }

}


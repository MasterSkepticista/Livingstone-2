//
// * See the file "l2-tools/disclaimers-and-notices.txt" for 
// * information on usage and redistribution of this file, 
// * and for a DISCLAIMER OF ALL WARRANTIES.
//
/*
* Author : Mark Shirley
* Description : JLabelPresentation.java
*
* A Justification
*
* CHANGELOG:
* 11/20/99      Mhs     Created.
*/

package gov.nasa.arc.l2tools.gutil;

import java.awt.Color;
import java.awt.Graphics;
import java.awt.FontMetrics;
import java.awt.Rectangle;
import java.awt.Component;

import javax.swing.JLabel;

public class JLabelPresentation extends JLabel implements SurfacePresentation {
    static final int BOX_EXPAND = 4;

    Object object;

    public JLabelPresentation() {
        super();
        setBackground(Color.red);
    }

    public JLabelPresentation(Object object) {
        super();
        this.object = object;
        setBounds(0,0,100,20);
    }

    public void initPresentation() {
        updateJLabel();
    }

    public Object getObject() { return object; }
    public void setObject(Object object) {
        this.object = object;
        updateJLabel();
    }

    public void addPopupMenuItems(Component c, java.util.Vector items) {
        items.addElement(menuItemDescribe());
    }

    protected PopupMenuItem menuItemDescribe() {
        return new PopupMenuItem("Describe",
                                 new PopupMenuCallback() {
            public void callback(Component context) { JLabelPresentation.this.describe(context); }
        });
    }

    /** Default describe operation: pop up an HtmlObjectBrowser. */
    public void describe(Component context) {
        HtmlObjectBrowser.presentObject(this);
    }

    protected void updateJLabel() {
        if (object==null) return;
        String s = object.toString();
        setText(s);

        Graphics g = getGraphics();
        if (g==null) return;

        try {
            FontMetrics fm = g.getFontMetrics();
            int height = fm.getHeight() + 2 * BOX_EXPAND;
            int width = fm.stringWidth(getText()) + 2 * BOX_EXPAND;
            Rectangle r = getBounds();
            setBounds(r.x,r.y,width,height);
        } finally {
            g.dispose();
        }
    }
    
    public String stringForWholine() { return getText(); }
    public void initPresentation(java.awt.Component ignore) {}
}

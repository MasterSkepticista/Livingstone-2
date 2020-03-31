//
// * See the file "l2-tools/disclaimers-and-notices.txt" for 
// * information on usage and redistribution of this file, 
// * and for a DISCLAIMER OF ALL WARRANTIES.
//
/*
* Author : Mark Shirley
* Description : Worksurface.java
*
* CHANGELOG:
* 11/01/99      Mhs     Created
*/

package gov.nasa.arc.l2tools.gutil;

import javax.swing.JComponent;
import java.awt.Graphics;
import java.util.Vector;

public class Worksurface extends JComponent {

    protected Vector presentations = new Vector();

    public Worksurface() {
        super();
    }

    public void addPresentation(SurfacePresentation p) {
        if (presentations.contains(p)) return;
        presentations.addElement(p);
        p.initPresentation(this);
        repaint(100);
    }

    public void addPresentation(SurfacePresentation p, int x, int y) {
        if (presentations.contains(p)) return;
        p.setLocation(x,y);
        presentations.addElement(p);
        p.initPresentation(this);
        repaint(100);
    }

    public void removePresentation(SurfacePresentation p) {
        if (!presentations.contains(p)) return;
        presentations.removeElement(p);
        repaint(100);
    }

    public void paint(Graphics g) {
        super.paint(g);
        int max = presentations.size();
        for (int i=0;i<max;i++)
            ((SurfacePresentation)presentations.elementAt(i)).paint(g);
    }

}

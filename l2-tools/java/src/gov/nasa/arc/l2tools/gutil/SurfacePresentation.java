//
// * See the file "l2-tools/disclaimers-and-notices.txt" for 
// * information on usage and redistribution of this file, 
// * and for a DISCLAIMER OF ALL WARRANTIES.
//
/*
 * Author : Mark Shirley
 * Description : SurfacePresentation.java
 *
 * CHANGELOG:
 * 11/27/99      Mhs     Created
 */

package gov.nasa.arc.l2tools.gutil;

import java.awt.Rectangle;
import java.awt.Dimension;
import java.awt.Graphics;
import java.awt.Component;

/** A presentation that's intended for use in a window like a normal
    component. */
public interface SurfacePresentation extends Presentation {

    /** Modify P to be the location of the upper left of the presentation.  If
        p is null, then create a new point and return that. */
    public Rectangle getBounds(Rectangle r);

    /** Set the upper left of the presentation to the specified point */
    public void setLocation(int x, int y);

    /** Allow the presentation to initialize itself.  If the presentation's a
        JComponent, this will be called after the presentation's been added to
        the container.  Other presentation can use it to get properties of the
        containing window as well. */
    public void initPresentation(Component container);

    /** Draw onto the graphics */
    public void paint(Graphics g);

}

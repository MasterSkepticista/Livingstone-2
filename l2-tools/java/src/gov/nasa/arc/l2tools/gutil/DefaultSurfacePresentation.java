//
// * See the file "l2-tools/disclaimers-and-notices.txt" for 
// * information on usage and redistribution of this file, 
// * and for a DISCLAIMER OF ALL WARRANTIES.
//
/*
 * Author : Mark Shirley
 * Description : DefaultSurfacePresentation.java
 *
 * Notes:
 * - For setLocation(), what about invalidating the rectangle I had?
 *   I don't have a pointer to the containing window.
 * - I don't know how to use resources properly.  I'm not loading the
 *   icon in the best way, I think.
 * 
 * CHANGELOG:
 * 11/27/99      Mhs     Created
 */

package gov.nasa.arc.l2tools.gutil;

import java.awt.Graphics;
import java.awt.Rectangle;
import java.awt.Component;
import java.awt.Image;
import java.util.Hashtable;
import javax.swing.ImageIcon;

/** A simple presentation for 2D drawing surfaces.  Instances of this class
    don't allow resizing.  Use ResizableSurfacePresentation if you need that.
    For Presentable objects, this will ask the object for an icon.  For
    non-presentable objects, this will look in a class -> icon mapping.
    (Presentable objects can also use the convenience of this mapping by
    returning null to sourceForIcon().)  */
public class DefaultSurfacePresentation extends PresentationBase implements SurfacePresentation {

    // Seeing whether this works
    static final String[] popupLabels = {"Describe"};
    static final String[] popupMethods = {"describe"};

    ImageIcon icon;
    String objectString;
    protected Rectangle bounds = new Rectangle(0,0,10,10);

    static final ImageIcon defaultImageIcon = new ImageIcon("images/defaultSurfacePresentationIcon.gif");

    Component surface;

    public DefaultSurfacePresentation(Object object) {
        super(object);
    }

    /** Modify P to be the location of the upper left of the presentation.  If
        p is null, then create a new point and return that. */
    public Rectangle getBounds(Rectangle r) {
        if (r==null) r = new Rectangle();
        r.x = bounds.x;
        r.y = bounds.y;
        r.width = bounds.width;
        r.height = bounds.height;
        return r;
    }

    /** Set the upper left of the presentation to the specified point.  Right
        now, this does no invalidation of the old and new regions of the
        containing window.  You have to repaint the whole window.  */
    public void setLocation(int x, int y) {
        bounds.x = x;
        bounds.y = y;
    }

    /** Allow the presentation to initialize itself.  If the presentation's a
        JComponent, this will be called after the presentation's been added to
        the container.  It can be used to get properties of the containing
        window as well. */
    public void initPresentation(Component container) {
        if (object==null) {
            icon = defaultImageIcon;
            objectString = "no object";
        } else {
            objectString = object.toString();
            // Take care of the icon
            if (object instanceof Presentable) {
                String source = ((Presentable)object).sourceForIcon();
                if (source!=null) icon = new ImageIcon(source);
            }
            if (icon==null)
                icon = (ImageIcon)PresentationUtils.getIcon(object.getClass());
            if (icon==null)
                icon = defaultImageIcon;
        }
        // Remember the surface if we're going to need it
        if (icon.getImageLoadStatus()!=java.awt.MediaTracker.COMPLETE)
            surface = container;

    }
    
    /** Draw onto the graphics */
    public void paint(Graphics g) {
        Image image = icon.getImage();
        g.drawImage(image,bounds.x,bounds.y,surface);
        g.drawString(objectString,bounds.x,bounds.y+image.getHeight(null)+g.getFontMetrics().getHeight());
    }
    
    public void open() {
    }

}

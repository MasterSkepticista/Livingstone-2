//
// * See the file "l2-tools/disclaimers-and-notices.txt" for 
// * information on usage and redistribution of this file, 
// * and for a DISCLAIMER OF ALL WARRANTIES.
//
package gov.nasa.arc.l2tools.gutil;

import java.awt.Rectangle;

/** A utility class supporting a sprite surface.  This isn't currently being used in skunkworks, but it may be. */
public class DirtyRectangle {
    public Rectangle r = new Rectangle();
    public DirtyRectangle next;

    public boolean intersects(DirtyRectangle other) {
        return r.intersects(other.r);
    }

    public void union(DirtyRectangle other) {
        Rectangle o = other.r;
        int x1 = Math.min(o.x, r.x);
        int x2 = Math.max(o.x + o.width, r.x + r.width);
        int y1 = Math.min(o.y, r.y);
        int y2 = Math.max(o.y + o.height, r.y + r.height);
        r.x = x1;
        r.y = y1;
        r.width = x2 - x1;
        r.height = y2 - y1;
    }
}


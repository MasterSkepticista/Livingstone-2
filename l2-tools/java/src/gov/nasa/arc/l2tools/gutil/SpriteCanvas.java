//
// * See the file "l2-tools/disclaimers-and-notices.txt" for 
// * information on usage and redistribution of this file, 
// * and for a DISCLAIMER OF ALL WARRANTIES.
//
/*
 * Author : Mark Shirley
 * Description : SpriteCanvas.java
 *
 * This is a very simple sprite surface.  Learning somebody else's
 * would take too much time.  Sigh.

 * CHANGELOG:
 * 08/15/98      Mhs     Created
 * 11/30/98      Mhs     Starting to use w/ Voyager.  Offscreen images are
 *                       a problem.
 * 12/08/98      Mhs     Moved this version into NAS sim
 *
 * TODO:
 * - Add synchronization
 * - Sort the dirty rectangles
 * - Better decision rule for combining dirty rectangles
 * - use the Component's getBackground() method instead of backgroundColor.
 */

package gov.nasa.arc.l2tools.gutil;

import java.awt.Image;
import java.awt.Color;
import java.awt.Graphics;
import java.awt.Dimension;
import javax.swing.JComponent;

// Was subclassed from Canvas

/** This is a very simple sprite surface.  It's not currently used in skunkworks.  It will be if we move to plan editing. */
public class SpriteCanvas extends JComponent {
    public Image backgroundImage;
    public Color backgroundColor;
    protected Image offScreenImage;
    protected Graphics offScreenGraphics;
    protected Dimension offScreenSize;
    static DirtyRectangle firstDirtyResource;
    static DirtyRectangle lastDirtyResource;
    DirtyRectangle firstDirty;

    public void setBackgroundImage(Image i) {
        backgroundImage = i;
    }

    public void setBackgroundColor(Color c) {
        backgroundColor = c;
    }

    public Color getBackgroundColor() {
        if (backgroundColor != null) return backgroundColor;
        return Color.lightGray;
    }

    protected void allocateOffscreenImage() {
        if (offScreenImage != null) return;
        Dimension d = getSize();
        offScreenImage = createImage(d.width, d.height);
        offScreenGraphics = offScreenImage.getGraphics();
        offScreenSize = d;
    }

    // Hacking around serialization problems for now
    public void dumpOffscreenImage() {
        backgroundImage = null;
        offScreenImage = null;
        if (offScreenGraphics != null) offScreenGraphics.dispose();
        offScreenGraphics = null;
        System.out.println("SpriteCanvas>>Dumping offscreen for serialization");
    }

    //  protected void allocateOffscreenImage() {
    //    if (offScreenImage==null) {
    //      Dimension d=size();
    //      if(offScreenSize==null || (d.width != offScreenSize.width) || (d.height != offScreenSize.height)) {
    //        offScreenImage = createImage(d.width,d.height);
    //        offScreenSize = d;
    //        offScreenGraphics = offScreenImage.getGraphics();
    //      }
    //    }
    //  }
    // No clearing of the area
    public final void update(Graphics g) {
        paint(g);
    }

    public void paint(Graphics g) {
        allocateOffscreenImage();
        paintBackground(offScreenGraphics);
        paintSprites(offScreenGraphics);
        g.drawImage(offScreenImage, 0, 0, null);
        drawXORDecorations(g, true);
    }

    public void paintSprites(Graphics g) {
    }

    public void drawXORDecorations(Graphics g, boolean force) {}

    // Call this with true if you're about to do the erasing yourself.
    // Else, call with false and the method's implementation has to get rid
    // of them.
    public boolean eraseXORDecorations(Graphics g, boolean takenCareOf) {
        return false;
    }

    public void setXORDecorationsVisible(boolean v) {}

    public void paintBackground(Graphics g) {
        //long time = System.currentTimeMillis();
        if (backgroundImage != null) {
            g.drawImage(backgroundImage, 0, 0, null);
        } else {
            Dimension s = getSize();
            g.setColor(getBackgroundColor());
            g.fillRect(0, 0, s.width, s.height);
        }
        //System.out.println("paintBackground time="+(System.currentTimeMillis()-time));
    }

    public void allocateBackground() {
        if (backgroundImage == null) {
            Dimension d = getSize();
            backgroundImage = createImage(d.width, d.height);
            // Why was this here?  It forms a loop with paintBackgroundImage()
            //paintBackgroundImage();
        }
    }

    // Draws a clean background image (if you need it)
    public void paintBackgroundImage() {
        if (backgroundImage == null) allocateBackground();
        if (backgroundImage == null) return;
        Dimension d = getSize();
        Graphics g = backgroundImage.getGraphics();
        g.setColor(Color.black);
        g.fillRect(0, 0, d.width, d.height);
        g.dispose();
    }

    public void paintBackgroundDirtyRects() {
        //long time = System.currentTimeMillis();
        allocateOffscreenImage(); // Why wasn't this done?
        if (backgroundImage != null) {
            DirtyRectangle d = firstDirty;
            while (d != null) {
                Graphics g = offScreenGraphics.create(d.r.x, d.r.y, d.r.width, d.r.height);
                g.drawImage(backgroundImage, -d.r.x, -d.r.y, null);
                g.dispose();
                d = d.next;
            }
        } else {
            offScreenGraphics.setColor(getBackgroundColor());
            DirtyRectangle d = firstDirty;
            while (d != null) {
                offScreenGraphics.fillRect(d.r.x, d.r.y, d.r.width, d.r.height);
                d = d.next;
            }
        }
        //System.out.println("paintBackgroundDirtyRects time="+(System.currentTimeMillis()-time));
    }

    public void updateScreen() {
        //long time = System.currentTimeMillis();
        if (offScreenImage == null) return;
        Graphics g = getGraphics();
        eraseXORDecorations(g, true);
        g.drawImage(offScreenImage, 0, 0, null);
        g.dispose();
        freeDirtyRectList(firstDirty);
        firstDirty = null;
        //System.out.println("updateScreen time="+(System.currentTimeMillis()-time));
    }

    public void markCanvasDirty() {
        DirtyRectangle d = allocDirtyRect();
        Dimension s = getSize();
        d.r.x = 0;
        d.r.y = 0;
        d.r.width = s.width;
        d.r.height = s.height;
        freeDirtyRectList(firstDirty);
        firstDirty = d;
    }

    // Performance version
    public void updateScreenDirtyRects() {
        Graphics g;
        g = getGraphics();
        boolean decorations = eraseXORDecorations(g, false);
        g.dispose();
        DirtyRectangle d = firstDirty;
        while (d != null) {
            g = getGraphics();
            g.clipRect(d.r.x, d.r.y, d.r.width, d.r.height);
            g.drawImage(offScreenImage, 0, 0, null);
            g.dispose();
            d = d.next;
        }
        freeDirtyRectList(firstDirty);
        firstDirty = null;
        // I'm not happy about this allocation.  Need a way to
        // check whether there are any decorations
        g = getGraphics();
        drawXORDecorations(g, false);
        g.dispose();
    }

    // Debugging version

  /*
  public void updateScreenDirtyRects() {
    //long time = System.currentTimeMillis();
    Graphics outer = getGraphics();//debugging
    Graphics g;
    g = getGraphics();
    boolean decorations = eraseXORDecorations(g,false);
    g.dispose();
    DirtyRectangle d = firstDirty;
    while (d!=null) {
      g = getGraphics();
      //if (g==null) System.out.println("g is null");
      //if (d.r==null) System.out.println("d.r is null");
      //if (d.r.x==null) System.out.println("d.r.x is null");
      //if (d.r.y==null) System.out.println("d.r.y is null");
      //if (d.r.width==null) System.out.println("d.r.width is null");
      //if (d.r.height==null) System.out.println("d.r.height is null");
      g.clipRect(d.r.x,d.r.y,d.r.width,d.r.height);
      g.drawImage(offScreenImage,0,0,null);
      g.dispose();
      // Debugging
      outer.setColor(Color.green);
      outer.drawRect(d.r.x,d.r.y,d.r.width,d.r.height);
      d = d.next;
    }
    outer.dispose();//debugging
    freeDirtyRectList(firstDirty);
    firstDirty = null;
    // I'm not happy about this allocation.  Need a way to
    // check whether there are any decorations
    g = getGraphics();
    drawXORDecorations(g,false);
    g.dispose();
    //System.out.println("updateScreenDirtyRects time="+(System.currentTimeMillis()-time));
  }
  */

    public int countDirtyRectangles() {
        int v = 0;
        DirtyRectangle r = firstDirty;
        while (r != null) {
            r = r.next;
            v++;
        }
        return v;
    }

    // Resource of dirty rectangles
    public static DirtyRectangle allocDirtyRect() {
        if (firstDirtyResource == null)
            return new DirtyRectangle();
        DirtyRectangle temp = firstDirtyResource;
        firstDirtyResource = temp.next;
        temp.next = null;
        if (firstDirtyResource == null) lastDirtyResource = null;
        return temp;
    }

    public void addDirtyRect(DirtyRectangle r) {
        DirtyRectangle current = firstDirty;
        while (current != null) {
            if (current.intersects(r)) {
                current.union(r);
                return;
            }
            current = current.next;
        }
        r.next = firstDirty;
        firstDirty = r;
    }

    public void freeDirtyRect(DirtyRectangle b) {
        b.next = null;
        if (lastDirtyResource == null) {
            lastDirtyResource = firstDirtyResource = b;
            return;
        }
        b.next = firstDirtyResource;
        firstDirtyResource = b;
    }

    public void freeDirtyRectList(DirtyRectangle b) {
        if (lastDirtyResource == null) {
            firstDirtyResource = b;
        } else {
            lastDirtyResource.next = b;
        }
        // Push lastDirtyResource out to the end
        DirtyRectangle a = b;
        while (a != null) {
            lastDirtyResource = a;
            a = a.next;
        }
    }

    public void printDirtyRectList() {
        //System.out.println("Dirty Rects--->");
        DirtyRectangle a = firstDirty;
        while (a != null) {
            System.out.println("   {" + a.r.x + "," + a.r.y + "," + a.r.width + "," + a.r.height + "}");
            a = a.next;
        }
    }

    protected void finalize() throws Throwable {
        if (offScreenGraphics != null) offScreenGraphics.dispose();
        super.finalize();
    }
}


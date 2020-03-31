// 
// * See the file "l2-tools/disclaimers-and-notices.txt" for 
// * information on usage and redistribution of this file, 
// * and for a DISCLAIMER OF ALL WARRANTIES. 
// 

/*
 * PromoteHandle.java
 *
 * Created on December 5, 2001, 2:39 PM
 */

package oliver.schematic;

/**
 *
 * @author  Stephen Wragg
 * @version
 */
public class PromoteHandle extends CH.ifa.draw.standard.AbstractHandle implements java.awt.image.ImageObserver, CH.ifa.draw.framework.Connector {
    
    private java.awt.Point fPoint = new java.awt.Point();
    private java.awt.Dimension fSize = new java.awt.Dimension();
    private java.awt.Image fImage;
    private oliver.xmpl.Promotion promotion;
    //private java.awt.Rectangle fDisplayBox = new java.awt.Rectangle(); // relative to the center of the owner
    
    /** Creates new PromoteHandle */
    public PromoteHandle(CH.ifa.draw.framework.Figure owner, oliver.xmpl.Promotion promotion) {
        super(owner);
        this.promotion = promotion;
        if(fImage != null) fImage.flush();
        fImage = new javax.swing.ImageIcon(getClass().getResource("/oliver/schematic/dot1.gif")).getImage();
        int w = fImage.getWidth(this);
        int h = fImage.getHeight(this);
        if(w > -1 && h > -1) {
            owner().willChange();
            fSize.width = w;
            fSize.height = h;
            updatePosition();
        } else {
            fSize.width = w;
            fSize.height = h;
        }
    }
    
    public String getAttributeFullname() {
        return promotion.getInstanceName();
    }
    
    /**
     * Handles asynchroneous image updates.
     */
    public boolean imageUpdate(java.awt.Image img, int flags, int x, int y, int w, int h) {
        if ((flags & (FRAMEBITS|ALLBITS)) != 0) {
            owner().willChange();
            updatePosition();
        }
        return (flags & (ALLBITS|ABORT)) == 0;
    }
    
    
    // **********************************************************************
    // **********************************************************************
    // **********************************************************************
    
    public void invokeStep(int x, int y, int fAnchorX, int fAnchorY, CH.ifa.draw.framework.DrawingView view) {
        owner().willChange();
        java.awt.Point center = owner().center();
        int a2 = 0;
        if(x != center.x) {
            double tan = (center.y-y)/(((double)x-center.x));
            double a1 = Math.atan(tan);
            a2 = (int)(a1/Math.PI*180f);
        } else {
            if((center.y-y) > 0)
                a2 = 90;
            else
                a2 = 270;
        }
        if((x-center.x) < 0) a2 = a2 + 180;
        else if((center.y-y) < 0) a2 = 360 + a2;
        promotion.setAngle(a2);
        updatePosition();
    }
    
    public void invokeEnd(int x, int y, int anchorX, int anchorY, CH.ifa.draw.framework.DrawingView view) {
        super.invokeEnd(x,y,anchorX,anchorY,view);
        Object root = promotion.getRoot();
        if(root instanceof oliver.xmpl.Project) {
            ((oliver.xmpl.Project)root).fireNodesChanged(this, ((ComponentFigure)owner()).getType(), "", null, null);
        }
    }
    
    public void updatePosition() {
        java.awt.Rectangle rec = owner().displayBox();
        rec.setLocation(-rec.width/2,-rec.height/2);
        fPoint = calcRecRadiusIntersect(rec, promotion.getAngle());
        owner().changed();
    }
    
    public java.awt.Point findStart(CH.ifa.draw.framework.Connector c) {
        return locate();
    }
    
    public java.awt.Point findEnd(CH.ifa.draw.framework.Connector c) {
        return locate();
    }
    
    /**
     * Locates the handle on the figure. The handle is drawn
     * centered around the returned point.
     */
    public java.awt.Point locate() {
        java.awt.Point p = owner().center();
        p.translate(fPoint.x, fPoint.y);
        //System.out.println("PromoteHandle.locate() returned x="+p.x+" y="+p.y);
        return p;
    }
    
    // **********************************************************************
    // **********************************************************************
    // **********************************************************************
    
    /**
     * Finds the start point for the connection.
     */
    public java.awt.Point findStart(CH.ifa.draw.framework.ConnectionFigure connection) {
        return locate();
    }
    
    /**
     * Finds the end point for the connection.
     */
    public java.awt.Point findEnd(CH.ifa.draw.framework.ConnectionFigure connection) {
        return locate();
    }
    
    /**
     * Gets the display box of the connector.
     */
    public java.awt.Rectangle displayBox() {
        java.awt.Point p = owner().center();
        java.awt.Rectangle r = new java.awt.Rectangle(fPoint, fSize);
        r.translate(p.x, p.y);
        r.translate(-fSize.width/2, -fSize.height/2);
        return r;
    }
    
    
    /**
     * Tests if a point is contained in the connector.
     */
    public boolean containsPoint(int x, int y) {
        return displayBox().contains(x,y);
    }
    
    /**
     * Draws this connector. Connectors don't have to be visible
     * and it is OK leave this method empty.
     */
    public void draw(java.awt.Graphics g) {
        // used to draw handle when selected
        // this is done in the StandardDrawingView Class
        java.awt.Rectangle r = displayBox();
        g.setColor(java.awt.Color.white);
        g.fillRect(r.x, r.y, r.width, r.height);
        g.setColor(java.awt.Color.black);
        g.drawImage(fImage,r.x, r.y, this);
    }
    
    public void drawUnselected(java.awt.Graphics g) {
        // used in the figure to draw the handles
        java.awt.Rectangle r = displayBox();
        g.drawImage(fImage,r.x, r.y, this);
    }
    
    // **********************************************************************
    // **********************************************************************
    // **********************************************************************
    
    public java.awt.Point calcRecRadiusIntersect(java.awt.Rectangle rec, int deg) {
        int gridsize = oliver.xmpl.Project.getGridSize();
        oliver.xmpl.Xmpl root = ((ComponentFigure)owner()).getType().getRoot();
        if(root != null && root instanceof oliver.xmpl.Project) {
            gridsize = oliver.xmpl.Project.getGridSize();
        }
        return calcRecRadiusIntersect(rec, promotion.getAngle(), gridsize);
    }
    
    public static java.awt.Point calcRecRadiusIntersect(java.awt.Rectangle rec, int deg, int gridsize) {
        double a = deg/360d*2d*Math.PI;
        double tan = Math.tan(a);
        //System.out.println("tan "+tan);
        int x = rec.x + rec.width/2;
        int y = rec.y + rec.height/2;
        int xb = 0;
        int yb = 0;
        if(Math.abs(tan) < ((double)rec.height)/rec.width) {
            //System.out.println("hit side");
            if(Math.cos(a) > 0) {
                //System.out.println("right side");
                x += rec.width/2;
                //xb = gridsize;
                y -= rec.width/2 * tan;
                //yb = -gridsize;
            } else {
                //System.out.println("left side");
                x -= rec.width/2;
                //xb = -gridsize;
                xb = -gridsize;
                y += rec.width/2 * tan;
                //yb = gridsize;
            }
        } else {
            //System.out.println("hit top/bottom");
            if(Math.sin(a) > 0) {
                //System.out.println("top");
                x += rec.height/2 / tan;
                //xb = gridsize;
                y -= rec.height/2;
                //yb = -gridsize;
                yb = -gridsize;
            } else {
                //System.out.println("bottom");
                x -= rec.height/2 / tan;
                //xb = -gridsize;
                y += rec.height/2;
                //yb = gridsize;
            }
        }
        //System.out.println("x="+x+" y="+y);
        x = ((x+xb+gridsize/2)/gridsize)*gridsize;
        //x += xb;
        y = ((y+yb+gridsize/2)/gridsize)*gridsize;
        //y += yb;
        //System.out.println("x="+x+" y="+y);
        return new java.awt.Point(x,y);
    }
    
    /**
     * Reads the object from the StorableInput.
     */
    public void read(CH.ifa.draw.util.StorableInput dr) throws java.io.IOException {
    }
    
    /**
     * Writes the object to the StorableOutput.
     */
    public void write(CH.ifa.draw.util.StorableOutput dw) {
    }
    
}

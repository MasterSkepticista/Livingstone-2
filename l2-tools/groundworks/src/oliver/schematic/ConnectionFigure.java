// 
// * See the file "l2-tools/disclaimers-and-notices.txt" for 
// * information on usage and redistribution of this file, 
// * and for a DISCLAIMER OF ALL WARRANTIES. 
// 

/*
 * Connection.java
 *
 * Created on September 27, 2001, 3:58 PM
 */

package oliver.schematic;

/**
 *
 * @author  Stephen Wragg
 * @version
 */
public class ConnectionFigure extends CH.ifa.draw.figures.LineConnection implements XmplFigure {
    
    protected oliver.xmpl.Xmpl xmpl;
    protected net.n3.nanoxml.XMLElement line;
    protected java.util.Vector xmlPoints = new java.util.Vector();
    
    public ConnectionFigure(oliver.xmpl.Xmpl xmpl) {
        super();
        this.xmpl = xmpl;
        java.util.Enumeration enum = xmpl.getChildrenNamed("gr:line").elements();
        if(enum.hasMoreElements()) {
            this.line = (net.n3.nanoxml.XMLElement)enum.nextElement();
        } else {
            this.line = new net.n3.nanoxml.XMLElement();
            this.line.setName("gr:line");
            xmpl.addChild(this.line);
        }
        //setLine(this.line); do this after the ends are connected, see addPromoteHandle()
    }
    
    protected void setLine(net.n3.nanoxml.IXMLElement line) {
        java.util.Enumeration enum = line.getChildrenNamed("gr:point").elements();
        while(enum.hasMoreElements()) {
            net.n3.nanoxml.IXMLElement child = (net.n3.nanoxml.IXMLElement)enum.nextElement();
            xmlPoints.addElement(child);
            int x = Integer.valueOf(child.getAttribute("x","100")).intValue();
            int y = Integer.valueOf(child.getAttribute("y","100")).intValue();
            int g = oliver.xmpl.Project.getGridSize();
            x = ((x+g/2)/g*g);
            y = ((y+g/2)/g*g);
            fPoints.insertElementAt(new java.awt.Point(x, y),fPoints.size()-1);
        }
    }
    
    public void insertPointAt(java.awt.Point p, int i) {
        net.n3.nanoxml.IXMLElement point = new net.n3.nanoxml.XMLElement();
        point.setName("gr:point");
        point.setAttribute("x",Integer.toString(p.x));
        point.setAttribute("y",Integer.toString(p.y));
        line.insertChild(point, i-1);
        xmlPoints.insertElementAt(point, i-1);
        super.insertPointAt(p, i);
    }
    
    public void removePointAt(int i) {
        line.removeChildAtIndex(i-1);
        xmlPoints.remove(i-1);
        super.removePointAt(i);
    }
    
    /**
     * Copied from super and updated.
     * Gets the handles of the figure. It returns the normal
     * PolyLineHandles but adds ChangeConnectionHandles at the
     * start and end.
     */
    public java.util.Vector handles() {
        java.util.Vector handles = new java.util.Vector(fPoints.size());
        handles.addElement(new CH.ifa.draw.standard.ChangeConnectionStartHandle(this));
        for (int i = 1; i < fPoints.size()-1; i++)
            handles.addElement(new MyPolyLineHandle(this, locator(i), i, (net.n3.nanoxml.IXMLElement)xmlPoints.elementAt(i-1)));
        handles.addElement(new CH.ifa.draw.standard.ChangeConnectionEndHandle(this));
        return handles;
    }
    
    private class MyPolyLineHandle extends CH.ifa.draw.figures.PolyLineHandle {
        private net.n3.nanoxml.IXMLElement xml;
        public MyPolyLineHandle(CH.ifa.draw.figures.PolyLineFigure owner, CH.ifa.draw.framework.Locator l, int index, net.n3.nanoxml.IXMLElement xml) {
            super(owner, l, index);
            this.xml = xml;
        }
        public java.awt.Point locate() {
            java.awt.Point p = super.locate();
            int g = oliver.xmpl.Project.getGridSize();
            p.x = ((p.x+g/2)/g*g);
            p.y = ((p.y+g/2)/g*g);
            return p;
        }
        public void invokeStep(int x, int y, int anchorX, int anchorY, CH.ifa.draw.framework.DrawingView view) {
            int g = oliver.xmpl.Project.getGridSize();
            x = ((x+g/2)/g*g);
            y = ((y+g/2)/g*g);
            xml.setAttribute("x", Integer.toString(x));
            xml.setAttribute("y", Integer.toString(y));
            super.invokeStep(x, y,  anchorX, anchorY, view);
        }
    }
    
    public int findSegment(int x, int y) {
        int r = super.findSegment(x, y);
        return r;
    }
    
    public javax.swing.Action[] getActions(final int context, final oliver.application.UserProperties userProperties, final java.awt.Point popupLocation, final java.awt.Point drawingLocation) {
        javax.swing.Action[] a = new javax.swing.Action[2];
        a[0] = new javax.swing.AbstractAction("Insert Point") {
            public void actionPerformed(java.awt.event.ActionEvent e) {
                int index = splitSegment(drawingLocation.x, drawingLocation.y);
            }
        };
        a[1] = new javax.swing.AbstractAction("Delete Point") {
            public void actionPerformed(java.awt.event.ActionEvent e) {
                int index = findPointIndex(drawingLocation.x, drawingLocation.y);
                if(index > -1) removePointAt(index);
            }
        };
        return a;
    }
    
    private int findPointIndex(int x, int y){
        int i = -1;
        int i2 = 0;
        int d = Integer.MAX_VALUE;
        java.util.Enumeration enum = points();
        while(enum.hasMoreElements()) {
            java.awt.Point p = (java.awt.Point)enum.nextElement();
            int d2 = (int)Math.sqrt(Math.pow((p.x - x),2)+Math.pow((p.y - y),2));
            if(d2 < d) {
                i = i2;
                d = d2;
            }
            i2++;
        }
        return i;
    }
    
    public void addPromoteHandle(PromoteHandle handle) {
        if(start() == null) {
            connectStart(handle);
            java.awt.Point p = handle.locate();
            startPoint(p.x,p.y);  // very strange.  this is the fix for a weird bug in hotdraw
        } else if(end() == null) {
            connectEnd(handle);
            java.awt.Point p = handle.locate();
            endPoint(p.x,p.y);  // very strange.  this is the fix for a weird bug in hotdraw
            setLine(this.line);
        }
        else System.out.println("ConnectionFigure.addPromoteHandle() more than two connectors");
    }
    
    public java.awt.Point location() {
        return null;
    }
    
    public String getMouseOverText() {
        return xmpl.getMouseOverText();
    }
    
    protected void drawLine(java.awt.Graphics g, int x1, int y1, int x2, int y2) {
        java.awt.Stroke stroke = ((java.awt.Graphics2D)g).getStroke();
        setFrameColor(java.awt.Color.DARK_GRAY);
        ((java.awt.Graphics2D)g).setRenderingHint(java.awt.RenderingHints.KEY_ANTIALIASING, java.awt.RenderingHints.VALUE_ANTIALIAS_ON);
        ((java.awt.Graphics2D)g).setStroke(new java.awt.BasicStroke(2,java.awt.BasicStroke.CAP_ROUND,java.awt.BasicStroke.JOIN_ROUND));
        g.drawLine(x1, y1, x2, y2);
        ((java.awt.Graphics2D)g).setRenderingHint(java.awt.RenderingHints.KEY_ANTIALIASING, java.awt.RenderingHints.VALUE_ANTIALIAS_OFF);
        ((java.awt.Graphics2D)g).setStroke(stroke);
    }
    
}

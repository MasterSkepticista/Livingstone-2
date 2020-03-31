// 
// * See the file "l2-tools/disclaimers-and-notices.txt" for 
// * information on usage and redistribution of this file, 
// * and for a DISCLAIMER OF ALL WARRANTIES. 
// 

/*
 * ComponentFigure.java
 *
 * Created on October 6, 2002, 4:43 PM
 */

package oliver.schematic;

/**
 *
 * @author  stephen
 */
public class ComponentFigure extends CH.ifa.draw.figures.AttributeFigure implements AttributeXmplFigure, java.awt.image.ImageObserver {
    
    private java.awt.Image fImage;
    //private java.util.HashMap images = new java.util.HashMap();
    private java.awt.Rectangle fDisplayBox; // the origon is real, but the width snaps to the grid
    private oliver.xmpl.Attribute att;
    private oliver.xmpl.ComponentType type;
    private java.util.Vector handles = new java.util.Vector();
    private java.util.Vector connectors = new java.util.Vector();
    private java.awt.Insets insets = new java.awt.Insets(0,0,0,0);
    protected String lastvalueformouseover = null;

    public ComponentFigure(oliver.scenario.State state, oliver.xmpl.Attribute att) throws Exception {
        super();
        this.att = att;
        oliver.xmpl.Xmpl xmpl = att.getType();
        java.awt.Point origin = att.getPoint();
        if(!(xmpl instanceof oliver.xmpl.ComponentType)) throw new Exception("ComponentFigure constructor type is class "+xmpl.getClass().toString());
        this.type = (oliver.xmpl.ComponentType)xmpl;
        fDisplayBox = new java.awt.Rectangle(origin.x, origin.y, 0, 0);
        updateType(state);
        java.util.Enumeration enum = type.getAllChildrenNamed("gr:promotion").elements();
        while(enum.hasMoreElements()) {
            net.n3.nanoxml.IXMLElement obj = (net.n3.nanoxml.IXMLElement)enum.nextElement();
            if(obj instanceof oliver.xmpl.Promotion) {
                PromoteHandle h = new PromoteHandle(this, (oliver.xmpl.Promotion)obj);
                connectors.addElement(h);
                handles.addElement(h);
                h.updatePosition();
            }
            /*
            if(obj.getName().equals("gr:image_map")) {
                java.util.Enumeration enum2 = type.getChildrenNamed("gr:gif_image").elements();
                while(enum2.hasMoreElements()) {
                    net.n3.nanoxml.IXMLElement obj2 = (net.n3.nanoxml.IXMLElement)enum2.nextElement();
                    String name = obj2.getAttribute("name", null);
                    String url = obj2.getAttribute("url", null);
                    if(name != null && url != null) images.put(name, url);
                }
            }
             **/
        }
    }
    
    public void updateType(oliver.scenario.State state) {
        if(fImage != null) fImage.flush();
        try {
            String imagename = getAttribute().getInstanceName();
            lastvalueformouseover = state.getAttributeAssignment("test."+imagename+".mode");
            fImage = state.getImage(getType(), imagename).getImage();
            Thread thread = new Thread(new Runnable() {
                public void run() {
                    int w = fImage.getWidth(ComponentFigure.this);
                    int h = fImage.getHeight(ComponentFigure.this);
                    if(w > -1 && h > -1) {
                        willChange();
                        int g = oliver.xmpl.Project.getGridSize();
                        fDisplayBox.width = (w+g)/(2*g)*2*g;
                        fDisplayBox.height = (h+g)/(2*g)*2*g;
                        updateAllPromoteHandlePositions();
                        changed();
                        if(listener() != null) {
                            listener().figureRequestUpdate(new CH.ifa.draw.framework.FigureChangeEvent(ComponentFigure.this));
                        }
                    }
                }
            }
            );
            thread.setPriority(thread.MIN_PRIORITY);
            thread.start();
        } catch(Exception e) {
        }
    }
    
    private void updateAllPromoteHandlePositions() {
        java.util.Enumeration enum = connectors.elements();
        while(enum.hasMoreElements()) {
            Object obj = enum.nextElement();
            if(obj instanceof PromoteHandle) {
                ((PromoteHandle)obj).updatePosition();
            }
        }
    }
    
    public javax.swing.Action[] getActions(final int context, final oliver.application.UserProperties userProperties, final java.awt.Point popupLocation, final java.awt.Point drawingLocation) {
        if(context == EDITOR_CONTEXT) {
            javax.swing.Action[] a = new javax.swing.Action[3];
            a[0] = new javax.swing.AbstractAction("Promote Attribute") {
                public void actionPerformed(java.awt.event.ActionEvent e) {
                    promoteAttribute(userProperties, popupLocation);
                }
            };
            a[1] = new javax.swing.AbstractAction("Select Image") {
                public void actionPerformed(java.awt.event.ActionEvent e) {
                    selectImage(userProperties, popupLocation);
                }
            };
            a[2] = new javax.swing.AbstractAction("Delete") {
                public void actionPerformed(java.awt.event.ActionEvent e) {
                    removeFromContainer(listener());
                }
            };
            //javax.swing.Action[] b = type.getActions(context, userProperties); this makes it possible to delete the type from the instance
            return a;
        } else if(context == DEBUGGER_CONTEXT) {
            javax.swing.Action[] a = new javax.swing.Action[2];
            a[0] = new javax.swing.AbstractAction("Properties") {
                public void actionPerformed(java.awt.event.ActionEvent e) {
                }
            };
            a[1] = new javax.swing.AbstractAction("Open") {
                public void actionPerformed(java.awt.event.ActionEvent e) {
                }
            };
            return a;
        } else {
            return null;
        }
    }
    
    private void selectImage(oliver.application.UserProperties userProperties, java.awt.Point point) {
        ImageDialog d = new ImageDialog(userProperties, getType(), true);
        d.setLocationRelativeTo(userProperties.getFrame());
        d.show();
        oliver.xmpl.Xmpl root = type.getRoot();
        if(root instanceof oliver.xmpl.Project) {
            // the view should pick this up and call updateXmplImage() on all attributes of this type.
            ((oliver.xmpl.Project)root).fireNodesChanged(this, getType(), "gr:image_map", null, null);
        }
    }
    
    private void promoteAttribute(oliver.application.UserProperties userProperties, java.awt.Point point) {
        oliver.project.SortedListModel names = new oliver.project.SortedListModel();
        java.util.Enumeration enum = getType().getAllChildrenNamed("ct:attribute").elements();
        while(enum.hasMoreElements()) {
            Object obj = enum.nextElement();
            if(obj instanceof oliver.xmpl.Attribute) {
                java.util.Vector p = new java.util.Vector();
                ((oliver.xmpl.Attribute)obj).getAllNames(p, names);
            }
        }
        PromoteAttributeDialog d = new PromoteAttributeDialog(userProperties.getFrame(),true);
        d.setListModel(names);
        d.setLocationRelativeTo(userProperties.getFrame());
        d.show();
        Object[] sel = d.getSelectedValues();
        if(sel != null && sel.length > 0) {
            for(int i = 0; i < sel.length; i++) {
                try {
                    getPromoteHandle(sel[i].toString());
                } catch(java.util.NoSuchElementException e) {
                    System.out.println(e);
                }
            }
        }
    }
    
    
    // For every case that I can think of the "fullname" will only have one name,
    // but I want to support promoting deep attributes.
    public oliver.schematic.PromoteHandle getPromoteHandle(String fullname) throws java.util.NoSuchElementException {
        // first let's find the attribute
        oliver.xmpl.Xmpl xmpl = type.recursiveSearchForAttributeOrRelationByFullname(fullname);  // yeah, it's a mouthfull
        if(xmpl == null) throw new java.util.NoSuchElementException("ComponentFigure.getPromoteHandle() no xmpl found for "+fullname);
        
        // found it, let's see if there is a handle
        oliver.schematic.PromoteHandle p = null;
        java.util.Enumeration enum = handles.elements();
        while(enum.hasMoreElements()) {
            Object obj = enum.nextElement();
            if(obj instanceof oliver.schematic.PromoteHandle) {
                String fn = ((oliver.schematic.PromoteHandle)obj).getAttributeFullname();
                if(fullname.equals(fn)) return (oliver.schematic.PromoteHandle)obj;
            }
        }
        if(p == null) {
            // does the xml promotion exist
            oliver.xmpl.Promotion promotion = null;
            enum = type.getAllChildrenNamed("gr:promotion").elements();
            while(enum.hasMoreElements()) {
                Object obj = enum.nextElement();
                if(obj instanceof oliver.xmpl.Promotion) {
                    if(((oliver.xmpl.Promotion)obj).getInstanceName().equals(fullname)) {
                        promotion = (oliver.xmpl.Promotion)obj;
                        break;
                    }
                }
            }
            if(promotion == null) {
                promotion = new oliver.xmpl.Promotion();
                promotion.setInstanceName(fullname);
                type.insertXmplChild(this, promotion, 0);
            }
            p = new PromoteHandle(this, promotion);
            // do this before fireing the insertion event and remember to check
            // for the new handle before creating it.
            handles.addElement(p);
            connectors.addElement(p);
            changed();
        }
        return p;
    }
    
    
    public oliver.xmpl.ComponentType getType() {
        return type;
    }
    
    public oliver.xmpl.Attribute getAttribute() {
        return att;
    }
    
    public void basicDisplayBox(java.awt.Point origin, java.awt.Point corner) {
        fDisplayBox = new java.awt.Rectangle(origin);
        int g = oliver.xmpl.Project.getGridSize();
        corner.x = (corner.x+(g/2))/g*g;
        corner.y = (corner.y+(g/2))/g*g;
        fDisplayBox.add(corner);
    }
    
    public java.util.Vector handles() {
        //java.util.Vector handles = new java.util.Vector();
        //CH.ifa.draw.standard.BoxHandleKit.addHandles(this, handles);
        return handles;
    }
    
    public java.awt.Rectangle displayBox() {
        int g = oliver.xmpl.Project.getGridSize();
        int x = (fDisplayBox.x+(g/2))/g*g;
        int y = (fDisplayBox.y+(g/2))/g*g;
        return new java.awt.Rectangle(x,y,fDisplayBox.width, fDisplayBox.height);
    }
    
    public CH.ifa.draw.framework.Connector findStart(int x, int y, CH.ifa.draw.framework.ConnectionFigure con) {
        int d = Integer.MAX_VALUE;
        CH.ifa.draw.framework.Connector c = null;
        java.util.Enumeration enum = connectors.elements();
        while(enum.hasMoreElements()) {
            CH.ifa.draw.framework.Connector pc = (CH.ifa.draw.framework.Connector)enum.nextElement();
            java.awt.Point p = pc.findStart(con);
            int d2 = (int)Math.sqrt(Math.pow(p.x-x,2)+Math.pow(p.y-y,2));
            if(d2 < d) {
                d = d2;
                c = pc;
            }
        }
        return c;
    }
    
    public CH.ifa.draw.framework.Connector findEnd(int x, int y, CH.ifa.draw.framework.ConnectionFigure con) {
        int d = Integer.MAX_VALUE;
        CH.ifa.draw.framework.Connector c = null;
        java.util.Enumeration enum = connectors.elements();
        while(enum.hasMoreElements()) {
            CH.ifa.draw.framework.Connector pc = (CH.ifa.draw.framework.Connector)enum.nextElement();
            java.awt.Point p = pc.findEnd(con);
            int d2 = (int)Math.sqrt(Math.pow(p.x-x,2)+Math.pow(p.y-y,2));
            if(d2 < d) {
                d = d2;
                c = pc;
            }
        }
        return c;
    }
    
    protected void basicMoveBy(int x, int y) {
        fDisplayBox.translate(x,y);
    }
    
    public void draw(java.awt.Graphics g) {
        if (fImage != null) {
            java.awt.Rectangle r = displayBox();
            g.drawImage(fImage, r.x, r.y, r.width, r.height, this);
        } else {
            drawGhost(g);
        }
        java.util.Enumeration enum = connectors.elements();
        while(enum.hasMoreElements()) {
            Object obj = enum.nextElement();
            if(obj instanceof PromoteHandle) {
                ((PromoteHandle)obj).drawUnselected(g);
            }
        }
    }
    
    private void drawGhost(java.awt.Graphics g) {
        g.setColor(java.awt.Color.gray);
        java.awt.Rectangle r = displayBox();
        g.fillRect(r.x, r.y, r.width, r.height);
    }
    
    /**
     * Handles asynchroneous image updates.
     */
    public boolean imageUpdate(java.awt.Image img, int flags, int x, int y, int w, int h) {
        if ((flags & (FRAMEBITS|ALLBITS)) != 0) {
            willChange();
            int g = oliver.xmpl.Project.getGridSize();
            fDisplayBox.width = (w+g)/(2*g)*2*g;
            fDisplayBox.height = (h+g)/(2*g)*2*g;
            updateAllPromoteHandlePositions();
            changed();
            if(listener() != null) {
                listener().figureRequestUpdate(new CH.ifa.draw.framework.FigureChangeEvent(this));
            }
        }
        return (flags & (ALLBITS|ABORT)) == 0;
    }
    
    public java.awt.Point location() {
        return displayBox().getLocation();
    }
    
    public String getMouseOverText() {
        if(lastvalueformouseover==null) return att.getMouseOverText();
        else return att.getMouseOverText()+" = "+lastvalueformouseover;
    }
    
}

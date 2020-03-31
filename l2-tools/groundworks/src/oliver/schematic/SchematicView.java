// 
// * See the file "l2-tools/disclaimers-and-notices.txt" for 
// * information on usage and redistribution of this file, 
// * and for a DISCLAIMER OF ALL WARRANTIES. 
// 

/*
 * SchematicView.java
 *
 * Created on October 1, 2002, 11:51 AM
 */

package oliver.schematic;

/**
 *
 * @author  stephen
 */
public class SchematicView extends DrawingEditorPanel implements oliver.project.ProjectListener {
    
    private oliver.project.ProjectModel model;
    private oliver.application.UserProperties userProperties;
    private boolean editable = false;
    private oliver.scenario.State state;
    private int context;
    
    public static int EDITOR_CONTEXT = 1;
    public static int DEBUGGER_CONTEXT = 2;
   
    // property I create to support drag and drop
    private oliver.xmpl.Xmpl softSelectedXmpl;
    
    /** Creates a new instance of SchematicView */
    public SchematicView(int context, oliver.application.UserProperties userProperties) {
        super();
        this.context = context;
        this.userProperties = userProperties;
        this.context = context;
        this.state = new oliver.scenario.State(userProperties);
        init();
    }
    
    protected void init() {
        setTransferHandler(new javax.swing.TransferHandler("softSelectedXmpl"));
        try {
            getDropTarget().addDropTargetListener(new java.awt.dnd.DropTargetAdapter() {
                public void drop(java.awt.dnd.DropTargetDropEvent dtde) {
                    createAttributeInstanceAt(getSoftSelectedXmpl(), dtde.getLocation());
                }
            });
        } catch(java.util.TooManyListenersException e) {
            System.out.println(e);
        }
    }
    
    public void setState(livdll.AttributeAssignment[] data) {
        try {
            state.setAttributeAssignments(data);
            if(drawing() instanceof ComponentDrawing) {
                ((ComponentDrawing)drawing()).updateFigures(state);
            }
        } catch(Exception e) {
            System.out.println(e);
        }
    }
    
    public void setDrawing(CH.ifa.draw.framework.Drawing drawing) {
        super.setDrawing(drawing);
        if(drawing instanceof MyStandardDrawingView) {
            ((MyStandardDrawingView)drawing).setEditable(editable);
        }
    }
    
    public oliver.schematic.ComponentDrawing createDrawing(oliver.xmpl.ComponentType ct) {
        return new oliver.schematic.ComponentDrawing(state, ct, "test");
    }
    
    // Had to override this to make popup menus work.
    protected CH.ifa.draw.standard.StandardDrawingView createDrawingView() {
        CH.ifa.draw.standard.StandardDrawingView v = super.createDrawingView();
        v.addMouseListener(new java.awt.event.MouseAdapter() {
            public void mouseClicked(java.awt.event.MouseEvent evt) {
                SchematicView.this.mouseClicked(evt);
            }
            public void mousePressed(java.awt.event.MouseEvent evt) {
                SchematicView.this.mousePressed(evt);
            }
            public void mouseReleased(java.awt.event.MouseEvent evt) {
                SchematicView.this.mouseReleased(evt);
            }
        });
        return v;
    }
    
    public void setModel(oliver.project.ProjectModel model) {
        this.model = model;
        this.editable = true;
        if(view() instanceof MyStandardDrawingView) {
            ((MyStandardDrawingView)view()).setEditable(editable);
        } else {
            System.out.println("SchematicView.setModel() not MyStandardDrawingView");
        }
    }
    
    public oliver.xmpl.Xmpl getSoftSelectedXmpl() {
        return this.softSelectedXmpl;
    }
    
    public void setSoftSelectedXmpl(oliver.xmpl.Xmpl softSelectedXmpl) {
        this.softSelectedXmpl = softSelectedXmpl;
    }
    
    private void createAttributeInstanceAt(oliver.xmpl.Xmpl type, java.awt.Point location) {
        java.awt.Container panel = (java.awt.Container)view();
        java.awt.Dimension offset = new java.awt.Dimension();
        while(panel != null && panel != this) {
            java.awt.Point l = panel.getLocation();
            offset.width = offset.width - l.x;
            offset.height = offset.height - l.y;
            panel = panel.getParent();
        }
        location.x = location.x + offset.width;
        location.y = location.y + offset.height;
        
        if(type instanceof oliver.xmpl.ComponentType) {
            String name = javax.swing.JOptionPane.showInputDialog(userProperties.getRootComponent(), "Enter name of new Component Instance:");
            oliver.xmpl.ComponentType parent = getDrawingComponentType();
            if(name != null && parent != null && parent.isLegalName(name)) {
                oliver.xmpl.Attribute att = new oliver.xmpl.Attribute();
                att.setPoint(location);
                att.setTypeName(type.getAttribute("name","######")); // I can do this because there are no inner componentTypes
                att.setInstanceName(name);
                parent.insertXmplChild(this, att, 0);  // updates the drawing in the listener
            }
        } else {
            System.out.println("SchematicView.createAttributeInstanceAt() not implemented for "+type.getFullName());
        }
    }
    
    private oliver.xmpl.ComponentType getDrawingComponentType() {
        oliver.xmpl.ComponentType type = null;
        if(drawing() instanceof ComponentDrawing) {
            type = ((ComponentDrawing)drawing()).getComponentType();
        }
        return type;
    }
    
    protected void createTools(javax.swing.JToolBar palette) {
        super.createTools(palette);
        
        CH.ifa.draw.framework.Tool tool = new CH.ifa.draw.figures.TextTool(view(), new CH.ifa.draw.figures.TextFigure());
        palette.add(createToolButton(IMAGES+"TEXT", "Text Tool", tool));
        
        tool = new CH.ifa.draw.standard.CreationTool(view(), new CH.ifa.draw.figures.RectangleFigure());
        palette.add(createToolButton(IMAGES+"RECT", "Rectangle Tool", tool));
        
        tool = new CH.ifa.draw.standard.CreationTool(view(), new CH.ifa.draw.figures.RoundRectangleFigure());
        palette.add(createToolButton(IMAGES+"RRECT", "Round Rectangle Tool", tool));
        
        tool = new CH.ifa.draw.standard.CreationTool(view(), new CH.ifa.draw.figures.EllipseFigure());
        palette.add(createToolButton(IMAGES+"ELLIPSE", "Ellipse Tool", tool));
        
        tool = new CH.ifa.draw.standard.CreationTool(view(), new CH.ifa.draw.figures.LineFigure());
        palette.add(createToolButton(IMAGES+"LINE", "Line Tool", tool));
        
        tool = new CH.ifa.draw.contrib.PolygonTool(view());
        palette.add(createToolButton(IMAGES+"POLYGON", "Polygon Tool", tool));
        
        tool = new CH.ifa.draw.standard.ConnectionTool(view(), new CH.ifa.draw.figures.LineConnection());
        palette.add(createToolButton(IMAGES+"CONN", "Connection Tool", tool));
        
        tool = new CH.ifa.draw.standard.ConnectionTool(view(), new CH.ifa.draw.figures.ElbowConnection());
        palette.add(createToolButton(IMAGES+"OCONN", "Elbow Connection Tool", tool));
    }
    
    // I think this happens before every mouseClicked event
    // This was created to support moving figures and line points
    // there is a better way to do this, check out PromoteHandle.invokeEnd()
    // not sure about moving figures, but for handles it looks better
    private void mousePressed(java.awt.event.MouseEvent evt) {
        java.awt.Point p = evt.getPoint();
        Object obj = view().drawing().findFigure(p.x, p.y);
        if(obj != null && obj instanceof XmplFigure) {
            mousePressedOnFigure = (XmplFigure)obj;
            mousePressedOnFigureLastLocation = mousePressedOnFigure.location();
        } else {
            mousePressedOnFigure = null;
            mousePressedOnFigureLastLocation = null;
        }
    }
    
    private void mouseReleased(java.awt.event.MouseEvent evt) {
        if(mousePressedOnFigure != null && mousePressedOnFigureLastLocation != null && mousePressedOnFigure instanceof AttributeXmplFigure) {
            java.awt.Point np = mousePressedOnFigure.location();
            if(!np.equals(mousePressedOnFigureLastLocation)) {
                String nx = String.valueOf(np.x);
                String ny = String.valueOf(np.y);
                String ox = String.valueOf(mousePressedOnFigureLastLocation.x);
                String oy = String.valueOf(mousePressedOnFigureLastLocation.y);
                model.getRoot().fireNodesChanged(this, ((AttributeXmplFigure)mousePressedOnFigure).getAttribute(), "x", nx, ox);
                model.getRoot().fireNodesChanged(this, ((AttributeXmplFigure)mousePressedOnFigure).getAttribute(), "y", ny, oy);
            }
        }
    }
    
    private XmplFigure mousePressedOnFigure = null;
    private java.awt.Point mousePressedOnFigureLastLocation = null;
    
    private void mouseClicked(java.awt.event.MouseEvent evt) {
        if(evt.isPopupTrigger() || (evt.getModifiers() & evt.BUTTON3_MASK)>0) {
            if(mousePressedOnFigure != null ) {
                java.awt.Point popupLocation = evt.getPoint();
                java.awt.Point drawingLocation = new java.awt.Point(popupLocation);
                
                java.awt.Container con = (java.awt.Container)view();
                while(con != this) {
                    java.awt.Point l = con.getLocation();
                    popupLocation.translate(l.x, l.y);
                    con = con.getParent();
                }
                javax.swing.Action[] actions = mousePressedOnFigure.getActions(context, userProperties, popupLocation, drawingLocation);
                createPopupMenu(popupLocation, actions);
                drawing().figureRequestUpdate(new CH.ifa.draw.framework.FigureChangeEvent(mousePressedOnFigure));
            }
        }
    }
    
    private void createPopupMenu(java.awt.Point point, javax.swing.Action[] actions) {
        javax.swing.JPopupMenu menu = new javax.swing.JPopupMenu();
        if(actions != null && actions.length > 0) {
            for(int i = 0; i < actions.length; i++) {
                menu.add(actions[i]);
            }
        }
        menu.show(this, point.x, point.y);
    }
    
    /** An undoable edit happened
     *
     */
    public void undoableEditHappened(javax.swing.event.UndoableEditEvent e) {
        if(!(e.getEdit() instanceof oliver.project.ProjectModelUndoEvent)) return;
        oliver.project.ProjectModelUndoEvent event = (oliver.project.ProjectModelUndoEvent)e.getEdit();
        
        if(event.getType() == event.EVERYTHING_CHANGED) {
            setDrawing(createDrawing());
            
        } else if(event.getType() == event.NODES_INSERTED) {
            oliver.xmpl.Xmpl parent = event.getParent();
            oliver.xmpl.Xmpl child = event.getChild();
            if(parent != null && child instanceof oliver.xmpl.Attribute && parent == getDrawingComponentType()) {
                oliver.xmpl.Attribute att = (oliver.xmpl.Attribute)child;
                try {
                    ComponentFigure cf = new ComponentFigure(state, att);
                    drawing().add(cf);
                    // The ComponentFigure constructor calls changed after the image update,
                    // but sometimes that completes before it's added to the drawing,
                    // even thought it's in another thread.
                    //cf.changed();  // Belt and suspenders
                } catch(Exception ee) {
                    System.out.println("SchematicView.createAttributeInstanceAt() "+ee);
                }
            }
            
        } else if(event.getType() == event.NODES_REMOVED) {
            if(drawing() instanceof ComponentDrawing) {
                oliver.xmpl.ComponentType myComponentType = ((ComponentDrawing)drawing()).getComponentType();
                oliver.xmpl.Xmpl child = event.getChild();
                if(event.getSource() != this && event.getParent() == myComponentType && child != null && child instanceof oliver.xmpl.Attribute) {
                    String childname = ((oliver.xmpl.Attribute)child).getInstanceName();
                    CH.ifa.draw.framework.FigureEnumeration enum = drawing().figures();
                    while(enum.hasMoreElements()) {
                        CH.ifa.draw.framework.Figure figure = enum.nextFigure();
                        if(figure instanceof oliver.schematic.AttributeXmplFigure) {
                            String name = ((oliver.schematic.AttributeXmplFigure)figure).getAttribute().getAttribute("name",null);
                            if(name.equals(childname)) {
                                figure.release();  // might have to overload this in my figures
                                drawing().remove(figure);
                            }
                        }
                    }
                }
            }
            
        } else if(event.getType() == event.NODES_CHANGED) {
            oliver.xmpl.Xmpl parent = event.getParent();
            if(event.getSource() == this && parent instanceof oliver.xmpl.Attribute) {
                parent.setAttribute(event.getName(), event.getNewValue()); // see mouseReleased()
            }
            if(parent instanceof oliver.xmpl.ComponentType && drawing() instanceof ComponentDrawing) {
                ((ComponentDrawing)drawing()).updateComponentType(state, (oliver.xmpl.ComponentType)parent);
            }
            
        } else if(event.getType() == event.NODE_SELECTED) {
            
        } else if(event.getType() == event.NODE_INSTANCE_ROOT) {
            final oliver.xmpl.Xmpl parent = event.getParent();
            if(parent != null && parent instanceof oliver.xmpl.ComponentType) {
                Thread thread = new Thread(new Runnable() {
                    public void run() {
                        oliver.xmpl.ComponentType ct = (oliver.xmpl.ComponentType)parent;
                        ComponentDrawing d = createDrawing(ct);
                        setDrawing(d);
                    }
                });
                thread.setPriority(thread.MIN_PRIORITY);
                thread.start();
            } else {
                setDrawing(createDrawing());
            }
            
        }
    }
    
}

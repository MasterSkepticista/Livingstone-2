//
// * See the file "l2-tools/disclaimers-and-notices.txt" for
// * information on usage and redistribution of this file,
// * and for a DISCLAIMER OF ALL WARRANTIES.
//

/*
 * ComponentDrawing.java
 *
 * Created on May 9, 2002, 11:19 AM
 */

package oliver.schematic;

/**
 *
 * @author  stephen
 */
public class ComponentDrawing extends CH.ifa.draw.standard.StandardDrawing {
    
    private String name = "no name";
    /** Holds value of property componentType. */
    private oliver.xmpl.ComponentType componentType;
    
    private java.util.HashMap componentFiguresByName = new java.util.HashMap();
    private java.util.HashMap attributeFiguresByName = new java.util.HashMap();
    
    /** Creates a new instance of ComponentDrawing */
    public ComponentDrawing(oliver.scenario.State state, oliver.xmpl.ComponentType componentType, String name) {
        this.componentType = componentType;
        initDrawing(state);
    }
    
    public void updateFigures(oliver.scenario.State state) {
        CH.ifa.draw.framework.FigureEnumeration enum = figures();
        while(enum.hasMoreElements()) {
            CH.ifa.draw.framework.Figure f = enum.nextFigure();
            if(f instanceof ComponentFigure) {
                ((ComponentFigure)f).updateType(state);
            }
        }
    }
    
    public void updateComponentType(oliver.scenario.State state, oliver.xmpl.ComponentType type) {
        oliver.xmpl.Xmpl root = type.getRoot();
        java.util.Iterator enum = componentFiguresByName.values().iterator();
        while(enum.hasNext()) {
            ComponentFigure f = (ComponentFigure)enum.next();
            oliver.xmpl.ComponentType ct = f.getType();
            while(ct != null) {
                f.updateType(state);
                String sup = ct.getAttribute("extends",null);
                if(sup!=null) {
                    Object obj = root.findType(sup);
                    if(obj != null && obj instanceof oliver.xmpl.ComponentType) {
                        ct = (oliver.xmpl.ComponentType)obj;
                    } else {
                        ct = null;
                    }
                } else {
                    ct = null;
                }
            }
        }
        String sup = type.getAttribute("extends",null);
        if(sup!=null) {
            Object obj = root.findType(sup);
            if(obj != null && obj instanceof oliver.xmpl.ComponentType) {
                // if this type has a parent we should update that as well
                updateComponentType(state, (oliver.xmpl.ComponentType)obj);
            }
        }
    }
    
    private void initDrawing(oliver.scenario.State state) {
        // First get the attributes so later you can use them to init the connections
        java.util.Enumeration enum = componentType.getAllChildrenNamed("ct:attribute").elements();
        while(enum.hasMoreElements()) {
            Object obj = enum.nextElement();
            if(obj instanceof oliver.xmpl.Attribute) {
                oliver.xmpl.Attribute att = (oliver.xmpl.Attribute)obj;
                try {
                    if(att.getType() instanceof oliver.xmpl.ComponentType) {
                        ComponentFigure figure = new ComponentFigure(state, att);
                        String name = att.getAttribute("name", null);
                        if(name != null) componentFiguresByName.put(name, figure);
                        add(figure);
                    } else {
                        // not adding attribute instances yet
                    }
                } catch(Exception e) {
                    System.out.println(e);
                }
            }
        }
        // now lets look at the constraint group and set up the connections
        enum = componentType.getChildrenNamed("ex:group").elements();
        while(enum.hasMoreElements()) {
            Object obj = enum.nextElement();
            if(obj instanceof oliver.xmpl.Group) {
                // The constraint is the only group child in a component
                java.util.Enumeration enum2 = ((oliver.xmpl.Group)obj).getChildren().elements();
                while(enum2.hasMoreElements()) {
                    Object obj2 = enum2.nextElement();
                    if(obj2 instanceof oliver.xmpl.Equals) {
                        addEqualsConnection((oliver.xmpl.Equals)obj2);
                    } else if(obj2 instanceof oliver.xmpl.Rel) {
                        addRelConnection((oliver.xmpl.Rel)obj2);
                    }
                }
            }
        }
        enum = componentType.getChildrenNamed("gr:text").elements();
        while(enum.hasMoreElements()) {
            TextFigure f = new TextFigure((net.n3.nanoxml.IXMLElement)enum.nextElement());
            add(f);
        }
        
    }
    
    /*
        <ex:rel name="tk02.pneumaticsLineOut.getPressure" arguments="mpre102p.sensedPressure">
          <gr:line startAttribute="tk02.pneumaticsLineOut.getPressure" endAttribute="mpre102p.sensedPressure">
            <gr:point x="80" y="320"/>
            <gr:point x="55" y="300"/>
            <gr:point x="165" y="300"/>
            <gr:point x="165" y="160"/>
            <gr:point x="150" y="155"/>
          </gr:line>
        </ex:rel>
     */
    private void addRelConnection(oliver.xmpl.Rel rel) {
        java.util.Enumeration enum = RelConnection.createRelConnections(rel);
        while(enum.hasMoreElements()) {
            RelConnection rc = (RelConnection)enum.nextElement();
            String startFullname = rc.getStartAttribute();  // don't think this could be null
            String endFullname = rc.getEndAttribute();
            int is = startFullname.indexOf("."); // should be at least one
            int ie = endFullname.indexOf(".");
            if(is != -1 && ie != -1) {
                String startFigureName = startFullname.substring(0, is);
                String endFigureName = endFullname.substring(0, ie);
                ComponentFigure startfigure = (ComponentFigure)componentFiguresByName.get(startFigureName);
                ComponentFigure endfigure = (ComponentFigure)componentFiguresByName.get(endFigureName);
                if(startfigure != null && endfigure != null) {
                    PromoteHandle startHandle = startfigure.getPromoteHandle(startFullname.substring(is+1));
                    PromoteHandle endHandle = endfigure.getPromoteHandle(endFullname.substring(ie+1));
                    if(startHandle != null && endHandle != null) {
                        rc.addPromoteHandle(startHandle);
                        rc.addPromoteHandle(endHandle);
                        add(rc);
                    }
                }
            } else {
                System.out.println("ComponentDrawing.addRelConnection() couldnt find figure");
            }
        }
    }
    
    private void addEqualsConnection(oliver.xmpl.Equals equals) {
        EqualsConnection f = new EqualsConnection(equals);
        f.setStartDecoration(null);
        f.setEndDecoration(null);
        java.util.StringTokenizer tok = equals.getArguments();
        while(tok.hasMoreElements()) {
            String fullname = tok.nextToken();
            int index = fullname.indexOf(".");
            if(index > -1) {
                String attName = fullname.substring(0,index);
                ComponentFigure figure = (ComponentFigure)componentFiguresByName.get(attName);
                if(figure != null) {
                    try {
                        PromoteHandle handle = figure.getPromoteHandle(fullname.substring(index+1));
                        if(handle != null) f.addPromoteHandle(handle);
                    } catch(java.util.NoSuchElementException e) {
                        System.out.println(e);
                    }
                }
            }
        }
        if(f.start() != null && f.end() != null) {
            add(f);
        } else System.out.println("ComponentDrawing.addEqualsConnection() didn't add connection because it's missing end(s)");
    }
    
    /** Getter for property name.
     * @return Value of property name.
     */
    public String getName() {
        return this.name;
    }
    
    /** Setter for property name.
     * @param name New value of property name.
     */
    public void setName(String name) {
        this.name = name;
    }
    
    /** Getter for property componentType.
     * @return Value of property componentType.
     */
    public oliver.xmpl.ComponentType getComponentType() {
        return this.componentType;
    }
    
    /** Setter for property componentType.
     * @param componentType New value of property componentType.
     */
    public void setComponentType(oliver.xmpl.ComponentType componentType) {
        this.componentType = componentType;
    }
    
}

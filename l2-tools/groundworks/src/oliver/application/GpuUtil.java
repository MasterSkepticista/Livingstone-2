// 
// * See the file "l2-tools/disclaimers-and-notices.txt" for 
// * information on usage and redistribution of this file, 
// * and for a DISCLAIMER OF ALL WARRANTIES. 
// 

/*
 * GpuUtil.java
 *
 * Created on January 30, 2002, 10:46 AM
 */

package oliver.application;

/**
 *
 * @author  stephen
 */
public class GpuUtil {
    
    private java.util.TreeMap attTypes = new java.util.TreeMap();
    private java.util.TreeMap atts = new java.util.TreeMap();
    
    /** Creates a new instance of GpuUtil */
    public GpuUtil() {
    }
    
    public static void main(String[] args) {
        GpuUtil g = new GpuUtil();
        try {
            //g.grabGraphicalData("C:/cvs/GPU/GUI/docs/sample/pitex_2_01.L2model","c:/PITEX_1_04/source1.smpl");
            g.removeStuffNotUsed("C:/cvs/skunkworks/groundworks/examples/pitex_1_04/source4.smpl","mainPropulsionSystem");
        } catch(Exception e) {
            System.out.println(e);
        }
    }
    
    private void removeStuffNotUsed(String file, String topComponentName) {
        try {
            net.n3.nanoxml.StdXMLBuilder builder = new net.n3.nanoxml.StdXMLBuilder();
            net.n3.nanoxml.IXMLParser parser = net.n3.nanoxml.XMLParserFactory.createDefaultXMLParser(builder);
            net.n3.nanoxml.IXMLReader reader = new net.n3.nanoxml.StdXMLReader(new java.io.FileReader(file));
            parser.setReader(reader);
            net.n3.nanoxml.IXMLElement top = (net.n3.nanoxml.XMLElement) parser.parse();
            net.n3.nanoxml.IXMLElement project = top;
            while(!project.getName().equals("project")) {
                project = (net.n3.nanoxml.IXMLElement)project.getChildren().firstElement();
            }
            net.n3.nanoxml.IXMLElement ci = findComponentType(project, topComponentName);
            java.util.Vector a = new java.util.Vector();
            java.util.Vector c = new java.util.Vector();
            c.addElement(ci);
            String s = "test";
            java.util.Vector typepath = new java.util.Vector();
            typepath.add(ci);
            recuresComponentInstance(typepath, a, c, s);
            java.util.Enumeration enum = project.getChildrenNamed("attributetype").elements();
            while(enum.hasMoreElements()) {
                project.removeChild((net.n3.nanoxml.IXMLElement)enum.nextElement());
            }
            enum = project.getChildrenNamed("componenttype").elements();
            while(enum.hasMoreElements()) {
                project.removeChild((net.n3.nanoxml.IXMLElement)enum.nextElement());
            }
            enum = c.elements();
            while(enum.hasMoreElements()) {
                project.addChild((net.n3.nanoxml.IXMLElement)enum.nextElement());
            }
            enum = a.elements();
            while(enum.hasMoreElements()) {
                project.addChild((net.n3.nanoxml.IXMLElement)enum.nextElement());
            }
            java.io.FileWriter filewriter = new java.io.FileWriter(file+".bak");
            net.n3.nanoxml.XMLWriter writer = new net.n3.nanoxml.XMLWriter(filewriter);
            writer.write(top, true);
            filewriter.flush();
            filewriter.close();
        } catch(Exception e) {
            System.out.println(e);
        }
    }
    
    private void recuresComponentInstance(java.util.Vector typepath, java.util.Vector a, java.util.Vector c, String s) {
        net.n3.nanoxml.IXMLElement ci = (net.n3.nanoxml.IXMLElement)typepath.lastElement();
        java.util.Enumeration enum = ci.getChildrenNamed("ct:attribute").elements();
        while(enum.hasMoreElements()) {
            net.n3.nanoxml.IXMLElement att = (net.n3.nanoxml.IXMLElement)enum.nextElement();
            net.n3.nanoxml.IXMLElement type = findAttributeType(ci, att.getAttribute("type", ""));
            String s2 = s+"."+att.getAttribute("name","");
            if(type != null) {
                String value = null;
                if(type.getParent().getName().equals("project") && !a.contains(type)) a.addElement(type);
                if(s2.indexOf("Reading.") > -1 || s2.indexOf(".rp1VentLineOut.") > -1) {
                    if(s2.endsWith(".upperBound") || s2.endsWith(".lowerBound")
                    || s2.endsWith(".controllerPressureBias") || s2.endsWith(".pr02Crack")) {
                        value = "Observable";
                    }
                } else if(s2.indexOf(".pv") > -1 || s2.indexOf(".sv") > -1) {
                    if(s2.endsWith(".openMicroSwitch") || s2.endsWith(".closedMicroSwitch")) {
                        value = "Observable";
                    } else if(s2.endsWith(".valveCmdIn")) {
                        value = "Commandable";
                    }
                } else if(s2.endsWith("pressureDifferenceA") || s2.endsWith("pressureDifferenceB")) {
                        value = "Observable";
                }
                
                
                
                if(value!=null) {
                    if(typepath.size()>1) {
                        net.n3.nanoxml.IXMLElement second = (net.n3.nanoxml.IXMLElement)typepath.elementAt(1);
                        if(second.getName().equals("componenttype")) {
                            net.n3.nanoxml.IXMLElement n = second.getFirstChildNamed("gr:harnessValues");
                            if(n == null) {
                                n = new net.n3.nanoxml.XMLElement("gr:harnessValues");
                                second.addChild(n);
                            }
                            boolean found = false;
                            String shortname = s2.substring(s2.indexOf(".")+1);
                            shortname = shortname.substring(shortname.indexOf(".")+1);
                            
                            java.util.Enumeration enum2 = n.getChildrenNamed("gr:harnessValue").elements();
                            while(enum2.hasMoreElements()) {
                                if(((net.n3.nanoxml.IXMLElement)enum2.nextElement()).getAttribute("name","").equals(shortname)) {
                                    found = true;
                                    break;
                                }
                            }
                            if(!found) {
                                net.n3.nanoxml.XMLElement o = new net.n3.nanoxml.XMLElement("gr:harnessValue");
                                o.setAttribute("name", shortname);
                                o.setAttribute("value", value);
                                n.addChild(o);
                            }
                        }
                    }
                }
                
                
            } else {
                type = findComponentType(ci, att.getAttribute("type", ""));
                if(type != null) {
                    if(!c.contains(type)) c.addElement(type);
                }
                java.util.Vector typepath2 = new java.util.Vector(typepath);
                typepath2.addElement(type);
                recuresComponentInstance(typepath2, a, c, s2);
            }
        }
    }
    
    private net.n3.nanoxml.IXMLElement findAttributeType(net.n3.nanoxml.IXMLElement node, String name) {
        java.util.Vector types = null;
        if(node.getName().equals("project")) types = node.getChildrenNamed("attributetype");
        else types = node.getChildrenNamed("ct:attributetype");
        java.util.Enumeration enum = types.elements();
        while(enum.hasMoreElements()) {
            net.n3.nanoxml.IXMLElement child = (net.n3.nanoxml.IXMLElement)enum.nextElement();
            System.out.println(name+" : "+child.getAttribute("name",""));
            if(child.getAttribute("name","").equals(name)) {
                return child;
            }
        }
        if(!node.getName().equals("project")) return findAttributeType(node.getParent(), name);
        else return null;
    }
    
    private net.n3.nanoxml.IXMLElement findComponentType(net.n3.nanoxml.IXMLElement node, String name) {
        java.util.Vector types = null;
        if(node.getName().equals("project")) types = node.getChildrenNamed("componenttype");
        else types = node.getChildrenNamed("ct:componenttype");
        java.util.Enumeration enum = types.elements();
        while(enum.hasMoreElements()) {
            net.n3.nanoxml.IXMLElement child = (net.n3.nanoxml.IXMLElement)enum.nextElement();
            if(child.getAttribute("name","").equals(name)) return child;
        }
        if(!node.getName().equals("project")) return findComponentType(node.getParent(), name);
        else return null;
    }
    
    private void grabGraphicalData(String from, String to) {
        net.n3.nanoxml.XMLElement xmlFrom = null;
        net.n3.nanoxml.XMLElement xmlTo = null;
        try {
            net.n3.nanoxml.StdXMLBuilder builder = new net.n3.nanoxml.StdXMLBuilder();
            net.n3.nanoxml.IXMLParser parser = net.n3.nanoxml.XMLParserFactory.createDefaultXMLParser(builder);
            net.n3.nanoxml.IXMLReader reader = new net.n3.nanoxml.StdXMLReader(new java.io.FileReader(from));
            parser.setReader(reader);
            xmlFrom = (net.n3.nanoxml.XMLElement) parser.parse();
            reader = new net.n3.nanoxml.StdXMLReader(new java.io.FileReader(to));
            parser.setReader(reader);
            xmlTo = (net.n3.nanoxml.XMLElement) parser.parse();
            
            addGraphicalData(xmlFrom, xmlTo);
            
            java.io.FileWriter filewriter = new java.io.FileWriter(to+".bak");
            net.n3.nanoxml.XMLWriter writer = new net.n3.nanoxml.XMLWriter(filewriter);
            writer.write(xmlTo, true);
            filewriter.flush();
            filewriter.close();
            
        } catch(Exception e) {
            System.out.println(e);
        }
    }
    
    private void addGraphicalData(net.n3.nanoxml.XMLElement xmlFrom, net.n3.nanoxml.XMLElement xmlTo) {
        if(xmlFrom.getName().equals("gui:setup")) return;
        if(xmlFrom.getName().equals("ct:attribute")) {
            xmlTo.setAttribute("x", xmlFrom.getAttribute("x","0"));
            xmlTo.setAttribute("y", xmlFrom.getAttribute("y","0"));
        } else if(xmlFrom.getName().equals("gr:promotion")) {
            xmlTo.setAttribute("angle", xmlFrom.getAttribute("angle","0"));
        } else if(xmlFrom.getName().equals("gr:line")) {
            java.util.Enumeration enum = xmlTo.getChildrenNamed("gr:point").elements();
            while(enum.hasMoreElements()) {
                xmlTo.removeChild((net.n3.nanoxml.XMLElement)enum.nextElement());
            }
            enum = xmlFrom.getChildrenNamed("gr:point").elements();
            while(enum.hasMoreElements()) {
                xmlTo.addChild((net.n3.nanoxml.XMLElement)enum.nextElement());
            }
        } else if(xmlFrom.getName().endsWith("type")) {
            //net.n3.nanoxml.IXMLElement image = xmlTo.getFirstChildNamed("gr:image");
            //if(image != null) xmlTo.removeChild(image);
            //image = xmlFrom.getFirstChildNamed("gr:image");
            //if(image != null) xmlTo.addChild(image);
            
            java.util.Enumeration enum = xmlTo.getChildrenNamed("textfigure").elements();
            while(enum.hasMoreElements()) {
                xmlTo.removeChild((net.n3.nanoxml.XMLElement)enum.nextElement());
            }
            enum = xmlFrom.getChildrenNamed("textfigure").elements();
            while(enum.hasMoreElements()) {
                xmlTo.addChild((net.n3.nanoxml.XMLElement)enum.nextElement());
            }
        }
        java.util.Enumeration enum = xmlTo.getChildren().elements();
        while(enum.hasMoreElements()) {
            net.n3.nanoxml.XMLElement child = (net.n3.nanoxml.XMLElement)enum.nextElement();
            net.n3.nanoxml.XMLElement likeChild = findLikeChild(xmlFrom, child);
            if(likeChild != null) addGraphicalData(likeChild, child);
        }
        
    }
    
    public net.n3.nanoxml.XMLElement findLikeChild(net.n3.nanoxml.XMLElement parent, net.n3.nanoxml.XMLElement child) {
        net.n3.nanoxml.XMLElement ret;
        if(child.getName().equals("ex:group")) return (net.n3.nanoxml.XMLElement)parent.getFirstChildNamed("ex:group");
        if(child.getName().equals("ex:line")) return (net.n3.nanoxml.XMLElement)parent.getFirstChildNamed("ex:line");
        java.util.Enumeration enum = parent.getChildrenNamed(child.getName()).elements();
        while(enum.hasMoreElements()) {
            net.n3.nanoxml.XMLElement child2 = (net.n3.nanoxml.XMLElement)enum.nextElement();
            String startAttribute2 = child2.getAttribute("startAttribute", null);
            String endAttribute2 = child2.getAttribute("endAttribute", null);
            String startAttribute = child.getAttribute("startAttribute", null);
            String endAttribute = child.getAttribute("endAttribute", null);
            if(startAttribute != null && endAttribute != null) {
                if(startAttribute2.equals(startAttribute) && endAttribute2.equals(endAttribute)) return child2;
                else return null;
            } else {
                String arguments = child.getAttribute("arguments", null);
                String arguments2 = child2.getAttribute("arguments", null);
                if(arguments != null) {
                    if(arguments.equals(arguments2)) return child2;
                } else {
                    String childName = child.getAttribute("name",null);
                    String childName2 = child2.getAttribute("name", null);
                    if(childName != null && childName2.equals(childName)) return child2;
                }
            }
        }
        return null;
    }
    
    
}

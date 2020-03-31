//
// * See the file "l2-tools/disclaimers-and-notices.txt" for
// * information on usage and redistribution of this file,
// * and for a DISCLAIMER OF ALL WARRANTIES.
//

package oliver.xmpl;

import java.util.List;
import javax.swing.text.*;

public class ComponentType extends Xmpl {
    
    private int numStateVectors = -1;
    //private javax.swing.text.GapContent content = new javax.swing.text.GapContent();
    private boolean compiled = false;
    
    public ComponentType() {
        super();
    }
    
    public ComponentType(String fullName, String namespace, String systemID, int lineNr) {
        super(fullName, namespace, systemID, lineNr);
    }
    
    public boolean compiledFileUpToDate() {
        java.io.File file = getCompiledFile();
        if(file == null || file.isDirectory() || !file.exists()) return false;
        long current = file.lastModified();
        String recordedString = getAttribute("compiledFileModifiedDate", null);
        if(recordedString == null) return false;
        try {
            long recorded = Long.decode(recordedString).longValue();
            if(current == recorded) return true;
            else return false;
        } catch(NumberFormatException e) {
            return false;
        }
    }
    
    public java.io.File getCompiledFile() {
        String filename = getAttribute("compiledFileName", null);
        if(filename == null) return null;
        java.io.File file = new java.io.File(filename);
        return file;
    }
    
    public Xmpl recursiveSearchForAttributeOrRelationByFullname(String fullname) {
        String name = fullname;
        int i = fullname.indexOf(".");
        if(i > -1) name = fullname.substring(0, i);
        
        Xmpl xmpl = null;
        java.util.Enumeration enum = getChildrenNamed("ct:attribute").elements();
        while(enum.hasMoreElements()) {
            Object obj = enum.nextElement();
            if(obj instanceof Attribute) {
                String instanceName = ((Attribute)obj).getInstanceName();
                if(instanceName != null && instanceName.equals(name)) {
                    xmpl =  (Attribute)obj;
                    break;
                }
            }
        }
        
        if(xmpl != null && xmpl instanceof Attribute) {
            Attribute att = (Attribute)xmpl;
            if(i > 0) {
                fullname = fullname.substring(i+1);
                Xmpl ct = null;
                try {
                    ct = att.getType();
                } catch(Exception e) {
                    System.out.println("ComponentType.recursiveSearchForAttributeByFullname() no type found");
                    System.out.println(e);
                    return null;
                }
                if(ct instanceof ComponentType) {
                    return ((ComponentType)ct).recursiveSearchForAttributeOrRelationByFullname(fullname);
                } else {
                    System.out.println("ComponentType.recursiveSearchForAttributeByFullname() non ComponentType in path");
                    return null;
                }
            } else {
                return att;
            }
        }
        
        if(xmpl == null) {
            enum = getChildrenNamed("relation").elements();
            while(enum.hasMoreElements()) {
                Object obj = enum.nextElement();
                if(obj instanceof Relation) {
                    String relname = ((Relation)obj).getAttribute("name", null);
                    if(relname != null && relname.equals(fullname)) {
                        return (Relation)obj;
                    }
                }
            }
        }
        
        
        System.out.println("ComponentType.recursiveSearchForAttributeByFullname() no xml named "+name+" in "+getFullName());
        return null;
        
    }
    
    
    public int compareTo(Object o) {
        if(o instanceof AttributeType) return 1;
        else if(o instanceof ComponentType) {
            boolean hasState1 = ((ComponentType)o).getHasState();
            boolean hasState2 = getHasState();
            if(hasState1 ^ hasState2) {
                if(hasState1) return -1;
                else return 1;
            }
        }
        return super.compareTo(o);
    }
    
    public boolean getHasState() {
        if(numStateVectors < 0) {
            java.util.Vector v = getChildrenNamed("ct:statevector");
            numStateVectors = v.size();
        }
        return numStateVectors > 0;
    }
    
    public javax.swing.Action[] getActions(final int context, final oliver.application.UserProperties userProperties) {
        javax.swing.Action[] a = super.getActions(context, userProperties);
        javax.swing.Action[] b = new javax.swing.Action[a.length+4];
        if(a.length > 0) {
            for(int i = 0; i < a.length; i++) {
                b[i] = a[i];
            }
        }
        b[a.length] = new javax.swing.AbstractAction("Make Root Component") {
            public void actionPerformed(java.awt.event.ActionEvent e) {
                Xmpl root = getRoot();
                if(root instanceof Project) {
                    ((Project)root).fireNodeInstanceRoot(this, ComponentType.this);
                }
            }
        };
        b[a.length+1] = new javax.swing.AbstractAction("Compile") {
            public void actionPerformed(java.awt.event.ActionEvent a) {
                String compiledDirectoryName = userProperties.getProperty(userProperties.CURRENT_DIRECTORY);
                String fileName = getAttribute("name",null)+".xmpl";
                java.io.File file = new java.io.File(compiledDirectoryName+userProperties.SLASH+fileName);
                while(file.exists()) {
                    Object[] options = {"Delete","Cancel","Options"};
                    int n = javax.swing.JOptionPane.showOptionDialog(userProperties.getRootComponent(),"File "+file.getName()+" exist, would you like to delete it?"
                    ,"Warning",javax.swing.JOptionPane.OK_CANCEL_OPTION,javax.swing.JOptionPane.WARNING_MESSAGE,null,options,options[0]);
                    if(n == 0) {
                        file.delete();
                        break;
                    } else if(n == 1) {
                        return;
                    } else {
                        javax.swing.JFileChooser fc = new javax.swing.JFileChooser(file.getParentFile());
                        fc.setDialogTitle("Compiled file name");
                        fc.setName(fileName);
                        javax.swing.filechooser.FileFilter ff = new javax.swing.filechooser.FileFilter() {
                            public boolean accept(java.io.File f) {
                                if(f.isDirectory() || f.getName().endsWith(".xmpl")) return true;
                                else return false;
                            }
                            public String getDescription() { return "Compiled model file"; }
                        };
                        fc.setFileFilter(ff);
                        int n2 = fc.showDialog(userProperties.getRootComponent(), "Compile");
                        if(n2 != fc.APPROVE_OPTION) return;
                        file = fc.getSelectedFile();
                        if(file.isDirectory()) file = new java.io.File(file, fileName);
                    }
                }
                try {
                    try {
                        ComponentType.this.compile(file, userProperties.getOutputStream());
                        net.n3.nanoxml.StdXMLBuilder builder = new net.n3.nanoxml.StdXMLBuilder();
                        net.n3.nanoxml.IXMLParser parser = net.n3.nanoxml.XMLParserFactory.createDefaultXMLParser(builder);
                        net.n3.nanoxml.IXMLReader reader = new net.n3.nanoxml.StdXMLReader(new java.io.FileReader(file));
                        parser.setReader(reader);
                        
                        net.n3.nanoxml.IXMLElement xmpl = (net.n3.nanoxml.IXMLElement) parser.parse();
                        
                        oliver.project.ProjectModel model = (oliver.project.ProjectModel)((Project)getRoot()).getModel();
                        oliver.tree.ComponentInstance ci = new oliver.tree.ComponentInstance(model,ComponentType.this);
                        net.n3.nanoxml.IXMLElement hrn = ci.createHarness();
                        net.n3.nanoxml.IXMLElement ini = ci.createInitialState();
                        
                        xmpl.addChild(hrn);
                        xmpl.addChild(ini);
                        
                        if(file.exists()) file.delete();
                        java.io.FileOutputStream fos = new java.io.FileOutputStream(file);
                        NoNamespaceXMLWriter writer = new NoNamespaceXMLWriter(fos);
                        writer.write(xmpl, true);
                        fos.flush();
                        fos.close();
                        
                        setAttribute("compiledFileName", file.getAbsolutePath());
                        setAttribute("compiledFileModifiedDate", String.valueOf(file.lastModified()));
                        
                    } catch(Exception e) {
                        userProperties.getOutputStream().flush();
                        userProperties.getPrintStream().println(e.toString());
                    }
                    userProperties.getOutputStream().flush();
                } catch(java.io.IOException e) {
                    System.out.println(e);
                }
            }
        };
        
        b[a.length+2] = new javax.swing.AbstractAction("Run") {
            public void actionPerformed(java.awt.event.ActionEvent a) {
                try {
                    
                    java.io.File file = getCompiledFile();
                    String precompiledModelFileName = userProperties.getProperty(userProperties.CURRENT_MODEL_FILE);
                    int i = precompiledModelFileName.lastIndexOf(userProperties.SLASH);
                    String dir = precompiledModelFileName.substring(0, i+1);
                    String name = precompiledModelFileName.substring(i+1);
                    i = name.lastIndexOf(".");
                    if(i == -1) return;
                    name = name.substring(0, i);
                    String compiledModelFileName = file.getAbsolutePath();
                    String paramsFileName = dir+name+".params";
                    
                    java.io.File params = new java.io.File(paramsFileName);
                    if(!params.exists()) {
                        java.io.PrintWriter p = new java.io.PrintWriter(new java.io.FileOutputStream(params));
                        p.println("L2SearchMethod = cbfs");
                        p.println("L2MaxCBFSCandidates = 10");
                        p.println("L2MaxCBFSSearchSpace = 1000");
                        p.println("L2MaxCBFSCutoffWeight = 6");
                        p.println("L2MaxCoverCandidateRank = 8");
                        p.println("L2MaxHistorySteps = 3");
                        p.println("L2ProgressCmdType = full");
                        p.println("L2NumTrajectoriesTracked = 10");
                        p.println("L2FindCandidatesCmdType = prune-search");
                        p.flush();
                        p.close();
                    }
                    
                    System.out.println("precompiledModelFileName "+precompiledModelFileName);
                    System.out.println("compiledModelFileName "+compiledModelFileName);
                    System.out.println("paramsFileName "+paramsFileName);
                    new oliver.application.Debugger(userProperties, precompiledModelFileName,compiledModelFileName,paramsFileName).show();
                } catch(Exception e) {
                    System.out.println(e);
                }
            }
        };
        
        b[a.length+3] = new javax.swing.AbstractAction("LMV") {
            public void actionPerformed(java.awt.event.ActionEvent a) {
                java.io.File file = getCompiledFile();
                String command = "java -classpath \""+System.getProperty("java.class.path")+"\" lmv.Application1";
                try {
                    Runtime.getRuntime().exec(command);
                } catch(Exception e) {
                    javax.swing.JOptionPane.showMessageDialog(userProperties.getRootComponent(), e.toString());
                }
            }
        };
        
        ((javax.swing.AbstractAction)b[a.length+2]).setEnabled(compiledFileUpToDate());
        return b;
    }
    
    public void compile(java.io.File save, java.io.OutputStream stdout) throws Exception {
        Xmpl root = getRoot();
        if(root instanceof Project) {
            Project p = (Project)root;
            java.io.PrintStream log = new java.io.PrintStream(stdout);
            String componentTypeName = getAttribute("name", null);
            if(componentTypeName == null) throw new Exception("ComponentType.compile() componentTypeName is null");
            java.io.ByteArrayOutputStream out = new java.io.ByteArrayOutputStream();
            java.io.PrintWriter pw = new java.io.PrintWriter(out);
            pw.write("<?xml version=\"1.0\"?>\n");
            pw.write("<xmpl>\n"); // xmlns:ci=\"http://www.w3.org/1999/XSL/Format\">\n");
            pw.flush();
            net.n3.nanoxml.XMLWriter writer = new net.n3.nanoxml.XMLWriter(out);
            writer.write(p, true);
            out.flush();
            pw.write("</xmpl>");
            pw.flush();
            pw.close();
            out.flush();
            out.close();
            
            java.io.ByteArrayInputStream in = new java.io.ByteArrayInputStream(out.toByteArray());
            
            gov.nasa.arc.l2tools.Workspace workspace=new gov.nasa.arc.l2tools.Workspace();
            gov.nasa.arc.l2tools.Project project=new gov.nasa.arc.l2tools.Project();
            project.set("name",componentTypeName);
            log.println("Loading current project into compiler.");
            gov.nasa.arc.l2tools.io.xmpl.XmplLoader loader=new gov.nasa.arc.l2tools.io.xmpl.XmplLoader(workspace,in);
            loader.load(project);
            
            //// FIND THE COMPONENT TYPE
            log.println("Finding the component type: "+componentTypeName);
            java.util.Enumeration children=((gov.nasa.arc.l2tools.Project)workspace.getChildren().firstElement()).getChildren().elements();
            gov.nasa.arc.l2tools.livingstone.ComponentType componentType = null;
            while (children.hasMoreElements()) {
                java.lang.Object child=children.nextElement();
                if (child instanceof gov.nasa.arc.l2tools.livingstone.ComponentType) {
                    if (((gov.nasa.arc.l2tools.livingstone.ComponentType)child).get("name").equals(componentTypeName)) {
                        componentType = (gov.nasa.arc.l2tools.livingstone.ComponentType)child;
                        break;
                    }
                } else if (child instanceof gov.nasa.arc.l2tools.livingstone.AttributeType) {
                    if (((gov.nasa.arc.l2tools.livingstone.AttributeType)child).get("name").equals(componentTypeName)) {
                        String errorString="The name '"+componentTypeName+"' corresponds to an attribute type, not component type.";
                        throw new gov.nasa.arc.l2tools.Exception(errorString);
                    }
                }
            }
            if (componentType==null) {
                String errorString2="The name '"+componentTypeName+"' is not defined as a type in the files supplied";
                throw new gov.nasa.arc.l2tools.Exception(errorString2);
            }
            
            //// COMPILE
            String resultName = "test";
            log.println("XmplCompiler beginning compile of component to be named: "+resultName);
            // need to redirect standard out to the display
            java.io.PrintStream saveOut = System.out;
            gov.nasa.arc.l2tools.livingstone.ComponentInstance result = null;
            try {
                System.setOut(log);
                result=componentType.buildInstance(resultName);
            } catch(Exception e) {
                System.setOut(saveOut);
                log.println("\n");
                throw e;
            }
            log.flush();
            System.setOut(saveOut);
            log.println("\n");
            log.println("XmplCompiler finished compile.");
            
            //// SAVE
            log.println("XmplCompiler writing output file: "+save.getAbsolutePath()+"\n");
            gov.nasa.arc.l2tools.io.xmpl.XmplSaver saver
            = new gov.nasa.arc.l2tools.io.xmpl.XmplSaver(save);
            saver.save(result);
        }
    }
    
    public void writeJmpl(java.io.PrintWriter writer, boolean prettyprint, int indent) throws java.io.IOException {
        writeSpaces(writer, indent);
        writer.print("class "+getAttribute("name", "######"));
        String sup = getAttribute("extends",null);
        if(sup != null) writer.print(" extends "+sup);
        writer.print(" {");
        writer.println();
        
        indent = indent + getTabSize();
        Class lastChildClass = null;
        java.util.Enumeration enum = getChildren().elements();
        while(enum.hasMoreElements()) {
            Object obj = enum.nextElement();
            if(obj instanceof Xmpl) {
                if(obj.getClass() != lastChildClass) writer.println();
                ((Xmpl)obj).writeJmpl(writer, prettyprint, indent);
                lastChildClass = obj.getClass();
            }
        }
        indent = indent - getTabSize();
        
        writeSpaces(writer, indent);
        writer.println("}");
    }
    
    private java.util.Vector removeFromThisListSubsetChildrenNamed(java.util.Vector children, String name) {
        java.util.Vector myChildrenThatHaveSubsets = new java.util.Vector();
        java.util.Enumeration enum1 = getChildrenNamed(name).elements();
        while(enum1.hasMoreElements()) {
            Object obj1 = enum1.nextElement();
            if(obj1 instanceof Xmpl) {
                Xmpl myChild = (Xmpl)obj1;
                java.util.Enumeration enum2 = children.elements();
                while(enum2.hasMoreElements()) {
                    Object obj2 = enum2.nextElement();
                    if(obj2 instanceof Xmpl) {
                        Xmpl next = (Xmpl)obj2;
                        if(next.isSubsetOf(myChild)) {
                            children.remove(next);
                            myChildrenThatHaveSubsets.addElement(myChild);
                            break;
                        }
                    }
                }
            }
        }
        return myChildrenThatHaveSubsets;
    }
    
    public void setXmpl(Xmpl n) throws Exception {
        super.setXmpl(n);
        java.util.Vector whoAmIGoingToClobber = new java.util.Vector(n.getChildren());
        java.util.Vector myChildrenThatHaveSubsets = removeFromThisListSubsetChildrenNamed(whoAmIGoingToClobber, "ct:attribute");
        numStateVectors = -1;
        // I have to clone the vector or it f**ks up my iterator
        java.util.Vector myChildren = new java.util.Vector(getChildren());
        java.util.Iterator enum = myChildren.iterator();
        while(enum.hasNext()) {
            Object mychild = enum.next();
            if(mychild instanceof Xmpl && !myChildrenThatHaveSubsets.contains(mychild)) {
                // remove all the values, but nothing else
                ((Xmpl)mychild).removeXmplFromParent(this);
            }
        }
        enum = new java.util.Vector(n.getChildren()).iterator();
        while(enum.hasNext()) {
            net.n3.nanoxml.IXMLElement nchild = (net.n3.nanoxml.IXMLElement)enum.next();
            if(nchild instanceof Xmpl && whoAmIGoingToClobber.contains(nchild)) {
                int index = getChildrenCount();
                insertXmplChild(this, (Xmpl)nchild, index);
            }
        }
    }
    
    
}

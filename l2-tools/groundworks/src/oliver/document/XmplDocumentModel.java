// 
// * See the file "l2-tools/disclaimers-and-notices.txt" for 
// * information on usage and redistribution of this file, 
// * and for a DISCLAIMER OF ALL WARRANTIES. 
// 

/*
 * XmplDocumentModel.java
 *
 * Created on September 13, 2002, 11:23 AM
 */

package oliver.document;

import oliver.xmpl.*;

/**
 *
 * @author  stephen
 */
public class XmplDocumentModel extends javax.swing.text.DefaultStyledDocument {
    
    private java.util.Properties userProperties;
    private Xmpl root = new Xmpl();
    private boolean xmplReflectsText = true;
    private Xmpl selectedNode = null;
    
    public XmplDocumentModel(java.util.Properties userProperties) {
        // I'll have to make temp files to comile this stuff,
        // the properties have the location for temp files.
        this.userProperties = userProperties;
    }
    
    public void readXmplFile(java.io.File file) throws Exception {
        net.n3.nanoxml.StdXMLBuilder builder = new net.n3.nanoxml.StdXMLBuilder(new Xmpl());
        net.n3.nanoxml.IXMLParser parser = net.n3.nanoxml.XMLParserFactory.createDefaultXMLParser(builder);
        net.n3.nanoxml.IXMLReader reader = new net.n3.nanoxml.StdXMLReader(new java.io.FileReader(file));
        parser.setReader(reader);
        net.n3.nanoxml.IXMLElement xml = (net.n3.nanoxml.IXMLElement) parser.parse();
        Xmpl project = findProjectNode(xml);
        if(project == null) {
            System.out.println("XmplDocumentModel.readXmplFile() no project in file");
        } else {
            this.root = project;
        }
    }
    
    private void setText(Xmpl xmpl) {
        try {
            remove(0, getLength());
            java.io.ByteArrayOutputStream out = new java.io.ByteArrayOutputStream();
            JmplWriter writer = new JmplWriter(out);
            writer.write(xmpl);
            writer.flush();
            insertString(0,out.toString(),new javax.swing.text.SimpleAttributeSet());
        } catch(Exception e) {
            System.out.println(e);
        }
    }
    
    protected Project findProjectNode(net.n3.nanoxml.IXMLElement xml) {
        while(xml != null) {
            if(xml instanceof Project) return (Project)xml;
            if(xml.getChildrenCount() > 0) {
                xml = (net.n3.nanoxml.IXMLElement)xml.getChildren().get(0);
            } else {
                return null;
            }
        }
        return null;
    }
    
    /** just need to know when changes have been made to the text */
    public void insertString(int offs, String str, javax.swing.text.AttributeSet a)
    throws javax.swing.text.BadLocationException {
        super.insertString(offs, str, a);
        xmplReflectsText = false;
    }
    
    public class OhWhataHack extends java.io.OutputStream {
        private StringBuffer data = new StringBuffer();
        public void write(int b) {
            data.append((char)b);
        }
        public int getLength() {
            return data.length();
        }
        public String toString() {
            return data.toString();
        }
    }
    
    /** Parses the current document and updates the node if everything is okay.
     *  Source xmpl file saved as .xmpl file. */
    public Object parseXmpl(oliver.xmpl.Xmpl node, java.io.PrintStream stdout) throws Exception {
        // setup system out to cach to this stream
        java.io.PrintStream err = System.err;
        OhWhataHack hack = new OhWhataHack();
        System.setErr(new java.io.PrintStream(hack));
        
        String name = node.getAttribute("name","temp");
        //StringBuffer fn = new StringBuffer(userProperties.getProperty("currentDirectory"));
        //fn.append(System.getProperty("file.separator"));
        //fn.append(name);
        
        //java.io.File file = new java.io.File(fn.toString()+".jmpl");
        //if(file.exists()) file.delete();
        //java.io.FileWriter out = new java.io.FileWriter(file);
        java.io.StringWriter jmplout = new java.io.StringWriter();
        jmplout.write(getText(0,getLength()));
        jmplout.flush();
        jmplout.close();
        java.io.StringReader jmplin = new java.io.StringReader(jmplout.getBuffer().toString());
        
        gov.nasa.arc.l2tools.Workspace workspace=new gov.nasa.arc.l2tools.Workspace();
        
        // LOAD
        gov.nasa.arc.l2tools.Project project=new gov.nasa.arc.l2tools.Project();
        project.set("name",name);
        gov.nasa.arc.l2tools.io.jmpl.JmplLoader loader=new gov.nasa.arc.l2tools.io.jmpl.JmplLoader(workspace,jmplin);
        loader.load(project);
        
        OhWhataHack xmplout = new OhWhataHack();
        
        // SAVE
        gov.nasa.arc.l2tools.io.xmpl.XmplSaver saver = new gov.nasa.arc.l2tools.io.xmpl.XmplSaver(xmplout);
        saver.save(project);
        saver.getWriter().close();
        
        System.err.flush();
        System.setErr(err);
        if(hack.getLength() > 0) throw new Exception(hack.toString());
        
        java.io.StringReader xmplin = new java.io.StringReader(xmplout.toString());
        
        // if everything worked this is the new element.
        net.n3.nanoxml.StdXMLBuilder builder = new net.n3.nanoxml.StdXMLBuilder(new oliver.xmpl.Xmpl());
        net.n3.nanoxml.IXMLParser parser = net.n3.nanoxml.XMLParserFactory.createDefaultXMLParser(builder);
        net.n3.nanoxml.IXMLReader reader = new net.n3.nanoxml.StdXMLReader(xmplin);
        parser.setReader(reader);
        net.n3.nanoxml.IXMLElement xml = (net.n3.nanoxml.IXMLElement) parser.parse();
                
        while(xml != null && !(xml instanceof oliver.xmpl.Xmpl)) {
            java.util.Vector children = xml.getChildren();
            if(children.size() == 0) xml = null;
            else xml = (net.n3.nanoxml.IXMLElement)xml.getChildren().firstElement();
        }
        
        Object obj = xml.getChildren().firstElement();
        return obj;
    }
    /*
    private void updateXmpl(java.io.PrintStream stdout) throws Exception {
        java.io.StringWriter out = new java.io.StringWriter();
        out.write(getText(0,getLength()));
        out.flush();
        out.close();
        compile(out, stdout);
        
        java.io.StringReader in = new java.io.StringReader(out.toString());
        // No exception was thrown, I assume everything is okay.
        // Update the model
        net.n3.nanoxml.StdXMLBuilder builder = new net.n3.nanoxml.StdXMLBuilder(new Xmpl());
        net.n3.nanoxml.IXMLParser parser = net.n3.nanoxml.XMLParserFactory.createDefaultXMLParser(builder);
        net.n3.nanoxml.IXMLReader reader = new net.n3.nanoxml.StdXMLReader(in);
        parser.setReader(reader);
        net.n3.nanoxml.IXMLElement xml = (net.n3.nanoxml.IXMLElement) parser.parse();
        
        root.setXmpl((Xmpl)xml);
    }
    */
    public void compile(java.io.Writer save, java.io.PrintStream log) throws Exception {
        String componentTypeName = root.getAttribute("name", null);
        if(componentTypeName == null) throw new Exception("XmplDocumentModel.compile() componentTypeName is null");
        java.io.ByteArrayOutputStream out = new java.io.ByteArrayOutputStream();
        java.io.PrintWriter pw = new java.io.PrintWriter(out);
        //pw.write("<?xml version=\"1.0\"?>\n");
        //pw.write("<xmpl xmlns:ci=\"http://www.w3.org/1999/XSL/Format\">\n");
        //pw.flush();
        net.n3.nanoxml.XMLWriter writer = new net.n3.nanoxml.XMLWriter(out);
        writer.write(root, true);
        //pw.write("</xmpl>");
        //pw.flush();
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
            result=componentType.buildInstance(resultName); // andrew's function
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
        log.println("XmplCompiler writing output.\n");
        gov.nasa.arc.l2tools.io.xmpl.XmplSaver saver = new gov.nasa.arc.l2tools.io.xmpl.XmplSaver(new gov.nasa.arc.l2tools.io.Writer(save));
        saver.save(result);
    }
    
}

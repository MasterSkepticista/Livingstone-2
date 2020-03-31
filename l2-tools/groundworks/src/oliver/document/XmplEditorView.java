// 
// * See the file "l2-tools/disclaimers-and-notices.txt" for 
// * information on usage and redistribution of this file, 
// * and for a DISCLAIMER OF ALL WARRANTIES. 
// 

/*
 * XmplEditorView.java
 *
 * Created on September 23, 2002, 3:59 PM
 */

package oliver.document;

/**
 *
 * @author  stephen
 */
public class XmplEditorView extends javax.swing.JEditorPane implements oliver.project.ProjectListener {
    
    private oliver.application.UserProperties userProperties;
    private oliver.project.ProjectModel model;
    private oliver.xmpl.Xmpl node;
    
    /** Creates a new instance of XmplEditorView */
    public XmplEditorView(oliver.application.UserProperties userProperties) {
        super();
        this.userProperties = userProperties;
        
        setDocument(new XmplDocumentModel(userProperties));
        setFont(new java.awt.Font("Courier New", 0, 12));
        
        addFocusListener(new java.awt.event.FocusAdapter() {
            public void focusLost(java.awt.event.FocusEvent evt) {
                formFocusLost(evt);
            }
        });
    }
    
    private void formFocusLost(java.awt.event.FocusEvent evt) {
        // Add your handling code here:
        if(node != null) {
            Thread thread = new Thread(new Runnable() {
                public void run() {
                    try {
                        Object obj = ((XmplDocumentModel)getDocument()).parseXmpl(node, userProperties.getPrintStream());
                        if(obj instanceof oliver.xmpl.Xmpl) {
                            // update the existing tree
                            node.setXmpl((oliver.xmpl.Xmpl)obj);
                        }
                    } catch(Exception e) {
                        javax.swing.JOptionPane.showConfirmDialog(userProperties.getRootComponent(),
                        e.toString(), "Parsing Error",
                        javax.swing.JOptionPane.CLOSED_OPTION, javax.swing.JOptionPane.ERROR_MESSAGE);
                        grabFocus();
                    }
                }
            });
            thread.start();
        }
    }
    
    public void setModel(oliver.project.ProjectModel model) {
        this.model = model;
    }
    
    /** An undoable edit happened
     *
     */
    public void undoableEditHappened(javax.swing.event.UndoableEditEvent e) {
        if(!(e.getEdit() instanceof oliver.project.ProjectModelUndoEvent)) return;
        oliver.project.ProjectModelUndoEvent event = (oliver.project.ProjectModelUndoEvent)e.getEdit();
        
        if(event.getType() == event.EVERYTHING_CHANGED) {
            try {
                node = null;
                getDocument().remove(0, getDocument().getLength());
            } catch(javax.swing.text.BadLocationException ee) {
                System.out.println(ee);
            }
            
        } else if(event.getType() == event.NODE_SELECTED) {
            node = event.getParent();
            if(node instanceof oliver.xmpl.ComponentType || node instanceof oliver.xmpl.AttributeType) {
                this.node = node;
                try {
                    getDocument().remove(0, getDocument().getLength());
                    java.io.ByteArrayOutputStream out = new java.io.ByteArrayOutputStream();
                    oliver.xmpl.JmplWriter writer = new oliver.xmpl.JmplWriter(out);
                    writer.write(node);
                    writer.flush();
                    getDocument().insertString(0,out.toString(),new javax.swing.text.SimpleAttributeSet());
                } catch(Exception ee) {
                    System.out.println(ee);
                }
            }
        } else if(event.getType() == event.NODES_INSERTED || event.getType() == event.NODES_REMOVED) {
            oliver.xmpl.Xmpl n = event.getParent();
            if(n != null && n==node) {
                if(n instanceof oliver.xmpl.ComponentType || n instanceof oliver.xmpl.AttributeType) {
                    this.node = n;
                    try {
                        getDocument().remove(0, getDocument().getLength());
                        java.io.ByteArrayOutputStream out = new java.io.ByteArrayOutputStream();
                        oliver.xmpl.JmplWriter writer = new oliver.xmpl.JmplWriter(out);
                        writer.write(node);
                        writer.flush();
                        getDocument().insertString(0,out.toString(),new javax.swing.text.SimpleAttributeSet());
                    } catch(Exception ee) {
                        System.out.println(ee);
                    }
                }
            }
        }
    }
}

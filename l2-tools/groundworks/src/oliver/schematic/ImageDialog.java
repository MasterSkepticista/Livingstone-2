// 
// * See the file "l2-tools/disclaimers-and-notices.txt" for 
// * information on usage and redistribution of this file, 
// * and for a DISCLAIMER OF ALL WARRANTIES. 
// 

/*
 * ImageDialog.java
 *
 * Created on December 9, 2002, 5:28 PM
 */

package oliver.schematic;

/**
 *
 * @author  stephen
 */
public class ImageDialog extends javax.swing.JDialog {
    
    private oliver.application.UserProperties userProperties;
    private net.n3.nanoxml.IXMLElement type = new net.n3.nanoxml.XMLElement();
    
    /** Creates new form ImageDialog */
    public ImageDialog(oliver.application.UserProperties userProperties, boolean modal) {
        super(userProperties.getFrame(), modal);
        this.userProperties = userProperties;
        initComponents();
        stuffIcons();
    }
    
    /** Creates new form ImageDialog */
    public ImageDialog(oliver.application.UserProperties userProperties, net.n3.nanoxml.IXMLElement type, boolean modal) {
        super(userProperties.getFrame(), modal);
        this.userProperties = userProperties;
        this.type = type;
        initComponents();
        stuffIcons();
        net.n3.nanoxml.IXMLElement n = getScriptNode(this.type);
        localEditorPane.setText(n.getAttribute("src", null));
        useGlobalScriptCheckBox.setSelected(Boolean.valueOf(n.getAttribute("useglobalscript",Boolean.toString(true))).booleanValue());
        globalEditorPane.setText(getGlobalScriptNode(this.type).getAttribute("src", null));
    }
    
    public void insertText(String text) {
        javax.swing.text.Document doc = localEditorPane.getDocument();
        try {
            doc.insertString(localEditorPane.getCaret().getDot(), text, new javax.swing.text.SimpleAttributeSet());
        } catch(Exception e) {
            System.out.println(e);
        }
    }
    
    private void stuffIcons() {
        String iconDirName = userProperties.getProperty(userProperties.IMAGE_DIRECTORY);
        java.io.File dir = new java.io.File(iconDirName);
        if(!dir.exists() || !dir.isDirectory()) return;
        java.io.File[] imageFiles = dir.listFiles(new java.io.FileFilter() {
            public boolean accept(java.io.File file) {
                if(file.getName().endsWith(".jpg")) return true;
                else if(file.getName().endsWith(".JPG")) return true;
                else if(file.getName().endsWith(".gif")) return true;
                else if(file.getName().endsWith(".GIF")) return true;
                else return false;
            }
        });
        if(imageFiles.length > 0) {
            java.awt.MediaTracker tracker = new java.awt.MediaTracker(ImageDialog.this);
            for(int i = 0; i < imageFiles.length; i++) {
                java.awt.Image icon = getToolkit().createImage(imageFiles[i].getAbsolutePath());
                tracker.addImage(icon, i);
                javax.swing.JButton tb = new javax.swing.JButton(new javax.swing.ImageIcon(icon));
                tb.setText(imageFiles[i].getName());
                tb.setHorizontalTextPosition(tb.CENTER);
                tb.setVerticalTextPosition(tb.BOTTOM);
                tb.addActionListener(new java.awt.event.ActionListener() {
                    public void actionPerformed(java.awt.event.ActionEvent e) {
                        javax.swing.JButton tb2 = (javax.swing.JButton)e.getSource();
                        ImageDialog.this.insertText(tb2.getText());
                    }
                });
                iconsPanel.add(tb);
            }
            try {
                tracker.waitForAll();
            } catch(Exception e) {}
        }
    }
    
    public static net.n3.nanoxml.IXMLElement getScriptNode(net.n3.nanoxml.IXMLElement type) {
        net.n3.nanoxml.IXMLElement node = type.getFirstChildNamed("gr:image");
        if(node == null) {
            node = new net.n3.nanoxml.XMLElement("gr:image");
            node.setAttribute("src", "\"default.gif\";");
            type.addChild(node);
        }
        return node;
    }
    
    public static net.n3.nanoxml.IXMLElement getGlobalScriptNode(net.n3.nanoxml.IXMLElement type) {
        net.n3.nanoxml.IXMLElement project = type.getParent();
        while(project != null && !project.getName().equals("project")) project = project.getParent();
        if(project == null) return null;
        net.n3.nanoxml.IXMLElement node = project.getFirstChildNamed("gr:globalImageJavascript");
        if(node == null) {
            node = new net.n3.nanoxml.XMLElement("gr:globalImageJavascript");
            StringBuffer src = new StringBuffer();
            src.append("var key = \"pink\";\n");
            src.append("if(state != null) {\n");
            src.append("var mode = state.get(\"test.\"+instance.name+\".mode\");\n");
            src.append("if(mode != null) {\n");
            src.append("if(/nominal/i.test(mode)) key = \"PaleGreen3\";\n");
            src.append("else if(/fault/i.test(mode)||/failed/i.test(mode)||/stuck/i.test(mode)||/regulatingHigh/i.test(mode)||/regulatingLow/i.test(mode)) key = \"red\";\n");
            src.append("else if(/open/i.test(mode)) key = \"SeaGreen\";\n");
            src.append("else if(/closed/i.test(mode)) key = \"grey50\";\n");
            src.append("else if(/biased/i.test(mode)) key = \"orange\";\n");
            src.append("else if(/dry/i.test(mode)||/empty/i.test(mode)) key = \"brown\";\n");
            src.append("else if(/full/i.test(mode)||/low/i.test(mode)||/wet/i.test(mode)) key = \"blue\";\n");
            src.append("else if(/high/i.test(mode)||/thrusting/i.test(mode)) key = \"yellow\";\n");
            src.append("else if(/ok/i.test(mode)||/quiescent/i.test(mode)||/zero/i.test(mode)) key = \"gray90\";\n");
            src.append("else if(/positive/i.test(mode)) key = \"khaki1\";\n");
            src.append("else if(/negative/i.test(mode)||/filling/i.test(mode)) key = \"SkyBlue\";\n");
            src.append("else if(/emptying/i.test(mode)) key = \"tan\";\n");
            src.append("}\n");
            src.append("}\n");
            node.setAttribute("src", src.toString());
            project.addChild(node);
        }
        return node;
    }
    
    
    private void test() throws Exception {
        org.mozilla.javascript.Context cx = org.mozilla.javascript.Context.enter();
        org.mozilla.javascript.Scriptable scope = cx.initStandardObjects(null);
        
        org.mozilla.javascript.ScriptableObject.defineClass(scope, oliver.scenario.ScriptableHashMap.class);
        
        String[] arg = { "key", "value" };
        org.mozilla.javascript.Scriptable myHash = cx.newObject(scope, "ScriptableHashMap", arg);
        scope.put("myHash", scope, myHash);
        
        String src = localEditorPane.getText();
        Object result = cx.evaluateString(scope, src, "<cmd>", 1, null);
        net.n3.nanoxml.IXMLElement scriptNode = getScriptNode(this.type);
        scriptNode.setAttribute("src", src);
        System.out.println(cx.toString(result));
    }
    
    /** This method is called from within the constructor to
     * initialize the form.
     * WARNING: Do NOT modify this code. The content of this method is
     * always regenerated by the Form Editor.
     */
    private void initComponents() {//GEN-BEGIN:initComponents
        buttonsPanel = new javax.swing.JPanel();
        okayButton = new javax.swing.JButton();
        cancelButton = new javax.swing.JButton();
        useGlobalScriptCheckBox = new javax.swing.JCheckBox();
        iconsSrollPane = new javax.swing.JScrollPane();
        iconsPanel = new javax.swing.JPanel();
        jTabbedPane1 = new javax.swing.JTabbedPane();
        localScrollPane = new javax.swing.JScrollPane();
        localEditorPane = new javax.swing.JEditorPane();
        globalScrollPane = new javax.swing.JScrollPane();
        globalEditorPane = new javax.swing.JEditorPane();

        addWindowListener(new java.awt.event.WindowAdapter() {
            public void windowClosing(java.awt.event.WindowEvent evt) {
                closeDialog(evt);
            }
        });

        okayButton.setText("OK");
        okayButton.setToolTipText("Cancel");
        okayButton.addActionListener(new java.awt.event.ActionListener() {
            public void actionPerformed(java.awt.event.ActionEvent evt) {
                okayButtonActionPerformed(evt);
            }
        });

        buttonsPanel.add(okayButton);

        cancelButton.setText("Cancel");
        cancelButton.addActionListener(new java.awt.event.ActionListener() {
            public void actionPerformed(java.awt.event.ActionEvent evt) {
                cancelButtonActionPerformed(evt);
            }
        });

        buttonsPanel.add(cancelButton);

        useGlobalScriptCheckBox.setSelected(true);
        useGlobalScriptCheckBox.setText("use global script");
        buttonsPanel.add(useGlobalScriptCheckBox);

        getContentPane().add(buttonsPanel, java.awt.BorderLayout.SOUTH);

        iconsSrollPane.setViewportView(iconsPanel);

        getContentPane().add(iconsSrollPane, java.awt.BorderLayout.NORTH);

        localScrollPane.setViewportView(localEditorPane);

        jTabbedPane1.addTab("local", localScrollPane);

        globalScrollPane.setViewportView(globalEditorPane);

        jTabbedPane1.addTab("global", globalScrollPane);

        getContentPane().add(jTabbedPane1, java.awt.BorderLayout.CENTER);

        pack();
        java.awt.Dimension screenSize = java.awt.Toolkit.getDefaultToolkit().getScreenSize();
        setSize(new java.awt.Dimension(400, 300));
        setLocation((screenSize.width-400)/2,(screenSize.height-300)/2);
    }//GEN-END:initComponents
    
    private void cancelButtonActionPerformed(java.awt.event.ActionEvent evt) {//GEN-FIRST:event_cancelButtonActionPerformed
        // Add your handling code here:
        closeDialog(null);
    }//GEN-LAST:event_cancelButtonActionPerformed
    
    private void okayButtonActionPerformed(java.awt.event.ActionEvent evt) {//GEN-FIRST:event_okayButtonActionPerformed
        // Add your handling code here:
        try {
            net.n3.nanoxml.IXMLElement n = getScriptNode(this.type);
            n.setAttribute("src",localEditorPane.getText());
            n.setAttribute("useglobalscript", String.valueOf(useGlobalScriptCheckBox.isSelected()));
            getGlobalScriptNode(this.type).setAttribute("src", globalEditorPane.getText());
        } catch(Exception e) {
            javax.swing.JOptionPane.showMessageDialog(this, e.getLocalizedMessage());
        }
        closeDialog(null);
    }//GEN-LAST:event_okayButtonActionPerformed
    
    /** Closes the dialog */
    private void closeDialog(java.awt.event.WindowEvent evt) {//GEN-FIRST:event_closeDialog
        setVisible(false);
        dispose();
    }//GEN-LAST:event_closeDialog
    
    
    // Variables declaration - do not modify//GEN-BEGIN:variables
    private javax.swing.JScrollPane iconsSrollPane;
    private javax.swing.JScrollPane globalScrollPane;
    private javax.swing.JPanel buttonsPanel;
    private javax.swing.JCheckBox useGlobalScriptCheckBox;
    private javax.swing.JEditorPane localEditorPane;
    private javax.swing.JScrollPane localScrollPane;
    private javax.swing.JButton cancelButton;
    private javax.swing.JEditorPane globalEditorPane;
    private javax.swing.JButton okayButton;
    private javax.swing.JPanel iconsPanel;
    private javax.swing.JTabbedPane jTabbedPane1;
    // End of variables declaration//GEN-END:variables
    
}

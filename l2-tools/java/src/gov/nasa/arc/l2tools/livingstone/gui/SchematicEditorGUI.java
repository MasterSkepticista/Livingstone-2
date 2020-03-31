//
// * See the file "l2-tools/disclaimers-and-notices.txt" for 
// * information on usage and redistribution of this file, 
// * and for a DISCLAIMER OF ALL WARRANTIES.
//
package gov.nasa.arc.l2tools.livingstone.gui;

/**
 * Creation date: (10/20/2000 10:06:58 AM)
 * @author: Andrew Bachmann
 */
public class SchematicEditorGUI extends ComponentTypeGUI {
/**
 * SchematicEditorGUI constructor comment.
 */
public SchematicEditorGUI() {
	this(new gov.nasa.arc.l2tools.swing.Frame("SchematicEditorGUI"),new gov.nasa.arc.l2tools.livingstone.ComponentType());
}
/**
 * SchematicEditorGUI constructor comment.
 * @param componentType gov.nasa.arc.l2tools.livingstone.ComponentType
 */
public SchematicEditorGUI(gov.nasa.arc.l2tools.livingstone.ComponentType componentType) {
	this(new gov.nasa.arc.l2tools.swing.Frame("SchematicEditorGUI"),componentType);
}
/**
 * SchematicEditorGUI constructor comment.
 * @param view gov.nasa.arc.l2tools.swing.RootPaneContainer
 */
public SchematicEditorGUI(gov.nasa.arc.l2tools.swing.RootPaneContainer view) {
	this(view,new gov.nasa.arc.l2tools.livingstone.ComponentType());
}
/**
 * SchematicEditorGUI constructor comment.
 * @param view gov.nasa.arc.l2tools.swing.RootPaneContainer
 * @param componentType gov.nasa.arc.l2tools.livingstone.ComponentType
 */
public SchematicEditorGUI(gov.nasa.arc.l2tools.swing.RootPaneContainer view, gov.nasa.arc.l2tools.livingstone.ComponentType componentType) {
	super(view, componentType);
}
}

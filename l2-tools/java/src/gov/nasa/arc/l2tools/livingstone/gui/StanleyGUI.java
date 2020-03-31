//
// * See the file "l2-tools/disclaimers-and-notices.txt" for 
// * information on usage and redistribution of this file, 
// * and for a DISCLAIMER OF ALL WARRANTIES.
//
package gov.nasa.arc.l2tools.livingstone.gui;

import gov.nasa.arc.l2tools.swing.AbstractAction;
import gov.nasa.arc.l2tools.swing.Menu;
import gov.nasa.arc.l2tools.swing.MenuItem;
import java.awt.event.KeyEvent;
import javax.swing.KeyStroke;
import java.awt.Event;
/**
 * Creation date: (10/20/2000 10:09:18 AM)
 * @author: Andrew Bachmann
 */
public class StanleyGUI extends SchematicEditorGUI {
	private class MenuBar extends gov.nasa.arc.l2tools.swing.MenuBar {
		public MenuBar() {
			super();
			setName("MenuBar");
			createStanleyMenu();
		}
		public void createStanleyMenu() {
			// create it
			Menu menu = new Menu("Stanley");
			menu.setMnemonic('S');
			add(menu);
			// New Project
			MenuItem stanleyStart=menu.add(actions.stanleyStart);
			stanleyStart.setMnemonic(KeyEvent.VK_R);
			stanleyStart.setAccelerator(KeyStroke.getKeyStroke(KeyEvent.VK_S,Event.CTRL_MASK));
		}
	}
	private class ToolBar extends gov.nasa.arc.l2tools.swing.ToolBar {
		public ToolBar() {
			super();
			setName("ToolBar");
		}
	}
	Actions actions=new Actions();
	public class Actions {
		AbstractAction stanleyStart = new AbstractAction("Start Stanley") {
			public void actionPerformed(java.awt.event.ActionEvent event) {
				eventStanleyStart();
			}
		};
		public Actions() {
			super();
		}
	}
/**
 * StanleyGUI constructor comment.
 */
public StanleyGUI() {
	this(new gov.nasa.arc.l2tools.swing.Frame("StanleyGUI"),new gov.nasa.arc.l2tools.livingstone.ComponentType());
}
/**
 * StanleyGUI constructor comment.
 * @param componentType gov.nasa.arc.l2tools.livingstone.ComponentType
 */
public StanleyGUI(gov.nasa.arc.l2tools.livingstone.ComponentType componentType) {
	this(new gov.nasa.arc.l2tools.swing.Frame("StanleyGUI"),componentType);
}
/**
 * StanleyGUI constructor comment.
 * @param view gov.nasa.arc.l2tools.swing.RootPaneContainer
 */
public StanleyGUI(gov.nasa.arc.l2tools.swing.RootPaneContainer view) {
	this(view,new gov.nasa.arc.l2tools.livingstone.ComponentType());
}
/**
 * StanleyGUI constructor comment.
 * @param view gov.nasa.arc.l2tools.swing.RootPaneContainer
 * @param componentType gov.nasa.arc.l2tools.livingstone.ComponentType
 */
public StanleyGUI(gov.nasa.arc.l2tools.swing.RootPaneContainer view, gov.nasa.arc.l2tools.livingstone.ComponentType componentType) {
	super(view, componentType);
	getView().setToolBar(new ToolBar());
	if (getView() instanceof gov.nasa.arc.l2tools.swing.InternalFrame) {
		((gov.nasa.arc.l2tools.swing.InternalFrame)getView()).setJMenuBar(new MenuBar());
		((gov.nasa.arc.l2tools.swing.InternalFrame)getView()).setSize(300,75);
	} else if (getView() instanceof gov.nasa.arc.l2tools.swing.Frame) {
		((gov.nasa.arc.l2tools.swing.Frame)getView()).setJMenuBar(new MenuBar());
		((gov.nasa.arc.l2tools.swing.Frame)getView()).setSize(300,75);
	}
}
public void eventStanleyStart() {
	
}
}

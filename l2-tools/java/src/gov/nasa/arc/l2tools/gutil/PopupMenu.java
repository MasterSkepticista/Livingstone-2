//
// * See the file "l2-tools/disclaimers-and-notices.txt" for 
// * information on usage and redistribution of this file, 
// * and for a DISCLAIMER OF ALL WARRANTIES.
//
/*
* Author : Mark Shirley
* Description : PopupMenu.java
*
* Factory class for popup menus for presentations
*
* I'm not happy with this.  I'd like to simplify it by using the Java
* conventions rather than the Smalltalk convention.  I'd like the menu
* item callbacks to have access to the object the popup menu was
* originally called on.  One of the goals of this abstraction is to make
* that available.  However, the callbacks are generally inner classes
* that have access to that object because it's the instance of the outer
* class.  I don't need this abstraction to do that for me.  Passing the
* popup menu down rather than a vector would allow more interesting
* initialization of the method.  It wouldn't easily allow removal of
* items, but that's not critical.  Later.
*
* CHANGELOG:
* 11/02/99 Mhs Created */

package gov.nasa.arc.l2tools.gutil;

import gov.nasa.arc.l2tools.util.EventLog;
import java.awt.Component;
import java.awt.event.ActionEvent;
import java.awt.event.ActionListener;
import java.lang.Class;
import java.lang.reflect.Method;
import java.util.Vector;
import javax.swing.JComponent;
import javax.swing.JMenuItem;
import javax.swing.JPopupMenu;


/* A convenience class that simplifies popping up a menu.  This is patterned
   after one of Smalltalk's menu class methods. */
public class PopupMenu implements ActionListener {
  Vector popupMenuItems;
  Component argument;
  
  /** Prevent instantion outside the factory methods. */
  private PopupMenu() {};

  /** Used by the factor method. */
  private PopupMenu(Vector popupMenuItems, Component argument) {
    super();
    this.popupMenuItems = popupMenuItems;
    this.argument = argument;
  }

  /** Create a popup menu for a presentation, p, at point X,Y over the
    component.  All menu items invoke methods on the presentation.  The
    container argument is the GUI component containing the presentation.
    It'll be passed to the handler for the menu item as the only callback
    argument. */
  public static void popup(Presentation p, Component container, Component component, int x, int y) {
    //System.out.println("In popup container="+container);
    Vector v = new Vector();
    p.addPopupMenuItems(container, v);
    PopupMenu.popup("title", v, container, component, x, y);
  }

  /** Create and show a popup menu.  If an item is selected, call the
    corresponding method on target giving it argument as its single
    argument.  Right now, there's no notification of the user clicks away
    of the menu to deexpose it.  If argument==null, then no argument is
    passed. */
  public static void popup(String title, 
			   Vector popupMenuItems,
			   Component argument,
			   Component component, // Pop up over this.
			   int x,
			   int y) {
    JPopupMenu menu = getMenu(title,popupMenuItems,argument);
    if (menu!=null) menu.show(component,x,y);
  }

  /** Create and return a menu configured to call methods on target giving
    them one argument.  */
  public static JPopupMenu getMenu(String title,
				   Vector popupMenuItems,
				   Component argument) {

    if (popupMenuItems.size()==0) {
      EventLog.report(new Throwable("Empty popup menu requested"));
      return null;
    }

    PopupMenu listener = new PopupMenu(popupMenuItems, argument);
    javax.swing.JPopupMenu popup = new javax.swing.JPopupMenu(title);
    int max = popupMenuItems.size();
    for (int i=0;i<max;i++) {
      PopupMenuItem item = (PopupMenuItem)popupMenuItems.elementAt(i);
      if (item instanceof PopupMenuItemSeparator) {
	popup.addSeparator();
      } else {
	JMenuItem menuItem = new JMenuItem(item.label);
	//PopupMenuCallback callback = ((PopupMenuItem)popupMenuItems.elementAt(i)).callback;
	menuItem.addActionListener(listener);
	popup.add(menuItem);
      }
    }
    return popup;
  }

  public void actionPerformed(ActionEvent e) {
    String command = e.getActionCommand();
    int i = findIndex(popupMenuItems,command);
    if (i<0) {
      EventLog.report(new Throwable("couldn't find label in actionPerformed"));
      return;
    }
    PopupMenuItem item = (PopupMenuItem)popupMenuItems.elementAt(i);
    item.callback.callback(argument);
  }
  
  protected int findIndex(Vector items, String s) {
    if (s==null || items==null) return -1;
    int max = items.size();
    for (int i=0;i<max;i++)
      if (s.equals(((PopupMenuItem)items.elementAt(i)).label)) return i;
    return -1;
  }

}

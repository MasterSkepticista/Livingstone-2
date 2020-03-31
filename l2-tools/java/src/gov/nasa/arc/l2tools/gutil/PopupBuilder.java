//
// * See the file "l2-tools/disclaimers-and-notices.txt" for 
// * information on usage and redistribution of this file, 
// * and for a DISCLAIMER OF ALL WARRANTIES.
//
/*
* Author : Mark Shirley
* Description : PopupBuilder.java
*
* This provides an alternative interface to Presentable for objects that want
* to build their own menus.  They still have to provide the labels and methods,
* although those arrays can be empty.
*
* CHANGELOG:
* 11/02/99      Mhs     Created
*/

package gov.nasa.arc.l2tools.gutil;

import javax.swing.JPopupMenu;

/** This provides an alternative interface to Presentable for objects that want
  * to build their own menus.  They still have to provide the labels and methods,
  * although those arrays can be empty. */
public interface PopupBuilder {
  public JPopupMenu getMenu();
}

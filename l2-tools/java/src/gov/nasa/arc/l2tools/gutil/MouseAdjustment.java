//
// * See the file "l2-tools/disclaimers-and-notices.txt" for 
// * information on usage and redistribution of this file, 
// * and for a DISCLAIMER OF ALL WARRANTIES.
//
/*
* Author : Mark Shirley
* filename: MouseAdjustment.java
*
* CHANGELOG:
* 04/05/00      Mhs     Created
*/

package gov.nasa.arc.l2tools.gutil;

/** an interface for a continuous adjustment.  This is the twin to
    MouseScalarAdjustment */
public interface MouseAdjustment {
    public void addMouseAdjustmentListener(MouseAdjustmentListener source);
    public void deleteMouseAdjustmentListener(MouseAdjustmentListener source);
    public void deleteMouseAdjustmentListeners();
}

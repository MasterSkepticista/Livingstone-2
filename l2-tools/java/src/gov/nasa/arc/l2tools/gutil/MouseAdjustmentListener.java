//
// * See the file "l2-tools/disclaimers-and-notices.txt" for 
// * information on usage and redistribution of this file, 
// * and for a DISCLAIMER OF ALL WARRANTIES.
//
/*
* Author : Mark Shirley
* filename: MouseAdjustmentListener.java
*
* CHANGELOG:
* 04/05/00      Mhs     Created
*/

package gov.nasa.arc.l2tools.gutil;

/** an interface for a continuous adjustment.  There probably is an
    interface like this already somwhere, but I cant' find it.  This
    isn't Observable because I need to distinguish between the start and
    the stop and I don't want to pass that as a string. */
public interface MouseAdjustmentListener {

    /* Expect a rapid sequence of adjustments to follow */
    public void begin(MouseAdjustment source);

    /* The value of the source has changed */
    public void adjust(MouseAdjustment source);

    /* the end of a sequence of adjustments */
    public void end(MouseAdjustment source);
}

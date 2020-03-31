// 
// * See the file "l2-tools/disclaimers-and-notices.txt" for 
// * information on usage and redistribution of this file, 
// * and for a DISCLAIMER OF ALL WARRANTIES. 
// 

/*
 * L2Exception.java
 *
 * Created on September 10, 2002, 11:08 AM
 */

package livdll;

/**
 *
 * @author  stephen
 */
public abstract class L2Exception extends java.lang.Exception {
    
    /**
     * Creates a new instance of <code>L2Exception</code> without detail message.
     */
    public L2Exception() {
    }
    
    
    /**
     * Constructs an instance of <code>L2Exception</code> with the specified detail message.
     * @param msg the detail message.
     */
    public L2Exception(String msg) {
        super(msg);
    }
}

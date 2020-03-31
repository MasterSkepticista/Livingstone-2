// 
// * See the file "l2-tools/disclaimers-and-notices.txt" for 
// * information on usage and redistribution of this file, 
// * and for a DISCLAIMER OF ALL WARRANTIES. 
// 

/*
 * L2NativeException.java
 *
 * Created on September 10, 2002, 11:14 AM
 */

package livdll;

/**
 *
 * @author  stephen
 */
public class L2NativeException extends L2Exception {
    
    /**
     * Creates a new instance of <code>L2NativeException</code> without detail message.
     */
    public L2NativeException() {
    }
    
    
    /**
     * Constructs an instance of <code>L2NativeException</code> with the specified detail message.
     * @param msg the detail message.
     */
    public L2NativeException(String msg) {
        super(msg);
    }
}

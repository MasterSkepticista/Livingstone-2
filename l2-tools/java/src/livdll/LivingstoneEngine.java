//
// * See the file "l2-tools/disclaimers-and-notices.txt" for
// * information on usage and redistribution of this file,
// * and for a DISCLAIMER OF ALL WARRANTIES.
//
package livdll;

import java.util.*;

public abstract class LivingstoneEngine extends LivingstoneEngineJNI {
    
    public static final int CBFS = 1;
    public static final int COVER = 2;
    /* The address of the C++ Livingstone instance */
    private int pointer = -1;
    private int debugPointer = -1;
    
    /* Don't use this constructor directly.  Use the factory instead. */
    protected LivingstoneEngine(int pointer, int debugPointer) {
        this.pointer = pointer;
        this.debugPointer = debugPointer;
    }
    
    protected static List engines = new ArrayList();
    
    public static synchronized void deallocate(LivingstoneEngine e) {
        engines.remove(e);
        int result = e.deallocateEngineJNI(e.pointer, e.debugPointer);
        System.out.println("deallocated engine: " + e.pointer + ", " +
        e.debugPointer + "; result= "+ result);
    }
    
    public Throwable createNativeException(String msg) {
        return new L2NativeException(msg);
    }
    
    public Object createCandidateAssignment(String attribute, String value, int weight) {
        return new CandidateAssignment(attribute, value, weight);
    }
    
    public Object createAttributeAssignment(String name, String value) {
        return new AttributeAssignment(name, value);
    }
    
    public Object createAttributeAssignment(String name, String value, int time){
        return new AttributeAssignment(name, value, time);
    }
    
    public Object createDiscrepancy(String name, String value, int weight) {
        return new Discrepancy(name, value,  weight);
    }

}




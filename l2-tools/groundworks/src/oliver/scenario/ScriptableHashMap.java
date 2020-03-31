// 
// * See the file "l2-tools/disclaimers-and-notices.txt" for 
// * information on usage and redistribution of this file, 
// * and for a DISCLAIMER OF ALL WARRANTIES. 
// 

/*
 * ScriptableHashMap.java
 *
 * Created on December 10, 2002, 9:21 AM
 */

package oliver.scenario;

/**
 *
 * @author  stephen
 */
public class ScriptableHashMap extends org.mozilla.javascript.ScriptableObject {
    
    private java.util.HashMap hash = new java.util.HashMap();
    
    // The zero-argument constructor used by Rhino runtime to create instances
    public ScriptableHashMap() {
    }
    
    public static org.mozilla.javascript.Scriptable jsConstructor(org.mozilla.javascript.Context cx, Object[] args,
    org.mozilla.javascript.Function ctorObj, boolean inNewExpr) {
        ScriptableHashMap result = new ScriptableHashMap();
        if(args.length > 0) {
            for(int i = 0; i < args.length; i+=2) {
                String key = org.mozilla.javascript.Context.toString(args[i]);
                String value = org.mozilla.javascript.Context.toString(args[i+1]);
                result.jsFunction_put(key, value);
            }
        }
        return result;
    }
    
    // The class name is defined by the getClassName method
    public String getClassName() { return "ScriptableHashMap"; }
    
    // Methods can be defined using the jsFunction_ prefix. Here we define
    //  resetCount for JavaScript.
    public Object jsFunction_get(String key) { return hash.get(key); }
    
    public void jsFunction_put(String key, String value) { hash.put(key, value); }
    
}
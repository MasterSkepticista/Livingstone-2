// 
// * See the file "l2-tools/disclaimers-and-notices.txt" for 
// * information on usage and redistribution of this file, 
// * and for a DISCLAIMER OF ALL WARRANTIES. 
// 

/*
 * MyString.java
 *
 * Created on December 10, 2002, 2:18 PM
 */

package oliver.scenario;

/**
 *
 * @author  stephen
 */
public class MyString extends org.mozilla.javascript.ScriptableObject {
    
    private String instanceName;
    
    public MyString() {
    }
    
    public static org.mozilla.javascript.Scriptable jsConstructor(org.mozilla.javascript.Context cx, Object[] args,
    org.mozilla.javascript.Function ctorObj, boolean inNewExpr) {
        MyString result = new MyString();
        if(args.length > 0) {
            result.jsSet_name(args[0].toString());
        }
        return result;
    }

    public String getClassName() { return "MyString"; }
    public String jsGet_name() { return instanceName; }
    public void jsSet_name(String instanceName) { this.instanceName = instanceName; }
};
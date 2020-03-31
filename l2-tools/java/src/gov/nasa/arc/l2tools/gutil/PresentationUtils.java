//
// * See the file "l2-tools/disclaimers-and-notices.txt" for 
// * information on usage and redistribution of this file, 
// * and for a DISCLAIMER OF ALL WARRANTIES.
//
/*
 * Author : Mark Shirley
 * Description : PresentationUtils.java
 *
 * Not finished.
 *
 * CHANGELOG:
 * 11/28/99      Mhs     Created
 * 11/29/99      Mhs     Added hash
 */

package gov.nasa.arc.l2tools.gutil;

import gov.nasa.arc.l2tools.util.Vector; // Note
import gov.nasa.arc.l2tools.util.EventLog;

import java.util.Hashtable;
import java.util.Enumeration;
import javax.swing.ImageIcon;
import java.awt.List;
import java.lang.reflect.Constructor;

/** This class contains useful methods for dealing with presentations.  The
    main thing is the factory method for presentations.  This class holds a
    mapping from objectClass x contextClass -> presentationClass.

    The main entries are getPresentation() and registerPresentationClass().
    To make things more efficient, you can implement the Presentable interface
    on your object classes.  getPresentation() here delegates to the
    getPresentation() method on that interface.

    The mapping is implemented via a sorted list of Object class x Vector ....

 */
public final class PresentationUtils {
    
    static Vector presentationMap = new Vector();

    /** This class is not instantiable */
    private PresentationUtils() { }

    /** Use this to register objectClass x contextClass -> presentationClass mappings */
    public static void registerPresentationClass(Class theClass, Class context, Class presentationClass) {
        insertIntoClassMap(theClass, context, presentationClass);
    }

    /** The main entry point for creating presentations.  Use this unless
        you're sure you need to use getPresentationLookup(). */
    public static Presentation getPresentation(Object object, Class context) {
        if (object instanceof Presentable)
            return ((Presentable)object).getPresentation(context);
        return getPresentationLookup(object, context);
    }

    /** This entry doesn't handle Presentable objects specially and so is
        appropriate for Presentable objects to use as a way of deferring back
        to the registry. */
    public static Presentation getPresentationLookup(Object object, Class context) {
        return makePresentation(getPresentationClass(object.getClass(),context),object);
    }

    static {
        registerPresentationClass(Object.class,HtmlInspector.class,HtmlInspectorPresentation.class);
        registerPresentationClass(Object.class,HtmlPresentationPane.class,DefaultHtmlPresentation.class);
        registerPresentationClass(Object.class,Worksurface.class,DefaultSurfacePresentation.class);
        registerPresentationClass(Object.class,List.class,DefaultListPresentation.class);
    }

    protected static class PPair {
        Class theClass;
        Vector contexts;
        PPair(Class theClass, Vector contexts) { 
            this.theClass = theClass;
            this.contexts = contexts;
        }
    }

    protected static class CPair { 
        Class context;
        Class presentationClass;
        CPair(Class context, Class presentationClass) {
            this.context = context;
            this.presentationClass = presentationClass;
        }
    }

    public static Class getPresentationClass(Class objectClass, Class context) {
        Class c = null;
        int max = presentationMap.size();
        for (int i=0;i<max;i++) {
            PPair p = (PPair)presentationMap.elementAt(i);
            if (p.theClass.isAssignableFrom(objectClass)
                && null != (c=getPresentationClassFromContext(p.contexts,context)))
                return c;
        }
        return null;
    }

    protected static Class getPresentationClassFromContext(Vector contexts, Class context) {
        int max = contexts.size();
        for (int i=0;i<max;i++) {
            CPair cp = (CPair)contexts.elementAt(i);
            if (cp.context.isAssignableFrom(context))
                return cp.presentationClass;
        }
        return null;
    }

    protected static void insertIntoClassMap(Class newClass, Class context, Class presentationClass) {
        int max = presentationMap.size();
        for (int i=0;i<max;i++) {
            PPair p = (PPair)presentationMap.elementAt(i);
            if (p.theClass==newClass) {
                insertContext(p.contexts,context,presentationClass);
                return;
            } else if (p.theClass.isAssignableFrom(newClass)) {
                PPair p1 = new PPair(newClass,new Vector(new CPair(context,presentationClass)));
                presentationMap.insertElementAt(p1,i);
                return;
            }
        }
        PPair p1 = new PPair(newClass,new Vector(new CPair(context,presentationClass)));
        presentationMap.addElement(p1);
    }

    protected static void insertContext(Vector contexts, Class context, Class presentationClass) {
        int max = contexts.size();
        for (int i=0;i<max;i++) {
            CPair cp = (CPair)contexts.elementAt(i);
            if (cp.context==context) {
                cp.presentationClass = presentationClass;
                return;
            } else if (cp.context.isAssignableFrom(context)) {
                contexts.insertElementAt(new CPair(context,presentationClass),i);
                return;
            }
        }
        contexts.addElement(new CPair(context,presentationClass));
    }

    public static void printPresentationMap() {
        System.out.println("<PresentationMap>");
        int max = presentationMap.size();
        for (int i=0;i<max;i++) {
            PPair p = (PPair)presentationMap.elementAt(i);
            System.out.println(p.theClass.getName()+" -->");
            int max1 = p.contexts.size();
            for (int i1=0;i1<max1;i1++) {
                CPair cp = (CPair)p.contexts.elementAt(i1);
                System.out.println("  "+cp.context.getName()+" -> "+cp.presentationClass.getName());
            }
        }
        System.out.println("</PresentationMap>");
    }

    protected static Class[] classList = new Class[1];
    protected static Object[] objectList = new Object[1];

    public static synchronized Presentation makePresentation(Class presentationClass, Object object) {
        try {
            Constructor c = getConstructor(presentationClass,object.getClass());
            if (c==null) return null;
            objectList[0] = object;
            return (Presentation)c.newInstance(objectList);
        } catch (Exception e) {
            EventLog.report(e);
            return null;
        }
    }

    /** Compensate for getConstructor not handling inheritance */
    protected static Constructor getConstructor(Class presentationClass, Class objectClass) {
        if (objectClass==null) return null;
        classList[0] = objectClass;
        try {
            return presentationClass.getConstructor(classList);
        } catch (java.lang.NoSuchMethodException e) {
            return getConstructor(presentationClass,objectClass.getSuperclass());
        }
    }

    //
    // Icons table
    //

    static Vector iconMap = new Vector();

    public static void registerIconSource(Class theClass, String source) {
        ImageIcon icon = new ImageIcon(source);
        iconMap.addElement(new IconPair(theClass, source));
    }

    public static ImageIcon getIcon(Class theClass) {
        IconPair pair = null;
        Object o = null;
        for (Enumeration e=iconMap.elements();e.hasMoreElements();) {
            pair = (IconPair)e.nextElement();
            if (pair.key.isAssignableFrom(theClass)) { // Got it
                o = pair.object;
                break;
            }
        }
        if (o==null) return null;
        if (o instanceof ImageIcon) return (ImageIcon)o;
        if (o instanceof String) {
            ImageIcon icon = new ImageIcon((String)o);
            pair.object = icon;
            return icon;
        }
        throw new Error("Couldn't find an icon for "+theClass);
    }

    protected static class IconPair { 
        IconPair(Class key, Object object) {
            this.key = key;
            this.object = object;
            }
        Class key; Object object;
    }
        
}

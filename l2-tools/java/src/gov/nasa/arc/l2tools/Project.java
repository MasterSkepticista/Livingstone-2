//
// * See the file "l2-tools/disclaimers-and-notices.txt" for 
// * information on usage and redistribution of this file, 
// * and for a DISCLAIMER OF ALL WARRANTIES.
//
package gov.nasa.arc.l2tools;

/**
 * basically a bunch of skunkworks objects
 * Creation date: (2/9/2000 2:52:14 PM)
 * @author: Andrew Bachmann
 */
public class Project extends gov.nasa.arc.l2tools.DataObject {
  public final static String tagName = "project";
/**
 * Project constructor comment.
 */
public Project() {
	super(tagName);
}
/**
 * search for a Project with value <i>value</i> for key <i>key</i>
 * Creation date: (10/24/2000 4:05:22 PM)
 * @return gov.nasa.arc.l2tools.Project
 * @param key java.lang.String
 * @param value java.lang.String
 * @param object gov.nasa.arc.l2tools.DataObject
 */
public static Project search(String key, String value, gov.nasa.arc.l2tools.DataObject object) {
	return (Project)object.search(Project.class,key,value);
}
/**
 * search for a Project named <i>name</i>
 * Creation date: (10/24/2000 4:05:22 PM)
 * @return gov.nasa.arc.l2tools.Project
 * @param name java.lang.String
 * @param object gov.nasa.arc.l2tools.DataObject
 */
public static Project searchByName(String name, gov.nasa.arc.l2tools.DataObject object) {
	return (Project)object.searchByName(Project.class,name);
}
}

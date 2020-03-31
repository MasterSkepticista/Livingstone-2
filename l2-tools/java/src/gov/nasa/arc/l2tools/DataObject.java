//
// * See the file "l2-tools/disclaimers-and-notices.txt" for 
// * information on usage and redistribution of this file, 
// * and for a DISCLAIMER OF ALL WARRANTIES.
//
package gov.nasa.arc.l2tools;

/**
 * Insert the type's description here.
 * Creation date: (2/9/2000 2:31:34 PM)
 * @author: Andrew Bachmann
 * abstract
 */
public class DataObject extends java.util.Observable implements gov.nasa.arc.l2tools.io.TreeObject {
	public final static String tagName = "object";
	// all the structural content of the object
	private gov.nasa.arc.l2tools.io.TreeObject parent = null;
	private javax.swing.DefaultListModel children = new javax.swing.DefaultListModel();
	
	// a synchronized representation of content and structure
	private gov.nasa.arc.l2tools.DataObject.XmlElement xmlElement = null; 

	// a synchronized representation of content and structure
	public class XmlElement extends fr.dyade.koala.xml.domlight.XMLElement {
		public XmlElement() {
			super();
		}
		public XmlElement(String name) {
			super(name);
		}
		public void appendChild(fr.dyade.koala.xml.domlight.XMLNode node) {
			if (node instanceof XmlElement) {
				getChildren().addElement(((XmlElement)node).getObject());
			}
			if (node instanceof fr.dyade.koala.xml.domlight.XMLContent) {
				String content=((fr.dyade.koala.xml.domlight.XMLContent)node).getContent().trim();
				if (content.length()>0) {
					getChildren().addElement(new Content(content));
				}
			}
		}
		public void add(fr.dyade.koala.xml.domlight.XMLNode node) {
			super.appendChild(node);
		}
		public void insert(int index, fr.dyade.koala.xml.domlight.XMLNode node) {
			super.insertChild(index,node);
		}
		public void remove(int index1, int index2) {
			for (int i=index2;(i>=index1);i--) {
				super.removeChild(i);
			}
		}
		public DataObject getObject() {
			return DataObject.this;
		}
	}
	
	// another synchronized representation of content and structure
	class IndentContent extends gov.nasa.arc.l2tools.Content {
	  public String getContent() {
			String indent=new String("\n");
			gov.nasa.arc.l2tools.io.TreeObject climbTree=DataObject.this.getParent();
			while (climbTree!=null) {
				indent+="  ";
				climbTree=climbTree.getParent();
			}
			return indent;
	  }
  }
	private int subElements = 0;
/**
 * Object constructor comment.
 */
public DataObject() {
	this(tagName);
}
/**
 * Object constructor comment.
 */
public DataObject(XmlElement xmlElement) {
	super();
	setXmlElement(xmlElement);
	setupListModelListener();
}
/**
 * Object constructor comment.
 * @param name java.lang.String
 */
public DataObject(String name) {
	super();
	setXmlElement(new XmlElement(name));
	setupListModelListener();
}
/**
 * Creation date: (4/9/2000 12:53:01 AM)
 * @author: Andrew Bachmann
 * 
 */
private void addIndent() {
	xmlElement.add(new IndentContent());
}
/**
 * look through the children of this data object for a data object 
 * that satisfies the property:
 *   klass.isInstance(child) && child.get(key).equals(value)
 * returns null if no such object is found
 *
 * Creation date: (10/24/2000 2:08:05 PM)
 * @return gov.nasa.arc.l2tools.DataObject
 * @param klass java.lang.Class
 * @param key java.lang.String
 * @param value java.lang.String
 */
public DataObject find(Class klass, String key, String value) {
	java.util.Enumeration children=getChildren().elements();
	while (children.hasMoreElements()) {
		java.lang.Object object=children.nextElement();
		if ((klass.isInstance(object)) && (((DataObject)object).get(key).equals(value))) {
			return (DataObject)object;
		}
	}
	return null;
}
/**
 * look through the children of this data object for a data object 
 * that satisfies the property:
 *   (object instanceof DataObject) && (child.get(key).equals(value))
 * returns null if no such object is found
 *
 * Creation date: (10/24/2000 2:08:05 PM)
 * @return gov.nasa.arc.l2tools.DataObject
 * @param key java.lang.String
 * @param value java.lang.String
 */
public DataObject find(String key, String value) {
	java.util.Enumeration children=getChildren().elements();
	while (children.hasMoreElements()) {
		java.lang.Object object=children.nextElement();
		if ((object instanceof DataObject) && (((DataObject)object).get(key).equals(value))) {
			return (DataObject)object;
		}
	}
	return null;
}
/**
 * look through the children of this data object for a data object 
 * that satisfies the property:
 *   klass.isInstance(child) && child.get("name").equals(name)
 * returns null if no such object is found
 *
 * Creation date: (10/24/2000 2:08:05 PM)
 * @return gov.nasa.arc.l2tools.DataObject
 * @param klass java.lang.Class
 * @param name java.lang.String
 */
public DataObject findByName(Class klass, String name) {
	return find(klass,"name",name);
}
/**
 * look through the children of this data object for a data object 
 * that satisfies the property:
 *   (object instanceof DataObject) && (child.get("name").equals(name))
 * returns null if no such object is found
 *
 * Creation date: (10/24/2000 2:08:05 PM)
 * @return gov.nasa.arc.l2tools.DataObject
 * @param key java.lang.String
 * @param value java.lang.String
 */
public DataObject findByName(String name) {
	return findByName(DataObject.class,name);
}
/**
 *
 * Creation date: (3/10/2000 2:35:19 PM)
 * @author: Andrew Bachmann
 * 
 * @return gov.nasa.arc.l2tools.Object
 * @param name java.lang.String
 */
public gov.nasa.arc.l2tools.io.TreeObject findObject(String name) {
	gov.nasa.arc.l2tools.io.TreeObject obj = this;
	while (obj != null) {
		if (obj.get("name")==name) return obj;
		obj = obj.getParent();
	}
	return null;
}
/**
 * get method comment.
 */
public java.lang.String get(java.lang.String field) {
	return getXmlElement().getAttribute(field);
}
/**
 *
 * Creation date: (3/6/2000 1:48:11 PM)
 * @author: Andrew Bachmann
 * 
 * @return javax.swing.DefaultListModel
 */
public javax.swing.DefaultListModel getChildren() {
	return children;
}
/**
 *
 * Creation date: (3/6/2000 1:28:23 PM)
 * @author: Andrew Bachmann
 * 
 * @return gov.nasa.arc.l2tools.io.TreeObject
 */
public gov.nasa.arc.l2tools.io.TreeObject getParent() {
	return parent;
}
/**
 *
 * Creation date: (3/6/2000 12:58:25 PM)
 * @author: Andrew Bachmann
 * 
 * @return fr.dyade.koala.xml.domlight.XMLElement
 */
public fr.dyade.koala.xml.domlight.XMLElement getXmlElement() {
	return xmlElement;
}
/**
 *
 * Creation date: (3/8/2000 9:53:39 AM)
 * @author: Andrew Bachmann
 */
private void insertExternalObjectElement(int i, gov.nasa.arc.l2tools.Content o) {
	getXmlElement().insertChild(2*i,new Content(""));
	getXmlElement().insertChild(2*i+1,o);
}
/**
 *
 * Creation date: (3/8/2000 9:53:39 AM)
 * @author: Andrew Bachmann
 */
private void insertExternalObjectElement(int i, gov.nasa.arc.l2tools.DataObject o) {
	getXmlElement().insertChild(2*i,o.new IndentContent());
	getXmlElement().insertChild(2*i+1,o.getXmlElement());
	subElements++;
	o.parent=this;
}
/**
 *
 * Creation date: (3/8/2000 9:53:39 AM)
 * @author: Andrew Bachmann
 */
private void insertExternalObjectElement(int i, java.lang.Object o) {
	if (o instanceof DataObject) {
		insertExternalObjectElement(i,(gov.nasa.arc.l2tools.DataObject)o);
	} else if (o instanceof Content) {
		insertExternalObjectElement(i,(gov.nasa.arc.l2tools.Content)o);
	} else {
		System.err.print("Warning: inserting dummy xmlElement for object: "+o.toString());
		System.err.println(" of class: "+o.getClass().toString());
		getXmlElement().insertChild(i,new fr.dyade.koala.xml.domlight.XMLElement());
	}
}
/**
 *
 * Creation date: (3/8/2000 9:53:39 AM)
 * @author: Andrew Bachmann
 */
private void removeExternalObjectElement(int i) {
	if (getXmlElement().getChild(2*i+1) instanceof fr.dyade.koala.xml.domlight.XMLElement)
		subElements--;
	getXmlElement().removeChild(2*i+1);
	getXmlElement().removeChild(2*i);
}
/**
 * Creation date: (4/9/2000 12:53:01 AM)
 * @author: Andrew Bachmann
 * 
 */
private void removeIndent() {
	xmlElement.removeChild(xmlElement.getChildCount()-1);
}
/**
 * search for an object of class <i>klass</i> with the value <i>value</i> for the key <i>key</i>
 * Creation date: (10/24/2000 4:05:22 PM)
 * @return gov.nasa.arc.l2tools.DataObject
 * @param klass java.lang.Class
 * @param key java.lang.String
 * @param value java.lang.String
 */
public DataObject search(Class klass, String key, String value) {
	DataObject findResult=(DataObject)find(klass,key,value);
	if (findResult!=null) {
		return findResult;
	}
	java.util.Enumeration children=getChildren().elements();
	while (children.hasMoreElements()) {
		java.lang.Object child=children.nextElement();
		if (child instanceof gov.nasa.arc.l2tools.DataObject) {
			DataObject result=((gov.nasa.arc.l2tools.DataObject)child).search(klass,key,value);
			if (result!=null) return result;
		}
	}
	return null;
}
/**
 * search for a DataObject with the value <i>value</i> for the key <i>key</i>
 * Creation date: (10/24/2000 4:05:22 PM)
 * @return gov.nasa.arc.l2tools.DataObject
 * @param key java.lang.String
 * @param value java.lang.String
 */
public DataObject search(String key, String value) {
	return search(DataObject.class,key,value);
}
/**
 * search for an object of class <i>klass</i> named <i>name</i>
 * Creation date: (10/24/2000 4:05:22 PM)
 * @return gov.nasa.arc.l2tools.DataObject
 * @param klass java.lang.Class
 * @param name java.lang.String
 */
public DataObject searchByName(Class klass, String name) {
	return search(klass,"name",name);
}
/**
 * search for a DataObject named <i>name</i>
 * Creation date: (10/24/2000 4:05:22 PM)
 * @return gov.nasa.arc.l2tools.DataObject
 * @param klass java.lang.Class
 * @param name java.lang.String
 */
public DataObject searchByName(String name) {
	return searchByName(DataObject.class,name);
}
/**
 *
 * Creation date: (3/3/2000 3:10:59 PM)
 * @author: Andrew Bachmann
 * 
 * @param name java.lang.String
 * @param value gov.nasa.arc.l2tools.DataObject
 */
public void set(String name, String value) {
	getXmlElement().addAttribute(name,value);
	notifyObservers();
}
/**
 *
 * Creation date: (3/6/2000 1:48:11 PM)
 * @author: Andrew Bachmann
 * 
 * @param newChildren javax.swing.DefaultListModel
 */
private void setChildren(javax.swing.DefaultListModel newChildren) {
	children = newChildren;
}
/**
 * setParent method comment.
 */
public void setParent(gov.nasa.arc.l2tools.io.TreeObject newParent) {
	gov.nasa.arc.l2tools.io.TreeObject oldParent=getParent();
	if (oldParent != null) {
		oldParent.getChildren().removeElement(this);
	}
	newParent.getChildren().addElement(this);
	parent = newParent;
	notifyObservers();
}
/**
 *
 * Creation date: (3/6/2000 1:54:02 PM)
 * @author: Andrew Bachmann
 * 
 */
private void setupListModelListener() {
	javax.swing.event.ListDataListener listener = new javax.swing.event.ListDataListener() {
		public void contentsChanged(javax.swing.event.ListDataEvent event) {
			for (int i=event.getIndex1();(i>=event.getIndex0());i--) {
				updateExternalObjectElement(i,getChildren().elementAt(i));
			}
		}
		public void intervalAdded(javax.swing.event.ListDataEvent event) {
			if (getXmlElement().getChildCount()==0) addIndent();
			for (int i=event.getIndex1();(i>=event.getIndex0());i--) {
				insertExternalObjectElement(event.getIndex0(),getChildren().elementAt(i));
			}
			if (subElements==0) removeIndent();
			
		}
		public void intervalRemoved(javax.swing.event.ListDataEvent event) {
			for (int i=event.getIndex1();(i>=event.getIndex0());i--) {
				removeExternalObjectElement(i);
			}			
			if (getXmlElement().getChildCount()==1) removeIndent();
			else if (subElements==0) removeIndent();
		}
	};
	getChildren().addListDataListener(listener);
}
/**
 *
 * Creation date: (3/6/2000 12:58:25 PM)
 * @author: Andrew Bachmann
 * 
 * @param newXmlElement gov.nasa.arc.l2tools.DataObject.XmlElement
 */
private void setXmlElement(XmlElement newXmlElement) {
	xmlElement = newXmlElement;
}
/**
 * Extremely useful toString method to help with debugging
 * Shows element name, attributes, and their values
 * Creation date: (2/25/2000 10:54:23 AM)
 * @author: Andrew Bachmann
 */
public String toString() {
	String shortDescription = new String("");
	shortDescription+="<";
	shortDescription+=getXmlElement().getName();

	if (getXmlElement().getAttributes() != null) {
  	for (int i=0;(i<getXmlElement().getAttributes().length);i++) {
  		String attribute=getXmlElement().getAttributes()[i];
  		shortDescription+=" "+attribute;
  		String value=getXmlElement().getAttribute(attribute);
  		if (value != null)
  		  shortDescription+="="+'"'+value+'"';
  	}
	}
	
	shortDescription+=">";
	return shortDescription;
}
/**
 *
 * Creation date: (3/8/2000 9:53:39 AM)
 * @author: Andrew Bachmann
 */
private void updateExternalObjectElement(int i, gov.nasa.arc.l2tools.Content o) {
	if (getXmlElement().getChild(2*i+1) instanceof XmlElement)
		subElements--;
	getXmlElement().setChild(2*i,new Content(""));
	getXmlElement().setChild(2*i+1,o);
}
/**
 *
 * Creation date: (3/8/2000 9:53:39 AM)
 * @author: Andrew Bachmann
 */
private void updateExternalObjectElement(int i, gov.nasa.arc.l2tools.DataObject o) {
	if (getXmlElement().getChild(2*i+1) instanceof Content)
		subElements++;
	getXmlElement().setChild(2*i,o.new IndentContent());
	getXmlElement().setChild(2*i+1,o.getXmlElement());
}
/**
 *
 * Creation date: (3/8/2000 9:53:39 AM)
 * @author: Andrew Bachmann
 */
private void updateExternalObjectElement(int i, java.lang.Object o) {
	if (gov.nasa.arc.l2tools.DataObject.class.isAssignableFrom(o.getClass())) {
		updateExternalObjectElement(i,(gov.nasa.arc.l2tools.DataObject)o);
	} else if (gov.nasa.arc.l2tools.Content.class.isAssignableFrom(o.getClass())) {
		updateExternalObjectElement(i,(gov.nasa.arc.l2tools.Content)o);
	} else {
		System.err.print("Warning: updating, with dummy xmlElement, object: "+o.toString());
		System.err.println(" of class: "+o.getClass().toString());
		getXmlElement().setChild(i,new fr.dyade.koala.xml.domlight.XMLElement());
	}
}
}

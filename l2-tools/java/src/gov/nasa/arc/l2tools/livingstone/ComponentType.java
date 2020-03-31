//
// * See the file "l2-tools/disclaimers-and-notices.txt" for 
// * information on usage and redistribution of this file, 
// * and for a DISCLAIMER OF ALL WARRANTIES.
//
package gov.nasa.arc.l2tools.livingstone;

/**
 * Insert the type's description here.
 * Creation date: (2/8/2000 12:22:03 PM)
 * @author: Andrew Bachmann
 */
public class ComponentType extends gov.nasa.arc.l2tools.livingstone.code.CodeObject
implements gov.nasa.arc.l2tools.livingstone.code.Cachable,
gov.nasa.arc.l2tools.livingstone.code.Compilable,
gov.nasa.arc.l2tools.livingstone.code.Instantiable {
  public final static String tagName = "componenttype";
  // compilation caches
  private java.lang.String name = null;
  private javax.swing.DefaultListModel constraints = new javax.swing.DefaultListModel();
  private javax.swing.DefaultListModel relations = new javax.swing.DefaultListModel();
  private javax.swing.DefaultListModel transitions = new javax.swing.DefaultListModel();
  private javax.swing.DefaultListModel failures = new javax.swing.DefaultListModel();
  private gov.nasa.arc.l2tools.livingstone.code.componenttype.StateVector stateVector = null;
  private javax.swing.DefaultListModel attributes = new javax.swing.DefaultListModel();
  private javax.swing.DefaultListModel attributeTypes = new javax.swing.DefaultListModel();
  private java.lang.String superS = null;
  private ComponentType superComponentType = null;
  /**
   * ComponentType constructor comment.
   */
  public ComponentType() {
    super(tagName);
  }
  /**
   * ComponentType constructor comment.
   * @param name java.lang.String
   */
  public ComponentType(String name) {
    super(name);
  }
  /**
   *
   * Creation date: (3/22/2000 2:02:38 PM)
   * @author: Andrew Bachmann
   *
   * @return gov.nasa.arc.l2tools.livingstone.ComponentInstance
   */
  public ComponentInstance buildInstance() throws Exception {
    return buildInstance(null,getParent(),false);
  }
  public ComponentInstance buildInstance(boolean optimize) throws Exception {
    return buildInstance(null,getParent(),optimize);
  }
  /**
   *
   * Creation date: (3/22/2000 2:03:39 PM)
   * @author: Andrew Bachmann
   *
   * @return gov.nasa.arc.l2tools.livingstone.ComponentInstance
   * @param project gov.nasa.arc.l2tools.io.TreeObject
   */
  public ComponentInstance buildInstance(gov.nasa.arc.l2tools.io.TreeObject project) throws Exception {
    return buildInstance(null,project,false);
  }
  public ComponentInstance buildInstance(gov.nasa.arc.l2tools.io.TreeObject project, boolean optimize) throws Exception {
    return buildInstance(null,project,optimize);
  }
  /**
   *
   * Creation date: (3/22/2000 2:02:38 PM)
   * @author: Andrew Bachmann
   *
   * @return gov.nasa.arc.l2tools.livingstone.ComponentInstance
   * @param componentName String
   */
  public ComponentInstance buildInstance(String componentName) throws Exception {
    return buildInstance(componentName,getParent(),false);
  }
  public ComponentInstance buildInstance(String componentName, boolean optimize) throws Exception {
    return buildInstance(componentName,getParent(),optimize);
  }
  /**
   *
   * Creation date: (3/22/2000 2:03:39 PM)
   * @author: Andrew Bachmann
   *
   * @return gov.nasa.arc.l2tools.livingstone.ComponentInstance
   * @param componentName String
   * @param project gov.nasa.arc.l2tools.io.TreeObject
   */
  public ComponentInstance buildInstance(String componentName, gov.nasa.arc.l2tools.io.TreeObject project) throws Exception {
    return buildInstance(componentName,project,false);
  }
  public ComponentInstance buildInstance(String componentName, gov.nasa.arc.l2tools.io.TreeObject project, boolean optimize) throws Exception {
    ComponentInstance componentInstance=new ComponentInstance();
    // make up a name if not given
    String componentTypeName=get("name");
    if (componentName==null) {
      if ((componentTypeName==null) || (componentTypeName.length()==0)) {
        componentName="_";
      } else {
        if (new String("aeiou").indexOf(componentTypeName.substring(0,1))==-1) {
          componentTypeName=componentTypeName.substring(0,1).toUpperCase()+componentTypeName.substring(1);
          componentName="a"+componentTypeName;
        } else {
          componentTypeName=componentTypeName.substring(0,1).toUpperCase()+componentTypeName.substring(1);
          componentName="an"+componentTypeName;
        }
      }
    }
    componentInstance.set("name",componentName);
    componentInstance.set("type", componentTypeName);
    // do it
    instantiate(componentName,componentInstance,optimize);
    // check for unreferenced variables
    java.util.Enumeration children=componentInstance.getChildren().elements();
    while (children.hasMoreElements()) {
      java.lang.Object child=children.nextElement();
      if (child instanceof gov.nasa.arc.l2tools.livingstone.componentinstance.Component) {
        ((gov.nasa.arc.l2tools.livingstone.componentinstance.Component)child).checkVariableReferences();
      }
    }
    // group attribute types together
    groupAttributeTypes(componentInstance);
    // done!
    return componentInstance;
  }
  /**
   *
   * Creation date: (3/10/2000 6:32:19 PM)
   * @author: Andrew Bachmann
   *
   */
  private void cacheAttribute(gov.nasa.arc.l2tools.livingstone.code.componenttype.Attribute attribute)
  throws gov.nasa.arc.l2tools.Exception {
    // cache itself
    attribute.cache();
    
    // check for duplicate name
    java.util.Enumeration attributes=getAttributes().elements();
    while (attributes.hasMoreElements()) {
      java.lang.Object oldAttribute=attributes.nextElement();
      if (!(oldAttribute instanceof gov.nasa.arc.l2tools.livingstone.code.componenttype.Attribute)) {
        String errorString="cache error: attributes corrupted";
        throw new gov.nasa.arc.l2tools.livingstone.code.CacheError(errorString);
      } else if (attribute.getName()==((gov.nasa.arc.l2tools.livingstone.code.componenttype.Attribute)oldAttribute).getName()) {
        String errorString="illegal duplicate error: a attribute may only occur once";
        errorString+="\n  '"+attribute.getName()+"' occurs more than once";
        throw new gov.nasa.arc.l2tools.livingstone.code.IllegalDuplicateError(errorString);
      }
    }
    
    // add to cache
    getAttributes().addElement(attribute);
  }
  /**
   *
   * Creation date: (3/10/2000 6:32:19 PM)
   * @author: Andrew Bachmann
   *
   * @param attributeType gov.nasa.arc.l2tools.livingstone.code.componenttype.AttributeType
   * @exception gov.nasa.arc.l2tools.Exception The exception description.
   */
  private void cacheAttributeType(gov.nasa.arc.l2tools.livingstone.code.componenttype.AttributeType attributeType)
  throws gov.nasa.arc.l2tools.Exception {
    // cache itself
    attributeType.cache();
    
    // check for duplicate name
    java.util.Enumeration attributeTypes=getAttributeTypes().elements();
    while (attributeTypes.hasMoreElements()) {
      java.lang.Object oldAttributeType=attributeTypes.nextElement();
      if (!(oldAttributeType instanceof gov.nasa.arc.l2tools.livingstone.code.componenttype.AttributeType)) {
        String errorString="cache error: attributeTypes corrupted";
        throw new gov.nasa.arc.l2tools.livingstone.code.CacheError(errorString);
      } else if (attributeType.prototypesEqual((gov.nasa.arc.l2tools.livingstone.code.componenttype.AttributeType)oldAttributeType)) {
        String errorString="illegal duplicate error: a attributeType may only occur once";
        errorString+="\n  '"+attributeType.getName()+"' occurs more than once";
        throw new gov.nasa.arc.l2tools.livingstone.code.IllegalDuplicateError(errorString);
      }
    }
    
    // add to cache
    getAttributeTypes().addElement(attributeType);
  }
  /**
   *
   * Creation date: (3/24/2000 2:30:50 PM)
   * @author: Andrew Bachmann
   *
   * @param failure gov.nasa.arc.l2tools.livingstone.code.componenttype.FailureTransition
   * @exception gov.nasa.arc.l2tools.Exception The exception description.
   */
  private void cacheFailure(gov.nasa.arc.l2tools.livingstone.code.componenttype.FailureTransition failure)
  throws gov.nasa.arc.l2tools.Exception {
    // cache itself
    failure.cache();
    
    // check for duplicate name
    java.util.Enumeration failures=getFailures().elements();
    while (failures.hasMoreElements()) {
      java.lang.Object oldFailure=failures.nextElement();
      if (!(oldFailure instanceof gov.nasa.arc.l2tools.livingstone.code.componenttype.FailureTransition)) {
        String errorString="cache error: failures corrupted";
        throw new gov.nasa.arc.l2tools.livingstone.code.CacheError(errorString);
      } else if (failure.getName()==((gov.nasa.arc.l2tools.livingstone.code.componenttype.FailureTransition)oldFailure).getName()) {
        String errorString="illegal duplicate error: a failure may only occur once";
        errorString+="\n  '"+failure.getName()+"' occurs more than once";
        throw new gov.nasa.arc.l2tools.livingstone.code.IllegalDuplicateError(errorString);
      }
    }
    
    // add to cache
    getFailures().addElement(failure);
  }
  /**
   *
   * Creation date: (3/10/2000 6:32:19 PM)
   * @author: Andrew Bachmann
   *
   * @param relation gov.nasa.arc.l2tools.livingstone.Relation
   * @exception gov.nasa.arc.l2tools.Exception The exception description.
   */
  private void cacheRelation(gov.nasa.arc.l2tools.livingstone.code.Relation relation)
  throws gov.nasa.arc.l2tools.Exception {
    // cache itself
    relation.cache();
    
    // check for duplicate name
    java.util.Enumeration relations=getRelations().elements();
    while (relations.hasMoreElements()) {
      java.lang.Object oldRelation=relations.nextElement();
      if (!(oldRelation instanceof gov.nasa.arc.l2tools.livingstone.code.Relation)) {
        String errorString="cache error: relations corrupted";
        throw new gov.nasa.arc.l2tools.livingstone.code.CacheError(errorString);
      } else if (relation.prototypesEqual((gov.nasa.arc.l2tools.livingstone.code.Relation)oldRelation)) {
        String errorString="illegal duplicate error: a relation may only occur once";
        errorString+="\n  '"+relation.getName()+"' occurs more than once";
        throw new gov.nasa.arc.l2tools.livingstone.code.IllegalDuplicateError(errorString);
      }
    }
    
    // add to cache
    getRelations().addElement(relation);
  }
  /**
   *
   * Creation date: (3/10/2000 6:32:19 PM)
   * @author: Andrew Bachmann
   *
   * @param group gov.nasa.arc.l2tools.livingstone.code.constraint.StatementGroup
   * @exception gov.nasa.arc.l2tools.Exception The exception description.
   */
  private void cacheStatementGroup(gov.nasa.arc.l2tools.livingstone.code.constraint.StatementGroup group)
  throws gov.nasa.arc.l2tools.Exception {
    // cache itself
    group.cache();
    
    // add each statement to one large list of constraints
    // (this munges the constraints from all the groups)
    java.util.Enumeration statements=group.getStatements().elements();
    while (statements.hasMoreElements()) {
      java.lang.Object statement=statements.nextElement();
      if (statement instanceof gov.nasa.arc.l2tools.livingstone.code.constraint.Statement) {
        getConstraints().addElement(statement);
      } else {
        String errorString="cache error: statements corrupted in statement group";
        throw new gov.nasa.arc.l2tools.livingstone.code.CacheError(errorString);
      }
    }
  }
  /**
   *
   * Creation date: (3/24/2000 10:22:56 AM)
   * @author: Andrew Bachmann
   *
   * @param stateVector gov.nasa.arc.l2tools.livingstone.code.componenttype.StateVector
   * @exception gov.nasa.arc.l2tools.Exception The exception description.
   */
  private void cacheStateVector(gov.nasa.arc.l2tools.livingstone.code.componenttype.StateVector stateVector)
  throws gov.nasa.arc.l2tools.Exception {
    // cache itself
    stateVector.cache();
    
    // save it
    setStateVector(stateVector);
  }
  /**
   *
   * Creation date: (3/24/2000 2:30:50 PM)
   * @author: Andrew Bachmann
   *
   * @param transition gov.nasa.arc.l2tools.livingstone.code.componenttype.NominalTransition
   * @exception gov.nasa.arc.l2tools.Exception The exception description.
   */
  private void cacheTransition(gov.nasa.arc.l2tools.livingstone.code.componenttype.NominalTransition transition)
  throws gov.nasa.arc.l2tools.Exception {
    // cache itself
    transition.cache();
    
    // check for duplicate name
    java.util.Enumeration transitions=getTransitions().elements();
    while (transitions.hasMoreElements()) {
      java.lang.Object oldTransition=transitions.nextElement();
      if (!(oldTransition instanceof gov.nasa.arc.l2tools.livingstone.code.componenttype.NominalTransition)) {
        String errorString="cache error: transitions corrupted";
        throw new gov.nasa.arc.l2tools.livingstone.code.CacheError(errorString);
      } else if (transition.getName()==((gov.nasa.arc.l2tools.livingstone.code.componenttype.NominalTransition)oldTransition).getName()) {
        String errorString="illegal duplicate error: a transition may only occur once";
        errorString+="\n  '"+transition.getName()+"' occurs more than once";
        throw new gov.nasa.arc.l2tools.livingstone.code.IllegalDuplicateError(errorString);
      }
    }
    
    // add to cache
    getTransitions().addElement(transition);
  }
  /**
   * Creation date: (4/4/2000 1:21:35 PM)
   * @author: Andrew Bachmann
   *
   * @param component gov.nasa.arc.l2tools.livingstone.componentinstance.Component
   * @exception gov.nasa.arc.l2tools.Exception The exception description.
   */
  private void checkAttributes(gov.nasa.arc.l2tools.livingstone.componentinstance.Component component)
  throws gov.nasa.arc.l2tools.Exception {
    java.util.Enumeration attributes=getAttributes().elements();
    while (attributes.hasMoreElements()) {
      java.lang.Object attribute=attributes.nextElement();
      if (attribute instanceof gov.nasa.arc.l2tools.livingstone.code.componenttype.Attribute) {
        if (component.findAttribute(((gov.nasa.arc.l2tools.livingstone.code.componenttype.Attribute)attribute).get("name"))==null) {
          System.out.println("warning: attribute '"+
          (((gov.nasa.arc.l2tools.livingstone.code.componenttype.Attribute)attribute).get("name"))
          +"' in component type '"+getName()+"' is unused");
        }
      } else {
        String errorString="instantiation error: attributes corrupted";
        throw new InstantiationError(errorString);
      }
    }
  }
  /**
   *
   * Creation date: (3/23/2000 12:19:37 PM)
   * @author: Andrew Bachmann
   *
   * @exception gov.nasa.arc.l2tools.Exception The exception description.
   */
  private void compileAttributes() throws gov.nasa.arc.l2tools.Exception {
    java.util.Enumeration attributes=getAttributes().elements();
    while (attributes.hasMoreElements()) {
      java.lang.Object attribute=attributes.nextElement();
      if (attribute instanceof gov.nasa.arc.l2tools.livingstone.code.componenttype.Attribute) {
        ((gov.nasa.arc.l2tools.livingstone.code.componenttype.Attribute)attribute).compile();
      } else {
        String errorString="compiler error: attributes corrupted";
        throw new gov.nasa.arc.l2tools.livingstone.code.CompilerError(errorString);
      }
    }
  }
  /**
   *
   * Creation date: (3/23/2000 12:19:37 PM)
   * @author: Andrew Bachmann
   *
   * @exception gov.nasa.arc.l2tools.Exception The exception description.
   */
  private void compileAttributeTypes() throws gov.nasa.arc.l2tools.Exception {
    java.util.Enumeration attributeTypes=getAttributeTypes().elements();
    while (attributeTypes.hasMoreElements()) {
      java.lang.Object attributeType=attributeTypes.nextElement();
      if (attributeType instanceof gov.nasa.arc.l2tools.livingstone.code.componenttype.AttributeType) {
        ((gov.nasa.arc.l2tools.livingstone.code.componenttype.AttributeType)attributeType).compile();
      } else {
        String errorString="compiler error: attribute types are corrupted";
        throw new gov.nasa.arc.l2tools.livingstone.code.CompilerError(errorString);
      }
    }
  }
  /**
   *
   * Creation date: (3/23/2000 12:19:37 PM)
   * @author: Andrew Bachmann
   *
   * @exception gov.nasa.arc.l2tools.Exception The exception description.
   */
  private void compileConstraints() throws gov.nasa.arc.l2tools.Exception {
    java.util.Enumeration constraints=getConstraints().elements();
    setExpression(new gov.nasa.arc.l2tools.livingstone.code.logic.AndExpression());
    while (constraints.hasMoreElements()) {
      java.lang.Object constraint=constraints.nextElement();
      if (constraint instanceof gov.nasa.arc.l2tools.livingstone.code.constraint.Statement) {
        gov.nasa.arc.l2tools.livingstone.code.constraint.Statement statement
        = (gov.nasa.arc.l2tools.livingstone.code.constraint.Statement)constraint;
        statement.compile();
        getExpression().destructiveConjunction(statement.getExpression());
      } else {
        String errorString="compiler error: constraints are corrupted";
        throw new gov.nasa.arc.l2tools.livingstone.code.CompilerError(errorString);
      }
    }
  }
  /**
   *
   * Creation date: (3/23/2000 12:19:37 PM)
   * @author: Andrew Bachmann
   *
   * @exception gov.nasa.arc.l2tools.Exception The exception description.
   */
  private void compileFailures() throws gov.nasa.arc.l2tools.Exception {
    java.util.Enumeration failures=getFailures().elements();
    while (failures.hasMoreElements()) {
      java.lang.Object failure=failures.nextElement();
      if (failure instanceof gov.nasa.arc.l2tools.livingstone.code.componenttype.FailureTransition) {
        ((gov.nasa.arc.l2tools.livingstone.code.componenttype.FailureTransition)failure).compile();
      } else {
        String errorString="compiler error: failures are corrupted";
        throw new gov.nasa.arc.l2tools.livingstone.code.CompilerError(errorString);
      }
    }
  }
  /**
   *
   * Creation date: (3/23/2000 12:19:37 PM)
   * @author: Andrew Bachmann
   *
   * @exception gov.nasa.arc.l2tools.Exception The exception description.
   */
  private void compileRelations() throws gov.nasa.arc.l2tools.Exception {
    java.util.Enumeration relations=getRelations().elements();
    while (relations.hasMoreElements()) {
      java.lang.Object relation=relations.nextElement();
      if (relation instanceof gov.nasa.arc.l2tools.livingstone.code.Relation) {
        ((gov.nasa.arc.l2tools.livingstone.code.Relation)relation).compile();
      } else {
        String errorString="compiler error: relations are corrupted";
        throw new gov.nasa.arc.l2tools.livingstone.code.CompilerError(errorString);
      }
    }
  }
  /**
   *
   * Creation date: (3/23/2000 12:19:37 PM)
   * @author: Andrew Bachmann
   *
   * @exception gov.nasa.arc.l2tools.Exception The exception description.
   */
  private void compileStateVector() throws gov.nasa.arc.l2tools.Exception {
    if (getStateVector()!=null) {
      getStateVector().compile();
    } else {
      ComponentType componentType = this;
      while (componentType!=null) {
        if (componentType.getStateVector()!=null) {
          stateVector=componentType.getStateVector();
          return;
        }
        componentType = componentType.getSuperComponentType();
      }
    }
  }
  /** 
   * Creation date: (4/13/2001 12:07:00 PM)
   * @author: Andrew Bachmann
   * @exception gov.nasa.arc.l2tools.Exception The exception description.
   */
  public void compileSuper() throws gov.nasa.arc.l2tools.Exception {
    // null means no extends, nothing to do
    if (getSuper()==null) return;
    // find it
    gov.nasa.arc.l2tools.livingstone.DataObject superComponentType=findType(getSuper());
    // not here? bail
    if (superComponentType==null) {
      String errorString="find error: component type '"+getSuper()+"' specified in extends for component type '"+getName()+"' is undefined";
      throw new gov.nasa.arc.l2tools.Exception(errorString);
    }
    // not a component type? bail
    if (!(superComponentType instanceof ComponentType)) {
      String errorString2="syntax error: type '"+getSuper()+"' specified in extends for component type '"+getName()+"' is not a component type";
      throw new SyntaxError(errorString2);
    }
    // compile it
    ((ComponentType)superComponentType).compile();
    // now that we know it is okay, attach a pointer to it
    setSuperComponentType((ComponentType)superComponentType);
  }
  /**
   *
   * Creation date: (3/23/2000 12:19:37 PM)
   * @author: Andrew Bachmann
   *
   * @exception gov.nasa.arc.l2tools.Exception The exception description.
   */
  private void compileTransitions() throws gov.nasa.arc.l2tools.Exception {
    java.util.Enumeration transitions=getTransitions().elements();
    while (transitions.hasMoreElements()) {
      java.lang.Object transition=transitions.nextElement();
      if (transition instanceof gov.nasa.arc.l2tools.livingstone.code.componenttype.NominalTransition) {
        ((gov.nasa.arc.l2tools.livingstone.code.componenttype.NominalTransition)transition).compile();
      } else {
        String errorString="compiler error: transitions are corrupted";
        throw new gov.nasa.arc.l2tools.livingstone.code.CompilerError(errorString);
      }
    }
  }
  /**
   * Creation date: (4/4/2000 12:46:42 PM)
   * @author: Andrew Bachmann
   *
   * @return gov.nasa.arc.l2tools.livingstone.code.componenttype.Attribute
   * @param name java.lang.String
   */
  public gov.nasa.arc.l2tools.livingstone.code.componenttype.Attribute findAttributeDeclaration(String name) throws gov.nasa.arc.l2tools.Exception {
    // check ours first
    java.util.Enumeration attributes=getAttributes().elements();
    while (attributes.hasMoreElements()) {
      java.lang.Object attribute=attributes.nextElement();
      if (attribute instanceof gov.nasa.arc.l2tools.livingstone.code.componenttype.Attribute) {
        if (((gov.nasa.arc.l2tools.livingstone.code.componenttype.Attribute)attribute).getName().equals(name)) {
          return (gov.nasa.arc.l2tools.livingstone.code.componenttype.Attribute)attribute;
        }
      } else {
        String errorString="find error: attributes corrupted";
        throw new gov.nasa.arc.l2tools.Exception(errorString);
      }
    }
    // check those nested into attribute types
    java.util.Enumeration attributeTypes=getAttributeTypes().elements();
    while (attributeTypes.hasMoreElements()) {
      java.lang.Object attributeType=attributeTypes.nextElement();
      if (attributeType instanceof gov.nasa.arc.l2tools.livingstone.code.AbstractAttributeType) {
        java.util.Enumeration attributeTypeAttributes=((gov.nasa.arc.l2tools.livingstone.code.AbstractAttributeType)attributeType).getAttributes().elements();
        while (attributeTypeAttributes.hasMoreElements()) {
          java.lang.Object att=attributeTypeAttributes.nextElement();
          if (att instanceof gov.nasa.arc.l2tools.livingstone.code.componenttype.Attribute) {
            if (((gov.nasa.arc.l2tools.livingstone.code.componenttype.Attribute)att).getName().equals(name)) {
              return (gov.nasa.arc.l2tools.livingstone.code.componenttype.Attribute)att;
            }
          } else {
            String errorString="find error: attributes corrupted";
            throw new gov.nasa.arc.l2tools.Exception(errorString);
          }
        }
      } else {
        String errorString="instantiation error: attribute types corrupted";
        throw new InstantiationError(errorString);
      }
    }
    // check super
    if (getSuperComponentType()!=null) {
      gov.nasa.arc.l2tools.livingstone.code.componenttype.Attribute attribute
      = getSuperComponentType().findAttributeDeclaration(name);
      if (attribute!=null) return attribute;
    }
    return null;
  }
  /**
   * Creation date: (4/9/2000 3:51:18 PM)
   * @author: Andrew Bachmann
   *
   * @return gov.nasa.arc.l2tools.livingstone.Relation
   * @param name java.lang.String
   */
  public gov.nasa.arc.l2tools.livingstone.code.Relation findRelation(String name) throws gov.nasa.arc.l2tools.Exception {
    // safe not sorry
    compile();
    // look in my relations field
    java.util.Enumeration relations=getRelations().elements();
    while (relations.hasMoreElements()) {
      gov.nasa.arc.l2tools.livingstone.code.Relation relation=(gov.nasa.arc.l2tools.livingstone.code.Relation)relations.nextElement();
      if (name.equals(relation.getName()))
        return relation;
    }
    // look in my daddy's field
// functionality incorporated through compile() for ComponentType [which is done above]
//    compileSuper();
    if (getSuperComponentType()==null)
      return null;
    return getSuperComponentType().findRelation(name);
  }
  /**
   * Creation date: (4/4/2000 1:02:51 PM)
   * @author: Andrew Bachmann
   *
   * @return gov.nasa.arc.l2tools.livingstone.DataObject
   * @param name java.lang.String
   * @exception gov.nasa.arc.l2tools.Exception The exception description.
   */
  public gov.nasa.arc.l2tools.livingstone.DataObject findType(String name) throws gov.nasa.arc.l2tools.Exception {
    // check to see if it is in our containing class
    // note: as of 4/4/2000 the containing class of a component type must be a package
    //       but it may ultimately be allowed to be a component type and this is future compatible
    //       with the addition of one more "instanceof" check
    // TODO: add that
    gov.nasa.arc.l2tools.io.TreeObject climber=this;
    while ((climber!=null) && !(climber instanceof gov.nasa.arc.l2tools.Workspace)) {
      java.util.Enumeration children=climber.getChildren().elements();
      while (children.hasMoreElements()) {
        java.lang.Object child=children.nextElement();
        if (child instanceof gov.nasa.arc.l2tools.livingstone.ComponentType) {
          if (name.equals(((gov.nasa.arc.l2tools.livingstone.ComponentType)child).get("name"))) {
            return (gov.nasa.arc.l2tools.livingstone.ComponentType)child;
          }
        } else if (child instanceof gov.nasa.arc.l2tools.livingstone.AttributeType) {
          if (name.equals(((gov.nasa.arc.l2tools.livingstone.AttributeType)child).get("name"))) {
            return (gov.nasa.arc.l2tools.livingstone.AttributeType)child;
          }
        } else if (child instanceof gov.nasa.arc.l2tools.livingstone.code.componenttype.AttributeType) {
          if (name.equals(((gov.nasa.arc.l2tools.livingstone.code.componenttype.AttributeType)child).get("name"))) {
            return (gov.nasa.arc.l2tools.livingstone.code.componenttype.AttributeType)child;
          }
        }
      }
      climber=climber.getParent();
    }
    return null;
  }
  /**
   *
   * Creation date: (3/24/2000 10:17:42 AM)
   * @author: Andrew Bachmann
   *
   * @return javax.swing.DefaultListModel
   */
  public javax.swing.DefaultListModel getAttributes() {
    return attributes;
  }
  /**
   *
   * Creation date: (3/24/2000 10:18:08 AM)
   * @author: Andrew Bachmann
   *
   * @return javax.swing.DefaultListModel
   */
  public javax.swing.DefaultListModel getAttributeTypes() {
    return attributeTypes;
  }
  /**
   *
   * Creation date: (3/24/2000 9:27:33 AM)
   * @author: Andrew Bachmann
   *
   * @return javax.swing.DefaultListModel
   */
  public javax.swing.DefaultListModel getConstraints() {
    return constraints;
  }
  /**
   *
   * Creation date: (3/24/2000 10:16:42 AM)
   * @author: Andrew Bachmann
   *
   * @return javax.swing.DefaultListModel
   */
  public javax.swing.DefaultListModel getFailures() {
    return failures;
  }
  /**
   *
   * Creation date: (3/10/2000 4:28:40 PM)
   * @author: Andrew Bachmann
   *
   * @return java.lang.String
   */
  public java.lang.String getName() {
    return name;
  }
  /**
   *
   * Creation date: (3/24/2000 10:15:55 AM)
   * @author: Andrew Bachmann
   *
   * @return javax.swing.DefaultListModel
   */
  public javax.swing.DefaultListModel getRelations() {
    return relations;
  }
  /**
   *
   * Creation date: (3/24/2000 10:17:03 AM)
   * @author: Andrew Bachmann
   *
   * @return gov.nasa.arc.l2tools.livingstone.code.componenttype.StateVector
   */
  public gov.nasa.arc.l2tools.livingstone.code.componenttype.StateVector getStateVector() {
    return stateVector;
  }
  /**
   * Creation date: (4/4/2000 12:52:10 PM)
   * @author: Andrew Bachmann
   *
   * @return java.lang.String
   */
  public java.lang.String getSuper() {
    return superS;
  }
  /**
   * Creation date: (4/4/2000 1:17:38 PM)
   * @author: Andrew Bachmann
   *
   * @return gov.nasa.arc.l2tools.livingstone.ComponentType
   */
  public ComponentType getSuperComponentType() {
    return superComponentType;
  }
  /**
   *
   * Creation date: (3/24/2000 10:16:19 AM)
   * @author: Andrew Bachmann
   *
   * @return javax.swing.DefaultListModel
   */
  public javax.swing.DefaultListModel getTransitions() {
    return transitions;
  }
  /**
   * Creation date: (5/3/2000 5:01:15 PM)
   * @author: Andrew Bachmann
   *
   * @param instance gov.nasa.arc.l2tools.livingstone.ComponentInstance
   */
  private void groupAttributeTypes(ComponentInstance instance) {
    int index=0;
    while (index<instance.getChildren().getSize()) {
      java.lang.Object child=instance.getChildren().getElementAt(index);
      if (child instanceof gov.nasa.arc.l2tools.livingstone.componentinstance.AttributeType) {
        instance.getChildren().removeElementAt(index);
        instance.getChildren().insertElementAt(child,0);
      }
      index++;
    }
  }
  /**
   *
   * Creation date: (3/22/2000 2:03:39 PM)
   * @author: Andrew Bachmann
   */
  public gov.nasa.arc.l2tools.livingstone.componentinstance.Component instantiate(String name, ComponentInstance componentInstance)
  throws gov.nasa.arc.l2tools.Exception {
      return instantiate(name,componentInstance,false);
  }
  public gov.nasa.arc.l2tools.livingstone.componentinstance.Component instantiate(String name, ComponentInstance componentInstance, boolean optimize)
  throws gov.nasa.arc.l2tools.Exception {
    try {
      // just in case
      compile();
      // make the attachment
// functionality incorporated through compile() for ComponentType
//      compileSuper();
      // check to see if its already done
      gov.nasa.arc.l2tools.livingstone.componentinstance.Component component
      = componentInstance.findComponent(name);
      if (component!=null) return component;
      // make instantiated class
      component = new gov.nasa.arc.l2tools.livingstone.componentinstance.Component(name,componentInstance,this);
      // instantiate stuff
      instantiateStateVector(component);
      ComponentType componentType=this;
      while (componentType!=null) {
        // unoptimized one always makes all fields
        if (!optimize) {
          componentType.instantiateAttributes(component);
        }
        componentType.instantiateTransitions(component);
        componentType.instantiateFailures(component);
        componentType.instantiateExpression(component);
        // instantiate superclass if necessary
        componentType=componentType.getSuperComponentType();
      }
      instantiateFrameAxioms(component);
      // done
      return component;
    } catch (gov.nasa.arc.l2tools.Exception e) {
      e.concatMessage("\n  ");
      if (getLine()!=null) {
        e.concatMessage("line "+getLine()+": ");
      }
      e.concatMessage("while instantiating component '"+name+"'");
      if (getName()!=null) {
        e.concatMessage(" of type '"+getName()+"'");
      }
      throw e;
    }
  }
  private void instantiateAttributes(gov.nasa.arc.l2tools.livingstone.componentinstance.Component component)
  throws gov.nasa.arc.l2tools.Exception {
    java.util.Enumeration attributes=getAttributes().elements();
    while (attributes.hasMoreElements()) {
      java.lang.Object attribute=attributes.nextElement();
      if (attribute instanceof gov.nasa.arc.l2tools.livingstone.code.componenttype.Attribute) {
        gov.nasa.arc.l2tools.livingstone.code.componenttype.Attribute attributeDeclaration =
          (gov.nasa.arc.l2tools.livingstone.code.componenttype.Attribute)attribute;
        attributeDeclaration.compile();
        gov.nasa.arc.l2tools.livingstone.code.componenttype.Name name = 
          new gov.nasa.arc.l2tools.livingstone.code.componenttype.Name(attributeDeclaration.getName());
        gov.nasa.arc.l2tools.livingstone.componentinstance.Attribute attributeInstance =
          name.instantiate(component);
        attributeDeclaration.compileType();
        gov.nasa.arc.l2tools.livingstone.DataObject attributeType =
          attributeDeclaration.getTypeType();
        if (attributeType instanceof gov.nasa.arc.l2tools.livingstone.ComponentType) {
          gov.nasa.arc.l2tools.livingstone.ComponentType instantiatedComponentType =
            attributeInstance.getComponentType();
          if (instantiatedComponentType == null) {
            String errorString="syntax error: field redefined as an incompatible type";
            errorString+="\n  field '"+attributeDeclaration.getName()+"'";
            errorString+="\n    originally defined as component type '";
            errorString+=((gov.nasa.arc.l2tools.livingstone.ComponentType)attributeType).getName()+"'";
            errorString+="\n    redefined as attribute type '";
            errorString+=attributeInstance.getAbstractAttributeType().getName()+"'";
            throw new gov.nasa.arc.l2tools.livingstone.SyntaxError(errorString);
          } else {
            gov.nasa.arc.l2tools.livingstone.ComponentType climber = instantiatedComponentType;
            while (climber != null) {
              if (climber == attributeType) {
                break;
              }
              climber = climber.getSuperComponentType();
            }
            if (climber != attributeType) {
              String errorString="syntax error: field redefined as an incompatible type";
              errorString+="\n  field '"+attributeDeclaration.getName()+"'";
              errorString+="\n    originally defined as component type '";
              errorString+=((gov.nasa.arc.l2tools.livingstone.ComponentType)attributeType).getName()+"'";
              errorString+="\n    redefined as component type '";
              errorString+=attributeInstance.getComponentType().getName()+"'";
              throw new gov.nasa.arc.l2tools.livingstone.SyntaxError(errorString);
            }             
          }          
        } else if (attributeType instanceof gov.nasa.arc.l2tools.livingstone.code.AbstractAttributeType) {
          gov.nasa.arc.l2tools.livingstone.code.AbstractAttributeType instantiateAttributeType =
            attributeInstance.getAbstractAttributeType();
          if (attributeType != instantiateAttributeType) {
            String errorString="syntax error: field redefined as an incompatible type";
            errorString+="\n  field '"+attributeDeclaration.getName()+"'";
            errorString+="\n    originally defined as type '";
            errorString+=((gov.nasa.arc.l2tools.livingstone.code.AbstractAttributeType)attributeType).getName()+"'";
            if (instantiateAttributeType == null) {
                errorString+="\n    redefined as component type '";
                errorString+=attributeInstance.getComponentType().getName()+"'";
            } else {
                errorString+="\n    redefined as attribute type '";
                errorString+=instantiateAttributeType.getName()+"'";
            }
            throw new gov.nasa.arc.l2tools.livingstone.SyntaxError(errorString);
          }
        }
      } else {
        String errorString="compiler error: attributes corrupted";
        throw new gov.nasa.arc.l2tools.livingstone.code.CompilerError(errorString);
      }
    }
  }  
  /**
   * Creation date: (4/8/2000 10:07:36 PM)
   * @author: Andrew Bachmann
   *
   * @param component gov.nasa.arc.l2tools.livingstone.componentinstance.Component
   * @exception gov.nasa.arc.l2tools.Exception The exception description.
   */
  private void instantiateExpression(gov.nasa.arc.l2tools.livingstone.componentinstance.Component component)
  throws gov.nasa.arc.l2tools.Exception {
    if (getExpression()!=null) {
      getExpression().instantiate(component);
    }
  }
  /**
   * Creation date: (4/8/2000 10:07:36 PM)
   * @author: Andrew Bachmann
   *
   * @param component gov.nasa.arc.l2tools.livingstone.componentinstance.Component
   * @exception gov.nasa.arc.l2tools.Exception The exception description.
   */
  private void instantiateFailures(gov.nasa.arc.l2tools.livingstone.componentinstance.Component component)
  throws gov.nasa.arc.l2tools.Exception {
    // failures
    java.util.Enumeration failures=getFailures().elements();
    while (failures.hasMoreElements()) {
      gov.nasa.arc.l2tools.livingstone.code.componenttype.FailureTransition failure
      = (gov.nasa.arc.l2tools.livingstone.code.componenttype.FailureTransition)failures.nextElement();
      failure.instantiate(component);
    }
  }
  /**
   * Creation date: (8/4/2000 5:20:06 PM)
   * @author: Andrew Bachmann
   *
   * @param component gov.nasa.arc.l2tools.livingstone.componentinstance.Component
   * @exception java.lang.Exception The exception description.
   */
  private void instantiateFrameAxioms(gov.nasa.arc.l2tools.livingstone.componentinstance.Component component)
  throws gov.nasa.arc.l2tools.Exception {
    int i; // general purpose reused iterator variable
    // no state vector means you can quit
    if (getStateVector()==null) return;
    // create the state space
    gov.nasa.arc.l2tools.livingstone.compiler.StateSpace stateSpace
    = new gov.nasa.arc.l2tools.livingstone.compiler.StateSpace(this,component);
    stateSpace.addFrameAxioms(component);
  }
  /**
   * Creation date: (4/8/2000 10:07:36 PM)
   * @author: Andrew Bachmann
   *
   * @return gov.nasa.arc.l2tools.livingstone.componentinstance.StateVector
   * @param component gov.nasa.arc.l2tools.livingstone.componentinstance.Component
   * @exception gov.nasa.arc.l2tools.Exception The exception description.
   */
  private gov.nasa.arc.l2tools.livingstone.componentinstance.StateVector instantiateStateVector(gov.nasa.arc.l2tools.livingstone.componentinstance.Component component)
  throws gov.nasa.arc.l2tools.Exception {
    // state vector
    if (getStateVector()!=null) {
      return getStateVector().instantiate(component);
    }
    return null;
  }
  /**
   * Creation date: (4/8/2000 10:07:36 PM)
   * @author: Andrew Bachmann
   *
   * @param component gov.nasa.arc.l2tools.livingstone.componentinstance.Component
   * @exception gov.nasa.arc.l2tools.Exception The exception description.
   */
  private void instantiateTransitions(gov.nasa.arc.l2tools.livingstone.componentinstance.Component component)
  throws gov.nasa.arc.l2tools.Exception {
    // transitions
    java.util.Enumeration transitions=getTransitions().elements();
    while (transitions.hasMoreElements()) {
      gov.nasa.arc.l2tools.livingstone.code.componenttype.NominalTransition transition
      = (gov.nasa.arc.l2tools.livingstone.code.componenttype.NominalTransition)transitions.nextElement();
      transition.instantiate(component);
    }
  }
  /**
   *
   * Creation date: (3/10/2000 7:09:36 PM)
   * @author: Andrew Bachmann
   *
   * @exception gov.nasa.arc.l2tools.Exception The exception description.
   */
  public void refreshCache() throws gov.nasa.arc.l2tools.Exception {
    try {
      // cache attributes
      if ((get("name") != null) && (get("name") != "")) {
        setName(get("name"));
      } else {
        String errorString = "illegal anonymous error: each component type at the top level must have a name";
        throw new gov.nasa.arc.l2tools.livingstone.code.IllegalAnonymousError(errorString);
      }
      if ((get("extends") != null) && (get("extends") != "")) {
        setSuper(get("extends"));
      }
      
      // cache children
      java.util.Enumeration children = getChildren().elements();
      while (children.hasMoreElements()) {
        java.lang.Object child = children.nextElement();
        if (child instanceof gov.nasa.arc.l2tools.livingstone.code.componenttype.Attribute) {
          cacheAttribute((gov.nasa.arc.l2tools.livingstone.code.componenttype.Attribute)child);
        } else
          if (child instanceof gov.nasa.arc.l2tools.livingstone.code.componenttype.StateVector) {
            cacheStateVector((gov.nasa.arc.l2tools.livingstone.code.componenttype.StateVector)child);
          } else
            if (child instanceof gov.nasa.arc.l2tools.livingstone.code.componenttype.AttributeType) {
              cacheAttributeType((gov.nasa.arc.l2tools.livingstone.code.componenttype.AttributeType)child);
            } else
              if (child instanceof gov.nasa.arc.l2tools.livingstone.code.componenttype.NominalTransition) {
                cacheTransition((gov.nasa.arc.l2tools.livingstone.code.componenttype.NominalTransition)child);
              } else
                if (child instanceof gov.nasa.arc.l2tools.livingstone.code.componenttype.FailureTransition) {
                  cacheFailure((gov.nasa.arc.l2tools.livingstone.code.componenttype.FailureTransition)child);
                } else
                  if (child instanceof gov.nasa.arc.l2tools.livingstone.code.Relation) {
                    cacheRelation((gov.nasa.arc.l2tools.livingstone.code.Relation)child);
                  } else
                    if (child instanceof gov.nasa.arc.l2tools.livingstone.code.constraint.StatementGroup) {
                      cacheStatementGroup((gov.nasa.arc.l2tools.livingstone.code.constraint.StatementGroup)child);
                    } else {
                      //String errorString="unexpected child error: a child should be one of:";
                      //errorString+="\n  attribute, state vector, attribute type, nominal transition, failure transition, relation";
                      //throw new gov.nasa.arc.l2tools.livingstone.code.UnexpectedChildError(errorString);
                    }
      }
    } catch (gov.nasa.arc.l2tools.Exception error) {
      error.concatMessage("\n  ");
      if (getLine()!=null) {
        error.concatMessage("line "+getLine()+": ");
      }
      error.concatMessage("in component type");
      if (getName()!=null) {
        error.concatMessage(": "+getName());
      }
      throw error;
    }
  }
  /**
   * refreshClauses method comment.
   */
  public void refreshExpression() throws gov.nasa.arc.l2tools.Exception {
    try {
      compileSuper();
      compileAttributeTypes();
      compileAttributes();
      compileStateVector();
      compileRelations();
      compileTransitions();
      compileFailures();
      compileConstraints();
    } catch (gov.nasa.arc.l2tools.Exception error) {
      error.concatMessage("\n	in component type");
      if (getName()!=null) error.concatMessage(": "+getName());
      throw error;
    }
  }
  /**
   * search for a ComponentType with value <i>value</i> for key <i>key</i>
   * Creation date: (10/24/2000 4:05:22 PM)
   * @return gov.nasa.arc.l2tools.livingstone.ComponentType
   * @param key java.lang.String
   * @param value java.lang.String
   * @param object gov.nasa.arc.l2tools.DataObject
   */
  public static ComponentType search(String key, String value, gov.nasa.arc.l2tools.DataObject object) {
    return (ComponentType)object.search(ComponentType.class,key,value);
  }
  /**
   * search for a ComponentType named <i>name</i>
   * Creation date: (10/24/2000 4:05:22 PM)
   * @return gov.nasa.arc.l2tools.livingstone.ComponentType
   * @param name java.lang.String
   * @param object gov.nasa.arc.l2tools.DataObject
   */
  public static ComponentType searchByName(String name, gov.nasa.arc.l2tools.DataObject object) {
    return (ComponentType)object.searchByName(ComponentType.class,name);
  }
  /**
   *
   * Creation date: (3/24/2000 10:17:42 AM)
   * @author: Andrew Bachmann
   *
   * @param newAttributes javax.swing.DefaultListModel
   */
  protected void setAttributes(javax.swing.DefaultListModel newAttributes) {
    attributes = newAttributes;
  }
  /**
   *
   * Creation date: (3/24/2000 10:18:08 AM)
   * @author: Andrew Bachmann
   *
   * @param newAttributeTypes javax.swing.DefaultListModel
   */
  protected void setAttributeTypes(javax.swing.DefaultListModel newAttributeTypes) {
    attributeTypes = newAttributeTypes;
  }
  /**
   *
   * Creation date: (3/24/2000 9:27:33 AM)
   * @author: Andrew Bachmann
   *
   * @param newConstraints javax.swing.DefaultListModel
   */
  protected void setConstraints(javax.swing.DefaultListModel newConstraints) {
    constraints = newConstraints;
  }
  /**
   *
   * Creation date: (3/24/2000 10:16:42 AM)
   * @author: Andrew Bachmann
   *
   * @param newFailures javax.swing.DefaultListModel
   */
  protected void setFailures(javax.swing.DefaultListModel newFailures) {
    failures = newFailures;
  }
  /**
   *
   * Creation date: (3/10/2000 4:28:40 PM)
   * @author: Andrew Bachmann
   *
   * @param newName java.lang.String
   */
  protected void setName(java.lang.String newName) {
    name = newName;
  }
  /**
   *
   * Creation date: (3/24/2000 10:15:55 AM)
   * @author: Andrew Bachmann
   *
   * @param newRelations javax.swing.DefaultListModel
   */
  protected void setRelations(javax.swing.DefaultListModel newRelations) {
    relations = newRelations;
  }
  /**
   *
   * Creation date: (3/24/2000 10:17:03 AM)
   * @author: Andrew Bachmann
   *
   * @param newStateVector gov.nasa.arc.l2tools.livingstone.code.componenttype.StateVector
   */
  protected void setStateVector(gov.nasa.arc.l2tools.livingstone.code.componenttype.StateVector newStateVector) {
    stateVector = newStateVector;
  }
  /**
   * Creation date: (4/4/2000 12:52:10 PM)
   * @author: Andrew Bachmann
   *
   * @param newSuperS java.lang.String
   */
  protected void setSuper(java.lang.String newSuperS) {
    superS = newSuperS;
  }
  /**
   * Creation date: (4/4/2000 1:17:38 PM)
   * @author: Andrew Bachmann
   *
   * @param newSuperComponentType gov.nasa.arc.l2tools.livingstone.ComponentType
   */
  protected void setSuperComponentType(ComponentType newSuperComponentType) {
    superComponentType = newSuperComponentType;
  }
  /**
   *
   * Creation date: (3/24/2000 10:16:19 AM)
   * @author: Andrew Bachmann
   *
   * @param newTransitions javax.swing.DefaultListModel
   */
  protected void setTransitions(javax.swing.DefaultListModel newTransitions) {
    transitions = newTransitions;
  }
}

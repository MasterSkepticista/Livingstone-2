//
// * See the file "l2-tools/disclaimers-and-notices.txt" for 
// * information on usage and redistribution of this file, 
// * and for a DISCLAIMER OF ALL WARRANTIES.
//
package gov.nasa.arc.l2tools.livingstone.compiler;

/**
 * Creation date: (8/7/2000 4:06:33 PM)
 * @author: Andrew Bachmann
 */
public class StateSpace {
	public int stateVectorLength; // all arrays are this long
	public int[] dimensions; // dimensions of the constraints array
	public java.util.Hashtable[] valueHashtables; // names -> numbers
	public java.util.Vector[] valueTables; // numbers -> names
	public java.util.Vector[] constraints; // huge array with constraints in it
public StateSpace(gov.nasa.arc.l2tools.livingstone.ComponentType componentType, gov.nasa.arc.l2tools.livingstone.componentinstance.Component component) 
throws gov.nasa.arc.l2tools.Exception {
	stateVectorLength = componentType.getStateVector().getAttributes().getSize();
	initializeValueCaches(componentType);
	initializeConstraints();
	cacheTransitions(componentType,component);
}
/**
 * Creation date: (8/7/2000 4:40:16 PM)
 * @author: Andrew Bachmann
 * 
 */
public void addFrameAxioms(gov.nasa.arc.l2tools.livingstone.componentinstance.Component component) 
throws gov.nasa.arc.l2tools.Exception {
	// iterator
	int i;
	// now go through the entire state space and add frame axioms
	int[] valueIndices=new int[stateVectorLength];
	for(i=0;(i<stateVectorLength);i++) {
		valueIndices[i]=0;
	}
	boolean done=false;
	while (!done) {
		java.util.Vector constraintsVector=getConstraints(valueIndices);
		if (constraintsVector!=null) {
			gov.nasa.arc.l2tools.livingstone.componentinstance.DefaultTransition transition
			  = new gov.nasa.arc.l2tools.livingstone.componentinstance.DefaultTransition(component);
			// make state vector
			String stateVectorString=new String("");
			for (i=0;(i<stateVectorLength);i++) {
				stateVectorString+=valueTables[i].get(valueIndices[i]);
				if (i+1<stateVectorLength) stateVectorString+=" ";
			}
			transition.set("from",stateVectorString);
			transition.set("to",stateVectorString);
			// construct frame axiom condition
			gov.nasa.arc.l2tools.livingstone.code.logic.OrExpression frameAxiomCondition
			  = new gov.nasa.arc.l2tools.livingstone.code.logic.OrExpression();
			java.util.Enumeration constraints=constraintsVector.elements();
			while (constraints.hasMoreElements()) {
				java.lang.Object constraint=constraints.nextElement();
				if (constraint instanceof gov.nasa.arc.l2tools.livingstone.code.logic.BasicExpression) {
					frameAxiomCondition.destructiveDisjunction((gov.nasa.arc.l2tools.livingstone.code.logic.BasicExpression)constraint);
				}
			}
			// install the frame axiom
			frameAxiomCondition.instantiate(transition);
		}
		// count to the next step, see if we are done
		done=true;
		for(int n=0;(n<stateVectorLength);n++) {
			int nextIndex=valueIndices[n]+1;
			if (nextIndex>=dimensions[n]) {
				valueIndices[n]=0;
			} else {
				valueIndices[n]=nextIndex;
				done=false;
				break;
			}
		}
	}
}
/**
 * Creation date: (8/7/2000 4:26:38 PM)
 * @author: Andrew Bachmann
 * 
 */
public void buildValueTables() {
	// construct the table of possible values
	java.util.Vector[] valueTable=new java.util.Vector[stateVectorLength];
	// iterate down the variables in the state vector
	for(int i=0;(i<stateVectorLength);i++) {
		// create a new vector for the values for this variable
		valueTable[i]=new java.util.Vector();
		// go through the hashtable created for the values of this variable
		java.util.Iterator hashtableIterator=valueHashtables[i].entrySet().iterator();
		while (hashtableIterator.hasNext()) {
			java.util.Map.Entry entry=(java.util.Map.Entry)hashtableIterator.next();
			// get the position of this value in the definition
			int value=((Integer)entry.getValue()).intValue();
			// if the vector needs to be bigger, make it bigger
			if (value>=valueTable[i].size()) {
				valueTable[i].setSize(value+1);
			}
			// add the value in at it's appropriate location
			valueTable[i].setElementAt(entry.getKey(),value);
		}
	}
}
public void cacheTransition(gov.nasa.arc.l2tools.livingstone.code.componenttype.NominalTransition transition) 
throws gov.nasa.arc.l2tools.Exception {
	// construct the state vector indices
	int[] indices = new int[stateVectorLength];
	java.util.Vector wildcards = new java.util.Vector(); // keep track of any variables that can vary
	java.util.Enumeration fromStates = transition.getFromStates().elements();
	for (int i=0;(i<stateVectorLength);i++) {
		if (fromStates.hasMoreElements()) {
			String fromState = (String) fromStates.nextElement();
			Integer valueNum;
			if (fromState.equals("*")) {
				wildcards.add(new Integer(i));
				valueNum = new Integer(0);
			} else {
				valueNum = (Integer)valueHashtables[i].get(fromState);
			}
			if (valueNum == null) {
				gov.nasa.arc.l2tools.livingstone.SyntaxError error 
				  = new gov.nasa.arc.l2tools.livingstone.SyntaxError("syntax error: value in from state is not a valid value for the variable in the state vector");
				error.concatMessage("\n  value '" + fromState + "' is invalid");
				throw error;
			}
			indices[i]=valueNum.intValue();
		}
	}
	boolean done=false;
	while (!done) {
		java.util.Vector constraints=getConstraints(indices);
		if (constraints!=null) {
			gov.nasa.arc.l2tools.livingstone.code.logic.BasicExpression expression
			  = transition.getExpandedAndInstantiatedExpression();
			if (expression instanceof gov.nasa.arc.l2tools.livingstone.code.logic.TrueExpression) {
				setConstraints(indices,null);
			} else {
				constraints.addElement(expression);
			}
		}
		done=true;
		java.util.Enumeration wildcardEnumeration=wildcards.elements();
		while (wildcardEnumeration.hasMoreElements()) {
			Integer wildcard=(Integer)wildcardEnumeration.nextElement();
			int nextIndex=indices[wildcard.intValue()]+1;
			if (nextIndex>=dimensions[wildcard.intValue()]) {
				indices[wildcard.intValue()]=0;
			} else {
				indices[wildcard.intValue()]=nextIndex;
				done=false;
				break;
			}
		}
	}	
}
public void cacheTransitions(gov.nasa.arc.l2tools.livingstone.ComponentType componentType, gov.nasa.arc.l2tools.livingstone.componentinstance.Component component) 
throws gov.nasa.arc.l2tools.Exception {
  // go through the heirarchy to cache at all levels
  while (componentType != null) {
	  // go through the transitions and cache the constraints in the state space
	  java.util.Enumeration transitions=componentType.getTransitions().elements();
	  while (transitions.hasMoreElements()) {
  		java.lang.Object transition=transitions.nextElement();
  		if (transition instanceof gov.nasa.arc.l2tools.livingstone.code.componenttype.NominalTransition) {
  			cacheTransition((gov.nasa.arc.l2tools.livingstone.code.componenttype.NominalTransition)transition);
  		}
	  }
    componentType = componentType.getSuperComponentType();
  }
}
public java.util.Vector getConstraints(int[] indices) {
	int index = 0;
	for (int i = stateVectorLength - 1;(i >= 0); i--) {
		index *= dimensions[i];
		index += indices[i];
	}
	return constraints[index];
}
public void initializeConstraints() {
	int i;
	constraints = (java.util.Vector[]) java.lang.reflect.Array.newInstance(java.util.Vector.class, dimensions);
	// initialize the state space
	int maxFlat = 1;
	for (i = 0;(i < stateVectorLength); i++) {
		maxFlat *= dimensions[i];
	}
	for (i = 0;(i < maxFlat); i++) {
		constraints[i] = new java.util.Vector();
	}
}
/**
 * Creation date: (8/7/2000 4:20:37 PM)
 * @author: Andrew Bachmann
 * 
 */
public void initializeValueCaches(gov.nasa.arc.l2tools.livingstone.ComponentType componentType) 
throws gov.nasa.arc.l2tools.Exception {
	// initialize arrays
	dimensions = new int[stateVectorLength];
	valueHashtables = new java.util.Hashtable[stateVectorLength];
	valueTables = new java.util.Vector[stateVectorLength];
	// we'll go through each variable in the state vector and find out what its type is to create the array
	for(int i=0;(i<stateVectorLength);i++) {
		String variableName=(String)componentType.getStateVector().getAttributes().get(i);
		// find where it is declared
		gov.nasa.arc.l2tools.livingstone.code.componenttype.Attribute attribute = componentType.findAttributeDeclaration(variableName);
		// get the type out of the declaration
		gov.nasa.arc.l2tools.livingstone.DataObject attributeType=attribute.getTypeType();
		// make sure the type is valid for a state vector variable
		if (attributeType instanceof gov.nasa.arc.l2tools.livingstone.code.AbstractAttributeType) {
			// get the size for one of the array dimensions
			int attributeTypeSize=((gov.nasa.arc.l2tools.livingstone.code.AbstractAttributeType)attributeType).getValues().getSize();
			// save it in the state space dimensions vector
			dimensions[i]=attributeTypeSize;
			valueHashtables[i]=new java.util.Hashtable();
			valueTables[i]=new java.util.Vector();
			// cache the names, and their location in the list of values
			java.util.Enumeration values=((gov.nasa.arc.l2tools.livingstone.code.AbstractAttributeType)attributeType).getValues().elements();
			int j=0;
			while (values.hasMoreElements()) {
				String value=((gov.nasa.arc.l2tools.livingstone.code.AbstractValue)values.nextElement()).getName();
				valueHashtables[i].put(value,new Integer(j));
				valueTables[i].add(value);
				j++;
			}
		} else {
			gov.nasa.arc.l2tools.livingstone.SyntaxError error
			  = new gov.nasa.arc.l2tools.livingstone.SyntaxError("syntax error: type of attribute in state vector is not an attribute type");
			error.concatMessage("\n  attribute type '"+variableName+"'");
		}
	}
}
public void setConstraints(int[] indices, java.util.Vector value) {
	int index = 0;
	for (int i = stateVectorLength - 1;(i >= 0); i--) {
		index *= dimensions[i];
		index += indices[i];
	}
	constraints[index]=value;
}
}

//
// * See the file "l2-tools/disclaimers-and-notices.txt" for 
// * information on usage and redistribution of this file, 
// * and for a DISCLAIMER OF ALL WARRANTIES.
//
package gov.nasa.arc.l2tools.io.xmpl;

import fr.dyade.koala.xml.domlight.XMLElementFactory;
/**
 * Insert the type's description here.
 * Creation date: (2/9/2000 3:03:25 PM)
 * @author: Andrew Bachmann
 */
public class XmplElementFactoryImp implements XMLElementFactory {
/**
 * XmlElementFactoryImp constructor comment.
 */
public XmplElementFactoryImp() {
	super();
}
/**
 *
 * Creation date: (2/9/2000 3:07:43 PM)
 * @author: Andrew Bachmann
 * 
 * @return java.lang.Class
 * @param tagName java.lang.String
 * @exception java.lang.ClassNotFoundException The exception description.
 */
public Class classFor(String tagName) throws java.lang.ClassNotFoundException {
	// XMPL container tag
	if (tagName.equals(XmplContainer.tagName))
	  return XmplContainer.class;
	
	// from the gov.nasa.arc.l2tools.livingstone.code.constraint package
	if (tagName.equals(gov.nasa.arc.l2tools.livingstone.code.constraint.AndExpression.tagName))
		return gov.nasa.arc.l2tools.livingstone.code.constraint.AndExpression.class;
	if (tagName.equals(gov.nasa.arc.l2tools.livingstone.code.constraint.CaseGroup.tagName))
		return gov.nasa.arc.l2tools.livingstone.code.constraint.CaseGroup.class;
	if (tagName.equals(gov.nasa.arc.l2tools.livingstone.code.constraint.ConstraintInvocation.tagName))
		return gov.nasa.arc.l2tools.livingstone.code.constraint.ConstraintInvocation.class;
	if (tagName.equals(gov.nasa.arc.l2tools.livingstone.code.constraint.EqualityExpression.tagName))
		return gov.nasa.arc.l2tools.livingstone.code.constraint.EqualityExpression.class;
	if (tagName.equals(gov.nasa.arc.l2tools.livingstone.code.constraint.Expression.tagName))
		return gov.nasa.arc.l2tools.livingstone.code.constraint.Expression.class;
	if (tagName.equals(gov.nasa.arc.l2tools.livingstone.code.constraint.IffStatement.tagName))
		return gov.nasa.arc.l2tools.livingstone.code.constraint.IffStatement.class;
	if (tagName.equals(gov.nasa.arc.l2tools.livingstone.code.constraint.IfStatement.tagName))
		return gov.nasa.arc.l2tools.livingstone.code.constraint.IfStatement.class;
	if (tagName.equals(gov.nasa.arc.l2tools.livingstone.code.constraint.InequalityExpression.tagName))
		return gov.nasa.arc.l2tools.livingstone.code.constraint.InequalityExpression.class;
	if (tagName.equals(gov.nasa.arc.l2tools.livingstone.code.constraint.NegationExpression.tagName))
		return gov.nasa.arc.l2tools.livingstone.code.constraint.NegationExpression.class;
	if (tagName.equals(gov.nasa.arc.l2tools.livingstone.code.constraint.OrExpression.tagName))
		return gov.nasa.arc.l2tools.livingstone.code.constraint.OrExpression.class;
	if (tagName.equals(gov.nasa.arc.l2tools.livingstone.code.constraint.Statement.tagName))
		return gov.nasa.arc.l2tools.livingstone.code.constraint.Statement.class;
	if (tagName.equals(gov.nasa.arc.l2tools.livingstone.code.constraint.StatementGroup.tagName))
		return gov.nasa.arc.l2tools.livingstone.code.constraint.StatementGroup.class;
	if (tagName.equals(gov.nasa.arc.l2tools.livingstone.code.constraint.SwitchStatement.tagName))
		return gov.nasa.arc.l2tools.livingstone.code.constraint.SwitchStatement.class;
	
  // from the gov.nasa.arc.l2tools.livingstone.code.attributetype package
  if (tagName.equals(gov.nasa.arc.l2tools.livingstone.code.attributetype.Value.tagName))
  	return gov.nasa.arc.l2tools.livingstone.code.attributetype.Value.class;
  if (tagName.equals(gov.nasa.arc.l2tools.livingstone.code.componenttype.Value.tagName))
  	return gov.nasa.arc.l2tools.livingstone.code.componenttype.Value.class;

  // from the gov.nasa.arc.l2tools.livingstone.code.componenttype package
  if (tagName.equals(gov.nasa.arc.l2tools.livingstone.code.componenttype.AttributeType.tagName))
  	return gov.nasa.arc.l2tools.livingstone.code.componenttype.AttributeType.class;
  if (tagName.equals(gov.nasa.arc.l2tools.livingstone.code.componenttype.FailureProbability.tagName))
  	return gov.nasa.arc.l2tools.livingstone.code.componenttype.FailureProbability.class;
  if (tagName.equals(gov.nasa.arc.l2tools.livingstone.code.componenttype.FailureTransition.tagName))
  	return gov.nasa.arc.l2tools.livingstone.code.componenttype.FailureTransition.class;
  if (tagName.equals(gov.nasa.arc.l2tools.livingstone.code.componenttype.FromStateIdentifier.tagName))
  	return gov.nasa.arc.l2tools.livingstone.code.componenttype.FromStateIdentifier.class;
	if (tagName.equals(gov.nasa.arc.l2tools.livingstone.code.componenttype.NominalTransition.tagName))
		return gov.nasa.arc.l2tools.livingstone.code.componenttype.NominalTransition.class;
	if (tagName.equals(gov.nasa.arc.l2tools.livingstone.code.componenttype.Particle.tagName))
		return gov.nasa.arc.l2tools.livingstone.code.componenttype.Particle.class;
	if (tagName.equals(gov.nasa.arc.l2tools.livingstone.code.componenttype.StateIdentifier.tagName))
		return gov.nasa.arc.l2tools.livingstone.code.componenttype.StateIdentifier.class;
	if (tagName.equals(gov.nasa.arc.l2tools.livingstone.code.componenttype.StateVector.tagName))
		return gov.nasa.arc.l2tools.livingstone.code.componenttype.StateVector.class;
	if (tagName.equals(gov.nasa.arc.l2tools.livingstone.code.componenttype.ToStateIdentifier.tagName))
		return gov.nasa.arc.l2tools.livingstone.code.componenttype.ToStateIdentifier.class;
	if (tagName.equals(gov.nasa.arc.l2tools.livingstone.code.componenttype.AbstractTransition.tagName))
		return gov.nasa.arc.l2tools.livingstone.code.componenttype.AbstractTransition.class;
	if (tagName.equals(gov.nasa.arc.l2tools.livingstone.code.componenttype.TransitionCost.tagName))
		return gov.nasa.arc.l2tools.livingstone.code.componenttype.TransitionCost.class;
	if (tagName.equals(gov.nasa.arc.l2tools.livingstone.code.componenttype.Attribute.tagName))
		return gov.nasa.arc.l2tools.livingstone.code.componenttype.Attribute.class;	

	// from the gov.nasa.arc.l2tools.livingstone.code package
	if (tagName.equals(gov.nasa.arc.l2tools.livingstone.code.AbstractRelation.tagName))
	  return gov.nasa.arc.l2tools.livingstone.code.AbstractRelation.class;
	if (tagName.equals(gov.nasa.arc.l2tools.livingstone.code.CodeObject.tagName))
		return gov.nasa.arc.l2tools.livingstone.code.CodeObject.class;
	if (tagName.equals(gov.nasa.arc.l2tools.livingstone.code.CodeParticle.tagName))
		return gov.nasa.arc.l2tools.livingstone.code.CodeParticle.class;
	if (tagName.equals(gov.nasa.arc.l2tools.livingstone.code.Identifier.tagName))
		return gov.nasa.arc.l2tools.livingstone.code.Identifier.class;
	if (tagName.equals(gov.nasa.arc.l2tools.livingstone.code.Modifier.tagName))
		return gov.nasa.arc.l2tools.livingstone.code.Modifier.class;
	if (tagName.equals(gov.nasa.arc.l2tools.livingstone.code.Particle.tagName))
		return gov.nasa.arc.l2tools.livingstone.code.Particle.class;
	if (tagName.equals(gov.nasa.arc.l2tools.livingstone.code.Relation.tagName))
		return gov.nasa.arc.l2tools.livingstone.code.Relation.class;
			
	// from the gov.nasa.arc.l2tools.livingstone.componentinstance package
	if (tagName.equals(gov.nasa.arc.l2tools.livingstone.componentinstance.Attribute.tagName))
	  return gov.nasa.arc.l2tools.livingstone.componentinstance.Attribute.class;
	if (tagName.equals(gov.nasa.arc.l2tools.livingstone.componentinstance.AttributeType.tagName))
	  return gov.nasa.arc.l2tools.livingstone.componentinstance.AttributeType.class;
	if (tagName.equals(gov.nasa.arc.l2tools.livingstone.componentinstance.Component.tagName))
	  return gov.nasa.arc.l2tools.livingstone.componentinstance.Component.class;
	if (tagName.equals(gov.nasa.arc.l2tools.livingstone.componentinstance.Transition.tagName))
	  return gov.nasa.arc.l2tools.livingstone.componentinstance.Transition.class;
	if (tagName.equals(gov.nasa.arc.l2tools.livingstone.componentinstance.Clause.tagName))
	  return gov.nasa.arc.l2tools.livingstone.componentinstance.Clause.class;
	if (tagName.equals(gov.nasa.arc.l2tools.livingstone.componentinstance.EqualityTerm.tagName))
	  return gov.nasa.arc.l2tools.livingstone.componentinstance.EqualityTerm.class;
	if (tagName.equals(gov.nasa.arc.l2tools.livingstone.componentinstance.StateVector.tagName))
	  return gov.nasa.arc.l2tools.livingstone.componentinstance.StateVector.class;
	if (tagName.equals(gov.nasa.arc.l2tools.livingstone.componentinstance.Particle.tagName))
	  return gov.nasa.arc.l2tools.livingstone.componentinstance.Particle.class;
	
	// from the gov.nasa.arc.l2tools.livingstone package
	if (tagName.equals(gov.nasa.arc.l2tools.livingstone.AttributeType.tagName))
	  return gov.nasa.arc.l2tools.livingstone.AttributeType.class;
	if (tagName.equals(gov.nasa.arc.l2tools.livingstone.Commands.tagName))
		return gov.nasa.arc.l2tools.livingstone.Commands.class;
	if (tagName.equals(gov.nasa.arc.l2tools.livingstone.ComponentInstance.tagName))
	  return gov.nasa.arc.l2tools.livingstone.ComponentInstance.class;
	if (tagName.equals(gov.nasa.arc.l2tools.livingstone.ComponentType.tagName))
		return gov.nasa.arc.l2tools.livingstone.ComponentType.class;
	if (tagName.equals(gov.nasa.arc.l2tools.livingstone.DataObject.tagName))
		return gov.nasa.arc.l2tools.livingstone.DataObject.class;
	if (tagName.equals(gov.nasa.arc.l2tools.livingstone.Scenario.tagName))
		return gov.nasa.arc.l2tools.livingstone.Scenario.class;
	if (tagName.equals(gov.nasa.arc.l2tools.livingstone.SensorHistory.tagName))
		return gov.nasa.arc.l2tools.livingstone.SensorHistory.class;
	if (tagName.equals(gov.nasa.arc.l2tools.livingstone.StateHistory.tagName))
		return gov.nasa.arc.l2tools.livingstone.StateHistory.class;

	// from the gov.nasa.arc.l2tools.package
	if (tagName.equals(gov.nasa.arc.l2tools.DataObject.tagName))
		return gov.nasa.arc.l2tools.DataObject.class;
	if (tagName.equals(gov.nasa.arc.l2tools.Project.tagName))
		return gov.nasa.arc.l2tools.Project.class;
	if (tagName.equals(gov.nasa.arc.l2tools.Workspace.tagName))
		return gov.nasa.arc.l2tools.Workspace.class;

	// give up
	return null;
}
/**
 * create method comment.
 */
public fr.dyade.koala.xml.domlight.XMLElement create(String tagName) {
	Class skunkworksClass;
	gov.nasa.arc.l2tools.DataObject skunkworksObject;
	try {
		skunkworksClass = classFor(tagName);
		if (skunkworksClass!=null) {
			try {
				skunkworksObject = (gov.nasa.arc.l2tools.DataObject)skunkworksClass.newInstance();
				return skunkworksObject.getXmlElement();
			}
			catch (IllegalAccessException iae) {
				System.err.println("in gov.nasa.arc.l2tools.io.xmpl.XmlElementFactorImp.create():");
				System.err.println("IllegalAccessException on "+skunkworksClass.getName()+".newInstance().");
			} catch (InstantiationException ie) {
				System.err.println("in gov.nasa.arc.l2tools.io.xmpl.XmlElementFactorImp.create():");
				System.err.println("InstantiationException on "+skunkworksClass.getName()+".newInstance().");
			}
		}
	} catch (ClassNotFoundException cnfe) {
		System.err.println("in gov.nasa.arc.l2tools.io.xmpl.XmlElementFactorImp.create():");
		System.err.println("class not found by classFor() for tag: "+tagName);
	}
	return new gov.nasa.arc.l2tools.DataObject(tagName).getXmlElement();
}
}

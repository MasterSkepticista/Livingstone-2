//
// * See the file "l2-tools/disclaimers-and-notices.txt" for 
// * information on usage and redistribution of this file, 
// * and for a DISCLAIMER OF ALL WARRANTIES.
//
package gov.nasa.arc.l2tools.livingstone.code.logic;

/**
 * Creation date: (4/4/2000 4:11:40 PM)
 * @author: Andrew Bachmann
 */
public class AndExpression extends NaryExpression {
/**
 * AndExpression constructor comment.
 */
public AndExpression() {
	super();
}
/**
 * Creation date: (4/8/2000 11:59:21 PM)
 * @author: Andrew Bachmann
 * 
 * @return gov.nasa.arc.l2tools.livingstone.componentinstance.Clause
 * @param clause gov.nasa.arc.l2tools.livingstone.componentinstance.Clause
 * @exception gov.nasa.arc.l2tools.Exception The exception description.
 */
public gov.nasa.arc.l2tools.livingstone.componentinstance.Clause buildInstance(gov.nasa.arc.l2tools.livingstone.componentinstance.Clause clause) 
throws gov.nasa.arc.l2tools.Exception {
	String errorString="instantiation error: AndExpression inside of OrExpression in buildInstance";
	throw new InstantiationError(errorString);
}
/**
 * Creation date: (4/8/2000 9:48:33 PM)
 * @author: Andrew Bachmann
 * 
 * @return gov.nasa.arc.l2tools.livingstone.componentinstance.Clauses
 * @param component gov.nasa.arc.l2tools.livingstone.componentinstance.AbstractClauses
 * @exception gov.nasa.arc.l2tools.Exception The exception description.
 */
public gov.nasa.arc.l2tools.livingstone.componentinstance.Clauses buildInstance(gov.nasa.arc.l2tools.livingstone.componentinstance.Clauses clauses) 
throws gov.nasa.arc.l2tools.Exception {
	java.util.Enumeration expressions=getSubExpressions().elements();
	while (expressions.hasMoreElements()) {
		BasicExpression expression=(BasicExpression)expressions.nextElement();
		expression.buildInstance(clauses);
	}
	return clauses;
}
/**
 * copy method comment.
 */
public LogicExpression copy() {
	AndExpression newAnd = new AndExpression();
	java.util.Enumeration expressions=getSubExpressions().elements();
	while (expressions.hasMoreElements()) {
		newAnd.getSubExpressions().addElement(((LogicExpression)expressions.nextElement()).copy());
	}
	return newAnd;	
}
/**
 * destructiveConjunction method comment.
 */
public LogicExpression destructiveConjunction(LogicExpression expression) {
	getSubExpressions().addElement(expression);
	return this;
}
/**
 * OR((AND X Y),(AND A B))=>(AND (OR X A) (OR X B) (OR Y A) (OR Y B))
 * Creation date: (4/6/2000 2:43:09 PM)
 * @author: Andrew Bachmann
 */
protected BasicExpression distribute(AndExpression expr) {
	BasicExpression and=new AndExpression();
	for(int left=0;(left<this.getSubExpressions().size());left++) {
		for(int right=0;(right<expr.getSubExpressions().size());right++) {
			BasicExpression or=new OrExpression();
			or=((OrExpression)or).merge((BasicExpression)this.getSubExpressions().elementAt(left));
			if (or instanceof OrExpression) {
				or=((OrExpression)or).merge((BasicExpression)expr.getSubExpressions().elementAt(right));
				if (or instanceof OrExpression) {
					and=((AndExpression)and).merge(or);
					if (and instanceof BooleanExpression) {
						return and;
					}
				} else { 
					return or;
				}
			} else {
				return or;
			}
		}
	}
	return and;
}
/**
 * OR((AND X (NOT Y) (OR Z Q)),(OR A (NOT B) (AND C D)))=>OR((OR A (NOT B) (AND C D)),(AND X (NOT Y) (OR Z Q)))
 * Creation date: (4/6/2000 2:43:09 PM)
 * @author: Andrew Bachmann
 */
protected BasicExpression distribute(OrExpression expression) {
	return expression.distribute(this);
}
/**
 * Creation date: (4/8/2000 10:40:55 PM)
 * @author: Andrew Bachmann
 * 
 * @return gov.nasa.arc.l2tools.livingstone.logic.BasicExpression
 * @param gov.nasa.arc.l2tools.livingstone.componentinstance.Component component
 * @exception gov.nasa.arc.l2tools.Exception The exception description.
 */
protected BasicExpression expandAndInstantiate(gov.nasa.arc.l2tools.livingstone.componentinstance.Component component) 
throws gov.nasa.arc.l2tools.Exception {
	BasicExpression and=new AndExpression();
	java.util.Enumeration expressions=getSubExpressions().elements();
	while (expressions.hasMoreElements()) {
		LogicExpression expression=(LogicExpression)expressions.nextElement();
		BasicExpression result=expression.expandAndInstantiate(component);
		if (result instanceof FalseExpression) return result;
		if (!(result instanceof TrueExpression)) {
			and=((AndExpression)and).merge(result);
			if (and instanceof FalseExpression) return and;
			if (and instanceof TrueExpression) and=new AndExpression();
		}
	}
	if (((AndExpression)and).getSubExpressions().isEmpty()) {
		return new TrueExpression();
	}
	return and;
}
/**
 * Creation date: (4/6/2000 2:43:09 PM)
 * @author: Andrew Bachmann
 * 
 * @return gov.nasa.arc.l2tools.livingstone.logic.BasicExpression
 */
protected BasicExpression flatten() {
	AndExpression and=new AndExpression();
	flatten(and);
	return and;
}
/**
 * Creation date: (4/6/2000 2:43:09 PM)
 * @author: Andrew Bachmann
 */
protected void flatten(AndExpression result) {
	java.util.Enumeration subExpressions=getSubExpressions().elements();
	while (subExpressions.hasMoreElements()) {
		((BasicExpression)subExpressions.nextElement()).flatten(result);
	}
}
/**
 * This is not a complete de morgan, because de morgan's law on an expression
 * will not only introduce negations in terms and change the operator or/and
 * but will also introduce a negation to encapsulate the whole.  That's why
 * it is "notDeMorgan" instead of "deMorgan"
 */
protected NaryExpression notDeMorgan() {
	OrExpression or = new OrExpression();
	java.util.Enumeration subexpressions=getSubExpressions().elements();
	while (subexpressions.hasMoreElements()) {
		LogicExpression expression=(LogicExpression)subexpressions.nextElement();
		or.getSubExpressions().addElement(expression.negation());
	}
	return or;
}
/**
 * pushDisjunctionsIn method comment.
 */
protected BasicExpression pushDisjunctionsIn() {
	for (int i=0;(i<getSubExpressions().size());i++) {
		BasicExpression expression=(BasicExpression)getSubExpressions().elementAt(i);
		BasicExpression result=expression.pushDisjunctionsIn();
		if (result instanceof FalseExpression) return (FalseExpression)result;
		getSubExpressions().setElementAt(result,i);
	}
	return this;
}
/**
 * substitute method comment.
 */
public LogicExpression substitute(gov.nasa.arc.l2tools.livingstone.code.Relation relation, javax.swing.DefaultListModel values, String relationOwner)
throws gov.nasa.arc.l2tools.Exception {
	AndExpression and=new AndExpression();
	java.util.Enumeration expressions=getSubExpressions().elements();
	while (expressions.hasMoreElements()) {
		and.getSubExpressions().addElement(((LogicExpression)expressions.nextElement()).substitute(relation,values,relationOwner));
	}
	return and;
}
/**
 * Creation date: (4/5/2000 4:02:35 PM)
 * @author: Andrew Bachmann
 * 
 */
public String toString() {
	String result=null;
	java.util.Enumeration subexpressions=getSubExpressions().elements();
	while (subexpressions.hasMoreElements()) {
		if (result==null) {
			result="("+subexpressions.nextElement().toString();
		} else {
			result+="^"+subexpressions.nextElement().toString();
		}
	}
	if (result==null) {
		result="(AND)";
	} else {
		result+=")";
	}	
	return result;
}
}

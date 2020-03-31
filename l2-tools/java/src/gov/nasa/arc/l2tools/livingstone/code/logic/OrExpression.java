//
// * See the file "l2-tools/disclaimers-and-notices.txt" for 
// * information on usage and redistribution of this file, 
// * and for a DISCLAIMER OF ALL WARRANTIES.
//
package gov.nasa.arc.l2tools.livingstone.code.logic;

/**
 * Creation date: (4/4/2000 4:11:48 PM)
 * @author: Andrew Bachmann
 */
public class OrExpression extends NaryExpression {
/**
 * OrExpression constructor comment.
 */
public OrExpression() {
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
	java.util.Enumeration expressions=getSubExpressions().elements();
	while (expressions.hasMoreElements()) {
		BasicExpression expression=(BasicExpression)expressions.nextElement();
		expression.buildInstance(clause);
	}
	return clause;
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
	gov.nasa.arc.l2tools.livingstone.componentinstance.Clause clause
	  = clauses.makeClause();
	java.util.Enumeration expressions=getSubExpressions().elements();
	while (expressions.hasMoreElements()) {
		BasicExpression expression=(BasicExpression)expressions.nextElement();
		expression.buildInstance(clause);
	}
	return clauses;
}
/**
 * copy method comment.
 */
public LogicExpression copy() {
	OrExpression newOr = new OrExpression();
	java.util.Enumeration expressions=getSubExpressions().elements();
	while (expressions.hasMoreElements()) {
		newOr.getSubExpressions().addElement(((LogicExpression)expressions.nextElement()).copy());
	}
	return newOr;	
}
/**
 * conjunction method comment.
 */
public LogicExpression destructiveDisjunction(LogicExpression expression) {
	getSubExpressions().addElement(expression);
	return this;
}
/**
 * OR((OR A (NOT B) (AND C D)),(AND X (NOT Y) (OR Z Q)))=>
 * (AND (OR A (NOT B) C X) (OR A (NOT B) C (NOT Y)) (OR A (NOT B) C Z Q)
 *      (OR A (NOT B) D X) (OR A (NOT B) D (NOT Y)) (OR A (NOT B) D Z Q))
 * Creation date: (4/6/2000 2:43:09 PM)
 * @author: Andrew Bachmann
 */
protected BasicExpression distribute(AndExpression expression) {
	BasicExpression and=new AndExpression();
	java.util.Enumeration subExpressions=expression.getSubExpressions().elements();
	while (subExpressions.hasMoreElements()) {
		BasicExpression subExpression=(BasicExpression)subExpressions.nextElement();
		and=((AndExpression)and).merge(subExpression.distribute(this));
		if (and instanceof BooleanExpression)
			return and;
	}
	return and;
}
/**
 * OR((OR X Y),(OR Z W))=>(OR X Y Z W)
 * Creation date: (4/6/2000 2:43:09 PM)
 * @author: Andrew Bachmann
 */
protected BasicExpression distribute(OrExpression expression) {
	BasicExpression or=new OrExpression();
	java.util.Enumeration subExpressions=expression.getSubExpressions().elements();
	while (subExpressions.hasMoreElements()) {
		BasicExpression subExpression=(BasicExpression)subExpressions.nextElement();
		((OrExpression)or).getSubExpressions().addElement(subExpression);
	}
	java.util.Enumeration mySubExpressions=getSubExpressions().elements();
	while (mySubExpressions.hasMoreElements()) {
		BasicExpression mySubExpression=(BasicExpression)mySubExpressions.nextElement();
		or=((OrExpression)or).merge(mySubExpression);
		if (or instanceof BooleanExpression) {
			return or;
		}
	}
	return or;
}
/**
 * Creation date: (4/8/2000 10:40:55 PM)
 * @author: Andrew Bachmann
 * 
 * @return gov.nasa.arc.l2tools.livingstone.logic.BasicExpression
 * @param component gov.nasa.arc.l2tools.livingstone.componentinstance.Component
 * @exception gov.nasa.arc.l2tools.Exception The exception description.
 */
protected BasicExpression expandAndInstantiate(gov.nasa.arc.l2tools.livingstone.componentinstance.Component component) 
throws gov.nasa.arc.l2tools.Exception {
	BasicExpression or=new OrExpression();
	java.util.Enumeration expressions=getSubExpressions().elements();
	while (expressions.hasMoreElements()) {
		LogicExpression expression=(LogicExpression)expressions.nextElement();
		BasicExpression result=expression.expandAndInstantiate(component);
		if (result instanceof TrueExpression) return result;
		if (!(result instanceof FalseExpression)) {
			or=((OrExpression)or).merge(result);
			if (or instanceof TrueExpression) return or;
			if (or instanceof FalseExpression) or=new OrExpression();
		}
	}
	if (((OrExpression)or).getSubExpressions().isEmpty()) {
		return new FalseExpression();
	}
	return or;
}
/**
 * Creation date: (4/6/2000 2:43:09 PM)
 * @author: Andrew Bachmann
 * 
 * @return gov.nasa.arc.l2tools.livingstone.logic.BasicExpression
 */
protected BasicExpression flatten() {
	OrExpression or=new OrExpression();
	flatten(or);
	return or;
}
/**
 * Creation date: (4/6/2000 2:43:09 PM)
 * @author: Andrew Bachmann
 */
protected void flatten(OrExpression result) {
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
	AndExpression and = new AndExpression();
	java.util.Enumeration subexpressions=getSubExpressions().elements();
	while (subexpressions.hasMoreElements()) {
		LogicExpression expression=(LogicExpression)subexpressions.nextElement();
		and.getSubExpressions().addElement(expression.negation());
	}
	return and;
}
/**
 * (OR (AND X Y Z) (AND W R) (NOT R) P)=>
 * (AND (OR X W (NOT R) P) (OR X R (NOT R) P) 
 *      (OR Y W (NOT R) P) (OR Y R (NOT R) P) 
 *      (OR Z W (NOT R) P) (OR Z R (NOT R) P))
 */
protected BasicExpression pushDisjunctionsIn() {
	OrExpression flat=(OrExpression)flatten();
	boolean expressionOkay=true;
	int[] indices=new int[flat.getSubExpressions().size()];
	for (int initializer=0; (initializer<indices.length); initializer++) {
		if (flat.getSubExpressions().elementAt(initializer) instanceof NaryExpression)
		  expressionOkay=false;
		indices[initializer]=0;
	}
	AndExpression and=new AndExpression();
	if (expressionOkay) {
		and.getSubExpressions().addElement(flat);
		return and;
	}
	boolean done=false;
	while (!done) {
		int index;
		OrExpression or=new OrExpression();
		for (index=0; (index<indices.length); index++) {
			BasicExpression expression=(BasicExpression)flat.getSubExpressions().elementAt(index);
			if (expression instanceof UnaryExpression) {
				or.getSubExpressions().addElement(expression);
			} else if (expression instanceof BooleanExpression) {
				or.getSubExpressions().addElement(expression);
			} else {
				or.getSubExpressions().addElement(((NaryExpression)expression).getSubExpressions().elementAt(indices[index]));
			}
		}
		and.getSubExpressions().addElement(or);
		// move up the indices to grab the next set of terms
		done=true;
		for (index=0; (index<indices.length); index++) {
			BasicExpression expression=(BasicExpression)flat.getSubExpressions().elementAt(index);
			if (expression instanceof NaryExpression) {
				indices[index]++;
				if (indices[index]==((NaryExpression)expression).getSubExpressions().size()) {
				// we have moved through this expression once, we need to move through the next
					indices[index]=0;
				} else {
				// we are still moving through this expression, so let's go do it
					done=false;
					break;
				}
			}
		}
	}		
	return and.pushDisjunctionsIn();
}
/**
 * substitute method comment.
 */
public LogicExpression substitute(gov.nasa.arc.l2tools.livingstone.code.Relation relation, javax.swing.DefaultListModel values, String relationOwner)
throws gov.nasa.arc.l2tools.Exception {
	OrExpression or=new OrExpression();
	java.util.Enumeration expressions=getSubExpressions().elements();
	while (expressions.hasMoreElements()) {
		or.getSubExpressions().addElement(((LogicExpression)expressions.nextElement()).substitute(relation,values,relationOwner));
	}
	return or;
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
			result+="v"+subexpressions.nextElement().toString();
		}
	}
	if (result==null) {
		result="(OR)";
	} else {
		result+=")";
	}
	return result;
}
}

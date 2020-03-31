package gov.nasa.arc.l2tools.io.jmpl;

// $ANTLR 2.7.0: "antlr/jmpl.tree.g" -> "JmplTreeParser.java"$
import antlr.TreeParser;
import antlr.Token;
import antlr.collections.AST;
import antlr.RecognitionException;
import antlr.ANTLRException;
import antlr.NoViableAltException;
import antlr.MismatchedTokenException;
import antlr.SemanticException;
import antlr.collections.impl.BitSet;
import antlr.ASTPair;
import antlr.collections.impl.ASTArray;

public class JmplTreeParser extends antlr.TreeParser
       implements JmplTreeParserTokenTypes
 {

	protected void updateSourceInfo(gov.nasa.arc.l2tools.DataObject object, gov.nasa.arc.l2tools.io.jmpl.JmplASTNode node) {
		if ((node!=null) && (object!=null)) {
			if (node.getLine()>=0) {
				object.set("line",Integer.toString(node.getLine()));
			}
			if (node.getColumn()>=0) {
			  object.set("column",Integer.toString(node.getColumn()));
			}
			if ((node.getFilename()!=null) && (node.getFilename().length()>=0)) {
			  object.set("filename",node.getFilename());
			}
		}
  }            
public JmplTreeParser() {
	tokenNames = _tokenNames;
}

	public final void jmpl(AST _t,
		gov.nasa.arc.l2tools.DataObject environment
	) throws RecognitionException {
		
		AST jmpl_AST_in = (AST)_t;
		
		try {      // for error handling
			gov.nasa.arc.l2tools.livingstone.DataObject o;
			{
			_loop3:
			do {
				if (_t==null) _t=ASTNULL;
				switch ( _t.getType()) {
				case ATTRIBUTE_TYPE:
				{
					o=attributeType(_t,environment);
					_t = _retTree;
					break;
				}
				case COMPONENT_TYPE:
				{
					o=componentType(_t,environment);
					_t = _retTree;
					break;
				}
				default:
				{
					break _loop3;
				}
				}
			} while (true);
			}
		}
		catch (RecognitionException ex) {
			reportError(ex);
			if (_t!=null) {_t = _t.getNextSibling();}
		}
		_retTree = _t;
	}
	
	public final gov.nasa.arc.l2tools.livingstone.AttributeType 
          attributeType(AST _t,
		gov.nasa.arc.l2tools.DataObject parent
	) throws RecognitionException {
		gov.nasa.arc.l2tools.livingstone.AttributeType 
         attributeType=new gov.nasa.arc.l2tools.livingstone.AttributeType();
		
		AST attributeType_AST_in = (AST)_t;
		AST name = null;
		
		try {      // for error handling
			AST __t5 = _t;
			AST tmp1_AST_in = (AST)_t;
			match(_t,ATTRIBUTE_TYPE);
			_t = _t.getFirstChild();
			if (parent!=null) parent.getChildren().addElement(attributeType);
			updateSourceInfo(attributeType,(gov.nasa.arc.l2tools.io.jmpl.JmplASTNode)_t);
			name = (AST)_t;
			match(_t,IDENT);
			_t = _t.getNextSibling();
			attributeType.set("name",name.getText());
			gov.nasa.arc.l2tools.livingstone.code.Particle atv;
			{
			_loop7:
			do {
				if (_t==null) _t=ASTNULL;
				if ((_t.getType()==ATTRIBUTE_TYPE_VALUE)) {
					atv=attributeTypeValue(_t,attributeType);
					_t = _retTree;
				}
				else {
					break _loop7;
				}
				
			} while (true);
			}
			gov.nasa.arc.l2tools.livingstone.code.Particle ate;
			{
			_loop9:
			do {
				if (_t==null) _t=ASTNULL;
				if ((_tokenSet_0.member(_t.getType()))) {
					ate=attributeTypeEntry(_t,attributeType);
					_t = _retTree;
				}
				else {
					break _loop9;
				}
				
			} while (true);
			}
			_t = __t5;
			_t = _t.getNextSibling();
		}
		catch (RecognitionException ex) {
			reportError(ex);
			if (_t!=null) {_t = _t.getNextSibling();}
		}
		_retTree = _t;
		return attributeType;
	}
	
	public final gov.nasa.arc.l2tools.livingstone.ComponentType
          componentType(AST _t,
		gov.nasa.arc.l2tools.DataObject parent
	) throws RecognitionException {
		gov.nasa.arc.l2tools.livingstone.ComponentType
         componentType=new gov.nasa.arc.l2tools.livingstone.ComponentType();
		
		AST componentType_AST_in = (AST)_t;
		AST name = null;
		
		try {      // for error handling
			AST __t15 = _t;
			AST tmp2_AST_in = (AST)_t;
			match(_t,COMPONENT_TYPE);
			_t = _t.getFirstChild();
			if (parent!=null) parent.getChildren().addElement(componentType);
			updateSourceInfo(componentType,(gov.nasa.arc.l2tools.io.jmpl.JmplASTNode)_t);
			name = (AST)_t;
			match(_t,IDENT);
			_t = _t.getNextSibling();
			componentType.set("name",name.getText());
			gov.nasa.arc.l2tools.livingstone.code.Identifier ec;
			{
			if (_t==null) _t=ASTNULL;
			switch ( _t.getType()) {
			case EXTENSION_CLAUSE:
			{
				ec=extensionClause(_t,componentType);
				_t = _retTree;
				componentType.set("extends",ec.get("name"));
				break;
			}
			case 3:
			case CT_ATTRIBUTE_TYPE:
			case RELATION:
			case NOMINAL_TRANSITION:
			case FAILURE_TRANSITION:
			case STATE_VECTOR:
			case VARIABLE:
			case STATEMENT_LIST:
			{
				break;
			}
			default:
			{
				throw new NoViableAltException(_t);
			}
			}
			}
			gov.nasa.arc.l2tools.livingstone.code.Particle cte;
			{
			_loop18:
			do {
				if (_t==null) _t=ASTNULL;
				if ((_tokenSet_1.member(_t.getType()))) {
					cte=componentTypeEntry(_t,componentType);
					_t = _retTree;
				}
				else {
					break _loop18;
				}
				
			} while (true);
			}
			_t = __t15;
			_t = _t.getNextSibling();
		}
		catch (RecognitionException ex) {
			reportError(ex);
			if (_t!=null) {_t = _t.getNextSibling();}
		}
		_retTree = _t;
		return componentType;
	}
	
	public final gov.nasa.arc.l2tools.livingstone.code.attributetype.Value
          attributeTypeValue(AST _t,
		gov.nasa.arc.l2tools.livingstone.DataObject parent
	) throws RecognitionException {
		gov.nasa.arc.l2tools.livingstone.code.attributetype.Value
         attributeTypeValue=new gov.nasa.arc.l2tools.livingstone.code.attributetype.Value();
		
		AST attributeTypeValue_AST_in = (AST)_t;
		AST valueName = null;
		AST valueString = null;
		
		try {      // for error handling
			AST __t11 = _t;
			AST tmp3_AST_in = (AST)_t;
			match(_t,ATTRIBUTE_TYPE_VALUE);
			_t = _t.getFirstChild();
			if (parent!=null) parent.getChildren().addElement(attributeTypeValue);
			updateSourceInfo(attributeTypeValue,(gov.nasa.arc.l2tools.io.jmpl.JmplASTNode)_t);
			valueName = (AST)_t;
			match(_t,IDENT);
			_t = _t.getNextSibling();
			attributeTypeValue.set("name",valueName.getText());
			{
			if (_t==null) _t=ASTNULL;
			switch ( _t.getType()) {
			case STRING_LITERAL:
			{
				valueString = (AST)_t;
				match(_t,STRING_LITERAL);
				_t = _t.getNextSibling();
				attributeTypeValue.set("value",valueString.getText());
				break;
			}
			case 3:
			{
				break;
			}
			default:
			{
				throw new NoViableAltException(_t);
			}
			}
			}
			_t = __t11;
			_t = _t.getNextSibling();
		}
		catch (RecognitionException ex) {
			reportError(ex);
			if (_t!=null) {_t = _t.getNextSibling();}
		}
		_retTree = _t;
		return attributeTypeValue;
	}
	
	public final gov.nasa.arc.l2tools.livingstone.code.Particle  attributeTypeEntry(AST _t,
		gov.nasa.arc.l2tools.livingstone.DataObject parent
	) throws RecognitionException {
		gov.nasa.arc.l2tools.livingstone.code.Particle attributeTypeEntry=null;
		
		AST attributeTypeEntry_AST_in = (AST)_t;
		
		try {      // for error handling
			if (_t==null) _t=ASTNULL;
			switch ( _t.getType()) {
			case RELATION:
			{
				attributeTypeEntry=relation(_t,parent);
				_t = _retTree;
				break;
			}
			case EXPRESSION:
			case EMPTY_STATEMENT:
			case IF_STATEMENT:
			case IFF_STATEMENT:
			case SWITCH_STATEMENT:
			case STATEMENT_LIST:
			{
				attributeTypeEntry=statement(_t,parent);
				_t = _retTree;
				break;
			}
			default:
			{
				throw new NoViableAltException(_t);
			}
			}
		}
		catch (RecognitionException ex) {
			reportError(ex);
			if (_t!=null) {_t = _t.getNextSibling();}
		}
		_retTree = _t;
		return attributeTypeEntry;
	}
	
	public final gov.nasa.arc.l2tools.livingstone.code.Relation  relation(AST _t,
		gov.nasa.arc.l2tools.livingstone.DataObject parent
	) throws RecognitionException {
		gov.nasa.arc.l2tools.livingstone.code.Relation relation
         =new gov.nasa.arc.l2tools.livingstone.code.Relation();
		
		AST relation_AST_in = (AST)_t;
		AST name = null;
		
		try {      // for error handling
			AST __t60 = _t;
			AST tmp4_AST_in = (AST)_t;
			match(_t,RELATION);
			_t = _t.getFirstChild();
			if (parent!=null) parent.getChildren().addElement(relation);
			updateSourceInfo(relation,(gov.nasa.arc.l2tools.io.jmpl.JmplASTNode)_t);
			name = (AST)_t;
			match(_t,IDENT);
			_t = _t.getNextSibling();
			relation.set("name",name.getText());
			gov.nasa.arc.l2tools.livingstone.code.Particle argument;
			{
			_loop62:
			do {
				if (_t==null) _t=ASTNULL;
				if ((_t.getType()==VARIABLE)) {
					argument=attribute(_t,relation);
					_t = _retTree;
				}
				else {
					break _loop62;
				}
				
			} while (true);
			}
			gov.nasa.arc.l2tools.livingstone.code.Particle list;
			list=statementList(_t,relation);
			_t = _retTree;
			_t = __t60;
			_t = _t.getNextSibling();
		}
		catch (RecognitionException ex) {
			reportError(ex);
			if (_t!=null) {_t = _t.getNextSibling();}
		}
		_retTree = _t;
		return relation;
	}
	
	public final gov.nasa.arc.l2tools.livingstone.code.constraint.Statement  statement(AST _t,
		gov.nasa.arc.l2tools.livingstone.DataObject parent
	) throws RecognitionException {
		gov.nasa.arc.l2tools.livingstone.code.constraint.Statement st=null;
		
		AST statement_AST_in = (AST)_t;
		
		try {      // for error handling
			if (_t==null) _t=ASTNULL;
			switch ( _t.getType()) {
			case IF_STATEMENT:
			{
				AST __t99 = _t;
				AST tmp5_AST_in = (AST)_t;
				match(_t,IF_STATEMENT);
				_t = _t.getFirstChild();
				st=new gov.nasa.arc.l2tools.livingstone.code.constraint.IfStatement();
				if (parent!=null) parent.getChildren().addElement(st);
				updateSourceInfo(st,(gov.nasa.arc.l2tools.io.jmpl.JmplASTNode)_t);
				gov.nasa.arc.l2tools.livingstone.code.constraint.Expression testExpression;
				testExpression=expression(_t,st);
				_t = _retTree;
				gov.nasa.arc.l2tools.livingstone.code.constraint.Statement thenStatement;
				thenStatement=statement(_t,st);
				_t = _retTree;
				gov.nasa.arc.l2tools.livingstone.code.constraint.Statement elseStatement;
				{
				if (_t==null) _t=ASTNULL;
				switch ( _t.getType()) {
				case EXPRESSION:
				case EMPTY_STATEMENT:
				case IF_STATEMENT:
				case IFF_STATEMENT:
				case SWITCH_STATEMENT:
				case STATEMENT_LIST:
				{
					elseStatement=statement(_t,st);
					_t = _retTree;
					break;
				}
				case 3:
				{
					break;
				}
				default:
				{
					throw new NoViableAltException(_t);
				}
				}
				}
				_t = __t99;
				_t = _t.getNextSibling();
				break;
			}
			case IFF_STATEMENT:
			{
				AST __t101 = _t;
				AST tmp6_AST_in = (AST)_t;
				match(_t,IFF_STATEMENT);
				_t = _t.getFirstChild();
				st=new gov.nasa.arc.l2tools.livingstone.code.constraint.IffStatement();
				if (parent!=null) parent.getChildren().addElement(st);
				updateSourceInfo(st,(gov.nasa.arc.l2tools.io.jmpl.JmplASTNode)_t);
				gov.nasa.arc.l2tools.livingstone.code.constraint.Expression testExpression;
				testExpression=expression(_t,st);
				_t = _retTree;
				gov.nasa.arc.l2tools.livingstone.code.constraint.Statement thenStatement;
				thenStatement=statement(_t,st);
				_t = _retTree;
				_t = __t101;
				_t = _t.getNextSibling();
				break;
			}
			case SWITCH_STATEMENT:
			{
				AST __t102 = _t;
				AST tmp7_AST_in = (AST)_t;
				match(_t,SWITCH_STATEMENT);
				_t = _t.getFirstChild();
				st=new gov.nasa.arc.l2tools.livingstone.code.constraint.SwitchStatement();
				if (parent!=null) parent.getChildren().addElement(st);
				updateSourceInfo(st,(gov.nasa.arc.l2tools.io.jmpl.JmplASTNode)_t);
				gov.nasa.arc.l2tools.livingstone.code.Identifier id;
				id=contextIdentifier(_t,null);
				_t = _retTree;
				st.set("key",id.get("name"));
				gov.nasa.arc.l2tools.livingstone.code.constraint.CaseGroup cg;
				{
				_loop104:
				do {
					if (_t==null) _t=ASTNULL;
					if ((_t.getType()==CASE_GROUP)) {
						cg=casesGroup(_t,st);
						_t = _retTree;
					}
					else {
						break _loop104;
					}
					
				} while (true);
				}
				_t = __t102;
				_t = _t.getNextSibling();
				break;
			}
			case EMPTY_STATEMENT:
			{
				AST __t105 = _t;
				AST tmp8_AST_in = (AST)_t;
				match(_t,EMPTY_STATEMENT);
				_t = _t.getFirstChild();
				st=new gov.nasa.arc.l2tools.livingstone.code.constraint.StatementGroup();
				if (parent!=null) parent.getChildren().addElement(st);
				updateSourceInfo(st,(gov.nasa.arc.l2tools.io.jmpl.JmplASTNode)_t);
				_t = __t105;
				_t = _t.getNextSibling();
				break;
			}
			case STATEMENT_LIST:
			{
				st=statementList(_t,parent);
				_t = _retTree;
				break;
			}
			case EXPRESSION:
			{
				st=expression(_t,parent);
				_t = _retTree;
				break;
			}
			default:
			{
				throw new NoViableAltException(_t);
			}
			}
		}
		catch (RecognitionException ex) {
			reportError(ex);
			if (_t!=null) {_t = _t.getNextSibling();}
		}
		_retTree = _t;
		return st;
	}
	
	public final gov.nasa.arc.l2tools.livingstone.code.Identifier  extensionClause(AST _t,
		gov.nasa.arc.l2tools.livingstone.ComponentType parent
	) throws RecognitionException {
		gov.nasa.arc.l2tools.livingstone.code.Identifier componentTypeName=null;
		
		AST extensionClause_AST_in = (AST)_t;
		
		try {      // for error handling
			AST __t20 = _t;
			AST tmp9_AST_in = (AST)_t;
			match(_t,EXTENSION_CLAUSE);
			_t = _t.getFirstChild();
			componentTypeName=identifier(_t,null);
			_t = _retTree;
			updateSourceInfo(componentTypeName,(gov.nasa.arc.l2tools.io.jmpl.JmplASTNode)_t);
			_t = __t20;
			_t = _t.getNextSibling();
		}
		catch (RecognitionException ex) {
			reportError(ex);
			if (_t!=null) {_t = _t.getNextSibling();}
		}
		_retTree = _t;
		return componentTypeName;
	}
	
	public final gov.nasa.arc.l2tools.livingstone.code.Particle  componentTypeEntry(AST _t,
		gov.nasa.arc.l2tools.livingstone.ComponentType parent
	) throws RecognitionException {
		gov.nasa.arc.l2tools.livingstone.code.Particle componentTypeEntry=null;
		
		AST componentTypeEntry_AST_in = (AST)_t;
		
		try {      // for error handling
			if (_t==null) _t=ASTNULL;
			switch ( _t.getType()) {
			case VARIABLE:
			{
				componentTypeEntry=attribute(_t,parent);
				_t = _retTree;
				break;
			}
			case STATE_VECTOR:
			{
				componentTypeEntry=stateVector(_t,parent);
				_t = _retTree;
				break;
			}
			case CT_ATTRIBUTE_TYPE:
			{
				componentTypeEntry=componentTypeAttributeType(_t,parent);
				_t = _retTree;
				break;
			}
			case NOMINAL_TRANSITION:
			{
				componentTypeEntry=transition(_t,parent);
				_t = _retTree;
				break;
			}
			case FAILURE_TRANSITION:
			{
				componentTypeEntry=failure(_t,parent);
				_t = _retTree;
				break;
			}
			case RELATION:
			{
				componentTypeEntry=relation(_t,parent);
				_t = _retTree;
				break;
			}
			case STATEMENT_LIST:
			{
				componentTypeEntry=statementList(_t,parent);
				_t = _retTree;
				break;
			}
			default:
			{
				throw new NoViableAltException(_t);
			}
			}
		}
		catch (RecognitionException ex) {
			reportError(ex);
			if (_t!=null) {_t = _t.getNextSibling();}
		}
		_retTree = _t;
		return componentTypeEntry;
	}
	
	public final gov.nasa.arc.l2tools.livingstone.code.Identifier
          identifier(AST _t,
		gov.nasa.arc.l2tools.livingstone.DataObject parent
	) throws RecognitionException {
		gov.nasa.arc.l2tools.livingstone.code.Identifier
         id=new gov.nasa.arc.l2tools.livingstone.code.Identifier();
		
		AST identifier_AST_in = (AST)_t;
		AST i = null;
		AST identPart = null;
		AST contextPart = null;
		
		try {      // for error handling
			if (_t==null) _t=ASTNULL;
			switch ( _t.getType()) {
			case IDENT:
			{
				AST __t48 = _t;
				i = _t==ASTNULL ? null :(AST)_t;
				match(_t,IDENT);
				_t = _t.getFirstChild();
				if (parent!=null) parent.getChildren().addElement(id);
				updateSourceInfo(id,(gov.nasa.arc.l2tools.io.jmpl.JmplASTNode)_t);
				id.set("name",i.getText());
				_t = __t48;
				_t = _t.getNextSibling();
				break;
			}
			case QUALIFIED_NAME:
			{
				AST __t49 = _t;
				AST tmp10_AST_in = (AST)_t;
				match(_t,QUALIFIED_NAME);
				_t = _t.getFirstChild();
				if (parent!=null) parent.getChildren().addElement(id);
				updateSourceInfo(id,(gov.nasa.arc.l2tools.io.jmpl.JmplASTNode)_t);
				String name=new String("");
				gov.nasa.arc.l2tools.livingstone.code.Identifier qualifiedId;
				{
				qualifiedId=identifier(_t,id);
				_t = _retTree;
				name=qualifiedId.get("name");
				}
				{
				if (_t==null) _t=ASTNULL;
				switch ( _t.getType()) {
				case IDENT:
				{
					identPart = (AST)_t;
					match(_t,IDENT);
					_t = _t.getNextSibling();
					name+="."+identPart.getText();
					break;
				}
				case CONTEXT_KEYWORD:
				{
					contextPart = (AST)_t;
					match(_t,CONTEXT_KEYWORD);
					_t = _t.getNextSibling();
					name+="."+contextPart.getText();
					break;
				}
				default:
				{
					throw new NoViableAltException(_t);
				}
				}
				}
				id.set("name",name);
				_t = __t49;
				_t = _t.getNextSibling();
				break;
			}
			default:
			{
				throw new NoViableAltException(_t);
			}
			}
		}
		catch (RecognitionException ex) {
			reportError(ex);
			if (_t!=null) {_t = _t.getNextSibling();}
		}
		_retTree = _t;
		return id;
	}
	
	public final gov.nasa.arc.l2tools.livingstone.code.componenttype.Attribute
          attribute(AST _t,
		gov.nasa.arc.l2tools.livingstone.DataObject parent
	) throws RecognitionException {
		gov.nasa.arc.l2tools.livingstone.code.componenttype.Attribute
         attribute=new gov.nasa.arc.l2tools.livingstone.code.componenttype.Attribute();
		
		AST attribute_AST_in = (AST)_t;
		AST name = null;
		
		try {      // for error handling
			AST __t41 = _t;
			AST tmp11_AST_in = (AST)_t;
			match(_t,VARIABLE);
			_t = _t.getFirstChild();
			if (parent!=null) parent.getChildren().addElement(attribute);
			updateSourceInfo(attribute,(gov.nasa.arc.l2tools.io.jmpl.JmplASTNode)_t);
			{
			name = (AST)_t;
			match(_t,IDENT);
			_t = _t.getNextSibling();
			}
			attribute.set("name",name.getText());
			gov.nasa.arc.l2tools.livingstone.code.Identifier type;
			type=identifier(_t,null);
			_t = _retTree;
			attribute.set("type",type.get("name"));
			gov.nasa.arc.l2tools.livingstone.code.Identifier value;
			{
			if (_t==null) _t=ASTNULL;
			switch ( _t.getType()) {
			case QUALIFIED_NAME:
			case IDENT:
			{
				value=identifier(_t,null);
				_t = _retTree;
				attribute.set("value",value.get("name"));
				break;
			}
			case 3:
			case MODIFIERS:
			{
				break;
			}
			default:
			{
				throw new NoViableAltException(_t);
			}
			}
			}
			gov.nasa.arc.l2tools.livingstone.code.Particle mod;
			{
			if (_t==null) _t=ASTNULL;
			switch ( _t.getType()) {
			case MODIFIERS:
			{
				mod=modifier(_t,attribute);
				_t = _retTree;
				attribute.set("modifiers",mod.get("name"));
				break;
			}
			case 3:
			{
				break;
			}
			default:
			{
				throw new NoViableAltException(_t);
			}
			}
			}
			_t = __t41;
			_t = _t.getNextSibling();
		}
		catch (RecognitionException ex) {
			reportError(ex);
			if (_t!=null) {_t = _t.getNextSibling();}
		}
		_retTree = _t;
		return attribute;
	}
	
	public final gov.nasa.arc.l2tools.livingstone.code.componenttype.StateVector  stateVector(AST _t,
		gov.nasa.arc.l2tools.livingstone.DataObject parent
	) throws RecognitionException {
		gov.nasa.arc.l2tools.livingstone.code.componenttype.StateVector stateVector
         =new gov.nasa.arc.l2tools.livingstone.code.componenttype.StateVector();
		
		AST stateVector_AST_in = (AST)_t;
		
		try {      // for error handling
			AST __t56 = _t;
			AST tmp12_AST_in = (AST)_t;
			match(_t,STATE_VECTOR);
			_t = _t.getFirstChild();
			if (parent!=null) parent.getChildren().addElement(stateVector);
			updateSourceInfo(stateVector,(gov.nasa.arc.l2tools.io.jmpl.JmplASTNode)_t);
			String stateVars=new String();
			gov.nasa.arc.l2tools.livingstone.code.Identifier stateAttributeId;
			{
			_loop58:
			do {
				if (_t==null) _t=ASTNULL;
				if ((_t.getType()==QUALIFIED_NAME||_t.getType()==IDENT)) {
					stateAttributeId=identifier(_t,null);
					_t = _retTree;
					
					if (stateVars.length() == 0)
					stateVars=stateAttributeId.get("name");
					else
					stateVars+=" "+stateAttributeId.get("name");
					
				}
				else {
					break _loop58;
				}
				
			} while (true);
			}
			stateVector.set("vars",stateVars);
			_t = __t56;
			_t = _t.getNextSibling();
		}
		catch (RecognitionException ex) {
			reportError(ex);
			if (_t!=null) {_t = _t.getNextSibling();}
		}
		_retTree = _t;
		return stateVector;
	}
	
	public final gov.nasa.arc.l2tools.livingstone.code.componenttype.AttributeType  componentTypeAttributeType(AST _t,
		gov.nasa.arc.l2tools.DataObject parent
	) throws RecognitionException {
		gov.nasa.arc.l2tools.livingstone.code.componenttype.AttributeType attributeType
         =new gov.nasa.arc.l2tools.livingstone.code.componenttype.AttributeType();
		
		AST componentTypeAttributeType_AST_in = (AST)_t;
		AST name = null;
		
		try {      // for error handling
			AST __t23 = _t;
			AST tmp13_AST_in = (AST)_t;
			match(_t,CT_ATTRIBUTE_TYPE);
			_t = _t.getFirstChild();
			if (parent!=null) parent.getChildren().addElement(attributeType);
			updateSourceInfo(attributeType,(gov.nasa.arc.l2tools.io.jmpl.JmplASTNode)_t);
			{
			if (_t==null) _t=ASTNULL;
			switch ( _t.getType()) {
			case IDENT:
			{
				name = (AST)_t;
				match(_t,IDENT);
				_t = _t.getNextSibling();
				attributeType.set("name",name.getText());
				break;
			}
			case 3:
			case ATTRIBUTE_TYPE_VALUE:
			case RELATION:
			case VARIABLE:
			case EXPRESSION:
			case MODIFIERS:
			case EMPTY_STATEMENT:
			case IF_STATEMENT:
			case IFF_STATEMENT:
			case SWITCH_STATEMENT:
			case STATEMENT_LIST:
			{
				break;
			}
			default:
			{
				throw new NoViableAltException(_t);
			}
			}
			}
			gov.nasa.arc.l2tools.livingstone.code.Particle mod;
			{
			if (_t==null) _t=ASTNULL;
			switch ( _t.getType()) {
			case MODIFIERS:
			{
				mod=modifier(_t,attributeType);
				_t = _retTree;
				attributeType.set("modifiers",mod.get("name"));
				break;
			}
			case 3:
			case ATTRIBUTE_TYPE_VALUE:
			case RELATION:
			case VARIABLE:
			case EXPRESSION:
			case EMPTY_STATEMENT:
			case IF_STATEMENT:
			case IFF_STATEMENT:
			case SWITCH_STATEMENT:
			case STATEMENT_LIST:
			{
				break;
			}
			default:
			{
				throw new NoViableAltException(_t);
			}
			}
			}
			gov.nasa.arc.l2tools.livingstone.code.Particle atv;
			{
			_loop27:
			do {
				if (_t==null) _t=ASTNULL;
				if ((_t.getType()==ATTRIBUTE_TYPE_VALUE)) {
					atv=componentTypeAttributeTypeValue(_t,attributeType);
					_t = _retTree;
				}
				else {
					break _loop27;
				}
				
			} while (true);
			}
			gov.nasa.arc.l2tools.livingstone.code.Particle ate;
			{
			_loop29:
			do {
				if (_t==null) _t=ASTNULL;
				if ((_tokenSet_0.member(_t.getType()))) {
					ate=componentTypeAttributeTypeEntry(_t,attributeType);
					_t = _retTree;
				}
				else {
					break _loop29;
				}
				
			} while (true);
			}
			gov.nasa.arc.l2tools.livingstone.code.Particle var;
			{
			_loop31:
			do {
				if (_t==null) _t=ASTNULL;
				if ((_t.getType()==VARIABLE)) {
					var=componentTypeAttributeTypeAttribute(_t,attributeType);
					_t = _retTree;
				}
				else {
					break _loop31;
				}
				
			} while (true);
			}
			_t = __t23;
			_t = _t.getNextSibling();
		}
		catch (RecognitionException ex) {
			reportError(ex);
			if (_t!=null) {_t = _t.getNextSibling();}
		}
		_retTree = _t;
		return attributeType;
	}
	
	public final gov.nasa.arc.l2tools.livingstone.code.componenttype.NominalTransition  transition(AST _t,
		gov.nasa.arc.l2tools.DataObject parent
	) throws RecognitionException {
		gov.nasa.arc.l2tools.livingstone.code.componenttype.NominalTransition transition
         =new gov.nasa.arc.l2tools.livingstone.code.componenttype.NominalTransition();
		
		AST transition_AST_in = (AST)_t;
		AST name = null;
		
		try {      // for error handling
			AST __t64 = _t;
			AST tmp14_AST_in = (AST)_t;
			match(_t,NOMINAL_TRANSITION);
			_t = _t.getFirstChild();
			if (parent!=null) parent.getChildren().addElement(transition);
			updateSourceInfo(transition,(gov.nasa.arc.l2tools.io.jmpl.JmplASTNode)_t);
			{
			if (_t==null) _t=ASTNULL;
			switch ( _t.getType()) {
			case IDENT:
			{
				name = (AST)_t;
				match(_t,IDENT);
				_t = _t.getNextSibling();
				transition.set("name",name.getText());
				break;
			}
			case STATEMENT_LIST:
			case FROM_VECTOR:
			{
				break;
			}
			default:
			{
				throw new NoViableAltException(_t);
			}
			}
			}
			String string;
			{
			if (_t==null) _t=ASTNULL;
			switch ( _t.getType()) {
			case FROM_VECTOR:
			{
				string=fromVectorString(_t);
				_t = _retTree;
				transition.set("from",string);
				{
				if (_t==null) _t=ASTNULL;
				switch ( _t.getType()) {
				case TO_VECTOR:
				{
					string=toVectorString(_t);
					_t = _retTree;
					transition.set("to",string);
					gov.nasa.arc.l2tools.livingstone.code.Particle cost;
					{
					if (_t==null) _t=ASTNULL;
					switch ( _t.getType()) {
					case TRANSITION_COST:
					{
						cost=transitionCost(_t,null);
						_t = _retTree;
						transition.set("cost",cost.get("amount"));
						break;
					}
					case STATEMENT_LIST:
					{
						break;
					}
					default:
					{
						throw new NoViableAltException(_t);
					}
					}
					}
					break;
				}
				case STATEMENT_LIST:
				{
					break;
				}
				default:
				{
					throw new NoViableAltException(_t);
				}
				}
				}
				break;
			}
			case STATEMENT_LIST:
			{
				break;
			}
			default:
			{
				throw new NoViableAltException(_t);
			}
			}
			}
			gov.nasa.arc.l2tools.livingstone.code.Particle list;
			list=statementList(_t,transition);
			_t = _retTree;
			_t = __t64;
			_t = _t.getNextSibling();
		}
		catch (RecognitionException ex) {
			reportError(ex);
			if (_t!=null) {_t = _t.getNextSibling();}
		}
		_retTree = _t;
		return transition;
	}
	
	public final gov.nasa.arc.l2tools.livingstone.code.componenttype.FailureTransition  failure(AST _t,
		gov.nasa.arc.l2tools.DataObject parent
	) throws RecognitionException {
		gov.nasa.arc.l2tools.livingstone.code.componenttype.FailureTransition failure
         =new gov.nasa.arc.l2tools.livingstone.code.componenttype.FailureTransition();
		
		AST failure_AST_in = (AST)_t;
		AST name = null;
		
		try {      // for error handling
			AST __t70 = _t;
			AST tmp15_AST_in = (AST)_t;
			match(_t,FAILURE_TRANSITION);
			_t = _t.getFirstChild();
			if (parent!=null) parent.getChildren().addElement(failure);
			updateSourceInfo(failure,(gov.nasa.arc.l2tools.io.jmpl.JmplASTNode)_t);
			{
			if (_t==null) _t=ASTNULL;
			switch ( _t.getType()) {
			case IDENT:
			{
				name = (AST)_t;
				match(_t,IDENT);
				_t = _t.getNextSibling();
				failure.set("name",name.getText());
				break;
			}
			case STATEMENT_LIST:
			case FROM_VECTOR:
			{
				break;
			}
			default:
			{
				throw new NoViableAltException(_t);
			}
			}
			}
			String string;
			{
			if (_t==null) _t=ASTNULL;
			switch ( _t.getType()) {
			case FROM_VECTOR:
			{
				string=fromVectorString(_t);
				_t = _retTree;
				failure.set("from",string);
				{
				if (_t==null) _t=ASTNULL;
				switch ( _t.getType()) {
				case TO_VECTOR:
				{
					string=toVectorString(_t);
					_t = _retTree;
					failure.set("to",string);
					gov.nasa.arc.l2tools.livingstone.code.Particle probability;
					{
					if (_t==null) _t=ASTNULL;
					switch ( _t.getType()) {
					case FAILURE_PROBABILITY:
					{
						probability=failureProbability(_t,null);
						_t = _retTree;
						failure.set("probability",probability.get("amount"));
						break;
					}
					case STATEMENT_LIST:
					{
						break;
					}
					default:
					{
						throw new NoViableAltException(_t);
					}
					}
					}
					break;
				}
				case STATEMENT_LIST:
				{
					break;
				}
				default:
				{
					throw new NoViableAltException(_t);
				}
				}
				}
				break;
			}
			case STATEMENT_LIST:
			{
				break;
			}
			default:
			{
				throw new NoViableAltException(_t);
			}
			}
			}
			gov.nasa.arc.l2tools.livingstone.code.Particle list;
			list=statementList(_t,failure);
			_t = _retTree;
			_t = __t70;
			_t = _t.getNextSibling();
		}
		catch (RecognitionException ex) {
			reportError(ex);
			if (_t!=null) {_t = _t.getNextSibling();}
		}
		_retTree = _t;
		return failure;
	}
	
	public final gov.nasa.arc.l2tools.livingstone.code.constraint.StatementGroup
          statementList(AST _t,
		gov.nasa.arc.l2tools.livingstone.DataObject parent
	) throws RecognitionException {
		gov.nasa.arc.l2tools.livingstone.code.constraint.StatementGroup
         st=new gov.nasa.arc.l2tools.livingstone.code.constraint.StatementGroup();
		
		AST statementList_AST_in = (AST)_t;
		
		try {      // for error handling
			AST __t95 = _t;
			AST tmp16_AST_in = (AST)_t;
			match(_t,STATEMENT_LIST);
			_t = _t.getFirstChild();
			if (parent!=null) parent.getChildren().addElement(st);
			updateSourceInfo(st,(gov.nasa.arc.l2tools.io.jmpl.JmplASTNode)_t);
			gov.nasa.arc.l2tools.livingstone.code.constraint.Statement s;
			{
			_loop97:
			do {
				if (_t==null) _t=ASTNULL;
				if ((_tokenSet_2.member(_t.getType()))) {
					s=statement(_t,st);
					_t = _retTree;
				}
				else {
					break _loop97;
				}
				
			} while (true);
			}
			_t = __t95;
			_t = _t.getNextSibling();
		}
		catch (RecognitionException ex) {
			reportError(ex);
			if (_t!=null) {_t = _t.getNextSibling();}
		}
		_retTree = _t;
		return st;
	}
	
	public final gov.nasa.arc.l2tools.livingstone.code.Modifier
          modifier(AST _t,
		gov.nasa.arc.l2tools.livingstone.DataObject parent
	) throws RecognitionException {
		gov.nasa.arc.l2tools.livingstone.code.Modifier
         keyword=new gov.nasa.arc.l2tools.livingstone.code.Modifier();
		
		AST modifier_AST_in = (AST)_t;
		
		try {      // for error handling
			AST __t53 = _t;
			AST tmp17_AST_in = (AST)_t;
			match(_t,MODIFIERS);
			_t = _t.getFirstChild();
			{
			if (_t==null) _t=ASTNULL;
			switch ( _t.getType()) {
			case PRIVATE_KEYWORD:
			{
				AST tmp18_AST_in = (AST)_t;
				match(_t,PRIVATE_KEYWORD);
				_t = _t.getNextSibling();
				keyword.set("name","private");
				break;
			}
			case PUBLIC_KEYWORD:
			{
				AST tmp19_AST_in = (AST)_t;
				match(_t,PUBLIC_KEYWORD);
				_t = _t.getNextSibling();
				keyword.set("name","public");
				break;
			}
			default:
			{
				throw new NoViableAltException(_t);
			}
			}
			}
			_t = __t53;
			_t = _t.getNextSibling();
		}
		catch (RecognitionException ex) {
			reportError(ex);
			if (_t!=null) {_t = _t.getNextSibling();}
		}
		_retTree = _t;
		return keyword;
	}
	
	public final gov.nasa.arc.l2tools.livingstone.code.componenttype.Value
          componentTypeAttributeTypeValue(AST _t,
		gov.nasa.arc.l2tools.livingstone.DataObject parent
	) throws RecognitionException {
		gov.nasa.arc.l2tools.livingstone.code.componenttype.Value
         componentTypeAttributeTypeValue=new gov.nasa.arc.l2tools.livingstone.code.componenttype.Value();
		
		AST componentTypeAttributeTypeValue_AST_in = (AST)_t;
		AST valueName = null;
		AST valueString = null;
		
		try {      // for error handling
			AST __t37 = _t;
			AST tmp20_AST_in = (AST)_t;
			match(_t,ATTRIBUTE_TYPE_VALUE);
			_t = _t.getFirstChild();
			if (parent!=null) parent.getChildren().addElement(componentTypeAttributeTypeValue);
			updateSourceInfo(componentTypeAttributeTypeValue,(gov.nasa.arc.l2tools.io.jmpl.JmplASTNode)_t);
			valueName = (AST)_t;
			match(_t,IDENT);
			_t = _t.getNextSibling();
			componentTypeAttributeTypeValue.set("name",valueName.getText());
			{
			if (_t==null) _t=ASTNULL;
			switch ( _t.getType()) {
			case STRING_LITERAL:
			{
				valueString = (AST)_t;
				match(_t,STRING_LITERAL);
				_t = _t.getNextSibling();
				componentTypeAttributeTypeValue.set("value",valueString.getText());
				break;
			}
			case 3:
			{
				break;
			}
			default:
			{
				throw new NoViableAltException(_t);
			}
			}
			}
			_t = __t37;
			_t = _t.getNextSibling();
		}
		catch (RecognitionException ex) {
			reportError(ex);
			if (_t!=null) {_t = _t.getNextSibling();}
		}
		_retTree = _t;
		return componentTypeAttributeTypeValue;
	}
	
	public final gov.nasa.arc.l2tools.livingstone.code.Particle  componentTypeAttributeTypeEntry(AST _t,
		gov.nasa.arc.l2tools.livingstone.DataObject parent
	) throws RecognitionException {
		gov.nasa.arc.l2tools.livingstone.code.Particle componentTypeAttributeTypeEntry=null;
		
		AST componentTypeAttributeTypeEntry_AST_in = (AST)_t;
		
		try {      // for error handling
			if (_t==null) _t=ASTNULL;
			switch ( _t.getType()) {
			case RELATION:
			{
				componentTypeAttributeTypeEntry=relation(_t,parent);
				_t = _retTree;
				break;
			}
			case EXPRESSION:
			case EMPTY_STATEMENT:
			case IF_STATEMENT:
			case IFF_STATEMENT:
			case SWITCH_STATEMENT:
			case STATEMENT_LIST:
			{
				componentTypeAttributeTypeEntry=statement(_t,parent);
				_t = _retTree;
				break;
			}
			default:
			{
				throw new NoViableAltException(_t);
			}
			}
		}
		catch (RecognitionException ex) {
			reportError(ex);
			if (_t!=null) {_t = _t.getNextSibling();}
		}
		_retTree = _t;
		return componentTypeAttributeTypeEntry;
	}
	
	public final gov.nasa.arc.l2tools.livingstone.code.componenttype.Attribute  componentTypeAttributeTypeAttribute(AST _t,
		gov.nasa.arc.l2tools.livingstone.DataObject parent
	) throws RecognitionException {
		gov.nasa.arc.l2tools.livingstone.code.componenttype.Attribute attribute
         =new gov.nasa.arc.l2tools.livingstone.code.componenttype.Attribute();
		
		AST componentTypeAttributeTypeAttribute_AST_in = (AST)_t;
		AST name = null;
		
		try {      // for error handling
			AST __t33 = _t;
			AST tmp21_AST_in = (AST)_t;
			match(_t,VARIABLE);
			_t = _t.getFirstChild();
			if (parent!=null) parent.getChildren().addElement(attribute);
			updateSourceInfo(attribute,(gov.nasa.arc.l2tools.io.jmpl.JmplASTNode)_t);
			{
			name = (AST)_t;
			match(_t,IDENT);
			_t = _t.getNextSibling();
			}
			attribute.set("name",name.getText());
			gov.nasa.arc.l2tools.livingstone.code.Identifier value;
			{
			if (_t==null) _t=ASTNULL;
			switch ( _t.getType()) {
			case QUALIFIED_NAME:
			case IDENT:
			{
				value=identifier(_t,null);
				_t = _retTree;
				attribute.set("value",value.get("name"));
				break;
			}
			case 3:
			{
				break;
			}
			default:
			{
				throw new NoViableAltException(_t);
			}
			}
			}
			_t = __t33;
			_t = _t.getNextSibling();
		}
		catch (RecognitionException ex) {
			reportError(ex);
			if (_t!=null) {_t = _t.getNextSibling();}
		}
		_retTree = _t;
		return attribute;
	}
	
	public final gov.nasa.arc.l2tools.livingstone.code.Identifier  contextIdentifier(AST _t,
		gov.nasa.arc.l2tools.livingstone.DataObject parent
	) throws RecognitionException {
		gov.nasa.arc.l2tools.livingstone.code.Identifier id=null;
		
		AST contextIdentifier_AST_in = (AST)_t;
		AST ck = null;
		
		try {      // for error handling
			if (_t==null) _t=ASTNULL;
			switch ( _t.getType()) {
			case CONTEXT_KEYWORD:
			{
				AST __t46 = _t;
				ck = _t==ASTNULL ? null :(AST)_t;
				match(_t,CONTEXT_KEYWORD);
				_t = _t.getFirstChild();
				updateSourceInfo(id,(gov.nasa.arc.l2tools.io.jmpl.JmplASTNode)_t);
				id=new gov.nasa.arc.l2tools.livingstone.code.Identifier();
				if (parent!=null) parent.getChildren().addElement(id);
				id.set("name",ck.getText());
				_t = __t46;
				_t = _t.getNextSibling();
				break;
			}
			case QUALIFIED_NAME:
			case IDENT:
			{
				id=identifier(_t,parent);
				_t = _retTree;
				break;
			}
			default:
			{
				throw new NoViableAltException(_t);
			}
			}
		}
		catch (RecognitionException ex) {
			reportError(ex);
			if (_t!=null) {_t = _t.getNextSibling();}
		}
		_retTree = _t;
		return id;
	}
	
	public final String  fromVectorString(AST _t) throws RecognitionException {
		String fromVector = null;
		
		AST fromVectorString_AST_in = (AST)_t;
		
		try {      // for error handling
			AST __t83 = _t;
			AST tmp22_AST_in = (AST)_t;
			match(_t,FROM_VECTOR);
			_t = _t.getFirstChild();
			gov.nasa.arc.l2tools.livingstone.code.componenttype.FromStateIdentifier fromId;
			{
			_loop85:
			do {
				if (_t==null) _t=ASTNULL;
				if ((_tokenSet_3.member(_t.getType()))) {
					fromId=fromStateIdentifier(_t,null);
					_t = _retTree;
					
					if (fromVector == null)
					fromVector=fromId.get("name");
					else
					fromVector+=" "+fromId.get("name");
					
				}
				else {
					break _loop85;
				}
				
			} while (true);
			}
			_t = __t83;
			_t = _t.getNextSibling();
		}
		catch (RecognitionException ex) {
			reportError(ex);
			if (_t!=null) {_t = _t.getNextSibling();}
		}
		_retTree = _t;
		return fromVector;
	}
	
	public final String  toVectorString(AST _t) throws RecognitionException {
		String toVector = null;
		
		AST toVectorString_AST_in = (AST)_t;
		
		try {      // for error handling
			AST __t89 = _t;
			AST tmp23_AST_in = (AST)_t;
			match(_t,TO_VECTOR);
			_t = _t.getFirstChild();
			gov.nasa.arc.l2tools.livingstone.code.componenttype.ToStateIdentifier toId;
			{
			_loop91:
			do {
				if (_t==null) _t=ASTNULL;
				if ((_tokenSet_4.member(_t.getType()))) {
					toId=toStateIdentifier(_t,null);
					_t = _retTree;
					
					if (toVector == null)
					toVector=toId.get("name");
					else
					toVector+=" "+toId.get("name");
					
				}
				else {
					break _loop91;
				}
				
			} while (true);
			}
			_t = __t89;
			_t = _t.getNextSibling();
		}
		catch (RecognitionException ex) {
			reportError(ex);
			if (_t!=null) {_t = _t.getNextSibling();}
		}
		_retTree = _t;
		return toVector;
	}
	
	public final gov.nasa.arc.l2tools.livingstone.code.componenttype.TransitionCost  transitionCost(AST _t,
		gov.nasa.arc.l2tools.livingstone.code.Particle parent
	) throws RecognitionException {
		gov.nasa.arc.l2tools.livingstone.code.componenttype.TransitionCost cost
         =new gov.nasa.arc.l2tools.livingstone.code.componenttype.TransitionCost();
		
		AST transitionCost_AST_in = (AST)_t;
		AST c = null;
		
		try {      // for error handling
			AST __t76 = _t;
			AST tmp24_AST_in = (AST)_t;
			match(_t,TRANSITION_COST);
			_t = _t.getFirstChild();
			gov.nasa.arc.l2tools.livingstone.code.Identifier id;
			{
			if (_t==null) _t=ASTNULL;
			switch ( _t.getType()) {
			case QUALIFIED_NAME:
			case IDENT:
			{
				id=identifier(_t,null);
				_t = _retTree;
				if (parent!=null) parent.getChildren().addElement(cost);
				cost.set("amount",id.get("name"));
				break;
			}
			case STRING_LITERAL:
			case NUM_INT:
			case CHAR_LITERAL:
			case NUM_FLOAT:
			{
				c = _t==ASTNULL ? null : (AST)_t;
				constant(_t);
				_t = _retTree;
				if (parent!=null) parent.getChildren().addElement(cost);
				cost.set("amount",c.getText());
				break;
			}
			default:
			{
				throw new NoViableAltException(_t);
			}
			}
			}
			_t = __t76;
			_t = _t.getNextSibling();
		}
		catch (RecognitionException ex) {
			reportError(ex);
			if (_t!=null) {_t = _t.getNextSibling();}
		}
		_retTree = _t;
		return cost;
	}
	
	public final gov.nasa.arc.l2tools.livingstone.code.componenttype.FailureProbability  failureProbability(AST _t,
		gov.nasa.arc.l2tools.livingstone.code.Particle parent
	) throws RecognitionException {
		gov.nasa.arc.l2tools.livingstone.code.componenttype.FailureProbability probability
         =new gov.nasa.arc.l2tools.livingstone.code.componenttype.FailureProbability();
		
		AST failureProbability_AST_in = (AST)_t;
		AST c = null;
		
		try {      // for error handling
			AST __t79 = _t;
			AST tmp25_AST_in = (AST)_t;
			match(_t,FAILURE_PROBABILITY);
			_t = _t.getFirstChild();
			gov.nasa.arc.l2tools.livingstone.code.Identifier id;
			{
			if (_t==null) _t=ASTNULL;
			switch ( _t.getType()) {
			case QUALIFIED_NAME:
			case IDENT:
			{
				id=identifier(_t,null);
				_t = _retTree;
				probability.set("amount",id.get("name"));
				if (parent!=null) parent.getChildren().addElement(probability);
				break;
			}
			case STRING_LITERAL:
			case NUM_INT:
			case CHAR_LITERAL:
			case NUM_FLOAT:
			{
				c = _t==ASTNULL ? null : (AST)_t;
				constant(_t);
				_t = _retTree;
				if (parent!=null) parent.getChildren().addElement(probability);
				probability.set("amount",c.getText());
				break;
			}
			default:
			{
				throw new NoViableAltException(_t);
			}
			}
			}
			_t = __t79;
			_t = _t.getNextSibling();
		}
		catch (RecognitionException ex) {
			reportError(ex);
			if (_t!=null) {_t = _t.getNextSibling();}
		}
		_retTree = _t;
		return probability;
	}
	
	public final void constant(AST _t) throws RecognitionException {
		
		AST constant_AST_in = (AST)_t;
		
		try {      // for error handling
			if (_t==null) _t=ASTNULL;
			switch ( _t.getType()) {
			case NUM_INT:
			{
				AST tmp26_AST_in = (AST)_t;
				match(_t,NUM_INT);
				_t = _t.getNextSibling();
				break;
			}
			case CHAR_LITERAL:
			{
				AST tmp27_AST_in = (AST)_t;
				match(_t,CHAR_LITERAL);
				_t = _t.getNextSibling();
				break;
			}
			case STRING_LITERAL:
			{
				AST tmp28_AST_in = (AST)_t;
				match(_t,STRING_LITERAL);
				_t = _t.getNextSibling();
				break;
			}
			case NUM_FLOAT:
			{
				AST tmp29_AST_in = (AST)_t;
				match(_t,NUM_FLOAT);
				_t = _t.getNextSibling();
				break;
			}
			default:
			{
				throw new NoViableAltException(_t);
			}
			}
		}
		catch (RecognitionException ex) {
			reportError(ex);
			if (_t!=null) {_t = _t.getNextSibling();}
		}
		_retTree = _t;
	}
	
	public final gov.nasa.arc.l2tools.livingstone.code.componenttype.FromStateIdentifier  fromStateIdentifier(AST _t,
		gov.nasa.arc.l2tools.livingstone.DataObject parent
	) throws RecognitionException {
		gov.nasa.arc.l2tools.livingstone.code.componenttype.FromStateIdentifier fromState
         =new gov.nasa.arc.l2tools.livingstone.code.componenttype.FromStateIdentifier();
		
		AST fromStateIdentifier_AST_in = (AST)_t;
		AST n = null;
		AST s = null;
		
		try {      // for error handling
			gov.nasa.arc.l2tools.livingstone.code.Identifier id;
			{
			if (_t==null) _t=ASTNULL;
			switch ( _t.getType()) {
			case NOTHING_FOR_STATE_VAR:
			{
				n = (AST)_t;
				match(_t,NOTHING_FOR_STATE_VAR);
				_t = _t.getNextSibling();
				if (parent!=null) parent.getChildren().addElement(fromState);
				fromState.set("name",n.getText());
				break;
			}
			case STAR:
			{
				s = (AST)_t;
				match(_t,STAR);
				_t = _t.getNextSibling();
				if (parent!=null) parent.getChildren().addElement(fromState);
				fromState.set("name",s.getText());
				break;
			}
			case QUALIFIED_NAME:
			case IDENT:
			{
				id=identifier(_t,null);
				_t = _retTree;
				if (parent!=null) parent.getChildren().addElement(fromState);
				fromState.set("name",id.get("name"));
				break;
			}
			default:
			{
				throw new NoViableAltException(_t);
			}
			}
			}
		}
		catch (RecognitionException ex) {
			reportError(ex);
			if (_t!=null) {_t = _t.getNextSibling();}
		}
		_retTree = _t;
		return fromState;
	}
	
	public final gov.nasa.arc.l2tools.livingstone.code.componenttype.ToStateIdentifier  toStateIdentifier(AST _t,
		gov.nasa.arc.l2tools.livingstone.DataObject parent
	) throws RecognitionException {
		gov.nasa.arc.l2tools.livingstone.code.componenttype.ToStateIdentifier toState
         =new gov.nasa.arc.l2tools.livingstone.code.componenttype.ToStateIdentifier();
		
		AST toStateIdentifier_AST_in = (AST)_t;
		AST n = null;
		AST p = null;
		
		try {      // for error handling
			gov.nasa.arc.l2tools.livingstone.code.Identifier id;
			{
			if (_t==null) _t=ASTNULL;
			switch ( _t.getType()) {
			case NOTHING_FOR_STATE_VAR:
			{
				n = (AST)_t;
				match(_t,NOTHING_FOR_STATE_VAR);
				_t = _t.getNextSibling();
				if (parent!=null) parent.getChildren().addElement(toState);
				toState.set("name",n.getText());
				break;
			}
			case PLUS:
			{
				p = (AST)_t;
				match(_t,PLUS);
				_t = _t.getNextSibling();
				if (parent!=null) parent.getChildren().addElement(toState);
				toState.set("name",p.getText());
				break;
			}
			case QUALIFIED_NAME:
			case IDENT:
			{
				id=identifier(_t,null);
				_t = _retTree;
				if (parent!=null) parent.getChildren().addElement(toState);
				toState.set("name",id.get("name"));
				break;
			}
			default:
			{
				throw new NoViableAltException(_t);
			}
			}
			}
		}
		catch (RecognitionException ex) {
			reportError(ex);
			if (_t!=null) {_t = _t.getNextSibling();}
		}
		_retTree = _t;
		return toState;
	}
	
	public final gov.nasa.arc.l2tools.livingstone.code.constraint.Expression  expression(AST _t,
		gov.nasa.arc.l2tools.livingstone.DataObject parent
	) throws RecognitionException {
		gov.nasa.arc.l2tools.livingstone.code.constraint.Expression e=null;;
		
		AST expression_AST_in = (AST)_t;
		
		try {      // for error handling
			AST __t113 = _t;
			AST tmp30_AST_in = (AST)_t;
			match(_t,EXPRESSION);
			_t = _t.getFirstChild();
			e=expr(_t,parent);
			_t = _retTree;
			_t = __t113;
			_t = _t.getNextSibling();
		}
		catch (RecognitionException ex) {
			reportError(ex);
			if (_t!=null) {_t = _t.getNextSibling();}
		}
		_retTree = _t;
		return e;
	}
	
	public final gov.nasa.arc.l2tools.livingstone.code.constraint.CaseGroup
          casesGroup(AST _t,
		gov.nasa.arc.l2tools.livingstone.code.Particle parent
	) throws RecognitionException {
		gov.nasa.arc.l2tools.livingstone.code.constraint.CaseGroup
         cg=new gov.nasa.arc.l2tools.livingstone.code.constraint.CaseGroup();
		
		AST casesGroup_AST_in = (AST)_t;
		
		try {      // for error handling
			AST __t107 = _t;
			AST tmp31_AST_in = (AST)_t;
			match(_t,CASE_GROUP);
			_t = _t.getFirstChild();
			if (parent!=null) parent.getChildren().addElement(cg);
			updateSourceInfo(cg,(gov.nasa.arc.l2tools.io.jmpl.JmplASTNode)_t);
			String names=new String("");
			gov.nasa.arc.l2tools.livingstone.code.Identifier id;
			{
			int _cnt109=0;
			_loop109:
			do {
				if (_t==null) _t=ASTNULL;
				if ((_t.getType()==QUALIFIED_NAME||_t.getType()==IDENT)) {
					id=identifier(_t,null);
					_t = _retTree;
					
					if (names.length() == 0) 
					names=id.get("name");
					else
					names+=" "+id.get("name");
					
				}
				else {
					if ( _cnt109>=1 ) { break _loop109; } else {throw new NoViableAltException(_t);}
				}
				
				_cnt109++;
			} while (true);
			}
			cg.set("names",names);
			gov.nasa.arc.l2tools.livingstone.code.constraint.Statement s;
			{
			int _cnt111=0;
			_loop111:
			do {
				if (_t==null) _t=ASTNULL;
				if ((_tokenSet_2.member(_t.getType()))) {
					s=statement(_t,cg);
					_t = _retTree;
				}
				else {
					if ( _cnt111>=1 ) { break _loop111; } else {throw new NoViableAltException(_t);}
				}
				
				_cnt111++;
			} while (true);
			}
			_t = __t107;
			_t = _t.getNextSibling();
		}
		catch (RecognitionException ex) {
			reportError(ex);
			if (_t!=null) {_t = _t.getNextSibling();}
		}
		_retTree = _t;
		return cg;
	}
	
	public final gov.nasa.arc.l2tools.livingstone.code.constraint.Expression  expr(AST _t,
		gov.nasa.arc.l2tools.livingstone.DataObject parent
	) throws RecognitionException {
		gov.nasa.arc.l2tools.livingstone.code.constraint.Expression e=null;
		
		AST expr_AST_in = (AST)_t;
		gov.nasa.arc.l2tools.livingstone.code.constraint.Expression ignoreExpr;
		
		try {      // for error handling
			if (_t==null) _t=ASTNULL;
			switch ( _t.getType()) {
			case LOGICAL_OR_EXPRESSION:
			{
				AST __t115 = _t;
				AST tmp32_AST_in = (AST)_t;
				match(_t,LOGICAL_OR_EXPRESSION);
				_t = _t.getFirstChild();
				e=new gov.nasa.arc.l2tools.livingstone.code.constraint.OrExpression();
				if (parent!=null) parent.getChildren().addElement(e);
				updateSourceInfo(e,(gov.nasa.arc.l2tools.io.jmpl.JmplASTNode)_t);
				{
				_loop117:
				do {
					if (_t==null) _t=ASTNULL;
					if (((_t.getType() >= LOGICAL_OR_EXPRESSION && _t.getType() <= CONSTRAINT_INVOCATION))) {
						ignoreExpr=expr(_t,e);
						_t = _retTree;
					}
					else {
						break _loop117;
					}
					
				} while (true);
				}
				_t = __t115;
				_t = _t.getNextSibling();
				break;
			}
			case LOGICAL_AND_EXPRESSION:
			{
				AST __t118 = _t;
				AST tmp33_AST_in = (AST)_t;
				match(_t,LOGICAL_AND_EXPRESSION);
				_t = _t.getFirstChild();
				e=new gov.nasa.arc.l2tools.livingstone.code.constraint.AndExpression();
				if (parent!=null) parent.getChildren().addElement(e);
				updateSourceInfo(e,(gov.nasa.arc.l2tools.io.jmpl.JmplASTNode)_t);
				{
				_loop120:
				do {
					if (_t==null) _t=ASTNULL;
					if (((_t.getType() >= LOGICAL_OR_EXPRESSION && _t.getType() <= CONSTRAINT_INVOCATION))) {
						ignoreExpr=expr(_t,e);
						_t = _retTree;
					}
					else {
						break _loop120;
					}
					
				} while (true);
				}
				_t = __t118;
				_t = _t.getNextSibling();
				break;
			}
			case LOGICAL_NEGATION_EXPRESSION:
			{
				AST __t121 = _t;
				AST tmp34_AST_in = (AST)_t;
				match(_t,LOGICAL_NEGATION_EXPRESSION);
				_t = _t.getFirstChild();
				e=new gov.nasa.arc.l2tools.livingstone.code.constraint.NegationExpression();
				if (parent!=null) parent.getChildren().addElement(e);
				updateSourceInfo(e,(gov.nasa.arc.l2tools.io.jmpl.JmplASTNode)_t);
				ignoreExpr=expr(_t,e);
				_t = _retTree;
				_t = __t121;
				_t = _t.getNextSibling();
				break;
			}
			case LOGICAL_EQUALITY_EXPRESSION:
			{
				AST __t122 = _t;
				AST tmp35_AST_in = (AST)_t;
				match(_t,LOGICAL_EQUALITY_EXPRESSION);
				_t = _t.getFirstChild();
				e=new gov.nasa.arc.l2tools.livingstone.code.constraint.EqualityExpression();
				if (parent!=null) parent.getChildren().addElement(e);
				updateSourceInfo(e,(gov.nasa.arc.l2tools.io.jmpl.JmplASTNode)_t);
				gov.nasa.arc.l2tools.livingstone.code.Identifier equality_id;
				String arguments=new String("");
				{
				_loop124:
				do {
					if (_t==null) _t=ASTNULL;
					if ((_t.getType()==CONTEXT_KEYWORD||_t.getType()==QUALIFIED_NAME||_t.getType()==IDENT)) {
						equality_id=contextIdentifier(_t,null);
						_t = _retTree;
						
						if (arguments.length() == 0)
						arguments=equality_id.get("name");
						else
						arguments+=" "+equality_id.get("name");
						
					}
					else {
						break _loop124;
					}
					
				} while (true);
				}
				e.set("arguments",arguments);
				_t = __t122;
				_t = _t.getNextSibling();
				break;
			}
			case LOGICAL_INEQUALITY_EXPRESSION:
			{
				AST __t125 = _t;
				AST tmp36_AST_in = (AST)_t;
				match(_t,LOGICAL_INEQUALITY_EXPRESSION);
				_t = _t.getFirstChild();
				e=new gov.nasa.arc.l2tools.livingstone.code.constraint.InequalityExpression();
				if (parent!=null) parent.getChildren().addElement(e);
				updateSourceInfo(e,(gov.nasa.arc.l2tools.io.jmpl.JmplASTNode)_t);
				gov.nasa.arc.l2tools.livingstone.code.Identifier inequality_id;
				String arguments=new String("");
				{
				_loop127:
				do {
					if (_t==null) _t=ASTNULL;
					if ((_t.getType()==CONTEXT_KEYWORD||_t.getType()==QUALIFIED_NAME||_t.getType()==IDENT)) {
						inequality_id=contextIdentifier(_t,null);
						_t = _retTree;
						
						if (arguments.length() == 0)
						arguments=inequality_id.get("name");
						else
						arguments+=" "+inequality_id.get("name");
						
					}
					else {
						break _loop127;
					}
					
				} while (true);
				}
				e.set("arguments",arguments);
				_t = __t125;
				_t = _t.getNextSibling();
				break;
			}
			case CONSTRAINT_INVOCATION:
			{
				AST __t128 = _t;
				AST tmp37_AST_in = (AST)_t;
				match(_t,CONSTRAINT_INVOCATION);
				_t = _t.getFirstChild();
				e=new gov.nasa.arc.l2tools.livingstone.code.constraint.ConstraintInvocation();
				if (parent!=null) parent.getChildren().addElement(e);
				updateSourceInfo(e,(gov.nasa.arc.l2tools.io.jmpl.JmplASTNode)_t);
				gov.nasa.arc.l2tools.livingstone.code.Identifier name; 
				gov.nasa.arc.l2tools.livingstone.code.Identifier id;
				name=identifier(_t,null);
				_t = _retTree;
				e.set("name",name.get("name"));
				String arguments=new String("");
				{
				_loop130:
				do {
					if (_t==null) _t=ASTNULL;
					if ((_t.getType()==CONTEXT_KEYWORD||_t.getType()==QUALIFIED_NAME||_t.getType()==IDENT)) {
						id=contextIdentifier(_t,null);
						_t = _retTree;
						
						if (arguments.length() == 0)
						arguments=id.get("name");
						else
						arguments+=" "+id.get("name");
						
					}
					else {
						break _loop130;
					}
					
				} while (true);
				}
				e.set("arguments",arguments);
				_t = __t128;
				_t = _t.getNextSibling();
				break;
			}
			default:
			{
				throw new NoViableAltException(_t);
			}
			}
		}
		catch (RecognitionException ex) {
			reportError(ex);
			if (_t!=null) {_t = _t.getNextSibling();}
		}
		_retTree = _t;
		return e;
	}
	
	
	public static final String[] _tokenNames = {
		"<0>",
		"EOF",
		"<2>",
		"NULL_TREE_LOOKAHEAD",
		"\"private\"",
		"\"public\"",
		"\"Enum\"",
		"\"enum\"",
		"\"Class\"",
		"\"class\"",
		"\"extends\"",
		"\"stateVector\"",
		"\"statevector\"",
		"\"relation\"",
		"\"transition\"",
		"\"failure\"",
		"\"iff\"",
		"\"if\"",
		"\"else\"",
		"\"switch\"",
		"\"case\"",
		"\"this\"",
		"ATTRIBUTE_TYPE",
		"ATTRIBUTE_TYPE_VALUE",
		"COMPONENT_TYPE",
		"EXTENSION_CLAUSE",
		"CT_ATTRIBUTE_TYPE",
		"RELATION",
		"NOMINAL_TRANSITION",
		"TRANSITION_COST",
		"FAILURE_TRANSITION",
		"FAILURE_PROBABILITY",
		"STATE_VECTOR",
		"VARIABLE",
		"EXPRESSION",
		"MODIFIERS",
		"CTE_LIST",
		"EMPTY_STATEMENT",
		"IF_STATEMENT",
		"IFF_STATEMENT",
		"SWITCH_STATEMENT",
		"CASE_GROUP",
		"STATEMENT_LIST",
		"LOGICAL_OR_EXPRESSION",
		"LOGICAL_AND_EXPRESSION",
		"LOGICAL_NEGATION_EXPRESSION",
		"LOGICAL_EQUALITY_EXPRESSION",
		"LOGICAL_INEQUALITY_EXPRESSION",
		"CONSTRAINT_INVOCATION",
		"QUALIFIED_NAME",
		"FROM_VECTOR",
		"TO_VECTOR",
		"NOTHING_FOR_STATE_VAR",
		"an identifier, which must begin with a letter, underscore, or dollar sign",
		"a left curly brace '{'",
		"a right curly brace '}'",
		"a semicolon ';'",
		"a comma ','",
		"an equals operator '='",
		"a string literal, such as \\\"a String Literal\\\"",
		"DOT",
		"a left bracket '['",
		"a right bracket ']'",
		"a left parenthesis '('",
		"a right parenthesis ')'",
		"a star '*'",
		"a plus sign '+'",
		"a numeric literal, which must start with a digit or a period",
		"a character literal, such as 'z'",
		"NUM_FLOAT",
		"a colon ':'",
		"a logical or operator '|'",
		"a double or operator '||'",
		"a logical and operator '&'",
		"a double and operator '&&'",
		"a logical not operator '~'",
		"a logical not operator '!'",
		"a double equals operator '=='",
		"a not equals operator '!='",
		"whitespace, such as a space, tab, or newline",
		"a single line comment, delimited by //",
		"a multiple line comment, such as /* <comment> */",
		"an escape sequence, such as \\\\\\\\039",
		"a hexadecimal digit, such as A or a or 3",
		"VOCAB",
		"a floating point number exponent, such as e+12 or E-002",
		"a floating point number suffix, one of: f, F, d, or D"
	};
	
	private static final long _tokenSet_0_data_[] = { 6476944900096L, 0L };
	public static final BitSet _tokenSet_0 = new BitSet(_tokenSet_0_data_);
	private static final long _tokenSet_1_data_[] = { 4412474916864L, 0L };
	public static final BitSet _tokenSet_1 = new BitSet(_tokenSet_1_data_);
	private static final long _tokenSet_2_data_[] = { 6476810682368L, 0L };
	public static final BitSet _tokenSet_2 = new BitSet(_tokenSet_2_data_);
	private static final long _tokenSet_3_data_[] = { 14073748835532800L, 2L, 0L, 0L };
	public static final BitSet _tokenSet_3 = new BitSet(_tokenSet_3_data_);
	private static final long _tokenSet_4_data_[] = { 14073748835532800L, 4L, 0L, 0L };
	public static final BitSet _tokenSet_4 = new BitSet(_tokenSet_4_data_);
	}
	

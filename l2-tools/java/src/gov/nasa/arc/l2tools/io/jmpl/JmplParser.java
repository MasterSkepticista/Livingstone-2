package gov.nasa.arc.l2tools.io.jmpl;

// $ANTLR 2.7.0: "antlr/jmpl.g" -> "JmplParser.java"$
import antlr.TokenBuffer;
import antlr.TokenStreamException;
import antlr.TokenStreamIOException;
import antlr.ANTLRException;
import antlr.LLkParser;
import antlr.Token;
import antlr.TokenStream;
import antlr.RecognitionException;
import antlr.NoViableAltException;
import antlr.MismatchedTokenException;
import antlr.SemanticException;
import antlr.ParserSharedInputState;
import antlr.collections.impl.BitSet;
import antlr.collections.AST;
import antlr.ASTPair;
import antlr.collections.impl.ASTArray;
public class JmplParser extends antlr.LLkParser
       implements JmplTokenTypes
 {


	protected void updateSourceInfo(JmplASTNode destination, JmplASTNode source) {
    destination.setLine(source.getLine());
    destination.setColumn(source.getColumn());
    destination.setFilename(source.getFilename());
	}

protected JmplParser(TokenBuffer tokenBuf, int k) {
  super(tokenBuf,k);
  tokenNames = _tokenNames;
}

public JmplParser(TokenBuffer tokenBuf) {
  this(tokenBuf,2);
}

protected JmplParser(TokenStream lexer, int k) {
  super(lexer,k);
  tokenNames = _tokenNames;
}

public JmplParser(TokenStream lexer) {
  this(lexer,2);
}

public JmplParser(ParserSharedInputState state) {
  super(state,2);
  tokenNames = _tokenNames;
}

	public final void jmpl() throws RecognitionException, TokenStreamException {
		
		returnAST = null;
		ASTPair currentAST = new ASTPair();
		AST jmpl_AST = null;
		
		try {      // for error handling
			{
			_loop3:
			do {
				switch ( LA(1)) {
				case ATTRIBUTE_TYPE_KEYWORD:
				case ATTRIBUTE_TYPE_KEYWORD_FLAT:
				{
					attributeType();
					astFactory.addASTChild(currentAST, returnAST);
					break;
				}
				case COMPONENT_TYPE_KEYWORD:
				case COMPONENT_TYPE_KEYWORD_FLAT:
				{
					componentType();
					astFactory.addASTChild(currentAST, returnAST);
					break;
				}
				default:
				{
					break _loop3;
				}
				}
			} while (true);
			}
			AST tmp1_AST = null;
			tmp1_AST = (AST)astFactory.create(LT(1));
			match(Token.EOF_TYPE);
			jmpl_AST = (AST)currentAST.root;
		}
		catch (RecognitionException ex) {
			reportError(ex);
			consume();
			consumeUntil(_tokenSet_0);
		}
		returnAST = jmpl_AST;
	}
	
	public final void attributeType() throws RecognitionException, TokenStreamException {
		
		returnAST = null;
		ASTPair currentAST = new ASTPair();
		AST attributeType_AST = null;
		Token  name = null;
		AST name_AST = null;
		AST values_AST = null;
		AST entries_AST = null;
		
		try {      // for error handling
			{
			switch ( LA(1)) {
			case ATTRIBUTE_TYPE_KEYWORD:
			{
				AST tmp2_AST = null;
				tmp2_AST = (AST)astFactory.create(LT(1));
				match(ATTRIBUTE_TYPE_KEYWORD);
				break;
			}
			case ATTRIBUTE_TYPE_KEYWORD_FLAT:
			{
				AST tmp3_AST = null;
				tmp3_AST = (AST)astFactory.create(LT(1));
				match(ATTRIBUTE_TYPE_KEYWORD_FLAT);
				break;
			}
			default:
			{
				throw new NoViableAltException(LT(1), getFilename());
			}
			}
			}
			name = LT(1);
			name_AST = (AST)astFactory.create(name);
			match(IDENT);
			AST tmp4_AST = null;
			tmp4_AST = (AST)astFactory.create(LT(1));
			match(LCURLY);
			attributeTypeValues();
			values_AST = (AST)returnAST;
			AST tmp5_AST = null;
			tmp5_AST = (AST)astFactory.create(LT(1));
			match(RCURLY);
			{
			switch ( LA(1)) {
			case LCURLY:
			{
				attributeTypeEntries();
				entries_AST = (AST)returnAST;
				break;
			}
			case SEMI:
			{
				break;
			}
			default:
			{
				throw new NoViableAltException(LT(1), getFilename());
			}
			}
			}
			AST tmp6_AST = null;
			tmp6_AST = (AST)astFactory.create(LT(1));
			match(SEMI);
			attributeType_AST = (AST)currentAST.root;
			attributeType_AST
			=(AST)astFactory.make( (new ASTArray(4)).add((AST)astFactory.create(ATTRIBUTE_TYPE,"ATTRIBUTE_TYPE")).add(name_AST).add(values_AST).add(entries_AST));
						updateSourceInfo((JmplASTNode)attributeType_AST,(JmplASTNode)name_AST);
			
			currentAST.root = attributeType_AST;
			currentAST.child = attributeType_AST!=null &&attributeType_AST.getFirstChild()!=null ?
				attributeType_AST.getFirstChild() : attributeType_AST;
			currentAST.advanceChildToEnd();
		}
		catch (RecognitionException ex) {
			reportError(ex);
			consume();
			consumeUntil(_tokenSet_1);
		}
		returnAST = attributeType_AST;
	}
	
	public final void componentType() throws RecognitionException, TokenStreamException {
		
		returnAST = null;
		ASTPair currentAST = new ASTPair();
		AST componentType_AST = null;
		Token  name = null;
		AST name_AST = null;
		AST ec_AST = null;
		AST entries_AST = null;
		
		try {      // for error handling
			{
			switch ( LA(1)) {
			case COMPONENT_TYPE_KEYWORD:
			{
				AST tmp7_AST = null;
				tmp7_AST = (AST)astFactory.create(LT(1));
				match(COMPONENT_TYPE_KEYWORD);
				break;
			}
			case COMPONENT_TYPE_KEYWORD_FLAT:
			{
				AST tmp8_AST = null;
				tmp8_AST = (AST)astFactory.create(LT(1));
				match(COMPONENT_TYPE_KEYWORD_FLAT);
				break;
			}
			default:
			{
				throw new NoViableAltException(LT(1), getFilename());
			}
			}
			}
			name = LT(1);
			name_AST = (AST)astFactory.create(name);
			match(IDENT);
			extensionClause();
			ec_AST = (AST)returnAST;
			AST tmp9_AST = null;
			tmp9_AST = (AST)astFactory.create(LT(1));
			match(LCURLY);
			componentTypeEntries();
			entries_AST = (AST)returnAST;
			AST tmp10_AST = null;
			tmp10_AST = (AST)astFactory.create(LT(1));
			match(RCURLY);
			componentType_AST = (AST)currentAST.root;
			componentType_AST
			=(AST)astFactory.make( (new ASTArray(4)).add((AST)astFactory.create(COMPONENT_TYPE,"COMPONENT_TYPE")).add(name_AST).add(ec_AST).add(entries_AST)); 
						updateSourceInfo((JmplASTNode)componentType_AST,(JmplASTNode)name_AST);
					
			currentAST.root = componentType_AST;
			currentAST.child = componentType_AST!=null &&componentType_AST.getFirstChild()!=null ?
				componentType_AST.getFirstChild() : componentType_AST;
			currentAST.advanceChildToEnd();
		}
		catch (RecognitionException ex) {
			reportError(ex);
			consume();
			consumeUntil(_tokenSet_1);
		}
		returnAST = componentType_AST;
	}
	
	public final void attributeTypeValues() throws RecognitionException, TokenStreamException {
		
		returnAST = null;
		ASTPair currentAST = new ASTPair();
		AST attributeTypeValues_AST = null;
		
		try {      // for error handling
			{
			switch ( LA(1)) {
			case IDENT:
			{
				attributeTypeValue();
				astFactory.addASTChild(currentAST, returnAST);
				{
				_loop10:
				do {
					if ((LA(1)==COMMA)) {
						AST tmp11_AST = null;
						tmp11_AST = (AST)astFactory.create(LT(1));
						match(COMMA);
						attributeTypeValue();
						astFactory.addASTChild(currentAST, returnAST);
					}
					else {
						break _loop10;
					}
					
				} while (true);
				}
				break;
			}
			case RCURLY:
			{
				break;
			}
			default:
			{
				throw new NoViableAltException(LT(1), getFilename());
			}
			}
			}
			attributeTypeValues_AST = (AST)currentAST.root;
		}
		catch (RecognitionException ex) {
			reportError(ex);
			consume();
			consumeUntil(_tokenSet_2);
		}
		returnAST = attributeTypeValues_AST;
	}
	
	public final void attributeTypeEntries() throws RecognitionException, TokenStreamException {
		
		returnAST = null;
		ASTPair currentAST = new ASTPair();
		AST attributeTypeEntries_AST = null;
		
		try {      // for error handling
			AST tmp12_AST = null;
			tmp12_AST = (AST)astFactory.create(LT(1));
			match(LCURLY);
			{
			_loop15:
			do {
				if ((LA(1)==RELATION_KEYWORD||LA(1)==LCURLY)) {
					attributeTypeEntry();
					astFactory.addASTChild(currentAST, returnAST);
				}
				else {
					break _loop15;
				}
				
			} while (true);
			}
			AST tmp13_AST = null;
			tmp13_AST = (AST)astFactory.create(LT(1));
			match(RCURLY);
			attributeTypeEntries_AST = (AST)currentAST.root;
		}
		catch (RecognitionException ex) {
			reportError(ex);
			consume();
			consumeUntil(_tokenSet_3);
		}
		returnAST = attributeTypeEntries_AST;
	}
	
	public final void attributeTypeValue() throws RecognitionException, TokenStreamException {
		
		returnAST = null;
		ASTPair currentAST = new ASTPair();
		AST attributeTypeValue_AST = null;
		Token  valueName = null;
		AST valueName_AST = null;
		Token  valueString = null;
		AST valueString_AST = null;
		
		try {      // for error handling
			valueName = LT(1);
			valueName_AST = (AST)astFactory.create(valueName);
			match(IDENT);
			{
			switch ( LA(1)) {
			case EQUALS:
			{
				AST tmp14_AST = null;
				tmp14_AST = (AST)astFactory.create(LT(1));
				match(EQUALS);
				valueString = LT(1);
				valueString_AST = (AST)astFactory.create(valueString);
				match(STRING_LITERAL);
				break;
			}
			case RCURLY:
			case COMMA:
			{
				break;
			}
			default:
			{
				throw new NoViableAltException(LT(1), getFilename());
			}
			}
			}
			attributeTypeValue_AST = (AST)currentAST.root;
			attributeTypeValue_AST
			=(AST)astFactory.make( (new ASTArray(3)).add((AST)astFactory.create(ATTRIBUTE_TYPE_VALUE,"ATTRIBUTE_TYPE_VALUE")).add(valueName_AST).add(valueString_AST)); 
						updateSourceInfo((JmplASTNode)attributeTypeValue_AST,(JmplASTNode)valueName_AST);
			
			currentAST.root = attributeTypeValue_AST;
			currentAST.child = attributeTypeValue_AST!=null &&attributeTypeValue_AST.getFirstChild()!=null ?
				attributeTypeValue_AST.getFirstChild() : attributeTypeValue_AST;
			currentAST.advanceChildToEnd();
		}
		catch (RecognitionException ex) {
			reportError(ex);
			consume();
			consumeUntil(_tokenSet_4);
		}
		returnAST = attributeTypeValue_AST;
	}
	
	public final void attributeTypeEntry() throws RecognitionException, TokenStreamException {
		
		returnAST = null;
		ASTPair currentAST = new ASTPair();
		AST attributeTypeEntry_AST = null;
		
		try {      // for error handling
			switch ( LA(1)) {
			case RELATION_KEYWORD:
			{
				relation();
				astFactory.addASTChild(currentAST, returnAST);
				attributeTypeEntry_AST = (AST)currentAST.root;
				break;
			}
			case LCURLY:
			{
				statementBlock();
				astFactory.addASTChild(currentAST, returnAST);
				attributeTypeEntry_AST = (AST)currentAST.root;
				break;
			}
			default:
			{
				throw new NoViableAltException(LT(1), getFilename());
			}
			}
		}
		catch (RecognitionException ex) {
			reportError(ex);
			consume();
			consumeUntil(_tokenSet_5);
		}
		returnAST = attributeTypeEntry_AST;
	}
	
	public final void relation() throws RecognitionException, TokenStreamException {
		
		returnAST = null;
		ASTPair currentAST = new ASTPair();
		AST relation_AST = null;
		Token  name = null;
		AST name_AST = null;
		AST args_AST = null;
		AST s_AST = null;
		
		try {      // for error handling
			AST tmp15_AST = null;
			tmp15_AST = (AST)astFactory.create(LT(1));
			match(RELATION_KEYWORD);
			name = LT(1);
			name_AST = (AST)astFactory.create(name);
			match(IDENT);
			AST tmp16_AST = null;
			tmp16_AST = (AST)astFactory.create(LT(1));
			match(LPAREN);
			arguments();
			args_AST = (AST)returnAST;
			AST tmp17_AST = null;
			tmp17_AST = (AST)astFactory.create(LT(1));
			match(RPAREN);
			statementBlock();
			s_AST = (AST)returnAST;
			relation_AST = (AST)currentAST.root;
			relation_AST
			=(AST)astFactory.make( (new ASTArray(4)).add((AST)astFactory.create(RELATION,"RELATION")).add(name_AST).add(args_AST).add(s_AST));
						updateSourceInfo((JmplASTNode)relation_AST,(JmplASTNode)name_AST);
			
			currentAST.root = relation_AST;
			currentAST.child = relation_AST!=null &&relation_AST.getFirstChild()!=null ?
				relation_AST.getFirstChild() : relation_AST;
			currentAST.advanceChildToEnd();
		}
		catch (RecognitionException ex) {
			reportError(ex);
			consume();
			consumeUntil(_tokenSet_6);
		}
		returnAST = relation_AST;
	}
	
	public final void statementBlock() throws RecognitionException, TokenStreamException {
		
		returnAST = null;
		ASTPair currentAST = new ASTPair();
		AST statementBlock_AST = null;
		Token  start = null;
		AST start_AST = null;
		
		try {      // for error handling
			start = LT(1);
			start_AST = (AST)astFactory.create(start);
			match(LCURLY);
			{
			_loop91:
			do {
				if ((_tokenSet_7.member(LA(1)))) {
					statement();
					astFactory.addASTChild(currentAST, returnAST);
				}
				else {
					break _loop91;
				}
				
			} while (true);
			}
			AST tmp18_AST = null;
			tmp18_AST = (AST)astFactory.create(LT(1));
			match(RCURLY);
			statementBlock_AST = (AST)currentAST.root;
			statementBlock_AST
			=(AST)astFactory.make( (new ASTArray(2)).add((AST)astFactory.create(STATEMENT_LIST,"STATEMENT_LIST")).add(statementBlock_AST));
						updateSourceInfo((JmplASTNode)statementBlock_AST,(JmplASTNode)start_AST);
			
			currentAST.root = statementBlock_AST;
			currentAST.child = statementBlock_AST!=null &&statementBlock_AST.getFirstChild()!=null ?
				statementBlock_AST.getFirstChild() : statementBlock_AST;
			currentAST.advanceChildToEnd();
			statementBlock_AST = (AST)currentAST.root;
		}
		catch (RecognitionException ex) {
			reportError(ex);
			consume();
			consumeUntil(_tokenSet_8);
		}
		returnAST = statementBlock_AST;
	}
	
	public final void extensionClause() throws RecognitionException, TokenStreamException {
		
		returnAST = null;
		ASTPair currentAST = new ASTPair();
		AST extensionClause_AST = null;
		AST componentTypeName_AST = null;
		
		try {      // for error handling
			{
			switch ( LA(1)) {
			case EXTENSION_KEYWORD:
			{
				AST tmp19_AST = null;
				tmp19_AST = (AST)astFactory.create(LT(1));
				match(EXTENSION_KEYWORD);
				identifier();
				componentTypeName_AST = (AST)returnAST;
				extensionClause_AST = (AST)currentAST.root;
				extensionClause_AST
				=(AST)astFactory.make( (new ASTArray(2)).add((AST)astFactory.create(EXTENSION_CLAUSE,"EXTENSION_CLAUSE")).add(componentTypeName_AST)); 
							updateSourceInfo((JmplASTNode)extensionClause_AST,(JmplASTNode)componentTypeName_AST);
				
				currentAST.root = extensionClause_AST;
				currentAST.child = extensionClause_AST!=null &&extensionClause_AST.getFirstChild()!=null ?
					extensionClause_AST.getFirstChild() : extensionClause_AST;
				currentAST.advanceChildToEnd();
				break;
			}
			case LCURLY:
			{
				break;
			}
			default:
			{
				throw new NoViableAltException(LT(1), getFilename());
			}
			}
			}
		}
		catch (RecognitionException ex) {
			reportError(ex);
			consume();
			consumeUntil(_tokenSet_9);
		}
		returnAST = extensionClause_AST;
	}
	
	public final void componentTypeEntries() throws RecognitionException, TokenStreamException {
		
		returnAST = null;
		ASTPair currentAST = new ASTPair();
		AST componentTypeEntries_AST = null;
		
		try {      // for error handling
			{
			_loop23:
			do {
				if ((_tokenSet_10.member(LA(1)))) {
					componentTypeEntry();
					astFactory.addASTChild(currentAST, returnAST);
				}
				else {
					break _loop23;
				}
				
			} while (true);
			}
			componentTypeEntries_AST = (AST)currentAST.root;
		}
		catch (RecognitionException ex) {
			reportError(ex);
			consume();
			consumeUntil(_tokenSet_2);
		}
		returnAST = componentTypeEntries_AST;
	}
	
	public final void identifier() throws RecognitionException, TokenStreamException {
		
		returnAST = null;
		ASTPair currentAST = new ASTPair();
		AST identifier_AST = null;
		Token  d = null;
		AST d_AST = null;
		
		try {      // for error handling
			AST tmp20_AST = null;
			tmp20_AST = (AST)astFactory.create(LT(1));
			astFactory.addASTChild(currentAST, tmp20_AST);
			match(IDENT);
			{
			_loop47:
			do {
				if ((LA(1)==DOT)) {
					d = LT(1);
					d_AST = (AST)astFactory.create(d);
					astFactory.makeASTRoot(currentAST, d_AST);
					match(DOT);
					AST tmp21_AST = null;
					tmp21_AST = (AST)astFactory.create(LT(1));
					astFactory.addASTChild(currentAST, tmp21_AST);
					match(IDENT);
					d_AST.setType(QUALIFIED_NAME);
					d_AST.setText("QUALIFIED_NAME");
				}
				else {
					break _loop47;
				}
				
			} while (true);
			}
			identifier_AST = (AST)currentAST.root;
		}
		catch (RecognitionException ex) {
			reportError(ex);
			consume();
			consumeUntil(_tokenSet_11);
		}
		returnAST = identifier_AST;
	}
	
	public final void componentTypeEntry() throws RecognitionException, TokenStreamException {
		
		returnAST = null;
		ASTPair currentAST = new ASTPair();
		AST componentTypeEntry_AST = null;
		
		try {      // for error handling
			switch ( LA(1)) {
			case PRIVATE_KEYWORD:
			case PUBLIC_KEYWORD:
			case ATTRIBUTE_TYPE_KEYWORD:
			case ATTRIBUTE_TYPE_KEYWORD_FLAT:
			case IDENT:
			{
				accessControlledComponentTypeEntry();
				astFactory.addASTChild(currentAST, returnAST);
				componentTypeEntry_AST = (AST)currentAST.root;
				break;
			}
			case STATE_VECTOR_KEYWORD:
			case STATE_VECTOR_KEYWORD_FLAT:
			{
				stateVector();
				astFactory.addASTChild(currentAST, returnAST);
				componentTypeEntry_AST = (AST)currentAST.root;
				break;
			}
			case NOMINAL_TRANSITION_KEYWORD:
			{
				transition();
				astFactory.addASTChild(currentAST, returnAST);
				componentTypeEntry_AST = (AST)currentAST.root;
				break;
			}
			case FAILURE_TRANSITION_KEYWORD:
			{
				failure();
				astFactory.addASTChild(currentAST, returnAST);
				componentTypeEntry_AST = (AST)currentAST.root;
				break;
			}
			case RELATION_KEYWORD:
			{
				relation();
				astFactory.addASTChild(currentAST, returnAST);
				componentTypeEntry_AST = (AST)currentAST.root;
				break;
			}
			case LCURLY:
			{
				statementBlock();
				astFactory.addASTChild(currentAST, returnAST);
				componentTypeEntry_AST = (AST)currentAST.root;
				break;
			}
			case SEMI:
			{
				AST tmp22_AST = null;
				tmp22_AST = (AST)astFactory.create(LT(1));
				match(SEMI);
				componentTypeEntry_AST = (AST)currentAST.root;
				break;
			}
			default:
			{
				throw new NoViableAltException(LT(1), getFilename());
			}
			}
		}
		catch (RecognitionException ex) {
			reportError(ex);
			consume();
			consumeUntil(_tokenSet_6);
		}
		returnAST = componentTypeEntry_AST;
	}
	
	public final void accessControlledComponentTypeEntry() throws RecognitionException, TokenStreamException {
		
		returnAST = null;
		ASTPair currentAST = new ASTPair();
		AST accessControlledComponentTypeEntry_AST = null;
		AST mod_AST = null;
		
		try {      // for error handling
			{
			switch ( LA(1)) {
			case PRIVATE_KEYWORD:
			case PUBLIC_KEYWORD:
			{
				modifier();
				mod_AST = (AST)returnAST;
				break;
			}
			case ATTRIBUTE_TYPE_KEYWORD:
			case ATTRIBUTE_TYPE_KEYWORD_FLAT:
			case IDENT:
			{
				break;
			}
			default:
			{
				throw new NoViableAltException(LT(1), getFilename());
			}
			}
			}
			{
			switch ( LA(1)) {
			case IDENT:
			{
				componentTypeVariables(mod_AST);
				astFactory.addASTChild(currentAST, returnAST);
				break;
			}
			case ATTRIBUTE_TYPE_KEYWORD:
			case ATTRIBUTE_TYPE_KEYWORD_FLAT:
			{
				componentTypeAttributeType(mod_AST);
				astFactory.addASTChild(currentAST, returnAST);
				break;
			}
			default:
			{
				throw new NoViableAltException(LT(1), getFilename());
			}
			}
			}
			accessControlledComponentTypeEntry_AST = (AST)currentAST.root;
		}
		catch (RecognitionException ex) {
			reportError(ex);
			consume();
			consumeUntil(_tokenSet_6);
		}
		returnAST = accessControlledComponentTypeEntry_AST;
	}
	
	public final void stateVector() throws RecognitionException, TokenStreamException {
		
		returnAST = null;
		ASTPair currentAST = new ASTPair();
		AST stateVector_AST = null;
		AST enumeration_AST = null;
		
		try {      // for error handling
			{
			switch ( LA(1)) {
			case STATE_VECTOR_KEYWORD:
			{
				AST tmp23_AST = null;
				tmp23_AST = (AST)astFactory.create(LT(1));
				match(STATE_VECTOR_KEYWORD);
				break;
			}
			case STATE_VECTOR_KEYWORD_FLAT:
			{
				AST tmp24_AST = null;
				tmp24_AST = (AST)astFactory.create(LT(1));
				match(STATE_VECTOR_KEYWORD_FLAT);
				break;
			}
			default:
			{
				throw new NoViableAltException(LT(1), getFilename());
			}
			}
			}
			stateVectorEnumeration();
			enumeration_AST = (AST)returnAST;
			AST tmp25_AST = null;
			tmp25_AST = (AST)astFactory.create(LT(1));
			match(SEMI);
			stateVector_AST = (AST)currentAST.root;
			stateVector_AST=(AST)astFactory.make( (new ASTArray(2)).add((AST)astFactory.create(STATE_VECTOR,"STATE_VECTOR")).add(enumeration_AST));
						updateSourceInfo((JmplASTNode)stateVector_AST,(JmplASTNode)enumeration_AST);
			
			currentAST.root = stateVector_AST;
			currentAST.child = stateVector_AST!=null &&stateVector_AST.getFirstChild()!=null ?
				stateVector_AST.getFirstChild() : stateVector_AST;
			currentAST.advanceChildToEnd();
		}
		catch (RecognitionException ex) {
			reportError(ex);
			consume();
			consumeUntil(_tokenSet_6);
		}
		returnAST = stateVector_AST;
	}
	
	public final void transition() throws RecognitionException, TokenStreamException {
		
		returnAST = null;
		ASTPair currentAST = new ASTPair();
		AST transition_AST = null;
		Token  start = null;
		AST start_AST = null;
		Token  name = null;
		AST name_AST = null;
		AST fv_AST = null;
		AST tv_AST = null;
		AST tc_AST = null;
		AST s_AST = null;
		
		try {      // for error handling
			start = LT(1);
			start_AST = (AST)astFactory.create(start);
			match(NOMINAL_TRANSITION_KEYWORD);
			{
			switch ( LA(1)) {
			case IDENT:
			{
				name = LT(1);
				name_AST = (AST)astFactory.create(name);
				match(IDENT);
				break;
			}
			case LPAREN:
			{
				break;
			}
			default:
			{
				throw new NoViableAltException(LT(1), getFilename());
			}
			}
			}
			AST tmp26_AST = null;
			tmp26_AST = (AST)astFactory.create(LT(1));
			match(LPAREN);
			{
			if ((_tokenSet_12.member(LA(1))) && (_tokenSet_13.member(LA(2)))) {
				fromVector();
				fv_AST = (AST)returnAST;
				{
				switch ( LA(1)) {
				case COMMA:
				{
					AST tmp27_AST = null;
					tmp27_AST = (AST)astFactory.create(LT(1));
					match(COMMA);
					toVector();
					tv_AST = (AST)returnAST;
					{
					switch ( LA(1)) {
					case COMMA:
					{
						AST tmp28_AST = null;
						tmp28_AST = (AST)astFactory.create(LT(1));
						match(COMMA);
						{
						switch ( LA(1)) {
						case IDENT:
						case STRING_LITERAL:
						case NUM_INT:
						case CHAR_LITERAL:
						case NUM_FLOAT:
						{
							transitionCost();
							tc_AST = (AST)returnAST;
							break;
						}
						case RPAREN:
						{
							break;
						}
						default:
						{
							throw new NoViableAltException(LT(1), getFilename());
						}
						}
						}
						break;
					}
					case RPAREN:
					{
						break;
					}
					default:
					{
						throw new NoViableAltException(LT(1), getFilename());
					}
					}
					}
					break;
				}
				case RPAREN:
				{
					break;
				}
				default:
				{
					throw new NoViableAltException(LT(1), getFilename());
				}
				}
				}
			}
			else if ((LA(1)==RPAREN) && (LA(2)==LCURLY)) {
			}
			else {
				throw new NoViableAltException(LT(1), getFilename());
			}
			
			}
			AST tmp29_AST = null;
			tmp29_AST = (AST)astFactory.create(LT(1));
			match(RPAREN);
			statementBlock();
			s_AST = (AST)returnAST;
			transition_AST = (AST)currentAST.root;
			transition_AST
			=(AST)astFactory.make( (new ASTArray(6)).add((AST)astFactory.create(NOMINAL_TRANSITION,"NOMINAL_TRANSITION")).add(name_AST).add(fv_AST).add(tv_AST).add(tc_AST).add(s_AST));
						updateSourceInfo((JmplASTNode)transition_AST,(JmplASTNode)start_AST);
			
			currentAST.root = transition_AST;
			currentAST.child = transition_AST!=null &&transition_AST.getFirstChild()!=null ?
				transition_AST.getFirstChild() : transition_AST;
			currentAST.advanceChildToEnd();
		}
		catch (RecognitionException ex) {
			reportError(ex);
			consume();
			consumeUntil(_tokenSet_6);
		}
		returnAST = transition_AST;
	}
	
	public final void failure() throws RecognitionException, TokenStreamException {
		
		returnAST = null;
		ASTPair currentAST = new ASTPair();
		AST failure_AST = null;
		Token  start = null;
		AST start_AST = null;
		Token  name = null;
		AST name_AST = null;
		AST fv_AST = null;
		AST tv_AST = null;
		AST fp_AST = null;
		AST s_AST = null;
		
		try {      // for error handling
			start = LT(1);
			start_AST = (AST)astFactory.create(start);
			match(FAILURE_TRANSITION_KEYWORD);
			{
			switch ( LA(1)) {
			case IDENT:
			{
				name = LT(1);
				name_AST = (AST)astFactory.create(name);
				match(IDENT);
				break;
			}
			case LPAREN:
			{
				break;
			}
			default:
			{
				throw new NoViableAltException(LT(1), getFilename());
			}
			}
			}
			AST tmp30_AST = null;
			tmp30_AST = (AST)astFactory.create(LT(1));
			match(LPAREN);
			{
			if ((_tokenSet_12.member(LA(1))) && (_tokenSet_13.member(LA(2)))) {
				fromVector();
				fv_AST = (AST)returnAST;
				{
				switch ( LA(1)) {
				case COMMA:
				{
					AST tmp31_AST = null;
					tmp31_AST = (AST)astFactory.create(LT(1));
					match(COMMA);
					toVector();
					tv_AST = (AST)returnAST;
					{
					switch ( LA(1)) {
					case COMMA:
					{
						AST tmp32_AST = null;
						tmp32_AST = (AST)astFactory.create(LT(1));
						match(COMMA);
						{
						switch ( LA(1)) {
						case IDENT:
						case STRING_LITERAL:
						case NUM_INT:
						case CHAR_LITERAL:
						case NUM_FLOAT:
						{
							failureProbability();
							fp_AST = (AST)returnAST;
							break;
						}
						case RPAREN:
						{
							break;
						}
						default:
						{
							throw new NoViableAltException(LT(1), getFilename());
						}
						}
						}
						break;
					}
					case RPAREN:
					{
						break;
					}
					default:
					{
						throw new NoViableAltException(LT(1), getFilename());
					}
					}
					}
					break;
				}
				case RPAREN:
				{
					break;
				}
				default:
				{
					throw new NoViableAltException(LT(1), getFilename());
				}
				}
				}
			}
			else if ((LA(1)==RPAREN) && (LA(2)==LCURLY)) {
			}
			else {
				throw new NoViableAltException(LT(1), getFilename());
			}
			
			}
			AST tmp33_AST = null;
			tmp33_AST = (AST)astFactory.create(LT(1));
			match(RPAREN);
			statementBlock();
			s_AST = (AST)returnAST;
			failure_AST = (AST)currentAST.root;
			failure_AST
			=(AST)astFactory.make( (new ASTArray(6)).add((AST)astFactory.create(FAILURE_TRANSITION,"FAILURE_TRANSITION")).add(name_AST).add(fv_AST).add(tv_AST).add(fp_AST).add(s_AST));
						updateSourceInfo((JmplASTNode)failure_AST,(JmplASTNode)start_AST);
			
			currentAST.root = failure_AST;
			currentAST.child = failure_AST!=null &&failure_AST.getFirstChild()!=null ?
				failure_AST.getFirstChild() : failure_AST;
			currentAST.advanceChildToEnd();
		}
		catch (RecognitionException ex) {
			reportError(ex);
			consume();
			consumeUntil(_tokenSet_6);
		}
		returnAST = failure_AST;
	}
	
	public final void modifier() throws RecognitionException, TokenStreamException {
		
		returnAST = null;
		ASTPair currentAST = new ASTPair();
		AST modifier_AST = null;
		
		try {      // for error handling
			{
			switch ( LA(1)) {
			case PRIVATE_KEYWORD:
			{
				AST tmp34_AST = null;
				tmp34_AST = (AST)astFactory.create(LT(1));
				astFactory.addASTChild(currentAST, tmp34_AST);
				match(PRIVATE_KEYWORD);
				break;
			}
			case PUBLIC_KEYWORD:
			{
				AST tmp35_AST = null;
				tmp35_AST = (AST)astFactory.create(LT(1));
				astFactory.addASTChild(currentAST, tmp35_AST);
				match(PUBLIC_KEYWORD);
				break;
			}
			default:
			{
				throw new NoViableAltException(LT(1), getFilename());
			}
			}
			}
			modifier_AST = (AST)currentAST.root;
			modifier_AST=(AST)astFactory.make( (new ASTArray(2)).add((AST)astFactory.create(MODIFIERS,"MODIFIERS")).add(modifier_AST));
			currentAST.root = modifier_AST;
			currentAST.child = modifier_AST!=null &&modifier_AST.getFirstChild()!=null ?
				modifier_AST.getFirstChild() : modifier_AST;
			currentAST.advanceChildToEnd();
			modifier_AST = (AST)currentAST.root;
		}
		catch (RecognitionException ex) {
			reportError(ex);
			consume();
			consumeUntil(_tokenSet_14);
		}
		returnAST = modifier_AST;
	}
	
	public final void componentTypeVariables(
		AST mod
	) throws RecognitionException, TokenStreamException {
		
		returnAST = null;
		ASTPair currentAST = new ASTPair();
		AST componentTypeVariables_AST = null;
		AST type_AST = null;
		AST vars_AST = null;
		
		try {      // for error handling
			identifier();
			type_AST = (AST)returnAST;
			variablesAndInitializers(mod,type_AST);
			vars_AST = (AST)returnAST;
			AST tmp36_AST = null;
			tmp36_AST = (AST)astFactory.create(LT(1));
			match(SEMI);
			componentTypeVariables_AST = (AST)currentAST.root;
			componentTypeVariables_AST=vars_AST;
						updateSourceInfo((JmplASTNode)componentTypeVariables_AST,(JmplASTNode)vars_AST);
			
			currentAST.root = componentTypeVariables_AST;
			currentAST.child = componentTypeVariables_AST!=null &&componentTypeVariables_AST.getFirstChild()!=null ?
				componentTypeVariables_AST.getFirstChild() : componentTypeVariables_AST;
			currentAST.advanceChildToEnd();
		}
		catch (RecognitionException ex) {
			reportError(ex);
			consume();
			consumeUntil(_tokenSet_6);
		}
		returnAST = componentTypeVariables_AST;
	}
	
	public final void componentTypeAttributeType(
		AST mod
	) throws RecognitionException, TokenStreamException {
		
		returnAST = null;
		ASTPair currentAST = new ASTPair();
		AST componentTypeAttributeType_AST = null;
		Token  name = null;
		AST name_AST = null;
		AST values_AST = null;
		AST vars_AST = null;
		AST entries_AST = null;
		
		try {      // for error handling
			{
			switch ( LA(1)) {
			case ATTRIBUTE_TYPE_KEYWORD:
			{
				AST tmp37_AST = null;
				tmp37_AST = (AST)astFactory.create(LT(1));
				match(ATTRIBUTE_TYPE_KEYWORD);
				break;
			}
			case ATTRIBUTE_TYPE_KEYWORD_FLAT:
			{
				AST tmp38_AST = null;
				tmp38_AST = (AST)astFactory.create(LT(1));
				match(ATTRIBUTE_TYPE_KEYWORD_FLAT);
				break;
			}
			default:
			{
				throw new NoViableAltException(LT(1), getFilename());
			}
			}
			}
			{
			switch ( LA(1)) {
			case IDENT:
			{
				name = LT(1);
				name_AST = (AST)astFactory.create(name);
				match(IDENT);
				break;
			}
			case LCURLY:
			{
				break;
			}
			default:
			{
				throw new NoViableAltException(LT(1), getFilename());
			}
			}
			}
			AST tmp39_AST = null;
			tmp39_AST = (AST)astFactory.create(LT(1));
			match(LCURLY);
			attributeTypeValues();
			values_AST = (AST)returnAST;
			AST tmp40_AST = null;
			tmp40_AST = (AST)astFactory.create(LT(1));
			match(RCURLY);
			{
			switch ( LA(1)) {
			case IDENT:
			{
				componentTypeAttributeTypeVariables();
				vars_AST = (AST)returnAST;
				break;
			}
			case LCURLY:
			case SEMI:
			{
				break;
			}
			default:
			{
				throw new NoViableAltException(LT(1), getFilename());
			}
			}
			}
			{
			switch ( LA(1)) {
			case LCURLY:
			{
				attributeTypeEntries();
				entries_AST = (AST)returnAST;
				break;
			}
			case SEMI:
			{
				break;
			}
			default:
			{
				throw new NoViableAltException(LT(1), getFilename());
			}
			}
			}
			AST tmp41_AST = null;
			tmp41_AST = (AST)astFactory.create(LT(1));
			match(SEMI);
			componentTypeAttributeType_AST = (AST)currentAST.root;
			componentTypeAttributeType_AST
			=(AST)astFactory.make( (new ASTArray(6)).add((AST)astFactory.create(CT_ATTRIBUTE_TYPE,"CT_ATTRIBUTE_TYPE")).add(name_AST).add(mod).add(values_AST).add(entries_AST).add(vars_AST));
						updateSourceInfo((JmplASTNode)componentTypeAttributeType_AST,(JmplASTNode)name_AST);
			
			currentAST.root = componentTypeAttributeType_AST;
			currentAST.child = componentTypeAttributeType_AST!=null &&componentTypeAttributeType_AST.getFirstChild()!=null ?
				componentTypeAttributeType_AST.getFirstChild() : componentTypeAttributeType_AST;
			currentAST.advanceChildToEnd();
		}
		catch (RecognitionException ex) {
			reportError(ex);
			consume();
			consumeUntil(_tokenSet_6);
		}
		returnAST = componentTypeAttributeType_AST;
	}
	
	public final void variablesAndInitializers(
		AST modifier, AST type
	) throws RecognitionException, TokenStreamException {
		
		returnAST = null;
		ASTPair currentAST = new ASTPair();
		AST variablesAndInitializers_AST = null;
		
		try {      // for error handling
			copyVariable(modifier,type);
			astFactory.addASTChild(currentAST, returnAST);
			{
			_loop41:
			do {
				if ((LA(1)==COMMA)) {
					AST tmp42_AST = null;
					tmp42_AST = (AST)astFactory.create(LT(1));
					match(COMMA);
					copyVariable(modifier,type);
					astFactory.addASTChild(currentAST, returnAST);
				}
				else {
					break _loop41;
				}
				
			} while (true);
			}
			variablesAndInitializers_AST = (AST)currentAST.root;
		}
		catch (RecognitionException ex) {
			reportError(ex);
			consume();
			consumeUntil(_tokenSet_3);
		}
		returnAST = variablesAndInitializers_AST;
	}
	
	public final void componentTypeAttributeTypeVariables() throws RecognitionException, TokenStreamException {
		
		returnAST = null;
		ASTPair currentAST = new ASTPair();
		AST componentTypeAttributeTypeVariables_AST = null;
		
		try {      // for error handling
			componentTypeAttributeTypeVariable();
			astFactory.addASTChild(currentAST, returnAST);
			{
			_loop36:
			do {
				if ((LA(1)==COMMA)) {
					AST tmp43_AST = null;
					tmp43_AST = (AST)astFactory.create(LT(1));
					match(COMMA);
					componentTypeAttributeTypeVariable();
					astFactory.addASTChild(currentAST, returnAST);
				}
				else {
					break _loop36;
				}
				
			} while (true);
			}
			componentTypeAttributeTypeVariables_AST = (AST)currentAST.root;
		}
		catch (RecognitionException ex) {
			reportError(ex);
			consume();
			consumeUntil(_tokenSet_15);
		}
		returnAST = componentTypeAttributeTypeVariables_AST;
	}
	
	public final void componentTypeAttributeTypeVariable() throws RecognitionException, TokenStreamException {
		
		returnAST = null;
		ASTPair currentAST = new ASTPair();
		AST componentTypeAttributeTypeVariable_AST = null;
		Token  name = null;
		AST name_AST = null;
		AST value_AST = null;
		
		try {      // for error handling
			name = LT(1);
			name_AST = (AST)astFactory.create(name);
			match(IDENT);
			{
			switch ( LA(1)) {
			case EQUALS:
			{
				AST tmp44_AST = null;
				tmp44_AST = (AST)astFactory.create(LT(1));
				match(EQUALS);
				identifier();
				value_AST = (AST)returnAST;
				break;
			}
			case LCURLY:
			case SEMI:
			case COMMA:
			{
				break;
			}
			default:
			{
				throw new NoViableAltException(LT(1), getFilename());
			}
			}
			}
			componentTypeAttributeTypeVariable_AST = (AST)currentAST.root;
			componentTypeAttributeTypeVariable_AST
			=(AST)astFactory.make( (new ASTArray(3)).add((AST)astFactory.create(VARIABLE,"VARIABLE")).add(name_AST).add(value_AST));
						updateSourceInfo((JmplASTNode)componentTypeAttributeTypeVariable_AST,(JmplASTNode)name_AST);
			
			currentAST.root = componentTypeAttributeTypeVariable_AST;
			currentAST.child = componentTypeAttributeTypeVariable_AST!=null &&componentTypeAttributeTypeVariable_AST.getFirstChild()!=null ?
				componentTypeAttributeTypeVariable_AST.getFirstChild() : componentTypeAttributeTypeVariable_AST;
			currentAST.advanceChildToEnd();
		}
		catch (RecognitionException ex) {
			reportError(ex);
			consume();
			consumeUntil(_tokenSet_16);
		}
		returnAST = componentTypeAttributeTypeVariable_AST;
	}
	
	public final void copyVariable(
		AST modifier, AST type
	) throws RecognitionException, TokenStreamException {
		
		returnAST = null;
		ASTPair currentAST = new ASTPair();
		AST copyVariable_AST = null;
		
		try {      // for error handling
			variable(getASTFactory().dupTree(modifier),
             getASTFactory().dupTree(type));
			astFactory.addASTChild(currentAST, returnAST);
			copyVariable_AST = (AST)currentAST.root;
		}
		catch (RecognitionException ex) {
			reportError(ex);
			consume();
			consumeUntil(_tokenSet_17);
		}
		returnAST = copyVariable_AST;
	}
	
	public final void variable(
		AST modifier, AST type
	) throws RecognitionException, TokenStreamException {
		
		returnAST = null;
		ASTPair currentAST = new ASTPair();
		AST variable_AST = null;
		Token  name = null;
		AST name_AST = null;
		AST value_AST = null;
		
		try {      // for error handling
			name = LT(1);
			name_AST = (AST)astFactory.create(name);
			match(IDENT);
			{
			switch ( LA(1)) {
			case EQUALS:
			{
				AST tmp45_AST = null;
				tmp45_AST = (AST)astFactory.create(LT(1));
				match(EQUALS);
				identifier();
				value_AST = (AST)returnAST;
				break;
			}
			case SEMI:
			case COMMA:
			case RPAREN:
			{
				break;
			}
			default:
			{
				throw new NoViableAltException(LT(1), getFilename());
			}
			}
			}
			variable_AST = (AST)currentAST.root;
			variable_AST
			=(AST)astFactory.make( (new ASTArray(5)).add((AST)astFactory.create(VARIABLE,"VARIABLE")).add(name_AST).add(type).add(value_AST).add(modifier));
						updateSourceInfo((JmplASTNode)variable_AST,(JmplASTNode)name_AST);
			
			currentAST.root = variable_AST;
			currentAST.child = variable_AST!=null &&variable_AST.getFirstChild()!=null ?
				variable_AST.getFirstChild() : variable_AST;
			currentAST.advanceChildToEnd();
		}
		catch (RecognitionException ex) {
			reportError(ex);
			consume();
			consumeUntil(_tokenSet_18);
		}
		returnAST = variable_AST;
	}
	
	public final void stateVectorEnumeration() throws RecognitionException, TokenStreamException {
		
		returnAST = null;
		ASTPair currentAST = new ASTPair();
		AST stateVectorEnumeration_AST = null;
		
		try {      // for error handling
			switch ( LA(1)) {
			case IDENT:
			{
				AST tmp46_AST = null;
				tmp46_AST = (AST)astFactory.create(LT(1));
				astFactory.addASTChild(currentAST, tmp46_AST);
				match(IDENT);
				stateVectorEnumeration_AST = (AST)currentAST.root;
				break;
			}
			case LBRACK:
			{
				bigStateVectorEnumeration();
				astFactory.addASTChild(currentAST, returnAST);
				stateVectorEnumeration_AST = (AST)currentAST.root;
				break;
			}
			default:
			{
				throw new NoViableAltException(LT(1), getFilename());
			}
			}
		}
		catch (RecognitionException ex) {
			reportError(ex);
			consume();
			consumeUntil(_tokenSet_3);
		}
		returnAST = stateVectorEnumeration_AST;
	}
	
	public final void bigStateVectorEnumeration() throws RecognitionException, TokenStreamException {
		
		returnAST = null;
		ASTPair currentAST = new ASTPair();
		AST bigStateVectorEnumeration_AST = null;
		
		try {      // for error handling
			AST tmp47_AST = null;
			tmp47_AST = (AST)astFactory.create(LT(1));
			match(LBRACK);
			AST tmp48_AST = null;
			tmp48_AST = (AST)astFactory.create(LT(1));
			astFactory.addASTChild(currentAST, tmp48_AST);
			match(IDENT);
			{
			_loop55:
			do {
				if ((LA(1)==COMMA)) {
					AST tmp49_AST = null;
					tmp49_AST = (AST)astFactory.create(LT(1));
					match(COMMA);
					AST tmp50_AST = null;
					tmp50_AST = (AST)astFactory.create(LT(1));
					astFactory.addASTChild(currentAST, tmp50_AST);
					match(IDENT);
				}
				else {
					break _loop55;
				}
				
			} while (true);
			}
			AST tmp51_AST = null;
			tmp51_AST = (AST)astFactory.create(LT(1));
			match(RBRACK);
			bigStateVectorEnumeration_AST = (AST)currentAST.root;
		}
		catch (RecognitionException ex) {
			reportError(ex);
			consume();
			consumeUntil(_tokenSet_3);
		}
		returnAST = bigStateVectorEnumeration_AST;
	}
	
	public final void arguments() throws RecognitionException, TokenStreamException {
		
		returnAST = null;
		ASTPair currentAST = new ASTPair();
		AST arguments_AST = null;
		
		try {      // for error handling
			{
			switch ( LA(1)) {
			case IDENT:
			{
				argumentSpec();
				astFactory.addASTChild(currentAST, returnAST);
				{
				_loop60:
				do {
					if ((LA(1)==COMMA)) {
						AST tmp52_AST = null;
						tmp52_AST = (AST)astFactory.create(LT(1));
						match(COMMA);
						argumentSpec();
						astFactory.addASTChild(currentAST, returnAST);
					}
					else {
						break _loop60;
					}
					
				} while (true);
				}
				break;
			}
			case RPAREN:
			{
				break;
			}
			default:
			{
				throw new NoViableAltException(LT(1), getFilename());
			}
			}
			}
			arguments_AST = (AST)currentAST.root;
		}
		catch (RecognitionException ex) {
			reportError(ex);
			consume();
			consumeUntil(_tokenSet_19);
		}
		returnAST = arguments_AST;
	}
	
	public final void argumentSpec() throws RecognitionException, TokenStreamException {
		
		returnAST = null;
		ASTPair currentAST = new ASTPair();
		AST argumentSpec_AST = null;
		AST argumentType_AST = null;
		AST argumentVar_AST = null;
		
		try {      // for error handling
			identifier();
			argumentType_AST = (AST)returnAST;
			variable(null,argumentType_AST);
			argumentVar_AST = (AST)returnAST;
			argumentSpec_AST = (AST)currentAST.root;
			argumentSpec_AST=argumentVar_AST;
			currentAST.root = argumentSpec_AST;
			currentAST.child = argumentSpec_AST!=null &&argumentSpec_AST.getFirstChild()!=null ?
				argumentSpec_AST.getFirstChild() : argumentSpec_AST;
			currentAST.advanceChildToEnd();
		}
		catch (RecognitionException ex) {
			reportError(ex);
			consume();
			consumeUntil(_tokenSet_20);
		}
		returnAST = argumentSpec_AST;
	}
	
	public final void fromVector() throws RecognitionException, TokenStreamException {
		
		returnAST = null;
		ASTPair currentAST = new ASTPair();
		AST fromVector_AST = null;
		
		try {      // for error handling
			switch ( LA(1)) {
			case IDENT:
			case COMMA:
			case RPAREN:
			case STAR:
			{
				fromStateIdentifier();
				astFactory.addASTChild(currentAST, returnAST);
				fromVector_AST = (AST)currentAST.root;
				fromVector_AST = (AST)astFactory.make( (new ASTArray(2)).add((AST)astFactory.create(FROM_VECTOR,"FROM_VECTOR")).add(fromVector_AST));
				currentAST.root = fromVector_AST;
				currentAST.child = fromVector_AST!=null &&fromVector_AST.getFirstChild()!=null ?
					fromVector_AST.getFirstChild() : fromVector_AST;
				currentAST.advanceChildToEnd();
				fromVector_AST = (AST)currentAST.root;
				break;
			}
			case LBRACK:
			{
				{
				AST tmp53_AST = null;
				tmp53_AST = (AST)astFactory.create(LT(1));
				match(LBRACK);
				fromStateIdentifier();
				astFactory.addASTChild(currentAST, returnAST);
				{
				_loop79:
				do {
					if ((LA(1)==COMMA)) {
						AST tmp54_AST = null;
						tmp54_AST = (AST)astFactory.create(LT(1));
						match(COMMA);
						fromStateIdentifier();
						astFactory.addASTChild(currentAST, returnAST);
					}
					else {
						break _loop79;
					}
					
				} while (true);
				}
				AST tmp55_AST = null;
				tmp55_AST = (AST)astFactory.create(LT(1));
				match(RBRACK);
				}
				fromVector_AST = (AST)currentAST.root;
				fromVector_AST = (AST)astFactory.make( (new ASTArray(2)).add((AST)astFactory.create(FROM_VECTOR,"FROM_VECTOR")).add(fromVector_AST));
				currentAST.root = fromVector_AST;
				currentAST.child = fromVector_AST!=null &&fromVector_AST.getFirstChild()!=null ?
					fromVector_AST.getFirstChild() : fromVector_AST;
				currentAST.advanceChildToEnd();
				fromVector_AST = (AST)currentAST.root;
				break;
			}
			default:
			{
				throw new NoViableAltException(LT(1), getFilename());
			}
			}
		}
		catch (RecognitionException ex) {
			reportError(ex);
			consume();
			consumeUntil(_tokenSet_20);
		}
		returnAST = fromVector_AST;
	}
	
	public final void toVector() throws RecognitionException, TokenStreamException {
		
		returnAST = null;
		ASTPair currentAST = new ASTPair();
		AST toVector_AST = null;
		
		try {      // for error handling
			switch ( LA(1)) {
			case IDENT:
			case COMMA:
			case RPAREN:
			case PLUS:
			{
				toStateIdentifier();
				astFactory.addASTChild(currentAST, returnAST);
				toVector_AST = (AST)currentAST.root;
				toVector_AST = (AST)astFactory.make( (new ASTArray(2)).add((AST)astFactory.create(TO_VECTOR,"TO_VECTOR")).add(toVector_AST));
				currentAST.root = toVector_AST;
				currentAST.child = toVector_AST!=null &&toVector_AST.getFirstChild()!=null ?
					toVector_AST.getFirstChild() : toVector_AST;
				currentAST.advanceChildToEnd();
				toVector_AST = (AST)currentAST.root;
				break;
			}
			case LBRACK:
			{
				{
				AST tmp56_AST = null;
				tmp56_AST = (AST)astFactory.create(LT(1));
				match(LBRACK);
				toStateIdentifier();
				astFactory.addASTChild(currentAST, returnAST);
				{
				_loop85:
				do {
					if ((LA(1)==COMMA)) {
						AST tmp57_AST = null;
						tmp57_AST = (AST)astFactory.create(LT(1));
						match(COMMA);
						toStateIdentifier();
						astFactory.addASTChild(currentAST, returnAST);
					}
					else {
						break _loop85;
					}
					
				} while (true);
				}
				AST tmp58_AST = null;
				tmp58_AST = (AST)astFactory.create(LT(1));
				match(RBRACK);
				}
				toVector_AST = (AST)currentAST.root;
				toVector_AST = (AST)astFactory.make( (new ASTArray(2)).add((AST)astFactory.create(TO_VECTOR,"TO_VECTOR")).add(toVector_AST));
				currentAST.root = toVector_AST;
				currentAST.child = toVector_AST!=null &&toVector_AST.getFirstChild()!=null ?
					toVector_AST.getFirstChild() : toVector_AST;
				currentAST.advanceChildToEnd();
				toVector_AST = (AST)currentAST.root;
				break;
			}
			default:
			{
				throw new NoViableAltException(LT(1), getFilename());
			}
			}
		}
		catch (RecognitionException ex) {
			reportError(ex);
			consume();
			consumeUntil(_tokenSet_20);
		}
		returnAST = toVector_AST;
	}
	
	public final void transitionCost() throws RecognitionException, TokenStreamException {
		
		returnAST = null;
		ASTPair currentAST = new ASTPair();
		AST transitionCost_AST = null;
		AST i_AST = null;
		AST c_AST = null;
		
		try {      // for error handling
			switch ( LA(1)) {
			case IDENT:
			{
				identifier();
				i_AST = (AST)returnAST;
				astFactory.addASTChild(currentAST, returnAST);
				transitionCost_AST = (AST)currentAST.root;
				transitionCost_AST=(AST)astFactory.make( (new ASTArray(2)).add((AST)astFactory.create(TRANSITION_COST,"TRANSITION_COST")).add(i_AST));
				currentAST.root = transitionCost_AST;
				currentAST.child = transitionCost_AST!=null &&transitionCost_AST.getFirstChild()!=null ?
					transitionCost_AST.getFirstChild() : transitionCost_AST;
				currentAST.advanceChildToEnd();
				transitionCost_AST = (AST)currentAST.root;
				break;
			}
			case STRING_LITERAL:
			case NUM_INT:
			case CHAR_LITERAL:
			case NUM_FLOAT:
			{
				constant();
				c_AST = (AST)returnAST;
				astFactory.addASTChild(currentAST, returnAST);
				transitionCost_AST = (AST)currentAST.root;
				transitionCost_AST=(AST)astFactory.make( (new ASTArray(2)).add((AST)astFactory.create(TRANSITION_COST,"TRANSITION_COST")).add(c_AST));
				currentAST.root = transitionCost_AST;
				currentAST.child = transitionCost_AST!=null &&transitionCost_AST.getFirstChild()!=null ?
					transitionCost_AST.getFirstChild() : transitionCost_AST;
				currentAST.advanceChildToEnd();
				transitionCost_AST = (AST)currentAST.root;
				break;
			}
			default:
			{
				throw new NoViableAltException(LT(1), getFilename());
			}
			}
		}
		catch (RecognitionException ex) {
			reportError(ex);
			consume();
			consumeUntil(_tokenSet_19);
		}
		returnAST = transitionCost_AST;
	}
	
	public final void constant() throws RecognitionException, TokenStreamException {
		
		returnAST = null;
		ASTPair currentAST = new ASTPair();
		AST constant_AST = null;
		
		try {      // for error handling
			switch ( LA(1)) {
			case NUM_INT:
			{
				AST tmp59_AST = null;
				tmp59_AST = (AST)astFactory.create(LT(1));
				astFactory.addASTChild(currentAST, tmp59_AST);
				match(NUM_INT);
				constant_AST = (AST)currentAST.root;
				break;
			}
			case CHAR_LITERAL:
			{
				AST tmp60_AST = null;
				tmp60_AST = (AST)astFactory.create(LT(1));
				astFactory.addASTChild(currentAST, tmp60_AST);
				match(CHAR_LITERAL);
				constant_AST = (AST)currentAST.root;
				break;
			}
			case STRING_LITERAL:
			{
				AST tmp61_AST = null;
				tmp61_AST = (AST)astFactory.create(LT(1));
				astFactory.addASTChild(currentAST, tmp61_AST);
				match(STRING_LITERAL);
				constant_AST = (AST)currentAST.root;
				break;
			}
			case NUM_FLOAT:
			{
				AST tmp62_AST = null;
				tmp62_AST = (AST)astFactory.create(LT(1));
				astFactory.addASTChild(currentAST, tmp62_AST);
				match(NUM_FLOAT);
				constant_AST = (AST)currentAST.root;
				break;
			}
			default:
			{
				throw new NoViableAltException(LT(1), getFilename());
			}
			}
		}
		catch (RecognitionException ex) {
			reportError(ex);
			consume();
			consumeUntil(_tokenSet_19);
		}
		returnAST = constant_AST;
	}
	
	public final void failureProbability() throws RecognitionException, TokenStreamException {
		
		returnAST = null;
		ASTPair currentAST = new ASTPair();
		AST failureProbability_AST = null;
		AST i_AST = null;
		AST c_AST = null;
		
		try {      // for error handling
			switch ( LA(1)) {
			case IDENT:
			{
				identifier();
				i_AST = (AST)returnAST;
				astFactory.addASTChild(currentAST, returnAST);
				failureProbability_AST = (AST)currentAST.root;
				failureProbability_AST=(AST)astFactory.make( (new ASTArray(2)).add((AST)astFactory.create(FAILURE_PROBABILITY,"FAILURE_PROBABILITY")).add(i_AST));
				currentAST.root = failureProbability_AST;
				currentAST.child = failureProbability_AST!=null &&failureProbability_AST.getFirstChild()!=null ?
					failureProbability_AST.getFirstChild() : failureProbability_AST;
				currentAST.advanceChildToEnd();
				failureProbability_AST = (AST)currentAST.root;
				break;
			}
			case STRING_LITERAL:
			case NUM_INT:
			case CHAR_LITERAL:
			case NUM_FLOAT:
			{
				constant();
				c_AST = (AST)returnAST;
				astFactory.addASTChild(currentAST, returnAST);
				failureProbability_AST = (AST)currentAST.root;
				failureProbability_AST=(AST)astFactory.make( (new ASTArray(2)).add((AST)astFactory.create(FAILURE_PROBABILITY,"FAILURE_PROBABILITY")).add(c_AST));
				currentAST.root = failureProbability_AST;
				currentAST.child = failureProbability_AST!=null &&failureProbability_AST.getFirstChild()!=null ?
					failureProbability_AST.getFirstChild() : failureProbability_AST;
				currentAST.advanceChildToEnd();
				failureProbability_AST = (AST)currentAST.root;
				break;
			}
			default:
			{
				throw new NoViableAltException(LT(1), getFilename());
			}
			}
		}
		catch (RecognitionException ex) {
			reportError(ex);
			consume();
			consumeUntil(_tokenSet_19);
		}
		returnAST = failureProbability_AST;
	}
	
	public final void fromStateIdentifier() throws RecognitionException, TokenStreamException {
		
		returnAST = null;
		ASTPair currentAST = new ASTPair();
		AST fromStateIdentifier_AST = null;
		Token  s = null;
		AST s_AST = null;
		AST i_AST = null;
		
		try {      // for error handling
			{
			switch ( LA(1)) {
			case COMMA:
			case RBRACK:
			case RPAREN:
			{
				fromStateIdentifier_AST = (AST)currentAST.root;
				
				fromStateIdentifier_AST
				=(AST)astFactory.make( (new ASTArray(1)).add((AST)astFactory.create(NOTHING_FOR_STATE_VAR,".")));
				
				currentAST.root = fromStateIdentifier_AST;
				currentAST.child = fromStateIdentifier_AST!=null &&fromStateIdentifier_AST.getFirstChild()!=null ?
					fromStateIdentifier_AST.getFirstChild() : fromStateIdentifier_AST;
				currentAST.advanceChildToEnd();
				break;
			}
			case STAR:
			{
				s = LT(1);
				s_AST = (AST)astFactory.create(s);
				astFactory.addASTChild(currentAST, s_AST);
				match(STAR);
				fromStateIdentifier_AST = (AST)currentAST.root;
				fromStateIdentifier_AST=s_AST;
				currentAST.root = fromStateIdentifier_AST;
				currentAST.child = fromStateIdentifier_AST!=null &&fromStateIdentifier_AST.getFirstChild()!=null ?
					fromStateIdentifier_AST.getFirstChild() : fromStateIdentifier_AST;
				currentAST.advanceChildToEnd();
				break;
			}
			case IDENT:
			{
				identifier();
				i_AST = (AST)returnAST;
				astFactory.addASTChild(currentAST, returnAST);
				fromStateIdentifier_AST = (AST)currentAST.root;
				fromStateIdentifier_AST=i_AST;
				currentAST.root = fromStateIdentifier_AST;
				currentAST.child = fromStateIdentifier_AST!=null &&fromStateIdentifier_AST.getFirstChild()!=null ?
					fromStateIdentifier_AST.getFirstChild() : fromStateIdentifier_AST;
				currentAST.advanceChildToEnd();
				break;
			}
			default:
			{
				throw new NoViableAltException(LT(1), getFilename());
			}
			}
			}
			fromStateIdentifier_AST = (AST)currentAST.root;
		}
		catch (RecognitionException ex) {
			reportError(ex);
			consume();
			consumeUntil(_tokenSet_21);
		}
		returnAST = fromStateIdentifier_AST;
	}
	
	public final void toStateIdentifier() throws RecognitionException, TokenStreamException {
		
		returnAST = null;
		ASTPair currentAST = new ASTPair();
		AST toStateIdentifier_AST = null;
		Token  p = null;
		AST p_AST = null;
		AST i_AST = null;
		
		try {      // for error handling
			{
			switch ( LA(1)) {
			case COMMA:
			case RBRACK:
			case RPAREN:
			{
				toStateIdentifier_AST = (AST)currentAST.root;
				
				toStateIdentifier_AST
				=(AST)astFactory.make( (new ASTArray(1)).add((AST)astFactory.create(NOTHING_FOR_STATE_VAR,".")));
				
				currentAST.root = toStateIdentifier_AST;
				currentAST.child = toStateIdentifier_AST!=null &&toStateIdentifier_AST.getFirstChild()!=null ?
					toStateIdentifier_AST.getFirstChild() : toStateIdentifier_AST;
				currentAST.advanceChildToEnd();
				break;
			}
			case PLUS:
			{
				p = LT(1);
				p_AST = (AST)astFactory.create(p);
				match(PLUS);
				toStateIdentifier_AST = (AST)currentAST.root;
				toStateIdentifier_AST=p_AST;
				currentAST.root = toStateIdentifier_AST;
				currentAST.child = toStateIdentifier_AST!=null &&toStateIdentifier_AST.getFirstChild()!=null ?
					toStateIdentifier_AST.getFirstChild() : toStateIdentifier_AST;
				currentAST.advanceChildToEnd();
				break;
			}
			case IDENT:
			{
				identifier();
				i_AST = (AST)returnAST;
				toStateIdentifier_AST = (AST)currentAST.root;
				toStateIdentifier_AST=i_AST;
				currentAST.root = toStateIdentifier_AST;
				currentAST.child = toStateIdentifier_AST!=null &&toStateIdentifier_AST.getFirstChild()!=null ?
					toStateIdentifier_AST.getFirstChild() : toStateIdentifier_AST;
				currentAST.advanceChildToEnd();
				break;
			}
			default:
			{
				throw new NoViableAltException(LT(1), getFilename());
			}
			}
			}
		}
		catch (RecognitionException ex) {
			reportError(ex);
			consume();
			consumeUntil(_tokenSet_21);
		}
		returnAST = toStateIdentifier_AST;
	}
	
	public final void statement() throws RecognitionException, TokenStreamException {
		
		returnAST = null;
		ASTPair currentAST = new ASTPair();
		AST statement_AST = null;
		Token  ifk = null;
		AST ifk_AST = null;
		Token  iffk = null;
		AST iffk_AST = null;
		Token  switchk = null;
		AST switchk_AST = null;
		Token  s = null;
		AST s_AST = null;
		
		try {      // for error handling
			switch ( LA(1)) {
			case LCURLY:
			{
				statementBlock();
				astFactory.addASTChild(currentAST, returnAST);
				statement_AST = (AST)currentAST.root;
				break;
			}
			case IF_KEYWORD:
			{
				{
				ifk = LT(1);
				ifk_AST = (AST)astFactory.create(ifk);
				astFactory.makeASTRoot(currentAST, ifk_AST);
				match(IF_KEYWORD);
				ifk_AST.setType(IF_STATEMENT);
				ifk_AST.setText("IF_STATEMENT");
				AST tmp63_AST = null;
				tmp63_AST = (AST)astFactory.create(LT(1));
				match(LPAREN);
				expression();
				astFactory.addASTChild(currentAST, returnAST);
				AST tmp64_AST = null;
				tmp64_AST = (AST)astFactory.create(LT(1));
				match(RPAREN);
				statement();
				astFactory.addASTChild(currentAST, returnAST);
				{
				if ((LA(1)==ELSE_KEYWORD) && (_tokenSet_7.member(LA(2)))) {
					AST tmp65_AST = null;
					tmp65_AST = (AST)astFactory.create(LT(1));
					match(ELSE_KEYWORD);
					statement();
					astFactory.addASTChild(currentAST, returnAST);
				}
				else if ((_tokenSet_22.member(LA(1))) && (_tokenSet_23.member(LA(2)))) {
				}
				else {
					throw new NoViableAltException(LT(1), getFilename());
				}
				
				}
				}
				statement_AST = (AST)currentAST.root;
				break;
			}
			case IFF_KEYWORD:
			{
				{
				iffk = LT(1);
				iffk_AST = (AST)astFactory.create(iffk);
				astFactory.makeASTRoot(currentAST, iffk_AST);
				match(IFF_KEYWORD);
				iffk_AST.setType(IFF_STATEMENT);
				iffk_AST.setText("IFF_STATEMENT");
				AST tmp66_AST = null;
				tmp66_AST = (AST)astFactory.create(LT(1));
				match(LPAREN);
				expression();
				astFactory.addASTChild(currentAST, returnAST);
				AST tmp67_AST = null;
				tmp67_AST = (AST)astFactory.create(LT(1));
				match(RPAREN);
				statement();
				astFactory.addASTChild(currentAST, returnAST);
				}
				statement_AST = (AST)currentAST.root;
				break;
			}
			case SWITCH_KEYWORD:
			{
				{
				switchk = LT(1);
				switchk_AST = (AST)astFactory.create(switchk);
				astFactory.makeASTRoot(currentAST, switchk_AST);
				match(SWITCH_KEYWORD);
				switchk_AST.setType(SWITCH_STATEMENT);
				switchk_AST.setText("SWITCH_STATEMENT");
				AST tmp68_AST = null;
				tmp68_AST = (AST)astFactory.create(LT(1));
				match(LPAREN);
				{
				switch ( LA(1)) {
				case IDENT:
				{
					identifier();
					astFactory.addASTChild(currentAST, returnAST);
					break;
				}
				case CONTEXT_KEYWORD:
				{
					AST tmp69_AST = null;
					tmp69_AST = (AST)astFactory.create(LT(1));
					astFactory.addASTChild(currentAST, tmp69_AST);
					match(CONTEXT_KEYWORD);
					break;
				}
				default:
				{
					throw new NoViableAltException(LT(1), getFilename());
				}
				}
				}
				AST tmp70_AST = null;
				tmp70_AST = (AST)astFactory.create(LT(1));
				match(RPAREN);
				AST tmp71_AST = null;
				tmp71_AST = (AST)astFactory.create(LT(1));
				match(LCURLY);
				{
				_loop99:
				do {
					if ((LA(1)==CASE_KEYWORD)) {
						casesGroup();
						astFactory.addASTChild(currentAST, returnAST);
					}
					else {
						break _loop99;
					}
					
				} while (true);
				}
				AST tmp72_AST = null;
				tmp72_AST = (AST)astFactory.create(LT(1));
				match(RCURLY);
				}
				statement_AST = (AST)currentAST.root;
				break;
			}
			case SEMI:
			{
				{
				s = LT(1);
				s_AST = (AST)astFactory.create(s);
				astFactory.addASTChild(currentAST, s_AST);
				match(SEMI);
				s_AST.setType(EMPTY_STATEMENT); 
				s_AST.setText("EMPTY_STATEMENT");
				
				}
				statement_AST = (AST)currentAST.root;
				break;
			}
			case CONTEXT_KEYWORD:
			case IDENT:
			case LPAREN:
			case LNOT:
			case BANG:
			{
				{
				expression();
				astFactory.addASTChild(currentAST, returnAST);
				AST tmp73_AST = null;
				tmp73_AST = (AST)astFactory.create(LT(1));
				match(SEMI);
				}
				statement_AST = (AST)currentAST.root;
				break;
			}
			default:
			{
				throw new NoViableAltException(LT(1), getFilename());
			}
			}
		}
		catch (RecognitionException ex) {
			reportError(ex);
			consume();
			consumeUntil(_tokenSet_22);
		}
		returnAST = statement_AST;
	}
	
	public final void expression() throws RecognitionException, TokenStreamException {
		
		returnAST = null;
		ASTPair currentAST = new ASTPair();
		AST expression_AST = null;
		
		try {      // for error handling
			logicalOrExpression();
			astFactory.addASTChild(currentAST, returnAST);
			expression_AST = (AST)currentAST.root;
			
					 JmplASTNode save=(JmplASTNode)expression_AST;
					 expression_AST = (AST)astFactory.make( (new ASTArray(2)).add((AST)astFactory.create(EXPRESSION,"EXPRESSION")).add(expression_AST));
					 updateSourceInfo((JmplASTNode)expression_AST,save);
					
			currentAST.root = expression_AST;
			currentAST.child = expression_AST!=null &&expression_AST.getFirstChild()!=null ?
				expression_AST.getFirstChild() : expression_AST;
			currentAST.advanceChildToEnd();
			expression_AST = (AST)currentAST.root;
		}
		catch (RecognitionException ex) {
			reportError(ex);
			consume();
			consumeUntil(_tokenSet_24);
		}
		returnAST = expression_AST;
	}
	
	public final void casesGroup() throws RecognitionException, TokenStreamException {
		
		returnAST = null;
		ASTPair currentAST = new ASTPair();
		AST casesGroup_AST = null;
		
		try {      // for error handling
			{
			int _cnt105=0;
			_loop105:
			do {
				if ((LA(1)==CASE_KEYWORD) && (LA(2)==IDENT)) {
					{
					AST tmp74_AST = null;
					tmp74_AST = (AST)astFactory.create(LT(1));
					match(CASE_KEYWORD);
					identifier();
					astFactory.addASTChild(currentAST, returnAST);
					AST tmp75_AST = null;
					tmp75_AST = (AST)astFactory.create(LT(1));
					match(COLON);
					}
				}
				else {
					if ( _cnt105>=1 ) { break _loop105; } else {throw new NoViableAltException(LT(1), getFilename());}
				}
				
				_cnt105++;
			} while (true);
			}
			caseStatementList();
			astFactory.addASTChild(currentAST, returnAST);
			casesGroup_AST = (AST)currentAST.root;
			casesGroup_AST = (AST)astFactory.make( (new ASTArray(2)).add((AST)astFactory.create(CASE_GROUP,"CASE_GROUP")).add(casesGroup_AST));
					
			currentAST.root = casesGroup_AST;
			currentAST.child = casesGroup_AST!=null &&casesGroup_AST.getFirstChild()!=null ?
				casesGroup_AST.getFirstChild() : casesGroup_AST;
			currentAST.advanceChildToEnd();
			casesGroup_AST = (AST)currentAST.root;
		}
		catch (RecognitionException ex) {
			reportError(ex);
			consume();
			consumeUntil(_tokenSet_25);
		}
		returnAST = casesGroup_AST;
	}
	
	public final void caseStatementList() throws RecognitionException, TokenStreamException {
		
		returnAST = null;
		ASTPair currentAST = new ASTPair();
		AST caseStatementList_AST = null;
		
		try {      // for error handling
			{
			_loop108:
			do {
				if ((_tokenSet_7.member(LA(1)))) {
					statement();
					astFactory.addASTChild(currentAST, returnAST);
				}
				else {
					break _loop108;
				}
				
			} while (true);
			}
			caseStatementList_AST = (AST)currentAST.root;
			caseStatementList_AST 
			=(AST)astFactory.make( (new ASTArray(2)).add((AST)astFactory.create(STATEMENT_LIST,"STATEMENT_LIST")).add(caseStatementList_AST));
			
			currentAST.root = caseStatementList_AST;
			currentAST.child = caseStatementList_AST!=null &&caseStatementList_AST.getFirstChild()!=null ?
				caseStatementList_AST.getFirstChild() : caseStatementList_AST;
			currentAST.advanceChildToEnd();
			caseStatementList_AST = (AST)currentAST.root;
		}
		catch (RecognitionException ex) {
			reportError(ex);
			consume();
			consumeUntil(_tokenSet_25);
		}
		returnAST = caseStatementList_AST;
	}
	
	public final void logicalOrExpression() throws RecognitionException, TokenStreamException {
		
		returnAST = null;
		ASTPair currentAST = new ASTPair();
		AST logicalOrExpression_AST = null;
		Token  lor = null;
		AST lor_AST = null;
		Token  dor = null;
		AST dor_AST = null;
		
		try {      // for error handling
			logicalAndExpression();
			astFactory.addASTChild(currentAST, returnAST);
			{
			_loop113:
			do {
				if ((LA(1)==LOR||LA(1)==DOR)) {
					{
					switch ( LA(1)) {
					case LOR:
					{
						lor = LT(1);
						lor_AST = (AST)astFactory.create(lor);
						astFactory.makeASTRoot(currentAST, lor_AST);
						match(LOR);
						logicalOrExpression_AST = (AST)currentAST.root;
						logicalOrExpression_AST.setType(LOGICAL_OR_EXPRESSION);
						logicalOrExpression_AST.setText("LOGICAL_OR_EXPRESSION"); 
										
						break;
					}
					case DOR:
					{
						dor = LT(1);
						dor_AST = (AST)astFactory.create(dor);
						astFactory.makeASTRoot(currentAST, dor_AST);
						match(DOR);
						logicalOrExpression_AST = (AST)currentAST.root;
						logicalOrExpression_AST.setType(LOGICAL_OR_EXPRESSION);
						logicalOrExpression_AST.setText("LOGICAL_OR_EXPRESSION");
						
						break;
					}
					default:
					{
						throw new NoViableAltException(LT(1), getFilename());
					}
					}
					}
					logicalAndExpression();
					astFactory.addASTChild(currentAST, returnAST);
				}
				else {
					break _loop113;
				}
				
			} while (true);
			}
			logicalOrExpression_AST = (AST)currentAST.root;
		}
		catch (RecognitionException ex) {
			reportError(ex);
			consume();
			consumeUntil(_tokenSet_24);
		}
		returnAST = logicalOrExpression_AST;
	}
	
	public final void logicalAndExpression() throws RecognitionException, TokenStreamException {
		
		returnAST = null;
		ASTPair currentAST = new ASTPair();
		AST logicalAndExpression_AST = null;
		
		try {      // for error handling
			logicalNegationExpression();
			astFactory.addASTChild(currentAST, returnAST);
			{
			_loop117:
			do {
				if ((LA(1)==LAND||LA(1)==DAND)) {
					{
					switch ( LA(1)) {
					case LAND:
					{
						AST tmp76_AST = null;
						tmp76_AST = (AST)astFactory.create(LT(1));
						astFactory.makeASTRoot(currentAST, tmp76_AST);
						match(LAND);
						logicalAndExpression_AST = (AST)currentAST.root;
						logicalAndExpression_AST.setType(LOGICAL_AND_EXPRESSION);
						logicalAndExpression_AST.setText("LOGICAL_AND_EXPRESSION");
						break;
					}
					case DAND:
					{
						AST tmp77_AST = null;
						tmp77_AST = (AST)astFactory.create(LT(1));
						astFactory.makeASTRoot(currentAST, tmp77_AST);
						match(DAND);
						logicalAndExpression_AST = (AST)currentAST.root;
						logicalAndExpression_AST.setType(LOGICAL_AND_EXPRESSION);
						logicalAndExpression_AST.setText("LOGICAL_AND_EXPRESSION");
						break;
					}
					default:
					{
						throw new NoViableAltException(LT(1), getFilename());
					}
					}
					}
					logicalNegationExpression();
					astFactory.addASTChild(currentAST, returnAST);
				}
				else {
					break _loop117;
				}
				
			} while (true);
			}
			logicalAndExpression_AST = (AST)currentAST.root;
		}
		catch (RecognitionException ex) {
			reportError(ex);
			consume();
			consumeUntil(_tokenSet_26);
		}
		returnAST = logicalAndExpression_AST;
	}
	
	public final void logicalNegationExpression() throws RecognitionException, TokenStreamException {
		
		returnAST = null;
		ASTPair currentAST = new ASTPair();
		AST logicalNegationExpression_AST = null;
		
		try {      // for error handling
			switch ( LA(1)) {
			case LNOT:
			{
				AST tmp78_AST = null;
				tmp78_AST = (AST)astFactory.create(LT(1));
				astFactory.makeASTRoot(currentAST, tmp78_AST);
				match(LNOT);
				secondaryExpression();
				astFactory.addASTChild(currentAST, returnAST);
				logicalNegationExpression_AST = (AST)currentAST.root;
				logicalNegationExpression_AST.setType(LOGICAL_NEGATION_EXPRESSION);
				logicalNegationExpression_AST.setText("LOGICAL_NEGATION_EXPRESSION");
				logicalNegationExpression_AST = (AST)currentAST.root;
				break;
			}
			case BANG:
			{
				AST tmp79_AST = null;
				tmp79_AST = (AST)astFactory.create(LT(1));
				astFactory.makeASTRoot(currentAST, tmp79_AST);
				match(BANG);
				secondaryExpression();
				astFactory.addASTChild(currentAST, returnAST);
				logicalNegationExpression_AST = (AST)currentAST.root;
				logicalNegationExpression_AST.setType(LOGICAL_NEGATION_EXPRESSION);
				logicalNegationExpression_AST.setText("LOGICAL_NEGATION_EXPRESSION");
				logicalNegationExpression_AST = (AST)currentAST.root;
				break;
			}
			case CONTEXT_KEYWORD:
			case IDENT:
			case LPAREN:
			{
				secondaryExpression();
				astFactory.addASTChild(currentAST, returnAST);
				logicalNegationExpression_AST = (AST)currentAST.root;
				break;
			}
			default:
			{
				throw new NoViableAltException(LT(1), getFilename());
			}
			}
		}
		catch (RecognitionException ex) {
			reportError(ex);
			consume();
			consumeUntil(_tokenSet_27);
		}
		returnAST = logicalNegationExpression_AST;
	}
	
	public final void secondaryExpression() throws RecognitionException, TokenStreamException {
		
		returnAST = null;
		ASTPair currentAST = new ASTPair();
		AST secondaryExpression_AST = null;
		
		try {      // for error handling
			switch ( LA(1)) {
			case CONTEXT_KEYWORD:
			case IDENT:
			{
				booleanValueExpression();
				astFactory.addASTChild(currentAST, returnAST);
				secondaryExpression_AST = (AST)currentAST.root;
				break;
			}
			case LPAREN:
			{
				AST tmp80_AST = null;
				tmp80_AST = (AST)astFactory.create(LT(1));
				match(LPAREN);
				logicalOrExpression();
				astFactory.addASTChild(currentAST, returnAST);
				AST tmp81_AST = null;
				tmp81_AST = (AST)astFactory.create(LT(1));
				match(RPAREN);
				secondaryExpression_AST = (AST)currentAST.root;
				break;
			}
			default:
			{
				throw new NoViableAltException(LT(1), getFilename());
			}
			}
		}
		catch (RecognitionException ex) {
			reportError(ex);
			consume();
			consumeUntil(_tokenSet_27);
		}
		returnAST = secondaryExpression_AST;
	}
	
	public final void booleanValueExpression() throws RecognitionException, TokenStreamException {
		
		returnAST = null;
		ASTPair currentAST = new ASTPair();
		AST booleanValueExpression_AST = null;
		AST id_AST = null;
		AST rt_AST = null;
		AST rt2_AST = null;
		
		try {      // for error handling
			primaryExpression();
			id_AST = (AST)returnAST;
			{
			switch ( LA(1)) {
			case LPAREN:
			{
				constraintInvocation(id_AST);
				rt_AST = (AST)returnAST;
				booleanValueExpression_AST = (AST)currentAST.root;
				booleanValueExpression_AST=rt_AST;
				currentAST.root = booleanValueExpression_AST;
				currentAST.child = booleanValueExpression_AST!=null &&booleanValueExpression_AST.getFirstChild()!=null ?
					booleanValueExpression_AST.getFirstChild() : booleanValueExpression_AST;
				currentAST.advanceChildToEnd();
				break;
			}
			case EQUALS:
			case DEQUALS:
			case NOT_EQUALS:
			{
				equationExpression(id_AST);
				rt2_AST = (AST)returnAST;
				booleanValueExpression_AST = (AST)currentAST.root;
				booleanValueExpression_AST=rt2_AST;
				currentAST.root = booleanValueExpression_AST;
				currentAST.child = booleanValueExpression_AST!=null &&booleanValueExpression_AST.getFirstChild()!=null ?
					booleanValueExpression_AST.getFirstChild() : booleanValueExpression_AST;
				currentAST.advanceChildToEnd();
				break;
			}
			default:
			{
				throw new NoViableAltException(LT(1), getFilename());
			}
			}
			}
		}
		catch (RecognitionException ex) {
			reportError(ex);
			consume();
			consumeUntil(_tokenSet_27);
		}
		returnAST = booleanValueExpression_AST;
	}
	
	public final void primaryExpression() throws RecognitionException, TokenStreamException {
		
		returnAST = null;
		ASTPair currentAST = new ASTPair();
		AST primaryExpression_AST = null;
		
		try {      // for error handling
			switch ( LA(1)) {
			case IDENT:
			{
				identifier();
				astFactory.addASTChild(currentAST, returnAST);
				primaryExpression_AST = (AST)currentAST.root;
				break;
			}
			case CONTEXT_KEYWORD:
			{
				AST tmp82_AST = null;
				tmp82_AST = (AST)astFactory.create(LT(1));
				astFactory.addASTChild(currentAST, tmp82_AST);
				match(CONTEXT_KEYWORD);
				primaryExpression_AST = (AST)currentAST.root;
				break;
			}
			default:
			{
				throw new NoViableAltException(LT(1), getFilename());
			}
			}
		}
		catch (RecognitionException ex) {
			reportError(ex);
			consume();
			consumeUntil(_tokenSet_28);
		}
		returnAST = primaryExpression_AST;
	}
	
	public final void constraintInvocation(
		AST constraintName
	) throws RecognitionException, TokenStreamException {
		
		returnAST = null;
		ASTPair currentAST = new ASTPair();
		AST constraintInvocation_AST = null;
		AST args_AST = null;
		
		try {      // for error handling
			AST tmp83_AST = null;
			tmp83_AST = (AST)astFactory.create(LT(1));
			match(LPAREN);
			constraintInvocationArguments();
			args_AST = (AST)returnAST;
			AST tmp84_AST = null;
			tmp84_AST = (AST)astFactory.create(LT(1));
			match(RPAREN);
			constraintInvocation_AST = (AST)currentAST.root;
			constraintInvocation_AST
			=(AST)astFactory.make( (new ASTArray(3)).add((AST)astFactory.create(CONSTRAINT_INVOCATION,"CONSTRAINT_INVOCATION")).add(constraintName).add(args_AST));
			updateSourceInfo((JmplASTNode)constraintInvocation_AST,(JmplASTNode)constraintName);
			
			currentAST.root = constraintInvocation_AST;
			currentAST.child = constraintInvocation_AST!=null &&constraintInvocation_AST.getFirstChild()!=null ?
				constraintInvocation_AST.getFirstChild() : constraintInvocation_AST;
			currentAST.advanceChildToEnd();
		}
		catch (RecognitionException ex) {
			reportError(ex);
			consume();
			consumeUntil(_tokenSet_27);
		}
		returnAST = constraintInvocation_AST;
	}
	
	public final void equationExpression(
		AST leftHandSide
	) throws RecognitionException, TokenStreamException {
		
		returnAST = null;
		ASTPair currentAST = new ASTPair();
		AST equationExpression_AST = null;
		AST rightHandSide1_AST = null;
		AST rightHandSide2_AST = null;
		
		try {      // for error handling
			switch ( LA(1)) {
			case EQUALS:
			case DEQUALS:
			{
				logicalEqualityExpression();
				rightHandSide1_AST = (AST)returnAST;
				equationExpression_AST = (AST)currentAST.root;
				equationExpression_AST
				=(AST)astFactory.make( (new ASTArray(3)).add((AST)astFactory.create(LOGICAL_EQUALITY_EXPRESSION,"LOGICAL_EQUALITY_EXPRESSION")).add(leftHandSide).add(rightHandSide1_AST));
				updateSourceInfo((JmplASTNode)equationExpression_AST,(JmplASTNode)leftHandSide);
				
				currentAST.root = equationExpression_AST;
				currentAST.child = equationExpression_AST!=null &&equationExpression_AST.getFirstChild()!=null ?
					equationExpression_AST.getFirstChild() : equationExpression_AST;
				currentAST.advanceChildToEnd();
				break;
			}
			case NOT_EQUALS:
			{
				logicalInequalityExpression();
				rightHandSide2_AST = (AST)returnAST;
				equationExpression_AST = (AST)currentAST.root;
				equationExpression_AST
				=(AST)astFactory.make( (new ASTArray(3)).add((AST)astFactory.create(LOGICAL_INEQUALITY_EXPRESSION,"LOGICAL_INEQUALITY_EXPRESSION")).add(leftHandSide).add(rightHandSide2_AST)); 
				updateSourceInfo((JmplASTNode)equationExpression_AST,(JmplASTNode)leftHandSide);
				
				currentAST.root = equationExpression_AST;
				currentAST.child = equationExpression_AST!=null &&equationExpression_AST.getFirstChild()!=null ?
					equationExpression_AST.getFirstChild() : equationExpression_AST;
				currentAST.advanceChildToEnd();
				break;
			}
			default:
			{
				throw new NoViableAltException(LT(1), getFilename());
			}
			}
		}
		catch (RecognitionException ex) {
			reportError(ex);
			consume();
			consumeUntil(_tokenSet_27);
		}
		returnAST = equationExpression_AST;
	}
	
	public final void logicalEqualityExpression() throws RecognitionException, TokenStreamException {
		
		returnAST = null;
		ASTPair currentAST = new ASTPair();
		AST logicalEqualityExpression_AST = null;
		
		try {      // for error handling
			{
			int _cnt126=0;
			_loop126:
			do {
				if ((LA(1)==EQUALS||LA(1)==DEQUALS)) {
					{
					switch ( LA(1)) {
					case EQUALS:
					{
						AST tmp85_AST = null;
						tmp85_AST = (AST)astFactory.create(LT(1));
						match(EQUALS);
						break;
					}
					case DEQUALS:
					{
						AST tmp86_AST = null;
						tmp86_AST = (AST)astFactory.create(LT(1));
						match(DEQUALS);
						break;
					}
					default:
					{
						throw new NoViableAltException(LT(1), getFilename());
					}
					}
					}
					primaryExpression();
					astFactory.addASTChild(currentAST, returnAST);
				}
				else {
					if ( _cnt126>=1 ) { break _loop126; } else {throw new NoViableAltException(LT(1), getFilename());}
				}
				
				_cnt126++;
			} while (true);
			}
			logicalEqualityExpression_AST = (AST)currentAST.root;
		}
		catch (RecognitionException ex) {
			reportError(ex);
			consume();
			consumeUntil(_tokenSet_27);
		}
		returnAST = logicalEqualityExpression_AST;
	}
	
	public final void logicalInequalityExpression() throws RecognitionException, TokenStreamException {
		
		returnAST = null;
		ASTPair currentAST = new ASTPair();
		AST logicalInequalityExpression_AST = null;
		
		try {      // for error handling
			{
			int _cnt129=0;
			_loop129:
			do {
				if ((LA(1)==NOT_EQUALS)) {
					AST tmp87_AST = null;
					tmp87_AST = (AST)astFactory.create(LT(1));
					match(NOT_EQUALS);
					primaryExpression();
					astFactory.addASTChild(currentAST, returnAST);
				}
				else {
					if ( _cnt129>=1 ) { break _loop129; } else {throw new NoViableAltException(LT(1), getFilename());}
				}
				
				_cnt129++;
			} while (true);
			}
			logicalInequalityExpression_AST = (AST)currentAST.root;
		}
		catch (RecognitionException ex) {
			reportError(ex);
			consume();
			consumeUntil(_tokenSet_27);
		}
		returnAST = logicalInequalityExpression_AST;
	}
	
	public final void constraintInvocationArguments() throws RecognitionException, TokenStreamException {
		
		returnAST = null;
		ASTPair currentAST = new ASTPair();
		AST constraintInvocationArguments_AST = null;
		
		try {      // for error handling
			{
			switch ( LA(1)) {
			case CONTEXT_KEYWORD:
			case IDENT:
			{
				primaryExpression();
				astFactory.addASTChild(currentAST, returnAST);
				{
				_loop134:
				do {
					if ((LA(1)==COMMA)) {
						AST tmp88_AST = null;
						tmp88_AST = (AST)astFactory.create(LT(1));
						match(COMMA);
						primaryExpression();
						astFactory.addASTChild(currentAST, returnAST);
					}
					else {
						break _loop134;
					}
					
				} while (true);
				}
				break;
			}
			case RPAREN:
			{
				break;
			}
			default:
			{
				throw new NoViableAltException(LT(1), getFilename());
			}
			}
			}
			constraintInvocationArguments_AST = (AST)currentAST.root;
		}
		catch (RecognitionException ex) {
			reportError(ex);
			consume();
			consumeUntil(_tokenSet_19);
		}
		returnAST = constraintInvocationArguments_AST;
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
	
	private static final long _tokenSet_0_data_[] = { 2L, 0L };
	public static final BitSet _tokenSet_0 = new BitSet(_tokenSet_0_data_);
	private static final long _tokenSet_1_data_[] = { 962L, 0L };
	public static final BitSet _tokenSet_1 = new BitSet(_tokenSet_1_data_);
	private static final long _tokenSet_2_data_[] = { 36028797018963968L, 0L };
	public static final BitSet _tokenSet_2 = new BitSet(_tokenSet_2_data_);
	private static final long _tokenSet_3_data_[] = { 72057594037927936L, 0L };
	public static final BitSet _tokenSet_3 = new BitSet(_tokenSet_3_data_);
	private static final long _tokenSet_4_data_[] = { 180143985094819840L, 0L };
	public static final BitSet _tokenSet_4 = new BitSet(_tokenSet_4_data_);
	private static final long _tokenSet_5_data_[] = { 54043195528454144L, 0L };
	public static final BitSet _tokenSet_5 = new BitSet(_tokenSet_5_data_);
	private static final long _tokenSet_6_data_[] = { 135107988821178608L, 0L };
	public static final BitSet _tokenSet_6 = new BitSet(_tokenSet_6_data_);
	private static final long _tokenSet_7_data_[] = { -9124292845049806848L, 6144L, 0L, 0L };
	public static final BitSet _tokenSet_7 = new BitSet(_tokenSet_7_data_);
	private static final long _tokenSet_8_data_[] = { -9088264048029468432L, 6144L, 0L, 0L };
	public static final BitSet _tokenSet_8 = new BitSet(_tokenSet_8_data_);
	private static final long _tokenSet_9_data_[] = { 18014398509481984L, 0L };
	public static final BitSet _tokenSet_9 = new BitSet(_tokenSet_9_data_);
	private static final long _tokenSet_10_data_[] = { 99079191802214640L, 0L };
	public static final BitSet _tokenSet_10 = new BitSet(_tokenSet_10_data_);
	private static final long _tokenSet_11_data_[] = { -4080261262397669376L, 26561L, 0L, 0L };
	public static final BitSet _tokenSet_11 = new BitSet(_tokenSet_11_data_);
	private static final long _tokenSet_12_data_[] = { 2458965396544290816L, 3L, 0L, 0L };
	public static final BitSet _tokenSet_12 = new BitSet(_tokenSet_12_data_);
	private static final long _tokenSet_13_data_[] = { 8241587318088007680L, 7L, 0L, 0L };
	public static final BitSet _tokenSet_13 = new BitSet(_tokenSet_13_data_);
	private static final long _tokenSet_14_data_[] = { 9007199254741184L, 0L };
	public static final BitSet _tokenSet_14 = new BitSet(_tokenSet_14_data_);
	private static final long _tokenSet_15_data_[] = { 90071992547409920L, 0L };
	public static final BitSet _tokenSet_15 = new BitSet(_tokenSet_15_data_);
	private static final long _tokenSet_16_data_[] = { 234187180623265792L, 0L };
	public static final BitSet _tokenSet_16 = new BitSet(_tokenSet_16_data_);
	private static final long _tokenSet_17_data_[] = { 216172782113783808L, 0L };
	public static final BitSet _tokenSet_17 = new BitSet(_tokenSet_17_data_);
	private static final long _tokenSet_18_data_[] = { 216172782113783808L, 1L, 0L, 0L };
	public static final BitSet _tokenSet_18 = new BitSet(_tokenSet_18_data_);
	private static final long _tokenSet_19_data_[] = { 0L, 1L, 0L, 0L };
	public static final BitSet _tokenSet_19 = new BitSet(_tokenSet_19_data_);
	private static final long _tokenSet_20_data_[] = { 144115188075855872L, 1L, 0L, 0L };
	public static final BitSet _tokenSet_20 = new BitSet(_tokenSet_20_data_);
	private static final long _tokenSet_21_data_[] = { 4755801206503243776L, 1L, 0L, 0L };
	public static final BitSet _tokenSet_21 = new BitSet(_tokenSet_21_data_);
	private static final long _tokenSet_22_data_[] = { -9088264048029532160L, 6144L, 0L, 0L };
	public static final BitSet _tokenSet_22 = new BitSet(_tokenSet_22_data_);
	private static final long _tokenSet_23_data_[] = { -7647112167270909712L, 30720L, 0L, 0L };
	public static final BitSet _tokenSet_23 = new BitSet(_tokenSet_23_data_);
	private static final long _tokenSet_24_data_[] = { 72057594037927936L, 1L, 0L, 0L };
	public static final BitSet _tokenSet_24 = new BitSet(_tokenSet_24_data_);
	private static final long _tokenSet_25_data_[] = { 36028797020012544L, 0L };
	public static final BitSet _tokenSet_25 = new BitSet(_tokenSet_25_data_);
	private static final long _tokenSet_26_data_[] = { 72057594037927936L, 385L, 0L, 0L };
	public static final BitSet _tokenSet_26 = new BitSet(_tokenSet_26_data_);
	private static final long _tokenSet_27_data_[] = { 72057594037927936L, 1921L, 0L, 0L };
	public static final BitSet _tokenSet_27 = new BitSet(_tokenSet_27_data_);
	private static final long _tokenSet_28_data_[] = { -8718968878589280256L, 26497L, 0L, 0L };
	public static final BitSet _tokenSet_28 = new BitSet(_tokenSet_28_data_);
	
	}

/**
 *  IfStatement.h
 *
 *  Class that represents an if-statement
 *
 *  @author Emiel Bruijntjes <emiel.bruijntjes@copernica.com>
 *  @copyright 2014 Copernica BV
 */

/**
 *  Set up namespace
 */
namespace SmartTpl {

/**
 *  Class definition
 */
class IfStatement : public Statement
{
private:
    /**
     *  The condition
     *  @var    Expression
     */
    std::unique_ptr<Expression> _expression;
    
    /**
     *  The statements in the 'if' part
     *  @var    Statements
     */
    std::unique_ptr<Statements> _trueStatements;
    
    /**
     *  The statements in the 'else' part
     *  @var    Statement
     */
    std::unique_ptr<Statements> _falseStatements;
    
public:
    /**
     *  Constructor
     *  @param  expression
     *  @param  statements
     */
    IfStatement(Expression *expression, Statements *statements) :
        _expression(expression), _trueStatements(statements) {}
    
    /**
     *  Constructor
     *  @param  expression
     *  @param  trueStatements
     *  @param  falseStatements
     */
    IfStatement(Expression *expression, Statements *trueStatements, Statements *falseStatements) :
        _expression(expression), _trueStatements(trueStatements), _falseStatements(falseStatements) {}
        
    /**
     *  Destructor
     */
    virtual ~IfStatement() {}
};

/**
 *  End namespace
 */
}


/**
 *  CCode.h
 *
 *  Class that turns a template syntax tree into C code that can be compiled
 *  into a shared library.
 *
 *  This class overrides from the Generator base class, because it is one
 *  of the generators of the SmartTpl library (the template-to-c-code generator)
 *
 *  @author Emiel Bruijntjes <emiel.bruijntjes@copernica.com>
 *  @copyright 2014 Copernica BV
 */

/**
 *  Namespace
 */
namespace SmartTpl {

/**
 *  Class definition
 */
class CCode : public Generator
{
private:
    /**
     *  Output stream
     *  @var    std::ostream
     */
    std::ostringstream _out;

    /**
     *  Output raw data
     *  @param  data        buffer to output
     */
    virtual void raw(const std::string &data) override;
        
    /**
     *  Generate the code to output a variable
     *  @param  variable           The variable to output
     */
    virtual void output(const Variable *variable) override;

    /**
     *  Generate the code to write an expression as a string
     *  @param  expression          the expression to write as a string
     */
    virtual void write(const Expression *expression) override;

    /**
     *  Generate a conditional statement
     *  @param  expression          the expression to evaluate
     *  @param  ifstatements        the statements in the 'if' part
     *  @param  elsestatements      the statements in the 'else' part
     */
    virtual void condition(const Expression *expression, const Statements *ifstatements, const  Statements *elsestatements) override;

    /**
     *  Generate the code to get a pointer to a variable
     *  There are three formats, to get a pointer to a literal variable by name,
     *  to get a pointer to a variable inside a table with a literal name, and
     *  to get a pointer to a variable with variable name
     *  @param  parent              parent variable from which the var is retrieved
     *  @param  name                name of the variable
     *  @param  expression          Expression that evaluates to a var name
     */
    virtual void varPointer(const Variable *parent, const std::string &name) override;
    virtual void varPointer(const Variable *parent, const Expression *expression) override;
    virtual void varPointer(const std::string &name) override;

    /**
     *  Create a string or numeric literal
     *  @param  value
     */
    virtual void string(const std::string &value) override;
    virtual void numeric(int value) override;
    
    /**
     *  Create a string, numeric or boolean constant for a variable
     *  @param  variable
     */
    virtual void string(const Variable *variable) override;
    virtual void numeric(const Variable *variable) override;
    virtual void boolean(const Variable *variable) override;
    virtual void variable(const Variable *variable) override;

    /**
     *  Create a string literal from an expression that is known to return a numeric value,
     *  and a method to do this in the other direction
     *  @param  expression
     */
    virtual void numericToString(const Expression *expression) override;
    virtual void stringToNumeric(const Expression *expression) override;
    
    /**
     *  Arithmetric operations
     *  @param  left
     *  @param  right
     */
    virtual void plus(const Expression *left, const Expression *right) override;
    virtual void minus(const Expression *left, const Expression *right) override;
    virtual void divide(const Expression *left, const Expression *right) override;
    virtual void multiply(const Expression *left, const Expression *right) override;
  
    /**
     *  Comparison operators
     *  @param  left
     *  @param  right
     */
    virtual void equals(const Expression *left, const Expression *right) override;
    virtual void notEquals(const Expression *left, const Expression *right) override;
    virtual void greater(const Expression *left, const Expression *right) override;
    virtual void greaterEquals(const Expression *left, const Expression *right) override;
    virtual void lesser(const Expression *left, const Expression *right) override;
    virtual void lesserEquals(const Expression *left, const Expression *right) override;
    
    /**
     *  Boolean operators
     *  @param  left
     *  @param  right
     */
    virtual void booleanAnd(const Expression *left, const Expression *right) override;
    virtual void booleanOr(const Expression *left, const Expression *right) override;

    /**
     *  Generate the code to apply a set of modifiers on an expression
     *  @param  modifiers          The set of modifiers to apply
     *  @param  expression         The expression to apply to modifiers on
     */
    virtual void modifiers(const Modifiers* modifiers, const Expression *expression) override;
    
public:
    /**
     *  Constructor
     *  @param  tree        The abstract syntax tree of the template
     */
    CCode(const SyntaxTree &tree);
    
    /**
     *  Destructor
     */
    virtual ~CCode() {}

    /**
     *  Retrieve the C source as string
     *  @return std::string
     */
    std::string asString() const
    {
        return _out.str();
    }
};

/**
 *  Custom output stream operator
 *  @param  stream
 *  @param  value
 *  @return ostream
 */
inline std::ostream &operator<<(std::ostream &stream, const CCode &value)
{
    return stream << value.asString();
}

/**
 *  End namespace
 */
}


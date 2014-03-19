/**
 *  VarStatement.h
 *
 *  Statement to echo output of a {$variable}.
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
class VarStatement : public Statement
{
private:
    /**
     *  The variable that should be displayed
     *  @var    Variable
     */
    std::unique_ptr<Variable> _var;

public:
    /**
     *  Constructor
     *  @param  var
     */
    VarStatement(Variable *var) : _var(std::unique_ptr<Variable>(var)) {}
    
    /**
     *  Destructor
     */
    virtual ~VarStatement() {}

    /**
     *  Generate the output of this statement
     *  @param  str     output stream
     */
    virtual void generate(std::ostream &str) const override
    {
        // pass on to the variable
        _var->generate(str);
    }
};

/**
 *  End of namespace
 */
}


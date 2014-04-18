/**
 *  Filter.h
 *
 *  A filter combines an expression with a number of modifiers
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
class Filter : public Variable
{
private:
    /**
     *  The base expression
     *  @var    Expression
     */
    std::unique_ptr<const Expression> _expression;

    /**
     *  The modifiers that should be applied
     *  @var    Modifiers
     */
    std::unique_ptr<const Modifiers> _modifiers;

public:
    /**
     *  Constructor
     *  @param  expression
     *  @param  modifiers
     */
    Filter(const Expression *expression, const Modifiers *modifiers) :
        _expression(expression), _modifiers(modifiers) {}
        
    /**
     *  Destructor
     */
    virtual ~Filter() {}

    virtual void pointer(Generator *generator) const override
    {
        _modifiers.get()->generate(generator, _expression.get());
    }

    /**
     *  Generate the expression as a numeric value
     *  @param  generator
     */
    virtual void numeric(Generator *generator) const override
    {
        // @todo implementation
    }
    
    /**
     *  Generate the expression as a boolean value
     *  @param  generator
     */
    virtual void boolean(Generator *generator) const override
    {
        // @todo implementation
    }
    
    /**
     *  Generate the expression as string value
     *  @param  generator
     */
    virtual void string(Generator *generator) const override
    {
        std::cout << "Filter::string(" << generator << ");" << std::endl;
        _modifiers.get()->generate(generator, _expression.get());
        // @todo implementation
    }

    virtual void variable(Generator *generator) const override
    {
        _expression.get()->variable(generator);
    }
};

/**
 *  End namespace
 */
}


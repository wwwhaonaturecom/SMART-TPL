/**
 *  String.h
 *
 *  Implementation of a literal string value
 *
 *  @author Emiel Bruijntjes <emiel.bruijntjes@copernica.com>
 *  @copyright 2014 Copernica BV
 */

/**
 *  Namespace
 */
namespace SmartTpl { namespace Internal {

/**
 *  Class definition
 */
class LiteralString : public Literal
{
private:
    /**
     *  The actual value
     *  @var    std::string
     */
    std::unique_ptr<const Token> _value;

public:
    /**
     *  Constructor
     *  @param  std::string
     */
    LiteralString(Token *value) : _value(value) {}

    /**
     *  Destructor
     */
    virtual ~LiteralString() {}

    /**
     *  The return type of the expression
     *  @return Type
     */
    Type type() const override { return Type::String; }

    /**
     *  Generate the code to get the const char * to the expression
     *  @param  generator
     */
    void string(Generator *generator) const override
    {
        // generate our actual value
        generator->string(*_value);
    }

    /**
     *  Generate the code to get the boolean value of the expression
     *  @param  generator
     */
    void boolean(Generator *generator) const override
    {
        // just return 0
        generator->numeric(0);
    }

    /**
     *  Generate the code to get the integer value of the expression
     *  @param  generator
     */
    void numeric(Generator *generator) const override
    {
        // just return 0
        generator->numeric(0);
    }

    /**
     *  Generate the expression as a double value
     *  @param  generator
     */
    void double_type(Generator *generator) const override
    {
        // just return 0
        generator->double_type(0);
    }
};

/**
 *  End namespace
 */
}}
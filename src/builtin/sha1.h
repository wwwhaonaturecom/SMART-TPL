/**
 *  Sha1.h
 *
 *  Built-in "|sha1" modifier
 *
 *  @author Toon Schoenmakers <toon.schoenmakers@copernica.com>
 *  @copyright 2014 Copernica BV
 */

/**
 *  Namespace
 */
namespace SmartTpl { namespace Internal {

/**
 *  Class definition
 */
class Sha1Modifier : public Modifier
{
public:
    /**
     *  Destructor
     */
    virtual ~Sha1Modifier() {};

    /**
     *  Modify a value object
     *  @param  input
     *  @param  params      Parameters used for this modification
     *  @return Value
     */
    virtual Variant modify(const Variant &input, const SmartTpl::Parameters &params) override
    {
        // initialize our output
        unsigned char digest[SHA_DIGEST_LENGTH];

        // Call the openssl md5 method
        SHA1((unsigned char*) input.toString(), input.size(), (unsigned char*) &digest);

        std::ostringstream stream;
        stream << std::setfill('0') << std::hex;

        for (int i = 0; i < sizeof(digest); ++i) stream << std::setw(2) << ((unsigned int) digest[i]);

        // Return our stream as a string
        return stream.str();
    }
};

/**
 *  End namespace
 */
}}
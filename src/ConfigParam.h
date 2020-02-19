#ifndef ConfigParam_h
#define ConfigParam_h

class ConfigParam
{
private:
    const char * id;
    const char * label;
    char *  value;
    const char * fieldType;
public:
    ConfigParam(const char * _id, const char * label, char * value, const char * fieldType);

    const char * getId();
    const char * getLabel();
    char * getValue();
    const char * getFieldType();

    ~ConfigParam();
};

#endif

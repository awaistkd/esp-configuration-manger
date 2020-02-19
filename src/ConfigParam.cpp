#include "ConfigParam.h"
#include <string>
ConfigParam::ConfigParam(const char * _id, const char * _label, char * _value, const char * _fieldType) {
    id = _id;
    label = _label;
    value = _value;
    fieldType = _fieldType;
}

ConfigParam::~ConfigParam() {
    if (value != NULL)
    {
        delete[] value;
    }
}

const char * ConfigParam::getId() {
    return id;
}

const char * ConfigParam::getLabel() {
    return label;
}

char * ConfigParam::getValue() {
    return value;
}

const char * ConfigParam::getFieldType() {
    return fieldType;
}

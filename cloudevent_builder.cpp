/**
  Karim Cheratt
  MIT Licence
**/

#include "cloudevent_builder.h"

CloudEventBuilder::CloudEventBuilder(){};
// CloudEventBuilder ~CloudEventBuilder(){
//   this->specversion = "";
// };

void CloudEventBuilder::setSpecVersion(string version)
{
  this->_specversion = version;
}

void CloudEventBuilder::setId(string id)
{
  this->_id = id;
}

void CloudEventBuilder::setSource(string source)
{
  this->_source = source;
}

void CloudEventBuilder::setType(string type)
{
  this->_type = type;
}

void CloudEventBuilder::setSubject(string subject)
{
  this->_subject = subject;
}

void CloudEventBuilder::setDataContenType(string content_type)
{
  this->_datacontenttype = content_type;
}

void CloudEventBuilder::setDataSchema(string data_schema)
{
  this->_dataschema = data_schema;
}

void CloudEventBuilder::setData(string data[], int dataArraySize)
{
  this->_data = this->toJSON(data, dataArraySize);
}

string CloudEventBuilder::toJSON(string data[], int dataArraySize)
{
  int i = 0;
  string temp_str = "";
  string result = "{";
  while (i < dataArraySize)
  {
    result.append("\"");
    // extract propertie
    temp_str = data[i].substr(0, data[i].find(":"));
    result.append(temp_str);
    result.append("\":\"");
    // extract propertie value
    temp_str = data[i].substr(temp_str.length() + 1);
    result.append(temp_str);
    result.append("\"");
    i++;
    if (i < dataArraySize)
    {
      result.append(",");
    }
  }
  result = result + "}";
  return result;
}

/**
 * used in structured format of the ce
 */
string CloudEventBuilder::HttpPayload()
{
  string ce_json = "{";
  ce_json += "\"specversion\":\"" + this->_specversion + "\"";
  ce_json += ",\"id\":\"" + this->_id + "\"";
  ce_json += ",\"source\":\"" + this->_source + "\"";
  ce_json += ",\"type\":\"" + this->_type + "\"";

  // if empty , do not include datatcontentype in the payload
  if (!this->_subject.empty())
  {
    ce_json += ",\"subject\":\"" + this->_subject + "\"";
  }

  // if empty , do not include datatcontentype in the payload
  if (!this->_datacontenttype.empty())
  {
    ce_json += ",\"datacontenttype\":\"" + this->_datacontenttype + "\"";
  }
  // if empty , do not include dataschema in the payload
  if (!this->_dataschema.empty())
  {
    ce_json += ",\"dataschema\":\"" + this->_dataschema + "\"";
  }
  ce_json += ",\"data\":" + this->_data; // data is already formated as json
  ce_json += "}";

  return ce_json;
}

// };

/**
  Karim Cheratt
  MIT Licence 2021

**/
#include <string>
using namespace std;

class CEBuilder
{

private:
  string ce_specversion = "",
         ce_id = "",
         ce_source = "",
         ce_type = "",
         ce_data = "";

public:
  // CEBuilder CEBuilder(){};
  // CEBuilder ~CEBuilder(){
  //   this->ce_specversion = "";
  // };

  void setSpecVersion(string version)
  {
    this->ce_specversion = version;
  }
  void setId(string id)
  {
    this->ce_id = id;
  }
  void setSource(string source)
  {
    this->ce_source = source;
  }
  void setType(string ce_type)
  {
    this->ce_type = ce_type;
  }

  void setData(string data[], int dataArraySize)
  {
    this->ce_data = this->toJSON(data, dataArraySize);
  }

  string toJSON(string data[], int dataArraySize)
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

  string structuredFormat()
  {
    string __json = "{";
    __json += "\"specversion\":\"" + this->ce_specversion + "\"";
    __json += ",\"id\":\"" + this->ce_id + "\"";
    __json += ",\"source\":\"" + this->ce_source + "\"";
    __json += ",\"type\":\"" + this->ce_type + "\"";
    __json += ",\"data\":" + this->ce_data ; // ce_data is already on a json format
    __json += "}";
    return __json;
  }

};

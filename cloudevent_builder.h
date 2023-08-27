/**
  Karim Cheratt
  MIT Licence
**/

#ifndef CloudEvent_Builder_H
#define CloudEvent_Builder_H

#include <string>
using namespace std;

class CloudEventBuilder {

private:
  // REQUIRED ATTRIBUTES
  string _specversion;
  string _id;
  string _source;
  string _type;

  // OPTIONAL
  string _datacontenttype;
  string _dataschema;
  string _subject;
  string _data;
  //  time = "";

public:
  CloudEventBuilder();

  void setSpecVersion(string version);

  void setId(string id);

  void setSource(string source);

  void setType(string type);

  void setSubject(string subject);

  void setDataContenType(string content_type);

  void setDataSchema(string data_schema);

  void setData(string data[], int dataArraySize);

  string toJSON(string data[], int dataArraySize);

  /**
  * used in structured format of the cloudevent
  */
  string HttpPayload();

};

#endif

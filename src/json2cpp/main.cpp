#include <iostream>
#include <fstream>
#include <stdexcept>

// RapidJSON
#include <rapidjson/document.h>
#include <rapidjson/writer.h>
#include <rapidjson/istreamwrapper.h>

int main ( ) {

   std::ifstream file("/home/vijay/eows/share/eows/config/srs.json", std::ifstream::in);

   if(file)
   {
       try
       {
          rapidjson::IStreamWrapper istr(file);

          rapidjson::Document doc;

          doc.ParseStream(istr);

        if(doc.HasParseError())
        {
          throw std::runtime_error("Erro de Parsing");
        }

        if(!doc.IsObject() || doc.IsNull())
        {

          throw std::runtime_error("Error parsing input file '%1%': unexpected file format.");
        }

        file.close();
      }
      catch(...)
      {
        file.close();
        throw;
      }
   }

    return 0;
}

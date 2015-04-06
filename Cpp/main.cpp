#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <stdio.h>
#include <string.h>
#include <chrono>
#include "rapidxml-1.13/rapidxml.hpp"
#include "rapidxml-1.13/rapidxml_print.hpp"
#include "myhash.h"

using namespace std;
using namespace std::chrono;


std::string pseudonimize(std::string value){
    auto dot = value.find('.');
    auto prefix = value.substr(0, dot+1);
    auto tohash = value.substr(dot+1, std::string::npos);
    auto hexdigest = createSha512Hash(tohash);
    auto suffix = hexdigest.substr(0, tohash.size());
    return prefix + suffix;
}

int process(std::string fname){
    auto t0 = high_resolution_clock::now();
    std::ifstream file(fname);
    std::stringstream buffer;
    buffer << file.rdbuf();
    file.close();
    std::string content(buffer.str());

    rapidxml::xml_document<> doc;
    doc.parse<rapidxml::parse_no_data_nodes>(&content[0]);
    auto item = doc.first_node()->first_node();
    while(item){
        for(auto field=item->first_node("field"); field; field=field->next_sibling()){
            auto pAtttr =  field->first_attribute("name");
            std::string name = pAtttr->value();
            if(name == "origin"){
                std::string value = field->value();
                auto nval = pseudonimize(value).c_str();
                const char * text = doc.allocate_string(nval, strlen(nval));
                field->value(text);
                break;
            }
        }
        item = item->next_sibling();
    }
    auto t1 = high_resolution_clock::now();
    std::ofstream out(fname + ".cpp.anon");
    out << doc;
    out.close();
    return duration_cast<milliseconds>(t1-t0).count();
}

int main()
{
    auto t0 = high_resolution_clock::now();
    auto ptime = process("../access_log.1.xml");
    auto t1 = high_resolution_clock::now();
    auto elapsed = duration_cast<milliseconds>(t1-t0).count();
    cout << ptime << "\t" << elapsed << "\n";
    return 0;
}


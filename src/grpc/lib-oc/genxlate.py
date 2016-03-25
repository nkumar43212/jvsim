
import argparse
import time
import mosquitto
import os
import sys

def replace(filename, search_str, replace_str):
    f = open(filename,'r')
    filedata = f.read()
    f.close()
    newdata = filedata.replace(search_str, replace_str);
    f = open(filename,'w')
    f.write(newdata)
    f.close()

#set up the execution parameters
parser = argparse.ArgumentParser()
parser.add_argument("resource")
parser.add_argument("resource_reg_str")
args = parser.parse_args()
resource     = args.resource
resource_str = args.resource_reg_str

#Make a copy of the template
copy_hpp = "OpenConfig" + resource + ".hpp"
copy_cpp = "OpenConfig" + resource + ".cpp"
cmd = "cp OpenConfigTemplateCpp " + copy_cpp;
os.system(cmd);
cmd = "cp OpenConfigTemplateHpp " + copy_hpp;
os.system(cmd);

replace(copy_hpp, "@", resource);
replace(copy_cpp, "@", resource);
replace(copy_hpp, "$", resource_str);

#include the new header
search_str = "//@header";
header_include = "#include " + "\"" + copy_hpp + "\"" + "\n" + search_str; 
#print(header_include);
replace("oc.cpp", search_str, header_include);

#Add the registration
search_str = "//@next"
registration = "    OpenConfig" + resource + "\t *" + resource + " = new OpenConfig" + resource + ";\n" + search_str;
replace("oc.cpp", search_str, registration);
#print(registration);

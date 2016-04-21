
import argparse
import time
import os
import sys
import subprocess
import string

#Replace a search_str with replace_str in a file
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
args = parser.parse_args()
resource     = args.resource

#Convert file name from abc_xyz to AbcXyz
resource_file_name = resource.replace("_", " ")
resource_file_name = string.capwords(resource_file_name)
resource_file_name = resource_file_name.replace(" ", "")
#print resource_file_name

#Make a copy of the template
copy_hpp = "OpenConfig" + resource_file_name + ".hpp"
copy_cpp = "OpenConfig" + resource_file_name + ".cpp"
cmd = "cp OpenConfigTemplateCpp " + copy_cpp;
os.system(cmd);
cmd = "cp OpenConfigTemplateHpp " + copy_hpp;
os.system(cmd);

#Now substitute required params with proper names
replace(copy_hpp, "@", resource_file_name);
replace(copy_cpp, "@", resource_file_name);
replace(copy_hpp, "$", resource);

#include the new header
search_str = "//@header";
header_include = "#include " + "\"" + copy_hpp + "\"" + "\n" + search_str; 
#print(header_include);
replace("oc.cpp", search_str, header_include);

#Add the registration
search_str = "//@next"
registration = "    OpenConfig" + resource_file_name + "\t *" + resource + \
               " = new OpenConfig" + resource_file_name + ";\n" + search_str;
#print(registration);
replace("oc.cpp", search_str, registration);

#Add extension type
#start with greping the right contents in the proto file
filename="../../../protos/" + resource + ".proto"
command_str='grep -A 1 \"extend JuniperNetworksSensors\" ' + filename
output=subprocess.check_output(command_str, shell=True)
linebyline = output.split("\n")
#second line is extension definition
extensions=linebyline[1]
#Now split words to get to get the extension type, 3rd word is extension type
words=extensions.split()
ext_type=words[2]
#print ext_type
search_str = "//@ext_type"
ext_type_stmt = "    if (handle->HasExtension(" + ext_type + ")) {\n" + "        return \"" + ext_type + "\";\n    }\n" +search_str;
#print ext_type_stmt
replace("oc.cpp", search_str, ext_type_stmt)
replace(copy_hpp, "^", ext_type);
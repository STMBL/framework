import sys

conf_objs = []


header = open(sys.argv[2], 'w')
code = open(sys.argv[3], 'w')

header.write("#pragma once\n")
header.write("template<uint8_t len> constexpr uint64_t name(const char* name);")

with open(sys.argv[1]) as f:
  header.write("struct conf_t{\n")
  for line in f:
    start = line.split(" ")[0]
    if start == "//":
      header.write(line + "\n")
    elif start == "\n":
      header.write("\n")
    else:
      start, name, conf_name, default, *values = line.rstrip("\n").split(" ")
      type = ""
      if start == "int8_t":
        type = "int32_t"
      elif start == "int16_t":
        type = "int32_t"
      elif start == "int32_t":
        type = "int32_t"
      elif start == "uint8_t":
        type = "uint32_t"
      elif start == "uint16_t":
        type = "uint32_t"
      elif start == "uint32_t":
        type = "uint32_t"
      elif start == "float":
        type = "float"
      elif start == "q8_24":
        type = "q8_24"
      elif start == "q16_16":
        type = "q16_16"
      elif start == "q24_8":
        type = "q24_8"
      if type != "":
        tup = (type, name, conf_name)
        conf_objs.append(tup)
        header.write("  " + type + " " + name + " = " + default + "; // " + conf_name + "\n")

      if start == "enum":
        type = "conf_t::" + name + "_t"
        tup = (type, name, conf_name)
        conf_objs.append(tup)
        header.write("  enum " + name + "_t { // " + conf_name + "\n")
        for v in values:
          header.write("    " + v + ",\n")
#          header.write("    " + v + " = name<4>(\"" + v + "\"),\n")
        header.write("  } " + name + " = " + default + ";\n")
  header.write("};\n")
header.close()

code.write("#include <fp_lib.hpp>\n")
code.write("#include <hal.hpp>\n")
code.write("#include <conf.hpp>\n")
code.write("void conf_comp::set(hal_ctx_t* ctx, const char* n, uint8_t len, conf_obj::conf_obj_type_t type, uint8_t crypted, void* data){\n")
code.write("  switch(name<6>(n)){\n")
for obj in conf_objs:
  code.write("    case name<6>(\"" + obj[2] + "\"):\n")
  code.write("      ctx->conf." + obj[1] + "= *((" + obj[0] + "*) data);\n")
  code.write("    break;\n")
code.write("  }\n")
code.write("}\n")
code.close()
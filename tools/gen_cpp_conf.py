import sys

conf_objs = []

def name_hash(s, l):
  r = 0
  for i in range(0, l):
    if i < len(s):
      r <<= 8
      r += ord(s[i])
    # else:
    #   r <<= 8
  return r


header = open(sys.argv[2], 'w')
code = open(sys.argv[3], 'w')

header.write("#include \"fp_lib.hpp\"\n")
header.write("#pragma once\n")
header.write("\n")
header.write("template<uint8_t len> constexpr uint64_t name(const char* name);\n")
header.write("\n")

open_struct = ""

with open(sys.argv[1]) as f:
  header.write("class hal_ctx_t{\n")
  header.write("  public:\n")
  header.write("    q8_24 period;\n")
  header.write("    uint32_t freq;\n")
  header.write("\n")
  for line in f:
    # print("parse: " + line)
    start, *tmp = line.rstrip("\n").split(",")
    if start == "//":
      header.write("    " + start + " ".join(tmp) + "\n")
    elif start == "":
      header.write("\n")
    elif start == "struct":
      if open_struct != "":
        header.write("    } " + open_struct + ";\n")
        open_struct = ""

      open_struct = tmp[0].strip(" ")
      header.write("    class" + tmp[0] + "_t {\n")
      header.write("      public:\n")
    
    elif start == "bit":
        type = "uint32_t"
        start, name, conf_name, default, fileds = line.rstrip("\n").split(",")
        name = name.strip(" ")
        conf_name = conf_name.strip(" ")
        default = default.strip(" ")
        header.write("      struct " + name + "_t {\n")
        for f in fileds.split(" "):
          if f != "":
            header.write("        " + type + " " + f + ": 1;\n")
        header.write("      } " + name + " = (" + name + "_t)" + default + ";\n")
        type = "hal_ctx_t::" + open_struct + "_t::" + name + "_t"
        tup = (type, open_struct + "." + name, conf_name)
        conf_objs.append(tup)

    else:
      start, name, conf_name, default, *values = line.rstrip("\n").split(",")
      name = name.strip(" ")
      conf_name = conf_name.strip(" ")
      default = default.strip(" ")
      type = ""
      if start == "bool":
        type = "uint32_t"
      elif start == "int8_t":
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
        tup = (type, open_struct + "." + name, conf_name)
        conf_objs.append(tup)
        header.write("      " + type + " " + name + " = " + default + "; // " + conf_name + "\n")

      if start == "enum":
        # print("enum ")
        # print(values)
        type = "hal_ctx_t::" + open_struct + "_t::" + name + "_t"
        tup = (type, open_struct + "." + name, conf_name)
        conf_objs.append(tup)
        header.write("      enum class " + name + "_t { // " + conf_name + "\n")
        for v in values[0].split(" "):
          if v != "":
          # header.write("    " + v + ",\n")
#          header.write("    " + v + " = name<4>(\"" + v + "\"),\n")
            header.write("        " + v + " = " + str(name_hash(v.rstrip("\n")[::-1], 4)) + ",\n")
        header.write("      } " + name + " = " + name + "_t::" + default + ";\n")
  if open_struct != "":
    header.write("    } " + open_struct + ";\n")
    open_struct = ""
  header.write("};\n")
header.close()

code.write("#include <fp_lib.hpp>\n")
code.write("#include <hal.hpp>\n")
code.write("#include <conf.hpp>\n")

code.write("void conf_comp::set(hal_ctx_t* ctx, const char* n, uint8_t len, conf_obj::conf_obj_type_t type, uint8_t crypted, void* data){\n")
code.write("  switch(name<6>(n)){\n")
for obj in conf_objs:
  code.write("    case name<6>(\"" + obj[2].rstrip("\n") + "\"): // " + obj[2] + "\n")
  # code.write("    case " + str(name_hash(obj[2].rstrip("\n"), 6)) + ": // " + obj[2] + "\n")
  code.write("      ctx->" + obj[1] + "= *((" + obj[0] + "*) data);\n")
  code.write("    break;\n")
code.write("  }\n")
code.write("}\n")

code.write("uint32_t conf_comp::get(hal_ctx_t* ctx, const char* n){\n")
code.write("  switch(name<6>(n)){\n")
for obj in conf_objs:
  code.write("    case name<6>(\"" + obj[2].rstrip("\n") + "\"): // " + obj[2] + "\n")
  # code.write("    case " + str(name_hash(obj[2].rstrip("\n"), 6)) + ": // " + obj[2] + "\n")
  code.write("      return(*(uint32_t*)&(ctx->" + obj[1] + "));\n")
  code.write("    break;\n")
code.write("  }\n")
code.write("}\n")

code.close()
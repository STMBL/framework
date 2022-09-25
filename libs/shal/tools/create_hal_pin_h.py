import sys

def name_hash(s, l):
  r = 0
  for i in range(0, l):
    if i < len(s):
      r <<= 8
      r += ord(s[i])
  return r

comps = {}
pin_count = 0
cmd_count = 0

q_types = ["Q8_24", "Q16_16", "Q24_8", "C8_24", "C16_16", "C24_8", "INT32_T", "UINT32_T", "ENUM", "FIFO"]

for file in sys.argv[2:]:
  print("\033[92m" + "parse file: " + file + "\033[0m")
  with open(file) as f: # TODO raise error if file cannot be opened instead of silently succeeding
    for line in f:
      if line.lstrip()[0:7] == "HAL_PIN":
        line = line.split("(")[1]
        line = line.lstrip().split(")")[0]

        type, name, *entries = line.split(" ")
        comp, pin = name.split(".")
        
        #print("  found: " + type + " " + comp + " " + pin)

        if not comp in comps:
          comps[comp] = {}

        type = (type + " " + " ".join(entries)).rstrip()

        if not pin in comps[comp]:
            comps[comp][pin] = type
            pin_count += 1
            if type[0:3] == "bit":
              pin_count += len(entries)

        else:
          if comps[comp][pin] != type:
            print("\033[91m" + "type mismatch: " + name + "\033[0m")

header = open(sys.argv[1], 'w')
header.write("#include \"fp_lib.hpp\"\n")
header.write("#include \"fifo.hpp\"\n")
header.write("#pragma once\n")
header.write("\n")
header.write("class hal_ctx_t {\n")
header.write("  public:\n")
header.write("    q8_24 period;\n")
header.write("    uint32_t freq;\n")
header.write("\n")

for comp_name, pins in comps.items():
  header.write("  class " + comp_name + "_t {\n")
  header.write("    public:\n")
  for pin_name, type in pins.items():      
    if type[0:3] == "bit":
      header.write("      struct " + pin_name + "_t {\n")
      for e in type.split(" ")[1:]:
        header.write("        uint32_t " + e + " : 1;\n")
      header.write("      } " + pin_name + " = (" + pin_name + "_t) 0;\n\n")
    
    elif type[0:4] == "enum":
      header.write("      enum class " + pin_name + "_t {\n")
      for e in type.split(" ")[1:]:
        header.write("        " + e + " = " + hex(name_hash(e.rstrip("\n"), 4)) + ",\n")
      header.write("      } " + pin_name + " = hal_ctx_t::" + comp_name + "_t::" + pin_name + "_t::" + type.split(" ")[1] +  ";\n\n")
    
    elif type[0:4] == "fifo":
      type, count = type.split(" ")
      header.write("      " + type + "_t<" + count + "> " + pin_name + ";\n")
    
    else:
      header.write("      " + type + " " + pin_name + " = 0;\n")

  header.write("  } " + comp_name + ";\n\n")

# header.write("  private:\n")
header.write("    const struct hal_pin_t{\n")
header.write("      const char * name;\n")
header.write("      void * ptr;\n")
header.write("      enum class type_t{\n")
for i in range(32):
    header.write("        B" + str(i) + " = " + str(i) + ",\n")
for t in q_types:
  header.write("        " + t + " = " + hex(name_hash(t, 4)) + ",\n")
header.write("        } type;\n")
header.write("    } hal_pins[" + str(pin_count) + "] = {\n")

for comp_name, pins in comps.items():
  for pin_name, type in pins.items():
    if type[0:4] == "enum":
      header.write("      {\"" + comp_name + "." + pin_name + "\", (void *)&(" + comp_name + "." + pin_name + "), hal_pin_t::type_t::ENUM},\n")
    elif type[0:3] == "bit":
      header.write("      {\"" + comp_name + "." + pin_name + "\", (void *)&(" + comp_name + "." + pin_name + "), hal_pin_t::type_t::UINT32_T},\n")
      for i, e in enumerate(type.split(" ")[1:]):
        header.write("      {\"" + comp_name + "." + pin_name + "." + e + "\", (void *)&(" + comp_name + "." + pin_name + "), hal_pin_t::type_t::B" + str(i) + "},\n")
    elif type[0:4] == "fifo":
      header.write("      {\"" + comp_name + "." + pin_name + "\", (void *)&(" + comp_name + "." + pin_name + "), hal_pin_t::type_t::" + type.split(" ")[0].upper() + "},\n")
    else:
      header.write("      {\"" + comp_name + "." + pin_name + "\", (void *)&(" + comp_name + "." + pin_name + "), hal_pin_t::type_t::" + type.upper() + "},\n")

    


header.write("    };\n\n")

header.write("    const struct hal_cmd_t{\n")
header.write("      const char * name;\n")
header.write("      void * ptr;\n")
header.write("    } hal_cmds[" + str(cmd_count) + "] = {\n")
header.write("    };\n\n")

header.write("};\n\n")

header.close()
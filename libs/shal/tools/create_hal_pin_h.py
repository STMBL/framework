import sys
import re

def name_hash(s, l):
  r = 0
  for i in range(0, l):
    if i < len(s):
      r <<= 8
      r += ord(s[i])
  return r

signal_pattern = "ctx->[a-zA-Z_]+[.][a-zA-Z_]+"

comps = {}
pin_count = 0
cmd_count = 0

q_types = ["Q0_32", "Q8_24", "Q16_16", "Q24_8", "C0_32", "C8_24", "C16_16", "C24_8", "INT32_T", "UINT32_T", "ENUM", "FIFO"]

for file in sys.argv[2:]:
  print("\033[92m" + "parse file: " + file + "\033[0m")
  api = []
  with open(file) as f: # TODO raise error if file cannot be opened instead of silently succeeding
    for line in f:
      if line.lstrip()[0:8] == "HAL_PIN(":
        line = line.lstrip()[8:].rstrip()

        signal = line.lstrip().split(")")[0]

        comment = ")".join(line.split(")")[1:])
        comment = comment.lstrip()

        if ">" in signal:
          one, two = signal.split(">")
          type, name, *entries = two.split(" ")
          type = one + ">"
          
        else:
          type, name, *entries = signal.split(" ")
        comp, pin = name.split(".")

        if name not in api:
          api.append(name)
        else:
          print("\033[31m" + "multiple definitions of signal: " + name + "\033[0m")
       
        if not comp in comps:
          comps[comp] = {}

        type = (type + " " + " ".join(entries)).rstrip()

        if not pin in comps[comp]:
            comps[comp][pin] = (type, comment)
            pin_count += 1
            if type[0:3] == "bit":
              pin_count += len(entries)

        else:
          if comps[comp][pin][0] != type:
            print("\033[31m" + "type mismatch: " + name + "\033[0m")
          if comps[comp][pin][1] == "":
            comps[comp][pin] = (type, comment) # update empty comment
          elif comment != "" and comps[comp][pin][1] != comment:
            print("\033[93m" + "comment mismatch: " + name + "\033[0m")
      for sig in re.findall(signal_pattern, line):
        if sig[5:] not in api:
          print("\033[93m" + "missing definition of signal: " + sig[5:] + "\033[0m")
          api.append(sig[5:])

header = open(sys.argv[1], 'w')
header.write("#include \"fp_lib.hpp\"\n")
header.write("#include \"units.hpp\"\n")
header.write("#include \"fifo.hpp\"\n")
header.write("#pragma once\n")
header.write("\n")
header.write("#define HAL_PIN(...)\n")
header.write("\n")
header.write("class hal_ctx_t {\n")
header.write("  public:\n")
header.write("    q8_24 period;\n")
header.write("    uint32_t freq;\n")
header.write("\n")

for comp_name, pins in comps.items():
  header.write("  class " + comp_name + "_t {\n")
  header.write("    public:\n")
  for pin_name, (type, comment) in pins.items():
    if type[0:3] == "bit":
      header.write("      struct " + pin_name + "_t {\n")
      for e in type.split(" ")[1:]:
        header.write("        uint32_t " + e + " : 1 = 0;  " + comment + "\n")
      if len(type.split(" ")[1:]) < 32:
        header.write("        uint32_t : " + str(32 - len(type.split(" ")[1:])) + ";\n")
      header.write("      } " + pin_name + ";\n\n")
    
    elif type[0:4] == "enum":
      header.write("      enum class " + pin_name + "_t {  " + comment + "\n")
      for e in type.split(" ")[1:]:
        header.write("        " + e + " = " + hex(name_hash(e.rstrip("\n"), 4)) + ",\n")
      header.write("      } " + pin_name + " = hal_ctx_t::" + comp_name + "_t::" + pin_name + "_t::" + type.split(" ")[1] +  ";\n\n")
    
    elif type[0:4] == "fifo":
      type, count = type.split(" ")
      header.write("      " + type + "_t<" + count + "> " + pin_name + ";  " + comment + "\n")
    
    else:
      header.write("      " + type + " " + pin_name + " = 0;  " + comment + "\n")

  header.write("  } " + comp_name + ";\n\n")

# header.write("  private:\n")
header.write("    const struct hal_pin_t{\n")
header.write("      const char * name;\n")
header.write("      const char * doc;\n")
header.write("      void * ptr;\n")
header.write("      enum class type_t{\n")
for i in range(32):
    header.write("        B" + str(i) + " = " + str(i) + ",\n")
for t in q_types:
  header.write("        " + t + " = " + hex(name_hash(t, 4)) + ",\n")
header.write("        } type;\n")
header.write("    } hal_pins[" + str(pin_count) + "] = {\n")

for comp_name, pins in comps.items():
  for pin_name, (type, comment) in pins.items():
    if len(type.split("<")) > 1 and len(type.split(">")) > 1:
      type = type.split("<")[1].split(">")[0].split(",")[0]
    
    if type[0:4] == "enum":
      header.write("      {\"" + comp_name + "." + pin_name + "\", \"" + comment + "\", (void *)&(" + comp_name + "." + pin_name + "), hal_pin_t::type_t::ENUM},\n")
    elif type[0:3] == "bit":
      header.write("      {\"" + comp_name + "." + pin_name + "\", \"" + comment + "\", (void *)&(" + comp_name + "." + pin_name + "), hal_pin_t::type_t::UINT32_T},\n")
      for i, e in enumerate(type.split(" ")[1:]):
        header.write("      {\"" + comp_name + "." + pin_name + "." + e + "\", \"" + comment + "\", (void *)&(" + comp_name + "." + pin_name + "), hal_pin_t::type_t::B" + str(i) + "},\n")
    elif type[0:4] == "fifo":
      header.write("      {\"" + comp_name + "." + pin_name + "\", \"" + comment + "\", (void *)&(" + comp_name + "." + pin_name + "), hal_pin_t::type_t::" + type.split(" ")[0].upper() + "},\n")
    else:
      header.write("      {\"" + comp_name + "." + pin_name + "\", \"" + comment + "\", (void *)&(" + comp_name + "." + pin_name + "), hal_pin_t::type_t::" + type.upper() + "},\n")

    


header.write("    };\n\n")

header.write("    const struct hal_cmd_t{\n")
header.write("      const char * name;\n")
header.write("      void * ptr;\n")
header.write("    } hal_cmds[" + str(cmd_count) + "] = {\n")
header.write("    };\n\n")

header.write("};\n\n")

header.close()

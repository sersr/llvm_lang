#ifndef PARSER_H
#define PARSER_H

#include "ast.h"
#include <iterator>
#include <regex>
#include <sstream>

class ModuleFile {
public:
  static ModuleFile parser(string str) {
    SmallVector<StringRef> lines;
    SplitString(str, lines);
    ModuleFile file;
    if (lines.empty())
      file;
    size_t cursor = 0, current = 0, maxLines = lines.size();

    auto mm = string("");
    raw_string_ostream rawStr(mm);

    while (current <= maxLines) {
      auto currentStr = lines[current];
      if (current == maxLines && cursor >= currentStr.size()) {
        break;
      }
      buffer_ostream os(rawStr);
    }
  };
  string name;
  vector<AssignStmt> globalValues;
  vector<Function> functions;
  vector<StructAst> structs;
};

#endif
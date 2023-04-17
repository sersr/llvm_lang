#ifndef AST_H
#define AST_H
#include "module.h"
#include "llvm/ADT/APFloat.h"
#include "llvm/IR/BasicBlock.h"
#include "llvm/IR/Constants.h"
#include "llvm/IR/DerivedTypes.h"
#include "llvm/IR/IRBuilder.h"
#include "llvm/IR/Type.h"

using namespace llvm;
using namespace std;

enum LLType {
  None,
  Double,
  Int,
  String,
};

Type *getType(LLVMContext *context, LLType type);

class Pos {
public:
  Pos(int start, int length, int column)
      : start(start), length(length), column(column) {}
  int start;
  int length;
  int column;
};
class Stmt {};
class Expr : public Stmt {
public:
  Expr(Pos pos) : pos(pos) {}
  Pos pos;
};

class DoubleVar : public Expr {
public:
  DoubleVar(Pos pos, double value) : Expr(pos), value(value) {}
  double value;
};

class AssignStmt : public Stmt {
public:
  bool mut;
  string name;
  LLType type;
  Expr stmt;
};

class Block : public Stmt {
public:
  vector<Stmt> stmts;
};

class Ast {

public:
  virtual Value *codegen(KModule &module) = 0;
};

class Param {
public:
  Param(string name, LLType type) : name(name), type(type) {}
  string name;
  LLType type;

  Type *getType(LLVMContext *context) { return ::getType(context, type); }
};

class FunctionAst : public Ast {
public:
  FunctionAst(Pos pos, string name, LLType returnType, vector<Param> &params);
  Value *codegen(KModule &module) override;

private:
  Pos pos;
  string name;
  LLType returnType;
  vector<Param> params;
};

class StructFieldAst : public Ast {
  public:
  Pos pos;
  string name;
  LLType type;
};

class StructAst : public Ast {
  public:
  Pos pos;
  string name;
  vector<StructFieldAst> fields;
  vector<FunctionAst> functions;
};
#endif
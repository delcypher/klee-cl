#include "klee/util/ExprCPrinter.h"
#include <sstream>

using namespace klee;

ExprCPrinter::ExprBinding ExprCPrinter::getExprBinding(ref<Expr> e) {
  std::map<ref<Expr>, ExprBinding>::iterator it = bindings.find(e);
  if (it != bindings.end())
    return it->second;

  return bindExpr(e);
}

ExprCPrinter::CType ExprCPrinter::getUIntType(unsigned w) {
  if (w <= 8)
    return UInt8;
  if (w <= 16)
    return UInt16;
  if (w <= 32)
    return UInt32;
  if (w <= 64)
    return UInt64;
  assert(0 && "expr too wide");
  return UInt64;
}

ExprCPrinter::CType ExprCPrinter::getSIntType(unsigned w) {
  if (w <= 8)
    return Int8;
  if (w <= 16)
    return Int16;
  if (w <= 32)
    return Int32;
  if (w <= 64)
    return Int64;
  assert(0 && "expr too wide");
  return Int64;
}

ExprCPrinter::CType ExprCPrinter::getFloatType(unsigned w) {
  if (w == 32)
    return Float;
  if (w == 64)
    return Double;
  assert(0 && "bad width for floating expr");
  return Double;
}

void ExprCPrinter::printExpr(std::ostream &out, CType &ty, ref<Expr> e) {
  Expr &ep = *e.get();

  switch (ep.getKind()) {
    case Expr::NotOptimized: printNotOptimized(out, ty, static_cast<NotOptimizedExpr&>(ep)); break;
    case Expr::Constant: printConstant(out, ty, static_cast<ConstantExpr&>(ep)); break;
    case Expr::Read: printRead(out, ty, static_cast<ReadExpr&>(ep)); break;
    case Expr::Select: printSelect(out, ty, static_cast<SelectExpr&>(ep)); break;
    case Expr::Concat: printConcat(out, ty, static_cast<ConcatExpr&>(ep)); break;
    case Expr::Extract: printExtract(out, ty, static_cast<ExtractExpr&>(ep)); break;
    case Expr::ZExt: printZExt(out, ty, static_cast<ZExtExpr&>(ep)); break;
    case Expr::SExt: printSExt(out, ty, static_cast<SExtExpr&>(ep)); break;
    case Expr::FPExt: printFPExt(out, ty, static_cast<FPExtExpr&>(ep)); break;
    case Expr::FPTrunc: printFPTrunc(out, ty, static_cast<FPTruncExpr&>(ep)); break;
    case Expr::UIToFP: printUIToFP(out, ty, static_cast<UIToFPExpr&>(ep)); break;
    case Expr::SIToFP: printSIToFP(out, ty, static_cast<SIToFPExpr&>(ep)); break;
    case Expr::FPToUI: printFPToUI(out, ty, static_cast<FPToUIExpr&>(ep)); break;
    case Expr::FPToSI: printFPToSI(out, ty, static_cast<FPToSIExpr&>(ep)); break;
    case Expr::FOrd1: printFOrd1(out, ty, static_cast<FOrd1Expr&>(ep)); break;
    case Expr::FSqrt: printFSqrt(out, ty, static_cast<FSqrtExpr&>(ep)); break;
    case Expr::FCos: printFCos(out, ty, static_cast<FCosExpr&>(ep)); break;
    case Expr::FSin: printFSin(out, ty, static_cast<FSinExpr&>(ep)); break;
    case Expr::Add: printAdd(out, ty, static_cast<AddExpr&>(ep)); break;
    case Expr::Sub: printSub(out, ty, static_cast<SubExpr&>(ep)); break;
    case Expr::Mul: printMul(out, ty, static_cast<MulExpr&>(ep)); break;
    case Expr::UDiv: printUDiv(out, ty, static_cast<UDivExpr&>(ep)); break;
    case Expr::SDiv: printSDiv(out, ty, static_cast<SDivExpr&>(ep)); break;
    case Expr::URem: printURem(out, ty, static_cast<URemExpr&>(ep)); break;
    case Expr::SRem: printSRem(out, ty, static_cast<SRemExpr&>(ep)); break;
    case Expr::FAdd: printFAdd(out, ty, static_cast<FAddExpr&>(ep)); break;
    case Expr::FSub: printFSub(out, ty, static_cast<FSubExpr&>(ep)); break;
    case Expr::FMul: printFMul(out, ty, static_cast<FMulExpr&>(ep)); break;
    case Expr::FDiv: printFDiv(out, ty, static_cast<FDivExpr&>(ep)); break;
    case Expr::FRem: printFRem(out, ty, static_cast<FRemExpr&>(ep)); break;
    case Expr::Not: printNot(out, ty, static_cast<NotExpr&>(ep)); break;
    case Expr::And: printAnd(out, ty, static_cast<AndExpr&>(ep)); break;
    case Expr::Or: printOr(out, ty, static_cast<OrExpr&>(ep)); break;
    case Expr::Xor: printXor(out, ty, static_cast<XorExpr&>(ep)); break;
    case Expr::Shl: printShl(out, ty, static_cast<ShlExpr&>(ep)); break;
    case Expr::LShr: printLShr(out, ty, static_cast<LShrExpr&>(ep)); break;
    case Expr::AShr: printAShr(out, ty, static_cast<AShrExpr&>(ep)); break;
    case Expr::Eq: printEq(out, ty, static_cast<EqExpr&>(ep)); break;
    case Expr::Ne: printNe(out, ty, static_cast<NeExpr&>(ep)); break;
    case Expr::Ult: printUlt(out, ty, static_cast<UltExpr&>(ep)); break;
    case Expr::Ule: printUle(out, ty, static_cast<UleExpr&>(ep)); break;
    case Expr::Ugt: printUgt(out, ty, static_cast<UgtExpr&>(ep)); break;
    case Expr::Uge: printUge(out, ty, static_cast<UgeExpr&>(ep)); break;
    case Expr::Slt: printSlt(out, ty, static_cast<SltExpr&>(ep)); break;
    case Expr::Sle: printSle(out, ty, static_cast<SleExpr&>(ep)); break;
    case Expr::Sgt: printSgt(out, ty, static_cast<SgtExpr&>(ep)); break;
    case Expr::Sge: printSge(out, ty, static_cast<SgeExpr&>(ep)); break;
    case Expr::FCmp: printFCmp(out, ty, static_cast<FCmpExpr&>(ep)); break;
    case Expr::InvalidKind: ;
  }
  assert(0 && "invalid expression kind");
}

void ExprCPrinter::printNotOptimized(std::ostream &out, CType &ty, NotOptimizedExpr &e) {
  printExpr(out, ty, e.src);
}

void ExprCPrinter::printConstant(std::ostream &out, CType &ty, ConstantExpr &e) {
  ty = getUIntType(e.getWidth());
  printConstantExpr(out, ty, &e);
}

void ExprCPrinter::printRead(std::ostream &out, CType &ty, ReadExpr &e) {
  // TODO: use the update list
  const std::string &arrName = e.updates.root->name;
  parmDecls.insert(arrName);

  out << arrName << "[";
  printUIntSubExpr(out, e.index);
  out << "]";

  ty = UInt8;
}

void ExprCPrinter::printSelect(std::ostream &out, CType &ty, SelectExpr &e) {
  printUIntSubExpr(out, e.cond);
  out << " ? ";
  printAnyTySubExpr(out, ty, e.trueExpr);
  out << " : ";
  printSubExpr(out, ty, e.falseExpr);
}

/// Print the given expression. The higher order bits of the result are
/// guaranteed to be zero extended (as opposed to the general case, where
/// they are undefined).
void ExprCPrinter::printZExtSubExpr(std::ostream &out, ref<Expr> e) {
  if (e->getWidth() == 1) {
    out << "(";
    printUIntSubExpr(out, e);
    out << " & 1U)";
  } else if (e->getWidth() == 8 || e->getWidth() == 16 ||
             e->getWidth() == 32 || e->getWidth() == 64) {
    printUIntSubExpr(out, e);
  } else {
    assert(0 && "weird bitwidth not handled yet");
  }
}

/// Print the given expression. The higher order bits of the result are
/// guaranteed to be sign extended (as opposed to the general case, where
/// they are undefined).
void ExprCPrinter::printSExtSubExpr(std::ostream &out, ref<Expr> e) {
  if (e->getWidth() == 1) {
    out << "-(";
    printSIntSubExpr(out, e);
    out << " & 1)";
  } else if (e->getWidth() == 8 || e->getWidth() == 16 ||
             e->getWidth() == 32 || e->getWidth() == 64) {
    printSIntSubExpr(out, e);
  } else {
    assert(0 && "weird bitwidth not handled yet");
  }
}

void ExprCPrinter::printConcat(std::ostream &out, CType &ty, ConcatExpr &e) {
  ty = getUIntType(e.getWidth());
  out << "(";
  printSubExpr(out, ty, e.getLeft());
  out << " << " << e.getRight()->getWidth() << ") | (";
  printSubExpr(out, ty, e.getRight());
  out << " & " << ((1ULL << e.getRight()->getWidth()) - 1) << "ULL)";
}

void ExprCPrinter::printExtract(std::ostream &out, CType &ty, ExtractExpr &e) {
  printUIntSubExpr(out, e.expr);
  out << " >> " << e.offset;
}

void ExprCPrinter::printZExt(std::ostream &out, CType &ty, ZExtExpr &e) {
  ty = getUIntType(e.getWidth());
  printZExtSubExpr(out, e.src);
}

void ExprCPrinter::printSExt(std::ostream &out, CType &ty, SExtExpr &e) {
  ty = getSIntType(e.getWidth());
  printSExtSubExpr(out, e.src);
}

void ExprCPrinter::printFPExt(std::ostream &out, CType &ty, FPExtExpr &e) {
  ty = getFloatType(e.getWidth());
  printSubExpr(out, ty, e.src);
}

void ExprCPrinter::printFPTrunc(std::ostream &out, CType &ty, FPTruncExpr &e) {
  ty = getFloatType(e.getWidth());
  printSubExpr(out, ty, e.src);
}

void ExprCPrinter::printUIToFP(std::ostream &out, CType &ty, UIToFPExpr &e) {
  ty = getFloatType(e.getWidth());
  printUIntSubExpr(out, e.src);
}

void ExprCPrinter::printSIToFP(std::ostream &out, CType &ty, SIToFPExpr &e) {
  ty = getFloatType(e.getWidth());
  printSIntSubExpr(out, e.src);
}

void ExprCPrinter::printFPToUI(std::ostream &out, CType &ty, FPToUIExpr &e) {
  ty = getUIntType(e.getWidth());
  printFloatSubExpr(out, e.src);
}

void ExprCPrinter::printFPToSI(std::ostream &out, CType &ty, FPToSIExpr &e) {
  ty = getSIntType(e.getWidth());
  printFloatSubExpr(out, e.src);
}

void ExprCPrinter::printFOrd1(std::ostream &out, CType &ty, FOrd1Expr &e) {
  ty = getUIntType(e.getWidth());
  printFloatSubExpr(out, e.src);
  out << " == ";
  printFloatSubExpr(out, e.src);
}

void ExprCPrinter::printFUnaryMath(std::ostream &out, CType &ty, FUnaryExpr &e,
                                const char *floatName, const char *doubleName) {
  ty = getFloatType(e.getWidth());
  switch (ty) {
  case Float:  out << floatName;  break;
  case Double: out << doubleName; break;
  default:     assert(0 && "floating type expected");
  }
  out << "(";
  printSubExpr(out, ty, e.src);
  out << ")";
}

void ExprCPrinter::printFSqrt(std::ostream &out, CType &ty, FSqrtExpr &e) {
  printFUnaryMath(out, ty, e, "sqrtf", "sqrt");
}

void ExprCPrinter::printFCos(std::ostream &out, CType &ty, FCosExpr &e) {
  printFUnaryMath(out, ty, e, "cosf", "cos");
}

void ExprCPrinter::printFSin(std::ostream &out, CType &ty, FSinExpr &e) {
  printFUnaryMath(out, ty, e, "sinf", "sin");
}

void ExprCPrinter::printAdd(std::ostream &out, CType &ty, AddExpr &e) {
  ty = getUIntType(e.getWidth());
  printSubExpr(out, ty, e.left);
  out << " + ";
  printSubExpr(out, ty, e.right);
}

void ExprCPrinter::printSub(std::ostream &out, CType &ty, SubExpr &e) {
  ty = getUIntType(e.getWidth());
  printSubExpr(out, ty, e.left);
  out << " - ";
  printSubExpr(out, ty, e.right);
}

void ExprCPrinter::printMul(std::ostream &out, CType &ty, MulExpr &e) {
  ty = getUIntType(e.getWidth());
  printSubExpr(out, ty, e.left);
  out << " * ";
  printSubExpr(out, ty, e.right);
}

void ExprCPrinter::printUDiv(std::ostream &out, CType &ty, UDivExpr &e) {
  ty = getUIntType(e.getWidth());  
  printZExtSubExpr(out, e.left);
  out << " / ";
  printZExtSubExpr(out, e.right);
}

void ExprCPrinter::printSDiv(std::ostream &out, CType &ty, SDivExpr &e) {
  ty = getSIntType(e.getWidth());  
  printSExtSubExpr(out, e.left);
  out << " / ";
  printSExtSubExpr(out, e.right);
}

void ExprCPrinter::printURem(std::ostream &out, CType &ty, URemExpr &e) {
  ty = getUIntType(e.getWidth());  
  printZExtSubExpr(out, e.left);
  out << " % ";
  printZExtSubExpr(out, e.right);
}

void ExprCPrinter::printSRem(std::ostream &out, CType &ty, SRemExpr &e) {
  ty = getSIntType(e.getWidth());  
  printSExtSubExpr(out, e.left);
  out << " % ";
  printSExtSubExpr(out, e.right);
}

void ExprCPrinter::printFAdd(std::ostream &out, CType &ty, FAddExpr &e) {
  ty = getFloatType(e.getWidth());
  printSubExpr(out, ty, e.left);
  out << " + ";
  printSubExpr(out, ty, e.right);
}

void ExprCPrinter::printFSub(std::ostream &out, CType &ty, FSubExpr &e) {
  ty = getFloatType(e.getWidth());
  printSubExpr(out, ty, e.left);
  out << " - ";
  printSubExpr(out, ty, e.right);
}

void ExprCPrinter::printFMul(std::ostream &out, CType &ty, FMulExpr &e) {
  ty = getFloatType(e.getWidth());
  printSubExpr(out, ty, e.left);
  out << " * ";
  printSubExpr(out, ty, e.right);
}

void ExprCPrinter::printFDiv(std::ostream &out, CType &ty, FDivExpr &e) {
  ty = getFloatType(e.getWidth());
  printSubExpr(out, ty, e.left);
  out << " / ";
  printSubExpr(out, ty, e.right);
}

void ExprCPrinter::printFRem(std::ostream &out, CType &ty, FRemExpr &e) {
  ty = getFloatType(e.getWidth());
  printSubExpr(out, ty, e.left);
  out << " % ";
  printSubExpr(out, ty, e.right);
}

void ExprCPrinter::printNot(std::ostream &out, CType &ty, NotExpr &e) {
  ty = getUIntType(e.getWidth());
  out << "~";
  printSubExpr(out, ty, e.expr);
}

void ExprCPrinter::printAnd(std::ostream &out, CType &ty, AndExpr &e) {
  ty = getUIntType(e.getWidth());
  printSubExpr(out, ty, e.left);
  out << " & ";
  printSubExpr(out, ty, e.right);
}

void ExprCPrinter::printOr(std::ostream &out, CType &ty, OrExpr &e) {
  ty = getUIntType(e.getWidth());
  printSubExpr(out, ty, e.left);
  out << " | ";
  printSubExpr(out, ty, e.right);
}

void ExprCPrinter::printXor(std::ostream &out, CType &ty, XorExpr &e) {
  ty = getUIntType(e.getWidth());
  printSubExpr(out, ty, e.left);
  out << " ^ ";
  printSubExpr(out, ty, e.right);
}

void ExprCPrinter::printShl(std::ostream &out, CType &ty, ShlExpr &e) {
  ty = getUIntType(e.getWidth());
  printSubExpr(out, ty, e.left);
  out << " << ";
  printSubExpr(out, ty, e.right);
}

void ExprCPrinter::printLShr(std::ostream &out, CType &ty, LShrExpr &e) {
  ty = getUIntType(e.getWidth());
  printZExtSubExpr(out, e.left);
  out << " >> ";
  printSubExpr(out, ty, e.right);
}

void ExprCPrinter::printAShr(std::ostream &out, CType &ty, AShrExpr &e) {
  ty = getSIntType(e.getWidth());
  printSExtSubExpr(out, e.left);
  out << " >> ";
  printUIntSubExpr(out, e.right);
}

void ExprCPrinter::printEq(std::ostream &out, CType &ty, EqExpr &e) {
  ty = UInt8;
  printZExtSubExpr(out, e.left);
  out << " == ";
  printZExtSubExpr(out, e.right);
}

void ExprCPrinter::printNe(std::ostream &out, CType &ty, NeExpr &e) {
  ty = UInt8;
  printZExtSubExpr(out, e.left);
  out << " != ";
  printZExtSubExpr(out, e.right);
}

void ExprCPrinter::printUlt(std::ostream &out, CType &ty, UltExpr &e) {
  ty = UInt8;
  printZExtSubExpr(out, e.left);
  out << " < ";
  printZExtSubExpr(out, e.right);
}

void ExprCPrinter::printUle(std::ostream &out, CType &ty, UleExpr &e) {
  ty = UInt8;
  printZExtSubExpr(out, e.left);
  out << " <= ";
  printZExtSubExpr(out, e.right);
}

void ExprCPrinter::printUgt(std::ostream &out, CType &ty, UgtExpr &e) {
  ty = UInt8;
  printZExtSubExpr(out, e.left);
  out << " > ";
  printZExtSubExpr(out, e.right);
}

void ExprCPrinter::printUge(std::ostream &out, CType &ty, UgeExpr &e) {
  ty = UInt8;
  printZExtSubExpr(out, e.left);
  out << " >= ";
  printZExtSubExpr(out, e.right);
}

void ExprCPrinter::printSlt(std::ostream &out, CType &ty, SltExpr &e) {
  ty = UInt8;
  printSExtSubExpr(out, e.left);
  out << " < ";
  printSExtSubExpr(out, e.right);
}

void ExprCPrinter::printSle(std::ostream &out, CType &ty, SleExpr &e) {
  ty = UInt8;
  printSExtSubExpr(out, e.left);
  out << " <= ";
  printSExtSubExpr(out, e.right);
}

void ExprCPrinter::printSgt(std::ostream &out, CType &ty, SgtExpr &e) {
  ty = UInt8;
  printSExtSubExpr(out, e.left);
  out << " > ";
  printSExtSubExpr(out, e.right);
}

void ExprCPrinter::printSge(std::ostream &out, CType &ty, SgeExpr &e) {
  ty = UInt8;
  printSExtSubExpr(out, e.left);
  out << " > ";
  printSExtSubExpr(out, e.right);
}

void ExprCPrinter::printFCmp(std::ostream &out, CType &ty, FCmpExpr &e) {
  if (e.getPredicate() >= FCmpExpr::UNO)
    out << "!(";
  switch (e.getPredicate()) {
  case FCmpExpr::FALSE:
  case FCmpExpr::TRUE:
    out << "0";
    break;
  case FCmpExpr::OEQ:
  case FCmpExpr::UNE:
    printFloatSubExpr(out, e.left);
    out << " == ";
    printFloatSubExpr(out, e.right);
    break;
  case FCmpExpr::OGT:
  case FCmpExpr::ULE:
    printFloatSubExpr(out, e.left);
    out << " > ";
    printFloatSubExpr(out, e.right);
    break;
  case FCmpExpr::OGE:
  case FCmpExpr::ULT:
    printFloatSubExpr(out, e.left);
    out << " >= ";
    printFloatSubExpr(out, e.right);
    break;
  case FCmpExpr::OLT:
  case FCmpExpr::UGE:
    printFloatSubExpr(out, e.left);
    out << " < ";
    printFloatSubExpr(out, e.right);
    break;
  case FCmpExpr::OLE:
  case FCmpExpr::UGT:
    printFloatSubExpr(out, e.left);
    out << " <= ";
    printFloatSubExpr(out, e.right);
    break;
  case FCmpExpr::ONE:
  case FCmpExpr::UEQ:
    printFloatSubExpr(out, e.left);
    out << " < ";
    printFloatSubExpr(out, e.right);
    out << " | ";
    printFloatSubExpr(out, e.left);
    out << " > ";
    printFloatSubExpr(out, e.right);
    break;
  case FCmpExpr::ORD:
  case FCmpExpr::UNO:
    printFloatSubExpr(out, e.left);
    out << " < ";
    printFloatSubExpr(out, e.right);
    out << " | ";
    printFloatSubExpr(out, e.left);
    out << " >= ";
    printFloatSubExpr(out, e.right);
    break;
  }
  if (e.getPredicate() >= FCmpExpr::UNO)
    out << ")";
}

void ExprCPrinter::printBindingRef(std::ostream &out, unsigned binding) {
  out << "bindings->n" << binding;
}

void ExprCPrinter::printAnyTySubExpr(std::ostream &out, CType &ty, ref<Expr> e){
  ExprBinding binding = getExprBinding(e);
  ty = binding.first;
  printBindingRef(out, binding.second);
}

void ExprCPrinter::printSubExpr(std::ostream &out, CType ty, ref<Expr> e) {
  ExprBinding binding = getExprBinding(e);
  if (binding.first == ty) {
    printBindingRef(out, binding.second);
    return;
  }

  if ((ty >= FirstInt && ty <= LastInt && binding.first >= FirstFloat && binding.first <= LastFloat) ||
      (ty >= FirstFloat && ty <= LastFloat && binding.first >= FirstInt && binding.first <= LastInt)) {
    out << "(*(" << getTypeName(ty) << " *) &";
    printBindingRef(out, binding.second);
    out << ")";
  } else {
    out << "((" << getTypeName(ty) << ") ";
    printBindingRef(out, binding.second);
    out << ")";
  }
}

void ExprCPrinter::printUIntSubExpr(std::ostream &out, ref<Expr> e) {
  printSubExpr(out, getUIntType(e->getWidth()), e);
}

void ExprCPrinter::printSIntSubExpr(std::ostream &out, ref<Expr> e) {
  printSubExpr(out, getSIntType(e->getWidth()), e);
}

void ExprCPrinter::printFloatSubExpr(std::ostream &out, ref<Expr> e) {
  printSubExpr(out, getFloatType(e->getWidth()), e);
}

const char *ExprCPrinter::getTypeName(CType ty) {
  switch (ty) {
  case Int8:   return "int8_t";
  case Int16:  return "int16_t";
  case Int32:  return "int32_t";
  case Int64:  return "int64_t";
  case UInt8:  return "uint8_t";
  case UInt16: return "uint16_t";
  case UInt32: return "uint32_t";
  case UInt64: return "uint64_t";
  case Float:  return "float";
  case Double: return "double";
  default:     assert(0 && "invalid ctype"); return 0;
  }
}

void ExprCPrinter::printConstantExpr(std::ostream &out, CType ty,
                                     ConstantExpr *ce) {
  if (ty != Float && ty != Double)
    out << "((" << getTypeName(ty) << ")";

  switch (ty) {
  case Int8:
  case Int16:
    out << ce->getAPValue().getSExtValue(); break;
  case Int32:
    out << ce->getAPValue().getSExtValue() << "L"; break;
  case Int64:
    out << ce->getAPValue().getSExtValue() << "LL"; break;
  case UInt8:
  case UInt16:
    out << ce->getZExtValue() << "U"; break;
  case UInt32:
    out << ce->getZExtValue() << "UL"; break;
  case UInt64:
    out << ce->getZExtValue() << "ULL"; break;
  case Float:
    out << ce->getAPFloatValue(false).convertToFloat() << "f"; break;
  case Double:
    out << ce->getAPFloatValue(false).convertToDouble(); break;
  }

  if (ty != Float && ty != Double)
    out << ")";
}

ExprCPrinter::ExprBinding ExprCPrinter::bindExpr(ref<Expr> e) {
  unsigned bindingNo = bindings.size();
  ExprBinding &binding = bindings[e];
  binding.second = bindingNo;

  std::ostringstream out;
  out << "  binding->n" << bindingNo << " = ";
  printExpr(out, binding.first, e);
  out << ";\n";
  fnOut << out.str();

  structOut << "  " << getTypeName(binding.first) << " n" << bindingNo << ";\n";

  return binding;
}

void ExprCPrinter::printExprEvaluator(std::ostream &out, ref<Expr> e) {
  std::ostringstream structOut, fnOut;
  ExprCPrinter cp(structOut, fnOut);

  ExprBinding retBinding = cp.getExprBinding(e);
  out << "struct CPbinding {\n";
  out << structOut.str();
  out << "};\n\n";

  out << getTypeName(retBinding.first) << " CPeval(struct CPbinding *bindings";
  for (llvm::StringSet<>::iterator i = cp.parmDecls.begin(),
       e = cp.parmDecls.end(); i != e; ++i) {
    out << ", char *" << i->first();
  }
  out << ") {\n";
  out << fnOut.str();
  out << "  return ";
  cp.printBindingRef(out, retBinding.second);
  out << ";\n}" << std::endl;
}
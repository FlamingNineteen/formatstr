#include "fmtstr.hpp"

int main() {
    std::cout << ("FINAL:" + fmtstr::fmtstr("hello {'big {'big {'big'}'}'} world")) << std::endl;
    std::cout << ("FINAL:" + fmtstr::fmtstr("{'six'+'7'+'eight}")) << std::endl;
    std::cout << ("FINAL:" + fmtstr::fmtstr("{10+9-47}")) << std::endl;
    // std::cout << ("FINAL:" + fmtstr::fmtstr("{0x38C5}")) << std::endl;
    std::cout << ("FINAL:" + fmtstr::fmtstr("{9+10*9}")) << std::endl;
    std::cout << ("FINAL:" + fmtstr::fmtstr("{(9==9)!=(10==6)}")) << std::endl;
    std::cout << ("FINAL:" + fmtstr::fmtstr("{32<<1}")) << std::endl;
    std::cout << ("FINAL:" + fmtstr::fmtstr("{true?63:'balright'}")) << std::endl;
    std::cout << ("FINAL:" + fmtstr::fmtstr("{(1-1==1)?6+3:'bal'+\"right\"}")) << std::endl;
    return 0;
}

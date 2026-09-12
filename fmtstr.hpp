#include <iostream>
#include <string>
#include "json.hpp"

using json = nlohmann::json;

namespace fmtstr {
    void debug(std::string s) {
        std::cout << s << std::endl;
    }

    std::string ReadUntil(std::string fstr, int &i, std::string strcheck = "") {
        std::string result = "";

        while (
            i < fstr.size() && (
                (strcheck == "isdigit" ? std::isdigit(fstr[i]) : false) ||
                (strcheck == "ishex" ? std::isdigit(fstr[i]) || std::string("ABCDEF").find(fstr[i]) != std::string::npos : false) ||
                (strcheck == "isspace" ? std::isspace(fstr[i]) : false) ||
                (strcheck == "isoperator" ? std::string("+-*/%<=>!&|^?:").find(fstr[i]) != std::string::npos : false)
            )
        ) {
            result += fstr[i];
            i++;
        }

        return result;
    }

    std::string ReadUntil(std::string fstr, int &i, char stop) {
        debug("read " + fstr);

        std::string result = "";
        int indent = 0;
        i++;

        while (i < fstr.size() && (fstr[i] != stop || indent > 0)) {
            if (fstr[i] == '}' || fstr[i] == ')') indent--;
            if (fstr[i] == '{' || fstr[i] == '(') indent++;
            result += fstr[i];
            i++;
        }

        return result;
    }

    std::string ReadSetTimes(std::string fstr, int &i, int end) {
        debug("read " + fstr);

        std::string result = "";
        end += i;

        for (int j = 0; j < fstr.size() && j < end; j++) {
            result += fstr[i];
            i++;
        }

        i--;
        
        return result;
    }

    int HexToDec(std::string hex) {
        int result = 0;
        int deg = 1;

        for (int i = hex.size() - 1; i >= 0; i--) {
            result += deg * (hex[i] < 'A' ? hex[i] - '0' : hex[i] - 'A' + 10);
            deg*=16;
        }

        return result;
    }

    std::string fmtstr(std::string fstr);
    json Braces(std::string fstr);

    json Braces(std::string fstr) {
        debug("braces | " + fstr);
        json result;
        std::string read = "";
        int size = fstr.size();
        char let;

        json oplist = {};

        for (int i = 0; i < size; i++) {
            ReadUntil(fstr, i, "isspace");
            if (i >= size) break;

            let = fstr[i];

            if (oplist.size() % 2 == 0) {
                if (let == '(') oplist += Braces(ReadUntil(fstr, i, ')'))[0];

                if (let == '\'' || let == '\"') oplist += fmtstr(ReadUntil(fstr, i, let));

                if (std::isdigit(let)) {
                    if (let == '0' && i+2 < fstr.size()) {
                        if (fstr[i+1] == 'x') {
                            i+=2;
                            oplist += HexToDec(ReadUntil(fstr, i, "ishex"));
                        }
                        else oplist += std::stoi(ReadUntil(fstr, i, "isdigit"));
                    }
                    else oplist += std::stoi(ReadUntil(fstr, i, "isdigit"));
                    i--;
                }

                if (let == 't') {
                    if (ReadSetTimes(fstr, i, 4) == "true") {
                        oplist += true;
                    }
                    else throw std::runtime_error("Expected true, got " + ReadSetTimes(fstr, i, 4));
                }

                if (let == 'f') {
                    if (ReadSetTimes(fstr, i, 5) == "false") {
                        oplist += false;
                    }
                    else throw std::runtime_error("Expected falalse, got " + ReadSetTimes(fstr, i, 5));
                }

            }
            else {
                oplist += ReadUntil(fstr, i, "isoperator");
                i--;
            }
        }

        debug(oplist.dump());

        for (int op = 0; oplist.size() > 1 && op < 11; op++) {
            for (int i = 1; oplist.size() > 1 && i < oplist.size(); i+=2) {

                // Operator Precedance 0

                if (oplist[i] == "*" && op >= 0) {
                    if (
                        oplist[i-1].is_number_integer() && oplist[i+1].is_number_integer() ||(
                            oplist[i-1].is_number_integer() && oplist[i+1].is_boolean() ||
                            oplist[i-1].is_boolean() && oplist[i+1].is_number_integer()
                        )
                    ) {
                        oplist[i] = (int)(oplist[i-1]) * (int)(oplist[i+1]);
                    }
                    else if (oplist[i-1].is_boolean() && oplist[i+1].is_boolean()) {
                        oplist[i] = (bool)(oplist[i-1]) * (bool)(oplist[i+1]);
                    }
                    else throw std::runtime_error("Types differ");
                    oplist.erase(i+1);
                    oplist.erase(i-1);
                    i-=2;
                }

                else if (oplist[i] == "/" && op >= 0) {
                    if (
                        oplist[i-1].is_number_integer() && oplist[i+1].is_number_integer() ||(
                            oplist[i-1].is_number_integer() && oplist[i+1].is_boolean() ||
                            oplist[i-1].is_boolean() && oplist[i+1].is_number_integer()
                        )
                    ) {
                        oplist[i] = (int)(oplist[i-1]) / (int)(oplist[i+1]);
                    }
                    else if (oplist[i-1].is_boolean() && oplist[i+1].is_boolean()) {
                        oplist[i] = (bool)(oplist[i-1]) / (bool)(oplist[i+1]);
                    }
                    else throw std::runtime_error("Types differ");
                    oplist.erase(i+1);
                    oplist.erase(i-1);
                    i-=2;
                }

                else if (oplist[i] == "%" && op >= 0) {
                    if (
                        oplist[i-1].is_number_integer() && oplist[i+1].is_number_integer() ||(
                            oplist[i-1].is_number_integer() && oplist[i+1].is_boolean() ||
                            oplist[i-1].is_boolean() && oplist[i+1].is_number_integer()
                        )
                    ) {
                        oplist[i] = (int)(oplist[i-1]) % (int)(oplist[i+1]);
                    }
                    else if (oplist[i-1].is_boolean() && oplist[i+1].is_boolean()) {
                        oplist[i] = (bool)(oplist[i-1]) % (bool)(oplist[i+1]);
                    }
                    else throw std::runtime_error("Types differ");
                    oplist.erase(i+1);
                    oplist.erase(i-1);
                    i-=2;
                }

                // Operator Precedance 1

                else if (oplist[i] == "+" && op >= 1) {
                    if (
                        oplist[i-1].is_number_integer() && oplist[i+1].is_number_integer() ||(
                            oplist[i-1].is_number_integer() && oplist[i+1].is_boolean() ||
                            oplist[i-1].is_boolean() && oplist[i+1].is_number_integer()
                        )
                    ) {
                        oplist[i] = (int)(oplist[i-1]) + (int)(oplist[i+1]);
                    }
                    else if (oplist[i-1].is_boolean() && oplist[i+1].is_boolean()) {
                        oplist[i] = (bool)(oplist[i-1]) + (bool)(oplist[i+1]);
                    }
                    else if (oplist[i-1].is_string() && oplist[i+1].is_string()) {
                        oplist[i] = std::string(oplist[i-1]) + std::string(oplist[i+1]);
                    }
                    else throw std::runtime_error("Types differ");
                    oplist.erase(i+1);
                    oplist.erase(i-1);
                    i-=2;
                }

                else if (oplist[i] == "-" && op >= 1) {
                    if (
                        oplist[i-1].is_number_integer() && oplist[i+1].is_number_integer() ||(
                            oplist[i-1].is_number_integer() && oplist[i+1].is_boolean() ||
                            oplist[i-1].is_boolean() && oplist[i+1].is_number_integer()
                        )
                    ) {
                        oplist[i] = (int)(oplist[i-1]) - (int)(oplist[i+1]);
                    }
                    else if (oplist[i-1].is_boolean() && oplist[i+1].is_boolean()) {
                        oplist[i] = (bool)(oplist[i-1]) - (bool)(oplist[i+1]);
                    }
                    else throw std::runtime_error("Types differ");
                    oplist.erase(i+1);
                    oplist.erase(i-1);
                    i-=2;
                }

                // Operator Precedance 2

                else if (oplist[i] == "<<" && op >= 2) {
                    if (
                        oplist[i-1].is_number_integer() && oplist[i+1].is_number_integer() ||(
                            oplist[i-1].is_number_integer() && oplist[i+1].is_boolean() ||
                            oplist[i-1].is_boolean() && oplist[i+1].is_number_integer()
                        )
                    ) {
                        oplist[i] = (int)(oplist[i-1]) << (int)(oplist[i+1]);
                    }
                    else if (oplist[i-1].is_boolean() && oplist[i+1].is_boolean()) {
                        oplist[i] = (bool)(oplist[i-1]) << (bool)(oplist[i+1]);
                    }
                    else throw std::runtime_error("Types differ");
                    oplist.erase(i+1);
                    oplist.erase(i-1);
                    i-=2;
                }

                else if (oplist[i] == ">>" && op >= 2) {
                    if (
                        oplist[i-1].is_number_integer() && oplist[i+1].is_number_integer() ||(
                            oplist[i-1].is_number_integer() && oplist[i+1].is_boolean() ||
                            oplist[i-1].is_boolean() && oplist[i+1].is_number_integer()
                        )
                    ) {
                        oplist[i] = (int)(oplist[i-1]) >> (int)(oplist[i+1]);
                    }
                    else if (oplist[i-1].is_boolean() && oplist[i+1].is_boolean()) {
                        oplist[i] = (bool)(oplist[i-1]) >> (bool)(oplist[i+1]);
                    }
                    else throw std::runtime_error("Types differ");
                    oplist.erase(i+1);
                    oplist.erase(i-1);
                    i-=2;
                }

                // Operator Precedance 3

                else if (oplist[i] == "<" && op >= 3) {
                    if (
                        oplist[i-1].is_number_integer() && oplist[i+1].is_number_integer() ||(
                            oplist[i-1].is_number_integer() && oplist[i+1].is_boolean() ||
                            oplist[i-1].is_boolean() && oplist[i+1].is_number_integer()
                        )
                    ) {
                        oplist[i] = (int)(oplist[i-1]) < (int)(oplist[i+1]);
                    }
                    else if (oplist[i-1].is_boolean() && oplist[i+1].is_boolean()) {
                        oplist[i] = (bool)(oplist[i-1]) < (bool)(oplist[i+1]);
                    }
                    else throw std::runtime_error("Types differ");
                    oplist.erase(i+1);
                    oplist.erase(i-1);
                    i-=2;
                }

                else if (oplist[i] == "<=" && op >= 3) {
                    if (
                        oplist[i-1].is_number_integer() && oplist[i+1].is_number_integer() ||(
                            oplist[i-1].is_number_integer() && oplist[i+1].is_boolean() ||
                            oplist[i-1].is_boolean() && oplist[i+1].is_number_integer()
                        )
                    ) {
                        oplist[i] = (int)(oplist[i-1]) <= (int)(oplist[i+1]);
                    }
                    else if (oplist[i-1].is_boolean() && oplist[i+1].is_boolean()) {
                        oplist[i] = (bool)(oplist[i-1]) <= (bool)(oplist[i+1]);
                    }
                    else throw std::runtime_error("Types differ");
                    oplist.erase(i+1);
                    oplist.erase(i-1);
                    i-=2;
                }

                else if (oplist[i] == ">" && op >= 3) {
                    if (
                        oplist[i-1].is_number_integer() && oplist[i+1].is_number_integer() ||(
                            oplist[i-1].is_number_integer() && oplist[i+1].is_boolean() ||
                            oplist[i-1].is_boolean() && oplist[i+1].is_number_integer()
                        )
                    ) {
                        oplist[i] = (int)(oplist[i-1]) > (int)(oplist[i+1]);
                    }
                    else if (oplist[i-1].is_boolean() && oplist[i+1].is_boolean()) {
                        oplist[i] = (bool)(oplist[i-1]) > (bool)(oplist[i+1]);
                    }
                    else throw std::runtime_error("Types differ");
                    oplist.erase(i+1);
                    oplist.erase(i-1);
                    i-=2;
                }

                else if (oplist[i] == ">=" && op >= 3) {
                    if (
                        oplist[i-1].is_number_integer() && oplist[i+1].is_number_integer() ||(
                            oplist[i-1].is_number_integer() && oplist[i+1].is_boolean() ||
                            oplist[i-1].is_boolean() && oplist[i+1].is_number_integer()
                        )
                    ) {
                        oplist[i] = (int)(oplist[i-1]) >= (int)(oplist[i+1]);
                    }
                    else if (oplist[i-1].is_boolean() && oplist[i+1].is_boolean()) {
                        oplist[i] = (bool)(oplist[i-1]) >= (bool)(oplist[i+1]);
                    }
                    else throw std::runtime_error("Types differ");
                    oplist.erase(i+1);
                    oplist.erase(i-1);
                    i-=2;
                }

                // Operator Precedance 4

                else if (oplist[i] == "==" && op >= 4) {
                    if (
                        oplist[i-1].is_number_integer() && oplist[i+1].is_number_integer() ||(
                            oplist[i-1].is_number_integer() && oplist[i+1].is_boolean() ||
                            oplist[i-1].is_boolean() && oplist[i+1].is_number_integer()
                        )
                    ) {
                        oplist[i] = (int)(oplist[i-1]) == (int)(oplist[i+1]);
                    }
                    else if (oplist[i-1].is_boolean() && oplist[i+1].is_boolean()) {
                        oplist[i] = (bool)(oplist[i-1]) == (bool)(oplist[i+1]);
                    }
                    else if (oplist[i-1].is_string() && oplist[i+1].is_string()) {
                        oplist[i] = std::string(oplist[i-1]) == std::string(oplist[i+1]);
                    }
                    else throw std::runtime_error("Types differ");
                    oplist.erase(i+1);
                    oplist.erase(i-1);
                    i-=2;
                }

                else if (oplist[i] == "!=" && op >= 4) {
                    if (
                        oplist[i-1].is_number_integer() && oplist[i+1].is_number_integer() ||(
                            oplist[i-1].is_number_integer() && oplist[i+1].is_boolean() ||
                            oplist[i-1].is_boolean() && oplist[i+1].is_number_integer()
                        )
                    ) {
                        oplist[i] = (int)(oplist[i-1]) != (int)(oplist[i+1]);
                    }
                    else if (oplist[i-1].is_boolean() && oplist[i+1].is_boolean()) {
                        oplist[i] = (bool)(oplist[i-1]) != (bool)(oplist[i+1]);
                    }
                    else if (oplist[i-1].is_string() && oplist[i+1].is_string()) {
                        oplist[i] = std::string(oplist[i-1]) != std::string(oplist[i+1]);
                    }
                    else throw std::runtime_error("Types differ");
                    oplist.erase(i+1);
                    oplist.erase(i-1);
                    i-=2;
                }

                // Operator Precedance 5

                else if (oplist[i] == "&" && op >= 5) {
                    if (
                        oplist[i-1].is_number_integer() && oplist[i+1].is_number_integer() ||(
                            oplist[i-1].is_number_integer() && oplist[i+1].is_boolean() ||
                            oplist[i-1].is_boolean() && oplist[i+1].is_number_integer()
                        )
                    ) {
                        oplist[i] = (int)(oplist[i-1]) & (int)(oplist[i+1]);
                    }
                    else if (oplist[i-1].is_boolean() && oplist[i+1].is_boolean()) {
                        oplist[i] = (bool)(oplist[i-1]) & (bool)(oplist[i+1]);
                    }
                    else throw std::runtime_error("Types differ");
                    oplist.erase(i+1);
                    oplist.erase(i-1);
                    i-=2;
                }

                // Operator Precedance 6

                else if (oplist[i] == "^" && op >= 6) {
                    if (
                        oplist[i-1].is_number_integer() && oplist[i+1].is_number_integer() ||(
                            oplist[i-1].is_number_integer() && oplist[i+1].is_boolean() ||
                            oplist[i-1].is_boolean() && oplist[i+1].is_number_integer()
                        )
                    ) {
                        oplist[i] = (int)(oplist[i-1]) ^ (int)(oplist[i+1]);
                    }
                    else if (oplist[i-1].is_boolean() && oplist[i+1].is_boolean()) {
                        oplist[i] = (bool)(oplist[i-1]) ^ (bool)(oplist[i+1]);
                    }
                    else throw std::runtime_error("Types differ");
                    oplist.erase(i+1);
                    oplist.erase(i-1);
                    i-=2;
                }
                
                // Operator Precedance 7

                else if (oplist[i] == "|" && op >= 7) {
                    if (
                        oplist[i-1].is_number_integer() && oplist[i+1].is_number_integer() ||(
                            oplist[i-1].is_number_integer() && oplist[i+1].is_boolean() ||
                            oplist[i-1].is_boolean() && oplist[i+1].is_number_integer()
                        )
                    ) {
                        oplist[i] = (int)(oplist[i-1]) | (int)(oplist[i+1]);
                    }
                    else if (oplist[i-1].is_boolean() && oplist[i+1].is_boolean()) {
                        oplist[i] = (bool)(oplist[i-1]) | (bool)(oplist[i+1]);
                    }
                    else throw std::runtime_error("Types differ");
                    oplist.erase(i+1);
                    oplist.erase(i-1);
                    i-=2;
                }

                // Operator Precedance 8

                else if (oplist[i] == "&&" && op >= 8) {
                    if (
                        oplist[i-1].is_number_integer() && oplist[i+1].is_number_integer() ||(
                            oplist[i-1].is_number_integer() && oplist[i+1].is_boolean() ||
                            oplist[i-1].is_boolean() && oplist[i+1].is_number_integer()
                        )
                    ) {
                        oplist[i] = (int)(oplist[i-1]) && (int)(oplist[i+1]);
                    }
                    else if (oplist[i-1].is_boolean() && oplist[i+1].is_boolean()) {
                        oplist[i] = (bool)(oplist[i-1]) && (bool)(oplist[i+1]);
                    }
                    else throw std::runtime_error("Types differ");
                    oplist.erase(i+1);
                    oplist.erase(i-1);
                    i-=2;
                }

                // Operator Precedance 9

                else if (oplist[i] == "||" && op >= 9) {
                    if (
                        oplist[i-1].is_number_integer() && oplist[i+1].is_number_integer() ||(
                            oplist[i-1].is_number_integer() && oplist[i+1].is_boolean() ||
                            oplist[i-1].is_boolean() && oplist[i+1].is_number_integer()
                        )
                    ) {
                        oplist[i] = (int)(oplist[i-1]) || (int)(oplist[i+1]);
                    }
                    else if (oplist[i-1].is_boolean() && oplist[i+1].is_boolean()) {
                        oplist[i] = (bool)(oplist[i-1]) || (bool)(oplist[i+1]);
                    }
                    else throw std::runtime_error("Types differ");
                    oplist.erase(i+1);
                    oplist.erase(i-1);
                    i-=2;
                }

                // Operator Precedance 10

                else if (oplist[i] == "?" && op >= 10) {
                    if (i + 3 < oplist.size()) { 
                        if (oplist[i-1].is_boolean()) {
                            if (oplist[i+2].is_string()) {
                                if (std::string(oplist[i+2]) == ":") {
                                    oplist[i] = (bool)(oplist[i-1]) ? oplist[i+1] : oplist[i+3];
                                } else throw std::runtime_error("Expected : operator, found " + std::string(oplist[i+2]));
                            }     else throw std::runtime_error("Did not find string in ternary operation (expected : operator)");
                        }         else throw std::runtime_error("Ternary condition is not a boolean");
                    }             else throw std::runtime_error("Not enough space to perform ternary operation");

                    oplist.erase(i+3);
                    oplist.erase(i+2);
                    oplist.erase(i+1);
                    oplist.erase(i-1);
                    i-=2;
                }

                // Unknown Operator

                else if (op >= 10) {
                    throw std::runtime_error("Unknown operator: " + std::string(oplist[i]));
                }
            }
        }

        result = {oplist[0]};
        return result;
    }

    std::string fmtstr(std::string fstr) {
        debug("string | " + fstr);
        std::string result = "";
        std::string read = "";
        int i = 0;
        int size = fstr.size();
        char let;

        while (i < size) {
            let = fstr[i];

            if (let == '{') { // expression
                json temp = Braces(ReadUntil(fstr, i, '}'));
                if (temp[0].is_string()) result += std::string(temp[0]);
                else if (temp[0].is_boolean()) result += temp[0] ? "true" : "false";
                else if (temp[0].is_number_integer()) result += std::to_string((int)(temp[0]));
            }

            else {
                result += let;
            }
        
            i++;
        }

        return result;
    }

    int main() {
        debug("FINAL:" + fmtstr("hello {'big {'big {'big'}'}'} world"));
        debug("FINAL:" + fmtstr("{'six'+'7'+'eight}"));
        debug("FINAL:" + fmtstr("{10+9-47}"));
        // debug("FINAL:" + fmtstr("{0x38C5}"));
        debug("FINAL:" + fmtstr("{9+10*9}"));
        debug("FINAL:" + fmtstr("{(9==9)!=(10==6)}"));
        debug("FINAL:" + fmtstr("{32<<1}"));
        debug("FINAL:" + fmtstr("{(1==1)?69:'balright'}"));
        debug("FINAL:" + fmtstr("{(1-1==1)?6+9:'bal'+\"right\"}"));
        return 0;
    }
} // namespace fmtstr

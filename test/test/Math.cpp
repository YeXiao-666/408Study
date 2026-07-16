#include <iostream>
#include <cmath>
#include <string>
#include <sstream>
#include <iomanip>

using namespace std;

//// 格式化输出辅助函数
//string format_term(double coef, string term, bool is_first) {
//    if (abs(coef) < 1e-6) return "";
//    stringstream ss;
//    if (coef > 0 && !is_first) ss << " + ";
//    if (coef < 0) ss << (is_first ? "-" : " - ");
//
//    double abs_coef = abs(coef);
//    if (abs_coef != 1.0) ss << abs_coef;
//    ss << term;
//    return ss.str();
//}
//
//void solve_particular_solution(double a, double b, double c, double alpha, double beta) {
//    // 步骤 1：计算平移后的算子系数 Q(D) = A1*D^2 + B1*D + C1
//    double A1 = a;
//    double B1 = 2 * a * alpha + b;
//    double C1 = a * alpha * alpha + b * alpha + c;
//
//    // 步骤 2：代换 D^2 = -beta^2，整理为 u + vD
//    double u = C1 - A1 * beta * beta;
//    double v = B1;
//
//    double epsilon = 1e-6;
//    string result = "y* = ";
//
//    // 构造 e^(alpha * x) 字符串
//    string exp_str = "";
//    if (abs(alpha) > epsilon) {
//        if (alpha == 1.0) exp_str = "e^x";
//        else if (alpha == -1.0) exp_str = "e^(-x)";
//        else {
//            stringstream ss;
//            ss << "e^(" << alpha << "x)";
//            exp_str = ss.str();
//        }
//    }
//
//    // 步骤 3：分类讨论
//    if (abs(u) < epsilon && abs(v) < epsilon) {
//        // 【共振情况】 (如图片中的 注例8)
//        // 对应式子: -x / (2 * A1 * beta) * cos(beta * x)
//        double coef_cos = -1.0 / (2.0 * A1 * beta);
//
//        if (!exp_str.empty()) result += exp_str + " * ";
//
//        stringstream ss_cos;
//        if (beta == 1.0) ss_cos << "cos(x)";
//        else ss_cos << "cos(" << beta << "x)";
//
//        result += format_term(coef_cos, "x" + ss_cos.str(), true);
//    }
//    else {
//        // 【非共振情况】
//        double denominator = u * u + v * v * beta * beta;
//        double coef_sin = u / denominator;
//        double coef_cos = -v * beta / denominator;
//
//        stringstream ss_sin, ss_cos;
//        if (beta == 1.0) { ss_sin << "sin(x)"; ss_cos << "cos(x)"; }
//        else { ss_sin << "sin(" << beta << "x)"; ss_cos << "cos(" << beta << "x)"; }
//
//        string term_sin = format_term(coef_sin, ss_sin.str(), true);
//        string term_cos = format_term(coef_cos, ss_cos.str(), term_sin.empty());
//
//        if (!exp_str.empty()) result += exp_str + " * (";
//        result += term_sin + term_cos;
//        if (!exp_str.empty()) result += ")";
//    }
//
//    cout << "方程: " << a << "y'' + " << b << "y' + " << c << "y = e^(" << alpha << "x)sin(" << beta << "x)" << endl;
//    cout << "解得特解: " << result << endl << endl;
//}
//
//int main() {
//    // 【测试用例 1】图片中的 注例8
//    // 方程: y'' + 4y' + 5y = e^(-2x)sin(x)
//    // a=1, b=4, c=5, alpha=-2, beta=1
//    // 预期输出应当包含 -0.5*x*e^(-2x)cos(x)
//    solve_particular_solution(1, 4, 5, -2, 1);
//
//    // 【测试用例 2】普通非共振情况
//    // 方程: y'' - 2y' + y = e^x sin(2x)
//    solve_particular_solution(1, -2, 1, 1, 2);
//
//    return 0;
//}



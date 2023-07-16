
#include "optimizer.h"
using namespace std;

const int MAX_OUTPUT_SIZE = 1024;

// 哈希表类型定义，将表达式作为键，节点作为值
typedef std::map<std::string, ExpressionNode> ExpressionHash;

//将任何类型转化为string
template <typename T>
std::string convertToString(T value) {
    std::stringstream ss;
    ss << value;
    return ss.str();
}

char* intToCharPointer(int number) {
    std::stringstream ss;
    ss << number;  // 将整数转换为字符串

    std::string str = ss.str();
    char* charPtr = new char[str.length() + 1];
    std::strcpy(charPtr, str.c_str());  // 将字符串复制到字符数组

    return charPtr;
}

bool get_ir(instr_ir ir, char*& output) {
    if (output == NULL) {
        return false; // 输出指针为空，返回错误
    }
    switch (ir.kind) {
        case CONST:
            snprintf(output, MAX_OUTPUT_SIZE, "     %s = %s", ir.x.c_str(), ir.y.c_str());
            break;
        case MOVE:
            snprintf(output, MAX_OUTPUT_SIZE, "     %s = %s", ir.x.c_str(), ir.y.c_str());
            break;
        case BIOP:
            switch (ir.biop) {
                case ADD:
                    snprintf(output, MAX_OUTPUT_SIZE, "     %s = %s + %s", ir.x.c_str(), ir.y.c_str(), ir.z.c_str());
                    break;
                case SUB:
                    snprintf(output, MAX_OUTPUT_SIZE, "     %s = %s - %s", ir.x.c_str(), ir.y.c_str(), ir.z.c_str());
                    break;
                case TIMES:
                    snprintf(output, MAX_OUTPUT_SIZE, "     %s = %s * %s", ir.x.c_str(), ir.y.c_str(), ir.z.c_str());
                    break;
                case DIV:
                    snprintf(output, MAX_OUTPUT_SIZE, "     %s = %s / %s", ir.x.c_str(), ir.y.c_str(), ir.z.c_str());
                    break;
                case MOD:
                    snprintf(output, MAX_OUTPUT_SIZE, "     %s = %s %% %s", ir.x.c_str(), ir.y.c_str(), ir.z.c_str());
                    break;
                case AND:
                    snprintf(output, MAX_OUTPUT_SIZE, "     %s = %s && %s", ir.x.c_str(), ir.y.c_str(), ir.z.c_str());
                    break;
                case OR:
                    snprintf(output, MAX_OUTPUT_SIZE, "     %s = %s || %s", ir.x.c_str(), ir.y.c_str(), ir.z.c_str());
                    break;
                case EQU:
                    snprintf(output, MAX_OUTPUT_SIZE, "     %s = %s == %s", ir.x.c_str(), ir.y.c_str(), ir.z.c_str());
                    break;
                case NOTEQU:
                    snprintf(output, MAX_OUTPUT_SIZE, "     %s = %s != %s", ir.x.c_str(), ir.y.c_str(), ir.z.c_str());
                    break;
                case GREATER:
                    snprintf(output, MAX_OUTPUT_SIZE, "     %s = %s > %s", ir.x.c_str(), ir.y.c_str(), ir.z.c_str());
                    break;
                case GREATER_EQU:
                    snprintf(output, MAX_OUTPUT_SIZE, "     %s = %s >= %s", ir.x.c_str(), ir.y.c_str(), ir.z.c_str());
                    break;
                case LESS:
                    snprintf(output, MAX_OUTPUT_SIZE, "     %s = %s < %s", ir.x.c_str(), ir.y.c_str(), ir.z.c_str());
                    break;
                case LESS_EQU:
                    snprintf(output, MAX_OUTPUT_SIZE, "     %s = %s <= %s", ir.x.c_str(), ir.y.c_str(), ir.z.c_str());
                    break;
                default:
                    return false; // 未知的二元运算符，返回错误
            }
            break;
        case UNOP:
            switch (ir.unop) {
                case NEG:
                    snprintf(output, MAX_OUTPUT_SIZE, "     %s = -%s", ir.x.c_str(), ir.y.c_str());
                    break;
                case REVERSE:
                    snprintf(output, MAX_OUTPUT_SIZE, "     %s = !%s", ir.x.c_str(), ir.y.c_str());
                    break;
                default:
                    return false; // 未知的一元运算符，返回错误
            }
            break;
        case MEMWRITE:
            snprintf(output, MAX_OUTPUT_SIZE, "     %s[%s] = %s", ir.x.c_str(), ir.y.c_str(), ir.z.c_str());
            break;
        case MEMREAD:
            snprintf(output, MAX_OUTPUT_SIZE, "     %s = %s[%s]", ir.x.c_str(), ir.y.c_str(), ir.z.c_str());
            break;
        case PARAM:
            snprintf(output, MAX_OUTPUT_SIZE, "     PARAM %s", ir.x.c_str());
            break;
        case CALL:
            snprintf(output, MAX_OUTPUT_SIZE, "     CALL %s, %s", ir.x.c_str(), ir.y.c_str());
            break;
        case CJMP:
            snprintf(output, MAX_OUTPUT_SIZE, "     IF %s GOTO %s", ir.x.c_str(), ir.y.c_str());
            break;
        case JMP:
            snprintf(output, MAX_OUTPUT_SIZE, "     GOTO %s", ir.x.c_str());
            break;
        case LABEL://maybe useless
            snprintf(output, MAX_OUTPUT_SIZE, "%s:", ir.x.c_str());
            break;
        case RETURN:
            snprintf(output, MAX_OUTPUT_SIZE, "     RETURN %s", ir.x.c_str());
            break;
        default:
            return false; // 未知的指令类型，返回错误
    }
    char* number = intToCharPointer(ir.num);
    strcpy(output ,("(" + std::string(number) + ")" + std::string(output)).c_str());
    return true; // 成功生成三地址代码字符串
}


bool write_ir(std::vector<instr_ir>& ir,std::ofstream& outputfile)
{
    if(outputfile.is_open())
    {
        std::cout<<"begin writing IR"<<std::endl;
        for(int i = 0;i<ir.size();i++){
            char* output = new char[1024];
            if(!get_ir(ir[i], output)){
                delete[] output;
                return false;
            }
            outputfile<< output <<std::endl;
            delete[] output;
        }
        std::cout<<"end writing IR"<<std::endl;
        return true;
    }
    else {
        std::cout<<"fail in open file!"<<std::endl;
        return false;
    }
}

bool output_ir(std::vector<instr_ir>& ir)
{
    std::cout<<"begin output IR"<<std::endl;
    for (int i=0;i<ir.size();i++){
        char* output = new char[1024];
        // snprintf(output, 1024, "     GOTO %s", ir[i].x.c_str());
        if(!get_ir(ir[i], output)){
            delete[] output;
            return false;
        }
        std::cout<<output<<std::endl;
        delete[] output;
    }
    return true;
}

//测试
// (0)     GOTO 1
// (1)     t1 = 1
// (2)     t3 = 3
// (3)     t2 = t1 < t3
// (4)     IF t2 GOTO 6
// (5)     GOTO 9
// (6)     t4 = 1
// (7)     t1 = t1 + t4
// (8)     GOTO 2
// (9)     t5 = t1
basicblock test_bb()
{
    std::vector<instr_ir> bbir;

    instr_ir ir;
    ir.num = 0;
    ir.kind = JMP;
    ir.x = (const char*)"1";
    bbir.push_back(ir);

    ir.num = 1;
    ir.kind = CONST;
    ir.x = "t1";
    ir.y = "1";
    bbir.push_back(ir);

    ir.num = 2;
    ir.kind = CONST;
    ir.x = "t3";
    ir.y = "3";
    bbir.push_back(ir);

    ir.num = 3;
    ir.kind = BIOP;
    ir.biop = LESS;
    ir.x = "t2";
    ir.y = "t1";
    ir.z = "t3";
    bbir.push_back(ir);

    ir.num = 4;
    ir.kind = CJMP;
    ir.x = "t2";
    ir.y = "6";
    bbir.push_back(ir);

    ir.num = 5;
    ir.kind = JMP;
    ir.x = "9";
    bbir.push_back(ir);

    ir.num = 6;
    ir.kind = CONST;
    ir.x = "t4";
    ir.y = "1";
    bbir.push_back(ir);

    ir.num = 7;
    ir.kind = BIOP;
    ir.biop = ADD;
    ir.x = "t1";
    ir.y = "t1";
    ir.z = "t4";
    bbir.push_back(ir);

    ir.num = 8;
    ir.kind = JMP;
    ir.x = "2";
    bbir.push_back(ir);

    ir.num = 9;
    ir.kind = MOVE;
    ir.x = "t5";
    ir.y = "t1";
    bbir.push_back(ir);

    basicblock bb;
    bb.bbir = bbir;

    return bb;
}

// 查询公共子表达式
int queryCommonExpression(const std::string& expression, ExpressionHash& hashTable,DAG& dag) {
    // 在哈希表中查找是否存在该表达式
    auto it = hashTable.find(expression);
    if (it != hashTable.end()) {
        dag.adjacencyList[it->second.result].push_back(dag.numNodes + 1);
        return it->second.result;
    } else {
        //如果没有的话，更新dag
        dag.numNodes += 1;
        ExpressionNode exp;
        exp.expression = expression;
        exp.leftchild = queryCommonExpression(expression.substr(0, expression.find_first_of("+-*/&|%=<>!")), hashTable,dag);
        exp.rightchild = queryCommonExpression(expression.substr(expression.find_last_of("+-*/&|%=<>!") + 1), hashTable,dag);
        int result = dag.numNodes + 1;  
        hashTable.emplace(expression, ExpressionNode{expression, result});
        dag.Exps.push_back(exp);
        dag.adjacencyList[result].push_back(dag.numNodes);

        return result;
    }
}

//取出bb所有变量
bool find_all_values(basicblock bb,std::unordered_set<std::string>& values)
{
    for(int i=0;i<bb.bbir.size();i++)
    {
        instr_ir ir = bb.bbir[i];
        switch (ir.kind) {
            case CONST:
                values.insert(ir.x);
                break;
            case MOVE:
                values.insert(ir.x);
                values.insert(ir.y);
                break;
            case BIOP:
                values.insert(ir.x);
                values.insert(ir.y);
                values.insert(ir.z);
                break;
            case UNOP:
                values.insert(ir.x);
                values.insert(ir.y);
                break;
            case MEMWRITE:
                values.insert(ir.x);
                values.insert(ir.y);
                values.insert(ir.z);
                break;
            case MEMREAD:
                values.insert(ir.x);
                values.insert(ir.y);
                values.insert(ir.z);
                break;
            default:
                break;
        }
    }
    return true;
}

//寻找公共子表达式并合并
bool CommonSubexp(basicblock* bb){
    int ir_size = bb->bbir.size();
    std::unordered_set<std::string> values;
    find_all_values(*bb,values);

    //计算是否是常数
    std::map<std::string, int>is_const;
    std::map<std::string, int>int_value;
    std::map<std::string, float>float_value;
    std::map<std::string, double>double_value;
    for(auto it = values.begin(); it != values.end(); ++it)//这里要改！！！！与块间联系。
    {
        std::string value = *it;
        is_const[value] = 0;
    }
    for(int i=0;i<ir_size;i++)
    {
        // instr_ir ir = bb->bbir[i];
        const_trans(bb->bbir[i],is_const,int_value,float_value,double_value);
    }
    
    //计算块内的活跃性与后续使用信息
    //上面一步的死代码也会在这里消除
    std::map<int, std::map<std::string, int>> lives;//活跃性信息
    std::map<int, std::map<std::string, int>> uses;//后续使用
    for(int i=ir_size - 1;i>=0;i--)
    {
        if(i == ir_size - 1){
            for(auto it = values.begin(); it != values.end(); ++it)
            {
                lives[i][*it] = 1;
                uses[i][*it] = -1;
            }
        }
        lives[i][bb->bbir[i].x] = 0;
        uses[i][bb->bbir[i].x] = -1;
        uses[i][bb->bbir[i].y] = i;
        uses[i][bb->bbir[i].z] = i; 
        lives[i][bb->bbir[i].y] = 1;
        lives[i][bb->bbir[i].z] = 1;
    }
    return true;

    //寻找公共子表达式
    ExpressionHash hashTable;
    DAG dag;
    for(int i=0;i<ir_size;i++)
    {
        instr_ir ir = bb->bbir[i];
        if(ir.kind == BIOP)
        {
            std::string biop = op_str(ir.biop,ir.unop);
            std::string expression = ir.y + biop + ir.z;
            int result = queryCommonExpression(expression, hashTable,dag);
        }
    }

    //TODO
    /*
    将dag转换为ir
    对每个具有一个或多个附加变量的结点，构造一个三地址语句计算其中某个变量的值
    优先选择在基本块出口活跃的变量
    需要考虑依赖
    */
}

std::string op_str(instr_biop biop,instr_unop unop){
        switch (biop) {
            case ADD:
                return "+";
            case SUB:
                return "-";
            case TIMES:
                return "*";
            case DIV:
                return "/";
            case MOD:
                return "%";
            case AND:
                return "&&";
            case OR:
                return "||";
            case LESS:
                return "<";
            case GREATER:
                return ">";
            case LESS_EQU:
                return "<=";
            case GREATER_EQU:
                return ">=";
            case EQU:
                return "==";
            case NOTEQU:
                return "!=";
            default:
                break;
        }
        switch (unop){
            case NEG:
                return "-";
            case REVERSE:
                return "!";
            default:
                return "";
        }
}

//定值传递
bool const_trans(instr_ir& ir,std::map<std::string, int>& is_const,std::map<std::string, int>& int_value,std::map<std::string, float>& float_value,std::map<std::string, double>& double_value)
{
    switch(ir.kind){
    case CONST:
        is_const[ir.x] = 1;
        if(ir.unit == INT)
            int_value[ir.x] = std::atoi(ir.y.c_str());
        if(ir.unit == FLOAT)
            float_value[ir.x] = std::atof(ir.y.c_str());
        if(ir.unit == DOUBLE)
            double_value[ir.x] = std::strtod(ir.y.c_str(), NULL);
        break;
    case MOVE:
        is_const[ir.x] = is_const[ir.y];
        if(is_const[ir.x])
        {
            int x_value;
            if(ir.unit == INT)
                int_value[ir.x] = int_value[ir.y],x_value = int_value[ir.x];
            if(ir.unit == FLOAT)
                float_value[ir.x] = float_value[ir.y],x_value = float_value[ir.x];
            if(ir.unit == DOUBLE)
                double_value[ir.x] = double_value[ir.y],x_value = double_value[ir.x];
            ir.kind = CONST;
            ir.y = convertToString(x_value);
        }
        break;
    case BIOP:
        switch (ir.biop) {
            case ADD:
                if(is_const[ir.y] && is_const[ir.z])
                {
                    int y_value,z_value;
                    if(ir.unit == INT)
                        y_value = int_value[ir.y],z_value = int_value[ir.z];
                    if(ir.unit == FLOAT)
                        y_value = float_value[ir.y],z_value = float_value[ir.z];
                    if(ir.unit == DOUBLE)
                        y_value = double_value[ir.y],z_value = double_value[ir.z];
                    ir.kind = CONST;
                    ir.y = convertToString(y_value + z_value);
                }
                break;
            case SUB:
                if(is_const[ir.y] && is_const[ir.z])
                {
                    int y_value,z_value;
                    if(ir.unit == INT)
                        y_value = int_value[ir.y],z_value = int_value[ir.z];
                    if(ir.unit == FLOAT)
                        y_value = float_value[ir.y],z_value = float_value[ir.z];
                    if(ir.unit == DOUBLE)
                        y_value = double_value[ir.y],z_value = double_value[ir.z];
                    ir.kind = CONST;
                    ir.y = convertToString(y_value - z_value);
                }
                break;
            case TIMES:
                if(is_const[ir.y] && is_const[ir.z])
                {
                    int y_value,z_value;
                    if(ir.unit == INT)
                        y_value = int_value[ir.y],z_value = int_value[ir.z];
                    if(ir.unit == FLOAT)
                        y_value = float_value[ir.y],z_value = float_value[ir.z];
                    if(ir.unit == DOUBLE)
                        y_value = double_value[ir.y],z_value = double_value[ir.z];
                    ir.kind = CONST;
                    ir.y = convertToString(y_value * z_value);
                }
                break;
            case DIV:
                if(is_const[ir.y] && is_const[ir.z])
                {
                    int y_value,z_value;
                    if(ir.unit == INT)
                        y_value = int_value[ir.y],z_value = int_value[ir.z];
                    if(ir.unit == FLOAT)
                        y_value = float_value[ir.y],z_value = float_value[ir.z];
                    if(ir.unit == DOUBLE)
                        y_value = double_value[ir.y],z_value = double_value[ir.z];
                    ir.kind = CONST;
                    ir.y = convertToString(y_value / z_value);
                }
                break;
            case MOD:
                if(is_const[ir.y] && is_const[ir.z])
                {
                    int y_value,z_value;
                    if(ir.unit == INT)
                        y_value = int_value[ir.y],z_value = int_value[ir.z];
                    if(ir.unit == FLOAT)
                        y_value = float_value[ir.y],z_value = float_value[ir.z];
                    if(ir.unit == DOUBLE)
                        y_value = double_value[ir.y],z_value = double_value[ir.z];
                    ir.kind = CONST;
                    ir.y = convertToString(y_value % z_value);
                }
                break;
            case AND:
                if(is_const[ir.y] && is_const[ir.z])
                {
                    int y_value,z_value;
                    if(ir.unit == INT)
                        y_value = int_value[ir.y],z_value = int_value[ir.z];
                    if(ir.unit == FLOAT)
                        y_value = float_value[ir.y],z_value = float_value[ir.z];
                    if(ir.unit == DOUBLE)
                        y_value = double_value[ir.y],z_value = double_value[ir.z];
                    ir.kind = CONST;
                    ir.y = convertToString(y_value && z_value);
                }
                break;
            case OR:
                if(is_const[ir.y] && is_const[ir.z])
                {
                    int y_value,z_value;
                    if(ir.unit == INT)
                        y_value = int_value[ir.y],z_value = int_value[ir.z];
                    if(ir.unit == FLOAT)
                        y_value = float_value[ir.y],z_value = float_value[ir.z];
                    if(ir.unit == DOUBLE)
                        y_value = double_value[ir.y],z_value = double_value[ir.z];
                    ir.kind = CONST;
                    ir.y = convertToString(y_value || z_value);
                }
                break;
            case EQU:
                if(is_const[ir.y] && is_const[ir.z])
                {
                    int y_value,z_value;
                    if(ir.unit == INT)
                        y_value = int_value[ir.y],z_value = int_value[ir.z];
                    if(ir.unit == FLOAT)
                        y_value = float_value[ir.y],z_value = float_value[ir.z];
                    if(ir.unit == DOUBLE)
                        y_value = double_value[ir.y],z_value = double_value[ir.z];
                    if(y_value == z_value)
                        ir.kind = CONST,ir.y = "1";
                    else
                        ir.kind = CONST,ir.y = "0";
                }
                break;
            case NOTEQU:
                if(is_const[ir.y] && is_const[ir.z])
                {
                    int y_value,z_value;
                    if(ir.unit == INT)
                        y_value = int_value[ir.y],z_value = int_value[ir.z];
                    if(ir.unit == FLOAT)
                        y_value = float_value[ir.y],z_value = float_value[ir.z];
                    if(ir.unit == DOUBLE)
                        y_value = double_value[ir.y],z_value = double_value[ir.z];
                    if(y_value != z_value)
                        ir.kind = CONST,ir.y = "1";
                    else
                        ir.kind = CONST,ir.y = "0";
                }
                break;
            case GREATER:
                if(is_const[ir.y] && is_const[ir.z])
                {
                    int y_value,z_value;
                    if(ir.unit == INT)
                        y_value = int_value[ir.y],z_value = int_value[ir.z];
                    if(ir.unit == FLOAT)
                        y_value = float_value[ir.y],z_value = float_value[ir.z];
                    if(ir.unit == DOUBLE)
                        y_value = double_value[ir.y],z_value = double_value[ir.z];
                    if(y_value > z_value)
                        ir.kind = CONST,ir.y = "1";
                    else
                        ir.kind = CONST,ir.y = "0";
                }
                break;
            case GREATER_EQU:
                if(is_const[ir.y] && is_const[ir.z])
                {
                    int y_value,z_value;
                    if(ir.unit == INT)
                        y_value = int_value[ir.y],z_value = int_value[ir.z];
                    if(ir.unit == FLOAT)
                        y_value = float_value[ir.y],z_value = float_value[ir.z];
                    if(ir.unit == DOUBLE)
                        y_value = double_value[ir.y],z_value = double_value[ir.z];
                    if(y_value >= z_value)
                        ir.kind = CONST,ir.y = "1";
                    else
                        ir.kind = CONST,ir.y = "0";
                }
                break;
            case LESS:
                if(is_const[ir.y] && is_const[ir.z])
                {
                    int y_value,z_value;
                    if(ir.unit == INT)
                        y_value = int_value[ir.y],z_value = int_value[ir.z];
                    if(ir.unit == FLOAT)
                        y_value = float_value[ir.y],z_value = float_value[ir.z];
                    if(ir.unit == DOUBLE)
                        y_value = double_value[ir.y],z_value = double_value[ir.z];
                    if(y_value < z_value)
                        ir.kind = CONST,ir.y = "1";
                    else
                        ir.kind = CONST,ir.y = "0";
                }
                break;
            case LESS_EQU:
                if(is_const[ir.y] && is_const[ir.z])
                {
                    int y_value,z_value;
                    if(ir.unit == INT)
                        y_value = int_value[ir.y],z_value = int_value[ir.z];
                    if(ir.unit == FLOAT)
                        y_value = float_value[ir.y],z_value = float_value[ir.z];
                    if(ir.unit == DOUBLE)
                        y_value = double_value[ir.y],z_value = double_value[ir.z];
                    if(y_value <= z_value)
                        ir.kind = CONST,ir.y = "1";
                    else
                        ir.kind = CONST,ir.y = "0";
                }
                break;
            default:
                return false; // 未知的二元运算符，返回错误
        }
        break;
    case UNOP:
        switch (ir.unop) {
            case NEG:
                if(is_const[ir.y])
                {
                    int y_value;
                    if(ir.unit == INT)
                        y_value = int_value[ir.y];
                    if(ir.unit == FLOAT)
                        y_value = float_value[ir.y];
                    if(ir.unit == DOUBLE)
                        y_value = double_value[ir.y];
                    ir.kind = CONST;
                    ir.y = convertToString(-y_value);
                }
            case REVERSE:
                if(is_const[ir.y])
                {
                    int y_value;
                    if(ir.unit == INT)
                        y_value = int_value[ir.y];
                    if(ir.unit == FLOAT)
                        y_value = float_value[ir.y];
                    if(ir.unit == DOUBLE)
                        y_value = double_value[ir.y];
                    ir.kind = CONST;
                    ir.y = convertToString(~y_value);
                }
            default:
                return false; // 未知的一元运算符，返回错误
        }
        break;
    }
    return true;
}


//划分基本快建立cfg
CFG BuildCFG(basicblock bb,std::vector<basicblock>& bbs){
    std::vector<int> head;
    head.push_back(0);
    for(int i=0;i<bb.bbir.size();i++)
    {
        instr_ir ir = bb.bbir[i];
        if(ir.kind == JMP)
        {
            if(i < bb.bbir.size() - 1)
            {
                head.push_back(ir.num + 1);
            }
            head.push_back(atoi(ir.x.c_str()));
        }
        if(ir.kind == CJMP)
        {
            if(i < bb.bbir.size() - 1)
            {
                head.push_back(ir.num + 1);
            }
            head.push_back(atoi(ir.y.c_str()));
        }
    }
    std::vector<int> head_save = head;
    std::sort(head.begin(),head.end());

    head.erase(std::unique(head.begin(), head.end()), head.end());

    if(head[head.size()-1]!=bb.bbir.size())
        head.push_back(bb.bbir.size());
    for(int i=0;i<head.size()-1;i++)
    {
        basicblock bb_cfg;
        for(int j=head[i];j<head[i+1];j++)
        {
            instr_ir ir_cfg = bb.bbir[j];
            bb_cfg.bbir.push_back(ir_cfg);
        }
        // cout<<bb_cfg.bbir[0].num<<endl;
        // output_ir(bb_cfg.bbir);
        bbs.push_back(bb_cfg);
    }
    CFG cfg(bbs);
    //划分bb
    int size = bbs.size();
    int bb_max_num[1000];
    for(int i=0;i<head.size()-1;i++)
    {
        if(i != head.size()-2)
            cfg.addEdge(i,i+1);
        bb_max_num[i] = bbs[i].bbir[bbs[i].bbir.size()-1].num;
    }
    for(int i=1;i<head_save.size();i+=2)
    {
        int source_num = head_save[i]-1;
        int target_num = head_save[i+1];
        int source = find_in_bbs(source_num,bb_max_num,head.size()-1);
        int target = find_in_bbs(target_num,bb_max_num,head.size()-1);
        cfg.addEdge(source,target);
    }
    return cfg;
}

int find_in_bbs(int num,int bb_max_num[],int size)
{
    int find = 0;
    for(int i=0;i<size;i++)
    {
        if(num <= bb_max_num[i])
        {
            find = i;
            break;
        }
    }
    return find;
}

//test over here
int main()
{
    basicblock bb = test_bb();//这里处理一下，赋个值
    std::vector<basicblock> bbs;
    CFG cfg = BuildCFG(bb,bbs);
    cfg.printGraph();
    //公共子表达式的提取与化简
    cout<<bbs.size()<<endl;
    for(int i=0;i<bbs.size();i++)
    {
        if(!CommonSubexp(&bbs[i]))
        {
            std::cout<<"fault in commonsubexp"<<endl;
            return 1;
        }
        std::cout<<i<<": ";
        output_ir(bbs[i].bbir);
    }
    return 0;
}


/*
优化的第三项：块间优化还没有开始做

在这里梳理思路：
上面的代码中已经进行了基本块的划分，接下来就是块间优化了

1.到达定制
gen[B]：基本块B中的定值d到达B的结束点，则d∈gen[B]
kill[B]：程序中不能到达B的结束点的定值d，则d∈kill[B]
in[B]：到达B的开始点的定值集合
out[B]：到达B的结束点的定值集合

2.活跃变量
in[B]：块B开始点的活跃变量集合
out[B]：块B结束点的活跃变量集合
def[B]：块B中无二义定值且在该定值前没有引用的变量集
use[B]：块B中可能引用且在该引用前没有定值的变量集

3.可用表达式
in[B]：块B开始点的可用表达式集合
out[B]：块B结束点的可用表达式集合
e_gen[B]：块B生成的可用表达式集合
e_kill[B]：U中被块B注销的可用表达式集合

上面的变量使用集合和数组保存。在函数如下中实现：
//控制流分析
bool flow_analysis(std::string<basicblock>& bbs);
在上面实现的还有功能：
1，基本块间的公共子表达式分析：
对每个形如x := y + z（+表示一般的算符）的语句s，如果   y + z在s所在块的开始点可用，且在该块中s前没有y和z的定值，则执行下面的步骤：
	(1)	为了寻找到达s所在块的y + z的计算，顺着流图的边，从该块开始反向搜索，但不穿过任何计算y + z的块。在遇到的每个块中，y + z的最后一个计算是到达s的y + z的计算
	(2)	建立新变量u
	(3)	把(1)中找到的每个语句w := y + z用下面的两个语句代替
			u := y + z
			w := u
	(4)	用x := u代替语句s
2.复写传播
	复写传播的流分析：
	in[B]：是复写语句s: x := y的集合，即从初始结点到块B的开始点的每条路径上都有语句s，且最后出现的s后面没有对x或y的赋值，在这个集合中只可能含有一个x在左边的复写语句（因为对x的重新赋值也会改变复写语句的影响范围）
out[B]：类似于in[B]，只是定义在块B的结束点
c_gen[B]：如果复写语句s: x := y出现在块B中，且块B中后面没有对x或y的赋值，则称s在块B中生成
c_kill[B]：如果x或y在B中赋值，且s不在块B中，则称s: x := y在块B中注销
流方程：

代码实现：
对每个复写s:  x := y执行下面的步骤：
	(1)	找出该x的定值能够到达的那些x的引用
	(2)	对(1)中找到的每个x引用，确定s是否在c_in[B]中（块B是含这个x引用的基本块，且块B中该引用的前面没有x或y的定值），如果s在c_in[B]中，则x是到达块B的唯一的x定值
	(3)	如果s满足(2)的条件，则删掉s，且把(1)中找出的所有x引用以y代替

3.对于循环不变的计算：
	(1)	对所有下列语句标记“不变”：它的运算对象是常数，和所有的到达—定值在循环L外
	(2)	给下面的语句标记“不变”：它们先前没有标记，所有运算对象是常数，或到达—定值都在循环L外，或者只有一个到达—定值，这个定值是循环L中已标记为“不变”的语句
	(3)	重复(2)，知道某次重复没有新的语句可标记为“不变”为止

    进行的优化：
    1.代码外提：
    (1)	用前面的算法寻找循环不变计算
        (2)	对(1)中找到的每个x定值语句s，检查是否满足
            (a)   s所在的块是L所有出口的必经块
            (b)   x在L的其它地方没有定值
            (c)   L中所有x的引用只有s的x定值才能到达
        (3)	按寻找循环不变计算的算法找出次序，把(1)中找出的满足(2)中3个条件的每个语句移到新的前置块，但是若s的	运算对象在L中定义（有前面的算法找出这种s），则只	有这种对象的定值语句外提到前置块后，才能外提s
    2. 归纳变量与强度削弱
        2.1归纳变量的实现：
        (1)	扫描L的语句，找出所有基本归纳变量（使用循环不变计算的信息）。对应于基本归纳变量i的是三元组（i, 1, 0）
        (2)	寻找L中只有一个赋值的变量k，它具有下面的形式之一：
            k := j  b,  k := b  j,  k := j / b,   k := j ± b,  k := b ± j
            其中，b是常数，j是基本的和非基本的归纳变量，如果j是基本的，对应的三元组定义简单；
            如果j是非基本的，它属于i族，则附加如下要求：
            (a) 在循环L中对j唯一赋值和对k的赋值之间没有对i的赋值
            (b) 循环L外没有j的定值可到达k的这个定值点
            如果j的三元组( i, c, d)，k := b  j，则k的三元组( i, bc, b*d)
        2.2强度削弱：
            (1)	建立新变量s，但如果变量j1和j2有同样的三元组，则仅建一个新变量用于两者
        (2)	用 j := s 代替 j 的赋值
        (3)	在L中每个赋值 i := i + n后面（n是常数），紧接它加上:
                s := s + c * n
            其中的表达式 c * n 计算为一个常数（c和n都是常数），把s加入i族，三元组为（ i , c , d ）
        (4)	还必须保证在循环的入口处s的初值为c * i + d，这个初始化可以放在前置块的末尾，它由：
                s := c  i   （如果c为1，则s := i ）
                s := s + d	 （如果d为0，则省略 ）
        2.3代码删除
            (1)	考虑每个仅用于计算同族中其它归纳变量和条件分支的基本归纳变量i。
            (a) 取i族的某个j，优先于其三元组( i, c, d )中的c = 1或d = 0这种尽可能简单的j，把每个含i的测试改成用j代替i。假定c>0，则测试if i relop x goto B等价于(其中，x不是归纳变量)：
                r := c  x		（ 如果c = 1，则r := x ）
                r := r + d		（如果d = 0，则省略 ）
                if j relop r goto B
            (b) if x relop i goto B的处理类似
            (c) 如果if  i1 relop i2 goto B中i1和i2都是归纳变量，也可以考虑借鉴上述方法处理，特别是存在三元组( i1, c1, d1 )的j1和三元组( i2, c2, d2 )的j2，且c1 = c2, d1 = d2，则i1 relop i2等价于j1 relop j2
            (d)  当被删掉的归纳变量不再引用时，从循环中删去所有对它的赋值
        (2)	考虑前面算法引入的赋值语句j := s的每个归纳变量j
            (a)如果在引入的j := s和任何j的引用间没有对s的赋值（一般是没有这种赋值的），则用引用s代替所有j的引用，并删除语句j := s
            (b) 如果存在对s的赋值，则要考虑到达—定值信息等，进行检查，以决定是否可以进行删除。

*/



#include "ir.h"
using namespace std;

const int MAX_OUTPUT_SIZE = 1024;

// 哈希表类型定义，将表达式作为键，节点作为值
typedef std::unordered_map<std::string, ExpressionNode> ExpressionHash;

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
                case POS:
                    snprintf(output, MAX_OUTPUT_SIZE, "     %s = +%s", ir.x.c_str(), ir.y.c_str());
                    break;
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
        return it->second.result;
    } else {
        //如果没有的话，更新dag
        dag.numNodes += 1;
        ExpressionNode exp;
        exp.expression = expression;
        exp.leftchild = 
        int result = dag.numNodes + 1;  // 假设有一个函数用于计算表达式结果
        hashTable.emplace(expression, ExpressionNode{expression, result});
        return result;
    }
}

//取出bb所有变量
bool find_all_values(basicblock bb,std::unordered_set<std::string>& values)
{
    for(int i=0;i<bb.bbir.size();i++)
    {
        instr_ir ir = bb.bbir[i];
        switch(ir.instr_kind){
            case CONST:
                values.insert(ir.x);
                break;
            case MOVE:
                values.insert(ir.x,ir.y);
                break;
            case BIOP:
                values.insert(ir.x,ir.y,ir.z);
                break;
            case UNOP:
                values.insert(ir.x,ir.y);
                break;
            case MEMWRITE:
                values.insert(ir.x,ir.y,ir.z);
                break;
            case MEMREAD:
                values.insert(ir.x,ir.y,ir.z);
                break;
            default:
                break;
        }
    }
    return true;
}

//寻找公共子表达式并合并
bool CommonSubexp(basicblock* bb){
    ir_size = bb.bbir.size();
    std::unordered_set<std::string> values;
    find_all_values(bb,values);

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
        instr_ir ir = bb.bbir[i];

    }
    
    //计算块内的活跃性与后续使用信息
    std::map<int, std::map<std::string, int>> lives;//活跃性信息
    std::map<int, std::map<std::string, int>> uses;//后续使用
    for(int i=ir_size - 1;i>=0;i--)
    {
        if(i == ir_size - 1){
            for(auto it = values.begin(); it != values.end(); ++it)
            {
                lives[i][*it] = 1;
                uses
            }
        }
    }
    return true;
}

//定值传递
bool const_trans(instr_ir& ir)
{
    switch(ir.type){
    case CONST:
        is_const[ir.x] = 1;
        if(ir.unit == INT)
            int_value[ir.x] = std::atoi(ir.y);
        if(ir.unit == FLOAT)
            float_value[ir.x] = std::atof(ir.y);
        if(ir.unit == DOUBLE)
            double_value[ir.x] = std::atod(ir.y);
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
            ir.type = CONST;
            ir.y = convertToString(x_value);
        }
        break;
    case 
    }
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
    int bb_max_num[bbs.size()];
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


//////////////////////////////////////
// // 定义表达式节点结构
// struct ExpressionNode {
//     std::string expression;
//     int result;
// };

// // 哈希表类型定义，将表达式作为键，节点作为值
// typedef std::unordered_map<std::string, ExpressionNode> ExpressionHash;

// // 查询公共子表达式
// int queryCommonExpression(const std::string& expression, ExpressionHash& hashTable) {
//     // 在哈希表中查找是否存在该表达式
//     auto it = hashTable.find(expression);
//     if (it != hashTable.end()) {
//         // 如果存在，则返回对应的结果
//         return it->second.result;
//     } else {
//         // 如果不存在，则计算表达式的结果，并将结果插入哈希表中
//         int result = evaluateExpression(expression);  // 假设有一个函数用于计算表达式结果
//         hashTable.emplace(expression, ExpressionNode{expression, result});
//         return result;
//     }
// }

// //控制流分析
// bool flow_analysis(std::string<basicblock>& bbs);
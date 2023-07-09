#include <iostream>
#include <map>
#include <string>
#include <cstring>
#include <sstream>
#include <fstream>
#include <vector>
#include <algorithm>
#include <cstdlib>
#include <map>

enum instr_kind {
    NONE,
    CONST, //常数赋值
    MOVE,  //数据移动
    BIOP,  //二元运算符
    UNOP,  //一元运算
    MEMWRITE,  //内存写
    MEMREAD,  // 内存读
    PARAM,
    CALL,  //函数调用
    CJMP,  //条件跳转
    JMP,  //无条件跳转
    LABEL,  // 标号，这个大概率使用不到了
    RETURN  //返回
};

enum instr_biop{
    NONEBIOP,
    ADD,
    SUB,
    TIMES,
    DIV,
    MOD,
    AND,
    OR,
    EQU,
    NOTEQU,
    GREATER,
    GREATER_EQU,
    LESS,
    LESS_EQU
};

enum instr_unop{
    NONEUNOP,
    POS,
    NEG,
    REVERSE
};

enum dag_op
{
    DAG_ADD,
    DAG_SUB,
    DAG_TIMES,
    DAG_DIV,
    DAG_MOD,
    DAG_AND,
    DAG_OR,
    DAG_EQU,
    DAG_NOTEQU,
    DAG_GREATER,
    DAG_GREATER_EQU,
    DAG_LESS,
    DAG_LESS_EQU,

    DAG_POS,
    DAG_NEG,
    DAG_REVERSE,

    DAG_ARREQU,
    DAG_EQUARR
};

enum unit
{
    NONEUNIT,
    INT,
    FLOAT,
    DOUBLE
}

struct instr_ir
{
    int num;
    unit unit;
    instr_kind kind;
    instr_biop biop;
    instr_unop unop;
    std::string x;
    std::string y;
    std::string z;
    char *notes;//备注
};


std::string convertToString(T value)；
char* intToCharPointer(int number);
bool get_ir(instr_ir ir,char*& output);
bool write_ir(std::vector<instr_ir>& ir, std::ofstream& outputfile);
bool output_ir(std::vector<instr_ir>& ir);

//基本块
struct basicblock{
    std::vector<instr_ir> bbir;
};

//建立cgf
class CFG{
private:
    std::vector<std::vector<int> > adjacencyMatrix;
    std::vector<basicblock> bbs;
public:
    CFG(std::vector<basicblock> bbsin) : adjacencyMatrix(bbsin.size(), std::vector<int>(bbsin.size(), 0)), bbs(bbsin) {}

    void addEdge(int source, int destination) {
        adjacencyMatrix[source][destination] = 1;
    }

    void removeEdge(int source, int destination) {
        adjacencyMatrix[source][destination] = 0;
    }

    void printGraph() {
        for (int i = 0; i < adjacencyMatrix.size(); ++i) {
            for (int j = 0; j < adjacencyMatrix[i].size(); ++j) {
                std::cout << adjacencyMatrix[i][j] << " ";
            }
            std::cout << std::endl;
        }
    }
};

// 定义表达式节点结构
struct ExpressionNode {
    std::string expression;
    ExpressionNode leftchild;
    ExpressionNode rightchild;
    dag_op op;
    int result; //是在dag里面代表的位置
};

//建立DAG
class DAG {
public:
    std::vector<ExpressionNode> Exps;
    int numNodes;
    std::vector<int> adjacencyList[];

    void addEdge(int source, int destination) {
        adjacencyList[source].push_back(destination);
    }

    void printGraph() {
        for (int i = 0; i < numNodes; ++i) {
            std::cout << "Node " << i << " -> ";
            for (int j=0; j<adjacencyList[i].size();j++) {
                std::cout << adjacencyList[i][j] << " ";
            }
            std::cout << std::endl;
        }
    }
};

// 查询公共子表达式
int queryCommonExpression(const std::string& expression, ExpressionHash& hashTable);


//寻找公共子表达式
bool CommonSubexp(basicblock* bb);

//定值传递
bool const_trans(instr_ir& ir);

//划分基本快建立cfg
CFG BuildCFG(basicblock bb,std::vector<basicblock>& bbs);
int find_in_bbs(int num,int bb_max_num[],int size);

basicblock test_bb();

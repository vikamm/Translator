#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <locale.h>
#include <stdio.h>
#include <stack>
#include <list>

using namespace std;

std::ifstream service_words;
std::ifstream code;

std::vector <std::string> Return_fuction;

std::vector <int> label_for_choice;

bool check_break = 0;
int label_for_break = 0;
bool check_continue = 0;
int label_for_continue = 0;

enum type_of_object {
    Integer,
    Line,
    Valid,
    Bool,
    Nothing,
    NoChar,
    Void
};

void Print(type_of_object object) {
    switch (object) {
    case Integer:
        std::cout << "Integer ";
        break;
    case Line:
        std::cout << "Line ";
        break;
    case Valid:
        std::cout << "Valid ";
        break;
    case Bool:
        std::cout << "Bool ";
        break;
    case Void:
        std::cout << "Void ";
        break;
    case NoChar:
        std::cout << "NoChar ";
        break;
    }
}

struct table {
    bool array;
    std::string name; // id
    double v_lex_int; //int, bool, double, char
    std::string v_lex_string; //string
    std::vector<double> v_lex_vector_int;
    std::vector<char> v_lex_vector_char;
    std::vector<std::string> v_lex_vector_string;
    type_of_object type;
    table(std::string name_, type_of_object type_, bool array_) {
        name = name_;
        type = type_;
        array = array_;
        v_lex_int = 0;
        v_lex_string = "";
    }
    table(std::string name_, type_of_object type_) {
        name = name_;
        type = type_;
        array = false;
        v_lex_int = 0;
        v_lex_string = "";
    }
    table(std::string name_) {
        name = name_;
        array = false;
        v_lex_int = 0;
        v_lex_string = "";
    }
    table() {
        name = "";
        array = false;
        v_lex_int = 0;
        v_lex_string = "";
    }
};

struct List_Struct {
    std::vector<table> TID;
    int level;
    List_Struct() = default;
    List_Struct(int level_) {
        level = level_;
    }
};

std::list <List_Struct> tid_list, tid_list_not_delete;

type_of_object Check_ID(std::string str) {
    std::list<List_Struct>::iterator it;
    for (it = tid_list.begin(); it != tid_list.end(); ++it) {
        for (int j = 0; j < it->TID.size(); ++j) {
            if (it->TID[j].name == str) {
                return it->TID[j].type;
            }
        }
    }
    return type_of_object::Nothing;
}

bool Check_Array(std::string str) {
    std::list<List_Struct>::iterator it;
    for (it = tid_list.begin(); it != tid_list.end(); ++it) {
        for (int j = 0; j < it->TID.size(); ++j) {
            if (it->TID[j].name == str) {
                return it->TID[j].array;
            }
        }
    }
    return 0;
}

int SizeOfArray(type_of_object type, std::string str) {
    std::list<List_Struct>::iterator it;
    for (it = tid_list.begin(); it != tid_list.end(); ++it) {
        for (int j = 0; j < it->TID.size(); ++j) {
            if (it->TID[j].name == str) {
                if (type == Integer || type == Valid || type == Bool) {
                    return it->TID[j].v_lex_vector_int.size();
                }
                if (type == Line) {
                    return it->TID[j].v_lex_vector_string.size();
                }
                if (type == NoChar) {
                    return it->TID[j].v_lex_vector_char.size();
                }
            }
        }
    }
    return 0;
}

/////

struct table_f {
    std::string name;
    type_of_object type;
    std::vector<std::pair<type_of_object, std::pair<std::string, bool>>> parameters;
    int label = 10000;
    std::stack<int> prev_label;

    table_f(std::string name_, type_of_object type_, std::vector<std::pair<type_of_object, std::pair<std::string, bool>>> parameters_) {
        name = name_;
        type = type_;
        parameters = parameters_;
    }
    table_f(std::string name_, type_of_object type_) {
        name = name_;
        type = type_;
    }
    table_f(std::string name_) {
        name = name_;
    }
    table_f() {
        name = "";
    }
};

struct List_Struct_F {
    std::vector<table_f> TID;
};

std::list <List_Struct_F> tid_list_f;
std::vector<std::pair<std::string, int>> label_function;

std::string GET_NAME_BY_POLIZ_INDEX(int index) {
    for (int i = 0; i < label_function.size(); ++i) {
        if (label_function[i].second > index) {
            return label_function[i - 1].first;
        }
    }
    return label_function[label_function.size() - 1].first;
}

int GET_RETURN_LABEL_OF_FUNCTION(std::string name_f) {
    std::list<List_Struct_F>::iterator it;
    for (it = tid_list_f.begin(); it != tid_list_f.end(); ++it) {
        for (int j = 0; j < it->TID.size(); ++j) {
            if (it->TID[j].name == name_f) {
                return it->TID[j].label;
            }
        }
    }
}
void CORRECT_LABLE_FOR_FUNCTION(std::string name_f, int correct_label) {
    std::list<List_Struct_F>::iterator it;
    for (it = tid_list_f.begin(); it != tid_list_f.end(); ++it) {
        for (int j = 0; j < it->TID.size(); ++j) {
            if (it->TID[j].name == name_f) {
                it->TID[j].prev_label.push(it->TID[j].label);
                it->TID[j].label = correct_label;
                return;
            }
        }
    }
}

type_of_object Check_ID_F(std::string str) {
    std::list<List_Struct_F>::iterator it;
    for (it = tid_list_f.begin(); it != tid_list_f.end(); ++it) {
        for (int j = 0; j < it->TID.size(); ++j) {
            if (it->TID[j].name == str) {
                return it->TID[j].type;
            }
        }
    }
    return type_of_object::Nothing;
}

////

std::vector<std::pair<type_of_object, std::pair<std::string, bool>>> GetParameters(std::string function_name) {
    std::list<List_Struct_F>::iterator it;
    for (it = tid_list_f.begin(); it != tid_list_f.end(); ++it) {
        for (int j = 0; j < it->TID.size(); ++j) {
            if (it->TID[j].name == function_name) {
                return it->TID[j].parameters;
            }
        }
    }
}

bool CheckIsParametersBeen(std::string function_name, std::string comparison_name) {
    std::list<List_Struct_F>::iterator it;
    for (it = tid_list_f.begin(); it != tid_list_f.end(); ++it) {
        for (int j = 0; j < it->TID.size(); ++j) {
            if (it->TID[j].name == function_name) {
                for (int k = 0; k < it->TID[j].parameters.size(); ++k) {
                    if (it->TID[j].parameters[k].second.first == comparison_name) {
                        return 0;
                    }
                }
            }
        }
    }
    return 1;
}

enum type_of_lexem {
    Arithmetic,
    Error,
    EndOfFile,
    ID,
    Comma,
    SpecialWords,
    Logical,
    Comparison, //сравнение
    Equal,
    Negation, //~
    Equally_NotEqually,
    Number,
    RoundBracket,
    CurlyBracket,
    TriangularBrackets,
    SquareBrackets,
    ExclamationMark, // !
    Point,
    String,
    Comment,
    Nothing0,
    Semicolon,
    NoChar0
};

enum Condition {
    Begin2,
    Word2,
    Number2,
    FractionalNumber2,
    Error2,
    Arithmetic2,
    Comma2,
    Point2,
    ComparisonLogical2,
    Equal2,
    Negation2, //~
    RoundBracket2,
    CurlyBracket2,
    ExclamationMark2, // !
    QuotationMarks2,
    ComparisonСomposite2, // ""
    Comment2
};

struct WrongRoundBracket_open {
    int str;
    std::string now_lex;
    WrongRoundBracket_open(int num, std::string get);
};

WrongRoundBracket_open::WrongRoundBracket_open(int num, std::string get) {
    now_lex = get;
    str = num;
}

struct WrongRoundBracket_close {
    int str;
    std::string now_lex;
    WrongRoundBracket_close(int num, std::string get);
};

WrongRoundBracket_close::WrongRoundBracket_close(int num, std::string get) {
    now_lex = get;
    str = num;
}

struct WrongCurlyBracket_open {
    int str;
    std::string now_lex;
    WrongCurlyBracket_open(int num, std::string get);
};

WrongCurlyBracket_open::WrongCurlyBracket_open(int num, std::string get) {
    now_lex = get;
    str = num;
}

struct WrongArraySize {
    int str;
    std::string now_lex;
    WrongArraySize(int num, std::string get);
};

WrongArraySize::WrongArraySize(int num, std::string get) {
    now_lex = get;
    str = num;
}

struct WrongCurlyBracket_close {
    int str;
    std::string now_lex;
    WrongCurlyBracket_close(int num, std::string get);
};

WrongCurlyBracket_close::WrongCurlyBracket_close(int num, std::string get) {
    now_lex = get;
    str = num;
}

struct WrongSquareBracket_open {
    int str;
    std::string now_lex;
    WrongSquareBracket_open(int num, std::string get);
};

WrongSquareBracket_open::WrongSquareBracket_open(int num, std::string get) {
    now_lex = get;
    str = num;
}

struct WrongSquareBracket_close {
    int str;
    std::string now_lex;
    WrongSquareBracket_close(int num, std::string get);
};

WrongSquareBracket_close::WrongSquareBracket_close(int num, std::string get) {
    now_lex = get;
    str = num;
}

struct Wrong_id {
    int str;
    std::string now_lex;
    Wrong_id(int num, std::string get);
};

Wrong_id::Wrong_id(int num, std::string get) {
    now_lex = get;
    str = num;
}

struct WrongSpecial_word_type {
    int str;
    std::string now_lex;
    WrongSpecial_word_type(int num, std::string get);
};

WrongSpecial_word_type::WrongSpecial_word_type(int num, std::string get) {
    now_lex = get;
    str = num;
}

struct WrongComprasion_sk1 {
    int str;
    std::string now_lex;
    WrongComprasion_sk1(int num, std::string get);
};

WrongComprasion_sk1::WrongComprasion_sk1(int num, std::string get) {
    now_lex = get;
    str = num;
}

struct WrongComprasion_sk2 {
    int str;
    std::string now_lex;
    WrongComprasion_sk2(int num, std::string get);
};

WrongComprasion_sk2::WrongComprasion_sk2(int num, std::string get) {
    now_lex = get;
    str = num;
}

struct WrongComprasion {
    int str;
    std::string now_lex;
    WrongComprasion(int num, std::string get);
};

WrongComprasion::WrongComprasion(int num, std::string get) {
    now_lex = get;
    str = num;
}

struct Wrong_arit {
    int str;
    std::string now_lex;
    Wrong_arit(int num, std::string get);
};

Wrong_arit::Wrong_arit(int num, std::string get) {
    now_lex = get;
    str = num;
}

struct Wrong_type {
    int num = 0;
    type_of_object str1, str2;
    Wrong_type(int num, type_of_object str_now1, type_of_object str_now2);
};

Wrong_type::Wrong_type(int num_now, type_of_object str_now1, type_of_object str_now2) {
    num = num_now;
    str1 = str_now1;
    str2 = str_now2;
}

struct Wrong_field_of_view {
    int num = 0;
    std::string str;
    Wrong_field_of_view(int num, std::string now_str);
};

Wrong_field_of_view::Wrong_field_of_view(int num_now, std::string now_str) {
    num = num_now;
    str = now_str;
}

struct Wrong_Array_cell {
    int num = 0;
    type_of_object type1;
    Wrong_Array_cell(int num, type_of_object now_type1);
};

Wrong_Array_cell::Wrong_Array_cell(int num_now, type_of_object now_type1) {
    num = num_now;
    type1 = now_type1;
}

struct Token {
    type_of_lexem type;
    std::string name;
    int number_str;
    Token() {
        name = "";
        type = type_of_lexem::Nothing0;
        number_str = 0;
    }
    Token(std::string str, type_of_lexem type_lex, int number, int value_ = 0) {
        name = str;
        type = type_lex;
        number_str = number;
    }

    void Print() {
        switch (type) {
        case Arithmetic:
            std::cout << "Arithmetic";
            break;
        case Error:
            std::cout << "Unknown sign";
            break;
        case EndOfFile:
            std::cout << "EndOfFile";
            break;
        case ID:
            std::cout << "ID";
            break;
        case Comma:
            std::cout << "Comma";
            break;
        case SpecialWords:
            std::cout << "SpecialWords";
            break;
        case Logical:
            std::cout << "Logical";
            break;
        case Comparison:
            std::cout << "Comparison";
            break;
        case Equal:
            std::cout << "Equal";
            break;
        case Number:
            std::cout << "Number";
            break;
        case RoundBracket:
            std::cout << "RoundBracket";
            break;
        case CurlyBracket:
            std::cout << "CurlyBracket";
            break;
        case TriangularBrackets:
            std::cout << "TriangularBrackets";
            break;
        case ExclamationMark:
            std::cout << "ExclamationMark";
            break;
        case Point:
            std::cout << "Point";
            break;
        case String:
            std::cout << "String";
            break;
        case Comment:
            std::cout << "Comment";
            break;
        case Negation:
            std::cout << "Negation";
            break;
        case Equally_NotEqually:
            std::cout << "Equally_NotEqually";
            break;
        case SquareBrackets:
            std::cout << "SquareBrackets";
        case Semicolon:
            std::cout << "Semicolon";
        }
    }
};

struct WrongSpecial_word {
    int str;
    std::string now_lex;
    WrongSpecial_word(int num, std::string get);
};

WrongSpecial_word::WrongSpecial_word(int num, std::string get) {
    now_lex = get;
    str = num;
}

struct WrongSpecial_word_type_bool {
    int str;
    std::string now_lex;
    WrongSpecial_word_type_bool(int num, std::string get);
};

WrongSpecial_word_type_bool::WrongSpecial_word_type_bool(int num, std::string get) {
    now_lex = get;
    str = num;
}

struct WrongEqual {
    int str;
    std::string now_lex;
    WrongEqual(int num, std::string get);
};

WrongEqual::WrongEqual(int num, std::string get) {
    now_lex = get;
    str = num;
}

struct WrongExMark {
    int str;
    std::string now_lex;
    WrongExMark(int num, std::string get);
};

WrongExMark::WrongExMark(int num, std::string get) {
    now_lex = get;
    str = num;
}

struct WrongSpecial_word_len {
    int str;
    std::string now_lex;
    WrongSpecial_word_len(int num, std::string get);
};

WrongSpecial_word_len::WrongSpecial_word_len(int num, std::string get) {
    now_lex = get;
    str = num;
}

struct Wrong_String {
    int str;
    std::string now_lex;
    Wrong_String(int num, std::string get);
};

Wrong_String::Wrong_String(int num, std::string get) {
    now_lex = get;
    str = num;
}

struct WrongSpecialWord_choice {
    int str;
    std::string now_lex;
    WrongSpecialWord_choice(int num, std::string get);
};
WrongSpecialWord_choice::WrongSpecialWord_choice(int num, std::string get) {
    now_lex = get;
    str = num;
}

struct WrongEqually_NotEqually {
    int str;
    std::string now_lex;
    WrongEqually_NotEqually(int num, std::string get);
};
WrongEqually_NotEqually::WrongEqually_NotEqually(int num, std::string get) {
    now_lex = get;
    str = num;
}

struct  WrongPlus {
    int str;
    std::string now_lex;
    WrongPlus(int num, std::string get);
};
WrongPlus::WrongPlus(int num, std::string get) {
    now_lex = get;
    str = num;
}

struct  Wrong_pop_add {
    int str;
    std::string now_lex;
    Wrong_pop_add(int num, std::string get);
};
Wrong_pop_add::Wrong_pop_add(int num, std::string get) {
    now_lex = get;
    str = num;
}

struct  WrongOverFlow {
    WrongOverFlow();
};
WrongOverFlow::WrongOverFlow() {
}

struct  WrongOperationObject {
    int str;
    std::string now_lex;
    WrongOperationObject(int num, std::string get);
};
WrongOperationObject::WrongOperationObject(int num, std::string get) {
    now_lex = get;
    str = num;
}

struct  WrongNumber {
    int str;
    std::string now_lex;
    WrongNumber(int num, std::string get);
};
WrongNumber::WrongNumber(int num, std::string get) {
    now_lex = get;
    str = num;
}

struct  WrongVariable {
    int str;
    std::string now_lex;
    WrongVariable(int num, std::string get);
};
WrongVariable::WrongVariable(int num, std::string get) {
    now_lex = get;
    str = num;
}

struct  WrongInitialization {
    int num;
    std::string str;
    WrongInitialization(int num, std::string str);
};
WrongInitialization::WrongInitialization(int num_now, std::string str_now) {
    str = str_now;
    num = num_now;
}

struct  WrongCount_parametries {
    int num;
    WrongCount_parametries(int num);
};
WrongCount_parametries::WrongCount_parametries(int num_now) {
    num = num_now;
}

struct  WrongType_parametries {
    int num;
    WrongType_parametries(int num);
};
WrongType_parametries::WrongType_parametries(int num_now) {
    num = num_now;
}

struct  WrongArray {
    std::string str;
    int num;
    WrongArray(int num, std::string str_now);
};
WrongArray::WrongArray(int num_now, std::string str_now) {
    num = num_now;
    str = str_now;
}

struct  WrongArray_name {
    std::string str;
    int num;
    WrongArray_name(int num, std::string str_now);
};
WrongArray_name::WrongArray_name(int num_now, std::string str_now) {
    num = num_now;
    str = str_now;
}

struct  WrongNo_return {
    std::string str;
    WrongNo_return(std::string str_now);
};
WrongNo_return::WrongNo_return(std::string str_now) {
    str = str_now;
}

struct  WrongArray_arith {
    int num;
    WrongArray_arith(int num);
};
WrongArray_arith::WrongArray_arith(int now_num) {
    num = now_num;
}

struct  WrongVoid {
    std::string str;
    WrongVoid(std::string str_now);
};
WrongVoid::WrongVoid(std::string str_now) {
    str = str_now;
}

struct  WrongReturn_array {
    std::string str;
    WrongReturn_array(std::string str_now);
};
WrongReturn_array::WrongReturn_array(std::string str_now) {
    str = str_now;
}

struct  WrongChar_arith {
    int num;
    WrongChar_arith(int str_now);
};
WrongChar_arith::WrongChar_arith(int str_now) {
    num = str_now;
}

struct  WrongTryDelete {
    std::string name;
    WrongTryDelete(std::string name_);
};
WrongTryDelete::WrongTryDelete(std::string name_) {
    name = name_;
}

struct Wrong_ArrayIndexOut {
    std::string now;
    Wrong_ArrayIndexOut(std::string name);
};

Wrong_ArrayIndexOut::Wrong_ArrayIndexOut(std::string name) {
    now = name;
}

enum PolizElement {
    NULL_P,
    ARRAY_INT_VALID_BOOL,
    ARRAY_NOCHAR,
    ARRAY_LINE,
    TRUE,
    FALSE,
    OUTPUT,
    INPUT,
    ADD,
    POP,
    LEN,
    UNOPLUS,
    UNOMINUS,
    UNODENY,
    PLUS,
    MINUS,
    DIVISION,
    MULTIPLY,
    REMAINDIVISION,
    IDENTIF,
    INT_DOUBLE_BOOL,
    STRING,
    CHAR,
    COMMA,
    OR,
    AND,
    MORE,
    LESS,
    MORE_EQUAL,
    LESS_EQUAL,
    EQUAL,
    YESEQUAL,
    NOTEQUAL,
    INDEX,
    GOTO,
    GOTOCALLF,
    GOTOFALSE,
    LABEL,
    LABEL_F
};

class Lex
{
public:
    PolizElement t_lex;
    std::string name; // id
    double v_lex_int; //int, bool, double, char
    std::string v_lex_string; //string
    std::vector<double> v_lex_vector_int;
    std::vector<char> v_lex_vector_char;
    std::vector<std::string> v_lex_vector_string;
    Lex() = default;
    Lex(PolizElement t) {
        t_lex = t;
        v_lex_int = 0;
    }
    Lex(PolizElement t, bool v) {
        t_lex = t;
        v_lex_int = v ? 1.0 : 0.0;
    }
    Lex(PolizElement t, double v) {
        t_lex = t; v_lex_int = v;
    }
    Lex(PolizElement t, std::string v) {
        t_lex = t; v_lex_string = v;
    }
    Lex(PolizElement t, std::string v, bool p) {
        t_lex = t; name = v;
    }
    Lex(PolizElement t, std::vector<double> v) {
        t_lex = t; v_lex_vector_int = v;
    }
    Lex(PolizElement t, std::vector<std::string> v) {
        t_lex = t; v_lex_vector_string = v;
    }
    Lex(PolizElement t, std::vector<char> v) {
        t_lex = t; v_lex_vector_char = v;
    }
    PolizElement get_type() { return t_lex; }
};
int T = 0;

void LexemWord(PolizElement el) {
    std::cout << "(" << T++ << ")";
    if (el == EQUAL) {
        std::cout << "= | ";
    }
    if (el == INT_DOUBLE_BOOL) {
        std::cout << "number | ";
    }
    if (el == IDENTIF) {
        std::cout << "id | ";
    }
    if (el == STRING) {
        std::cout << "string | ";
    }
    if (el == INDEX) {
        std::cout << "[] | ";
    }
    if (el == TRUE) {
        std::cout << "true | ";
    }
    if (el == FALSE) {
        std::cout << "false | ";
    }
    if (el == MORE) {
        std::cout << "> | ";
    }
    if (el == LESS) {
        std::cout << "< | ";
    }
    if (el == MORE_EQUAL) {
        std::cout << ">= | ";
    }
    if (el == LESS_EQUAL) {
        std::cout << "<= | ";
    }
    if (el == PLUS) {
        std::cout << "+ | ";
    }
    if (el == MINUS) {
        std::cout << "- | ";
    }
    if (el == MULTIPLY) {
        std::cout << "* | ";
    }
    if (el == DIVISION) {
        std::cout << "/ | ";
    }
    if (el == REMAINDIVISION) {
        std::cout << "% | ";
    }
    if (el == UNODENY) {
        std::cout << "~ | ";
    }
    if (el == UNOPLUS) {
        std::cout << "uno+ | ";
    }
    if (el == UNOMINUS) {
        std::cout << "uno- | ";
    }
    if (el == INPUT) {
        std::cout << "IN | ";
    }
    if (el == OUTPUT) {
        std::cout << "OUT | ";
    }
    if (el == LABEL) {
        std::cout << "label | ";
    }
    if (el == LABEL_F) {
        std::cout << "label_f | ";
    }
    if (el == GOTO) {
        std::cout << "!P | ";
    }
    if (el == GOTOCALLF) {
        std::cout << "!P(F) | ";
    }
    if (el == GOTOFALSE) {
        std::cout << "!F | ";
    }
    if (el == ADD) {
        std::cout << "push | ";
    }
    if (el == POP) {
        std::cout << "pop | ";
    }
    if (el == YESEQUAL) {
        std::cout << "== | ";
    }
    if (el == NOTEQUAL) {
        std::cout << "~= | ";
    }
}

class Poliz
{
    Lex* p;
    int size;
    int free;
public:
    Poliz() = default;
    Poliz(int max_size) {
        size = max_size;
        p = new Lex[size];
        free = 0;
    };
    ~Poliz() { delete[]p; };
    void put_lex(Lex l) { p[free] = l; ++free; };
    void put_lex(Lex l, int place) { p[place] = l; };
    void blank() { ++free; };
    void antiblank() { --free; }
    int get_free() { return free; };
    Lex& operator[] (int index) {
        if (index > size) {
            throw "POLIZ:out of array";
        }
        else {
            if (index > free) {
                throw "POLIZ:indefinite element of array";
            }
            else {
                return p[index];
            }
        }
    };
    void print() {
        for (int i = 0; i < free; ++i) {
            if (p[i].t_lex == LABEL) {
                std::cout << p[i].v_lex_int;
            }
            if (p[i].t_lex == LABEL_F) {
                //p[i].v_lex_int = GET_RETURN_LABEL_OF_FUNCTION(GET_NAME_BY_POLIZ_INDEX(i));
                std::cout << p[i].v_lex_int;
            }
            LexemWord(p[i].t_lex);
        }
    };
};

int GET_POLIZ_INDEX_OF_FUNCTION(std::string str) {
    for (int i = 0; i < label_function.size(); ++i) {
        if (label_function[i].first == str) {
            return label_function[i].second;
        }
    }
    return 0;
}

Poliz POLIZ(100000);

PolizElement GetPolizElement(std::string str) {
    std::list<List_Struct>::iterator it;
    for (it = tid_list_not_delete.begin(); it != tid_list_not_delete.end(); ++it) {
        for (int j = 0; j < it->TID.size(); ++j) {
            if (it->TID[j].name == str) {
                if (it->TID[j].type == Integer || it->TID[j].type == Valid || it->TID[j].type == Bool) {
                    if (it->TID[j].array) {
                        return ARRAY_INT_VALID_BOOL;
                    }
                    else {
                        return INT_DOUBLE_BOOL;
                    }
                }
                if (it->TID[j].type == NoChar) {
                    if (it->TID[j].array) {
                        return ARRAY_NOCHAR;
                    }
                    else {
                        return CHAR;
                    }
                }
                if (it->TID[j].type == Line) {
                    if (it->TID[j].array) {
                        return  ARRAY_LINE;
                    }
                    else {
                        return STRING;
                    }
                }
            }
        }
    }
    return NULL_P;
}

struct Node {
    Node* next[26];
    int strings;
    Node() {
        for (int i = 0; i < 26; i++) {
            next[i] = nullptr;
        }
        strings = 0;
    }
};

Node* root = new Node();

void push(const std::string& s) {
    Node* current = root;
    for (int i = 0; i < s.length(); i++) {
        char c = s[i];
        if (current->next[c - 'a'] == nullptr) {
            current->next[c - 'a'] = new Node();
        }
        current = current->next[c - 'a'];
    }
    current->strings++;
}

bool be_here(const std::string& s) {
    Node* current = root;
    for (int i = 0; i < s.length(); i++) {
        if (!((s[i] >= 'a' && s[i] <= 'z') || ((s[i] >= 'A' && s[i] <= 'Z')))) {
            return false;
        }
        current = current->next[s[i] - (s[i] >= 'a' && s[i] <= 'z' ? 'a' : 'A')];
        if (current == nullptr) {
            return false;
        }
    }
    return current->strings > 0;
}

char ch;
int vector_index = 0;
std::vector<Token> tockens;

Token get_tocken() {
    if (vector_index < tockens.size()) {
        return tockens[vector_index++];
    }
    else {
        throw WrongOverFlow();
    }
}

class Parser {
public:
    Token curr_lex;
    type_of_lexem c_type;
    int c_val;
    std::string c_str;
    int c_number_str;
    std::stack < int > st_int[100];
    std::stack < type_of_lexem> st_lex[100];
    void program();
    void operator_();
    void function();
    void enumeration_type();
    void enumeration_arithmetic(std::vector<std::pair<type_of_object, std::pair<std::string, bool>>> parameters);
    void bool_(bool print = false);
    void assignment();
    void internal_operator();
    void selection(Lex MAIN_lex);
    std::pair<type_of_object, bool> priority_level1(bool print = false);
    std::pair<type_of_object, bool> priority_level2(bool print = false);
    std::pair<type_of_object, bool> priority_level3(bool print = false);
    std::pair<type_of_object, bool> priority_level4(bool print = false);
    std::pair<type_of_object, bool> priority_level5(bool print = false);
    std::pair<type_of_object, bool> priority_level6(bool print = false);
    std::pair<type_of_object, bool> priority_level7(bool print = false);
    std::pair<type_of_object, bool> priority_level8(bool print = false);
    std::pair<type_of_object, bool> operation_object(bool print = false);
    void multiple_definition();

    void get_lex() {
        curr_lex = get_tocken();
        c_type = curr_lex.type;
        c_str = curr_lex.name;
        c_number_str = curr_lex.number_str;
    }

public:
    Parser() : c_type(type_of_lexem::Nothing0), c_val(0), c_number_str(0) {}
    void analyze();
};

int IndexNow = 1;

void Parser::analyze() {
    get_lex();
    try {
        program();
    }
    catch (const WrongRoundBracket_open& ex) {
        std::cout << "В строке " << ex.str << " ожидалась круглая открывающаяся скобка!" << std::endl << "Получено: '" << ex.now_lex << "'.";
        return;
    }
    catch (const WrongArraySize& ex) {
        std::cout << "В строке " << ex.str << " ожидалось число для объявления массива!" << std::endl << "Получено: '" << ex.now_lex << "'.";
        return;
    }
    catch (const WrongRoundBracket_close& ex) {
        std::cout << "В строке " << ex.str << " ожидалась круглая закрывающаяся скобка!" << std::endl << "Получено: '" << ex.now_lex << "'.";
        return;
    }
    catch (const WrongCurlyBracket_open& ex) {
        std::cout << "В строке " << ex.str << " ожидалась фигурная открывающаяся скобка!" << std::endl << "Получено: '" << ex.now_lex << "'.";
        return;
    }
    catch (const WrongCurlyBracket_close& ex) {
        std::cout << "В строке " << ex.str << " ожидалась фигурная закрывающаяся скобка!" << std::endl << "Получено: '" << ex.now_lex << "'.";
        return;
    }
    catch (const WrongSquareBracket_open& ex) {
        std::cout << "В строке " << ex.str << " ожидалась квадратная открывающаяся скобка!" << std::endl << "Получено: '" << ex.now_lex << "'.";
        return;
    }
    catch (const WrongSquareBracket_close& ex) {
        std::cout << "В строке " << ex.str << " ожидалась квадратная закрывающаяся скобка!" << std::endl << "Получено: '" << ex.now_lex << "'.";
        return;
    }
    catch (const Wrong_id& ex) {
        std::cout << "В строке " << ex.str << " ожидался индитификатор!" << std::endl << "Получено: '" << ex.now_lex << "'.";
        return;
    }
    catch (const WrongSpecial_word_type& ex) {
        std::cout << "В строке " << ex.str << " ожидался тип (integer, bool, line, valid, nochar)!" << std::endl << "Получено: '" << ex.now_lex << "'.";
        return;
    }
    catch (const WrongSpecial_word_type_bool& ex) {
        std::cout << "В строке " << ex.str << " ожидались bool-значения (true или false)!" << std::endl << "Получено: '" << ex.now_lex << "'.";
        return;
    }
    catch (const WrongSpecial_word& ex) {
        std::cout << "В строке " << ex.str << " ожидалось специальное слово!" << std::endl << "Получено: '" << ex.now_lex << "'.";
        return;
    }
    catch (const WrongComprasion_sk2& ex) {
        std::cout << "В строке " << ex.str << " ожидался символ '>' !" << std::endl << "Получено: '" << ex.now_lex << "'.";
        return;
    }
    catch (const WrongComprasion_sk1& ex) {
        std::cout << "В строке " << ex.str << " ожидался символ '<' !" << std::endl << "Получено: '" << ex.now_lex << "'.";
        return;
    }
    catch (const WrongComprasion& ex) {
        std::cout << "В строке " << ex.str << " ожидалась операция сравнения!" << std::endl << "Получено: '" << ex.now_lex << "'.";
        return;
    }
    catch (const WrongEqual& ex) {
        std::cout << "В строке " << ex.str << " ожидался символ '=' !" << std::endl << "Получено: '" << ex.now_lex << "'.";
        return;
    }
    catch (const WrongExMark& ex) {
        std::cout << "В строке " << ex.str << " ожидался символ '!' !" << std::endl << "Получено: '" << ex.now_lex << "'.";
        return;
    }
    catch (const WrongSpecial_word_len& ex) {
        std::cout << "В строке " << ex.str << " ожидалось специальное слово 'len' !" << std::endl << "Получено: '" << ex.now_lex << "'.";
        return;
    }
    catch (const Wrong_String& ex) {
        std::cout << "В строке " << ex.str << " ожидалась строка!" << std::endl << "Получено: '" << ex.now_lex << "'.";
        return;
    }
    catch (const  WrongSpecialWord_choice& ex) {
        std::cout << "В строке " << ex.str << " ожидалось специальное слово 'choice' !" << std::endl << "Получено: '" << ex.now_lex << "'.";
        return;
    }
    catch (const  WrongEqually_NotEqually& ex) {
        std::cout << "В строке " << ex.str << " ожидалось равенство или неравенство!" << std::endl << "Получено: '" << ex.now_lex << "'.";
        return;
    }
    catch (const  WrongPlus& ex) {
        std::cout << "В строке " << ex.str << " ожидалась операция '+' !" << std::endl << "Получено: '" << ex.now_lex << "'.";
        return;
    }
    catch (const  Wrong_pop_add& ex) {
        std::cout << "В строке " << ex.str << " ожидалась операция 'add' или 'pop' !" << std::endl << "Получено: '" << ex.now_lex << "'.";
        return;
    }
    catch (const  Wrong_arit& ex) {
        std::cout << "В строке " << ex.str << " ожидалась арифметика!" << std::endl << "Получено: '" << ex.now_lex << "'.";
        return;
    }
    catch (const  WrongOverFlow& ex) {
        std::cout << "Не обнаружено символа конца программы!";
        return;
    }
    catch (const WrongOperationObject& ex) {
        std::cout << "В строке " << ex.str << " ожидался объект арифметики!" << std::endl << "Получено: '" << ex.now_lex << "'.";
        return;
    }
    catch (const WrongNumber& ex) {
        std::cout << "В строке " << ex.str << " ожидалось число!" << std::endl << "Получено: '" << ex.now_lex << "'.";
        return;
    }
    catch (const WrongVariable& ex) {
        std::cout << "В строке " << ex.str << " ошибка в работе с переменной!" << std::endl << "Получено: '" << ex.now_lex << "'.";
        return;
    }
    catch (const Wrong_type& ex) {
        std::cout << "В строке " << ex.num << " несоответствие типов данных!" << " ..... ";
        Print(ex.str1);
        std::cout << "~<type>~ ";
        Print(ex.str2);
        std::cout << " ..... " << std::endl;
        return;
    }
    catch (const Wrong_field_of_view& ex) {
        std::cout << "В строке " << ex.num << " множественное объявление объекта " << ex.str << "!";
        return;
    }
    catch (const WrongInitialization& ex) {
        std::cout << "В строке " << ex.num << " используется не объявленный идентификатор " << ex.str << "!";
        return;
    }
    catch (const Wrong_Array_cell& ex) {
        std::cout << "В строке " << ex.num << " обнаружен некорректный тип для обращения к ячейке массива!" << std::endl;
        std::cout << "Полученный тип: ";
        Print(ex.type1);
        return;
    }
    catch (const  WrongCount_parametries& ex) {
        std::cout << "В строке " << ex.num << " обнаружено несоответствие количества формальных и фактических параметров в функции!";
        return;
    }
    catch (const  WrongType_parametries& ex) {
        std::cout << "В строке " << ex.num << " обнаружено несоответствие типов формальных и фактических параметров в функции!";
        return;
    }
    catch (const  WrongArray& ex) {
        std::cout << "В строке " << ex.num << " обнаружено некорректное использование массива '" << ex.str << "'!";
        return;
    }
    catch (const  WrongArray_name& ex) {
        std::cout << "В строке " << ex.num << " используется не обнаруженный массив '" << ex.str << "'!";
        return;
    }
    catch (const  WrongNo_return& ex) {
        std::cout << "Функция '" << ex.str << "' должна возвращать значение!";
        return;
    }
    catch (const  WrongArray_arith& ex) {
        std::cout << "В строке " << ex.num << " вы пытаетесь совершить неприменимую к массиву операцию!";
        return;
    }
    catch (const  WrongVoid& ex) {
        std::cout << "У void-функции '" << ex.str << "' не должно быть возвращаемого значения!";
        return;
    }
    catch (const WrongReturn_array& ex) {
        std::cout << "В функции '" << ex.str << "' обнаружена попытка вернуть массив!";
        return;
    }
    catch (const WrongChar_arith& ex) {
        std::cout << "В строке '" << ex.num << "' обнаружено некорректное использование типа 'nochar'!";
        return;
    }

    std::list<List_Struct_F>::iterator it;
    bool check_main = 0;
    for (it = tid_list_f.begin(); it != tid_list_f.end(); ++it) {
        for (int j = 0; j < it->TID.size(); ++j) {
            if (it->TID[j].name == "main") {
                check_main = true;
            }
            bool check = 0;
            for (int i = 0; i < Return_fuction.size(); ++i) {
                if (it->TID[j].name == Return_fuction[i]) {
                    check = 1;
                    break;
                }
            }
            if (!check) {
                std::cout << "Функция '" << it->TID[j].name << "' должна возвращать значение!";
                return;
            }
        }
    }
    if (!check_main) {
        std::cout << "Не обнаружена главная функция main!";
    }
    else {
        std::cout << std::endl << "Корректная программа!" << std::endl;
        POLIZ.print();
    }
}

void Parser::program() {
    if (c_type == EndOfFile) {
        return;
    }
    else {
        function();
    }
}

void Parser::function() {
    type_of_object type_now;
    std::string name_object;
    if (c_type == SpecialWords) {
        if (c_str == "integer" || c_str == "bool" || c_str == "line" || c_str == "valid" || c_str == "nochar") {
            if (c_str == "integer") {
                type_now = Integer;
            }
            if (c_str == "bool") {
                type_now = Bool;
            }
            if (c_str == "line") {
                type_now = Line;
            }
            if (c_str == "valid") {
                type_now = Valid;
            }
            if (c_str == "nochar") {
                type_now = NoChar;
            }
            get_lex();
            if (c_type == ID) {
                Lex lexic(IDENTIF, c_str, true);
                name_object = c_str;
                if (!(Check_ID(c_str) == type_of_object::Nothing && Check_ID_F(c_str) == type_of_object::Nothing)) {
                    throw Wrong_field_of_view(c_number_str, c_str);
                }
                get_lex();
                if (c_type == RoundBracket && c_str == "(") {
                    label_function.push_back({ name_object, POLIZ.get_free() });
                    tid_list_f.back().TID.push_back(table_f(name_object, type_now));
                    get_lex();
                    tid_list.push_back(List_Struct());
                    tid_list_not_delete.push_back(List_Struct(IndexNow));
                    IndexNow++;
                    enumeration_type();
                    if (c_type == RoundBracket && c_str == ")") {
                        get_lex();
                        if (c_type == CurlyBracket && c_str == "{") {
                            get_lex();
                            operator_();
                            if (c_type == CurlyBracket && c_str == "}") {
                                tid_list.pop_back();
                                --IndexNow;
                                get_lex();
                                if (c_type == EndOfFile) {
                                    return;
                                }
                                else {
                                    function();
                                }
                            }
                            else {
                                throw WrongCurlyBracket_close(c_number_str, c_str);;
                            }
                        }
                        else {
                            throw WrongCurlyBracket_open(c_number_str, c_str);
                        }
                    }
                    else {
                        throw WrongRoundBracket_close(c_number_str, c_str);
                    }
                }
                else {
                    tid_list.back().TID.push_back(table(name_object, type_now));
                    tid_list_not_delete.back().TID.push_back(table(name_object, type_now));
                    if (c_type == SquareBrackets && c_str == "[") {
                        tid_list.back().TID[tid_list.back().TID.size() - 1].array = true;
                        tid_list_not_delete.back().TID[tid_list_not_delete.back().TID.size() - 1].array = true;
                        get_lex();
                        if (c_type != Number) {
                            throw WrongArraySize(c_number_str, c_str);
                        }
                        type_of_object type_2 = Integer;
                        if (type_2 != Integer) {
                            throw Wrong_Array_cell(c_number_str, type_2);
                        }
                        get_lex();
                        if (c_type == SquareBrackets && c_str == "]") {
                            get_lex();
                        }
                        else {
                            throw WrongSquareBracket_close(c_number_str, c_str);
                        }
                    }
                    if (c_type == Equal) {
                        if (tid_list.back().TID[tid_list.back().TID.size() - 1].array) {
                            throw WrongArray(c_number_str, tid_list.back().TID[tid_list.back().TID.size() - 1].name);
                        }
                        POLIZ.put_lex(lexic);
                        get_lex();
                        priority_level1(true);
                        Lex lexic2(EQUAL);
                        POLIZ.put_lex(lexic2);
                    }
                    while (c_type == Semicolon) {
                        get_lex();
                        if (c_type == ID) {
                            Lex lexic3(IDENTIF, c_str, true);
                            if (Check_ID(c_str) == type_of_object::Nothing) {
                                tid_list.back().TID.push_back(table(c_str, type_now));
                                tid_list_not_delete.back().TID.push_back(table(c_str, type_now));
                            }
                            else {
                                throw WrongInitialization(c_number_str, c_str);
                            }
                            get_lex();
                            if (c_type == Equal) {
                                if (tid_list.back().TID[tid_list.back().TID.size() - 1].array) {
                                    throw WrongArray(c_number_str, tid_list.back().TID[tid_list.back().TID.size() - 1].name);
                                }
                                POLIZ.put_lex(lexic3);
                                get_lex();
                                priority_level1(true);
                                Lex lexic2(EQUAL);
                                POLIZ.put_lex(lexic2);
                            }
                        }
                        else {
                            throw Wrong_id(c_number_str, c_str);
                        }
                    }
                    if (c_type == ExclamationMark) {
                        get_lex();
                    }
                    else {
                        throw WrongExMark(c_number_str, c_str);
                    }
                    if (c_type == EndOfFile) {
                        return;
                    }
                    else {
                        function();
                    }
                }
            }
            else {
                throw Wrong_id(c_number_str, c_str);
            }
        }
        else {
            if (c_str == "void") {
                type_now = Void;
                get_lex();
                if (c_type == ID) {
                    label_function.push_back({ c_str, POLIZ.get_free() });
                    if (Check_ID_F(c_str) == type_of_object::Nothing && Check_ID(c_str) == type_of_object::Nothing) {
                        tid_list_f.back().TID.push_back(table_f(c_str, type_now));
                    }
                    else {
                        throw WrongInitialization(c_number_str, c_str);
                    }
                    get_lex();
                    if (c_type == RoundBracket && c_str == "(") {
                        get_lex();
                        tid_list.push_back(List_Struct());
                        tid_list_not_delete.push_back(List_Struct(IndexNow));
                        IndexNow++;
                        enumeration_type();
                        if (c_type == RoundBracket && c_str == ")") {
                            get_lex();
                            if (c_type == CurlyBracket && c_str == "{") {
                                get_lex();
                                operator_();
                                if (c_type == CurlyBracket && c_str == "}") {
                                    tid_list.pop_back();
                                    IndexNow--;
                                    get_lex();
                                    if (c_type == EndOfFile) {
                                        return;
                                    }
                                    else {
                                        function();
                                    }
                                }
                                else {
                                    throw WrongCurlyBracket_close(c_number_str, c_str);;
                                }
                            }
                            else {
                                throw WrongCurlyBracket_open(c_number_str, c_str);
                            }
                        }
                        else {
                            throw WrongRoundBracket_close(c_number_str, c_str);
                        }
                    }
                    else {
                        throw WrongRoundBracket_open(c_number_str, c_str);
                    }
                }
                else {
                    throw Wrong_id(c_number_str, c_str);
                }
            }
            else {
                throw WrongSpecial_word_type(c_number_str, c_str);
            }
        }
    }
    else {
        throw WrongSpecial_word_type(c_number_str, c_str);
    }
}

void Parser::enumeration_type() {
    type_of_object type_now;
    if (c_type == SpecialWords) {
        if (c_str == "integer" || c_str == "bool" || c_str == "line" || c_str == "valid" || c_str == "nochar") {
            if (c_str == "integer") {
                type_now = Integer;
            }
            if (c_str == "bool") {
                type_now = Bool;
            }
            if (c_str == "line") {
                type_now = Line;
            }
            if (c_str == "valid") {
                type_now = Valid;
            }
            if (c_str == "nochar") {
                type_now = NoChar;
            }
            get_lex();
            if (c_type == ID) {
                if (Check_ID(c_str) == type_of_object::Nothing) {
                    tid_list_f.back().TID.back().parameters.push_back({ type_now, {c_str, 0} });
                    tid_list.back().TID.push_back(table(c_str, type_now));
                    tid_list_not_delete.back().TID.push_back(table(c_str, type_now));
                }
                else {
                    throw WrongInitialization(c_number_str, c_str);
                }
                get_lex();
                if (c_type == SquareBrackets && c_str == "[") {
                    tid_list_f.back().TID.back().parameters.back().second.second = true;
                    tid_list.back().TID[tid_list.back().TID.size() - 1].array = true;
                    tid_list_not_delete.back().TID[tid_list_not_delete.back().TID.size() - 1].array = true;
                    get_lex();
                    if (c_type != Number) {
                        throw WrongArraySize(c_number_str, c_str);
                    }
                    //std::pair<type_of_object, bool> now = priority_level1();
                    //type_of_object type_2 = now.first;
                    type_of_object type_2 = Integer;
                    if (type_2 != Integer) {
                        throw Wrong_Array_cell(c_number_str, type_2);
                    }
                    get_lex();
                    if (c_type == SquareBrackets && c_str == "]") {
                        get_lex();
                    }
                    else {
                        throw WrongSquareBracket_close(c_number_str, c_str);
                    }
                }
                if (c_type == Semicolon) {
                    get_lex();
                    enumeration_type();
                }
            }
            else {
                throw Wrong_id(c_number_str, c_str);
            }
        }
        else {
            throw WrongSpecial_word_type(c_number_str, c_str);
        }
    }
    else {
        if (c_type == RoundBracket && c_str == ")") {
            return;
        }
        else {
            throw WrongSpecial_word_type(c_number_str, c_str);
        }
    }
}

void Parser::enumeration_arithmetic(std::vector<std::pair<type_of_object, std::pair<std::string, bool>>> parameters) {
    if (c_str == ")") {
        return;
    }
    int index = 0;
    int number_str_fuction = c_number_str;
    std::pair<type_of_object, bool> now = priority_level1(true);
    type_of_object type_now = now.first;
    bool check_array_now = now.second;
    if (parameters.size() < index + 1) {
        throw WrongCount_parametries(number_str_fuction);
    }
    if (type_now == Line && (parameters[index].first != Line) || type_now == NoChar && (parameters[index].first != NoChar && parameters[index].first != Line) || (type_now != Line && type_now != NoChar && (parameters[index].first == Line || parameters[index].first == NoChar) || parameters[index].second.second != check_array_now)) {
        throw WrongType_parametries(number_str_fuction);//wrong parametrs type
    }
    index++;
    while (c_type == Semicolon) {
        int number_str_fuction = c_number_str;
        get_lex();
        std::pair<type_of_object, bool> now = priority_level1(true);
        type_now = now.first;
        bool check_array_now = now.second;
        if (parameters.size() < index + 1) {
            throw WrongCount_parametries(number_str_fuction);
        }
        if (type_now == Line && (parameters[index].first != Line) || type_now == NoChar && (parameters[index].first != NoChar && parameters[index].first != Line) || (type_now != Line && type_now != NoChar && (parameters[index].first == Line || parameters[index].first == NoChar) || parameters[index].second.second != check_array_now)) {
            throw WrongType_parametries(number_str_fuction);//wrong parametrs type
        }
        index++;
    }
    if (index < parameters.size()) {
        throw WrongCount_parametries(number_str_fuction);//wrong parametrs count
    }
}

void Parser::multiple_definition() {
    type_of_object type_1;
    type_of_object type_now;
    if (c_type == SpecialWords) {
        if (c_str == "integer" || c_str == "bool" || c_str == "line" || c_str == "valid" || c_str == "nochar") {
            if (c_str == "integer") {
                type_now = Integer;
            }
            if (c_str == "bool") {
                type_now = Bool;
            }
            if (c_str == "line") {
                type_now = Line;
            }
            if (c_str == "valid") {
                type_now = Valid;
            }
            if (c_str == "nochar") {
                type_now = NoChar;
            }
            get_lex();
            if (c_type == ID) {
                std::string name = c_str;
                if (Check_ID(c_str) == type_of_object::Nothing) {
                    tid_list.back().TID.push_back(table(c_str, type_now));
                    tid_list_not_delete.back().TID.push_back(table(c_str, type_now));
                }
                else {
                    throw Wrong_field_of_view(c_number_str, c_str);
                }
                type_1 = Check_ID(c_str);
                if (type_1 == Nothing) {
                    throw WrongInitialization(c_number_str, c_str);
                }
                Lex lexic(IDENTIF, name, true);
                std::string name_lex_now = c_str;
                get_lex();
                if (c_type == SquareBrackets && c_str == "[") {
                    if (tid_list_f.size() > 0) {
                        if (!CheckIsParametersBeen(tid_list_f.back().TID[tid_list_f.back().TID.size() - 1].name, name_lex_now)) {
                            throw Wrong_field_of_view(c_number_str, name_lex_now);
                        }
                    }
                    tid_list.back().TID[tid_list.back().TID.size() - 1].array = true;
                    tid_list_not_delete.back().TID[tid_list_not_delete.back().TID.size() - 1].array = true;
                    get_lex();
                    if (c_type != Number) {
                        throw WrongArraySize(c_number_str, c_str);
                    }
                    //std::pair<type_of_object, bool> now = priority_level1();
                    //type_of_object type_2 = now.first;
                    type_of_object type_2 = Integer;
                    if (type_2 != Integer) {
                        throw Wrong_Array_cell(c_number_str, type_2);
                    }
                    get_lex();
                    if (c_type == SquareBrackets && c_str == "]") {
                        get_lex();
                    }
                    else {
                        throw WrongSquareBracket_close(c_number_str, c_str);
                    }
                }
                if (c_type == Equal) {
                    if (Check_Array(name)) {
                        throw WrongArray(c_number_str, name);
                    }
                    POLIZ.put_lex(lexic);
                    type_of_object type_2;
                    get_lex();
                    std::pair<type_of_object, bool> now = priority_level1(true);
                    type_2 = now.first;
                    if (type_1 == Line && (type_2 != Line) || type_1 == NoChar && (type_2 != Line && type_2 != NoChar) || (type_1 != Line && type_1 != NoChar && (type_2 == Line || type_2 == NoChar) || now.second != tid_list.back().TID[tid_list.back().TID.size() - 1].array)) {
                        throw Wrong_type(c_number_str, type_1, type_2);
                    }
                    Lex lexic2(PolizElement::EQUAL);
                    POLIZ.put_lex(lexic2);
                }
                while (c_type == Semicolon) {
                    get_lex();
                    if (c_type == ID) {
                        std::string name = c_str;
                        if (Check_ID(c_str) == type_of_object::Nothing) {
                            tid_list.back().TID.push_back(table(c_str, type_now));
                            tid_list_not_delete.back().TID.push_back(table(c_str, type_now));
                        }
                        else {
                            throw Wrong_field_of_view(c_number_str, c_str);
                        }
                        type_1 = Check_ID(c_str);
                        if (type_1 == Nothing) {
                            throw WrongInitialization(c_number_str, c_str);
                        }
                        Lex lexic5(IDENTIF, name, true);
                        std::string name_lex_now = c_str;
                        get_lex();
                        if (c_type == SquareBrackets && c_str == "[") {
                            if (tid_list_f.size() > 0) {
                                if (!CheckIsParametersBeen(tid_list_f.back().TID[tid_list_f.back().TID.size() - 1].name, name_lex_now)) {
                                    throw Wrong_field_of_view(c_number_str, name_lex_now);
                                }
                            }
                            tid_list.back().TID[tid_list.back().TID.size() - 1].array = true;
                            tid_list_not_delete.back().TID[tid_list_not_delete.back().TID.size() - 1].array = true;
                            get_lex();
                            if (c_type != Number) {
                                throw WrongArraySize(c_number_str, c_str);
                            }
                            //std::pair<type_of_object, bool> now = priority_level1();
                            //type_of_object type_2 = now.first;
                            type_of_object type_2 = Integer;
                            if (type_2 != Integer) {
                                throw Wrong_Array_cell(c_number_str, type_2);
                            }
                            get_lex();
                            if (c_type == SquareBrackets && c_str == "]") {
                                get_lex();
                            }
                            else {
                                throw WrongSquareBracket_close(c_number_str, c_str);
                            }
                        }
                        if (c_type == Equal) {
                            if (Check_Array(name)) {
                                throw WrongArray(c_number_str, name);
                            }
                            POLIZ.put_lex(lexic5);
                            type_of_object type_2;
                            get_lex();
                            std::pair<type_of_object, bool> now = priority_level1(true);
                            type_2 = now.first;
                            if (type_1 == Line && (type_2 != Line) || type_1 == NoChar && (type_2 != Line && type_2 != NoChar) || (type_1 != Line && type_1 != NoChar && (type_2 == Line || type_2 == NoChar) || now.second != tid_list.back().TID[tid_list.back().TID.size() - 1].array)) {
                                throw Wrong_type(c_number_str, type_1, type_2);
                            }
                            Lex lexic(PolizElement::EQUAL);
                            POLIZ.put_lex(lexic);
                        }
                    }
                    else {
                        throw Wrong_id(c_number_str, c_str);
                    }
                }
                if (c_type == ExclamationMark) {
                    get_lex();
                }
                else {
                    throw WrongExMark(c_number_str, c_str);
                }
            }
            else {
                throw Wrong_id(c_number_str, c_str);
            }
        }
        else {
            throw WrongSpecial_word_type(c_number_str, c_str);
        }
    }
    else {
        throw WrongSpecial_word(c_number_str, c_str);
    }
}

void Parser::bool_(bool print) {
    if (c_type == SpecialWords && (c_str == "true" || c_str == "false")) {
        if (c_str == "true" && print) {
            Lex lexic(TRUE, true);
            POLIZ.put_lex(lexic);
        }
        if (c_str == "false" && print) {
            Lex lexic(FALSE, false);
            POLIZ.put_lex(lexic);
        }
        get_lex();
    }
    else {
        throw WrongSpecial_word_type_bool(c_number_str, c_str);
    }
}

void Parser::assignment() {
    type_of_object type_1;
    if (c_type == ID) {
        type_1 = Check_ID(c_str);
        if (type_1 == Nothing) {
            throw WrongInitialization(c_number_str, c_str);
        }
        Lex lexic(IDENTIF, c_str, Check_Array(c_str));
        POLIZ.put_lex(lexic);
        get_lex();
        if (c_type == SquareBrackets && c_str == "[") {
            get_lex();
            std::pair<type_of_object, bool> now = priority_level1(true);
            type_of_object type_2 = now.first;
            if (type_2 != Integer) {
                throw Wrong_Array_cell(c_number_str, type_2);
            }
            if (c_type == SquareBrackets && c_str == "]") {
                get_lex();
            }
            else {
                throw WrongSquareBracket_close(c_number_str, c_str);
            }
            Lex lexic1(INDEX);
            POLIZ.put_lex(lexic1);
        }
        if (c_type == Equal) {
            type_of_object type_2;
            get_lex();
            std::pair<type_of_object, bool> now = priority_level1(true);
            type_2 = now.first;
            if (type_1 == Line && (type_2 != Line) || type_1 == NoChar && (type_2 != Line && type_2 != NoChar) || (type_1 != Line && type_1 != NoChar && (type_2 == Line || type_2 == NoChar))) {
                throw Wrong_type(c_number_str, type_1, type_2);
            }
            Lex lexic2(PolizElement::EQUAL);
            POLIZ.put_lex(lexic2);
            if (c_type == ExclamationMark) {
                get_lex();
            }
            else {
                throw WrongExMark(c_number_str, c_str);
            }
        }
        else {
            throw WrongEqual(c_number_str, c_str);
        }
    }
    else {
        throw Wrong_id(c_number_str, c_str);
    }
}

void Parser::operator_() {
    if (c_type == SpecialWords && c_str == "return") {
        Return_fuction.push_back(tid_list_f.back().TID.back().name);
        get_lex();
        if (c_type == ExclamationMark) {
            if (tid_list_f.back().TID.back().type != Void) {
                throw WrongNo_return(tid_list_f.back().TID.back().name);
            }
            Lex lexic(LABEL_F), lexic2(GOTO);
            POLIZ.put_lex(lexic);
            POLIZ.put_lex(lexic2);
            get_lex();
            if (c_str == "}" || c_str == ")") {
                return;
            }
            else {
                throw WrongCurlyBracket_close(c_number_str, c_str);
            }
        }
        else {
            if (tid_list_f.back().TID.back().type == Void) {
                throw WrongVoid(tid_list_f.back().TID.back().name);
            }
            std::pair<type_of_object, bool> now = priority_level1(true);
            Lex lexic(LABEL_F), lexic2(GOTO);
            POLIZ.put_lex(lexic);
            POLIZ.put_lex(lexic2);
            type_of_object type_1 = now.first;
            if (now.second) {
                throw WrongReturn_array(tid_list_f.back().TID.back().name);
            }
            type_of_object type_2 = tid_list_f.back().TID.back().type;
            if (type_1 == Line && (type_2 != Line) || type_1 == NoChar && (type_2 != Line && type_2 != NoChar) || (type_1 != Line && type_1 != NoChar && (type_2 == Line || type_2 == NoChar))) {
                throw Wrong_type(c_number_str, type_1, type_2);
            }
            if (c_type == ExclamationMark) {
                get_lex();
                if (c_str == "}" || c_str == ")") {
                    return;
                }
                else {
                    operator_();
                }
            }
            else {
                throw WrongExMark(c_number_str, c_str);
            }
        }
    }
    else {
        if (c_type == SpecialWords && c_str == "if") {
            get_lex();
            if (c_type == RoundBracket && c_str == "(") {
                get_lex();
                priority_level1(true);
                int label1 = POLIZ.get_free();
                POLIZ.blank();
                Lex lex_go_to_false(GOTOFALSE);
                POLIZ.put_lex(lex_go_to_false);
                if (c_type == RoundBracket && c_str == ")") {
                    get_lex();
                    if (c_type == CurlyBracket && c_str == "{") {
                        tid_list.push_back(List_Struct());
                        tid_list_not_delete.push_back(List_Struct(IndexNow));
                        IndexNow++;
                        get_lex();
                        operator_();
                        int label2 = POLIZ.get_free();
                        POLIZ.blank();
                        Lex lex_go_to(GOTO);
                        POLIZ.put_lex(lex_go_to);
                        if (c_type == CurlyBracket && c_str == "}") {
                            tid_list.pop_back();
                            IndexNow--;
                            get_lex();
                            if (c_type == SpecialWords && c_str == "else") {
                                get_lex();
                                Lex LABEL1(LABEL, (double)POLIZ.get_free());
                                POLIZ[label1] = LABEL1;
                                if (c_type == CurlyBracket && c_str == "{") {
                                    tid_list.push_back(List_Struct());
                                    tid_list_not_delete.push_back(List_Struct(IndexNow));
                                    IndexNow++;
                                    get_lex();
                                    operator_();
                                    if (c_type == CurlyBracket && c_str == "}") {
                                        Lex LABEL2(LABEL, (double)POLIZ.get_free());
                                        POLIZ[label2] = LABEL2;
                                        tid_list.pop_back();
                                        IndexNow--;
                                        get_lex();
                                        if (c_type == ExclamationMark) {
                                            get_lex();
                                            if (c_str == "}" || c_str == ")") {
                                                return;
                                            }
                                            else {
                                                operator_();
                                            }
                                        }
                                        else {
                                            throw WrongExMark(c_number_str, c_str);
                                        }
                                    }
                                    else {
                                        throw WrongCurlyBracket_close(c_number_str, c_str);
                                    }
                                }
                                else {
                                    throw WrongCurlyBracket_open(c_number_str, c_str);
                                }
                            }
                            else {
                                POLIZ.antiblank();
                                POLIZ.antiblank();
                                Lex LABEL1(LABEL, (double)POLIZ.get_free());
                                POLIZ[label1] = LABEL1;
                                if (c_type == ExclamationMark) {
                                    get_lex();
                                    if (c_str == "}" || c_str == ")") {
                                        return;
                                    }
                                    else {
                                        operator_();
                                    }
                                }
                                else {
                                    throw WrongExMark(c_number_str, c_str);
                                }
                            }
                        }
                        else {
                            throw WrongCurlyBracket_close(c_number_str, c_str);
                        }
                    }
                    else {
                        throw WrongCurlyBracket_open(c_number_str, c_str);
                    }
                }
                else {
                    throw WrongRoundBracket_close(c_number_str, c_str);
                }
            }
            else {
                throw WrongRoundBracket_open(c_number_str, c_str);
            }
        }
        else {
            if (c_type == SpecialWords && c_str == "while") {
                get_lex();
                if (c_type == RoundBracket && c_str == "(") {
                    Lex LABEL_FOR_IF(LABEL, (double)POLIZ.get_free());
                    get_lex();
                    priority_level1(true);
                    int label1 = POLIZ.get_free();
                    POLIZ.blank();
                    Lex lex_go_to_false(GOTOFALSE);
                    POLIZ.put_lex(lex_go_to_false);
                    if (c_type == RoundBracket && c_str == ")") {
                        get_lex();
                        if (c_type == CurlyBracket && c_str == "{") {
                            tid_list.push_back(List_Struct());
                            tid_list_not_delete.push_back(List_Struct(IndexNow));
                            IndexNow++;
                            get_lex();
                            internal_operator();
                            int label2 = POLIZ.get_free();
                            POLIZ.blank();
                            Lex lex_go_to(GOTO);
                            POLIZ.put_lex(lex_go_to);
                            Lex LABEL2(LABEL, (double)POLIZ.get_free());
                            POLIZ[label2] = LABEL_FOR_IF;
                            if (c_type == CurlyBracket && c_str == "}") {
                                if (check_break) {
                                    check_break = 0;
                                    POLIZ[label_for_break] = LABEL2;
                                }
                                if (check_continue) {
                                    check_break = 0;
                                    POLIZ[label_for_continue] = LABEL_FOR_IF;
                                }
                                tid_list.pop_back();
                                IndexNow--;
                                get_lex();
                                POLIZ[label1] = LABEL2;
                                if (c_type == ExclamationMark) {
                                    get_lex();
                                    if (c_str == "}" || c_str == ")") {
                                        return;
                                    }
                                    else {
                                        operator_();
                                    }
                                }
                                else {
                                    throw WrongExMark(c_number_str, c_str);
                                }
                            }
                            else {
                                throw WrongCurlyBracket_open(c_number_str, c_str);
                            }
                        }
                        else {
                            throw WrongCurlyBracket_close(c_number_str, c_str);
                        }
                    }
                    else {
                        throw WrongRoundBracket_close(c_number_str, c_str);
                    }
                }
                else {
                    throw WrongRoundBracket_open(c_number_str, c_str);
                }
            }
            else {
                if (c_type == SpecialWords && c_str == "toggle") {
                    get_lex();
                    if (c_type == RoundBracket && c_str == "(") {
                        get_lex();
                        if (c_type == ID) {
                            Lex MAIN_lexic(IDENTIF, c_str, Check_Array(c_str));
                            get_lex();
                            std::string name = c_str;
                            if (c_type == SquareBrackets && c_str == "[") {
                                if (!Check_Array(name) && !(Check_ID(name) == Line)) {
                                    throw WrongArray_name(c_number_str, name);
                                }
                                get_lex();
                                std::pair<type_of_object, bool> now = priority_level1(true);
                                type_of_object type_2 = now.first;
                                if (type_2 != Integer) {
                                    throw Wrong_Array_cell(c_number_str, type_2);
                                }
                                if (c_type == SquareBrackets && c_str == "]") {
                                    get_lex();
                                }
                                else {
                                    throw WrongSquareBracket_close(c_number_str, c_str);
                                }
                            }
                            if (c_type == RoundBracket && c_str == ")") {
                                get_lex();
                                if (c_type == CurlyBracket && c_str == "{") {
                                    get_lex();
                                    selection(MAIN_lexic);
                                    if (c_type == CurlyBracket && c_str == "}") {
                                        Lex LABEL1(LABEL, (double)POLIZ.get_free());
                                        for (int l = 0; l < label_for_choice.size(); ++l) {
                                            if (l + 1 < label_for_choice.size()) {
                                                Lex LABELNOW(LABEL, (double)label_for_choice[l + 1] - 2);
                                                POLIZ[label_for_choice[l]] = LABELNOW;
                                            }
                                            else {
                                                POLIZ[label_for_choice[l]] = LABEL1;
                                            }
                                        }
                                        label_for_choice.clear();
                                        label_for_choice.resize(0);
                                        get_lex();
                                        if (c_type == ExclamationMark) {
                                            get_lex();
                                            if (c_str == "}" || c_str == ")") {
                                                return;
                                            }
                                            else {
                                                operator_();
                                            }
                                        }
                                        else {
                                            throw WrongExMark(c_number_str, c_str);
                                        }
                                    }
                                    else {
                                        throw WrongCurlyBracket_close(c_number_str, c_str);
                                    }
                                }
                                else {
                                    throw WrongCurlyBracket_open(c_number_str, c_str);
                                }
                            }
                            else {
                                throw WrongRoundBracket_close(c_number_str, c_str);
                            }
                        }
                        else {
                            throw Wrong_id(c_number_str, c_str);
                        }
                    }
                    else {
                        throw WrongRoundBracket_close(c_number_str, c_str);
                    }
                }
                else {
                    if (c_type == SpecialWords && c_str == "for") {
                        get_lex();
                        if (c_type == RoundBracket && c_str == "(") {
                            tid_list.push_back(List_Struct());
                            tid_list_not_delete.push_back(List_Struct(IndexNow));
                            IndexNow++;
                            get_lex();
                            //ПЕРВЫЙ ПАРАМЕТР ЦИКЛА
                            if (c_type == SpecialWords && (c_str == "integer" || c_str == "bool" || c_str == "line" || c_str == "valid" || c_str == "nochar")) {
                                multiple_definition();
                            }
                            else {
                                if (c_type == ExclamationMark) {
                                    get_lex();
                                }
                                else {
                                    assignment();
                                }
                            }

                            Lex LABEL_IF(LABEL, (double)POLIZ.get_free());
                            priority_level1(true);

                            int label1 = POLIZ.get_free();
                            POLIZ.blank();
                            Lex lex_go_to_false(GOTOFALSE);
                            POLIZ.put_lex(lex_go_to_false);

                            int label2 = POLIZ.get_free();
                            POLIZ.blank();
                            Lex lex_go_to(GOTO);
                            POLIZ.put_lex(lex_go_to);

                            if (c_type == ExclamationMark) {
                                get_lex();
                                Lex LABEL_ASSIGM(LABEL, (double)POLIZ.get_free());
                                if (c_str != ")") {
                                    assignment();
                                }

                                int label3 = POLIZ.get_free();
                                POLIZ.blank();
                                Lex lex_go_to(GOTO);
                                POLIZ.put_lex(lex_go_to);
                                POLIZ[label3] = LABEL_IF;

                                if (c_type == RoundBracket && c_str == ")") {
                                    get_lex();
                                    if (c_type == CurlyBracket && c_str == "{") {
                                        get_lex();
                                        Lex LABEL_BODY(LABEL, (double)POLIZ.get_free());
                                        POLIZ[label2] = LABEL_BODY;
                                        internal_operator();

                                        int label4 = POLIZ.get_free();
                                        POLIZ.blank();
                                        Lex lex_go_to(GOTO);
                                        POLIZ.put_lex(lex_go_to);
                                        POLIZ[label4] = LABEL_ASSIGM;

                                        if (c_type == CurlyBracket && c_str == "}") {
                                            Lex LABEL_END(LABEL, (double)POLIZ.get_free());
                                            POLIZ[label1] = LABEL_END;
                                            tid_list.pop_back();
                                            IndexNow--;
                                            if (check_break) {
                                                check_break = 0;
                                                POLIZ[label_for_break] = LABEL_END;
                                            }
                                            if (check_continue) {
                                                check_break = 0;
                                                POLIZ[label_for_continue] = LABEL_IF;
                                            }
                                            get_lex();
                                            if (c_type == ExclamationMark) {
                                                get_lex();
                                                if (c_str == "}" || c_str == ")") {
                                                    return;
                                                }
                                                else {
                                                    operator_();
                                                }
                                            }
                                            else {
                                                throw WrongExMark(c_number_str, c_str);
                                            }
                                        }
                                        else {
                                            throw WrongCurlyBracket_close(c_number_str, c_str);
                                        }
                                    }
                                    else {
                                        throw WrongCurlyBracket_open(c_number_str, c_str);
                                    }
                                }
                                else {
                                    throw WrongRoundBracket_close(c_number_str, c_str);
                                }
                            }
                            else {
                                throw WrongExMark(c_number_str, c_str);
                            }
                        }
                        else {
                            throw WrongRoundBracket_open(c_number_str, c_str);
                        }
                    }
                    else {
                        if (c_type == ID) {
                            Lex lexic(IDENTIF, c_str, Check_Array(c_str));
                            std::string name = c_str;
                            type_of_object type_1;
                            type_1 = Check_ID(c_str);
                            bool check_array = Check_Array(c_str);
                            if (type_1 == Nothing) {
                                type_1 = Check_ID_F(c_str);
                                if (type_1 == Nothing) {
                                    throw WrongInitialization(c_number_str, c_str);
                                }
                                else {
                                    get_lex();
                                    if (c_type == RoundBracket && c_str == "(") {
                                        get_lex();
                                        enumeration_arithmetic(GetParameters(name));
                                        CORRECT_LABLE_FOR_FUNCTION(name, POLIZ.get_free() + 3);
                                        Lex lexic2(LABEL, (double)GET_POLIZ_INDEX_OF_FUNCTION(name)), lexic(LABEL, (double)POLIZ.get_free() + 3), lexic3(GOTOCALLF);
                                        POLIZ.put_lex(lexic);
                                        POLIZ.put_lex(lexic2);
                                        POLIZ.put_lex(lexic3);
                                        if (c_type == RoundBracket && c_str == ")") {
                                            get_lex();
                                            if (c_type == ExclamationMark) {
                                                get_lex();
                                                if (c_str == "}" || c_str == ")") {
                                                    return;
                                                }
                                                else {
                                                    operator_();
                                                }
                                            }
                                            else {
                                                throw WrongExMark(c_number_str, c_str);
                                            }
                                        }
                                        else {
                                            throw WrongRoundBracket_close(c_number_str, c_str);
                                        }
                                    }
                                    else {
                                        throw WrongRoundBracket_open(c_number_str, c_str);
                                    }
                                }
                            }
                            else {
                                get_lex();
                                if (c_type == Point) {
                                    if (!Check_Array(name)) {
                                        throw WrongArray_name(c_number_str, name);
                                    }
                                    get_lex();
                                    if (c_type == SpecialWords && c_str == "pop") {
                                        POLIZ.put_lex(lexic);
                                        Lex lex1(POP);
                                        POLIZ.put_lex(lex1);
                                        get_lex();
                                        if (c_type == ExclamationMark) {
                                            get_lex();
                                            if (c_str == "}" || c_str == ")") {
                                                return;
                                            }
                                            else {
                                                operator_();
                                            }
                                        }
                                        else {
                                            throw WrongExMark(c_number_str, c_str);
                                        }
                                    }
                                    else {
                                        if (c_type == SpecialWords && c_str == "add") {
                                            POLIZ.put_lex(lexic);
                                            get_lex();
                                            if (c_type == RoundBracket && c_str == "(") {
                                                get_lex();
                                                std::pair<type_of_object, bool> now = priority_level1(true);
                                                type_of_object type_2 = now.first;
                                                if (type_1 == Line && (type_2 != Line) || type_1 == NoChar && (type_2 != Line && type_2 != NoChar) || (type_1 != Line && type_1 != NoChar && (type_2 == Line || type_2 == NoChar))) {
                                                    throw Wrong_type(c_number_str, type_1, type_2);
                                                }
                                                Lex lex1(ADD);
                                                POLIZ.put_lex(lex1);
                                                if (c_type == RoundBracket && c_str == ")") {
                                                    get_lex();
                                                    if (c_type == ExclamationMark) {
                                                        get_lex();
                                                        if (c_str == "}" || c_str == ")") {
                                                            return;
                                                        }
                                                        else {
                                                            operator_();
                                                        }
                                                    }
                                                    else {
                                                        throw WrongExMark(c_number_str, c_str);
                                                    }
                                                }
                                                else {
                                                    throw WrongRoundBracket_close(c_number_str, c_str);
                                                }
                                            }
                                            else {
                                                throw WrongRoundBracket_open(c_number_str, c_str);
                                            }
                                        }
                                        else {
                                            throw Wrong_pop_add(c_number_str, c_str);
                                        }
                                    }
                                }
                                else {
                                    bool iacheika = 0;
                                    POLIZ.put_lex(lexic);
                                    if (c_type == SquareBrackets && c_str == "[") {
                                        iacheika = 1;
                                        if (!Check_Array(name) && !(Check_ID(name) == Line)) {
                                            throw WrongArray_name(c_number_str, name);
                                        }
                                        if (type_1 == Line) {
                                            type_1 = NoChar;
                                        }
                                        get_lex();
                                        std::pair<type_of_object, bool> now = priority_level1(true);
                                        type_of_object type_2 = now.first;
                                        if (type_2 != Integer) {
                                            throw Wrong_Array_cell(c_number_str, type_2);
                                        }
                                        if (c_type == SquareBrackets && c_str == "]") {
                                            get_lex();
                                        }
                                        else {
                                            throw WrongSquareBracket_close(c_number_str, c_str);
                                        }
                                        Lex lexic1(INDEX);
                                        POLIZ.put_lex(lexic1);
                                    }
                                    if (c_type == Equal) {
                                        type_of_object type_2;
                                        get_lex();
                                        std::pair<type_of_object, bool> now = priority_level1(true);
                                        type_2 = now.first;
                                        if (check_array != now.second && !iacheika) {
                                            throw WrongArray(c_number_str, name);
                                        }
                                        if (type_1 == Line && (type_2 != Line) || type_1 == NoChar && (type_2 != Line && type_2 != NoChar) || (type_1 != Line && type_1 != NoChar && (type_2 == Line || type_2 == NoChar))) {
                                            throw Wrong_type(c_number_str, type_1, type_2);
                                        }
                                        Lex lexic1(EQUAL);
                                        POLIZ.put_lex(lexic1);
                                        if (c_type == ExclamationMark) {
                                            get_lex();
                                            if (c_str == "}" || c_str == ")") {
                                                return;
                                            }
                                            else {
                                                operator_();
                                            }
                                        }
                                        else {
                                            throw WrongExMark(c_number_str, c_str);
                                        }
                                    }
                                    else {
                                        throw WrongVariable(c_number_str, c_str);
                                    }
                                }
                            }
                        }
                        else {
                            if (c_type == SpecialWords && c_str == "input") {
                                get_lex();
                                if (c_type == RoundBracket && c_str == "(") {
                                    get_lex();
                                    if (c_type == ID) {
                                        std::string name = c_str;
                                        if ((Check_ID(c_str) == type_of_object::Nothing)) {
                                            throw WrongInitialization(c_number_str, c_str);
                                        }
                                        Lex lexic(IDENTIF, c_str, Check_Array(c_str));
                                        POLIZ.put_lex(lexic);
                                        Lex lexic2(INPUT);
                                        POLIZ.put_lex(lexic2);

                                        get_lex();
                                        if (c_type == SquareBrackets && c_str == "[") {
                                            if (!Check_Array(name) && !(Check_ID(name) == Line)) {
                                                throw WrongArray_name(c_number_str, name);
                                            }
                                            get_lex();
                                            std::pair<type_of_object, bool> now = priority_level1();
                                            type_of_object type_2 = now.first;
                                            if (type_2 != Integer) {
                                                throw Wrong_Array_cell(c_number_str, type_2);
                                            }
                                            if (c_type == SquareBrackets && c_str == "]") {
                                                get_lex();
                                            }
                                            else {
                                                throw WrongSquareBracket_close(c_number_str, c_str);
                                            }
                                        }
                                        else {
                                            if (Check_Array(name)) {
                                                throw WrongArray(c_number_str, name);
                                            }
                                        }
                                        if (c_type == RoundBracket && c_str == ")") {
                                            get_lex();
                                            if (c_type == ExclamationMark) {
                                                get_lex();
                                                if (c_str == "}" || c_str == ")") {
                                                    return;
                                                }
                                                else {
                                                    operator_();
                                                }
                                            }
                                            else {
                                                throw WrongExMark(c_number_str, c_str);
                                            }
                                        }
                                        else {
                                            throw WrongRoundBracket_close(c_number_str, c_str);
                                        }
                                    }
                                    else {
                                        throw Wrong_id(c_number_str, c_str);
                                    }
                                }
                                else {
                                    throw WrongRoundBracket_open(c_number_str, c_str);
                                }
                            }
                            else {
                                if (c_type == SpecialWords && c_str == "output") {
                                    get_lex();
                                    if (c_type == RoundBracket && c_str == "(") {
                                        get_lex();
                                        priority_level1(true);
                                        Lex lexic2(OUTPUT);
                                        POLIZ.put_lex(lexic2);
                                        if (c_type == RoundBracket && c_str == ")") {
                                            get_lex();
                                            if (c_type == ExclamationMark) {
                                                get_lex();
                                                if (c_str == "}" || c_str == ")") {
                                                    return;
                                                }
                                                else {
                                                    operator_();
                                                }
                                            }
                                            else {
                                                throw WrongExMark(c_number_str, c_str);
                                            }
                                        }
                                        else {
                                            throw WrongRoundBracket_close(c_number_str, c_str);
                                        }
                                    }
                                    else {
                                        throw WrongRoundBracket_open(c_number_str, c_str);
                                    }
                                }
                                else {
                                    if (c_str == "}") {
                                        return;
                                    }
                                    multiple_definition();
                                    if (c_str == "}" || c_str == ")") {
                                        return;
                                    }
                                    else {
                                        operator_();
                                    }
                                }
                            }
                        }
                    }
                }
            }
        }
    }
}

void Parser::internal_operator() {
    if (c_type == SpecialWords && c_str == "return") {
        Return_fuction.push_back(tid_list_f.back().TID.back().name);
        get_lex();
        if (c_type == ExclamationMark) {
            if (tid_list_f.back().TID.back().type != Void) {
                throw WrongNo_return(tid_list_f.back().TID.back().name);
            }
            get_lex();
            if (c_str == "}" || c_str == ")") {
                return;
            }
            else {
                throw WrongCurlyBracket_close(c_number_str, c_str);
            }
        }
        else {
            if (tid_list_f.back().TID.back().type == Void) {
                throw WrongVoid(tid_list_f.back().TID.back().name);
            }
            std::pair<type_of_object, bool> now = priority_level1(true);
            type_of_object type_1 = now.first;
            if (now.second) {
                throw WrongReturn_array(tid_list_f.back().TID.back().name);
            }
            type_of_object type_2 = tid_list_f.back().TID.back().type;
            if (type_1 == Line && (type_2 != Line) || type_1 == NoChar && (type_2 != Line && type_2 != NoChar) || (type_1 != Line && type_1 != NoChar && (type_2 == Line || type_2 == NoChar))) {
                throw Wrong_type(c_number_str, type_1, type_2);
            }
            if (c_type == ExclamationMark) {
                get_lex();
                if (c_str == "}" || c_str == ")") {
                    return;
                }
                else {
                    internal_operator();
                }
            }
            else {
                throw WrongExMark(c_number_str, c_str);
            }
        }
    }
    else {
        if (c_type == SpecialWords && c_str == "if") {
            get_lex();
            if (c_type == RoundBracket && c_str == "(") {
                get_lex();
                priority_level1(true);
                int label1 = POLIZ.get_free();
                POLIZ.blank();
                Lex lex_go_to_false(GOTOFALSE);
                POLIZ.put_lex(lex_go_to_false);
                if (c_type == RoundBracket && c_str == ")") {
                    get_lex();
                    if (c_type == CurlyBracket && c_str == "{") {
                        tid_list.push_back(List_Struct());
                        tid_list_not_delete.push_back(List_Struct(IndexNow));
                        IndexNow++;
                        get_lex();
                        internal_operator();
                        int label2 = POLIZ.get_free();
                        POLIZ.blank();
                        Lex lex_go_to(GOTO);
                        POLIZ.put_lex(lex_go_to);
                        if (c_type == CurlyBracket && c_str == "}") {
                            tid_list.pop_back();
                            IndexNow--;
                            get_lex();
                            if (c_type == SpecialWords && c_str == "else") {
                                get_lex();
                                Lex LABEL1(LABEL, (double)POLIZ.get_free());
                                POLIZ[label1] = LABEL1;
                                if (c_type == CurlyBracket && c_str == "{") {
                                    tid_list.push_back(List_Struct());
                                    tid_list_not_delete.push_back(List_Struct(IndexNow));
                                    IndexNow++;
                                    get_lex();
                                    internal_operator();
                                    if (c_type == CurlyBracket && c_str == "}") {
                                        Lex LABEL2(LABEL, (double)POLIZ.get_free());
                                        POLIZ[label2] = LABEL2;
                                        tid_list.pop_back();
                                        IndexNow--;
                                        get_lex();
                                        if (c_type == ExclamationMark) {
                                            get_lex();
                                            if (c_str == "}" || c_str == ")") {
                                                return;
                                            }
                                            else {
                                                internal_operator();
                                            }
                                        }
                                        else {
                                            throw WrongExMark(c_number_str, c_str);
                                        }
                                    }
                                    else {
                                        throw WrongCurlyBracket_close(c_number_str, c_str);
                                    }
                                }
                                else {
                                    throw WrongCurlyBracket_open(c_number_str, c_str);
                                }
                            }
                            else {
                                POLIZ.antiblank();
                                POLIZ.antiblank();
                                Lex LABEL1(LABEL, (double)POLIZ.get_free());
                                POLIZ[label1] = LABEL1;
                                if (c_type == ExclamationMark) {
                                    get_lex();
                                    if (c_str == "}" || c_str == ")") {
                                        return;
                                    }
                                    else {
                                        internal_operator();
                                    }
                                }
                                else {
                                    throw WrongExMark(c_number_str, c_str);
                                }
                            }
                        }
                        else {
                            throw WrongCurlyBracket_close(c_number_str, c_str);
                        }
                    }
                    else {
                        throw WrongCurlyBracket_open(c_number_str, c_str);
                    }
                }
                else {
                    throw WrongRoundBracket_close(c_number_str, c_str);
                }
            }
            else {
                throw WrongRoundBracket_open(c_number_str, c_str);
            }
        }
        else {
            if (c_type == SpecialWords && c_str == "while") {
                get_lex();
                if (c_type == RoundBracket && c_str == "(") {
                    Lex LABEL_FOR_IF(LABEL, (double)POLIZ.get_free());
                    get_lex();
                    priority_level1(true);
                    int label1 = POLIZ.get_free();
                    POLIZ.blank();
                    Lex lex_go_to_false(GOTOFALSE);
                    POLIZ.put_lex(lex_go_to_false);
                    if (c_type == RoundBracket && c_str == ")") {
                        get_lex();
                        if (c_type == CurlyBracket && c_str == "{") {
                            tid_list.push_back(List_Struct());
                            tid_list_not_delete.push_back(List_Struct(IndexNow));
                            IndexNow++;
                            get_lex();
                            internal_operator();
                            int label2 = POLIZ.get_free();
                            POLIZ.blank();
                            Lex lex_go_to(GOTO);
                            POLIZ.put_lex(lex_go_to);
                            Lex LABEL2(LABEL, (double)POLIZ.get_free());
                            POLIZ[label2] = LABEL_FOR_IF;
                            if (c_type == CurlyBracket && c_str == "}") {
                                if (check_break) {
                                    check_break = 0;
                                    POLIZ[label_for_break] = LABEL2;
                                }
                                if (check_continue) {
                                    check_break = 0;
                                    POLIZ[label_for_continue] = LABEL_FOR_IF;
                                }
                                tid_list.pop_back();
                                IndexNow--;
                                get_lex();
                                POLIZ[label1] = LABEL2;
                                if (c_type == ExclamationMark) {
                                    get_lex();
                                    if (c_str == "}" || c_str == ")") {
                                        return;
                                    }
                                    else {
                                        internal_operator();
                                    }
                                }
                                else {
                                    throw WrongExMark(c_number_str, c_str);
                                }
                            }
                            else {
                                throw WrongCurlyBracket_open(c_number_str, c_str);
                            }
                        }
                        else {
                            throw WrongCurlyBracket_close(c_number_str, c_str);
                        }
                    }
                    else {
                        throw WrongRoundBracket_close(c_number_str, c_str);
                    }
                }
                else {
                    throw WrongRoundBracket_open(c_number_str, c_str);
                }
            }
            else {
                if (c_type == SpecialWords && c_str == "toggle") {
                    get_lex();
                    if (c_type == RoundBracket && c_str == "(") {
                        get_lex();
                        if (c_type == ID) {
                            Lex MAIN_lexic(IDENTIF, c_str, Check_Array(c_str));
                            get_lex();
                            std::string name = c_str;
                            if (c_type == SquareBrackets && c_str == "[") {
                                if (!Check_Array(name) && !(Check_ID(name) == Line)) {
                                    throw WrongArray_name(c_number_str, name);
                                }
                                get_lex();
                                std::pair<type_of_object, bool> now = priority_level1(true);
                                type_of_object type_2 = now.first;
                                if (type_2 != Integer) {
                                    throw Wrong_Array_cell(c_number_str, type_2);
                                }
                                if (c_type == SquareBrackets && c_str == "]") {
                                    get_lex();
                                }
                                else {
                                    throw WrongSquareBracket_close(c_number_str, c_str);
                                }
                            }
                            if (c_type == RoundBracket && c_str == ")") {
                                get_lex();
                                if (c_type == CurlyBracket && c_str == "{") {
                                    get_lex();
                                    selection(MAIN_lexic);
                                    if (c_type == CurlyBracket && c_str == "}") {
                                        Lex LABEL1(LABEL, (double)POLIZ.get_free());
                                        for (int l = 0; l < label_for_choice.size(); ++l) {
                                            if (l + 1 < label_for_choice.size()) {
                                                Lex LABELNOW(LABEL, (double)label_for_choice[l + 1] - 2);
                                                POLIZ[label_for_choice[l]] = LABELNOW;
                                            }
                                            else {
                                                POLIZ[label_for_choice[l]] = LABEL1;
                                            }
                                        }
                                        label_for_choice.clear();
                                        label_for_choice.resize(0);
                                        get_lex();
                                        if (c_type == ExclamationMark) {
                                            get_lex();
                                            if (c_str == "}" || c_str == ")") {
                                                return;
                                            }
                                            else {
                                                internal_operator();
                                            }
                                        }
                                        else {
                                            throw WrongExMark(c_number_str, c_str);
                                        }
                                    }
                                    else {
                                        throw WrongCurlyBracket_close(c_number_str, c_str);
                                    }
                                }
                                else {
                                    throw WrongCurlyBracket_open(c_number_str, c_str);
                                }
                            }
                            else {
                                throw WrongRoundBracket_close(c_number_str, c_str);
                            }
                        }
                        else {
                            throw Wrong_id(c_number_str, c_str);
                        }
                    }
                    else {
                        throw WrongRoundBracket_close(c_number_str, c_str);
                    }
                }
                else {
                    if (c_type == SpecialWords && c_str == "for") {
                        get_lex();
                        if (c_type == RoundBracket && c_str == "(") {
                            tid_list.push_back(List_Struct());
                            tid_list_not_delete.push_back(List_Struct(IndexNow));
                            IndexNow++;
                            get_lex();
                            //ПЕРВЫЙ ПАРАМЕТР ЦИКЛА
                            if (c_type == SpecialWords && (c_str == "integer" || c_str == "bool" || c_str == "line" || c_str == "valid" || c_str == "nochar")) {
                                multiple_definition();
                            }
                            else {
                                if (c_type == ExclamationMark) {
                                    get_lex();
                                }
                                else {
                                    assignment();
                                }
                            }

                            Lex LABEL_IF(LABEL, (double)POLIZ.get_free());
                            priority_level1(true);

                            int label1 = POLIZ.get_free();
                            POLIZ.blank();
                            Lex lex_go_to_false(GOTOFALSE);
                            POLIZ.put_lex(lex_go_to_false);

                            int label2 = POLIZ.get_free();
                            POLIZ.blank();
                            Lex lex_go_to(GOTO);
                            POLIZ.put_lex(lex_go_to);

                            if (c_type == ExclamationMark) {
                                get_lex();
                                Lex LABEL_ASSIGM(LABEL, (double)POLIZ.get_free());
                                if (c_str != ")") {
                                    assignment();
                                }

                                int label3 = POLIZ.get_free();
                                POLIZ.blank();
                                Lex lex_go_to(GOTO);
                                POLIZ.put_lex(lex_go_to);
                                POLIZ[label3] = LABEL_IF;

                                if (c_type == RoundBracket && c_str == ")") {
                                    get_lex();
                                    if (c_type == CurlyBracket && c_str == "{") {
                                        get_lex();
                                        Lex LABEL_BODY(LABEL, (double)POLIZ.get_free());
                                        POLIZ[label2] = LABEL_BODY;
                                        internal_operator();

                                        int label4 = POLIZ.get_free();
                                        POLIZ.blank();
                                        Lex lex_go_to(GOTO);
                                        POLIZ.put_lex(lex_go_to);
                                        POLIZ[label4] = LABEL_ASSIGM;

                                        if (c_type == CurlyBracket && c_str == "}") {
                                            Lex LABEL_END(LABEL, (double)POLIZ.get_free());
                                            POLIZ[label1] = LABEL_END;
                                            tid_list.pop_back();
                                            IndexNow--;
                                            if (check_break) {
                                                check_break = 0;
                                                POLIZ[label_for_break] = LABEL_END;
                                            }
                                            if (check_continue) {
                                                check_break = 0;
                                                POLIZ[label_for_continue] = LABEL_IF;
                                            }
                                            get_lex();
                                            if (c_type == ExclamationMark) {
                                                get_lex();
                                                if (c_str == "}" || c_str == ")") {
                                                    return;
                                                }
                                                else {
                                                    internal_operator();
                                                }
                                            }
                                            else {
                                                throw WrongExMark(c_number_str, c_str);
                                            }
                                        }
                                        else {
                                            throw WrongCurlyBracket_close(c_number_str, c_str);
                                        }
                                    }
                                    else {
                                        throw WrongCurlyBracket_open(c_number_str, c_str);
                                    }
                                }
                                else {
                                    throw WrongRoundBracket_close(c_number_str, c_str);
                                }
                            }
                            else {
                                throw WrongExMark(c_number_str, c_str);
                            }
                        }
                        else {
                            throw WrongRoundBracket_open(c_number_str, c_str);
                        }
                    }
                    else {
                        if (c_type == ID) {
                            Lex lexic(IDENTIF, c_str, Check_Array(c_str));
                            std::string name = c_str;
                            type_of_object type_1;
                            type_1 = Check_ID(c_str);
                            bool check_array = Check_Array(c_str);
                            if (type_1 == Nothing) {
                                type_1 = Check_ID_F(c_str);
                                if (type_1 == Nothing) {
                                    throw WrongInitialization(c_number_str, c_str);
                                }
                                else {
                                    get_lex();
                                    if (c_type == RoundBracket && c_str == "(") {
                                        get_lex();
                                        enumeration_arithmetic(GetParameters(name));
                                        CORRECT_LABLE_FOR_FUNCTION(name, POLIZ.get_free() + 3);
                                        Lex lexic2(LABEL, (double)GET_POLIZ_INDEX_OF_FUNCTION(name)), lexic(LABEL, (double)POLIZ.get_free() + 3), lexic3(GOTOCALLF);
                                        POLIZ.put_lex(lexic);
                                        POLIZ.put_lex(lexic2);
                                        POLIZ.put_lex(lexic3);
                                        if (c_type == RoundBracket && c_str == ")") {
                                            get_lex();
                                            if (c_type == ExclamationMark) {
                                                get_lex();
                                                if (c_str == "}" || c_str == ")") {
                                                    return;
                                                }
                                                else {
                                                    internal_operator();
                                                }
                                            }
                                            else {
                                                throw WrongExMark(c_number_str, c_str);
                                            }
                                        }
                                        else {
                                            throw WrongRoundBracket_close(c_number_str, c_str);
                                        }
                                    }
                                    else {
                                        throw WrongRoundBracket_open(c_number_str, c_str);
                                    }
                                }
                            }
                            else {
                                get_lex();
                                if (c_type == Point) {
                                    if (!Check_Array(name)) {
                                        throw WrongArray_name(c_number_str, name);
                                    }
                                    get_lex();
                                    if (c_type == SpecialWords && c_str == "pop") {
                                        POLIZ.put_lex(lexic);
                                        Lex lex1(POP);
                                        POLIZ.put_lex(lex1);
                                        get_lex();
                                        if (c_type == ExclamationMark) {
                                            get_lex();
                                            if (c_str == "}" || c_str == ")") {
                                                return;
                                            }
                                            else {
                                                internal_operator();
                                            }
                                        }
                                        else {
                                            throw WrongExMark(c_number_str, c_str);
                                        }
                                    }
                                    else {
                                        if (c_type == SpecialWords && c_str == "add") {
                                            POLIZ.put_lex(lexic);
                                            get_lex();
                                            if (c_type == RoundBracket && c_str == "(") {
                                                get_lex();
                                                std::pair<type_of_object, bool> now = priority_level1(true);
                                                type_of_object type_2 = now.first;
                                                if (type_1 == Line && (type_2 != Line) || type_1 == NoChar && (type_2 != Line && type_2 != NoChar) || (type_1 != Line && type_1 != NoChar && (type_2 == Line || type_2 == NoChar))) {
                                                    throw Wrong_type(c_number_str, type_1, type_2);
                                                }
                                                Lex lex1(ADD);
                                                POLIZ.put_lex(lex1);
                                                if (c_type == RoundBracket && c_str == ")") {
                                                    get_lex();
                                                    if (c_type == ExclamationMark) {
                                                        get_lex();
                                                        if (c_str == "}" || c_str == ")") {
                                                            return;
                                                        }
                                                        else {
                                                            internal_operator();
                                                        }
                                                    }
                                                    else {
                                                        throw WrongExMark(c_number_str, c_str);
                                                    }
                                                }
                                                else {
                                                    throw WrongRoundBracket_close(c_number_str, c_str);
                                                }
                                            }
                                            else {
                                                throw WrongRoundBracket_open(c_number_str, c_str);
                                            }
                                        }
                                        else {
                                            throw Wrong_pop_add(c_number_str, c_str);
                                        }
                                    }
                                }
                                else {
                                    bool iacheika = 0;
                                    POLIZ.put_lex(lexic);
                                    if (c_type == SquareBrackets && c_str == "[") {
                                        iacheika = 1;
                                        if (!Check_Array(name) && !(Check_ID(name) == Line)) {
                                            throw WrongArray_name(c_number_str, name);
                                        }
                                        if (type_1 == Line) {
                                            type_1 = NoChar;
                                        }
                                        get_lex();
                                        std::pair<type_of_object, bool> now = priority_level1(true);
                                        type_of_object type_2 = now.first;
                                        if (type_2 != Integer) {
                                            throw Wrong_Array_cell(c_number_str, type_2);
                                        }
                                        if (c_type == SquareBrackets && c_str == "]") {
                                            get_lex();
                                        }
                                        else {
                                            throw WrongSquareBracket_close(c_number_str, c_str);
                                        }
                                        Lex lexic1(INDEX);
                                        POLIZ.put_lex(lexic1);
                                    }
                                    if (c_type == Equal) {
                                        type_of_object type_2;
                                        get_lex();
                                        std::pair<type_of_object, bool> now = priority_level1(true);
                                        type_2 = now.first;
                                        if (check_array != now.second && !iacheika) {
                                            throw WrongArray(c_number_str, name);
                                        }
                                        if (type_1 == Line && (type_2 != Line) || type_1 == NoChar && (type_2 != Line && type_2 != NoChar) || (type_1 != Line && type_1 != NoChar && (type_2 == Line || type_2 == NoChar))) {
                                            throw Wrong_type(c_number_str, type_1, type_2);
                                        }
                                        Lex lexic1(EQUAL);
                                        POLIZ.put_lex(lexic1);
                                        if (c_type == ExclamationMark) {
                                            get_lex();
                                            if (c_str == "}" || c_str == ")") {
                                                return;
                                            }
                                            else {
                                                internal_operator();
                                            }
                                        }
                                        else {
                                            throw WrongExMark(c_number_str, c_str);
                                        }
                                    }
                                    else {
                                        throw WrongVariable(c_number_str, c_str);
                                    }
                                }
                            }
                        }
                        else {
                            if (c_type == SpecialWords && c_str == "input") {
                                get_lex();
                                if (c_type == RoundBracket && c_str == "(") {
                                    get_lex();
                                    if (c_type == ID) {
                                        std::string name = c_str;
                                        if ((Check_ID(c_str) == type_of_object::Nothing)) {
                                            throw WrongInitialization(c_number_str, c_str);
                                        }
                                        Lex lexic(IDENTIF, c_str, Check_Array(c_str));
                                        POLIZ.put_lex(lexic);
                                        Lex lexic2(INPUT);
                                        POLIZ.put_lex(lexic2);

                                        get_lex();
                                        if (c_type == SquareBrackets && c_str == "[") {
                                            if (!Check_Array(name) && !(Check_ID(name) == Line)) {
                                                throw WrongArray_name(c_number_str, name);
                                            }
                                            get_lex();
                                            std::pair<type_of_object, bool> now = priority_level1();
                                            type_of_object type_2 = now.first;
                                            if (type_2 != Integer) {
                                                throw Wrong_Array_cell(c_number_str, type_2);
                                            }
                                            if (c_type == SquareBrackets && c_str == "]") {
                                                get_lex();
                                            }
                                            else {
                                                throw WrongSquareBracket_close(c_number_str, c_str);
                                            }
                                        }
                                        else {
                                            if (Check_Array(name)) {
                                                throw WrongArray(c_number_str, name);
                                            }
                                        }
                                        if (c_type == RoundBracket && c_str == ")") {
                                            get_lex();
                                            if (c_type == ExclamationMark) {
                                                get_lex();
                                                if (c_str == "}" || c_str == ")") {
                                                    return;
                                                }
                                                else {
                                                    internal_operator();
                                                }
                                            }
                                            else {
                                                throw WrongExMark(c_number_str, c_str);
                                            }
                                        }
                                        else {
                                            throw WrongRoundBracket_close(c_number_str, c_str);
                                        }
                                    }
                                    else {
                                        throw Wrong_id(c_number_str, c_str);
                                    }
                                }
                                else {
                                    throw WrongRoundBracket_open(c_number_str, c_str);
                                }
                            }
                            else {
                                if (c_type == SpecialWords && c_str == "output") {
                                    get_lex();
                                    if (c_type == RoundBracket && c_str == "(") {
                                        get_lex();
                                        priority_level1(true);
                                        Lex lexic2(OUTPUT);
                                        POLIZ.put_lex(lexic2);
                                        if (c_type == RoundBracket && c_str == ")") {
                                            get_lex();
                                            if (c_type == ExclamationMark) {
                                                get_lex();
                                                if (c_str == "}" || c_str == ")") {
                                                    return;
                                                }
                                                else {
                                                    internal_operator();
                                                }
                                            }
                                            else {
                                                throw WrongExMark(c_number_str, c_str);
                                            }
                                        }
                                        else {
                                            throw WrongRoundBracket_close(c_number_str, c_str);
                                        }
                                    }
                                    else {
                                        throw WrongRoundBracket_open(c_number_str, c_str);
                                    }
                                }
                                else {
                                    if (c_type == SpecialWords && c_str == "break") {
                                        int label2 = POLIZ.get_free();
                                        POLIZ.blank();
                                        label_for_break = label2;
                                        Lex lex_go_to(GOTO);
                                        POLIZ.put_lex(lex_go_to);
                                        check_break = 1;

                                        get_lex();
                                        if (c_type == ExclamationMark) {
                                            get_lex();
                                            if (c_str == "}" || c_str == ")") {
                                                return;
                                            }
                                            else {
                                                internal_operator();
                                            }
                                        }
                                        else {
                                            throw WrongExMark(c_number_str, c_str);
                                        }
                                    }
                                    else {
                                        if (c_type == SpecialWords && c_str == "continue") {
                                            int label2 = POLIZ.get_free();
                                            POLIZ.blank();
                                            Lex lex_go_to(GOTO);
                                            POLIZ.put_lex(lex_go_to);
                                            check_continue = 1;
                                            label_for_continue = label2;
                                            get_lex();
                                            if (c_type == ExclamationMark) {
                                                get_lex();
                                                if (c_str == "}" || c_str == ")") {
                                                    return;
                                                }
                                                else {
                                                    internal_operator();
                                                }
                                            }
                                            else {
                                                throw WrongExMark(c_number_str, c_str);
                                            }
                                        }
                                        else {
                                            if (c_str == "}") {
                                                return;
                                            }
                                            multiple_definition();
                                            if (c_str == "}" || c_str == ")") {
                                                return;
                                            }
                                            else {
                                                internal_operator();
                                            }
                                        }
                                    }
                                }
                            }
                        }
                    }
                }
            }
        }
    }
}

void Parser::selection(Lex MAIN_lex) {
    if (c_type == SpecialWords && c_str == "choice") {
        get_lex();
        POLIZ.put_lex(MAIN_lex);
        if (c_type == RoundBracket && c_str == "(") {
            get_lex();
            if (c_type == Number) {

                Lex lexic(INT_DOUBLE_BOOL, std::stof(c_str));
                POLIZ.put_lex(lexic);
                Lex lexic1(YESEQUAL);
                POLIZ.put_lex(lexic1);
                int label1 = POLIZ.get_free();
                label_for_choice.push_back(label1);
                POLIZ.blank();
                Lex lex_go_to(GOTOFALSE);
                POLIZ.put_lex(lex_go_to);
                get_lex();
                if (c_type == RoundBracket && c_str == ")") {
                    get_lex();
                    if (c_type == CurlyBracket && c_str == "{") {
                        tid_list.push_back(List_Struct());
                        tid_list_not_delete.push_back(List_Struct(IndexNow));
                        IndexNow++;
                        get_lex();
                        internal_operator();
                        if (c_type == CurlyBracket && c_str == "}") {
                            tid_list.pop_back();
                            IndexNow--;
                            get_lex();
                            if (c_type == ExclamationMark) {
                                get_lex();
                                return;
                            }
                            else {
                                selection(MAIN_lex);
                            }
                        }
                        else {
                            throw WrongCurlyBracket_close(c_number_str, c_str);
                        }
                    }
                    else {
                        throw WrongCurlyBracket_open(c_number_str, c_str);
                    }
                }
                else {
                    throw WrongRoundBracket_close(c_number_str, c_str);
                }
            }
            else {
                throw WrongNumber(c_number_str, c_str);
            }
        }
        else {
            throw WrongRoundBracket_open(c_number_str, c_str);
        }
    }
    else {
        if (c_type == SpecialWords && c_str == "default") {
            get_lex();
            if (c_type == CurlyBracket && c_str == "{") {
                tid_list.push_back(List_Struct());
                tid_list_not_delete.push_back(List_Struct(IndexNow));
                IndexNow++;
                get_lex();
                internal_operator();
                if (c_type == CurlyBracket && c_str == "}") {
                    tid_list.pop_back();
                    IndexNow--;
                    get_lex();
                    if (c_type == ExclamationMark) {
                        get_lex();
                        return;
                    }
                    else {
                        selection(MAIN_lex);
                    }
                }
                else {
                    throw WrongCurlyBracket_close(c_number_str, c_str);
                }
            }
            else {
                throw WrongCurlyBracket_open(c_number_str, c_str);
            }
        }
        else {
            throw WrongSpecialWord_choice(c_number_str, c_str);
        }
    }
}

std::pair<type_of_object, bool> Parser::priority_level1(bool print) {
    bool check_array = 0;
    type_of_object type_1;
    std::pair<type_of_object, bool> now = priority_level2(print);
    type_1 = now.first;
    check_array = now.second;
    while (c_type == Comma) {
        get_lex();
        now = priority_level2(print);
        type_1 = now.first;
        if (now.second || check_array) {
            throw WrongArray_arith(c_number_str);
        }
        check_array = now.second;
        if (print) {
            Lex lexic(COMMA);
            POLIZ.put_lex(lexic);
        }
    }
    return std::make_pair(type_1, check_array);
}

std::pair<type_of_object, bool> Parser::priority_level2(bool print) {
    bool check_array = 0;
    type_of_object type_1;
    std::pair<type_of_object, bool> now = priority_level3(print);
    check_array = now.second;
    type_1 = now.first;
    while (c_type == Logical && c_str == "|") {
        get_lex();
        now = priority_level3(print);
        if (now.second || check_array) {
            throw WrongArray_arith(c_number_str);
        }
        type_1 = Bool;
        if (print) {
            Lex lexic(OR);
            POLIZ.put_lex(lexic);
        }
    }
    return std::make_pair(type_1, check_array);
}

std::pair<type_of_object, bool> Parser::priority_level3(bool print) {
    bool check_array = 0;
    type_of_object type_1;
    std::pair<type_of_object, bool> now = priority_level4(print);
    type_1 = now.first;
    check_array = now.second;
    while (c_type == Logical && c_str == "&") {
        get_lex();
        now = priority_level4(print);
        if (now.second || check_array) {
            throw WrongArray_arith(c_number_str);
        }
        type_1 = Bool;
        if (print) {
            Lex lexic(AND);
            POLIZ.put_lex(lexic);
        }
    }
    return std::make_pair(type_1, check_array);
}

std::pair<type_of_object, bool> Parser::priority_level4(bool print) {
    bool check_array = 0;
    type_of_object type_1, type_2;
    std::pair<type_of_object, bool> now = priority_level5(print);
    type_1 = now.first;
    check_array = now.second;
    while (c_type == Equally_NotEqually) {
        std::string sign = c_str;
        get_lex();
        now = priority_level5(print);
        type_2 = now.first;
        if (type_1 == Line && (type_2 != Line) || type_1 == NoChar && (type_2 != Line && type_2 != NoChar) || (type_1 != Line && type_1 != NoChar && (type_2 == Line || type_2 == NoChar)) || check_array != now.second) {
            if (check_array != now.second) {
                throw WrongArray_arith(c_number_str);
            }
            throw Wrong_type(c_number_str, type_1, type_2);
        }
        type_1 = Bool;
        if (sign == "~=" && print) {
            Lex lexic(NOTEQUAL);
            POLIZ.put_lex(lexic);
        }
        if (sign == "==" && print) {
            Lex lexic(YESEQUAL);
            POLIZ.put_lex(lexic);
        }
    }
    return std::make_pair(type_1, check_array);
}

std::pair<type_of_object, bool> Parser::priority_level5(bool print) {
    bool check_array = 0;
    type_of_object type_1, type_2;
    std::pair<type_of_object, bool> now = priority_level6(print);
    type_1 = now.first;
    check_array = now.second;
    while (c_type == Comparison) {
        std::string sign = c_str;
        if (check_array) {
            throw WrongArray_arith(c_number_str);
        }
        get_lex();
        now = priority_level6(print);
        type_2 = now.first;
        if (type_1 == Line && (type_2 != Line) || type_1 == NoChar && (type_2 != Line && type_2 != NoChar) || (type_1 != Line && type_1 != NoChar && (type_2 == Line || type_2 == NoChar)) || now.second) {
            if (now.second) {
                throw WrongArray_arith(c_number_str);
            }
            throw Wrong_type(c_number_str, type_1, type_2);
        }
        type_1 = Bool;
        if (sign == ">" && print) {
            Lex lexic(MORE);
            POLIZ.put_lex(lexic);
        }
        if (sign == ">=" && print) {
            Lex lexic(MORE_EQUAL);
            POLIZ.put_lex(lexic);
        }
        if (sign == "<" && print) {
            Lex lexic(LESS);
            POLIZ.put_lex(lexic);
        }
        if (sign == "<=" && print) {
            Lex lexic(LESS_EQUAL);
            POLIZ.put_lex(lexic);
        }
    }
    return std::make_pair(type_1, check_array);
}

std::pair<type_of_object, bool> Parser::priority_level6(bool print) {
    bool check_array = 0;
    type_of_object type_1, type_2;
    std::pair<type_of_object, bool> now = priority_level7(print);
    type_1 = now.first;
    check_array = now.second;
    while (c_type == Arithmetic && (c_str == "+" || c_str == "-")) {
        std::string sign = c_str;
        if (now.second || check_array) {
            throw WrongArray_arith(c_number_str);
        }
        get_lex();
        now = priority_level7(print);
        type_2 = now.first;
        if (now.second) {
            throw WrongArray_arith(c_number_str);
        }
        if (type_1 == NoChar || type_2 == NoChar) {
            throw WrongChar_arith(c_number_str);
        }
        if (type_1 == Line && (type_2 != Line) || type_1 == NoChar && (type_2 != Line && type_2 != NoChar) || (type_1 != Line && type_1 != NoChar && (type_2 == Line || type_2 == NoChar))) {
            throw Wrong_type(c_number_str, type_1, type_2);
        }
        if (type_1 == Valid || type_2 == Valid) {
            type_1 = Valid;
        }
        else {
            if (type_1 == Line && type_2 == Line) {
                type_1 = Line;
            }
            else {
                type_1 = Integer;
            }
        }
        if (sign == "+" && print) {
            Lex lexic(PLUS);
            POLIZ.put_lex(lexic);
        }
        if (sign == "-" && print) {
            Lex lexic(MINUS);
            POLIZ.put_lex(lexic);
        }
    }
    return std::make_pair(type_1, check_array);
}

std::pair<type_of_object, bool> Parser::priority_level7(bool print) {
    bool check_array = 0;
    type_of_object type_1, type_2;
    std::pair<type_of_object, bool> now = priority_level8(print);
    type_1 = now.first;
    check_array = now.second;
    while (c_type == Arithmetic && (c_str == "*" || c_str == "/" || c_str == "%")) {
        std::string sign = c_str;
        if (now.second || check_array) {
            throw WrongArray_arith(c_number_str);
        }
        get_lex();
        now = priority_level8(print);
        type_2 = now.first;
        if (type_1 == NoChar || type_2 == NoChar) {
            throw WrongChar_arith(c_number_str);
        }
        if (now.second) {
            throw WrongArray_arith(c_number_str);
        }
        if (type_1 == Line || type_2 == Line) {
            throw Wrong_type(c_number_str, type_1, type_2);
        }
        if (type_1 == Valid || type_2 == Valid) {
            type_1 = Valid;
        }
        else {
            type_1 = Integer;
        }
        if (sign == "*" && print) {
            Lex lexic(MULTIPLY);
            POLIZ.put_lex(lexic);
        }
        if (sign == "/" && print) {
            Lex lexic(DIVISION);
            POLIZ.put_lex(lexic);
        }
        if (sign == "%" && print) {
            Lex lexic(REMAINDIVISION);
            POLIZ.put_lex(lexic);
        }
    }
    return std::make_pair(type_1, check_array);
}

std::pair<type_of_object, bool> Parser::priority_level8(bool print) {
    bool check_array = 0;
    type_of_object type_1;
    if (c_str == "+" || c_str == "-" || c_str == "~") {
        std::string sign = c_str;
        get_lex();
        std::pair<type_of_object, bool> now = priority_level8(print);
        type_1 = now.first;
        if (now.second) {
            throw WrongArray_arith(c_number_str);
        }
        if (type_1 == NoChar) {
            throw WrongChar_arith(c_number_str);
        }
        if (type_1 == Line) {
            throw WrongNumber(c_number_str, "не число");
        }
        if (type_1 == Bool && sign != "~") {
            type_1 = Integer;
        }
        if (sign == "+" && print) {
            Lex lexic(UNOPLUS);
            POLIZ.put_lex(lexic);
        }
        if (sign == "-" && print) {
            Lex lexic(UNOMINUS);
            POLIZ.put_lex(lexic);
        }
        if (sign == "~" && print) {
            Lex lexic(UNODENY);
            POLIZ.put_lex(lexic);
        }
    }
    else {
        if (c_type == RoundBracket && c_str == "(") {
            get_lex();
            std::pair<type_of_object, bool> now = priority_level1(print);
            type_1 = now.first;
            check_array = now.second;
            if (c_type == RoundBracket && c_str == ")") {
                get_lex();
            }
            else {
                throw WrongRoundBracket_close(c_number_str, c_str);
            }
        }
        else {
            return operation_object(print);
        }
    }
    return std::make_pair(type_1, check_array);
}

std::pair<type_of_object, bool> Parser::operation_object(bool print) {
    type_of_object now_type;
    bool check_array = 0;
    if (c_type == Number) {
        Lex lexic(INT_DOUBLE_BOOL, std::stof(c_str));
        if (print) {
            POLIZ.put_lex(lexic);
        }
        if (c_str.find('.') == std::string::npos) {
            get_lex();
            return std::make_pair(Integer, 0);
        }
        else {
            get_lex();
            return std::make_pair(Valid, 0);
        }
    }
    else {
        if (c_type == ID) {
            Lex lexic(IDENTIF, c_str, Check_Array(c_str));
            std::string name = c_str;
            now_type = Check_ID(c_str);
            if (Check_Array(c_str)) {
                check_array = 1;
            }
            if (now_type == Nothing) {
                now_type = Check_ID_F(c_str);
                std::string name_f = c_str;
                if (now_type == Nothing) {
                    throw WrongInitialization(c_number_str, c_str);
                }
                else {
                    get_lex();
                    if (c_type == RoundBracket && c_str == "(") {
                        get_lex();
                        enumeration_arithmetic(GetParameters(name));
                        CORRECT_LABLE_FOR_FUNCTION(name_f, POLIZ.get_free() + 3);
                        Lex lexic2(LABEL, (double)GET_POLIZ_INDEX_OF_FUNCTION(name_f)), lexic(LABEL, (double)POLIZ.get_free() + 3), lexic3(GOTOCALLF);
                        POLIZ.put_lex(lexic);
                        POLIZ.put_lex(lexic2);
                        POLIZ.put_lex(lexic3);
                        if (c_type == RoundBracket && c_str == ")") {
                            get_lex();
                        }
                        else {
                            throw WrongRoundBracket_close(c_number_str, c_str);
                        }
                    }
                    else {
                        throw WrongRoundBracket_open(c_number_str, c_str);
                    }
                }
            }
            else {
                get_lex();
                if (c_type == Point) {
                    if (!Check_Array(name)) {
                        throw WrongArray_name(c_number_str, name);
                    }
                    get_lex();
                    check_array = 0;
                    if (c_type == SpecialWords && c_str == "len") {
                        Lex lexic(INT_DOUBLE_BOOL, (double)SizeOfArray(now_type, name));
                        if (print) {
                            POLIZ.put_lex(lexic);
                        }
                        now_type = Integer;
                        get_lex();
                    }
                    else {
                        throw WrongSpecial_word_len(c_number_str, c_str);
                    }
                }
                else {
                    POLIZ.put_lex(lexic);
                    if (c_type == SquareBrackets && c_str == "[") {
                        if (!Check_Array(name) && !(Check_ID(name) == Line)) {
                            throw WrongArray_name(c_number_str, name);
                        }
                        get_lex();
                        check_array = 0;
                        std::pair<type_of_object, bool> now = priority_level1(print);
                        type_of_object type_2 = now.first;
                        if (type_2 != Integer || now.second != 0) {
                            throw Wrong_Array_cell(c_number_str, type_2);
                        }
                        if (Check_ID(name) == Line) {
                            now_type = NoChar;
                        }
                        if (c_type == SquareBrackets && c_str == "]") {
                            get_lex();
                        }
                        else {
                            throw WrongSquareBracket_close(c_number_str, c_str);
                        }
                        Lex lexic1(INDEX);
                        POLIZ.put_lex(lexic1);
                    }
                }
            }
        }
        else {
            if (c_type == SpecialWords && (c_str == "true" || c_str == "false")) {
                now_type = Bool;
                bool_(print);
            }
            else {
                if (c_type == String) {
                    now_type = Line;
                    Lex lexic(STRING, c_str);
                    if (print) {
                        POLIZ.put_lex(lexic);
                    }
                    get_lex();
                }
                else {
                    if (c_type == NoChar0) {
                        now_type = NoChar;
                        Lex lexic(CHAR, c_str);
                        if (print) {
                            POLIZ.put_lex(lexic);
                        }
                        get_lex();
                    }
                    else {
                        throw WrongOperationObject(c_number_str, c_str);
                    }
                }
            }
        }
    }
    return std::make_pair(now_type, check_array);
}

void SaveValue(std::string name, std::string value) {
    std::list<List_Struct>::iterator it;
    for (it = tid_list_not_delete.begin(); it != tid_list_not_delete.end(); ++it) {
        for (int j = 0; j < it->TID.size(); ++j) {
            if (it->TID[j].name == name) {
                switch (it->TID[j].type) {
                case Integer:
                    it->TID[j].v_lex_int = std::stof(value);
                    return;
                    break;
                case Line:
                    it->TID[j].v_lex_string = value;
                    return;
                    break;
                case Valid:
                    it->TID[j].v_lex_int = std::stof(value);
                    return;
                    break;
                case Bool:
                    it->TID[j].v_lex_int = std::stof(value);
                    return;
                    break;
                case NoChar:
                    it->TID[j].v_lex_int = std::stof(value);
                    return;
                    break;
                }
            }
        }
    }
}

void PrintValue(std::string name) {
    std::list<List_Struct>::iterator it;
    for (it = tid_list_not_delete.begin(); it != tid_list_not_delete.end(); ++it) {
        for (int j = 0; j < it->TID.size(); ++j) {
            if (it->TID[j].name == name) {
                switch (it->TID[j].type) {
                case Integer:
                    std::cout << it->TID[j].v_lex_int << " ";
                    break;
                case Line:
                    std::cout << it->TID[j].v_lex_string << " ";
                    break;
                case Valid:
                    std::cout << it->TID[j].v_lex_int << " ";
                    break;
                case Bool:
                    std::cout << it->TID[j].v_lex_int << " ";
                    break;
                case NoChar:
                    std::cout << it->TID[j].v_lex_int << " ";
                    break;
                }
            }
        }
    }
}

type_of_object GetTypeOfId(std::string name) {
    std::list<List_Struct>::iterator it;
    for (it = tid_list_not_delete.begin(); it != tid_list_not_delete.end(); ++it) {
        for (int j = 0; j < it->TID.size(); ++j) {
            if (it->TID[j].name == name) {
                return it->TID[j].type;
            }
        }
    }
}

std::string GetStringOfId(std::string name) {
    std::list<List_Struct>::iterator it;
    for (it = tid_list_not_delete.begin(); it != tid_list_not_delete.end(); ++it) {
        for (int j = 0; j < it->TID.size(); ++j) {
            if (it->TID[j].name == name) {
                return it->TID[j].v_lex_string;
            }
        }
    }
}

double GetDoubleOfId(std::string name) {
    std::list<List_Struct>::iterator it;
    for (it = tid_list_not_delete.begin(); it != tid_list_not_delete.end(); ++it) {
        for (int j = 0; j < it->TID.size(); ++j) {
            if (it->TID[j].name == name) {
                return it->TID[j].v_lex_int;
            }
        }
    }
}

void SaveValueToVector(std::string name, std::string value) {
    std::list<List_Struct>::iterator it;
    for (it = tid_list_not_delete.begin(); it != tid_list_not_delete.end(); ++it) {
        for (int j = 0; j < it->TID.size(); ++j) {
            if (it->TID[j].name == name) {
                switch (it->TID[j].type) {
                case Integer:
                    it->TID[j].v_lex_vector_int.push_back(std::stof(value));
                    break;
                case Line:
                    it->TID[j].v_lex_vector_string.push_back(value);
                    break;
                case Valid:
                    it->TID[j].v_lex_vector_int.push_back(std::stof(value));
                    break;
                case Bool:
                    it->TID[j].v_lex_vector_int.push_back(std::stof(value));
                    break;
                case NoChar:
                    it->TID[j].v_lex_vector_int.push_back(std::stof(value));
                    break;
                }
            }
        }
    }
}

void DeleteValueToVector(std::string name) {
    std::list<List_Struct>::iterator it;
    for (it = tid_list_not_delete.begin(); it != tid_list_not_delete.end(); ++it) {
        for (int j = 0; j < it->TID.size(); ++j) {
            if (it->TID[j].name == name) {
                switch (it->TID[j].type) {
                case Integer:
                    if (it->TID[j].v_lex_vector_int.size() == 0) {
                        throw WrongTryDelete(name);
                    }
                    it->TID[j].v_lex_vector_int.pop_back();
                    break;
                case Line:
                    if (it->TID[j].v_lex_vector_string.size() == 0) {
                        throw WrongTryDelete(name);
                    }
                    it->TID[j].v_lex_vector_string.pop_back();
                    break;
                case Valid:
                    if (it->TID[j].v_lex_vector_int.size() == 0) {
                        throw WrongTryDelete(name);
                    }
                    it->TID[j].v_lex_vector_int.pop_back();
                    break;
                case Bool:
                    if (it->TID[j].v_lex_vector_int.size() == 0) {
                        throw WrongTryDelete(name);
                    }
                    it->TID[j].v_lex_vector_int.pop_back();
                    break;
                case NoChar:
                    if (it->TID[j].v_lex_vector_int.size() == 0) {
                        throw WrongTryDelete(name);
                    }
                    it->TID[j].v_lex_vector_int.pop_back();
                    break;
                }
            }
        }
    }
}

int GetVectorLen(std::string name) {
    std::list<List_Struct>::iterator it;
    for (it = tid_list_not_delete.begin(); it != tid_list_not_delete.end(); ++it) {
        for (int j = 0; j < it->TID.size(); ++j) {
            if (it->TID[j].name == name) {
                switch (it->TID[j].type) {
                case Integer:
                    return it->TID[j].v_lex_vector_int.size();
                    break;
                case Line:
                    return it->TID[j].v_lex_vector_string.size();
                    break;
                case Valid:
                    return it->TID[j].v_lex_vector_int.size();
                    break;
                case Bool:
                    return it->TID[j].v_lex_vector_int.size();
                    break;
                case NoChar:
                    return it->TID[j].v_lex_vector_int.size();
                    break;
                }
            }
        }
    }
}

void SaveVectorToVector(std::string name, std::string name_second) {
    std::list<List_Struct>::iterator it;
    std::list<List_Struct>::iterator it_second;
    bool find = 0;

    for (it = tid_list_not_delete.begin(); it != tid_list_not_delete.end(); ++it) {
        for (int j = 0; j < it->TID.size(); ++j) {
            if (it->TID[j].name == name) {
                switch (it->TID[j].type) {
                case Integer:
                    for (it_second = tid_list_not_delete.begin(); it_second != tid_list_not_delete.end(); ++it_second) {
                        for (int j_second = 0; j_second < it_second->TID.size(); ++j_second) {
                            if (it_second->TID[j_second].name == name_second) {
                                it->TID[j].v_lex_vector_int.clear();
                                for (int i = 0; i < it_second->TID[j_second].v_lex_vector_int.size(); ++i) {
                                    it->TID[j].v_lex_vector_int.push_back(it_second->TID[j_second].v_lex_vector_int[i]);
                                }
                            }
                        }
                    }
                    break;
                case Line:
                    for (it_second = tid_list_not_delete.begin(); it_second != tid_list_not_delete.end(); ++it_second) {
                        for (int j_second = 0; j_second < it_second->TID.size(); ++j_second) {
                            if (it_second->TID[j_second].name == name_second) {
                                it->TID[j].v_lex_vector_string.clear();
                                for (int i = 0; i < it_second->TID[j_second].v_lex_vector_string.size(); ++i) {
                                    it->TID[j].v_lex_vector_string[i] = it_second->TID[j_second].v_lex_vector_string[i];
                                }
                            }
                        }
                    }
                    break;
                case Valid:
                    for (it_second = tid_list_not_delete.begin(); it_second != tid_list_not_delete.end(); ++it_second) {
                        for (int j_second = 0; j_second < it_second->TID.size(); ++j_second) {
                            if (it_second->TID[j_second].name == name_second) {
                                for (int i = 0; i < std::min(it->TID[j].v_lex_vector_int.size(), it_second->TID[j_second].v_lex_vector_int.size()); ++i) {
                                    it->TID[j].v_lex_vector_int[i] = it_second->TID[j_second].v_lex_vector_int[i];
                                }
                                if (it_second->TID[j_second].v_lex_vector_int.size() > it->TID[j].v_lex_vector_int.size()) {
                                    for (int i = it->TID[j].v_lex_vector_int.size(); i < it_second->TID[j_second].v_lex_vector_int.size(); ++i) {
                                        it->TID[j].v_lex_vector_int.push_back(it_second->TID[j_second].v_lex_vector_int[i]);
                                    }
                                }
                            }
                        }
                    }
                    break;
                case Bool:
                    for (it_second = tid_list_not_delete.begin(); it_second != tid_list_not_delete.end(); ++it_second) {
                        for (int j_second = 0; j_second < it_second->TID.size(); ++j_second) {
                            if (it_second->TID[j_second].name == name_second) {
                                if (it_second->TID[j_second].name == name_second) {
                                    it->TID[j].v_lex_vector_int.clear();
                                    for (int i = 0; i < it_second->TID[j_second].v_lex_vector_int.size(); ++i) {
                                        it->TID[j].v_lex_vector_int.push_back(it_second->TID[j_second].v_lex_vector_int[i]);
                                    }
                                }
                            }
                        }
                    }
                    break;
                case NoChar:
                    for (it_second = tid_list_not_delete.begin(); it_second != tid_list_not_delete.end(); ++it_second) {
                        for (int j_second = 0; j_second < it_second->TID.size(); ++j_second) {
                            if (it_second->TID[j_second].name == name_second) {
                                if (it_second->TID[j_second].name == name_second) {
                                    it->TID[j].v_lex_vector_int.clear();
                                    for (int i = 0; i < it_second->TID[j_second].v_lex_vector_int.size(); ++i) {
                                        it->TID[j].v_lex_vector_int.push_back(it_second->TID[j_second].v_lex_vector_int[i]);
                                    }
                                }
                            }
                        }
                    }
                    break;
                }
            }
        }
    }
}

Lex GetValueInIndexToVector(std::string name, int value) {
    std::list<List_Struct>::iterator it;
    for (it = tid_list_not_delete.begin(); it != tid_list_not_delete.end(); ++it) {
        for (int j = 0; j < it->TID.size(); ++j) {
            if (it->TID[j].name == name) {
                switch (it->TID[j].type) {
                case Integer:
                    for (int i = 0; i < it->TID[j].v_lex_vector_int.size(); ++i) {
                        if (i == value) {
                            return Lex(INT_DOUBLE_BOOL, it->TID[j].v_lex_vector_int[i]);
                        }
                    }
                    throw Wrong_ArrayIndexOut(name);
                    break;
                case Line:
                    for (int i = 0; i < it->TID[j].v_lex_vector_string.size(); ++i) {
                        if (i == value) {
                            return Lex(STRING, it->TID[j].v_lex_vector_string[i]);
                        }
                    }
                    throw Wrong_ArrayIndexOut(name);
                    break;
                case Valid:
                    for (int i = 0; i < it->TID[j].v_lex_vector_int.size(); ++i) {
                        if (i == value) {
                            return Lex(INT_DOUBLE_BOOL, it->TID[j].v_lex_vector_int[i]);
                        }
                    }
                    throw Wrong_ArrayIndexOut(name);
                    break;
                case Bool:
                    for (int i = 0; i < it->TID[j].v_lex_vector_int.size(); ++i) {
                        if (i == value) {
                            return Lex(INT_DOUBLE_BOOL, it->TID[j].v_lex_vector_int[i]);
                        }
                    }
                    throw Wrong_ArrayIndexOut(name);
                    break;
                case NoChar:
                    for (int i = 0; i < it->TID[j].v_lex_vector_int.size(); ++i) {
                        if (i == value) {
                            return Lex(CHAR, it->TID[j].v_lex_vector_int[i]);
                        }
                    }
                    throw Wrong_ArrayIndexOut(name);
                    break;
                }
            }
        }
    }
}

void RETURN_PREV_INDEX(std::string name) {
    std::list<List_Struct_F>::iterator it;
    for (it = tid_list_f.begin(); it != tid_list_f.end(); ++it) {
        for (int j = 0; j < it->TID.size(); ++j) {
            if (it->TID[j].name == name) {
                it->TID[j].label = it->TID[j].prev_label.top();
                it->TID[j].prev_label.pop();
            }
        }
    }
}

void PushVector(std::string name, std::string str) {
    std::list<List_Struct>::iterator it;
    for (it = tid_list_not_delete.begin(); it != tid_list_not_delete.end(); ++it) {
        for (int j = 0; j < it->TID.size(); ++j) {
            if (it->TID[j].name == name) {
                switch (it->TID[j].type) {
                case Integer:
                    it->TID[j].v_lex_vector_int.push_back(stoi(str));
                    break;
                case Line:
                    it->TID[j].v_lex_vector_string.push_back(str);
                    break;
                case Valid:
                    it->TID[j].v_lex_vector_int.push_back(stoi(str));
                    break;
                case Bool:
                    it->TID[j].v_lex_vector_int.push_back(stoi(str));
                    break;
                case NoChar:
                    it->TID[j].v_lex_vector_int.push_back(stoi(str));
                    break;
                }
            }
        }
    }
}

void Interpreter() {
    std::stack<Lex> args;
    int index = GET_POLIZ_INDEX_OF_FUNCTION("main"), size = POLIZ.get_free();
    CORRECT_LABLE_FOR_FUNCTION("main", 1000);
    int dop_index;
    while (index < size) {
        Lex pc_el = POLIZ[index];
        Lex top, top1, top2;
        std::string str;
        // for (std::stack<Lex> dump = args; !dump.empty(); dump.pop())
          //   LexemWord(dump.top().get_type());
         //std::cout << "\n\n";
        switch (pc_el.get_type()) {
            break;
        case ADD:
            top2 = args.top();
            args.pop();
            top1 = args.top();
            args.pop();
            if (top1.get_type() == STRING || top2.get_type() == STRING) {
                std::string str1, str2;
                if (top2.get_type() == STRING) {
                    str2 = top2.v_lex_string;
                }
                else {
                    str2 = GetStringOfId(top2.name);
                }
                PushVector(top1.name, str2);
            }
            else {
                double arg1, arg2;
                if (top2.get_type() == INT_DOUBLE_BOOL) {
                    arg2 = top2.v_lex_int;
                }
                else {
                    arg2 = GetDoubleOfId(top2.name);
                }
                PushVector(top1.name, to_string(arg2));
            }
            break;
        case TRUE:
            args.push(pc_el);
            break;
        case FALSE:
            args.push(pc_el);
            break;
        case OUTPUT:
            top = args.top();
            args.pop();
            switch (top.get_type()) {
            case IDENTIF:
                PrintValue(top.name);
                break;
            case INT_DOUBLE_BOOL:
                std::cout << top.v_lex_int << "  ";
                break;
            case STRING:
                std::cout << top.v_lex_string << "  ";
                break;
            case CHAR:
                std::cout << top.v_lex_int << "  ";
                break;
            }
            break;
        case INPUT:
            std::cin >> str;
            top = args.top();
            args.pop();
            SaveValue(top.name, str);
            break;
        case POP:
            top = args.top();
            args.pop();
            try {
                DeleteValueToVector(top.name);
            }
            catch (const WrongTryDelete& ex) {
                std::cout << "Ошибка: удаление последнего элемента в пустом массиве " + ex.name << std::endl;
            }
            break;
        case LEN:
            top = args.top();
            args.pop();
            args.push(Lex(INT_DOUBLE_BOOL, (double)GetVectorLen(top.name)));
            break;
        case UNOPLUS:
            top1 = args.top();
            args.pop();
            if (true) {
                double arg1;
                if (top1.get_type() == INT_DOUBLE_BOOL) {
                    arg1 = top1.v_lex_int;
                }
                else {
                    arg1 = GetDoubleOfId(top1.name);
                }
                Lex element(INT_DOUBLE_BOOL, +arg1);
                args.push(element);
            }
            break;
        case UNOMINUS:
            top1 = args.top();
            args.pop();
            if (true) {
                double arg1;
                if (top1.get_type() == INT_DOUBLE_BOOL) {
                    arg1 = top1.v_lex_int;
                }
                else {
                    arg1 = GetDoubleOfId(top1.name);
                }
                Lex element(INT_DOUBLE_BOOL, -arg1);
                args.push(element);
            }
            break;
        case UNODENY:
            top1 = args.top();
            args.pop();
            if (true) {
                double arg1;
                if (top1.get_type() == INT_DOUBLE_BOOL) {
                    arg1 = top1.v_lex_int;
                }
                else {
                    arg1 = GetDoubleOfId(top1.name);
                }
                Lex element(INT_DOUBLE_BOOL, !arg1);
                args.push(element);
            }
            break;
        case PLUS:
            top1 = args.top();
            args.pop();
            top2 = args.top();
            args.pop();
            if (top1.get_type() == STRING || top2.get_type() == STRING) {
                std::string str1, str2;
                if (top1.get_type() == STRING) {
                    str1 = top1.v_lex_string;
                }
                else {
                    str1 = GetStringOfId(top1.name);
                }
                if (top2.get_type() == STRING) {
                    str2 = top2.v_lex_string;
                }
                else {
                    str2 = GetStringOfId(top2.name);
                }
                Lex element(STRING, str2 + str1);
                args.push(element);
            }
            else {
                double arg1, arg2;
                if (top1.get_type() == INT_DOUBLE_BOOL) {
                    arg1 = top1.v_lex_int;
                }
                else {
                    arg1 = GetDoubleOfId(top1.name);
                }
                if (top2.get_type() == INT_DOUBLE_BOOL) {
                    arg2 = top2.v_lex_int;
                }
                else {
                    arg2 = GetDoubleOfId(top2.name);
                }
                Lex element(INT_DOUBLE_BOOL, arg2 + arg1);
                args.push(element);
            }
            break;
        case MINUS:
            top1 = args.top();
            args.pop();
            top2 = args.top();
            args.pop();
            if (true) {
                double arg1, arg2;
                if (top1.get_type() == INT_DOUBLE_BOOL) {
                    arg1 = top1.v_lex_int;
                }
                else {
                    arg1 = GetDoubleOfId(top1.name);
                }
                if (top2.get_type() == INT_DOUBLE_BOOL) {
                    arg2 = top2.v_lex_int;
                }
                else {
                    arg2 = GetDoubleOfId(top2.name);
                }
                Lex element(INT_DOUBLE_BOOL, arg2 - arg1);
                args.push(element);
            }
            break;
        case DIVISION:
            top1 = args.top();
            args.pop();
            top2 = args.top();
            args.pop();
            if (true) {
                double arg1, arg2;
                if (top1.get_type() == INT_DOUBLE_BOOL) {
                    arg1 = top1.v_lex_int;
                }
                else {
                    arg1 = GetDoubleOfId(top1.name);
                }
                if (top2.get_type() == INT_DOUBLE_BOOL) {
                    arg2 = top2.v_lex_int;
                }
                else {
                    arg2 = GetDoubleOfId(top2.name);
                }
                Lex element(INT_DOUBLE_BOOL, arg2 / arg1);
                args.push(element);
            }
            break;
        case MULTIPLY:
            top1 = args.top();
            args.pop();
            top2 = args.top();
            args.pop();
            if (true) {
                double arg1, arg2;
                if (top1.get_type() == INT_DOUBLE_BOOL) {
                    arg1 = top1.v_lex_int;
                }
                else {
                    arg1 = GetDoubleOfId(top1.name);
                }
                if (top2.get_type() == INT_DOUBLE_BOOL) {
                    arg2 = top2.v_lex_int;
                }
                else {
                    arg2 = GetDoubleOfId(top2.name);
                }
                Lex element(INT_DOUBLE_BOOL, arg2 * arg1);
                args.push(element);
            }
            break;
        case REMAINDIVISION:
            top1 = args.top();
            args.pop();
            top2 = args.top();
            args.pop();
            if (true) {
                double arg1, arg2;
                if (top1.get_type() == INT_DOUBLE_BOOL) {
                    arg1 = top1.v_lex_int;
                }
                else {
                    arg1 = GetDoubleOfId(top1.name);
                }
                if (top2.get_type() == INT_DOUBLE_BOOL) {
                    arg2 = top2.v_lex_int;
                }
                else {
                    arg2 = GetDoubleOfId(top2.name);
                }
                Lex element(INT_DOUBLE_BOOL, double((int)arg2 % (int)arg1));
                args.push(element);
            }
            break;
        case IDENTIF:
            args.push(pc_el);
            break;
        case INT_DOUBLE_BOOL:
            args.push(pc_el);
            break;
        case STRING:
            args.push(pc_el);
            break;
        case CHAR:
            args.push(pc_el);
            break;
        case COMMA:
            top1 = args.top();
            args.pop();
            top2 = args.top();
            args.pop();
            args.push(top1);
            break;
        case OR:
            top1 = args.top();
            args.pop();
            top2 = args.top();
            args.pop();
            if (top1.get_type() == STRING || top2.get_type() == STRING) {
                std::string str1, str2;
                if (top1.get_type() == STRING) {
                    str1 = top1.v_lex_string;
                }
                else {
                    str1 = GetStringOfId(top1.name);
                }
                if (top2.get_type() == STRING) {
                    str2 = top2.v_lex_string;
                }
                else {
                    str2 = GetStringOfId(top2.name);
                }
                if (str1.size() || str2.size()) {
                    Lex element(TRUE, (bool)1);
                    args.push(element);
                }
                else {
                    Lex element(FALSE, (bool)0);
                    args.push(element);
                }
            }
            else {
                double arg1, arg2;
                if (top1.get_type() == INT_DOUBLE_BOOL) {
                    arg1 = top1.v_lex_int;
                }
                else {
                    arg1 = GetDoubleOfId(top1.name);
                }
                if (top2.get_type() == INT_DOUBLE_BOOL) {
                    arg2 = top2.v_lex_int;
                }
                else {
                    arg2 = GetDoubleOfId(top2.name);
                }
                if (arg1 || arg2) {
                    Lex element(TRUE, (bool)1);
                    args.push(element);
                }
                else {
                    Lex element(FALSE, (bool)0);
                    args.push(element);
                }
            }
            break;
        case AND:
            top1 = args.top();
            args.pop();
            top2 = args.top();
            args.pop();
            if (top1.get_type() == STRING || top2.get_type() == STRING) {
                std::string str1, str2;
                if (top1.get_type() == STRING) {
                    str1 = top1.v_lex_string;
                }
                else {
                    str1 = GetStringOfId(top1.name);
                }
                if (top2.get_type() == STRING) {
                    str2 = top2.v_lex_string;
                }
                else {
                    str2 = GetStringOfId(top2.name);
                }
                if (str1.size() && str2.size()) {
                    Lex element(TRUE, (bool)1);
                    args.push(element);
                }
                else {
                    Lex element(FALSE, (bool)0);
                    args.push(element);
                }
            }
            else {
                double arg1, arg2;
                if (top1.get_type() == INT_DOUBLE_BOOL) {
                    arg1 = top1.v_lex_int;
                }
                else {
                    arg1 = GetDoubleOfId(top1.name);
                }
                if (top2.get_type() == INT_DOUBLE_BOOL) {
                    arg2 = top2.v_lex_int;
                }
                else {
                    arg2 = GetDoubleOfId(top2.name);
                }
                if (arg1 && arg2) {
                    Lex element(TRUE, (bool)1);
                    args.push(element);
                }
                else {
                    Lex element(FALSE, (bool)0);
                    args.push(element);
                }
            }
            break;
        case MORE:
            top1 = args.top();
            args.pop();
            top2 = args.top();
            args.pop();
            if (top1.get_type() == STRING || top2.get_type() == STRING) {
                std::string str1, str2;
                if (top1.get_type() == STRING) {
                    str1 = top1.v_lex_string;
                }
                else {
                    str1 = GetStringOfId(top1.name);
                }
                if (top2.get_type() == STRING) {
                    str2 = top2.v_lex_string;
                }
                else {
                    str2 = GetStringOfId(top2.name);
                }
                if (str2.size() > str1.size()) {
                    Lex element(TRUE, (bool)1);
                    args.push(element);
                }
                else {
                    Lex element(FALSE, (bool)0);
                    args.push(element);
                }
            }
            else {
                double arg1, arg2;
                if (top1.get_type() == INT_DOUBLE_BOOL) {
                    arg1 = top1.v_lex_int;
                }
                else {
                    arg1 = GetDoubleOfId(top1.name);
                }
                if (top2.get_type() == INT_DOUBLE_BOOL) {
                    arg2 = top2.v_lex_int;
                }
                else {
                    arg2 = GetDoubleOfId(top2.name);
                }
                if (arg2 > arg1) {
                    Lex element(TRUE, (bool)1);
                    args.push(element);
                }
                else {
                    Lex element(FALSE, (bool)0);
                    args.push(element);
                }
            }
            break;
        case LESS:
            top1 = args.top();
            args.pop();
            top2 = args.top();
            args.pop();
            if (top1.get_type() == STRING || top2.get_type() == STRING) {
                std::string str1, str2;
                if (top1.get_type() == STRING) {
                    str1 = top1.v_lex_string;
                }
                else {
                    str1 = GetStringOfId(top1.name);
                }
                if (top2.get_type() == STRING) {
                    str2 = top2.v_lex_string;
                }
                else {
                    str2 = GetStringOfId(top2.name);
                }
                if (str2.size() < str1.size()) {
                    Lex element(TRUE, (bool)1);
                    args.push(element);
                }
                else {
                    Lex element(FALSE, (bool)0);
                    args.push(element);
                }
            }
            else {
                double arg1, arg2;
                if (top1.get_type() == INT_DOUBLE_BOOL) {
                    arg1 = top1.v_lex_int;
                }
                else {
                    arg1 = GetDoubleOfId(top1.name);
                }
                if (top2.get_type() == INT_DOUBLE_BOOL) {
                    arg2 = top2.v_lex_int;
                }
                else {
                    arg2 = GetDoubleOfId(top2.name);
                }
                if (arg2 < arg1) {
                    Lex element(TRUE, (bool)1);
                    args.push(element);
                }
                else {
                    Lex element(FALSE, (bool)0);
                    args.push(element);
                }
            }
            break;
        case MORE_EQUAL:
            top1 = args.top();
            args.pop();
            top2 = args.top();
            args.pop();
            if (top1.get_type() == STRING || top2.get_type() == STRING) {
                std::string str1, str2;
                if (top1.get_type() == STRING) {
                    str1 = top1.v_lex_string;
                }
                else {
                    str1 = GetStringOfId(top1.name);
                }
                if (top2.get_type() == STRING) {
                    str2 = top2.v_lex_string;
                }
                else {
                    str2 = GetStringOfId(top2.name);
                }
                if (str2.size() >= str1.size()) {
                    Lex element(TRUE, (bool)1);
                    args.push(element);
                }
                else {
                    Lex element(FALSE, (bool)0);
                    args.push(element);
                }
            }
            else {
                double arg1, arg2;
                if (top1.get_type() == INT_DOUBLE_BOOL) {
                    arg1 = top1.v_lex_int;
                }
                else {
                    arg1 = GetDoubleOfId(top1.name);
                }
                if (top2.get_type() == INT_DOUBLE_BOOL) {
                    arg2 = top2.v_lex_int;
                }
                else {
                    arg2 = GetDoubleOfId(top2.name);
                }
                if (arg2 >= arg1) {
                    Lex element(TRUE, (bool)1);
                    args.push(element);
                }
                else {
                    Lex element(FALSE, (bool)0);
                    args.push(element);
                }
            }
            break;
        case LESS_EQUAL:
            top1 = args.top();
            args.pop();
            top2 = args.top();
            args.pop();
            if (top1.get_type() == STRING || top2.get_type() == STRING) {
                std::string str1, str2;
                if (top1.get_type() == STRING) {
                    str1 = top1.v_lex_string;
                }
                else {
                    str1 = GetStringOfId(top1.name);
                }
                if (top2.get_type() == STRING) {
                    str2 = top2.v_lex_string;
                }
                else {
                    str2 = GetStringOfId(top2.name);
                }
                if (str2.size() <= str1.size()) {
                    Lex element(TRUE, (bool)1);
                    args.push(element);
                }
                else {
                    Lex element(FALSE, (bool)0);
                    args.push(element);
                }
            }
            else {
                double arg1, arg2;
                if (top1.get_type() == INT_DOUBLE_BOOL) {
                    arg1 = top1.v_lex_int;
                }
                else {
                    arg1 = GetDoubleOfId(top1.name);
                }
                if (top2.get_type() == INT_DOUBLE_BOOL) {
                    arg2 = top2.v_lex_int;
                }
                else {
                    arg2 = GetDoubleOfId(top2.name);
                }
                if (arg2 <= arg1) {
                    Lex element(TRUE, (bool)1);
                    args.push(element);
                }
                else {
                    Lex element(FALSE, (bool)0);
                    args.push(element);
                }
            }
            break;
        case EQUAL:
            top2 = args.top();
            bool check1 = 0, check2 = 0;
            int index1, index2;
            if (top2.get_type() == INDEX) {
                check2 = 1;
                top2 = args.top();

                args.pop();
                top2 = args.top();
                args.pop();
            }
            args.pop();
            top1 = args.top();

            args.pop();
            if (top1.get_type() == STRING) {
                std::string str1, str2;
                if (top2.get_type() == STRING) {
                    str2 = top2.v_lex_string;
                }
                else {
                    str2 = GetStringOfId(top2.name);
                }
                SaveValue(top1.name, str2);
            }
            else {
                if (top1.get_type() == ARRAY_INT_VALID_BOOL) {
                    SaveValueToVector(top1.name, top2.name);
                }
                else {
                    if (top1.get_type() == ARRAY_LINE) {
                        SaveValueToVector(top1.name, top2.name);
                    }
                    else {
                        if (top1.get_type() == ARRAY_NOCHAR) {
                            SaveValueToVector(top1.name, top2.name);
                        }
                        else {
                            double arg1, arg2;
                            if (top2.get_type() == INT_DOUBLE_BOOL) {
                                arg2 = top2.v_lex_int;
                            }
                            else {
                                arg2 = GetDoubleOfId(top2.name);
                            }
                            SaveValue(top1.name, to_string(arg2));
                        }
                    }
                }
            }
            break;
        case YESEQUAL:
            top1 = args.top();
            args.pop();
            top2 = args.top();
            args.pop();
            if (top1.get_type() == STRING || top2.get_type() == STRING) {
                std::string str1, str2;
                if (top1.get_type() == STRING) {
                    str1 = top1.v_lex_string;
                }
                else {
                    str1 = GetStringOfId(top1.name);
                }
                if (top2.get_type() == STRING) {
                    str2 = top2.v_lex_string;
                }
                else {
                    str2 = GetStringOfId(top2.name);
                }
                if (str1 == str2) {
                    Lex element(TRUE, (bool)1);
                    args.push(element);
                }
                else {
                    Lex element(FALSE, (bool)0);
                    args.push(element);
                }
            }
            else {
                double arg1, arg2;
                if (top1.get_type() == INT_DOUBLE_BOOL) {
                    arg1 = top1.v_lex_int;
                }
                else {
                    arg1 = GetDoubleOfId(top1.name);
                }
                if (top2.get_type() == INT_DOUBLE_BOOL) {
                    arg2 = top2.v_lex_int;
                }
                else {
                    arg2 = GetDoubleOfId(top2.name);
                }
                if (arg1 == arg2) {
                    Lex element(TRUE, (bool)1);
                    args.push(element);
                }
                else {
                    Lex element(FALSE, (bool)0);
                    args.push(element);
                }
            }
            break;
        case NOTEQUAL:
            top1 = args.top();
            args.pop();
            top2 = args.top();
            args.pop();
            if (top1.get_type() == STRING || top2.get_type() == STRING) {
                std::string str1, str2;
                if (top1.get_type() == STRING) {
                    str1 = top1.v_lex_string;
                }
                else {
                    str1 = GetStringOfId(top1.name);
                }
                if (top2.get_type() == STRING) {
                    str2 = top2.v_lex_string;
                }
                else {
                    str2 = GetStringOfId(top2.name);
                }
                if (str1 != str2) {
                    Lex element(TRUE, (bool)1);
                    args.push(element);
                }
                else {
                    Lex element(FALSE, (bool)0);
                    args.push(element);
                }
            }
            else {
                double arg1, arg2;
                if (top1.get_type() == INT_DOUBLE_BOOL) {
                    arg1 = top1.v_lex_int;
                }
                else {
                    arg1 = GetDoubleOfId(top1.name);
                }
                if (top2.get_type() == INT_DOUBLE_BOOL) {
                    arg2 = top2.v_lex_int;
                }
                else {
                    arg2 = GetDoubleOfId(top2.name);
                }
                if (arg1 != arg2) {
                    Lex element(TRUE, (bool)1);
                    args.push(element);
                }
                else {
                    Lex element(FALSE, (bool)0);
                    args.push(element);
                }
            }
            break;
        case INDEX:
            if (true) {
                Lex element(INDEX, (bool)1);
                args.push(element);
            }
            /*top1 = args.top();
            args.pop();
            top2 = args.top();
            args.pop();
            int arg1;
            if (top1.get_type() == INT_DOUBLE_BOOL) {
                arg1 = top1.v_lex_int;
            }
            else {
                arg1 = GetDoubleOfId(top1.name);
            }
            args.push(GetValueInIndexToVector(top2.name, arg1));*/
            break;
        case GOTO:
            index = args.top().v_lex_int - 1;
            args.pop();
            break;
        case GOTOCALLF:
            top = args.top();
            args.pop();
            top1 = args.top();
            args.pop();
            CORRECT_LABLE_FOR_FUNCTION(GET_NAME_BY_POLIZ_INDEX(top.v_lex_int), top1.v_lex_int);
            if (true) {
                std::vector<std::pair<type_of_object, std::pair<std::string, bool>>> argument = GetParameters(GET_NAME_BY_POLIZ_INDEX(top.v_lex_int));
                for (int i = argument.size() - 1; i >= 0; --i) {
                    top2 = args.top();
                    args.pop();
                    switch (top2.get_type()) {
                    case INT_DOUBLE_BOOL:
                        SaveValue(argument[i].second.first, std::to_string(top2.v_lex_int));
                        break;
                    case CHAR:
                        SaveValue(argument[i].second.first, std::to_string(top2.v_lex_int));
                        break;
                    case STRING:
                        SaveValue(argument[i].second.first, top2.v_lex_string);
                        break;
                    case IDENTIF:
                        if (Check_Array(argument[i].second.first)) {
                            type_of_object type = Check_ID(argument[i].second.first);
                            SaveVectorToVector(argument[i].second.first, top2.name);
                        }
                        else {
                            type_of_object type = Check_ID(argument[i].second.first);
                            switch (type) {
                            case Integer:
                            case Bool:
                            case NoChar:
                            case Valid:
                                SaveValue(argument[i].second.first, std::to_string(top2.v_lex_int));
                                break;
                            case String:
                                SaveValue(argument[i].second.first, top2.v_lex_string);
                                break;
                            }
                        }
                    }

                }
                index = top.v_lex_int - 1;
            }
            break;
        case GOTOFALSE:
            dop_index = args.top().v_lex_int;
            args.pop();
            top1 = args.top();
            args.pop();
            if (top1.get_type() == FALSE || (top1.get_type() == INT_DOUBLE_BOOL && top1.v_lex_int == 0) || (top1.get_type() == STRING && top1.v_lex_string.size() == 0)) {
                index = dop_index - 1;
            }
            break;
        case LABEL:
            args.push(pc_el);
            break;
        case LABEL_F:
            pc_el.v_lex_int = GET_RETURN_LABEL_OF_FUNCTION(GET_NAME_BY_POLIZ_INDEX(index));
            args.push(pc_el);
            RETURN_PREV_INDEX(GET_NAME_BY_POLIZ_INDEX(index));
            break;
        }
        // end of switch
        ++index;
    };
    //end of while
}

int main()
{
    tid_list.push_back(List_Struct());
    tid_list_not_delete.push_back(List_Struct());
    tid_list_f.push_back(List_Struct_F());
    setlocale(LC_ALL, "Russian");
    std::string service_word;
    service_words.open("C:/Users/mvikt/Desktop/service_words.txt");
    code.open("C:/Users/mvikt/Desktop/now_code.txt");
    int now_line_number = 1;
    while (service_words >> service_word) {
        push(service_word);
    }
    code.get(ch);
    std::string str;
    Condition now_condition = Begin2;
    bool check_comment = 0;
    try {
        while (!code.eof()) {
            switch (now_condition) {
            case Begin2:
                if (ch == 39) {
                    str += ch;
                    code.get(ch);
                    str += ch;
                    code.get(ch);
                    if (ch == 39) {
                        str += ch;
                        code.get(ch);
                        tockens.push_back(Token(str, type_of_lexem::NoChar0, now_line_number));
                        str = "";
                    }
                    else {
                        throw WrongChar_arith(now_line_number);
                    }
                    break;
                }
                if (ch == '?') {
                    code.get(ch);
                    while (ch != '?') {
                        code.get(ch);
                    }
                    code.get(ch);
                    break;
                }
                if (ch == ' ') {
                    code.get(ch);
                    break;
                }
                if (ch == '[' || ch == ']') {
                    str += ch;
                    tockens.push_back(Token(str, type_of_lexem::SquareBrackets, now_line_number));
                    str = "";
                    code.get(ch);
                    break;
                }
                if (ch == '\n') {
                    code.get(ch);
                    now_line_number++;
                    break;
                }
                if (('a' <= ch && ch <= 'z') || ('A' <= ch && ch <= 'Z') || ch == '_' || ('а' <= ch && ch <= 'я') || ('А' <= ch && ch <= 'Я')) {
                    now_condition = Word2;
                    str += ch;
                    code.get(ch);
                    break;
                }
                if (ch >= '0' && ch <= '9') {
                    now_condition = Number2;
                    str += ch;
                    code.get(ch);
                    break;
                }
                if (ch == '!') {
                    str += ch;
                    tockens.push_back(Token(str, type_of_lexem::ExclamationMark, now_line_number));
                    str = "";
                    code.get(ch);
                    break;
                }
                if (ch == ',') {
                    str += ch;
                    tockens.push_back(Token(str, type_of_lexem::Comma, now_line_number));
                    str = "";
                    code.get(ch);
                    break;
                }
                if (ch == ';') {
                    str += ch;
                    tockens.push_back(Token(str, type_of_lexem::Semicolon, now_line_number));
                    str = "";
                    code.get(ch);
                    break;
                }
                if (ch == '.') {
                    str += ch;
                    tockens.push_back(Token(str, type_of_lexem::Point, now_line_number));
                    str = "";
                    code.get(ch);
                    break;
                }
                if (ch == '(' || ch == ')') {
                    str += ch;
                    tockens.push_back(Token(str, type_of_lexem::RoundBracket, now_line_number));
                    str = "";
                    code.get(ch);
                    break;
                }
                if (ch == '{' || ch == '}') {
                    str += ch;
                    tockens.push_back(Token(str, type_of_lexem::CurlyBracket, now_line_number));
                    str = "";
                    code.get(ch);
                    break;
                }
                if (ch == '+' || ch == '-' || ch == '/' || ch == '*' || ch == '%') {
                    str += ch;
                    tockens.push_back(Token(str, type_of_lexem::Arithmetic, now_line_number));
                    str = "";
                    code.get(ch);
                    break;
                }
                if (ch == '=') {
                    now_condition = Equal2;
                    str += ch;
                    code.get(ch);
                    break;
                }
                if (ch == '<' || ch == '>') {
                    now_condition = ComparisonСomposite2;
                    str += ch;
                    code.get(ch);
                    break;
                }
                if (ch == '|' || ch == '&') {
                    str += ch;
                    tockens.push_back(Token(str, type_of_lexem::Logical, now_line_number));
                    str = "";
                    code.get(ch);
                    break;
                }
                if (ch == '#') {
                    str += ch;
                    tockens.push_back(Token(str, type_of_lexem::EndOfFile, now_line_number));
                    str = "";
                    code.get(ch);
                    break;
                }
                if (ch == '"') {
                    now_condition = QuotationMarks2;
                    str += ch;
                    code.get(ch);
                    break;
                }
                if (ch == '?') {
                    now_condition = Comment2;
                    str += ch;
                    code.get(ch);
                    break;
                }
                if (ch == '~') {
                    now_condition = Negation2;
                    str += ch;
                    code.get(ch);
                    break;
                }
                now_condition = Error2;
                break;
            case Word2:
                if (('a' <= ch && ch <= 'z') || ('A' <= ch && ch <= 'Z') || ch == '_' || (ch >= '0' && ch <= '9') || ('а' <= ch && ch <= 'я') || ('А' <= ch && ch <= 'Я')) {
                    now_condition = Word2;
                    str += ch;
                    code.get(ch);
                }
                else {
                    now_condition = Begin2;
                    if (be_here(str)) {
                        tockens.push_back(Token(str, type_of_lexem::SpecialWords, now_line_number));
                    }
                    else {
                        tockens.push_back(Token(str, type_of_lexem::ID, now_line_number));
                    }
                    str = "";
                }
                break;
            case Number2:
                if (ch >= '0' && ch <= '9') {
                    now_condition = Number2;
                    str += ch;
                    code.get(ch);
                }
                else {
                    if (ch == '.') {
                        now_condition = FractionalNumber2;
                        str += ch;
                        code.get(ch);
                    }
                    else {
                        tockens.push_back(Token(str, type_of_lexem::Number, now_line_number));
                        now_condition = Begin2;
                        str = "";
                    }
                }
                break;
            case FractionalNumber2:
                if (ch >= '0' && ch <= '9') {
                    str += ch;
                    code.get(ch);
                }
                else {
                    if (str[str.size() - 1] == '.') {
                        std::cout << "Некорректная запись дробного числа в строке " << now_line_number;
                        return 0;
                    }
                    tockens.push_back(Token(str, type_of_lexem::Number, now_line_number));
                    now_condition = Begin2;
                    str = "";
                }
                break;
            case Error2:
                str += ch;
                code.get(ch);
                tockens.push_back(Token(str, type_of_lexem::Error, now_line_number));
                now_condition = Begin2;
                str = "";
                break;
            case QuotationMarks2:
                if (ch != '"') {
                    now_condition = QuotationMarks2;
                    str += ch;
                    code.get(ch);
                }
                else {
                    str += ch;
                    code.get(ch);
                    tockens.push_back(Token(str, type_of_lexem::String, now_line_number));
                    now_condition = Begin2;
                    str = "";
                }
                break;
            case Comment2:
                if (ch != '?') {
                    now_condition = Comment2;
                    str += ch;
                    code.get(ch);
                }
                else {
                    str += ch;
                    code.get(ch);
                    tockens.push_back(Token(str, type_of_lexem::Comment, now_line_number));
                    now_condition = Begin2;
                    str = "";
                }
                break;
            case Equal2:
                if (ch == '=') {
                    str += ch;
                    code.get(ch);
                    tockens.push_back(Token(str, type_of_lexem::Equally_NotEqually, now_line_number));
                    now_condition = Begin2;
                    str = "";
                }
                else {
                    tockens.push_back(Token(str, type_of_lexem::Equal, now_line_number));
                    str = "";
                    now_condition = Begin2;
                }
                break;
            case Negation2:
                if (ch == '=') {
                    str += ch;
                    code.get(ch);
                    tockens.push_back(Token(str, type_of_lexem::Equally_NotEqually, now_line_number));
                    now_condition = Begin2;
                    str = "";
                }
                else {
                    tockens.push_back(Token(str, type_of_lexem::Negation, now_line_number));
                    str = "";
                    now_condition = Begin2;
                }
                break;
            case ComparisonСomposite2:
                if (ch == '=') {
                    str += ch;
                    code.get(ch);
                    tockens.push_back(Token(str, type_of_lexem::Comparison, now_line_number));
                    now_condition = Begin2;
                    str = "";
                }
                else {
                    tockens.push_back(Token(str, type_of_lexem::Comparison, now_line_number));
                    str = "";
                    now_condition = Begin2;
                }
            }
        }
        if (str != "") {
            switch (now_condition)
            {
            case Word2:
                if (be_here(str)) {
                    tockens.push_back(Token(str, type_of_lexem::SpecialWords, now_line_number));
                }
                else {
                    tockens.push_back(Token(str, type_of_lexem::ID, now_line_number));
                }
                break;
            case Number:
                tockens.push_back(Token(str, type_of_lexem::Number, now_line_number));
                break;
            case FractionalNumber2:
                if (str[str.size() - 1] == '.') {
                    std::cout << "Некорректная запись дробного числа в строке " << now_line_number;
                    return 0;
                }
                tockens.push_back(Token(str, type_of_lexem::Number, now_line_number));
                break;
            case Error2:
                tockens.push_back(Token(str, type_of_lexem::Error, now_line_number));
                break;
            case QuotationMarks2:
                if (str[str.size() - 1] != '"') {
                    std::cout << "Некорректная запись строчного литерала в строке " << now_line_number;
                    return 0;
                }
                tockens.push_back(Token(str, type_of_lexem::String, now_line_number));
                break;
            }
        }
    }
    catch (const WrongChar_arith& ex) {
        std::cout << "В строке '" << ex.num << "' обнаружено некорректное использование типа 'nochar'!";
        return 0;
    }

    Parser parser;
    parser.analyze();
    std::cout << std::endl << std::endl;
    std::cout << "КОНСОЛЬ:\n";
    try {
        Interpreter();
    }
    catch (const  Wrong_ArrayIndexOut& ex) {
        std::cout << "Обнаружен выход за границы массива '" << ex.now << "'!";
        return 0;
    }
    return 0;
}
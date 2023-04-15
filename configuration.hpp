#ifndef __CONFIGURATION_HPP__
#define __CONFIGURATION_HPP__

#include <unordered_map>
#include <string>
#include <functional>
#include <vector>
#include <algorithm>
#include <iostream>
#include <fstream>

using namespace std;

#include "requirements.hpp"
#include "pipeline.hpp"

struct Configuration
{
    vector<vector<int>> parametrs;
    std:: unordered_map<string,vector<int>> indices;
    std:: unordered_map<string,vector<string>> stage_names;
    struct Pipeline *pipeline;

    Configuration(int question){
        switch (question)
        {
        case 1:
            stage_names={{"add", { "IF", "ID", "EX", "ME", "WB" }}, {"and", { "IF", "ID", "EX", "ME", "WB" }}, {"or", { "IF", "ID", "EX", "ME", "WB" }}, {"sub", { "IF", "ID", "EX", "ME", "WB" }}, {"mul", { "IF", "ID", "EX", "ME", "WB" }}, {"beq", { "IF", "ID", "EX", "ME", "WB" }}, {"bne", { "IF", "ID", "EX", "ME", "WB" }}, {"slt", { "IF", "ID", "EX", "ME", "WB" }}, {"j", { "IF", "ID", "EX", "ME", "WB" }}, {"lw", { "IF", "ID", "EX", "ME", "WB" }}, {"sw", { "IF", "ID", "EX", "ME", "WB" }}, {"addi", { "IF", "ID", "EX", "ME", "WB" }}, {"sll", { "IF", "ID", "EX", "ME", "WB" }}, {"srl", { "IF", "ID", "EX", "ME", "WB" }}};
            parametrs={{1,20},{70,100,150,200,120}};
            indices = {{"add", {2,2,2,4}}, {"and", {2,2,2,4}}, {"or", {2,2,2,4}}, {"sub", {2,2,2,4}}, {"mul", {2,2,2,4}}, {"beq", {2,2,2,-1}}, {"bne", {2,2,2,-1}}, {"slt", {2,2,2,4}}, {"j", {-1,-1,1,-1}}, {"lw", {2,-1,3,4}}, {"sw", {2,2,3,-1}}, {"addi", {2,-1,2,4}}, {"sll", {2,-1,2,4}}, {"srl", {2,-1,2,4}}};
            pipeline=new Pipeline(5,false,true,32,0,{"IF","ID","EX","ME","WB"},true);
            break;
        case 2:
	        stage_names={{"add", { "IF", "ID", "EX", "ME", "WB" }}, {"and", { "IF", "ID", "EX", "ME", "WB" }}, {"or", { "IF", "ID", "EX", "ME", "WB" }}, {"sub", { "IF", "ID", "EX", "ME", "WB" }}, {"mul", { "IF", "ID", "EX", "ME", "WB" }}, {"beq", { "IF", "ID", "EX", "ME", "WB" }}, {"bne", { "IF", "ID", "EX", "ME", "WB" }}, {"slt", { "IF", "ID", "EX", "ME", "WB" }}, {"j", { "IF", "ID", "EX", "ME", "WB" }}, {"lw", { "IF", "ID", "EX", "ME", "WB" }}, {"sw", { "IF", "ID", "EX", "ME", "WB" }}, {"addi", { "IF", "ID", "EX", "ME", "WB" }}, {"sll", { "IF", "ID", "EX", "ME", "WB" }}, {"srl", { "IF", "ID", "EX", "ME", "WB" }}};
            parametrs={{1,20},{70,100,150,200,120}};
            indices = {{"add", {2,2,2,4}}, {"and", {2,2,2,4}}, {"or", {2,2,2,4}}, {"sub", {2,2,2,4}}, {"mul", {2,2,2,4}}, {"beq", {2,2,2,-1}}, {"bne", {2,2,2,-1}}, {"slt", {2,2,2,4}}, {"j", {-1,-1,1,-1}}, {"lw", {2,-1,3,4}}, {"sw", {3,2,3,-1}}, {"addi", {2,-1,2,4}}, {"sll", {2,-1,2,4}}, {"srl", {2,-1,2,4}}};
            pipeline=new Pipeline(5,true,true,32,0,{"IF","ID","EX","ME","WB"},true);
            break;
        case 3:
	        stage_names={{"add", { "IF1","IF2", "ID1","ID2","RR", "EX","WB" }}, {"and", { "IF1","IF2", "ID1","ID2","RR", "EX","WB" }}, {"or", { "IF1","IF2", "ID1","ID2","RR", "EX","WB" }}, {"sub",{ "IF1","IF2", "ID1","ID2","RR", "EX","WB" }}, {"mul", { "IF1","IF2", "ID1","ID2","RR", "EX","WB" }}, {"beq", { "IF1","IF2", "ID1","ID2","RR", "EX","WB" }}, {"bne", { "IF1","IF2", "ID1","ID2","RR", "EX","WB" }}, {"slt", { "IF1","IF2", "ID1","ID2","RR", "EX","WB" }}, {"j", { "IF1","IF2", "ID1","ID2","RR", "EX","WB" }}, {"lw", { "IF1","IF2", "ID1","ID2","RR", "EX","MEM1","MEM2","WB" }}, {"sw", { "IF1","IF2", "ID1","ID2","RR", "EX","MEM1","MEM2","WB" }}, {"addi", { "IF1","IF2", "ID1","ID2","RR", "EX","WB" }}, {"sll", { "IF1","IF2", "ID1","ID2","RR", "EX","WB" }}, {"srl", { "IF1","IF2", "ID1","ID2","RR", "EX","WB" }}};
            parametrs={{1,20},{35,35,33,33,33,150,100,100,120}};
            indices = {{"add", {4,4,5,6}}, {"and", {4,4,5,6}}, {"or", {4,4,5,6}}, {"sub", {4,4,5,6}}, {"mul", {4,4,5,6}}, {"beq", {4,4,5,-1}}, {"bne", {4,4,5,-1}}, {"slt", {4,4,5,6}}, {"j", {-1,-1,3,-1}}, {"lw", {4,-1,7,8}}, {"sw", {4,4,7,-1}}, {"addi", {4,-1,5,6}}, {"sll", {4,-1,5,6}}, {"srl", {4,-1,5,6}}};
            pipeline=new Pipeline(9,false,true,32,0,{"IF1","IF2","ID1","ID2","RR","EX","MEM1","MEM2","WB"},true);
            break;
        case 4:
	        stage_names={{"add", { "IF1","IF2", "ID1","ID2","RR", "EX","WB" }}, {"and", { "IF1","IF2", "ID1","ID2","RR", "EX","WB" }}, {"or", { "IF1","IF2", "ID1","ID2","RR", "EX","WB" }}, {"sub",{ "IF1","IF2", "ID1","ID2","RR", "EX","WB" }}, {"mul", { "IF1","IF2", "ID1","ID2","RR", "EX","WB" }}, {"beq", { "IF1","IF2", "ID1","ID2","RR", "EX","WB" }}, {"bne", { "IF1","IF2", "ID1","ID2","RR", "EX","WB" }}, {"slt", { "IF1","IF2", "ID1","ID2","RR", "EX","WB" }}, {"j", { "IF1","IF2", "ID1","ID2","RR", "EX","WB" }}, {"lw", { "IF1","IF2", "ID1","ID2","RR", "EX","MEM1","MEM2","WB" }}, {"sw", { "IF1","IF2", "ID1","ID2","RR", "EX","MEM1","MEM2","WB" }}, {"addi", { "IF1","IF2", "ID1","ID2","RR", "EX","WB" }}, {"sll", { "IF1","IF2", "ID1","ID2","RR", "EX","WB" }}, {"srl", { "IF1","IF2", "ID1","ID2","RR", "EX","WB" }}};
            parametrs={{1,20},{35,35,33,33,33,150,100,100,120}};
            indices = {{"add", {5,5,5,6}}, {"and", {5,5,5,6}}, {"or", {5,5,5,6}}, {"sub", {5,5,5,6}}, {"mul", {5,5,5,6}}, {"beq", {5,5,5,-1}}, {"bne", {5,5,5,-1}}, {"slt", {5,5,5,6}}, {"j", {-1,-1,3,-1}}, {"lw", {5,-1,7,8}}, {"sw", {6,5,7,-1}}, {"addi", {5,-1,5,6}}, {"sll", {5,-1,5,6}}, {"srl", {5,-1,5,6}}};
            pipeline=new Pipeline(9,true,true,32,0,{"IF1","IF2","ID1","ID2","RR","EX","MEM1","MEM2","WB"},true);
            break;
        default:
            std:: cerr << "Invalid question number" << std::endl;
            break;
        }
    }
};

#endif
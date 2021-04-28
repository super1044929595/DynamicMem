#include "mainwindow.h"

#include <QApplication>

#include <QString>
#include <QDebug>
#include <string.h>
#include <stdio.h>
#include <algorithm>
#include <stdio.h>
#include <vector>
#include <string>
#include <map>
#include <algorithm>
using namespace std;

struct HuffmanNode {
    HuffmanNode(char k, int w) : key(k), weight(w) ,
                                 left(nullptr),
                                 right(nullptr)
    {}
    HuffmanNode(int w) : key('\0'), weight(w) ,
                                 left(nullptr),
                                 right(nullptr)
    {}
    char key;
    int weight;
    HuffmanNode * left;
    HuffmanNode * right;
};

class ComHuffmanNode {
public:
    bool operator()(HuffmanNode * e1, HuffmanNode * e2) {
        return e1->weight > e2->weight;
    }
};

class HuffmanTree {
public:
    HuffmanTree() {
        DecodeTree = nullptr;
    }
    ~HuffmanTree() {
        ClearDecodeTree();
    }

    void Input(const map<char, int> & mapCh) {
        vector<HuffmanNode*> vecHufNode;
        for (auto itr=mapCh.begin(); itr!=mapCh.end(); ++itr) {
            vecHufNode.push_back(new HuffmanNode(itr->first, itr->second));
        }

        make_heap(vecHufNode.begin(), vecHufNode.end(), ComHuffmanNode());

        while (vecHufNode.size() > 1) {
            HuffmanNode * right = vecHufNode.front();
            pop_heap(vecHufNode.begin(), vecHufNode.end(), ComHuffmanNode());
            vecHufNode.pop_back();

            HuffmanNode * left = vecHufNode.front();
            pop_heap(vecHufNode.begin(), vecHufNode.end(), ComHuffmanNode());
            vecHufNode.pop_back();

            HuffmanNode * parent = new HuffmanNode(left->weight + right->weight);
            parent->left = left;
            parent->right = right;

            vecHufNode.push_back(parent);
            push_heap(vecHufNode.begin(), vecHufNode.end(), ComHuffmanNode());
        }

        if (!vecHufNode.empty()) {
            DecodeTree = vecHufNode.front();
        }

        veccode.resize(std::numeric_limits<char>().max());

        string code;

        BuildCode(DecodeTree, code);

    }

    void ClearDecodeTree(HuffmanNode * pNode) {
        if (pNode == nullptr) return;

        ClearDecodeTree(pNode->left);
        ClearDecodeTree(pNode->right);
        delete pNode;
    }

    void ClearDecodeTree() {
        ClearDecodeTree(DecodeTree);
        DecodeTree = nullptr;
    }

    void BuildCode(HuffmanNode * pNode, string & code) {
        if (pNode->left == NULL) {
            veccode[pNode->key] = code;
            return ;
        }

        code.push_back('0');
        BuildCode(pNode->left, code);
        code.pop_back();
        code.push_back('1');
        BuildCode(pNode->right, code);
        code.pop_back();
    }

    string Decode(const std::string & strB) {
        string strC;

        HuffmanNode * pNode = DecodeTree;
        for (int i=0; i<strB.size(); ++i) {
            if (strB[i] == '0') {
                pNode = pNode->left;
            } else {
                pNode = pNode->right;
            }

            if (pNode->left == NULL) {
                strC.push_back(pNode->key);
                pNode = DecodeTree;
            }
        }

        return strC;
    }

    string GetCode(const std::string & strA) {
        string strB;
        for (int i=0; i<strA.size(); ++i) {
            strB += veccode[strA[i]];
        }

        return strB;
    }


private:
    HuffmanNode * DecodeTree;
    vector<string> veccode;
};




class HufumanEncode{


};


typedef struct {
    int wight;
    char ch;

}huffmanInfo;

void HufumanEncode_Encode(const char * str)
{
    if(str==nullptr) return ;
    char strarr[52];
    char temp=0;

    huffmanInfo huffinfo[30];//=new huffmanInfo[strlen(str)];
    int len=strlen(str);
    memset((char*)strarr,0,52);
    for(int j=0;j<len;j++){
        strarr[str[j]-'a']++;
        huffinfo[j].ch=str[j];
        huffinfo[str[j]-'a'].wight=strarr[str[j]-'a'];
    }

    //find the min value
    for(int i=1;i<52;i++){
        if(strarr[0]<strarr[i]){

        }else{
            temp=strarr[0];
            strarr[0]=strarr[i];
            strarr[i]=temp;
        }
    }


    //delete huffinfo;
}




int main(int argc, char *argv[])
{
    QApplication a(argc, argv);

    //HufumanEncode_Encode("sabcsjashajsa  sasas");

    map<char, int> mapCh = {
            {'a', 50},
            {'b', 50},
            {'c', 40},
            {'d', 10},
            {'e', 50}
    };

    HuffmanTree huffTree;

    huffTree.Input(mapCh);

    string strA = "abmba";

    puts(strA.c_str());

    string strB = huffTree.GetCode(strA);

    puts(strB.c_str());

    printf("%d, %d\n", strA.size() * 8, strB.size());

    string strC = huffTree.Decode(strB);

    puts(strC.c_str());


    return a.exec();
}

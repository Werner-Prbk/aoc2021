#include <iostream>
#include <vector>
#include <algorithm>
#include <map>
#include <locale>
#include <memory>
#include "../common/aoc.h"

using namespace std;
using namespace aoc;

struct Node {
    bool isStart {false};
    bool isEnd {false};
    bool isSmallCave {false};
    vector<shared_ptr<Node>> connectedNodes {}; 
    string nodeName {};
    int visitCounter {0};

    bool CanBeVisited() {
        if (isSmallCave && visitCounter < (1 + *bonus)) return true;
        return !isSmallCave && !isStart;
    }

    bool Visit() {
        visitCounter++;

        if (isSmallCave && (visitCounter > 1) && (*bonus == 1)) {
            (*bonus) = 0;
            bonusTaken = true;
        }

        return isEnd;
    }

    void Reset() {
        visitCounter = 0;
    }

    bool bonusTaken {false};
    int* bonus;
    void SetBonus(int& val) {
        bonus  = &val;
    }

    static Node CreateNode(string const& n) {
        Node node;
        if (n == "start") {
            node.isStart = true;
        } else if (n == "end") {
            node.isEnd = true;
        }
        else if (std::islower(n[0])) {
            node.isSmallCave = true;
        }
        node.nodeName = n;
        return node;
    }

    void UpdateEdges(shared_ptr<Node> from, shared_ptr<Node> to) {
        //cout << "'" << nodeName << "' " << "'" << from.nodeName << "' " << to.nodeName << "'" << endl;  

        if (from->nodeName == nodeName) {
            cout << "connect:" << nodeName << " with " << to->nodeName << endl;
            connectedNodes.push_back(to);
            cout << "XXXX" <<(*connectedNodes.rbegin())->nodeName << endl;
        } else if (to->nodeName == nodeName) {
            cout << "connect:" << nodeName << " with " << from->nodeName << endl;
            connectedNodes.push_back(from);
        } else
        {
            cout << "no connection:" << nodeName << " with " << from->nodeName << " and " << to->nodeName << endl; 
        }
    }
};

void FindPath(shared_ptr<Node> n, string s, int* bonus) {
    if (n->CanBeVisited()) {
        s += "," + n->nodeName;
        if (!n->Visit()) {
            for_each(n->connectedNodes.begin(), n->connectedNodes.end(), [&s, bonus](auto const& nd) {
                FindPath(nd, s, bonus);                
                });
            n->Reset();    
            // give it back
                    if (n->bonusTaken)
                    {
                        *bonus = 1;
                        n->bonusTaken = false;
                    } 
        }
        else {
            cout << s << endl;
            
        }
    }
    
}

vector<shared_ptr<Node>>::iterator GetNodeByName(vector<shared_ptr<Node>>& nodes, string const& name)
{
    return find_if(nodes.begin(), nodes.end(), [&name](auto const& node) {return name == node->nodeName;});
}

int main() {
    vector<shared_ptr<Node>> nodes;
    readInput("test_0.txt", [&nodes](auto const& line) {
        auto path = splitString<string>(line, '-');        
        for_each(path.begin(), path.end(), [&nodes](auto const& n){
            if (GetNodeByName(nodes, n) == nodes.end()) {
                nodes.push_back(make_shared<Node>(Node::CreateNode(n)));
            }
        });

        for (auto &&n : nodes) {
            n->UpdateEdges(*GetNodeByName(nodes, path[0]), *GetNodeByName(nodes, path[1]));   
        }
    });   

    for (auto &&node : nodes)
    {
        cout << node->nodeName <<  "(" << node->connectedNodes.size() << ")" <<": ";
        for (auto &&c : node->connectedNodes)
        {
            cout << " '" << c->nodeName << "' ";
        }
        cout << endl;     
    }

    auto startNode = *GetNodeByName(nodes, "start");
    auto endNode = *GetNodeByName(nodes, "end");

    int sum = 0;
    for (auto &&s : startNode->connectedNodes)
    {
        int bonus = 0;

        for_each(nodes.begin(), nodes.end(), [&bonus](auto& node) {node->Reset(); node->SetBonus(bonus);});
        FindPath(s, "start", &bonus);
        sum += endNode->visitCounter;
        cout << endl;
    }
    
    cout << "Sum: " << sum << endl;

    return 0;
}
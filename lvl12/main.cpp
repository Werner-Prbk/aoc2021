#include <iostream>
#include <vector>
#include <algorithm>
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

    bool CanBeVisited() const {
        auto limit = 1;

        if (useBonusSystem && *bonus) {
            limit++;
        }

        if (isSmallCave && visitCounter < limit) return true;
        return !isSmallCave && !isStart;
    }

    bool Visit() {
        visitCounter++;

        if (isSmallCave && (visitCounter > 1)) {
            if (useBonusSystem && (*bonus)) {
                (*bonus) = false;
                bonusTaken = true;
            }
        }

        return isEnd;
    }

    bool Unvisit() {
        if(useBonusSystem && bonusTaken) {
            bonusTaken = false;
            *bonus = true;
        }

        visitCounter--;
        return visitCounter == 0;
    }

    void Reset() {
        visitCounter = 0;
    }

    void SetBonus(bool* val) {
        bonus  = val;
        useBonusSystem = true;
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
        if (from->nodeName == nodeName) {
            connectedNodes.push_back(to);
        } else if (to->nodeName == nodeName) {
            connectedNodes.push_back(from);
        }
    }

private:
    Node(){}

    bool useBonusSystem {false};
    bool bonusTaken {false};
    bool* bonus;
};

void FindPath(shared_ptr<Node> n) {
    if (n->CanBeVisited()) {
        if (!n->Visit()) {
            for_each(n->connectedNodes.begin(), n->connectedNodes.end(), [](auto const& nd) { FindPath(nd); });  
            n->Unvisit();
        }
    }
}

vector<shared_ptr<Node>>::iterator GetNodeByName(vector<shared_ptr<Node>>& nodes, string const& name) {
    return find_if(nodes.begin(), nodes.end(), [&name](auto const& node) {return name == node->nodeName;});
}

int main() {
    vector<shared_ptr<Node>> nodes;
    readInput("input.txt", [&nodes](auto const& line) {
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

    auto startNode = *GetNodeByName(nodes, "start");
    auto endNode = *GetNodeByName(nodes, "end");

    int sum = 0;
    for (auto &&s : startNode->connectedNodes)
    {
        // for part 2
        bool bonusSystem = true;

        for_each(nodes.begin(), nodes.end(), [&bonusSystem](auto& node) {node->Reset(); node->SetBonus(&bonusSystem);});
        FindPath(s);
        sum += endNode->visitCounter;
    }
    
    cout << "Sum: " << sum << endl;

    return 0;
}
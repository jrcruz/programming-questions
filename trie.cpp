#include <iostream>
#include <vector>
#include <memory>
#include <stack>
#include <deque>
#include <unordered_map>


// https://leetcode.com/problems/implement-trie-prefix-tree/


// We store a character per node, rather than per edge.
class Trie
{
    struct Node {
        std::unordered_map<char, std::unique_ptr<Node>> pointers;
    } roots;

public:
    Trie() = default;


    void insert(std::string str)
    {
        Node * ptr = &roots;
        for (char c : str) {
            ptr->pointers.try_emplace(c, std::make_unique<Node>());
            ptr = ptr->pointers[c].get();
        }
    }


    bool search(const std::string& str) const
    {
        Node const * ptr = &roots;
        for (char c : str) {
            if (not ptr->pointers.contains(c)) {
                return false;
            }
            ptr = ptr->pointers.at(c).get();
        }
        return true;
    }


    bool startsWith(const std::string& s) const
    {
        return search(s);
    }


    friend std::ostream& operator<<(std::ostream& o, const Trie& trie)
    {
        std::deque<const Node*> stack{&(trie.roots)};
        do {
            const Node* n = stack.front();
            stack.pop_front();
            if (not n->pointers.empty()) {
                o << "Printing for node" << n << '\n';
            }
            for (auto& [stored_char, node_ptr] : n->pointers) {
                o << stored_char << ": " << node_ptr.get() << '\n';
                stack.push_back(node_ptr.get());
            }
        } while (not stack.empty());
        return o;
    }
};



int main()
{
    Trie t;
    t.insert("Hello");
    t.insert("Help");
    t.insert("Amiga");
    t.insert("Amoya");
    std::cout << t;
    std::cout << std::boolalpha << t.search("Hello") << " " << t.search("Hell") << "\n";
    std::cout << std::boolalpha << t.search("Ami") << " " << t.search("Amoya") << " " << t.search("Amoyaya") << " " << t.search("Anoya")<< "\n";
}

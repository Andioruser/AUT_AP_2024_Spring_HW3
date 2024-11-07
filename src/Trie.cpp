#include "Trie.h"
Trie::Node::Node(char data = '\0', bool is_finished = false)
    :data(data),is_finished(is_finished),parent(nullptr),children{}{
}
Trie::Node::~Node() {
    deleteNode(this);
}
Trie::Node& Trie::Node::operator=(const Node& other) {
    data = other.data;
    is_finished = other.is_finished;
}
void Trie::Node::deleteNode(Node* root) {
    for(auto &node : root->children) {
        if(node != nullptr)deleteNode(node);
    }    
    delete root;
    root = nullptr;
}
void Trie::Node::createNode(Node* root,Node * other_root) {
    std::size_t child_index = 0;
    for(const auto& node : other_root->children) {
        if(node != nullptr) {
           root->children[child_index] = new Node();
           root->children[child_index]->parent = root;
           root->children[child_index] = node;
           createNode(root->children[child_index],node);
        }
        child_index++;
    }
}
Trie::Trie():root(new Node()) {
}
Trie::Trie(const Trie& other):root(other.root) {
    if(other.root == nullptr) {
    root = nullptr;
    return;
    }
    root = new Node();
    root = other.root;
    root->createNode(root,other.root);
}
Trie::Trie(Trie&& other):root(std::move(other.root)) {
    other.root = nullptr;
}
Trie::Trie(std::initializer_list<std::string> list):root(new Node()) {
    for(const auto& str: list) {
        insert(str);
    }
}

//注意要递归delete
Trie::~Trie() {
    root->deleteNode(root);
}

Trie& Trie::operator=(const Trie& other) {
    root = other.root;
}
Trie& Trie::operator=(Trie&& other) {
    root = std::move(other.root);
}

void Trie::insert(const std::string& str) {
    int str_index = 0;
    for(const auto& child : root->children) {

    }
}
bool Trie::search(const std::string& query) const {

}
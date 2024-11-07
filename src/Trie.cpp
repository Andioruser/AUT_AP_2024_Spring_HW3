#include "Trie.h"
Trie::Node::Node(char data = '\0', bool is_finished = false)
    :data(data),is_finished(is_finished),parent(nullptr),children{}{
}
Trie::Node::~Node() {
    deleteNode(this);
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
           root->children[child_index]->children = node->children;
           root->children[child_index]->data = node->data;
           root->children[child_index]->is_finished = node->is_finished;
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
    Node * node = root;
    std::size_t str_index = 0;
    while(node != nullptr && str_index < str.size()) {
        bool is_found = false;
        for(const auto& child : node->children) {
            if(node->data == str[str_index]) {
                node = child;
                str_index++;
                is_found = true;
                break;
            }
        }
        if(!is_found) {
            Node* new_node = new Node();
            new_node->data = str[str_index++];
            new_node->is_finished = str_index == str.size() ? true : false;
            new_node->parent = node;
            node = new_node;
        }
    }
  
}
bool Trie::search(const std::string& query) const {
    int str_index = 0;
    Node * node = root;
    std::size_t str_index = 0;
    while(node != nullptr && str_index < query.size()) {
        bool is_searched = false;
        for(const auto& child : node->children) {
            if(node->data == query[str_index]) {
                node = child;
                str_index++;
                if( str_index == query.size() - 1 && node->is_finished ) {
                    return true;
                }
                is_searched = true;
                break;
            }
        }
        if(!is_searched && str_index < query.size() - 1) {
            return false;
        }
    }
    return true;
}

// Check if there is any word in the trie that starts with the given prefix
bool Trie::startsWith(const std::string& prefix) const {
    int str_index = 0;
    Node * node = root;
    std::size_t str_index = 0;
    while(node != nullptr && str_index < prefix.size()) {
        bool is_searched = false;
        for(const auto& child : node->children) {
            if(node->data == prefix[str_index]) {
                node = child;
                str_index++;
                is_searched = true;
                break;
            }
        }
        if(!is_searched && str_index < prefix.size() - 1) {
            return false;
        }
    }
    return true;
}

// Remove a word from the Trie, consider removing the trace if needed.
void Trie::remove(const std::string& str) {
    
}
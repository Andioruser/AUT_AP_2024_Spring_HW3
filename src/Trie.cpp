#include "Trie.h"

#include<queue>
#include<stack>
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
                if(str_index = str.size() && node->is_finished)return;
                break;
            }
        }
        if(!is_found) {
            //将该节点加入parent的children
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
   Node * leaf = findLeaf(str);
   if(leaf == nullptr) {
        return;
   }
   std::size_t child_num = 0;
   while(leaf->parent != nullptr) {
        for(auto& child : leaf->parent->children) {
            if(child == leaf) {
                child = nullptr;
            } 
            if(child != nullptr)child_num++;
        }
        if(child_num >= 1) {
            delete leaf;
            leaf = leaf->parent;
        }
   }
}
Trie::Node* Trie::findLeaf(const std::string &str) const {
    int str_index = 0;
    Trie::Node * node = root;
    std::size_t str_index = 0;
    while(node != nullptr && str_index < str.size()) {
        bool is_found = false;
        for(const auto& child : node->children) {
            if(node->data == str[str_index]) {
                node = child;
                str_index++;
                if( str_index == str.size() - 1 && node->is_finished ) {
                    return node;
                }
                break;
            }
        }
    }
    return nullptr;
}

// Traversal and Utility

// Breadth-first over the node and calling "func" function over each of them
void Trie::bfs(std::function<void(Node*&)> func) {
    std::queue<Node*>q;
    q.push(root);
    while(!q.empty()) {
        Node * node = q.front();
        q.pop();
        func(node);
        for(auto& child: node->children) {
            if(child != nullptr) {
                q.push(node);
            }
        }
    }
}


// 可以加一个visited布尔数组判断该Node节点是否被访问过。
// (BONUS), Depth-first over the node and calling "func" function over each of them
void Trie::dfs(std::function<void(Node*&)> func) {
    std::stack<Node*>stk;
    stk.push(root);
    while(!stk.empty()) {
        Node* node = stk.top();
        stk.pop();
        func(node);
        for(auto& child : node->children) {
            if(child != nullptr) {
                stk.push(child);
            }
        }
    }
}

void Trie::getAllWords(Node* node, std::string currentWord,std::vector<std::string>result) const{
    if(node->is_finished) {
        result.push_back(currentWord);
    }
    for(const auto& child : node->children) {
        getAllWords(child,currentWord + child->data, result);
    }
}

std::vector<std::string> Trie::getAllWords() const {
    std::vector<std::string> result;
    getAllWords(root, "", result);  
    return result;
}

// Output operator
std::ostream& operator<<(std::ostream& os, const Trie& trie) {
   std::vector<std::string>lwords  = trie.getAllWords();
   for(const auto&str : lwords) {
        os<<str<<",";
   }
   os<<std::endl;
}

// Input operator
std::istream& operator>>(std::istream& is, Trie& trie) {
    std::string str;
    is>>str;
    trie.insert(str);
}

// Creates a new Trie containing all unique words from both operands
Trie Trie::operator+(const Trie& other) const {
    std::vector<std::string>lwords  = this->getAllWords();
    std::vector<std::string>rwords = other.getAllWords();
    std::vector<std::string> result;

    std::sort(lwords.begin(), lwords.end());
    std::sort(rwords.begin(), rwords.end());

    std::set_union(lwords.begin(), lwords.end(), rwords.begin(), rwords.end(), std::back_inserter(result));

    Trie * trie = new Trie();

    for(const auto& str : result) {
        trie->insert(str);
    }
    return *trie;
}

// Adds all words from the right-hand operand into the left-hand Trie
Trie& Trie::operator+=(const Trie& other) {
    Trie trie = (*this) + other;
    return trie;
}

// Creates a new Trie containing words from the first Trie not in the second
Trie Trie::operator-(const Trie& other) const {
    std::vector<std::string>lwords  = this->getAllWords();
    std::vector<std::string>rwords = other.getAllWords();
    std::vector<std::string> result;

    std::sort(lwords.begin(), lwords.end());
    std::sort(rwords.begin(), rwords.end());

    // 计算差集
    std::set_difference(lwords.begin(), lwords.end(), rwords.begin(), rwords.end(),
                        std::back_inserter(result));

    Trie * trie = new Trie();

    for(const auto& str : result) {
        trie->insert(str);
    }
    return *trie;
}

// Removes words from the left-hand Trie found in the right-hand Trie
Trie& Trie::operator-=(const Trie& other) {
    Trie trie = (*this) - other;
    return trie;
}

// Can be used to check existence or perform other string operations
bool Trie::operator()(const std::string& query) const {
    return search(query);
} 

// Check if two Tries have exactly the same words
bool Trie::operator==(const Trie& other) const {
    std::vector<std::string>lwords  = this->getAllWords();
    std::vector<std::string>rwords = other.getAllWords();
    std::vector<std::string> result;

    std::sort(lwords.begin(), lwords.end());
    std::sort(rwords.begin(), rwords.end());
    if(lwords.size() != rwords.size()) {
        return false;
    }
    for(std::size_t i = 0; i < lwords.size(); ++i) {
        if(lwords[i] != rwords[i]) {
            return false;
        }
    }
    return true;
}

// Check if two Tries differ in any word
bool Trie::operator!=(const Trie& other) const {
    std::vector<std::string>lwords  = this->getAllWords();
    std::vector<std::string>rwords = other.getAllWords();
    std::vector<std::string> result;

    std::sort(lwords.begin(), lwords.end());
    std::sort(rwords.begin(), rwords.end());

    std::set_intersection(lwords.begin(), lwords.end(), rwords.begin(), rwords.end(), std::back_inserter(result));
    return result.size() == 0 ? true : false;
}
enum NodeType {
    PROGRAM,
    DECLARATION,
    VARIABLE_DECLARATION,
    STRUCT_DECLARATION,
    FUNCTION_DECLARATION,
    FUNCTION_PROTOTYPE,
    FUNCTION_DEFINITION,
    BLOCK,
    STATEMENT,
    CONDITIONAL,
    LOOP,
    RETURN,
    JUMP,
    EXPRESSION
};

class Node {
public:
      Node(NodeType type, std::string value = "", std::vector<std::unique_ptr<Node>> branch = {});
  ~Node();

private:
    template <typename... Args>
    void emplace_back(Args&&... args) {
        Derive.emplace_back(std::forward<Args>(args)...);
    }

    NodeType type;
    std::string value;
    std::vector<Node*> Derive;




    // методы для доступа к типу, значению и дочерним узлам
    NodeType getType() const;
    const std::string& getValue() const;
    const std::vector<std::unique_ptr<Node>>& get_branch() const;

    // метод для добавления дочернего узла
    void emplace_back(std::unique_ptr<Node> child);

private:
    NodeType type;
    std::string value;
    std::vector<std::unique_ptr<Node>> branch;
};

Node::Node(NodeType type, std::string value, std::vector<std::unique_ptr<Node>> branch)
    : type(type), value(std::move(value)), branch(std::move(branch)) {}

Node::NodeType Node::getType() const {
    return type;
}

const std::string& Node::getValue() const {
    return value;
}

const std::vector<std::unique_ptr<Node>>& Node::get_branch() const {
    return branch;
}

void Node::emplace_back(std::unique_ptr<Node> branch) {
    branch.emplace_back(std::move(child));
}

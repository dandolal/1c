#include <iostream>
#include <unordered_map>
#include <vector>
#include <string>

struct Node {
  Node(size_t id, int parent) : id(id), parent(parent){}
  size_t max_number_of_words = 0;
  size_t number_of_words = 0;
  bool is_leaf = false;
  std::unordered_map<char, size_t> children;
  int parent;
  size_t id;
};

class Tree {
 public:
  Tree() {
    nodes.emplace_back(0, -1);
  };

  void AddWord(const std::string &word) {
    size_t cur_node_id = 0;
    for (size_t i = 0; i < word.size(); ++i) {
      Node& cur_node = nodes[cur_node_id];
      if (cur_node.children.count(word[i]) == 0) {
        cur_node.children.insert({word[i], nodes.size()});
        nodes.emplace_back(nodes.size(), cur_node.id);
        cur_node_id = nodes.size() - 1;
      } else {
        cur_node_id = cur_node.children[word[i]];
      }
      if (i == word.size() - 1) {
        Node& cur_node = nodes[cur_node_id];
        cur_node.is_leaf = true;
        ++cur_node.number_of_words;
        cur_node.max_number_of_words = std::max(cur_node.number_of_words, cur_node.max_number_of_words);
      }
    }
    while (nodes[cur_node_id].parent != -1) {
      Node& cur_node = nodes[cur_node_id];
      nodes[cur_node.parent].max_number_of_words =
          std::max(nodes[cur_node.parent].max_number_of_words, cur_node.max_number_of_words);
      cur_node_id = cur_node.parent;
    }
  }

  bool Find(const std::string& word, std::string& ans, size_t start_node = 0) {
    if (start_node == 0) {
      cur_ans = "";
      cur_node_query = 0;
    }
    cur_ans += word;
    Node cur_node = nodes[start_node];
    for (char ch: word) {
      if (cur_node.children.count(ch) == 0) {
        return false;
      } else {
        cur_node = nodes[cur_node.children[ch]];
        cur_node_query = cur_node.id;
      }
    }
    while (cur_node.max_number_of_words != cur_node.number_of_words) {
      for (auto pair: cur_node.children) {
        if (nodes[pair.second].max_number_of_words == cur_node.max_number_of_words) {
          ans += pair.first;
          cur_node = nodes[pair.second];
          break;
        }
      }
    }
    ans = cur_ans + ans;
    return true;
  }

  bool NewFind(const std::string& str, std::string& ans) {
    bool result = Find(str, ans, cur_node_query);
    return result;
  }
  std::string cur_ans;
  size_t cur_node_query = 0;
  std::vector<Node> nodes;
};

std::vector<std::string> split(const std::string& str) {
  std::vector<std::string> ans;
  std::string cur;
  for (char ch: str) {
    if (ch == ' ') {
      ans.push_back(cur);
      cur = "";
    } else {
      cur += ch;
    }
  }
  ans.push_back(cur);
  return ans;
}

int main() {
  std::cout << "Чтобы ввести слова, напишите: Добавить текст: \"\"\n";
  std::cout << "Чтобы найти слово, напишите: Запрос: \"\"\n";
  std::cout << "Чтобы дописать слово, напишите: Дописать: \"\"\n";
  std::cout << "Чтобы завершить программу, напишите: Заверишть\n";
  std::string str;
  Tree tree;
  while (std::getline(std::cin, str)) {
    if (str == "Завершить")
      return 0;
    if (str.find(':') == std::string::npos) {
      std::cout << "Извините, такой команды нет\n";
      continue;
    }
    size_t end_of_command = str.find(':');
    std::string command;
    for (size_t i = 0; i < end_of_command; ++i) {
      command += str[i];
    }
    if (str.find('\"') == std::string::npos) {
      std::cout << "Извините, такой команды нет\n";
      continue;
    }
    size_t i = str.find('\"') + 1;
    std::string text;
    while (str[i] != '\"' && i < str.size()) {
      text += str[i];
      ++i;
    }
    if (i == str.size()) {
      std::cout << "Извините, такой команды нет\n";
      continue;
    }
    if (command == "Добавить текст") {
      auto words = split(text);
      for (const auto& word: words) {
        tree.AddWord(word);
      }
    } else if (command == "Дописать") {
      std::string ans;
      if (tree.NewFind(text, ans)) {
        std::cout << "Подсказка: " << ans << '\n';
      } else {
        std::cout << "Извините, подходящего слова нет\n";
      }
    } else if (command == "Запрос"){
      std::string ans;
      if (tree.Find(text, ans)) {
        std::cout << "Подсказка: " << ans << '\n';
      } else {
        std::cout << "Извините, подходящего слова нет\n";
      }
    } else {
      std::cout << "Извините, такой команды нет\n";
    }
  }
}
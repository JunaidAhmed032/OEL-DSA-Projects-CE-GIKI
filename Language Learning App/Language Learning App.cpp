#include <iostream>
#include <string>
#include <vector>
#include <queue>
#include <algorithm>
#include <unordered_map>
#include <set>

using namespace std;

const int MAX_NODES = 100;

// ------------------- Stack Module -------------------
struct Stack {
    vector<string> data;

    void push(const string &lesson) { data.push_back(lesson); }

    void pop() {
        if (!data.empty()) data.pop_back();
    }

    string peek() const {
        return data.empty() ? "" : data.back();
    }

    bool isEmpty() const { return data.empty(); }
};

// ------------------- Queue Module -------------------
struct Queue {
    queue<string> data;

    void enqueue(const string &lesson) { data.push(lesson); }

    void dequeue() {
        if (!data.empty()) data.pop();
    }

    string peek() const {
        return data.empty() ? "" : data.front(); }

    bool isEmpty() const { return data.empty(); }
};

// ------------------- Priority Queue Module -------------------
struct PriorityQueue {
    priority_queue<pair<int, string>, vector<pair<int, string>>, greater<>> pq;

    void enqueue(const string &lesson, int priority) { pq.push({priority, lesson}); }

    void dequeue() {
        if (!pq.empty()) pq.pop(); }

    string peek() const {
        return pq.empty() ? "" : pq.top().second; }

    bool isEmpty() const { return pq.empty(); }
};

// ------------------- Map Module -------------------
struct Vocabulary {
    unordered_map<string, string> vocab;

    void addWord(const string &key, const string &value) {
        vocab[key] = value;
    }

    void removeWord(const string &key) {
        vocab.erase(key);
    }

    string getWord(const string &key) const {
        auto it = vocab.find(key);
        return it != vocab.end() ? it->second : "";
    }

    void print() const {
        if (vocab.empty()) {
            cout << "Vocabulary is empty!\n";
        } else {
            cout << "\n--- Vocabulary List ---\n";
            for (const auto &pair : vocab) {
                cout << pair.first << " - " << pair.second << "\n";
            }
        }
    }
};

// ------------------- Lesson Tree Module -------------------
struct TreeNode {
    string lessonName;
    vector<int> children;
};

struct LessonTree {
    vector<TreeNode> nodes;

    void addLesson(const string &parentLesson, const string &newLesson) {
        int parentIndex = -1;
        for (size_t i = 0; i < nodes.size(); i++) {
            if (nodes[i].lessonName == parentLesson) {
                parentIndex = i;
                break;
            }
        }
        TreeNode newNode = {newLesson, {}};
        nodes.push_back(newNode);

        if (parentIndex != -1) {
            nodes[parentIndex].children.push_back(nodes.size() - 1);
        }
        cout << "Added lesson: " << newLesson << "\n";
    }

    void displayLessons(int index = 0, int level = 0) const {
        if (index >= static_cast<int>(nodes.size())) return;
        for (int i = 0; i < level; i++) cout << "  ";
        cout << "- " << nodes[index].lessonName << "\n";
        for (int child : nodes[index].children) {
            displayLessons(child, level + 1);
        }
    }
};

// ------------------- Dynamic Programming Module -------------------
struct LearningPath {
    vector<string> path;

    void generatePath(const LessonTree &tree, const Vocabulary &scores) {
        path.clear();
        set<string> visited;
        string lessonName;
        cout << "Enter the name of the lesson you want to add to the path: ";
        cin >> lessonName;

        bool found = false;
        for (const auto &node : tree.nodes) {
            if (node.lessonName == lessonName) {
                found = true;
                dfs(tree, node.lessonName, scores, visited);
                break;
            }
        }

        if (!found) {
            cout << "Lesson not found in the tree.\n";
        }
    }

    void dfs(const LessonTree &tree, const string &current, const Vocabulary &scores, set<string> &visited) {
        if (visited.count(current)) return;
        visited.insert(current);
        path.push_back(current);

        for (const auto &node : tree.nodes) {
            if (node.lessonName == current) {
                for (int childIndex : node.children) {
                    dfs(tree, tree.nodes[childIndex].lessonName, scores, visited);
                }
            }
        }
    }

    void printPath() const {
        if (path.empty()) {
            cout << "No lessons in the learning path!\n";
        } else {
            cout << "\n--- Learning Path ---\n";
            for (size_t i = 0; i < path.size(); i++) {
                cout << (i + 1) << ". " << path[i] << "\n";
            }
        }
    }
};

// ------------------- Quiz Module -------------------
void generateQuizzes(const LearningPath &lp, const Vocabulary &vocab, Vocabulary &scores) {
    if (lp.path.empty()) {
        cout << "No lessons available for the quiz. Generate a learning path first.\n";
        return;
    }

    int totalQuestions = 0, correctAnswers = 0;

    for (const auto &lesson : lp.path) {
        cout << "\n--- Quiz for Vocabulary ---\n";
        for (const auto &vocabEntry : vocab.vocab) {
            if (lesson == vocabEntry.first) {
                totalQuestions++;
                cout << "Translate this word: " << vocabEntry.first << "\nYour Answer: ";
                string userAnswer;
                cin >> userAnswer;

                if (userAnswer == vocabEntry.second) {
                    cout << "Correct!\n";
                    correctAnswers++;
                    scores.addWord(vocabEntry.first, "high");
                } else {
                    cout << "Incorrect! Correct answer is: " << vocabEntry.second << "\n";
                    scores.addWord(vocabEntry.first, "low");
                }
            }
        }
    }

    cout << "\n--- Quiz Results ---\n";
    cout << "Total Questions: " << totalQuestions << "\n";
    cout << "Correct Answers: " << correctAnswers << "\n";
    cout << "Score: " << (totalQuestions > 0 ? (static_cast<double>(correctAnswers) / totalQuestions) * 100 : 0) << "%\n";
}

// ------------------- Main Program -------------------
int main() {
    Vocabulary vocabulary;
    LessonTree lessonTree;
    LearningPath learningPath;
    Vocabulary scores;
    Stack completedLessons;
    Queue pendingLessons;
    PriorityQueue priorityLessons;

    bool running = true;
    while (running) {
        cout << "\n1. Add Vocabulary\n2. Remove Vocabulary\n3. View Vocabulary\n4. Add Lesson\n5. View Lessons\n6. Generate Learning Path\n7. Start Quiz\n8. Exit\nChoice: ";
        int choice;
        cin >> choice;
        cin.ignore();

        switch (choice) {
            case 1: {
                string word, translation;
                cout << "Enter word: ";
                cin >> word;
                cout << "Enter translation: ";
                cin >> translation;
                vocabulary.addWord(word, translation);
                break;
            }
            case 2: {
                string word;
                cout << "Enter word to remove: ";
                cin >> word;
                vocabulary.removeWord(word);
                break;
            }
            case 3:
                vocabulary.print();
                break;
            case 4: {
                string parentLesson, newLesson;
                cout << "Enter parent lesson (or 'root' for top level): ";
                cin >> parentLesson;
                cout << "Enter new lesson name: ";
                cin >> newLesson;
                lessonTree.addLesson(parentLesson, newLesson);
                break;
            }
            case 5:
                lessonTree.displayLessons();
                break;
            case 6:
                learningPath.generatePath(lessonTree, scores);
                learningPath.printPath();
                break;
            case 7:
                generateQuizzes(learningPath, vocabulary, scores);
                break;
            case 8:
                running = false;
                cout << "Thank you for using the app!\n";
                break;
            default:
                cout << "Invalid choice. Try again.\n";
        }
    }
    return 0;
}
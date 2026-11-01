#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <chrono>
#include <cstdlib>
using namespace std;

//=====================================================

class Airport {
public:
    string id;
    string name;
    string city;
    string country;
    string iata;
    string icao;
    double latitude;
    double longitude;

    Airport() {
        latitude = 0.0;
        longitude = 0.0;
    }
};

//=====================================================

vector<string> splitLine(string line) {
    vector<string> fields;
    string field = "";
    bool insideQuotes = false;

    for (int i = 0; i < (int)line.length(); i++) {
        char c = line[i];
        if (c == '"') {
            insideQuotes = !insideQuotes;
        } else if (c == ',' && insideQuotes == false) {
            fields.push_back(field);
            field = "";
        } else {
            field = field + c;
        }
    }
    fields.push_back(field);

    return fields;
}

//=====================================================

bool readFile(string fileName, vector<string> &lines) {
    ifstream file(fileName);
    if (!file) {
        return false;
    }

    string line;
    while (getline(file, line)) {
        lines.push_back(line);
    }
    file.close();

    return true;
}

//=====================================================

void loadAirports(vector<string> &lines, vector<Airport> &airports, int &skipped) {
    skipped = 0;

    for (int i = 0; i < (int)lines.size(); i++) {
        vector<string> fields = splitLine(lines[i]);

        if ((int)fields.size() < 8) {
            continue;
        }

        if (fields[4] == "\\N") {
            skipped = skipped + 1;
            continue;
        }

        Airport a;
        a.id = fields[0];
        a.name = fields[1];
        a.city = fields[2];
        a.country = fields[3];
        a.iata = fields[4];
        a.icao = fields[5];
        a.latitude = stod(fields[6]);
        a.longitude = stod(fields[7]);

        airports.push_back(a);
    }
}

//=====================================================

class AirportBST {
private:
    struct Node {
        Airport data;
        Node* left;
        Node* right;

        Node(Airport a) {
            data = a;
            left = nullptr;
            right = nullptr;
        }
    };

    Node* root;

    void inorder(Node* node, vector<Airport> &result) {
        if (node == nullptr) {
            return;
        }
        inorder(node->left, result);
        result.push_back(node->data);
        inorder(node->right, result);
    }

    void preorder(Node* node, ofstream &file) {
        if (node == nullptr) {
            return;
        }

        file << node->data.id << ",\""
             << node->data.name << "\",\""
             << node->data.city << "\",\""
             << node->data.country << "\",\""
             << node->data.iata << "\",\""
             << node->data.icao << "\","
             << node->data.latitude << ","
             << node->data.longitude << endl;

        preorder(node->left, file);
        preorder(node->right, file);
    }

    bool compareTrees(Node* first, Node* second) {
        if (first == nullptr && second == nullptr) {
            return true;
        }
        if (first == nullptr || second == nullptr) {
            return false;
        }
        if (first->data.iata != second->data.iata) {
            return false;
        }

        bool leftSame = compareTrees(first->left, second->left);
        bool rightSame = compareTrees(first->right, second->right);

        return leftSame && rightSame;
    }

    bool validate(Node* node, string lo, string hi) {
        if (node == nullptr) {
            return true;
        }
        if (node->data.iata <= lo || node->data.iata >= hi) {
            return false;
        }
        if (validate(node->left, lo, node->data.iata) == false) {
            return false;
        }
        if (validate(node->right, node->data.iata, hi) == false) {
            return false;
        }
        return true;
    }

    void rangeSearch(Node* node, string from, string to, int &visited, int &found) {
        if (node == nullptr) {
            return;
        }

        visited = visited + 1;

        if (node->data.iata > from) {
            rangeSearch(node->left, from, to, visited, found);
        }

        if (node->data.iata >= from && node->data.iata <= to) {
            found = found + 1;
            cout << node->data.iata << " "
                 << node->data.name << ", "
                 << node->data.city << ", "
                 << node->data.country << endl;
        }

        if (node->data.iata < to) {
            rangeSearch(node->right, from, to, visited, found);
        }
    }

    Node* buildBalanced(vector<Airport> &sortedAirports, int start, int end) {
        if (start > end) {
            return nullptr;
        }

        int mid = (start + end) / 2;

        Node* node = new Node(sortedAirports[mid]);
        node->left = buildBalanced(sortedAirports, start, mid - 1);
        node->right = buildBalanced(sortedAirports, mid + 1, end);

        return node;
    }

    void findDeepest(Node* node, int depth, int &maxDepth, string &code) {
        if (node == nullptr) {
            return;
        }
        if (depth > maxDepth) {
            maxDepth = depth;
            code = node->data.iata;
        }
        findDeepest(node->left, depth + 1, maxDepth, code);
        findDeepest(node->right, depth + 1, maxDepth, code);
    }

    string findLeafCode(Node* node) {
        if (node == nullptr) {
            return "";
        }
        if (node->left == nullptr && node->right == nullptr) {
            return node->data.iata;
        }

        string code = findLeafCode(node->left);
        if (code != "") {
            return code;
        }
        return findLeafCode(node->right);
    }

    string findOnlyLeftChild(Node* node) {
        if (node == nullptr) {
            return "";
        }
        if (node->left != nullptr && node->right == nullptr) {
            return node->data.iata;
        }

        string code = findOnlyLeftChild(node->left);
        if (code != "") {
            return code;
        }
        return findOnlyLeftChild(node->right);
    }

    string findOnlyRightChild(Node* node) {
        if (node == nullptr) {
            return "";
        }
        if (node->left == nullptr && node->right != nullptr) {
            return node->data.iata;
        }

        string code = findOnlyRightChild(node->left);
        if (code != "") {
            return code;
        }
        return findOnlyRightChild(node->right);
    }

    string findTwoChildren(Node* node) {
        if (node == nullptr) {
            return "";
        }
        if (node != root && node->left != nullptr && node->right != nullptr) {
            return node->data.iata;
        }

        string code = findTwoChildren(node->left);
        if (code != "") {
            return code;
        }
        return findTwoChildren(node->right);
    }

    void calculateDepth(int &totalDepth, int &count) {
        totalDepth = 0;
        count = 0;
        if (root == nullptr) {
            return;
        }

        vector<Node*> nodeStack;
        vector<int> depthStack;
        nodeStack.push_back(root);
        depthStack.push_back(0);

        while ((int)nodeStack.size() > 0) {
            Node* current = nodeStack[nodeStack.size() - 1];
            int depth = depthStack[depthStack.size() - 1];
            nodeStack.pop_back();
            depthStack.pop_back();

            totalDepth = totalDepth + depth;
            count = count + 1;

            if (current->right != nullptr) {
                nodeStack.push_back(current->right);
                depthStack.push_back(depth + 1);
            }
            if (current->left != nullptr) {
                nodeStack.push_back(current->left);
                depthStack.push_back(depth + 1);
            }
        }
    }

public:
    AirportBST() {
        root = nullptr;
    }

    ~AirportBST() {
        clearTree();
    }

    void clearTree() {
        if (root == nullptr) {
            return;
        }

        vector<Node*> stack;
        stack.push_back(root);

        while ((int)stack.size() > 0) {
            Node* current = stack[stack.size() - 1];
            stack.pop_back();

            if (current->left != nullptr) {
                stack.push_back(current->left);
            }
            if (current->right != nullptr) {
                stack.push_back(current->right);
            }
            delete current;
        }

        root = nullptr;
    }

    bool insert(Airport a) {
        if (root == nullptr) {
            root = new Node(a);
            return true;
        }

        Node* current = root;

        while (true) {
            if (a.iata < current->data.iata) {
                if (current->left == nullptr) {
                    current->left = new Node(a);
                    return true;
                }
                current = current->left;
            } else if (a.iata > current->data.iata) {
                if (current->right == nullptr) {
                    current->right = new Node(a);
                    return true;
                }
                current = current->right;
            } else {
                return false;
            }
        }
    }

    void insertAll(vector<Airport> &airports, int &inserted, int &duplicates) {
        inserted = 0;
        duplicates = 0;

        for (int i = 0; i < (int)airports.size(); i++) {
            if (insert(airports[i])) {
                inserted = inserted + 1;
            } else {
                duplicates = duplicates + 1;
                cout << "Duplicate: " << airports[i].iata << " (rule I4, not inserted)" << endl;
            }
        }
    }

    int getHeight() {
        if (root == nullptr) {
            return -1;
        }

        vector<Node*> queue;
        queue.push_back(root);
        int index = 0;
        int height = -1;

        while (index < (int)queue.size()) {
            int levelSize = (int)queue.size() - index;

            for (int i = 0; i < levelSize; i++) {
                Node* current = queue[index];
                if (current->left != nullptr) {
                    queue.push_back(current->left);
                }
                if (current->right != nullptr) {
                    queue.push_back(current->right);
                }
                index = index + 1;
            }

            height = height + 1;
        }

        return height;
    }

    double getAverageDepth() {
        int totalDepth = 0;
        int count = 0;
        calculateDepth(totalDepth, count);

        if (count == 0) {
            return 0.0;
        }
        return (double)totalDepth / count;
    }

    int getCount() {
        int totalDepth = 0;
        int count = 0;
        calculateDepth(totalDepth, count);
        return count;
    }

    bool search(string code) {
        Node* current = root;
        int comparisons = 0;

        cout << "Path: ";

        while (current != nullptr) {
            comparisons = comparisons + 1;
            cout << current->data.iata;

            if (code == current->data.iata) {
                cout << endl;
                cout << "Comparisons: " << comparisons << endl;
                cout << current->data.iata << " "
                     << current->data.name << ", "
                     << current->data.city << ", "
                     << current->data.country << endl;
                return true;
            } else if (code < current->data.iata) {
                current = current->left;
            } else {
                current = current->right;
            }

            cout << " -> ";
        }

        cout << "NULL" << endl;
        cout << "Comparisons: " << comparisons << endl;
        cout << code << " not found" << endl;

        return false;
    }

    bool searchQuiet(string code) {
        Node* current = root;

        while (current != nullptr) {
            if (code == current->data.iata) {
                return true;
            } else if (code < current->data.iata) {
                current = current->left;
            } else {
                current = current->right;
            }
        }

        return false;
    }

    string findMin() {
        if (root == nullptr) {
            return "";
        }
        Node* current = root;
        while (current->left != nullptr) {
            current = current->left;
        }
        return current->data.iata;
    }

    string findMax() {
        if (root == nullptr) {
            return "";
        }
        Node* current = root;
        while (current->right != nullptr) {
            current = current->right;
        }
        return current->data.iata;
    }

    string getRootCode() {
        if (root == nullptr) {
            return "";
        }
        return root->data.iata;
    }

    string getDeepestCode() {
        int maxDepth = -1;
        string code = "";
        findDeepest(root, 0, maxDepth, code);
        return code;
    }

    string findLeafCode() {
        return findLeafCode(root);
    }

    string findOnlyLeftChild() {
        return findOnlyLeftChild(root);
    }

    string findOnlyRightChild() {
        return findOnlyRightChild(root);
    }

    string findTwoChildren() {
        return findTwoChildren(root);
    }

    bool remove(string code) {
        Node* current = root;
        Node* parent = nullptr;

        while (current != nullptr && current->data.iata != code) {
            parent = current;
            if (code < current->data.iata) {
                current = current->left;
            } else {
                current = current->right;
            }
        }

        if (current == nullptr) {
            cout << code << " not found, nothing deleted" << endl;
            return false;
        }

        if (current->left == nullptr && current->right == nullptr) {
            // Case D1: No children
            if (current == root) {
                root = nullptr;
            } else if (parent->left == current) {
                parent->left = nullptr;
            } else {
                parent->right = nullptr;
            }

            delete current;
            cout << "Deleted " << code << " (case D1: leaf)" << endl;
        } else if (current->left == nullptr || current->right == nullptr) {
            // Case D2: One child
            Node* child;
            string side;
            if (current->left != nullptr) {
                child = current->left;
                side = "only a left child";
            } else {
                child = current->right;
                side = "only a right child";
            }

            if (current == root) {
                root = child;
            } else if (parent->left == current) {
                parent->left = child;
            } else {
                parent->right = child;
            }

            cout << "Deleted " << code << " (case D2: " << side << ", "
                 << child->data.iata << ")" << endl;
            delete current;
        } else {
            // Case D3: Two children
            Node* successorParent = current;
            Node* successor = current->right;

            while (successor->left != nullptr) {
                successorParent = successor;
                successor = successor->left;
            }

            string successorCode = successor->data.iata;
            current->data = successor->data;

            if (successorParent == current) {
                successorParent->right = successor->right;
            } else {
                successorParent->left = successor->right;
            }

            delete successor;
            cout << "Deleted " << code << " (case D3: two children, replaced by successor "
                 << successorCode << ")" << endl;
        }

        return true;
    }

    void printLevelOrder() {
        if (root == nullptr) {
            cout << "Tree is empty" << endl;
            return;
        }

        vector<Node*> queue;
        queue.push_back(root);
        int index = 0;
        int level = 0;

        while (index < (int)queue.size() && level < 3) {
            int levelSize = (int)queue.size() - index;

            for (int i = 0; i < levelSize; i++) {
                Node* current = queue[index];
                cout << current->data.iata << " ";

                if (current->left != nullptr) {
                    queue.push_back(current->left);
                }
                if (current->right != nullptr) {
                    queue.push_back(current->right);
                }
                index = index + 1;
            }

            cout << endl;
            level = level + 1;
        }
    }

    void printFullLevelOrder() {
        if (root == nullptr) {
            cout << "Tree is empty" << endl;
            return;
        }

        vector<Node*> queue;
        queue.push_back(root);
        int index = 0;

        while (index < (int)queue.size()) {
            Node* current = queue[index];
            if (current->left != nullptr) {
                queue.push_back(current->left);
            }
            if (current->right != nullptr) {
                queue.push_back(current->right);
            }
            index = index + 1;
        }

        int total = (int)queue.size();

        if (total <= 20) {
            for (int i = 0; i < total; i++) {
                cout << queue[i]->data.iata << " ";
            }
        } else {
            for (int i = 0; i < 10; i++) {
                cout << queue[i]->data.iata << " ";
            }
            cout << "... ";
            for (int i = total - 10; i < total; i++) {
                cout << queue[i]->data.iata << " ";
            }
        }
        cout << "(" << total << " nodes)" << endl;
    }

    void printList() {
        vector<Airport> sorted;
        inorder(root, sorted);

        for (int i = 0; i < (int)sorted.size(); i++) {
            cout << sorted[i].iata << " "
                 << sorted[i].name << ", "
                 << sorted[i].city << ", "
                 << sorted[i].country << endl;
        }
        cout << "Listed " << sorted.size() << " airports" << endl;
    }

    bool checkSorted() {
        vector<Airport> sorted;
        inorder(root, sorted);

        for (int i = 1; i < (int)sorted.size(); i++) {
            if (sorted[i].iata < sorted[i - 1].iata) {
                return false;
            }
        }
        return true;
    }

    void getSortedAirports(vector<Airport> &sorted) {
        sorted.clear();
        inorder(root, sorted);
    }

    bool saveTree(string fileName) {
        ofstream file(fileName);
        if (!file) {
            return false;
        }

        file.precision(15);
        preorder(root, file);
        file.close();

        return true;
    }

    bool loadTree(string fileName, int &loaded, int &skipped, int &duplicates) {
        vector<string> lines;
        if (!readFile(fileName, lines)) {
            return false;
        }

        vector<Airport> airports;
        loadAirports(lines, airports, skipped);

        clearTree();
        insertAll(airports, loaded, duplicates);

        return true;
    }

    bool sameShape(AirportBST &other) {
        return compareTrees(root, other.root);
    }

    bool validateTree() {
        return validate(root, "", "ZZZZ");
    }

    void range(string from, string to) {
        if (from > to) {
            cout << "Invalid range: " << from << " comes after " << to << endl;
            return;
        }

        int visited = 0;
        int found = 0;
        rangeSearch(root, from, to, visited, found);

        cout << found << " airports, " << visited << " nodes visited" << endl;
    }

    void buildFromSorted(vector<Airport> &sortedAirports) {
        clearTree();
        root = buildBalanced(sortedAirports, 0, (int)sortedAirports.size() - 1);
    }

    void rebalance() {
        vector<Airport> sorted;
        getSortedAirports(sorted);
        buildFromSorted(sorted);
    }
};

//=====================================================

void printValidate(AirportBST &tree) {
    if (tree.validateTree()) {
        cout << "validate: OK" << endl;
    } else {
        cout << "validate: FAILED" << endl;
    }
}

//=====================================================

int countCountries(vector<Airport> &airports, int &usCount) {
    vector<string> countries;
    usCount = 0;

    for (int i = 0; i < (int)airports.size(); i++) {
        if (airports[i].country == "United States") {
            usCount = usCount + 1;
        }

        bool seen = false;
        for (int j = 0; j < (int)countries.size(); j++) {
            if (countries[j] == airports[i].country) {
                seen = true;
            }
        }
        if (seen == false) {
            countries.push_back(airports[i].country);
        }
    }

    return (int)countries.size();
}

//=====================================================

int longestIncreasingRun(vector<Airport> &airports, int &bestStart) {
    int bestLength = 1;
    int length = 1;
    int start = 0;
    bestStart = 0;

    for (int i = 1; i < (int)airports.size(); i++) {
        if (airports[i].iata > airports[i - 1].iata) {
            length = length + 1;
        } else {
            length = 1;
            start = i;
        }

        if (length > bestLength) {
            bestLength = length;
            bestStart = start;
        }
    }

    return bestLength;
}

//=====================================================

void shuffleAirports(vector<Airport> &airports) {
    int n = (int)airports.size();

    for (int i = 0; i < n - 1; i++) {
        int j = i + rand() % (n - i);
        Airport temp = airports[i];
        airports[i] = airports[j];
        airports[j] = temp;
    }
}

//=====================================================

double timeSearches(AirportBST &tree, vector<Airport> &airports, int &found) {
    int searchCount = 10000;

    srand(42);
    vector<int> randomIndexes;
    for (int i = 0; i < searchCount; i++) {
        randomIndexes.push_back(rand() % (int)airports.size());
    }

    found = 0;
    chrono::high_resolution_clock::time_point start = chrono::high_resolution_clock::now();

    for (int i = 0; i < searchCount; i++) {
        if (tree.searchQuiet(airports[randomIndexes[i]].iata)) {
            found = found + 1;
        }
    }

    chrono::high_resolution_clock::time_point end = chrono::high_resolution_clock::now();
    chrono::duration<double, milli> elapsed = end - start;

    return elapsed.count();
}

//=====================================================

string toUpperCode(string code) {
    string result = "";
    for (int i = 0; i < (int)code.length(); i++) {
        char c = code[i];
        if (c >= 'a' && c <= 'z') {
            c = c - 'a' + 'A';
        }
        result = result + c;
    }
    return result;
}

//=====================================================

vector<string> splitWords(string line) {
    vector<string> words;
    string word = "";

    for (int i = 0; i < (int)line.length(); i++) {
        if (line[i] == ' ' || line[i] == '\t' || line[i] == '\r') {
            if (word != "") {
                words.push_back(word);
                word = "";
            }
        } else {
            word = word + line[i];
        }
    }
    if (word != "") {
        words.push_back(word);
    }

    return words;
}

//=====================================================

string cleanPath(string path) {
    int start = 0;
    int end = (int)path.length() - 1;

    while (start <= end && (path[start] == ' ' || path[start] == '"')) {
        start = start + 1;
    }
    while (end >= start && (path[end] == ' ' || path[end] == '"' || path[end] == '\r')) {
        end = end - 1;
    }

    return path.substr(start, end - start + 1);
}

//=====================================================

void testDeletion(AirportBST &tree, string code, string label) {
    cout << endl << label << ":" << endl;
    cout << "Deleting: " << code << endl;
    cout << "Before:" << endl;
    tree.printLevelOrder();

    tree.remove(code);
    printValidate(tree);

    cout << "After:" << endl;
    tree.printLevelOrder();
}

//=====================================================

void runTask1(vector<Airport> &airports, int totalLines, int skipped) {
    cout << endl << "Task 1" << endl;

    AirportBST tree;
    int inserted = 0;
    int duplicates = 0;
    tree.insertAll(airports, inserted, duplicates);

    int usCount = 0;
    int countries = countCountries(airports, usCount);

    cout << "Total lines: " << totalLines << endl;
    cout << "Loaded " << airports.size() << " airports (" << skipped
         << " lines skipped: no IATA code). Duplicates: " << duplicates << endl;
    cout << "Inserted: " << inserted << endl;
    cout << "Countries: " << countries << ", United States airports: " << usCount << endl;
    cout << "Height: " << tree.getHeight() << endl;
    cout << "Average depth: " << tree.getAverageDepth() << endl;
}

//=====================================================

void runTask2(vector<Airport> &airports) {
    cout << endl << "Task 2" << endl;

    AirportBST tree;
    int inserted = 0;
    int duplicates = 0;
    tree.insertAll(airports, inserted, duplicates);

    cout << endl << "Minimum code: " << tree.findMin() << endl;
    cout << "Maximum code: " << tree.findMax() << endl;

    cout << endl << "Root:" << endl;
    tree.search(tree.getRootCode());

    cout << endl << "Leaf:" << endl;
    tree.search(tree.findLeafCode());

    cout << endl << "Deep node:" << endl;
    string deepCode = tree.getDeepestCode();
    cout << "Deepest code: " << deepCode << endl;
    tree.search(deepCode);

    cout << endl << "Nonexistent code:" << endl;
    tree.search("XYZ");

    cout << endl << "Smaller than minimum:" << endl;
    tree.search("000");

    cout << endl << "Larger than maximum:" << endl;
    tree.search("ZZZ");
}

//=====================================================

void runTask3(vector<Airport> &airports) {
    cout << endl << "Task 3" << endl;

    AirportBST tree;
    int inserted = 0;
    int duplicates = 0;
    tree.insertAll(airports, inserted, duplicates);

    // D1: Leaf
    testDeletion(tree, tree.findLeafCode(), "Leaf (D1)");

    // D2: Only left child
    testDeletion(tree, tree.findOnlyLeftChild(), "Only Left Child (D2)");

    // D2: Only right child
    testDeletion(tree, tree.findOnlyRightChild(), "Only Right Child (D2)");

    // D3: Two children
    testDeletion(tree, tree.findTwoChildren(), "Two Children (D3)");

    // Delete root
    testDeletion(tree, tree.getRootCode(), "Root");

    // Add a made-up airport
    cout << endl << "Add Made-Up Airport QQQ:" << endl;

    Airport fakeAirport;
    fakeAirport.id = "\\N";
    fakeAirport.name = "My Test Airport";
    fakeAirport.city = "Boston";
    fakeAirport.country = "United States";
    fakeAirport.iata = "QQQ";
    fakeAirport.icao = "KQQQ";
    fakeAirport.latitude = 42.0;
    fakeAirport.longitude = -71.0;

    if (tree.insert(fakeAirport)) {
        cout << "Inserted QQQ" << endl;
    } else {
        cout << "QQQ already exists (rule I4)" << endl;
    }
    printValidate(tree);

    cout << endl << "Searching for QQQ:" << endl;
    tree.search("QQQ");

    // Remove the made-up airport
    cout << endl << "Remove QQQ:" << endl;
    tree.remove("QQQ");
    printValidate(tree);

    cout << endl << "Searching for QQQ again:" << endl;
    tree.search("QQQ");
}

//=====================================================

void runTask4(vector<Airport> &airports) {
    cout << endl << "Task 4" << endl;

    AirportBST tree;
    int inserted = 0;
    int duplicates = 0;
    tree.insertAll(airports, inserted, duplicates);

    // Task 4(a): Inorder Traversal
    cout << endl << "List (inorder):" << endl;
    tree.printList();

    if (tree.checkSorted()) {
        cout << "Inorder codes are sorted" << endl;
    } else {
        cout << "Inorder codes are NOT sorted" << endl;
    }

    // Task 4(b): Save and Reload Tree
    cout << endl << "Save:" << endl;
    string fileName = "preorder.txt";

    if (tree.saveTree(fileName)) {
        cout << "Saved " << tree.getCount() << " airports in preorder to " << fileName << endl;
    } else {
        cout << "Error saving " << fileName << endl;
        return;
    }

    cout << endl << "Load:" << endl;
    AirportBST reloadedTree;
    int loaded = 0;
    int skipped = 0;
    int reloadDuplicates = 0;

    if (reloadedTree.loadTree(fileName, loaded, skipped, reloadDuplicates) == false) {
        cout << "Error loading " << fileName << endl;
        return;
    }
    cout << "Loaded " << loaded << " airports from " << fileName << endl;

    cout << endl << "Original height: " << tree.getHeight() << endl;
    cout << "Reloaded height: " << reloadedTree.getHeight() << endl;

    cout << endl << "Original top three levels:" << endl;
    tree.printLevelOrder();
    cout << "Reloaded top three levels:" << endl;
    reloadedTree.printLevelOrder();

    cout << endl << "Original full level order:" << endl;
    tree.printFullLevelOrder();
    cout << "Reloaded full level order:" << endl;
    reloadedTree.printFullLevelOrder();

    if (tree.sameShape(reloadedTree)) {
        cout << "Trees have the same shape" << endl;
    } else {
        cout << "Trees have different shapes" << endl;
    }

    // Task 4(c): BST Validation
    cout << endl << "Validate:" << endl;
    cout << "Original tree ";
    printValidate(tree);
    cout << "Reloaded tree ";
    printValidate(reloadedTree);
}

//=====================================================

void runTask5(vector<Airport> &airports) {
    cout << endl << "Task 5" << endl;

    AirportBST tree;
    int inserted = 0;
    int duplicates = 0;
    tree.insertAll(airports, inserted, duplicates);

    cout << endl << "Range BOS to BWI:" << endl;
    tree.range("BOS", "BWI");
}

//=====================================================

void runTask6(vector<Airport> &airports) {
    cout << endl << "Task 6" << endl;

    int inserted = 0;
    int duplicates = 0;

    // Build the file order tree
    AirportBST fileOrderTree;
    fileOrderTree.insertAll(airports, inserted, duplicates);

    // Build the sorted order tree
    vector<Airport> sortedAirports;
    fileOrderTree.getSortedAirports(sortedAirports);
    AirportBST sortedTree;
    sortedTree.insertAll(sortedAirports, inserted, duplicates);

    // Build the balanced tree
    AirportBST balancedTree;
    balancedTree.buildFromSorted(sortedAirports);

    // Build the random shuffle tree
    vector<Airport> shuffledAirports = airports;
    srand(2026);
    shuffleAirports(shuffledAirports);
    AirportBST randomTree;
    randomTree.insertAll(shuffledAirports, inserted, duplicates);

    // Time 10,000 random searches
    int fileFound = 0;
    int sortedFound = 0;
    int balancedFound = 0;
    int randomFound = 0;
    double fileTime = timeSearches(fileOrderTree, airports, fileFound);
    double sortedTime = timeSearches(sortedTree, airports, sortedFound);
    double balancedTime = timeSearches(balancedTree, airports, balancedFound);
    double randomTime = timeSearches(randomTree, airports, randomFound);

    // Print the results
    cout << endl << "File order: height " << fileOrderTree.getHeight()
         << ", average depth " << fileOrderTree.getAverageDepth()
         << ", 10,000 searches " << fileTime << " ms (found " << fileFound << ")" << endl;
    cout << "Sorted order: height " << sortedTree.getHeight()
         << ", average depth " << sortedTree.getAverageDepth()
         << ", 10,000 searches " << sortedTime << " ms (found " << sortedFound << ")" << endl;
    cout << "Balanced: height " << balancedTree.getHeight()
         << ", average depth " << balancedTree.getAverageDepth()
         << ", 10,000 searches " << balancedTime << " ms (found " << balancedFound << ")" << endl;
    cout << "Random shuffle: height " << randomTree.getHeight()
         << ", average depth " << randomTree.getAverageDepth()
         << ", 10,000 searches " << randomTime << " ms (found " << randomFound << ")" << endl;

    // Longest run of increasing codes in file order
    int runStart = 0;
    int runLength = longestIncreasingRun(airports, runStart);
    int runEnd = runStart + runLength - 1;

    cout << endl << "Longest run of increasing codes in file order: " << runLength
         << " airports (" << airports[runStart].iata << " to " << airports[runEnd].iata
         << ", " << airports[runStart].country << ")" << endl;
    cout << "Deepest node in the file order tree: " << fileOrderTree.getDeepestCode() << endl;
}

//=====================================================

void printHelp() {
    cout << "load FILE        rebuild the tree from airports.dat or a saved file" << endl;
    cout << "find CODE        search path, comparisons and airport" << endl;
    cout << "add CODE         add an airport (asks for name, city, country)" << endl;
    cout << "remove CODE      delete an airport, then validate" << endl;
    cout << "list             every airport in code order" << endl;
    cout << "save FILE        write the airports in preorder" << endl;
    cout << "validate         check the BST property" << endl;
    cout << "range FROM TO    every airport from FROM to TO" << endl;
    cout << "rebalance        rebuild as a balanced tree" << endl;
    cout << "levels           top three levels" << endl;
    cout << "stats            size, height, average depth, min and max" << endl;
    cout << "quit             back to the menu" << endl;
}

//=====================================================

void runCommandMode() {
    AirportBST tree;
    string line;

    cout << endl << "Command mode. Type help for the commands, quit to go back." << endl;

    while (true) {
        cout << "> ";
        if (!getline(cin, line)) {
            cout << endl;
            return;
        }

        vector<string> words = splitWords(line);
        if ((int)words.size() == 0) {
            continue;
        }

        string command = words[0];

        if (command == "quit") {
            return;
        } else if (command == "help") {
            printHelp();
        } else if (command == "load" && (int)words.size() == 2) {
            int loaded = 0;
            int skipped = 0;
            int duplicates = 0;
            if (tree.loadTree(words[1], loaded, skipped, duplicates)) {
                cout << "Loaded " << loaded << " airports (" << skipped
                     << " lines skipped: no IATA code). Duplicates: " << duplicates << endl;
                cout << "Height: " << tree.getHeight()
                     << " Average depth: " << tree.getAverageDepth() << endl;
            } else {
                cout << "Cannot open " << words[1] << endl;
            }
        } else if (command == "find" && (int)words.size() == 2) {
            tree.search(toUpperCode(words[1]));
        } else if (command == "add" && (int)words.size() == 2) {
            Airport a;
            a.id = "\\N";
            a.iata = toUpperCode(words[1]);

            if (tree.searchQuiet(a.iata)) {
                cout << a.iata << " already exists (rule I4), not added" << endl;
            } else {
                cout << "Name: ";
                getline(cin, a.name);
                cout << "City: ";
                getline(cin, a.city);
                cout << "Country: ";
                getline(cin, a.country);

                tree.insert(a);
                cout << "Added " << a.iata << endl;
                printValidate(tree);
            }
        } else if (command == "remove" && (int)words.size() == 2) {
            tree.remove(toUpperCode(words[1]));
            printValidate(tree);
        } else if (command == "list") {
            tree.printList();
        } else if (command == "save" && (int)words.size() == 2) {
            if (tree.saveTree(words[1])) {
                cout << "Saved " << tree.getCount() << " airports in preorder to " << words[1] << endl;
            } else {
                cout << "Cannot write " << words[1] << endl;
            }
        } else if (command == "validate") {
            printValidate(tree);
        } else if (command == "range" && (int)words.size() == 3) {
            tree.range(toUpperCode(words[1]), toUpperCode(words[2]));
        } else if (command == "rebalance") {
            tree.rebalance();
            cout << "Height: " << tree.getHeight()
                 << " Average depth: " << tree.getAverageDepth() << endl;
        } else if (command == "levels") {
            tree.printLevelOrder();
        } else if (command == "stats") {
            cout << "Airports: " << tree.getCount()
                 << " Height: " << tree.getHeight()
                 << " Average depth: " << tree.getAverageDepth() << endl;
            cout << "Minimum code: " << tree.findMin()
                 << " Maximum code: " << tree.findMax() << endl;
        } else {
            cout << "Unknown command or wrong number of words. Type help." << endl;
        }
    }
}

//=====================================================

int main(int argc, char* argv[]) {
    vector<string> lines;
    string fileName = "";

    // Find the data file
    vector<string> paths;
    if (argc > 1) {
        paths.push_back(argv[1]);
    }
    paths.push_back("data/airports.dat");
    paths.push_back("airports.dat");
    paths.push_back("../data/airports.dat");
    paths.push_back("../airports.dat");

    for (int i = 0; i < (int)paths.size(); i++) {
        if (fileName == "" && readFile(paths[i], lines)) {
            fileName = paths[i];
        }
    }

    while (fileName == "") {
        cout << "Cannot find airports.dat. Type its full path (or q to quit): ";

        string typed;
        if (!getline(cin, typed)) {
            cout << endl;
            return 1;
        }
        typed = cleanPath(typed);

        if (typed == "q") {
            return 0;
        }
        if (readFile(typed, lines)) {
            fileName = typed;
        } else {
            cout << "Cannot open " << typed << endl;
        }
    }

    cout << "Data file: " << fileName << endl;

    vector<Airport> airports;
    int skipped = 0;
    loadAirports(lines, airports, skipped);

    string choice = "";

    while (choice != "0") {
        cout << endl << "Airport BST Project" << endl;
        cout << "1. Task 1 - Load Data and Build BST" << endl;
        cout << "2. Task 2 - Search" << endl;
        cout << "3. Task 3 - Delete" << endl;
        cout << "4. Task 4 - Traversals and Validation" << endl;
        cout << "5. Task 5 - Range Queries" << endl;
        cout << "6. Task 6 - Shape Experiment" << endl;
        cout << "7. Command Mode (load, find, remove, range, validate, ...)" << endl;
        cout << "0. Exit" << endl;
        cout << "Enter task number: ";

        string line;
        if (!getline(cin, line)) {
            cout << endl;
            return 0;
        }

        vector<string> words = splitWords(line);
        if ((int)words.size() == 0) {
            continue;
        }
        choice = words[0];

        if (choice == "1") {
            runTask1(airports, (int)lines.size(), skipped);
        } else if (choice == "2") {
            runTask2(airports);
        } else if (choice == "3") {
            runTask3(airports);
        } else if (choice == "4") {
            runTask4(airports);
        } else if (choice == "5") {
            runTask5(airports);
        } else if (choice == "6") {
            runTask6(airports);
        } else if (choice == "7") {
            runCommandMode();
        } else if (choice == "0") {
            cout << "Exiting program." << endl;
        } else {
            cout << "Invalid choice." << endl;
        }
    }

    return 0;
}

#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <algorithm>
#include <chrono>
#include <cstdlib>
#include <algorithm>
#include <random>
#include <chrono>
using namespace std;

//=====================================================

class Airport
{
public:
	string name;
	string city;
	string country;
	string iata;
	string icao;
	double latitude = 0.0;
	double longitude = 0.0;
};

//=====================================================

vector<string> splitLine(const string& line);

//=====================================================

class AirportBST
{
private:

	struct Node
	{
		Airport data;
		Node* left;
		Node* right;

		Node(Airport a)
		{
			data = a;
			left = nullptr;
			right = nullptr;
		}
	};

	int height(Node* node)
	{
		if (node == nullptr)
		{
			return -1;
		}

		vector<Node*> queue;
		queue.push_back(node);

		int height = -1;
		int index = 0;

		while (index < queue.size())
		{
			int levelSize = (int)queue.size() - index;

			for (int i = 0; i < levelSize; i++)
			{
				Node* current = queue[index];

				if (current->left != nullptr)
				{
					queue.push_back(current->left);
				}

				if (current->right != nullptr)
				{
					queue.push_back(current->right);
				}

				index++;
			}

			height++;
		}

		return height;
	}

	void calculateDepth(Node* node, int depth, int& totalDepth, int& count)
	{
		if (node == nullptr)
		{
			return;
		}

		vector<pair<Node*, int>> stack;

		stack.push_back({ node, depth });

		while (!stack.empty())
		{
			pair<Node*, int> current = stack.back();
			stack.pop_back();

			Node* temp = current.first;
			int currentDepth = current.second;

			totalDepth += currentDepth;
			count++;

			if (temp->right != nullptr)
			{
				stack.push_back({ temp->right, currentDepth + 1 });
			}

			if (temp->left != nullptr)
			{
				stack.push_back({ temp->left, currentDepth + 1 });
			}
		}
	}

	Node* root = nullptr;

	bool findLeaf(Node* node)
	{
		if (node == nullptr)
		{
			return false;
		}

		if (node->left == nullptr && node->right == nullptr)
		{
			cout << "Leaf code: " << node->data.iata << '\n';
			return true;
		}

		if (findLeaf(node->left))
		{
			return true;
		}

		if (findLeaf(node->right))
		{
			return true;
		}

		return false;
	}

	void findDeepest(Node* node, int depth,
		int& maxDepth, string& code)
	{
		if (node == nullptr)
		{
			return;
		}

		if (depth > maxDepth)
		{
			maxDepth = depth;
			code = node->data.iata;
		}

		findDeepest(node->left, depth + 1, maxDepth, code);
		findDeepest(node->right, depth + 1, maxDepth, code);
	}

	Node* findSuccessor(Node* node)
	{
		Node* current = node->right;

		while (current->left != nullptr)
		{
			current = current->left;
		}

		return current;
	}

	void inorder(Node* node, vector<string>& codes)
	{
		if (node == nullptr)
		{
			return;
		}

		inorder(node->left, codes);

		codes.push_back(node->data.iata);

		inorder(node->right, codes);
	}

	void preorder(Node* node, ofstream& file)
	{
		if (node == nullptr)
		{
			return;
		}

		file << '"'
			<< node->data.name << "\",\""
			<< node->data.city << "\",\""
			<< node->data.country << "\",\""
			<< node->data.iata << "\",\""
			<< node->data.icao << "\",\""
			<< node->data.latitude << "\",\""
			<< node->data.longitude << "\"\n";

		preorder(node->left, file);
		preorder(node->right, file);
	}

	bool compareTrees(Node* first, Node* second)
	{
		if (first == nullptr && second == nullptr)
		{
			return true;
		}

		if (first == nullptr || second == nullptr)
		{
			return false;
		}

		if (first->data.iata != second->data.iata)
		{
			return false;
		}

		bool leftSame = compareTrees(first->left, second->left);
		bool rightSame = compareTrees(first->right, second->right);

		return leftSame && rightSame;
	}

	bool validate(Node* node, string min, string max)
	{
		if (node == nullptr)
		{
			return true;
		}

		if (node->data.iata <= min ||
			node->data.iata >= max)
		{
			return false;
		}

		if (!validate(node->left, min, node->data.iata))
		{
			return false;
		}

		if (!validate(node->right, node->data.iata, max))
		{
			return false;
		}

		return true;
	}

	void rangeSearch(Node* node, string from, string to, int& visited)
	{
		if (node == nullptr)
		{
			return;
		}

		visited++;

		if (node->data.iata > from)
		{
			rangeSearch(node->left, from, to, visited);
		}

		if (node->data.iata >= from && node->data.iata <= to)
		{
			cout << node->data.iata << " "
				<< node->data.name << ", "
				<< node->data.city << ", "
				<< node->data.country << '\n';
		}

		if (node->data.iata < to)
		{
			rangeSearch(node->right, from, to, visited);
		}
	}

	Node* buildBalanced(vector<Airport>& airports,
		int start, int end)
	{
		if (start > end)
		{
			return nullptr;
		}

		int mid = start + (end - start) / 2;

		Node* node = new Node(airports[mid]);

		node->left = buildBalanced(airports, start, mid - 1);
		node->right = buildBalanced(airports, mid + 1, end);

		return node;
	}

	bool searchQuiet(string code)
	{
		Node* current = root;

		while (current != nullptr)
		{
			if (code == current->data.iata)
			{
				return true;
			}
			else if (code < current->data.iata)
			{
				current = current->left;
			}
			else
			{
				current = current->right;
			}
		}

		return false;
	}

public:

	bool insert(Airport a)
	{
		if (root == nullptr)
		{
			root = new Node(a);
			return true;
		}

		Node* current = root;

		while (true)
		{
			if (a.iata < current->data.iata)
			{
				if (current->left == nullptr)
				{
					current->left = new Node(a);
					return true;
				}

				current = current->left;
			}

			else if (a.iata > current->data.iata)
			{
				if (current->right == nullptr)
				{
					current->right = new Node(a);
					return true;
				}

				current = current->right;
			}

			else
			{
				return false;
			}
		}
	}

	int getHeight()
	{
		return height(root);
	}

	double getAverageDepth()
	{
		int totalDepth = 0;
		int count = 0;

		calculateDepth(root, 0, totalDepth, count);

		if (count == 0)
		{
			return 0.0;
		}

		return (double)totalDepth / count;
	}


	bool search(string code)
	{
		Node* current = root;
		int comparisons = 0;

		cout << "Path: ";

		while (current != nullptr)
		{
			comparisons++;

			cout << current->data.iata;

			if (code == current->data.iata)
			{
				cout << "\nComparisons: " << comparisons << '\n';

				cout << current->data.iata << " "
					<< current->data.name << ", "
					<< current->data.city << ", "
					<< current->data.country << '\n';

				return true;
			}

			else if (code < current->data.iata)
			{
				current = current->left;
			}

			else
			{
				current = current->right;
			}

			if (current != nullptr)
			{
				cout << " -> ";
			}
		}

		cout << " -> NULL\n";
		cout << "Comparisons: " << comparisons << '\n';
		cout << "Airport not found.\n";

		return false;
	}

	string findMin()
	{
		if (root == nullptr)
		{
			return "";
		}

		Node* current = root;

		while (current->left != nullptr)
		{
			current = current->left;
		}

		return current->data.iata;
	}

	string findMax()
	{
		if (root == nullptr)
		{
			return "";
		}

		Node* current = root;

		while (current->right != nullptr)
		{
			current = current->right;
		}

		return current->data.iata;
	}

	void showLeaf()
	{
		findLeaf(root);
	}

	string getDeepestCode()
	{
		int maxDepth = -1;
		string code = "";

		findDeepest(root, 0, maxDepth, code);

		return code;
	}


	bool remove(string code)
	{
		Node* current = root;
		Node* parent = nullptr;

		while (current != nullptr && current->data.iata != code)
		{
			parent = current;

			if (code < current->data.iata)
			{
				current = current->left;
			}
			else
			{
				current = current->right;
			}
		}

		if (current == nullptr)
		{
			cout << "Airport not found.\n";
			return false;
		}

		// Case D1: No children
		if (current->left == nullptr && current->right == nullptr)
		{
			if (current == root)
			{
				root = nullptr;
			}
			else if (parent->left == current)
			{
				parent->left = nullptr;
			}
			else
			{
				parent->right = nullptr;
			}

			delete current;
			cout << "Deleted " << code << " (D1: leaf)\n";
		}

		// Case D2: One child
		else if (current->left == nullptr || current->right == nullptr)
		{
			Node* child;

			if (current->left != nullptr)
			{
				child = current->left;
			}
			else
			{
				child = current->right;
			}

			if (current == root)
			{
				root = child;
			}
			else if (parent->left == current)
			{
				parent->left = child;
			}
			else
			{
				parent->right = child;
			}

			delete current;
			cout << "Deleted " << code << " (D2: one child)\n";
		}

		// Case D3: Two children
		else
		{
			Node* successorParent = current;
			Node* successor = current->right;

			while (successor->left != nullptr)
			{
				successorParent = successor;
				successor = successor->left;
			}

			current->data = successor->data;

			if (successorParent == current)
			{
				successorParent->right = successor->right;
			}
			else
			{
				successorParent->left = successor->right;
			}

			delete successor;

			cout << "Deleted " << code << " (D3: two children)\n";
		}

		return true;
	}

	void printLevelOrder()
	{
		if (root == nullptr)
		{
			cout << "Tree is empty.\n";
			return;
		}

		vector<Node*> queue;
		queue.push_back(root);

		int level = 0;
		int index = 0;

		while (index < queue.size() && level < 3)
		{
			int levelSize = queue.size() - index;

			for (int i = 0; i < levelSize; i++)
			{
				Node* current = queue[index];

				cout << current->data.iata << " ";

				if (current->left != nullptr)
				{
					queue.push_back(current->left);
				}

				if (current->right != nullptr)
				{
					queue.push_back(current->right);
				}

				index++;
			}

			cout << '\n';
			level++;
		}
	}

	string findLeafCode(Node* node)
	{
		if (node == nullptr)
		{
			return "";
		}

		if (node->left == nullptr && node->right == nullptr)
		{
			return node->data.iata;
		}

		string left = findLeafCode(node->left);

		if (left != "")
		{
			return left;
		}

		return findLeafCode(node->right);
	}

	string findOneChild(Node* node)
	{
		if (node == nullptr)
		{
			return "";
		}

		if ((node->left == nullptr && node->right != nullptr) ||
			(node->left != nullptr && node->right == nullptr))
		{
			return node->data.iata;
		}

		string left = findOneChild(node->left);

		if (left != "")
		{
			return left;
		}

		return findOneChild(node->right);
	}


	string getRootCode()
	{
		if (root == nullptr)
		{
			return "";
		}

		return root->data.iata;
	}

	Node* getRoot()
	{
		return root;
	}

	string findOnlyLeftChild(Node* node)
	{
		if (node == nullptr)
		{
			return "";
		}

		if (node->left != nullptr && node->right == nullptr)
		{
			return node->data.iata;
		}

		string left = findOnlyLeftChild(node->left);

		if (left != "")
		{
			return left;
		}

		return findOnlyLeftChild(node->right);
	}

	string findOnlyRightChild(Node* node)
	{
		if (node == nullptr)
		{
			return "";
		}

		if (node->left == nullptr && node->right != nullptr)
		{
			return node->data.iata;
		}

		string left = findOnlyRightChild(node->left);

		if (left != "")
		{
			return left;
		}

		return findOnlyRightChild(node->right);
	}

	string findTwoChildren(Node* node)
	{
		if (node == nullptr)
		{
			return "";
		}

		if (node != root && node->left != nullptr &&
			node->right != nullptr)
		{
			return node->data.iata;
		}

		string left = findTwoChildren(node->left);

		if (left != "")
		{
			return left;
		}

		return findTwoChildren(node->right);
	}

	void printInorder()
	{
		vector<string> codes;

		inorder(root, codes);

		int total = (int)codes.size();

		if (total <= 20)
		{
			for (int i = 0; i < total; i++)
			{
				cout << codes[i] << '\n';
			}
		}
		else
		{
			cout << "First 10 codes:\n";

			for (int i = 0; i < 10; i++)
			{
				cout << codes[i] << '\n';
			}

			cout << "...\n";

			cout << "Last 10 codes:\n";

			for (int i = total - 10; i < total; i++)
			{
				cout << codes[i] << '\n';
			}
		}
	}

	bool checkSorted()
	{
		vector<string> codes;

		inorder(root, codes);

		for (int i = 1; i < (int)codes.size(); i++)
		{
			if (codes[i] < codes[i - 1])
			{
				return false;
			}
		}

		return true;
	}

	bool saveTree(string fileName)
	{
		ofstream file(fileName);

		if (!file)
		{
			return false;
		}

		preorder(root, file);

		file.close();

		return true;
	}

	bool loadTree(string fileName)
	{
		ifstream file(fileName);

		if (!file)
		{
			return false;
		}

		string line;

		while (getline(file, line))
		{
			vector<string> fields = splitLine(line);

			if (fields.size() < 7)
			{
				continue;
			}

			Airport a;

			a.name = fields[0];
			a.city = fields[1];
			a.country = fields[2];
			a.iata = fields[3];
			a.icao = fields[4];
			a.latitude = stod(fields[5]);
			a.longitude = stod(fields[6]);

			insert(a);
		}

		file.close();

		return true;
	}

	bool sameShape(AirportBST& other)
	{
		return compareTrees(root, other.root);
	}

	void printFullLevelOrder()
	{
		if (root == nullptr)
		{
			cout << "Tree is empty.\n";
			return;
		}

		vector<Node*> queue;
		queue.push_back(root);

		int index = 0;
		vector<string> codes;

		while (index < (int)queue.size())
		{
			Node* current = queue[index];

			codes.push_back(current->data.iata);

			if (current->left != nullptr)
			{
				queue.push_back(current->left);
			}

			if (current->right != nullptr)
			{
				queue.push_back(current->right);
			}

			index++;
		}

		int total = (int)codes.size();

		if (total <= 20)
		{
			for (int i = 0; i < total; i++)
			{
				cout << codes[i] << " ";
			}
		}
		else
		{
			for (int i = 0; i < 10; i++)
			{
				cout << codes[i] << " ";
			}

			cout << "... ";

			for (int i = total - 10; i < total; i++)
			{
				cout << codes[i] << " ";
			}
		}

		cout << '\n';
	}

	bool validateTree()
	{
		return validate(root, "", "ZZZ");
	}

	void range(string from, string to)
	{
		if (from > to)
		{
			cout << "Invalid range.\n";
			return;
		}

		int visited = 0;

		cout << "\nAirports between " << from
			<< " and " << to << ":\n";

		rangeSearch(root, from, to, visited);

		cout << "\nNodes visited: " << visited << '\n';
	}

	void makeBalanced(vector<Airport>& airports)
	{
		root = buildBalanced(airports, 0,
			(int)airports.size() - 1);
	}

	bool searchForTiming(string code)
	{
		return searchQuiet(code);
	}
};

//=====================================================

bool readFile(const string& fileName, vector<string> & lines)
{
	ifstream file(fileName);
	if (!file)
	{
		return false;
	}
	string line;
	while (getline(file, line))
	{
		lines.push_back(line);
	}
	return true;
}

//=====================================================

vector<string> splitLine(const string& line)
{
	vector<string> fields;
	string field = "";
	bool insideQuotes = false;

	for (int i = 0; i < line.length(); i++)
	{
		char c = line[i];

		if (c == '"')
		{
			insideQuotes = !insideQuotes;
		}
		else if (c == ',' && insideQuotes == false)
		{
			fields.push_back(field);
			field = "";
		}
		else
		{
			field += c;
		}
	}

	fields.push_back(field);

	return fields;
}

//=====================================================

void loadAirports(const vector<string>& lines,
	vector<Airport>& airports,
	int& skipped)
{
	skipped = 0;

	for (int i = 0; i < lines.size(); i++)
	{
		vector<string> fields = splitLine(lines[i]);

		if (fields.size() < 8)
		{
			continue;
		}

		if (fields[4] == "\\N")
		{
			skipped++;
			continue;
		}

		Airport a;

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

double timeSearches(AirportBST& tree,
	vector<Airport>& airports)
{
	const int SEARCH_COUNT = 10000;

	srand(42);

	vector<int> randomIndexes;

	for (int i = 0; i < SEARCH_COUNT; i++)
	{
		int index = rand() % airports.size();
		randomIndexes.push_back(index);
	}

	auto start = chrono::high_resolution_clock::now();

	for (int i = 0; i < SEARCH_COUNT; i++)
	{
		tree.searchForTiming(
			airports[randomIndexes[i]].iata);
	}

	auto end = chrono::high_resolution_clock::now();

	chrono::duration<double, milli> elapsed = end - start;

	return elapsed.count();
}

//=====================================================


int main()
{
	string fileName = "data/airports.dat";
	vector<string> lines;

	if (!readFile(fileName, lines))
	{
		cout << "Error: cannot open " << fileName << '\n';
		return 1;
	}

	vector<Airport> airports;
	int skipped = 0;

	loadAirports(lines, airports, skipped);

	AirportBST tree;

	int inserted = 0;
	int duplicates = 0;

	for (int i = 0; i < airports.size(); i++)
	{
		if (tree.insert(airports[i]))
		{
			inserted++;
		}
		else
		{
			duplicates++;
			cout << "Duplicate: " << airports[i].iata << '\n';
		}
	}

	int choice;

	cout << "\n===== AIRPORT BST PROJECT =====\n";
	cout << "1. Task 1 - Load Data and Build BST\n";
	cout << "2. Task 2 - Search\n";
	cout << "3. Task 3 - Delete\n";
	cout << "4. Task 4 - Traversals and Validation\n";
	cout << "5. Task 5 - Range Queries\n";
	cout << "6. Task 6 - Shape Experiment\n";
	cout << "0. Exit\n";
	cout << "Enter task number: ";

	cin >> choice;

	switch (choice)
	{
	case 1:
	{
		cout << "\n===== TASK 1 =====\n";

		cout << "Total lines: " << lines.size() << '\n';
		cout << "Airports loaded: " << airports.size() << '\n';
		cout << "Airports skipped: " << skipped << '\n';
		cout << "Inserted: " << inserted << '\n';
		cout << "Duplicates: " << duplicates << '\n';
		cout << "Height: " << tree.getHeight() << '\n';

		cout << "Average depth: "
			<< tree.getAverageDepth() << '\n';

		break;
	}

	case 2:
	{
		cout << "\n===== TASK 2 =====\n";

		cout << "\nMinimum code: " << tree.findMin() << '\n';
		cout << "Maximum code: " << tree.findMax() << '\n';

		cout << "\n--- Root ---\n";
		tree.search("GKA");

		cout << "\n--- Nonexistent code ---\n";
		tree.search("XYZ");

		cout << "\n--- Smaller than minimum ---\n";
		tree.search("000");

		cout << "\n--- Larger than maximum ---\n";
		tree.search("ZZZ");

		cout << "\n--- Leaf ---\n";
		tree.search("AAC");

		cout << "\n--- Deep node ---\n";

		string deepCode = tree.getDeepestCode();

		cout << "Deepest code: " << deepCode << '\n';
		tree.search(deepCode);

		break;
	}

	case 3:
	{
		cout << "\n===== TASK 3 =====\n";

		string code;

		// D1: Leaf
		code = tree.findLeafCode(tree.getRoot());

		cout << "\n--- D1: Leaf ---\n";
		cout << "Deleting: " << code << '\n';

		cout << "Before:\n";
		tree.printLevelOrder();

		tree.remove(code);

		cout << "After:\n";
		tree.printLevelOrder();

		// D2: Only left child
		code = tree.findOnlyLeftChild(tree.getRoot());

		cout << "\n--- D2: Only Left Child ---\n";
		cout << "Deleting: " << code << '\n';

		cout << "Before:\n";
		tree.printLevelOrder();

		tree.remove(code);

		cout << "After:\n";
		tree.printLevelOrder();

		// D2: Only right child
		code = tree.findOnlyRightChild(tree.getRoot());

		cout << "\n--- D2: Only Right Child ---\n";
		cout << "Deleting: " << code << '\n';

		cout << "Before:\n";
		tree.printLevelOrder();

		tree.remove(code);

		cout << "After:\n";
		tree.printLevelOrder();

		// D3: Two children
		code = tree.findTwoChildren(tree.getRoot());

		cout << "\n--- D3: Two Children ---\n";
		cout << "Deleting: " << code << '\n';

		cout << "Before:\n";
		tree.printLevelOrder();

		tree.remove(code);

		cout << "After:\n";
		tree.printLevelOrder();

		// Delete root
		code = tree.getRootCode();

		cout << "\n--- Root Deletion ---\n";
		cout << "Deleting: " << code << '\n';

		cout << "Before:\n";
		tree.printLevelOrder();

		tree.remove(code);

		cout << "After:\n";
		tree.printLevelOrder();

		// Add a made-up airport
		cout << "\n--- Add Made-Up Airport ---\n";

		Airport fakeAirport;

		fakeAirport.name = "My Test Airport";
		fakeAirport.city = "Boston";
		fakeAirport.country = "United States";
		fakeAirport.iata = "QQQ";
		fakeAirport.icao = "KQQQ";
		fakeAirport.latitude = 42.0;
		fakeAirport.longitude = -71.0;

		tree.insert(fakeAirport);

		cout << "\nSearching for QQQ:\n";
		tree.search("QQQ");

		// Remove the made-up airport
		cout << "\n--- Remove QQQ ---\n";

		tree.remove("QQQ");

		cout << "\nSearching for QQQ again:\n";
		tree.search("QQQ");

		break;
	}

	case 4:
	{
		cout << "\n===== TASK 4 =====\n";

		// Task 4(a): Inorder Traversal
		cout << "\n--- Inorder Traversal ---\n";

		tree.printInorder();

		cout << "\n--- Sorted Check ---\n";

		if (tree.checkSorted())
		{
			cout << "Airport codes are sorted.\n";
		}
		else
		{
			cout << "Airport codes are NOT sorted.\n";
		}

		// Task 4(b): Save and Reload Tree
		cout << "\n--- Save Tree ---\n";

		if (tree.saveTree("preorder.txt"))
		{
			cout << "Tree saved successfully.\n";
		}
		else
		{
			cout << "Error saving tree.\n";
		}

		cout << "\n--- Reload Tree ---\n";

		AirportBST reloadedTree;

		if (reloadedTree.loadTree("preorder.txt"))
		{
			cout << "Tree reloaded successfully.\n";

			cout << "\nOriginal Height: "
				<< tree.getHeight() << '\n';

			cout << "Reloaded Height: "
				<< reloadedTree.getHeight() << '\n';

			cout << "\nOriginal Level Order:\n";

			tree.printFullLevelOrder();

			cout << "\nReloaded Level Order:\n";

			reloadedTree.printFullLevelOrder();

			cout << "\n--- Shape Comparison ---\n";

			if (tree.sameShape(reloadedTree))
			{
				cout << "Trees have the same shape.\n";
			}
			else
			{
				cout << "Trees have different shapes.\n";
			}
		}
		else
		{
			cout << "Error loading tree.\n";
		}

		// Task 4(c): BST Validation
		cout << "\n--- BST Validation ---\n";

		if (tree.validateTree())
		{
			cout << "Original BST is valid.\n";
		}
		else
		{
			cout << "Original BST is NOT valid.\n";
		}

		if (reloadedTree.validateTree())
		{
			cout << "Reloaded BST is valid.\n";
		}
		else
		{
			cout << "Reloaded BST is NOT valid.\n";
		}

		break;
	}

	case 5:
	{
		cout << "\n===== TASK 5 =====\n";

		cout << "\n--- Range Query: BOS to BWI ---\n";

		tree.range("BOS", "BWI");

		break;
	}

	case 6:
	{
		cout << "\n===== TASK 6 =====\n";

		// Create the four BSTs
		AirportBST fileOrderTree;
		AirportBST sortedTree;
		AirportBST balancedTree;
		AirportBST randomTree;

		// Build the file order tree
		for (int i = 0; i < airports.size(); i++)
		{
			fileOrderTree.insert(airports[i]);
		}

		// Build the sorted order tree
		vector<Airport> sortedAirports = airports;

		sort(sortedAirports.begin(), sortedAirports.end(),
			[](const Airport& a, const Airport& b)
			{
				return a.iata < b.iata;
			});

		for (int i = 0; i < sortedAirports.size(); i++)
		{
			sortedTree.insert(sortedAirports[i]);
		}

		// Build the balanced tree
		balancedTree.makeBalanced(sortedAirports);

		// Build the random shuffle tree
		vector<Airport> shuffledAirports = airports;

		random_device rd;
		mt19937 generator(rd());

		shuffle(shuffledAirports.begin(),
			shuffledAirports.end(),
			generator);

		for (int i = 0; i < shuffledAirports.size(); i++)
		{
			randomTree.insert(shuffledAirports[i]);
		}

		// Print tree heights
		cout << "\n--- Tree Heights ---\n";

		cout << "File Order Height: "
			<< fileOrderTree.getHeight() << '\n';

		cout << "Sorted Order Height: "
			<< sortedTree.getHeight() << '\n';

		cout << "Balanced Height: "
			<< balancedTree.getHeight() << '\n';

		cout << "Random Shuffle Height: "
			<< randomTree.getHeight() << '\n';

		// Print average depths
		cout << "\n--- Average Depths ---\n";

		cout << "File Order: "
			<< fileOrderTree.getAverageDepth() << '\n';

		cout << "Sorted Order: "
			<< sortedTree.getAverageDepth() << '\n';

		cout << "Balanced: "
			<< balancedTree.getAverageDepth() << '\n';

		cout << "Random Shuffle: "
			<< randomTree.getAverageDepth() << '\n';

		// Time 10,000 random searches
		cout << "\n--- Search Timing (10,000 Searches) ---\n";

		double fileTime =
			timeSearches(fileOrderTree, airports);

		double sortedTime =
			timeSearches(sortedTree, airports);

		double balancedTime =
			timeSearches(balancedTree, airports);

		double randomTime =
			timeSearches(randomTree, airports);

		cout << "File Order: "
			<< fileTime << " ms\n";

		cout << "Sorted Order: "
			<< sortedTime << " ms\n";

		cout << "Balanced: "
			<< balancedTime << " ms\n";

		cout << "Random Shuffle: "
			<< randomTime << " ms\n";

		break;
	}

	case 0:
	{
		cout << "Exiting program.\n";
		break;
	}

	default:
	{
		cout << "Invalid choice.\n";
		break;
	}
	}

	return 0;
}

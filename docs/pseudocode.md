Airport Binary Search Tree – Pseudocode
Task 1: Load the Data and Build the Tree

Read airports.dat line by line, split each line into fields, and account for airport names that may contain commas inside quotation marks. Skip records whose IATA code is \N and store the remaining records in an array or vector of Airport.

1.1 Split a Line into Fields

Pseudocode:

text
function splitLine(line)
    fields = []
    field = ""
    insideQuotes = false

    for i = 0 to line.length() - 1
        if line[i] = '"'
            insideQuotes = NOT insideQuotes
        else if line[i] = ',' AND insideQuotes = false
            fields.push_back(field)
            field = ""
        else
            field = field + line[i]

    fields.push_back(field)
    return fields
end function

1.2 Load Airport Records

Pseudocode:

text
function loadAirports(lines, airports)
    for i = 0 to lines.size() - 1
        fields = splitLine(lines[i])

        if fields[4] = "\N"
            continue

        Airport a
        a.name = fields[1]
        a.city = fields[2]
        a.country = fields[3]
        a.iata = fields[4]
        a.icao = fields[5]
        a.latitude = fields[6]
        a.longitude = fields[7]

        airports.push_back(a)
end function

1.3 Insert Records and Build the BST

Build the BST by inserting the airport records in their original file order. Count the number of successfully inserted keys and report duplicates according to rule I4.

Pseudocode:

text
function insert(AirportBST tree, Airport a)
    if tree.root = NULL
        tree.root = new Node(a)
        return true

    current = tree.root

    while true
        if a.iata < current.data.iata
            if current.left = NULL
                current.left = new Node(a)
                return true
            current = current.left

        else if a.iata > current.data.iata
            if current.right = NULL
                current.right = new Node(a)
                return true
            current = current.right

        else
            return false
end function


Build the tree:

text
function buildTree(airports, tree)
    inserted = 0
    duplicates = 0

    for i = 0 to airports.size() - 1
        if insert(tree, airports[i]) = true
            inserted++
        else
            duplicates++

    return inserted, duplicates
end function

1.4 Calculate Tree Height and Average Depth

Calculate height:

text
function height(Node* node)
    if node = NULL
        return -1

    leftHeight = height(node.left)
    rightHeight = height(node.right)

    if leftHeight > rightHeight
        return leftHeight + 1
    else
        return rightHeight + 1
end function


Calculate the depth of each node:

text
function calculateDepth(Node* node, depth, totalDepth, count)
    if node = NULL
        return

    totalDepth = totalDepth + depth
    count++

    calculateDepth(node.left, depth + 1, totalDepth, count)
    calculateDepth(node.right, depth + 1, totalDepth, count)
end function


Calculate average depth:

text
function averageDepth(root)
    totalDepth = 0
    count = 0

    calculateDepth(root, 0, totalDepth, count)

    if count = 0
        return 0

    return totalDepth / count
end function

Task 2: Search

The find CODE command searches for an airport by its IATA code. It prints the airport information, the search path (every code compared), and the total number of comparisons.

2.1 Search for an Airport

Pseudocode:

text
function search(code)
    current = root
    comparisons = 0
    path = ""

    while current != NULL
        comparisons++
        path = path + current.data.iata

        if code = current.data.iata
            print path
            print comparisons
            print airport information
            return true

        else if code < current.data.iata
            current = current.left

        else
            current = current.right

    print path
    print comparisons
    print "Airport not found"

    return false
end function


Test the following search cases:

Search for the root.
Search for a leaf.
Search for a code deep in the tree.
Search for a code that does not exist (for example, XYZ).
Search for a code smaller than every key.
Search for a code larger than every key.

Also print the minimum and maximum airport codes.

2.2 Find the Minimum and Maximum Codes

Find minimum:

text
function findMin()
    if root = NULL
        return NULL

    current = root

    while current.left != NULL
        current = current.left

    return current.data.iata
end function


Find maximum:

text
function findMax()
    if root = NULL
        return NULL

    current = root

    while current.right != NULL
        current = current.right

    return current.data.iata
end function

2.3 Find a Leaf Node
text
function findLeaf(Node* node)
    if node = NULL
        return false

    if node.left = NULL AND node.right = NULL
        print node.data.iata
        return true

    if findLeaf(node.left) = true
        return true

    if findLeaf(node.right) = true
        return true

    return false
end function

2.4 Find the Deepest Node
text
function findDeepest(Node* node, depth, maxDepth, code)
    if node = NULL
        return

    if depth > maxDepth
        maxDepth = depth
        code = node.data.iata

    findDeepest(node.left, depth + 1, maxDepth, code)
    findDeepest(node.right, depth + 1, maxDepth, code)
end function


Get the deepest code:

text
function getDeepestCode()
    maxDepth = -1
    code = ""

    findDeepest(root, 0, maxDepth, code)

    return code
end function

Task 3: Delete

The remove CODE command deletes an airport from the BST. The deletion algorithm handles the three standard BST deletion cases:

A node with no children (leaf).
A node with one child.
A node with two children.
3.1 Find the Inorder Successor
text
function findSuccessor(Node* node)
    current = node.right

    while current.left != NULL
        current = current.left

    return current
end function

3.2 Remove an Airport
text
function remove(code)
    current = root
    parent = NULL

    while current != NULL AND current.data.iata != code
        parent = current

        if code < current.data.iata
            current = current.left
        else
            current = current.right

    if current = NULL
        return false

    // Case 1: Node has no children
    if current.left = NULL AND current.right = NULL
        if current = root
            root = NULL
        else if parent.left = current
            parent.left = NULL
        else
            parent.right = NULL

        delete current

    // Case 2: Node has one child
    else if current.left = NULL OR current.right = NULL
        if current.left != NULL
            child = current.left
        else
            child = current.right

        if current = root
            root = child
        else if parent.left = current
            parent.left = child
        else
            parent.right = child

        delete current

    // Case 3: Node has two children
    else
        successor = findSuccessor(current)
        successorParent = current

        while successorParent.left != successor
            successorParent = successorParent.left

        current.data = successor.data

        if successor.right != NULL
            successorParent.left = successor.right
        else
            successorParent.left = NULL

        delete successor

    return true
end function


Find one airport code for each deletion case and test it:

A leaf node (D1).
A node with only a left child.
A node with only a right child (D2).
A node with two children (D3).
The root node.

Print the level order of the top three levels before and after each deletion.

3.3 Print Level Order
text
function printLevelOrder()
    if root = NULL
        print "Tree is empty"
        return

    queue = [root]
    index = 0
    level = 0

    while index < queue.size() AND level < 3
        levelSize = queue.size() - index

        for i = 0 to levelSize - 1
            current = queue[index]

            print current.data.iata

            if current.left != NULL
                queue.push_back(current.left)

            if current.right != NULL
                queue.push_back(current.right)

            index++

        level++
end function

3.4 Test Deletion
text
function testDeletion(code)
    print "Before deletion"
    printLevelOrder()

    remove(code)

    print "After deletion"
    printLevelOrder()
end function


Also add a made-up airport with the IATA code QQQ, using your own airport name and city. Demonstrate that the airport can be found using search and then successfully removed.

Task 4: Traversals and Validation

The list command prints every airport in alphabetical IATA code order using inorder traversal. Check that the resulting codes are sorted.

4.1 Inorder Traversal
text
function inorder(Node* node)
    if node = NULL
        return

    inorder(node.left)
    print node.data.iata
    inorder(node.right)
end function

4.2 Check That the Codes Are Sorted
text
function checkSorted(codes)
    for i = 1 to codes.size() - 1
        if codes[i] < codes[i - 1]
            return false

    return true
end function

4.3 Save and Load the Tree

The save FILE command writes the airport codes in preorder. The load FILE command rebuilds the tree from the saved file.

Show that the reloaded tree has the same shape by comparing its height and level order.

Preorder traversal:

text
function preorder(Node* node, file)
    if node = NULL
        return

    write node.data to file

    preorder(node.left, file)
    preorder(node.right, file)
end function


Save the tree:

text
function saveTree(filename)
    file = open(filename, write)

    if file = NULL
        return false

    preorder(root, file)

    close(file)
    return true
end function


Load the tree:

text
function loadTree(filename)
    file = open(filename, read)

    if file = NULL
        return false

    clearTree(root)

    while file has data
        read airport record
        insert(airport)

    close(file)
    return true
end function

4.4 Compare Two Trees
text
function compareTrees(Node* first, Node* second)
    if first = NULL AND second = NULL
        return true

    if first = NULL OR second = NULL
        return false

    if first.data.iata != second.data.iata
        return false

    leftSame = compareTrees(first.left, second.left)
    rightSame = compareTrees(first.right, second.right)

    return leftSame AND rightSame
end function

4.5 Validate the BST

The validate command checks the BST property using the range (lo, hi) method. Validation should be performed after every insert and delete in the tests.

text
function validate(Node* node, string lo, string hi)
    if node = NULL
        return true

    if node.data.iata <= lo OR node.data.iata >= hi
        return false

    if validate(node.left, lo, node.data.iata) = false
        return false

    if validate(node.right, node.data.iata, hi) = false
        return false

    return true
end function


Validate the tree:

text
function validateTree()
    return validate(root, "", "ZZZ")
end function

Task 5: Range Queries

The range FROM TO command lists every airport with an IATA code between FROM and TO, inclusive.

The algorithm avoids visiting subtrees that cannot contain codes within the requested range. It also reports the number of nodes visited.

For example, range BOS BWI should return 113 airports.

5.1 Range Search
text
function rangeSearch(Node* node, string from, string to, int& visited)
    if node = NULL
        return

    visited++

    if node.data.iata > from
        rangeSearch(node.left, from, to, visited)

    if node.data.iata >= from AND node.data.iata <= to
        print node.data.iata

    if node.data.iata < to
        rangeSearch(node.right, from, to, visited)
end function

5.2 Execute a Range Query
text
function range(from, to)
    if from > to
        print "Invalid range"
        return

    visited = 0

    rangeSearch(root, from, to, visited)

    print visited
end function

Task 6: The Shape Experiment

Build the BST in different ways using the same 6,072 airport codes:

File order.
Sorted order.
Balanced build from the sorted array, using the middle element as the root.
Random shuffle (bonus).

For each build method, measure the tree height, average node depth, and time required for 10,000 random searches.

6.1 Build a Tree Using Sorted Order
text
function buildSortedTree(airports)
    sortedAirports = copy(airports)

    sort(sortedAirports by iata)

    tree = new AirportBST()

    for i = 0 to sortedAirports.size() - 1
        insert(tree, sortedAirports[i])

    return tree
end function

6.2 Build a Balanced Tree

Use the middle element of the sorted array as the root. Recursively build the left and right subtrees using the elements on either side of the middle.

text
function buildBalanced(sortedAirports, start, end)
    if start > end
        return NULL

    mid = (start + end) / 2

    node = new Node(sortedAirports[mid])

    node.left = buildBalanced(sortedAirports, start, mid - 1)
    node.right = buildBalanced(sortedAirports, mid + 1, end)

    return node
end function

6.3 Build a Tree Using Random Order
text
function buildRandomTree(airports)
    shuffledAirports = copy(airports)

    shuffle(shuffledAirports)

    tree = new AirportBST()

    for i = 0 to shuffledAirports.size() - 1
        insert(tree, shuffledAirports[i])

    return tree
end function

6.4 Time 10,000 Random Searches
text
function timeSearches(tree, airports)
    total = 10000

    indexes = generateRandomIndexes(airports.size(), total)

    startTime = currentTime()

    for i = 0 to total - 1
        code = airports[indexes[i]].iata
        search(tree, code)

    endTime = currentTime()

    elapsedTime = endTime - startTime

    return elapsedTime
end function
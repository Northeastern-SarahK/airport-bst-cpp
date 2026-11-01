# Airport Binary Search Tree – Pseudocode

## Task 1: Load the Data and Build the Tree

Read airports.dat line by line, split each line into fields, and account for airport names that may contain commas inside quotation marks. Skip records whose IATA code is \N and store the remaining records in an array or vector of Airport.

### 1.1 Split a Line into Fields

```text
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
```

### 1.2 Load Airport Records

```text
function loadAirports(lines, airports, skipped)
    skipped = 0

    for i = 0 to lines.size() - 1
        fields = splitLine(lines[i])

        if fields.size() < 8
            continue

        if fields[4] = "\N"
            skipped++
            continue

        Airport a
        a.id = fields[0]
        a.name = fields[1]
        a.city = fields[2]
        a.country = fields[3]
        a.iata = fields[4]
        a.icao = fields[5]
        a.latitude = fields[6]
        a.longitude = fields[7]

        airports.push_back(a)
end function
```

### 1.3 Insert Records and Build the BST

Build the BST by inserting the airport records in their original file order. Count the number of successfully inserted keys and report duplicates according to rule I4. Insert uses a loop, not recursion, so the 6,072-key sorted build cannot overflow the call stack.

```text
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
```

Build the tree:

```text
function insertAll(airports, tree)
    inserted = 0
    duplicates = 0

    for i = 0 to airports.size() - 1
        if insert(tree, airports[i]) = true
            inserted++
        else
            duplicates++
            print "Duplicate: " + airports[i].iata + " (rule I4)"

    return inserted, duplicates
end function
```

### 1.4 Calculate Tree Height and Average Depth

Both use a queue or stack instead of recursion, because the sorted-order tree is 6,071 levels deep.

Calculate height by counting levels in level order:

```text
function height()
    if root = NULL
        return -1

    queue = [root]
    index = 0
    height = -1

    while index < queue.size()
        levelSize = queue.size() - index

        for i = 0 to levelSize - 1
            current = queue[index]

            if current.left != NULL
                queue.push_back(current.left)

            if current.right != NULL
                queue.push_back(current.right)

            index++

        height++

    return height
end function
```

Calculate the depth of each node with a stack of (node, depth) pairs:

```text
function calculateDepth(totalDepth, count)
    totalDepth = 0
    count = 0

    if root = NULL
        return

    nodeStack = [root]
    depthStack = [0]

    while nodeStack is not empty
        current = nodeStack.pop()
        depth = depthStack.pop()

        totalDepth = totalDepth + depth
        count++

        if current.right != NULL
            nodeStack.push(current.right)
            depthStack.push(depth + 1)

        if current.left != NULL
            nodeStack.push(current.left)
            depthStack.push(depth + 1)
end function
```

Calculate average depth:

```text
function averageDepth()
    totalDepth = 0
    count = 0

    calculateDepth(totalDepth, count)

    if count = 0
        return 0

    return totalDepth / count
end function
```

## Task 2: Search

The find CODE command searches for an airport by its IATA code. It prints the airport information, the search path (every code compared), and the total number of comparisons.

### 2.1 Search for an Airport

```text
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

    print path + " -> NULL"
    print comparisons
    print code + " not found"

    return false
end function
```

Test the following search cases:

- Search for the root.
- Search for a leaf.
- Search for a code deep in the tree.
- Search for a code that does not exist (for example, XYZ).
- Search for a code smaller than every key.
- Search for a code larger than every key.

Also print the minimum and maximum airport codes.

### 2.2 Find the Minimum and Maximum Codes

Find minimum:

```text
function findMin()
    if root = NULL
        return NULL

    current = root

    while current.left != NULL
        current = current.left

    return current.data.iata
end function
```

Find maximum:

```text
function findMax()
    if root = NULL
        return NULL

    current = root

    while current.right != NULL
        current = current.right

    return current.data.iata
end function
```

### 2.3 Find a Leaf Node

```text
function findLeafCode(Node* node)
    if node = NULL
        return ""

    if node.left = NULL AND node.right = NULL
        return node.data.iata

    code = findLeafCode(node.left)

    if code != ""
        return code

    return findLeafCode(node.right)
end function
```

### 2.4 Find the Deepest Node

```text
function findDeepest(Node* node, depth, maxDepth, code)
    if node = NULL
        return

    if depth > maxDepth
        maxDepth = depth
        code = node.data.iata

    findDeepest(node.left, depth + 1, maxDepth, code)
    findDeepest(node.right, depth + 1, maxDepth, code)
end function
```

Get the deepest code:

```text
function getDeepestCode()
    maxDepth = -1
    code = ""

    findDeepest(root, 0, maxDepth, code)

    return code
end function
```

## Task 3: Delete

The remove CODE command deletes an airport from the BST. The deletion algorithm handles the three standard BST deletion cases:

- A node with no children (leaf), case D1.
- A node with one child, case D2.
- A node with two children, case D3.

### 3.1 Remove an Airport

In case D3 the inorder successor is the smallest code in the right subtree, so the search for it starts at current.right and goes left. The successor never has a left child, so its right child moves up into its place.

```text
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
        print code + " not found"
        return false

    // Case D1: Node has no children
    if current.left = NULL AND current.right = NULL
        if current = root
            root = NULL
        else if parent.left = current
            parent.left = NULL
        else
            parent.right = NULL

        delete current
        print "Deleted " + code + " (case D1: leaf)"

    // Case D2: Node has one child
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

        print "Deleted " + code + " (case D2: only a left/right child, " + child.data.iata + ")"
        delete current

    // Case D3: Node has two children
    else
        successorParent = current
        successor = current.right

        while successor.left != NULL
            successorParent = successor
            successor = successor.left

        current.data = successor.data

        if successorParent = current
            successorParent.right = successor.right
        else
            successorParent.left = successor.right

        delete successor
        print "Deleted " + code + " (case D3: two children, replaced by successor)"

    return true
end function
```

### 3.2 Find a Code for Each Deletion Case

Find one airport code for each deletion case and test it:

- A leaf node (D1), using findLeafCode from 2.3.
- A node with only a left child.
- A node with only a right child (D2).
- A node with two children (D3).
- The root node.

```text
function findOnlyLeftChild(Node* node)
    if node = NULL
        return ""

    if node.left != NULL AND node.right = NULL
        return node.data.iata

    code = findOnlyLeftChild(node.left)

    if code != ""
        return code

    return findOnlyLeftChild(node.right)
end function
```

findOnlyRightChild is the same with the test changed to `node.left = NULL AND node.right != NULL`.

findTwoChildren is the same with the test changed to `node != root AND node.left != NULL AND node.right != NULL`. The root is skipped here because it is tested as its own case.

Print the level order of the top three levels before and after each deletion.

### 3.3 Print Level Order

```text
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
```

### 3.4 Test Deletion

The BST property is validated after every delete (rule from Task 4c).

```text
function testDeletion(code)
    print "Before deletion"
    printLevelOrder()

    remove(code)
    print validateTree()

    print "After deletion"
    printLevelOrder()
end function
```

Also add a made-up airport with the IATA code QQQ, using your own airport name and city. Insert it, validate, show that it can be found using search, then remove it, validate again, and search once more to show it is gone.

## Task 4: Traversals and Validation

The list command prints every airport in alphabetical IATA code order using inorder traversal. Check that the resulting codes are sorted.

### 4.1 Inorder Traversal

```text
function inorder(Node* node, result)
    if node = NULL
        return

    inorder(node.left, result)
    result.push_back(node.data)
    inorder(node.right, result)
end function
```

List every airport:

```text
function printList()
    sorted = []
    inorder(root, sorted)

    for i = 0 to sorted.size() - 1
        print sorted[i].iata, sorted[i].name, sorted[i].city, sorted[i].country

    print sorted.size() + " airports listed"
end function
```

### 4.2 Check That the Codes Are Sorted

```text
function checkSorted(codes)
    for i = 1 to codes.size() - 1
        if codes[i] < codes[i - 1]
            return false

    return true
end function
```

### 4.3 Save and Load the Tree

The save FILE command writes the airports in preorder. The load FILE command rebuilds the tree from the saved file. Each saved line uses the same column order as airports.dat, so the same loader reads both files.

Show that the reloaded tree has the same shape by comparing its height and level order.

Preorder traversal:

```text
function preorder(Node* node, file)
    if node = NULL
        return

    write node.data to file as one line: id, name, city, country, iata, icao, latitude, longitude

    preorder(node.left, file)
    preorder(node.right, file)
end function
```

Save the tree:

```text
function saveTree(filename)
    file = open(filename, write)

    if file = NULL
        return false

    preorder(root, file)

    close(file)
    return true
end function
```

Clear the tree (with a stack, because the sorted-order tree is too deep for recursion):

```text
function clearTree()
    if root = NULL
        return

    stack = [root]

    while stack is not empty
        current = stack.pop()

        if current.left != NULL
            stack.push(current.left)

        if current.right != NULL
            stack.push(current.right)

        delete current

    root = NULL
end function
```

Load the tree:

```text
function loadTree(filename)
    lines = readFile(filename)

    if lines could not be read
        return false

    airports = []
    loadAirports(lines, airports, skipped)

    clearTree()
    insertAll(airports, tree)

    return true
end function
```

Because the file is in preorder, every parent is inserted before its children, so each code ends up in the same position and the tree has the same shape.

### 4.4 Compare Two Trees

```text
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
```

### 4.5 Validate the BST

The validate command checks the BST property using the range (lo, hi) method. Validation is performed after every insert and delete in the tests.

```text
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
```

Validate the tree. The upper bound is "ZZZZ" because it sorts after every three-letter code, including ZZZ itself:

```text
function validateTree()
    return validate(root, "", "ZZZZ")
end function
```

## Task 5: Range Queries

The range FROM TO command lists every airport with an IATA code between FROM and TO, inclusive.

The algorithm avoids visiting subtrees that cannot contain codes within the requested range. It reports the number of airports found and the number of nodes visited.

For example, range BOS BWI should return 113 airports.

### 5.1 Range Search

```text
function rangeSearch(Node* node, string from, string to, int& visited, int& found)
    if node = NULL
        return

    visited++

    if node.data.iata > from
        rangeSearch(node.left, from, to, visited, found)

    if node.data.iata >= from AND node.data.iata <= to
        found++
        print node.data.iata

    if node.data.iata < to
        rangeSearch(node.right, from, to, visited, found)
end function
```

### 5.2 Execute a Range Query

```text
function range(from, to)
    if from > to
        print "Invalid range"
        return

    visited = 0
    found = 0

    rangeSearch(root, from, to, visited, found)

    print found + " airports, " + visited + " nodes visited"
end function
```

## Task 6: The Shape Experiment

Build the BST in different ways using the same 6,072 airport codes:

- File order.
- Sorted order.
- Balanced build from the sorted array, using the middle element as the root.
- Random shuffle (bonus).

For each build method, measure the tree height, average node depth, and time required for 10,000 random searches.

### 6.1 Build a Tree Using Sorted Order

The inorder traversal of the file-order tree already gives the airports sorted by code, so no separate sort is needed.

```text
function buildSortedTree(fileOrderTree)
    sortedAirports = []
    inorder(fileOrderTree.root, sortedAirports)

    tree = new AirportBST()

    for i = 0 to sortedAirports.size() - 1
        insert(tree, sortedAirports[i])

    return tree
end function
```

### 6.2 Build a Balanced Tree

Use the middle element of the sorted array as the root. Recursively build the left and right subtrees using the elements on either side of the middle.

```text
function buildBalanced(sortedAirports, start, end)
    if start > end
        return NULL

    mid = (start + end) / 2

    node = new Node(sortedAirports[mid])

    node.left = buildBalanced(sortedAirports, start, mid - 1)
    node.right = buildBalanced(sortedAirports, mid + 1, end)

    return node
end function
```

The rebalance command rebuilds the current tree this way:

```text
function rebalance()
    sortedAirports = []
    inorder(root, sortedAirports)

    clearTree()

    root = buildBalanced(sortedAirports, 0, sortedAirports.size() - 1)
end function
```

### 6.3 Build a Tree Using Random Order

The shuffle is the Fisher-Yates method with a fixed seed, so the same run gives the same tree every time.

```text
function shuffleAirports(airports)
    n = airports.size()

    for i = 0 to n - 2
        j = i + random() % (n - i)
        swap airports[i] and airports[j]
end function
```

```text
function buildRandomTree(airports)
    shuffledAirports = copy(airports)

    setSeed(2026)
    shuffleAirports(shuffledAirports)

    tree = new AirportBST()

    for i = 0 to shuffledAirports.size() - 1
        insert(tree, shuffledAirports[i])

    return tree
end function
```

### 6.4 Time 10,000 Random Searches

The searches count how many codes were found. Using the result stops an optimizing compiler (-O2) from removing the search loop.

```text
function timeSearches(tree, airports)
    total = 10000

    setSeed(42)
    indexes = generateRandomIndexes(airports.size(), total)

    found = 0
    startTime = currentTime()

    for i = 0 to total - 1
        code = airports[indexes[i]].iata

        if searchQuiet(tree, code) = true
            found++

    endTime = currentTime()

    elapsedTime = endTime - startTime

    return elapsedTime, found
end function
```

# airport-bst-cpp

Mini Project 02 for EECE 2560: an airport code lookup service built on a binary search tree.

## The Problem

Every flight booking, boarding pass and baggage tag identifies an airport by its three-letter IATA code (BOS, JFK, DAC). A travel company needs a fast lookup service that takes a code and returns the airport's name, city, country and location. The service must stay correct while airports are added and removed. Agents also need alphabetical listings and range queries such as "every code from BOS to BWI".

This program stores the airports in a binary search tree keyed by IATA code. It supports insert, search, delete, traversals, validation and range queries. It also measures how the insertion order changes the shape of the tree.

## Repository Layout

```
src/main.cpp          Airport and AirportBST classes, task tests, command mode
data/airports.dat     OpenFlights airport database (course copy)
docs/pseudocode.md    pseudocode for build, insert, search, delete, validate, range, balanced build
docs/demo.mp4         screen recording of a session
```

## Build and Run

From the repository folder:

```
g++ -std=c++17 -O2 src/*.cpp -o airports
./airports
```

On Windows PowerShell, run `.\airports.exe` instead. Run `chcp 65001` first so accented airport names print correctly.

The program looks for the data file at `data/airports.dat`, then `airports.dat`, then the same two paths one folder up, so it works whether it is started from the repository folder or from `src/`. A path can also be given directly, for example `./airports path/to/airports.dat`. If the file is still not found, the program asks for its path.

The menu runs the tests for each task (1 to 6). Option 7 opens command mode, which works like the sample session in the handout:

| Command | What it does |
|---|---|
| `load FILE` | Rebuild the tree from airports.dat or from a file written by `save` |
| `find CODE` | Print the search path, the number of comparisons and the airport |
| `add CODE` | Add an airport (asks for name, city and country), then validate |
| `remove CODE` | Delete an airport (case D1, D2 or D3), then validate |
| `list` | Print every airport in code order (inorder) |
| `save FILE` | Write the airports in preorder |
| `validate` | Check the BST property with the (lo, hi) method |
| `range FROM TO` | Print every airport from FROM to TO, with the number of nodes visited |
| `rebalance` | Rebuild the tree as a balanced tree (middle element as the root) |
| `levels` | Print the top three levels |
| `stats` | Print the size, height, average depth, minimum and maximum code |
| `quit` | Go back to the menu |

## Sample Session

```
> load data/airports.dat
Loaded 6072 airports (1626 lines skipped: no IATA code). Duplicates: 0
Height: 177 Average depth: 18.3233
> find BOS
Path: GKA -> AEY -> EGS -> BUJ -> BJA -> BSK -> BOY -> BNI -> BOH -> BOO -> BOU -> BOS
Comparisons: 12
BOS General Edward Lawrence Logan International Airport, Boston, United States
> find XYZ
Path: GKA -> MAG -> POM -> WWK -> YAM -> XFW -> XGN -> XLS -> XRY -> XVS -> YAI -> YAA -> XYA -> XYE -> NULL
Comparisons: 14
XYZ not found
> range BOS BWI
BOS General Edward Lawrence Logan International Airport, Boston, United States
BOU Bourges Airport, Bourges, France
...
BWI Baltimore/Washington International Thurgood Marshall Airport, Baltimore, United States
113 airports, 127 nodes visited
> remove AEY
Deleted AEY (case D3: two children, replaced by successor AFA)
validate: OK
> remove BOS
Deleted BOS (case D2: only a left child, BOR)
validate: OK
> rebalance
Height: 12 Average depth: 10.6527
> quit
```

## Task 6: The Shape Experiment

All four trees hold the same 6,072 codes. Each time is for the same 10,000 random searches (seed 42). All 10,000 codes were found in every tree. Times come from the `-O2` build above and change a little from run to run and from machine to machine.

| Build Order | Height | Average depth | Time for 10,000 searches | Your explanation |
|---|---|---|---|---|
| File order | 177 | 18.3233 | 4.5736 ms | The original file order produces a somewhat unbalanced tree because the airport records are not randomly ordered. |
| Sorted order | 6071 | 3035.5 | 206.664 ms | Inserting sorted codes creates a chain-like tree because each new code is greater than the previous one. |
| Balanced (middle rule) | 12 | 10.6532 | 2.7165 ms | Choosing the middle element as the root creates a balanced tree with shorter search paths. |
| Random shuffle (bonus) | 28 | 13.9848 | 2.9608 ms | Randomizing the insertion order creates a relatively balanced tree, reducing its height and improving search performance. |

The random shuffle uses `srand(2026)` with a Fisher-Yates shuffle. The exact height depends on the compiler's `rand()`, but it stays in the expected 25 to 29 range.

**Why is file order so much worse than a random shuffle?**

The file is ordered by airport ID, not by code. Airports from the same country tend to sit together, and their codes often appear in increasing order. The longest such stretch is 162 Canadian airports in a row, from YDA to ZFM, each with a larger code than the one before. Inserting an increasing run puts each key to the right of the previous one, which builds a chain just like the sorted-order case. That chain is the deepest path in the tree: 155 of the 178 nodes on the path to YZR come from this one run. A random shuffle breaks these runs apart, so no long chain forms and the height drops to about 25 to 29.

Every BST operation is O(h), so the height in this table is what decides how fast find, insert and remove are.

## Team

This project was completed as a team by Natan Blumberg, Hamza Ghosheh, and Sarah Koesema.

## Data Credit

Airport data from OpenFlights.org (`airports.dat`, https://openflights.org/data.php), published under the Open Database License.

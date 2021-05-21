# CS50 Design review
## Team of 4: nunchuck-buccaneers

Speakers: James, William, Alan, Eric

* nice idea for overlaying grids
* rethink some datastructures == may be too complicated
* lots of modules - are these all separate .c files? 
	* managing the interface between modules could end up being a lot of code
	* are any of these shared between client and server?
* pseudocode is nicely thorough
* think about how to look up players - and what data structure
* can't (easily) key a hashtable by addr_t or x,y coordinates
* division of work: you have listed tasks, but need to be more clear about who will do what.


### Total (45/50)

#### Presentation (11/12 points)

"In 10 minutes you must present an *overview* of your design decisions and how your group will divide the work; the design document provides more detail. Be prepared to answer questions and receive feedback."

* (4/4) Good overview of the design.
* (4/4) Decomposition and data structures.
* (3/4) Plan for division of the work.

#### Document (34/38 points)

"Your design document (written in Markdown) shall describe the major design decisions, plan for testing, and the roles of each member of your group."

**Client (11):**

* (2/2) User interface
* (1/2) Inputs and outputs
	* (interwoven with server I/O)
* (4/4) Functional decomposition into functions/modules
* (2/2) Major data structures (not mentioned, but there are none)
	* none shown for the client
* (2/2) High-level pseudo code (plain English-like language) for logic/algorithmic flow
* (0/2) Testing plan, including unit tests, integration tests, system tests
	* no client tests mentioned in the testing plan

**Server (24):**

* (4/4) User interface
* (4/4) Inputs and outputs
* (4/4) Functional decomposition into functions/modules
* (4/4) Major data structures
	* but some may be overcomplicated
	* and hashtables may not be suitable
* (4/4) High-level pseudo code (plain English-like language) for logic/algorithmic flow
* (3/4) Testing plan, including unit tests, integration tests, system tests
	* a bit more detail would have been helpful
* (+1) nice idea for overlaying grids

**overall:**
* (-1) wrong branch name

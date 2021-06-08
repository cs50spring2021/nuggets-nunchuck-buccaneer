## CS50 Code style and quality

Grade: 13/15

Summary:  The following feedback is meant to highlight some of my general observations; look for specific comments with keyword "STYLE" or "GRADER" in your Makefile and source-code files.

* compiles smoothly
* README ok
* Overall impression: it feels like you "overdid it" when modularizing the code, breaking it up into so many files and modules that it actually *increased* the amount of code by requiring more getters and setters and 'glue' to hold the pieces together. Result is more complexity than needed.
* As an extreme example, `pos2D_t` adds a ton of complexity and o real value over storing two ints, or passing two ints to a function call.
* handleMessage code is far too complex, and also should be broken up by calling helper functions.
* Define named constants for all the grid characters (like walls, spots, and gold) rather than sprinkling code with character constants.  A named constant will make the code more readable, and more adaptable if there is a later need to change characters.
* Do the same for some of the other constants you use, like the '0'..'2' codes you use in a "sight grid".
* some of the code feels 'tight'; use blank lines more often, to separate 'paragraphs' of code.
*   Try to keep code lines less than 80 characters wide, so the code reads well in a standard 80-column window.  Set the indentation step size at 2 characters (or at most 4 characters) to keep the code horizontally compact.  Use spaces rather than tabs so the code views the same way in all viewers/editors.  Wrap lines at readable points rather than letting the viewer/editor wrap arbitrarily. Restructure code if needed.  Move in-line comments to the prior line if needed.


Recall the *[CS50 coding standards](https://www.cs.dartmouth.edu/~cs50/Labs/CodingStyle.html)*.

Recall the [style rubric](https://github.com/cs50spring2021/nuggets-info/blob/main/rubric.md#code-style-and-quality-15), which referred to the following general expectations:

* code should be well-organized with sensible filenames and subdirectories
* brief `README.md` in each directory
* clear Makefile for clean and build
* clear and consistent style
* clear code design, functional decomposition, naming
* clear code logic and flow
* at most one global variable in each program
* good choice of, and use of, data structures
* good modularity (strong cohesion, loose coupling)
* good in-code documentation (comments)
* code is consistent with documentation (IMPLEMENTATION.md)
* defensive programming (error checking, malloc, etc.)
* no compiler warnings
* no memory leaks/errors caused by student code
* no output to stdout other than what is required to play the game
* handles errors gracefully

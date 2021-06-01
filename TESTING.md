## Testing

For testing we are ignoring memory leaks within the testing and are only worried about mem
leaks in the actual program

To run module tests
    First make sure that in BOTH common and main makefiles
        `DEFINES = -DTESTING` IS a comment with a hashtag!
    Then run
        `make testModuleAndMains &> moduletest.out`
    This will create the moduletest.out output for the mod test

To run server and network test
    First make sure that in BOTH common and main makefiles
        `DEFINES = -DTESTING` is NOT a comment there should not be a hashtag!
    Then run
        `make testCmdsAndNetwork &> cmdsAndNetworkTest.out`
    This will create the cmdsAndNetworkTest.out output for the server and network test

To run Client test
    First make sure that in BOTH common and main makefiles
        `DEFINES = -DTESTING` IS a comment with a hashtag!
    Then run
        `make testClient 2> clienttest.out`
    Press x to exit the test program
    Press c to move forward a step in testing
        It will exit after a couple stages of c
    If you get an ensure dimensions message make sure to resize the window to the correct size before pressing c
    This will create the clienttest.out output for the server and network test

There should be 3 .out's we have submitted all of them
clienttest.out
cmdsAndNetworkTest.out
moduletest.out

/*
 * network.h - header file for the network module
 * 
 * *Network* - runs message.h and handles messages 
 * and timeouts, parses them and translates them 
 * to functions which must be called on the player.
 * 
 * Nunchuck Buccaneers, May 2021
 */

/******************** function prototypes ********************/

/********************* startNetworkServer ********************/
/*
 * startNetworkServer - handles starting the network on the
 * server side.
 * 
 * Inputs:
 *     * gameInfo_t* gameInfo - gameInfo from the server
 * Output:
 *     * returns nothing; will exit non-zero if there is an
 *       error starting the server.
 *     * start the message loop for the server and set the 
 *       address to be accessed.
 *     * message loop can then access gameInfo from the server.
 */
void startNetworkServer(gameInfo_t* gameInfo);

/********************** startNetworkClient *******************/
/*
 * startNetworkClient - handles starting the network on the
 * client side.
 * 
 * Inputs:
 *     * serverHost - IP address to connect client to server
 *     * port - string holding port number to the server
 * Output:
 *     * returns nothing; will exit non-zero if there is an
 *       error connecting to the server.
 *     * calls message_setAddr on the parameters to set
 *       address to server.
 *     * start message loop for the client
 */
void startNetworkClient(char* serverHost, char* port);

/* ***************** numWords() ********************** */
/*
 * numWords - This function counts the number of words in the message.
 *
 * Inputs:
 *     * char* string - a message sent either from client to server or server
 *                      to client.
 * 
 * Outputs: 
 *     * int numWords - the number of words that were in the message
 */
 
/********************* tokenizeMessage() *********************/
/*
 * tokenizeMessage - parses the message sent from server to client or from
 * client to server. It breaks each individual word of the message a part and
 * inserts it into an array.
 * 
 * Inputs:
 *     * message - char* containing the keystrokes sent from the
 *       client to the server.
 *     * numWords - int the number of separate words found in a message.
 * Output:
 *     * char** tokens - char** pointing to tokens from the original
 *       message string passed to the server.
 */
char** tokenizeMessage(const char* message, int numWords);

/********************** handleMessage() **********************/
/*
 * handleMessage - handles the message passed from client to server
 * or server to client.
 * 
 * Inputs:
 *     * void* arg - anything we want to pass through, i.e., gameInfo
 *       struct --> allows access to gameInfo for either side if needed.
 *     * addr_t from - address from which the message arrived.
 *     * char* message - string containing contents of message sent. This
 *       will be tokenized later.
 * Output:
 *     * boolean:
 *          * true to terminate looping - game quit or game over.
 *          * false to keep looping.
 */
bool handleMessage(void* arg, const addr_t from, const char* message);

/********************** handleTimeout() **********************/
/*
 * handleTimeout - called when time passes without input or message.
 *
 * Inputs:
 *     * void* arg - anything we want to pass through, i.e., gameInfo
 *       struct --> allows access to gameInfo for either side if needed.
 * Output:
 *     * boolean:
 *          * true to terminate looping - game quit or game over.
 *          * false to keep looping.
 */
bool handleTimeout(void* arg);

/*********************** handleInput() ************************/
/*
 * handleInput - should read once from stdin and process it.
 *
 * Inputs:
 *     * void* arg - anything we want to pass through, i.e., gameInfo
 *       struct --> allows access to gameInfo for either side if needed.
 *       could also (more likely) be the server address, but void* arg 
 *       allows for flexibility in that regard.
 * Output:
 *     * boolean:
 *          * true to terminate looping - game quit or game over.
 *          * false to keep looping.
 */
bool handleInput(void* arg);

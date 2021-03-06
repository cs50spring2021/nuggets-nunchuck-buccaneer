/*
 * network.h - header file for the network module
 * 
 * *Network* - runs message.h and handles messages 
 * and timeouts, parses them and translates them 
 * to functions which must be called on the player.
 * 
 * Nunchuck Buccaneers, May 2021
 */

#include "gameInfo.h"
#include "message.h"


/******************** function prototypes ********************/

/********************* startNetworkServer ********************/
/*
 * startNetworkServer - starts the network on the server side, initalizing the
 * message module, running the message loop, and terminating the msg module.
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
void startNetworkServer(gameInfo_t* gameInfo, FILE* errorFile);

/********************** startNetworkClient *******************/
/*
 * startNetworkClient - starts the network on the client side, initalizing the
 * message module, setting the server address, sending an initial join message,
 * running the message loop, and terminating the msg module.
 * 
 * Inputs:
 *     * serverHost - IP address to connect client to server
 *     * port - string holding port number to the server
 *     * errorFile - file to write errors to
 *     * name - the name for the client
 * Output:
 *     * returns nothing; will exit non-zero if there is an
 *       error connecting to the server.
 *     * calls message_setAddr on the parameters to set
 *       address to server.
 *     * start message loop for the client
 */
void startNetworkClient(char* serverHost, int* port, FILE* errorFile, char* name);

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
 int numWords(char* message);
 
/********************* tokenizeMessage() *********************/
/*
 * tokenizeMessage - parses the message sent from server to client or from
 * client to server. It breaks each individual word of the message a part and
 * inserts it into an array.
 * 
 * Inputs:
 *     * message - char* containing the keystrokes sent from the
 *       client to the server.
 * Output:
 *     * char** tokens - char** pointing to tokens from the original
 *       message string passed to the server.
 * Caller is responsible for:
 *       freeing tokens array
 */
char** tokenizeMessage(char* message);

/********************** handleMessage() **********************/
/*
 * handleMessage - handles the message passed from client to server
 * or server to client. This function checks the first word of every message to
 * determine what steps to take next (typically calls other functions).
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

/*********************** handleInput() ************************/
/*
 * handleInput - should read once from stdin and process it.
 *               Also compares input to an array of valid key strokes to see if
 *               it is an acceptable key stroke to send.
 *
 * Inputs:
 *     * void* arg - anything we want to pass through, in this case we will
 *                   be passing in the server address.
 * Output:
 *     * boolean:
 *          * true to terminate looping - game quit or game over.
 *          * false to keep looping.
 */
bool handleInput(void* arg);
